//=============================================================================
//
// �`���[�g���A�� [t.cpp]
// Author :Takahiro
//
//=============================================================================
#include "title.h"
#include "t.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLE_TEXTURENAME		"data/TEXTURE/bg1100.jpg"			//�w�i�摜
#define TITLE_POS_X			(0)					//X���W
#define TITLE_POS_Y			(0)					//Y���W
#define TITLE_WIDTH			(SCREEN_WIDTH)		//��ʂ̕�
#define TITLE_HEIGHT		(SCREEN_HEIGHT)		//��ʂ̍���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureT = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffT = NULL;

//=============================================================================
// ���C���֐�
//=============================================================================
void InitT(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAME, &g_pTextureT);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffT,
		NULL);

	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffT->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, TITLE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, TITLE_POS_Y + TITLE_HEIGHT, 0.0f);

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
	g_pVtxBuffT->Unlock();
}

void UninitT(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureT != NULL)
	{
		g_pTextureT->Release();
		g_pTextureT = NULL;
	}
	//���_�̔j��
	if (g_pVtxBuffT != NULL)
	{
		g_pVtxBuffT->Release();
		g_pVtxBuffT = NULL;
	}
}

void UpdateT(void)
{
	FADE fade;
	fade = GetFade();
	if (GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE)
	{
		SetFade(MODE_GAME);
		PlaySound(SOUND_LABEL_SE_DECIDE);
	}
}

void DrawT(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	pDevice->SetStreamSource(0, g_pVtxBuffT, 0, sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_pTextureT);

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}