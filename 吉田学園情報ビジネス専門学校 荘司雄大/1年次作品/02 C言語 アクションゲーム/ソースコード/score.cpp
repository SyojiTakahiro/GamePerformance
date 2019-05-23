//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : Takahiro
//
//=============================================================================
#include "score.h"
#include "fade.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
//void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
int						g_nScore;					// �X�R�A

//=============================================================================
// ����������
//=============================================================================
void InitScore(void)
{
	int nCntScore;
	float fScore;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �X�R�A�̏�����


	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
							TEXTURE_SCORE,			// �t�@�C���̖��O
							&g_pTextureScore);		// �e�N�X�`���ւ̃|�C���^

	// ���_���̍쐬
	//MakeVertexScore(pDevice);

	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SCORE,
		&g_pTextureScore
	);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N
	g_pVtxBuffScore->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntScore = 0, fScore = 1200; nCntScore < 8; nCntScore++, fScore -= 30)
	{

		//���_�ݒ�
		pVtx[0].pos = D3DXVECTOR3(fScore, 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fScore + 30.0f, 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fScore, 80.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fScore + 30.0f, 80.0f, 0.0f);
		//1.0f�ŌŒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//���_�J���[�ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 0, 255);
		//�e�N�X�`���ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//���_�o�b�t�@���A�����b�N
		pVtx += 4;
	}
	g_pVtxBuffScore->Unlock();
	g_nScore = 0;
}

//=============================================================================
// �I������
//=============================================================================
void UninitScore(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateScore(void)
{
	int nMemory;
	int nCntScore;
	nMemory = g_nScore;

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N
	g_pVtxBuffScore->Lock( 0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < 8; nCntScore++)
	{
		//�e�N�X�`���ݒ�
		pVtx[nCntScore * 4].tex = D3DXVECTOR2((nMemory % 10)*0.1f, 0.0f);
		pVtx[nCntScore * 4 + 1].tex = D3DXVECTOR2((nMemory % 10)*0.1f + 0.1f, 0.0f);
		pVtx[nCntScore * 4 + 2].tex = D3DXVECTOR2((nMemory % 10)*0.1f, 1.0f);
		pVtx[nCntScore * 4 + 3].tex = D3DXVECTOR2((nMemory % 10)*0.1f + 0.1f, 1.0f);
		nMemory /= 10;
	}

	if (g_nScore == 1700)
	{
		GAMESTATE pGameState;
		pGameState = *GetGameState();
		StopSound(SOUND_LABEL_BGM001);
		if (pGameState != GAMESTATE_END)
		{
			
			SetGameState(GAMESTATE_END);
			SetFade(MODE_CLEAR);
		}
	}
	g_pVtxBuffScore->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
	int nCntScore;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	// �|���S���̕`��
	for (nCntScore = 0; nCntScore < 8; nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

////=============================================================================
//// ���_���̍쐬
////=============================================================================
//void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
//{
//	// �e�N�X�`���̊J��
//	
//
//	// ���_�o�b�t�@�̊J��
//	
//
//}

//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void AddScore(int nValue)
{
	g_nScore += nValue;
}
