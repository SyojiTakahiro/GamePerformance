//=============================================================================
//
// �w�i���� [bg.cpp]
// Author : Takahiro Syoji
//
//=============================================================================
#include "bg2.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_TEXTURENAME		"data/TEXTURE/bg1000.png"			//�w�i�摜
#define BG_POS_X			(0)					//X���W
#define BG_POS_Y			(0)					//Y���W
#define BG_WIDTH			(SCREEN_WIDTH)		//��ʂ̕�
#define BG_HEIGHT			(SCREEN_HEIGHT)		//��ʂ̍���
#define BG_SPEED			(0.00005f)

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR2 pos;	//�ʒu
	D3DXVECTOR2 move;	//�ړ���
	bool bUse;			//�g�p���Ă��邩�ǂ���
}BG2;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBG2 = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG2 = NULL;

int g_nCntAnim;
int g_nPtnAnim;
//=============================================================================
// ���C���֐�
//=============================================================================
void InitBG2(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME, &g_pTextureBG2);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG2,
		NULL);

	g_nCntAnim = 0;
	g_nPtnAnim = 0;

	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBG2->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_POS_X + BG_WIDTH, BG_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y + BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_POS_X + BG_WIDTH, BG_POS_Y + BG_HEIGHT, 0.0f);

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
	g_pVtxBuffBG2->Unlock();
}

void UninitBG2(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBG2 != NULL)
	{
		g_pTextureBG2->Release();
		g_pTextureBG2 = NULL;
	}
	//���_�̔j��
	if (g_pVtxBuffBG2 != NULL)
	{
		g_pVtxBuffBG2->Release();
		g_pVtxBuffBG2 = NULL;
	}
}

void UpdateBG2(void)
{
	g_nCntAnim++;
	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBG2->Lock(0, 0, (void**)&pVtx, 0);


	pVtx[0].tex = D3DXVECTOR2(0.0f + (g_nCntAnim * BG_SPEED), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f + (g_nCntAnim * BG_SPEED), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (g_nCntAnim * BG_SPEED), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f + (g_nCntAnim * BG_SPEED), 1.0f);

	//�A�����b�N
	g_pVtxBuffBG2->Unlock();
}

void DrawBG2(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	pDevice->SetStreamSource(0, g_pVtxBuffBG2, 0, sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_pTextureBG2);

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}