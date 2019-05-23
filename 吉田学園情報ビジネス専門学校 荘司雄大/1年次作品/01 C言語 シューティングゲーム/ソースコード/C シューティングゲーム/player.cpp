//=============================================================================
//
// �v���C���[�̕\������ [player.cpp]
// Author : Takahiro Syoji
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "explosion.h"
#include "game.h"
#include "effect.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define POLYGON_TEXTURENAME		"data/TEXTURE/player000.png"
//Player g_aPlayer;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;

//D3DXVECTOR3 g_player.pos;
//D3DXVECTOR3 g_player.move;

Player g_player;

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
		g_player.nType = 0;
		g_player.nCounterState = 60;
		g_player.state;
		g_player.nLife = 5;
		g_player.bDisp = false;
		g_player.bUse = true;
	}

	g_nCounterAnim = 0;
	g_nPatternAnim = 0;

	g_player.pos = D3DXVECTOR3(200, 350, 0.0f);
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
	pVtx[0].pos = D3DXVECTOR3(g_player.pos.x - 50, g_player.pos.y - 50, g_player.pos.z);
	pVtx[1].pos = D3DXVECTOR3(g_player.pos.x + 50, g_player.pos.y - 50, g_player.pos.z);
	pVtx[2].pos = D3DXVECTOR3(g_player.pos.x - 50, g_player.pos.y + 50, g_player.pos.z);
	pVtx[3].pos = D3DXVECTOR3(g_player.pos.x + 50, g_player.pos.y + 50, g_player.pos.z);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0, 0);
	pVtx[1].tex = D3DXVECTOR2(1, 0);
	pVtx[2].tex = D3DXVECTOR2(0, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 1);

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
	g_player.pos += g_player.move;

	/*if (g_nCounterAnim %10)== 0)
	{
		g_nPatternAnim = (g_nPatternAnim + 1) % 10
	}*/

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
		//���b�N
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);
		if (g_player.bUse == true)
		{
			if (GetKeyboardPress(DIK_LSHIFT)== false)
			{
				//�΂߈ړ��̏���
				if (GetKeyboardPress(DIK_A) == true)
				{
					if (GetKeyboardPress(DIK_W) == true)
					{//����ړ�
						g_player.pos.x += sinf(-D3DX_PI*0.75f) * 6;
					}
					else if (GetKeyboardPress(DIK_S) == true)
					{//�����ړ�
						g_player.pos.x += sinf(-D3DX_PI*0.25f) * 6;
					}
					else
					{//���ړ�
						g_player.pos.x -= 6;
					}
				}

				//�΂߈ړ��̏���
				if (GetKeyboardPress(DIK_D) == true)
				{
					if (GetKeyboardPress(DIK_W) == true)
					{//�E��ړ�
						g_player.pos.x += sinf(D3DX_PI*0.75f) * 6;
					}
					else if (GetKeyboardPress(DIK_S) == true)
					{//�E���ړ�
						g_player.pos.x += sinf(D3DX_PI*0.25f) * 6;
					}
					else
					{//�E�ړ�
						g_player.pos.x += 6;
					}
				}

				if (GetKeyboardPress(DIK_W) == true)
				{
					g_player.pos.y += cosf(-D3DX_PI) * 6;
				}
				if (GetKeyboardPress(DIK_S) == true)
				{
					g_player.pos.y += cosf(D3DX_PI * 0) * 6;
				}
				if (GetKeyboardPress(DIK_SPACE) == true)		//Press or Trigger
				{
					g_BulletRecast++;
					if (g_BulletRecast >= 20)
					{
						SetBullet(g_player.pos, D3DXVECTOR3(10.0f, 0.0f, 0.0f), BULLETTYPE_PLAYER);
						PlaySound(SOUND_LABEL_SE_SHOT);

						D3DXVECTOR3 MPSL(0.0f, 0.0f, 0.0f);
						MPSL.x += sinf(D3DX_PI*0.535f) * 10;
						MPSL.y += cosf(D3DX_PI*0.535f) * 10;
						SetBullet(g_player.pos, MPSL, BULLETTYPE_PLAYER);

						D3DXVECTOR3 MPSR(0.0f, 0.0f, 0.0f);
						MPSR.x += sinf(D3DX_PI*0.465f) * 10;
						MPSR.y += cosf(D3DX_PI*0.465f) * 10;
						SetBullet(g_player.pos, MPSR, BULLETTYPE_PLAYER);
						g_BulletRecast = 0;
					}
				}
			}

			//
			else if (GetKeyboardPress(DIK_LSHIFT) == true)
			{
				//�΂߈ړ��̏���
				if (GetKeyboardPress(DIK_A) == true)
				{
					if (GetKeyboardPress(DIK_W) == true)
					{//����ړ�
						g_player.pos.x += sinf(-D3DX_PI*0.75f) * 3;
					}
					else if (GetKeyboardPress(DIK_S) == true)
					{//�����ړ�
						g_player.pos.x += sinf(-D3DX_PI*0.25f) * 3;
					}
					else
					{//���ړ�
						g_player.pos.x -= 3;
					}
				}

				//�΂߈ړ��̏���
				if (GetKeyboardPress(DIK_D) == true)
				{
					if (GetKeyboardPress(DIK_W) == true)
					{//�E��ړ�
						g_player.pos.x += sinf(D3DX_PI*0.75f) * 3;
					}
					else if (GetKeyboardPress(DIK_S) == true)
					{//�E���ړ�
						g_player.pos.x += sinf(D3DX_PI*0.25f) * 3;
					}
					else
					{//�E�ړ�
						g_player.pos.x += 3;
					}
				}

				if (GetKeyboardPress(DIK_W) == true)
				{
					g_player.pos.y += cosf(-D3DX_PI) * 3;
				}
				if (GetKeyboardPress(DIK_S) == true)
				{
					g_player.pos.y += cosf(D3DX_PI * 0) * 3;
				}
				if (GetKeyboardPress(DIK_SPACE) == true)		//Press or Trigger
				{
					g_BulletRecast++;
					if (g_BulletRecast >= 10)
					{
						SetBullet(g_player.pos, D3DXVECTOR3(10.0f, 0.0f, 0.0f), BULLETTYPE_PLAYER);
						PlaySound(SOUND_LABEL_SE_SHOT);
						g_BulletRecast = 0;
					}
				}
			}
			if (g_player.pos.x  < 20)
			{
				g_player.pos.x = 20;
			}
			if (g_player.pos.x + 20 > SCREEN_WIDTH)
			{
				g_player.pos.x = SCREEN_WIDTH - 20;
			}
			if (g_player.pos.y - 20 < 0)
			{
				g_player.pos.y = 20;
			}
			if (g_player.pos.y + 20 > SCREEN_HEIGHT)
			{
				g_player.pos.y = SCREEN_HEIGHT - 20;
			}

			pVtx[0].pos = D3DXVECTOR3(g_player.pos.x - 20, g_player.pos.y - 20, g_player.pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_player.pos.x + 20, g_player.pos.y - 20, g_player.pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_player.pos.x - 20, g_player.pos.y + 20, g_player.pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_player.pos.x + 20, g_player.pos.y + 20, g_player.pos.z);
	
			SetEffect(g_player.pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f),10.0f, 100);

			g_player.nCounterState--;
			/*if ((g_nCounterAnim % 3) == 0)
			{
				g_nPatternAnim = (g_nPatternAnim + 1) % 8;

				pVtx[0].tex = D3DXVECTOR2(g_nPatternAnim * 0.125f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(g_nPatternAnim * 0.125f + 0.125f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(g_nPatternAnim * 0.125f, 0.0f + 1.0f);
				pVtx[3].tex = D3DXVECTOR2(g_nPatternAnim * 0.125f + 0.125f, 0.0f + 1.0f);
			}*/
			//�A�����b�N
			g_pVtxBuffPlayer->Unlock();
		}
	}
}

void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	
	//�f�o�C�X�̎擾
	pDevice = GetDevice();
	
	if (g_player.bDisp == false)
	{
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_pTexturePlayer);

	pDevice->SetFVF(FVF_VERTEX_2D);
	/*pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);*/
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);
	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,0, 2);
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
	{//���C�t���O�ȉ��ɂȂ����Ƃ�����
		SetExplosion(g_player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		g_player.bDisp = true;
		g_player.state = PLAYERSTATE_DEATH;
	}
	else
	{//�q�b�g���G�̐F��ύX
		g_player.state = PLAYERSTATE_DAMAGE;
		g_player.nCounterState = 5;
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