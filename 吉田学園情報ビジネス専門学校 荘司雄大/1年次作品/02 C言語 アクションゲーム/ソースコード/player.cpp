//=============================================================================
//
// �v���C���[�̕\������ [player.cpp]
// Author : Takahiro Syoji
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "life.h"
#include "bullet.h"
#include "block.h"
#include "item.h"
#include "score.h"
#include "enemy.h"
#include "game.h"
#include "particle.h"
//#include "effect.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POLYGON_TEXTURENAME		"data/TEXTURE/player0000.png"

#define POLYGON_TEXTURE_UV_U		(0.090909090909f)		//U
#define POLYGON_TEXTURE_UV_V		(0.5f)		//V

#define PLAYER_LIFE			 (5)								//�v���C���[�̗̑�

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;

Player g_player;
Block g_block;
Item g_Item;
int g_BulletRecast = 30;

int g_nCounterAnim;
int g_nPatternAnim;

//=============================================================================
// ���C���֐�
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_player.state = PLAYERSTATE_NORMAL;
		g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.nType = 0;
		g_player.fHeight = 80;
		g_player.fWidth = 30;
		g_player.nCounterState = 60;
		g_player.state;
		g_player.nLife = PLAYER_LIFE;
		g_player.DirectionMove = 0;
		g_player.BulletKind = BULLETKIND_NOMAL;
		g_player.bJump = false;
		g_player.bUse = true;
	}

	g_nCounterAnim = 0;
	g_nPatternAnim = 0;

	g_player.pos = D3DXVECTOR3(100, 50,0.0f);
	//g_movePlayer = D3DXVECTOR3(1, 1, 0.0f);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME, &g_pTexturePlayer);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
	//���_���W
	pVtx[0].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y - (g_player.fHeight), g_player.pos.z);
	pVtx[1].pos = D3DXVECTOR3(g_player.pos.x + g_player.fWidth, g_player.pos.y - (g_player.fHeight), g_player.pos.z);
	pVtx[2].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y, g_player.pos.z);
	pVtx[3].pos = D3DXVECTOR3(g_player.pos.x + g_player.fWidth, g_player.pos.y, g_player.pos.z);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(g_nPatternAnim * 0.25f, 1.0f);
	pVtx[1].tex = D3DXVECTOR2(g_nPatternAnim * 0.25f + 0.25f, 1.0f);
	pVtx[2].tex = D3DXVECTOR2(g_nPatternAnim * 0.25f, 0.5f + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(g_nPatternAnim * 0.25f + 0.25f, 0.5f + 1.0f);
	
	//�A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

void UninitPlayer(void)
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
	//���_�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;
	g_nCounterAnim++;
	g_player.posOld = g_player.pos;		//�O��̈ʒu��ۑ�
	g_player.pos += g_player.move;

	g_player.pos.x = 500;
	
	//g_player.pos.y = 500;

	if (g_player.bUse == true)		//�v���C���[���g�p����Ă���
	{
		switch (g_player.state)
		{
		case PLAYERSTATE_NORMAL:
			break;

		case PLAYERSTATE_DAMAGE:
			g_player.nCounterState--;
			if (g_player.nCounterState <= 0)
			{//�F��߂�
				g_player.state = PLAYERSTATE_NORMAL;

				VERTEX_2D *pVtx;
				g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				g_pVtxBuffPlayer->Unlock();
			}
			break;
		case PLAYERSTATE_DEATH:
			g_player.nCounterState--;
			if (g_player.nCounterState <= 0)
			{
				g_player.nCounterState = 60;
				StopSound(SOUND_LABEL_BGM001);
				SetFade(MODE_RESULT);
			}
			return;
			break;
		}
	}
	//���b�N
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	if (g_player.bUse == true)
	{
		//-------------------------���ړ�-------------------------
		if (GetKeyboardPress(DIK_A) == true || GetKeyboardPress(DIK_LEFT) == true)
		{
			g_player.move.x -= 1;

			//���ړ��̔��f
			g_player.DirectionMove = 1;

			if ((g_nCounterAnim % 3) == 0)
			{
				g_nPatternAnim = (g_nPatternAnim + 1) % 10;
			}
		}

		//-------------------------�E�ړ�-------------------------
		else if (GetKeyboardPress(DIK_D) == true || GetKeyboardPress(DIK_RIGHT) == true)
		{
			g_player.move.x += 1;

			//�E�ړ��̔��f
			g_player.DirectionMove = 0;

			if ((g_nCounterAnim % 3) == 0)
			{
				g_nPatternAnim = (g_nPatternAnim + 1) % 10;
			}
		}
		//-------------------------�ړ����Ȃ�-------------------------
		else
		{
			if ((g_nCounterAnim % 4) == 0 && g_nPatternAnim == 1 || g_nPatternAnim == 2 || g_nPatternAnim == 3 || g_nPatternAnim == 4 || g_nPatternAnim == 5 || g_nPatternAnim == 6 || g_nPatternAnim == 7 || g_nPatternAnim == 8 || g_nPatternAnim == 9 || g_nPatternAnim == 10 && g_player.bJump == false)
			{
				g_nPatternAnim = 10;
			}
		}
		//-------------------------�W�����v-------------------------
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			if (g_player.bJump == false)
			{
					PlaySound(SOUND_LABEL_SE_JUMP);
					g_player.bJump = true;
					g_player.move.y -= 7;
					pVtx[0].tex = D3DXVECTOR2(0.090909090909f * 5, g_player.DirectionMove * POLYGON_TEXTURE_UV_V);
					pVtx[1].tex = D3DXVECTOR2(0.090909090909f * 5 + 0.1f, g_player.DirectionMove * POLYGON_TEXTURE_UV_V);
					pVtx[2].tex = D3DXVECTOR2(0.090909090909f * 5, g_player.DirectionMove * POLYGON_TEXTURE_UV_V + POLYGON_TEXTURE_UV_V);
					pVtx[3].tex = D3DXVECTOR2(0.090909090909f * 5 + 0.1f, g_player.DirectionMove * POLYGON_TEXTURE_UV_V + POLYGON_TEXTURE_UV_V);
			}
		}

		g_BulletRecast++;

		//if (g_player.DirectionMove == 0)
		//{
		//	if (GetKeyboardTrigger(DIK_F) == true)
		//	{
		//		// �e�𔭎�
		//			switch (g_player.BulletKind)
		//			{
		//			case BULLETKIND_NOMAL:
		//				if (g_BulletRecast >= 10)
		//				{
		//					PlaySound(SOUND_LABEL_SE_SHOT);
		//					SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y - g_player.fHeight / 2, g_player.pos.z), D3DXVECTOR3(10.0f, 0.0f, 0.0f), 10.0f, 10.0f, BULLETTYPE_PLAYER);
		//					g_BulletRecast = 0;
		//				}
		//				break;

		//			case BULLETKIND_DIFFUSION:
		//				if (g_BulletRecast >= 30)
		//				{
		//					PlaySound(SOUND_LABEL_SE_SHOT1);
		//					//��
		//					SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y - g_player.fHeight / 2, g_player.pos.z), D3DXVECTOR3(10.0f, 0.5f, 0.0f), 10.0f, 10.0f, BULLETTYPE_PLAYER);

		//					//�܂�����
		//					SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y - g_player.fHeight / 2, g_player.pos.z), D3DXVECTOR3(10.0f, 0.0f, 0.0f), 10.0f, 10.0f, BULLETTYPE_PLAYER);

		//					//��
		//					SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y - g_player.fHeight / 2, g_player.pos.z), D3DXVECTOR3(10.0f, -0.5f, 0.0f), 10.0f, 10.0f, BULLETTYPE_PLAYER);
		//					g_BulletRecast = 0;
		//				}
		//				break;
		//			
		//		}
		//	}
		//}
		//if (g_player.DirectionMove == 1)
		//{
		//	if (GetKeyboardTrigger(DIK_F) == true)
		//	{
		//		// �e�𔭎�
		//			switch (g_player.BulletKind)
		//			{
		//				if (g_BulletRecast >= 10)
		//				{
		//			case BULLETKIND_NOMAL:
		//				PlaySound(SOUND_LABEL_SE_SHOT);
		//				SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y - g_player.fHeight / 2, g_player.pos.z), D3DXVECTOR3(-10.0f, 0.0f, 0.0f), 10.0f, 10.0f, BULLETTYPE_PLAYER);
		//				g_BulletRecast = 0;
		//				}
		//				break;

		//			case BULLETKIND_DIFFUSION:
		//				if (g_BulletRecast >= 30)
		//				{
		//					PlaySound(SOUND_LABEL_SE_SHOT1);
		//					//��
		//					SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y - g_player.fHeight / 2, g_player.pos.z), D3DXVECTOR3(-10.0f, 0.5f, 0.0f), 10.0f, 10.0f, BULLETTYPE_PLAYER);

		//					//�܂�����
		//					SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y - g_player.fHeight / 2, g_player.pos.z), D3DXVECTOR3(-10.0f, 0.0f, 0.0f), 10.0f, 10.0f, BULLETTYPE_PLAYER);

		//					//��
		//					SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y - g_player.fHeight / 2, g_player.pos.z), D3DXVECTOR3(-10.0f, -0.5f, 0.0f), 10.0f, 10.0f, BULLETTYPE_PLAYER);
		//					g_BulletRecast = 0;
		//				}
		//				break;
		//		}
		//	}
		//}

		/*if (GetKeyboardTrigger(DIK_R) == true)
		{
			PlaySound(SOUND_LABEL_SE_CHANGE);
			switch (g_player.BulletKind)
			{
			case BULLETKIND_NOMAL:
				g_player.BulletKind = BULLETKIND_DIFFUSION;
				break;
			case BULLETKIND_DIFFUSION:
				g_player.BulletKind = BULLETKIND_NOMAL;
				break;
			}
		}*/

		g_player.move.y += 0.3f;	//�d��

		g_player.pos.x += g_player.move.x;
		g_player.pos.y += g_player.move.y;

		g_player.move.x += (0.0f - g_player.move.x) * 0.15f;
		//-------------------------���n-------------------------
		/*if (g_player.pos.y >= 700)
		{
			g_player.bJump = false;
			g_player.pos.y = 700;
			g_player.move.y = 0.0f;
		}*/

		
		
		if (g_player.bJump == false)
		{
			// �A�j���[�V����
			pVtx[0].tex = D3DXVECTOR2(POLYGON_TEXTURE_UV_U*(g_nPatternAnim), g_player.DirectionMove * POLYGON_TEXTURE_UV_V);
			pVtx[1].tex = D3DXVECTOR2(POLYGON_TEXTURE_UV_U*(g_nPatternAnim)+POLYGON_TEXTURE_UV_U, g_player.DirectionMove * POLYGON_TEXTURE_UV_V);
			pVtx[2].tex = D3DXVECTOR2(POLYGON_TEXTURE_UV_U*(g_nPatternAnim), g_player.DirectionMove * POLYGON_TEXTURE_UV_V + POLYGON_TEXTURE_UV_V);
			pVtx[3].tex = D3DXVECTOR2(POLYGON_TEXTURE_UV_U*(g_nPatternAnim)+POLYGON_TEXTURE_UV_U, g_player.DirectionMove * POLYGON_TEXTURE_UV_V + POLYGON_TEXTURE_UV_V);
		}

		//�ǂɂԂ������Ƃ�
		if (g_player.pos.x  < 15)
		{
			g_player.pos.x = 15;
		}
		if (g_player.pos.x + 15 > SCREEN_WIDTH)
		{
			g_player.pos.x = SCREEN_WIDTH - 15;
		}

		if (CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move,g_player.fWidth, g_player.fHeight/*, &g_player.pBlock*/) == true)
		{
			g_player.bJump = false;
		}
		else
		{
			if (CollisionBlock(&g_player.pos, &g_player.posOld, &g_player.move, g_player.fWidth, g_player.fHeight/*, &g_player.pBlock*/) == false)
			{
				g_player.bJump = true;
			}
		}

		if (g_player.pos.y > 800)
		{
			StopSound(SOUND_LABEL_BGM001);
			g_player.bUse = false;
			SetFade(MODE_RESULT);
			//SetGameState(GAMESTATE_END);
		}

		/*if (g_player.pBlock != NULL)
		{

		}*/

		if (CollisionItem(&g_player.pos, &g_player.posOld, g_player.fWidth, g_player.fHeight) == true)
		{
			//AddScore(+100);
			g_Item.bUse = false;
		}

		/*else
		{
			if (CollisionItem(&g_player.pos, &g_player.posOld, g_player.fWidth, g_player.fHeight) == false)
			{
				g_Item.bUse = true;
			}
		}*/

		if (CollisionEnemy(&g_player.pos, &g_player.posOld, g_player.fWidth, g_player.fHeight) == true)
		{
			g_player.bUse = false;
			//SetFade(MODE_RESULT);
		}

		//SetEffect(g_player.pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), 10.0f, 100);
		//���_�ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y - (g_player.fHeight), g_player.pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_player.pos.x + g_player.fWidth, g_player.pos.y - (g_player.fHeight), g_player.pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_player.pos.x - g_player.fWidth, g_player.pos.y, g_player.pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_player.pos.x + g_player.fWidth, g_player.pos.y, g_player.pos.z);
		//�A�����b�N
		g_pVtxBuffPlayer->Unlock();
	}
}

