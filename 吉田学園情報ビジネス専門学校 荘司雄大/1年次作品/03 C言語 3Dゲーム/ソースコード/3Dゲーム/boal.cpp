//=============================================================================
//
// �Ǐ��� [Boal.cpp]
// Author : 
//
//=============================================================================
#include "boal.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_NAME	"data/TEXTURE/bullet000.png"
#define MAX_Boal (128)
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexBoal(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBoal = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoal = NULL;

//D3DXVECTOR3 g_aBoal[nCntBoal].pos;
//D3DXVECTOR3 g_aBoal[nCntBoal].rot;
//D3DXMATRIX g_aBoal[nCntBoal].mtxWorld;

Boal g_aBoal[MAX_Boal];

//=============================================================================
// ����������
//=============================================================================
void InitBoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	pDevice = GetDevice();
	int nCntBoal;

	for (nCntBoal = 0; nCntBoal < MAX_Boal; nCntBoal++)
	{
		g_aBoal[nCntBoal].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoal[nCntBoal].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoal[nCntBoal].bUse = false;
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &g_pTextureBoal);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_Boal,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoal,
		NULL);

	VERTEX_3D*pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBoal->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBoal = 0; nCntBoal < MAX_Boal; nCntBoal++)
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(-10, 10, 0);
		pVtx[1].pos = D3DXVECTOR3(10, 10, 0);
		pVtx[2].pos = D3DXVECTOR3(-10, -10, 0);
		pVtx[3].pos = D3DXVECTOR3(10, -10, 0);

		pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0, 0);
		pVtx[1].tex = D3DXVECTOR2(1, 0);
		pVtx[2].tex = D3DXVECTOR2(0, 1);
		pVtx[3].tex = D3DXVECTOR2(1, 1);

		// �ʒu�E�����̏����ݒ�
		g_aBoal[nCntBoal].pos = D3DXVECTOR3(0, 30, 0);
		g_aBoal[nCntBoal].rot = D3DXVECTOR3(0, 0, 0);

		pVtx += 4;
	}
	//�A�����b�N
	g_pVtxBuffBoal->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitBoal(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureBoal != NULL)
	{
		g_pTextureBoal->Release();
		g_pTextureBoal = NULL;
	}
	//���_�̔j��
	if (g_pVtxBuffBoal != NULL)
	{
		g_pVtxBuffBoal->Release();
		g_pVtxBuffBoal = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBoal(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBoal(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans, mtxView;
	int nCntBoal;

	pDevice = GetDevice();	// �f�o�C�X�̎擾

	for (nCntBoal = 0; nCntBoal < MAX_Boal; nCntBoal++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aBoal[nCntBoal].mtxWorld);

		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		g_aBoal[nCntBoal].mtxWorld._11 = mtxView._11;
		g_aBoal[nCntBoal].mtxWorld._12 = mtxView._21;
		g_aBoal[nCntBoal].mtxWorld._13 = mtxView._31;
		g_aBoal[nCntBoal].mtxWorld._21 = mtxView._12;
		g_aBoal[nCntBoal].mtxWorld._22 = mtxView._22;
		g_aBoal[nCntBoal].mtxWorld._23 = mtxView._32;
		g_aBoal[nCntBoal].mtxWorld._31 = mtxView._13;
		g_aBoal[nCntBoal].mtxWorld._32 = mtxView._23;
		g_aBoal[nCntBoal].mtxWorld._33 = mtxView._33;


		//// �����𔽉f
		//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBoal[nCntBoal].rot.y, g_aBoal[nCntBoal].rot.x, g_aBoal[nCntBoal].rot.z);

		//D3DXMatrixMultiply(&g_aBoal[nCntBoal].mtxWorld, &g_aBoal[nCntBoal].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aBoal[nCntBoal].pos.x, g_aBoal[nCntBoal].pos.y, g_aBoal[nCntBoal].pos.z);

		D3DXMatrixMultiply(&g_aBoal[nCntBoal].mtxWorld, &g_aBoal[nCntBoal].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aBoal[nCntBoal].mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffBoal, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureBoal);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBoal, 2);
	}
}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexBoal(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�


	// ���_���̐ݒ�

}
