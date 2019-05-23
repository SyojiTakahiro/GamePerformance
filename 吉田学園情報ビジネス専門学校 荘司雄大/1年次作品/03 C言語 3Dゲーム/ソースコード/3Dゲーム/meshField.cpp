//=============================================================================
//
// �|���S������ [MeshField.cpp]
// Author : 
//
//=============================================================================
#include "meshField.h"
#include "player.h"
#include "shadow.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_NAME	"data/TEXTURE/field000.jpeg"
#define MAX_X			(10)
#define MAX_Z			(8)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;

D3DXVECTOR3 g_posMeshField;
D3DXVECTOR3 g_rotMeshField;
D3DXMATRIX g_mtxWorldMeshField;

int g_nNumVertexMeshField;
int g_nNumIndexMeshField;
int g_nNumPolygonMeshField;

//=============================================================================
// ����������
//=============================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	int nCountIdx = 0;
	int nCountVtx = 0;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &g_pTextureMeshField);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * ((MAX_X + 1) * (MAX_Z + 1)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	// �C���f�b�N�X�o�b�t�@�̍쐬
	pDevice->CreateIndexBuffer(sizeof(WORD) * (MAX_X*MAX_Z * 2 + (4 * (MAX_Z - 1)) + 2),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	VERTEX_3D*pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//for (int nCntVtx_z = 0; nCntVtx_z < MAX_Z + 1; nCntVtx_z++)
	//{
	//	for (int nCntVtx_x = 0; nCntVtx_x < MAX_X + 1; nCntVtx_x++)
	//	{
	//		//���_���W
	//		pVtx[nCntVtx_z + nCntVtx_x + nCountVtx].pos = D3DXVECTOR3(-50 + (50 * nCntVtx_x) ,0, 50 - (50 * nCntVtx_z));

	//		pVtx[nCntVtx_z + nCntVtx_x + nCountVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//		pVtx[nCntVtx_z + nCntVtx_x + nCountVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//		pVtx[nCntVtx_z + nCntVtx_x + nCountVtx].tex = D3DXVECTOR2(nCntVtx_x * 1.0f, nCntVtx_z * 1.0f);
	//	}
	//	nCountVtx += 2;
	//}
	

	for (int nCntVtx_z = 0; nCntVtx_z  < MAX_Z + 1; nCntVtx_z++)
	{
		for (int nCntVtx_x = 0; nCntVtx_x < MAX_X + 1; nCntVtx_x++)
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f + (nCntVtx_x  * 50.0f), 0, 50.0f - (nCntVtx_z  * 50.0f));
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f,1.0f, 1.0f);
			pVtx[0].tex = D3DXVECTOR2(nCntVtx_x *1.0f, nCntVtx_z *1.0f);
			pVtx += 1;
		}
	}

	//�A�����b�N
	g_pVtxBuffMeshField->Unlock();

	WORD*pIdx;		//�|�C���^
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx_z = 0; nCntIdx_z < MAX_Z; nCntIdx_z++)
	{
		for (int nCntIdx_x = 0; nCntIdx_x < MAX_X + 1; nCntIdx_x++, nCountIdx++)
		{
			pIdx[0] = nCountIdx + MAX_X + 1;
			pIdx[1] = nCountIdx;
			pIdx += 2;
			if (nCntIdx_z < MAX_Z && nCntIdx_x == MAX_X)
			{
				pIdx[0] = nCountIdx;
				pIdx[1] = nCountIdx + (MAX_X + 1) + 1;
				pIdx += 2;
			}
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

	g_pIdxBuffMeshField->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureMeshField != NULL)
	{
		g_pTextureMeshField->Release();
		g_pTextureMeshField = NULL;
	}
	//���_�̔j��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;

	pDevice = GetDevice();	// �f�o�C�X�̎擾

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldMeshField);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshField.y, g_rotMeshField.x, g_rotMeshField.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_posMeshField.x, g_posMeshField.y, g_posMeshField.z);

	D3DXMatrixMultiply(&g_mtxWorldMeshField, &g_mtxWorldMeshField, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldMeshField);
	
	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	pDevice->SetIndices(g_pIdxBuffMeshField);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshField);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, (MAX_X + 1) * (MAX_Z + 1),0, ((MAX_X + 1) * (MAX_Z + 1) + ((MAX_Z - 1) * 2) + (MAX_X + 1) * (MAX_Z - 1)) - 2);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexMeshField(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�


	// ���_���̐ݒ�

}