void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	if (g_player.bUse == true)
	{
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		pDevice->SetTexture(0, g_pTexturePlayer);

		pDevice->SetFVF(FVF_VERTEX_2D);
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

}


//=============================================================================
// Hit���̏���
//=============================================================================
void HitPlayer(int nDmage)
{
	Player *pPlayer;
	pPlayer = GetPlayer();

	g_player.nLife -= nDmage;
	if (g_player.nLife <= 0)
	{//���C�t���O�ȉ��ɂȂ����Ƃ�
		//PlaySound(SOUND_LABEL_SE_EXPLOSION);
		g_player.bDisp = true;
		for (int nCntParticle = 0; nCntParticle < 70; nCntParticle++)
		{// �p�[�e�B�N������
			D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);

			SetParticle(g_player.pos, movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 10 + 30, rand() % 20);
		}
		g_player.state = PLAYERSTATE_DEATH;
	}
	else
	{//�q�b�g�����@�̐F��ύX
		g_player.state = PLAYERSTATE_DAMAGE;
		g_player.nCounterState = 5;
		CutLife(1);
		PlaySound(SOUND_LABEL_SE_HIT001);
		VERTEX_2D *pVtx;
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		g_pVtxBuffPlayer->Unlock();
	}
}

Player *GetPlayer(void)
{
	return &g_player;
}