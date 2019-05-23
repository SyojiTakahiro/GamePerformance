//=============================================================================
//
// item���� [item.cpp]
// Author : Takahiro
//
//=============================================================================
#include "item.h"
#include "player.h"
#include "score.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "particle.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_ITEM		(256)		// �A�C�e���̍ő吔

#define POLYGON_TEXTURE_UV_U		(0.25f)		//U
#define POLYGON_TEXTURE_UV_V		(1.0f)		//V

#define POLYGON_TEXTURENAME		"data/TEXTURE/coin000.png"


//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureItem = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Item g_Item[MAX_ITEM];
float g_iSpin = 1.01f;
D3DXVECTOR3 g_posItem;
//=============================================================================
// ����������
//=============================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntItem;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �A�C�e���̏��̏�����
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item[nCntItem].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Item[nCntItem].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_Item[nCntItem].fWidth = 0;
		g_Item[nCntItem].fHeight = 0;
		g_Item[nCntItem].nCounterAnim = 0;
		g_Item[nCntItem].nPatternAnim = 0;
		g_Item[nCntItem].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME, &g_pTextureItem);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim * 0.25f, 1.0f);
		pVtx[1].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim * 0.25f + 0.25f, 1.0f);
		pVtx[2].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim * 0.25f, 0.0f + 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim * 0.25f + 0.25f, 0.0f + 1.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		

		pVtx += 4;//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	//�A�����b�N
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitItem(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureItem != NULL)
	{
		g_pTextureItem->Release();
		g_pTextureItem = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateItem(void)
{
	int nCntItem;
	VERTEX_2D*pVtx;
	Player *pPlayer;
	pPlayer = GetPlayer();

	//���_�o�b�t�@�����b�N
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		g_Item[nCntItem].pos.x -= pPlayer->move.x;
		g_Item[nCntItem].nCounterAnim++;
		if (g_Item[nCntItem].bUse == true)
		{	
			
			if ((g_Item[nCntItem].nCounterAnim % 8) == 0)
			{

				g_Item[nCntItem].nPatternAnim = (g_Item[nCntItem].nPatternAnim + 1) % 9;
				pVtx[nCntItem*4].tex = D3DXVECTOR2(POLYGON_TEXTURE_UV_U*(g_Item[nCntItem].nPatternAnim), 0 * POLYGON_TEXTURE_UV_V);
				pVtx[nCntItem*4+1].tex = D3DXVECTOR2(POLYGON_TEXTURE_UV_U*(g_Item[nCntItem].nPatternAnim) + POLYGON_TEXTURE_UV_U, 0 * POLYGON_TEXTURE_UV_V);
				pVtx[nCntItem*4+2].tex = D3DXVECTOR2(POLYGON_TEXTURE_UV_U*(g_Item[nCntItem].nPatternAnim), 0 * POLYGON_TEXTURE_UV_V + POLYGON_TEXTURE_UV_V);
				pVtx[nCntItem*4+3].tex = D3DXVECTOR2(POLYGON_TEXTURE_UV_U*(g_Item[nCntItem].nPatternAnim) + POLYGON_TEXTURE_UV_U, 0 * POLYGON_TEXTURE_UV_V + POLYGON_TEXTURE_UV_V);
			

				//for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
				//{// �p�[�e�B�N������
				//	D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);
				//	movePar.x = sinf(D3DX_PI * g_iSpin) * (rand() % 10 * 1.5f);
				//	movePar.y = cosf(D3DX_PI * g_iSpin) * (rand() % 5 * 1.5f);
				//	SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth/2, g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight / 2, g_Item[nCntItem].pos.z), movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 20 + 20.0f, rand() % 10);
				//}
			}
			pVtx[nCntItem * 4].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z);
			pVtx[nCntItem * 4+1].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + (g_Item[nCntItem].fWidth), g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z);
			pVtx[nCntItem * 4+2].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight, g_Item[nCntItem].pos.z);
			pVtx[nCntItem * 4+3].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + (g_Item[nCntItem].fWidth), g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight, g_Item[nCntItem].pos.z);


		}
		
	}
	
	//�A�����b�N
	g_pVtxBuffItem->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawItem(void)
{
	int nCntItem;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureItem);

	// �|���S���̕`��
	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntItem, 2);
		}
	}
}

//=============================================================================
// �A�C�e���̐ݒ菈��
//=============================================================================
void SetItem(D3DXVECTOR3 pos, D3DXCOLOR col,float fWidth, float fHeight)
{
	int nCntItem;
	VERTEX_2D*pVtx;

	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == false)
		{
			g_Item[nCntItem].fWidth = fWidth;
			g_Item[nCntItem].fHeight = fHeight;
			g_Item[nCntItem].pos = pos;

			pVtx[0].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + (g_Item[nCntItem].fWidth), g_Item[nCntItem].pos.y, g_Item[nCntItem].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x, g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight, g_Item[nCntItem].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_Item[nCntItem].pos.x + (g_Item[nCntItem].fWidth), g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight, g_Item[nCntItem].pos.z);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim * 0.25f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim * 0.25f + 0.25f, 1.0f);
			pVtx[2].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim * 0.25f, 0.0f + 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_Item[nCntItem].nPatternAnim * 0.25f + 0.25f, 0.0f + 1.0f);

			/*pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);*/

			//for (int nCntParticle = 0; nCntParticle < 70; nCntParticle++)
			//{// �p�[�e�B�N������
			//	D3DXVECTOR3 movePar(0.0f, 0.0f, 0.0f);
			//	movePar.x = sinf(D3DX_PI * g_iSpin) * (rand() % 20 * 0.3f);
			//	movePar.y = cosf(D3DX_PI * g_iSpin) * (rand() % 20 * 0.3f);
			//	SetParticle(g_Item[nCntItem].pos, movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 10 + 30.0f, rand() % 20);
			//}

			//g_Item[nCntItem].nCounterAnim = 0;
			//g_Item[nCntItem].nPatternAnim = 0;
			g_Item[nCntItem].bUse = true;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffItem->Unlock();
}

