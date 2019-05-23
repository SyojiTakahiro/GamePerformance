//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : 
//
//=============================================================================
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define POLYGON_POS_X			(20)							//���_���WX�̈ړ���
#define POLYGON_POS_Y			(20)							//���_���WY�̈ړ���
#define TEX_POS_X_INIT			(1.0f)							//�e�N�X�`�����WU�̏����ʒu
#define TEX_POS_Y_INIT			(1.0f)							//�e�N�X�`�����WV�̏����ʒu
#define TEX_LEFT				(0.0f)							//�e�N�X�`�����WU��
#define TEX_RIGHT				(1.0f)							//�e�N�X�`�����WU�E
#define TEX_TOP					(0.0f)							//�e�N�X�`�����WV��
#define TEX_BOT					(1.0f)							//�e�N�X�`�����WV��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
int						g_nScore = 0;				// �X�R�A
D3DXVECTOR3				g_ScorePos(0.0f, 0.0f, 0.0f);
//=============================================================================
// ����������
//=============================================================================
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	

	// �X�R�A�̏�����
	g_nScore = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,				// �f�o�C�X�ւ̃|�C���^
								TEXTURE_SCORE,		// �t�@�C���̖��O
								&g_pTextureScore);	// �e�N�X�`���ւ̃|�C���^

	// ���_���̍쐬
	MakeVertexScore(pDevice);
	
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
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntScore;

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
    pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	// �|���S���̕`��

	for (nCntScore = 0; nCntScore < 8; nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0 + (4 * nCntScore),	//�J�n���钸�_�̃C���f�b�N�X
			2); //�`�悷��v���~�e�B�u��
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
{	
	// ���_���̍쐬
	VERTEX_2D *pVtx;
	int nCntScore;
	int nPosMove = 0;
	g_ScorePos.x = 1100.0f;
	g_ScorePos.y = 20.0f;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	//���_����ݒ�
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore  < 8; nCntScore++)
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(g_ScorePos.x - POLYGON_POS_X , g_ScorePos.y - POLYGON_POS_Y, g_ScorePos.z);
		pVtx[1].pos = D3DXVECTOR3(g_ScorePos.x + POLYGON_POS_X, g_ScorePos.y - POLYGON_POS_Y, g_ScorePos.z);
		pVtx[2].pos = D3DXVECTOR3(g_ScorePos.x - POLYGON_POS_X, g_ScorePos.y + POLYGON_POS_Y, g_ScorePos.z);
		pVtx[3].pos = D3DXVECTOR3(g_ScorePos.x + POLYGON_POS_X, g_ScorePos.y + POLYGON_POS_Y, g_ScorePos.z);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//���_
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
		g_ScorePos.x -= 40;
	}
	 //���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//=============================================================================
// �X�R�A�̉��Z
//=============================================================================
void AddScore(int nValue)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;
	int nCntScore;
	int nPosMove = 0;
	int nAnswer = 0;
	int nScore = 1;
	g_ScorePos.x = 1100.0f;
	g_ScorePos.y = 20.0f;

	g_nScore += nValue;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < 8; nCntScore++)
	{
		nAnswer = g_nScore % (nScore * 10) / nScore;

		//���_���W
		pVtx[0].pos = D3DXVECTOR3(g_ScorePos.x - POLYGON_POS_X, g_ScorePos.y - POLYGON_POS_Y, g_ScorePos.z);
		pVtx[1].pos = D3DXVECTOR3(g_ScorePos.x + POLYGON_POS_X, g_ScorePos.y - POLYGON_POS_Y, g_ScorePos.z);
		pVtx[2].pos = D3DXVECTOR3(g_ScorePos.x - POLYGON_POS_X, g_ScorePos.y + POLYGON_POS_Y, g_ScorePos.z);
		pVtx[3].pos = D3DXVECTOR3(g_ScorePos.x + POLYGON_POS_X, g_ScorePos.y + POLYGON_POS_Y, g_ScorePos.z);

		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f + (nAnswer * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (nAnswer * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (nAnswer * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (nAnswer * 0.1f), 1.0f);

		//���_
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
		nScore *= 10;
		g_ScorePos.x -= 40;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();	
}

//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int GetScore(void)
{
	return g_nScore;
}
//=============================================================================
// �X�R�A�̃Z�b�g
//=============================================================================
void SetScore(int nScore)
{
	g_nScore = nScore;
}