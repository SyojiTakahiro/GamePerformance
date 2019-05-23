//=============================================================================
//
// �|���S������ [BG.cpp]
// Author : 
//
//=============================================================================
#include "bg.h"
#include "player.h"
#include "shadow.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_NAME	"data/TEXTURE/block000.jpg"

#define MAX_H			(8)
#define MAX_V			(2)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexBG(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBG = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffBG = NULL;

D3DXVECTOR3 g_posBG;
D3DXVECTOR3 g_rotBG;
D3DXMATRIX g_mtxWorldBG;

int g_nNumVertexBG;
int g_nNumIndexBG;
int g_nNumPolygonBG;

//=============================================================================
// ����������
//=============================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	int nCountIdx = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &g_pTextureBG);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MAX_H + 1) * (MAX_V + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	// �C���f�b�N�X�o�b�t�@�̍쐬
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffBG,
		NULL);

	VERTEX_3D*pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(-50, 0, 50);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 50);
	pVtx[2].pos = D3DXVECTOR3(50, 0, 50);
	pVtx[3].pos = D3DXVECTOR3(-50, 0, 0);
	pVtx[4].pos = D3DXVECTOR3(0, 0, 0);
	pVtx[5].pos = D3DXVECTOR3(50, 0, 0);
	pVtx[6].pos = D3DXVECTOR3(-50, 0, -50);
	pVtx[7].pos = D3DXVECTOR3(0, 0, -50);
	pVtx[8].pos = D3DXVECTOR3(50, 0, -50);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);

	//�A�����b�N
	g_pVtxBuffBG->Unlock();

	WORD*pIdx;		//�|�C���^
	g_pIdxBuffBG->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntV = 0; nCntV < MAX_V + 1; nCntV++)
	{
		for (int nCntH = 0; nCntH < MAX_H + 1; nCntH++)
		{
			pVtx[0].pos.x = sinf(D3DX_PI*0.0f) * 100;
			pVtx[0].pos.y = 100;
			pVtx[0].pos.z = cosf(D3DX_PI*0.0f) * 100;
		}
	}
	/*pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;*/

	g_pIdxBuffBG->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitBG(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBG != NULL)
	{
		g_pTextureBG->Release();
		g_pTextureBG = NULL;
	}
	//���_�̔j��
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffBG != NULL)
	{
		g_pIdxBuffBG->Release();
		g_pIdxBuffBG = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBG(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;

	pDevice = GetDevice();	// �f�o�C�X�̎擾

							// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldBG);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotBG.y, g_rotBG.x, g_rotBG.z);

	D3DXMatrixMultiply(&g_mtxWorldBG, &g_mtxWorldBG, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posBG.x, g_posBG.y, g_posBG.z);

	D3DXMatrixMultiply(&g_mtxWorldBG, &g_mtxWorldBG, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBG);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_3D));

	pDevice->SetIndices(g_pIdxBuffBG);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBG);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 9, 0, 12);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexBG(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�


	// ���_���̐ݒ�

}