bool CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fWidth, float fHeight)
{
	//Item *pItem;
	int nCntItem;
	//pItem = GetItem();
	bool bLand = false;

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++)
	{
		if (g_Item[nCntItem].bUse == true)
		{
			if (pPos->x - (fWidth / 2) <= g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth
				&& pPos->x + (fWidth / 2) >= g_Item[nCntItem].pos.x)
			{//�A�C�e���̉���
				if (pPosOld->y <= g_Item[nCntItem].pos.y
					&&pPos->y >= g_Item[nCntItem].pos.y)
				{//�ォ��߂荞��
					PlaySound(SOUND_LABEL_SE_GET);
					bLand = true;
					AddScore(+100);
					g_Item[nCntItem].bUse = false;
					for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
					{// �p�[�e�B�N������
						D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);
						movePar.x = sinf(D3DX_PI * g_iSpin) * (rand() % 10 * 1.5f);
						movePar.y = cosf(D3DX_PI * g_iSpin) * (rand() % 5 * 1.5f);
						SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth / 2, g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight / 2, g_Item[nCntItem].pos.z), movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 20 + 20.0f, rand() % 10);
					}
					//SetGameState(GAMESTATE_CLEAR);
					
				}
				else if (pPosOld->y - fHeight >= g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight
					&& pPos->y - fHeight <= g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight)
				{//������߂肱��
					PlaySound(SOUND_LABEL_SE_GET); 
					bLand = true;
					AddScore(+100);
					g_Item[nCntItem].bUse = false;
					for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
					{// �p�[�e�B�N������
						D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);
						movePar.x = sinf(D3DX_PI * g_iSpin) * (rand() % 10 * 1.5f);
						movePar.y = cosf(D3DX_PI * g_iSpin) * (rand() % 5 * 1.5f);
						SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth / 2, g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight / 2, g_Item[nCntItem].pos.z), movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 20 + 20.0f, rand() % 10);
					}
					//SetGameState(GAMESTATE_CLEAR);
				}
			}
			//�A�C�e���̏c��
			if (//�����瑫���ǂɂԂ�����
				pPosOld->y - fHeight <= g_Item[nCntItem].pos.y
				&&pPosOld->y >= g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight
				//�����ǂɂԂ�����
				|| pPosOld->y - fHeight > g_Item[nCntItem].pos.y
				&&pPosOld->y - fHeight < g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight
				//�����ǂɂԂ�����
				|| pPosOld->y > g_Item[nCntItem].pos.y
				&&pPosOld->y < g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight)
			{
				//������߂荞��
				if (pPosOld->x <= g_Item[nCntItem].pos.x
					&& pPos->x + fWidth / 2 >= g_Item[nCntItem].pos.x)
				{
					PlaySound(SOUND_LABEL_SE_GET);
					AddScore(+100);
					g_Item[nCntItem].bUse = false;
					for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
					{// �p�[�e�B�N������
						D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);
						movePar.x = sinf(D3DX_PI * g_iSpin) * (rand() % 10 * 1.5f);
						movePar.y = cosf(D3DX_PI * g_iSpin) * (rand() % 5 * 1.5f);
						SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth / 2, g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight / 2, g_Item[nCntItem].pos.z), movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 20 + 20.0f, rand() % 10);
					}
					//SetGameState(GAMESTATE_CLEAR);

				}
				//�E����߂荞��
				else if (pPosOld->x > g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth
					&&pPos->x - fWidth / 2 < g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth)
				{
					PlaySound(SOUND_LABEL_SE_GET);
					AddScore(+100);
					g_Item[nCntItem].bUse = false;
					for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
					{// �p�[�e�B�N������
						D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);
						movePar.x = sinf(D3DX_PI * g_iSpin) * (rand() % 10 * 1.5f);
						movePar.y = cosf(D3DX_PI * g_iSpin) * (rand() % 5 * 1.5f);
						SetParticle(D3DXVECTOR3(g_Item[nCntItem].pos.x + g_Item[nCntItem].fWidth / 2, g_Item[nCntItem].pos.y + g_Item[nCntItem].fHeight / 2, g_Item[nCntItem].pos.z), movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 20 + 20.0f, rand() % 10);
					}
					//SetGameState(GAMESTATE_CLEAR);
				}
			}
		}
	}
	return bLand;
}

Item *pItem(void)
{
	return &g_Item[MAX_ITEM];
}