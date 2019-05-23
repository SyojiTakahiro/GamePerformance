//=============================================================================
//
// �^�C�}�[���� [timer.cpp]
// Author : Takahiro
//
//=============================================================================
#include "timer.h"
#include "game.h"
#include "fade.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TIMER		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureTimer = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
int						g_nTimer = 30;		//
int						g_FrameTimer = 0;

//=============================================================================
// ����������
//=============================================================================
void InitTimer(void)
{
	int nCntTimer;
	float fTimer;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �^�C�}�[�̏�����
	g_nTimer = 30;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
		TEXTURE_TIMER,								// �t�@�C���̖��O
		&g_pTextureTimer);							// �e�N�X�`���ւ̃|�C���^

	// ���_���̍쐬
	//MakeVertexScore(pDevice);

	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_TIMER,
		&g_pTextureTimer
	);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL
	);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTimer->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntTimer = 0, fTimer = 900; nCntTimer < 3; nCntTimer++, fTimer -= 30)
	{

		//���_�ݒ�
		pVtx[0].pos = D3DXVECTOR3(fTimer, 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fTimer + 30.0f, 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fTimer, 80.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fTimer + 30.0f, 80.0f, 0.0f);
		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//���_�J���[�ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		//�e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//���_�o�b�t�@���A�����b�N
		pVtx += 4;
	}
	g_pVtxBuffTimer->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitTimer(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateTimer(void)
{
	int nCntTimer;
	g_FrameTimer++;
	int nAnswer;
	int nNmber = 1;

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	if (g_FrameTimer >= 60)
	{
		g_nTimer--;
		g_FrameTimer = 0;
	}

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < 3; nCntTimer++)
	{
		nAnswer = g_nTimer / nNmber;

		//�e�N�X�`���ݒ�
		pVtx[nCntTimer * 4].tex = D3DXVECTOR2((nAnswer % 10)*0.1f, 0.0f);
		pVtx[nCntTimer * 4 + 1].tex = D3DXVECTOR2((nAnswer % 10)*0.1f + 0.1f, 0.0f);
		pVtx[nCntTimer * 4 + 2].tex = D3DXVECTOR2((nAnswer % 10)*0.1f, 1.0f);
		pVtx[nCntTimer * 4 + 3].tex = D3DXVECTOR2((nAnswer % 10)*0.1f + 0.1f, 1.0f);
		nNmber *= 10;

	}

	if (g_nTimer <= 0)
	{
		g_nTimer = 0;
		GAMESTATE pGameState;
		pGameState = *GetGameState();

		StopSound(SOUND_LABEL_BGM001);

		if (pGameState != GAMESTATE_END)
		{
			SetGameState(GAMESTATE_END);
			SetFade(MODE_RESULT);
		}
	}
	g_pVtxBuffTimer->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawTimer(void)
{
	int nCntTimer;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTimer);

	// �|���S���̕`��
	for (nCntTimer = 0; nCntTimer < 3; nCntTimer++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTimer * 4, 2);
	}
}

