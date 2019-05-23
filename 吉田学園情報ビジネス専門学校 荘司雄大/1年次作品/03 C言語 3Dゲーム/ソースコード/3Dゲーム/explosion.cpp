//=============================================================================
//
// �������� [explosion.cpp]
// Author :
//
//=============================================================================
#include "explosion.h"
#include "Explosion.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_EXPLOSION		(256)		// �e�̍ő吔
#define EXPLOSION_WIDTH		(50)		//�����̕�
#define EXPLOSION_HEIGHT	(50)		//�����̍���
#define POLYGON_TEXTURENAME		"data/TEXTURE/explosion000.png"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
D3DXVECTOR3 pos;
D3DXCOLOR col;
D3DXMATRIX mtxWorld;
int nCounterAnim;	
int nPatternAnim;	
bool bUse;
}EXPLOSION;

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
EXPLOSION g_aExplosion[MAX_EXPLOSION];
D3DXVECTOR3 g_posExplosion;
//=============================================================================
// ����������
//=============================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�̏��̏�����
	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//g_posExplosion = D3DXVECTOR3(600, 600, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME, &g_pTextureExplosion);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffExplosion,
								NULL);

	VERTEX_3D*pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		pVtx[0].pos = D3DXVECTOR3(-1, 1, 0);
		pVtx[1].pos = D3DXVECTOR3(1, 1, 0);
		pVtx[2].pos = D3DXVECTOR3(-1, -1, 0);
		pVtx[3].pos = D3DXVECTOR3(1, -1, 0);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0, 0);
		pVtx[1].tex = D3DXVECTOR2(1, 0);
		pVtx[2].tex = D3DXVECTOR2(0, 1);
		pVtx[3].tex = D3DXVECTOR2(1, 1);
	
		pVtx += 4;//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	//�A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitExplosion(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateExplosion(void)
{
	int nCntExplosion;
	VERTEX_3D*pVtx;

	/*if (g_aExplosion[nCntExplosion].nCounterAnim %10)== 0)
	{
		g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % 10
	}*/

	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].nCounterAnim++;
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
		/*	g_aExplosion[nCntExplosion].pos.y -= 10.0f;
			g_posExplosion.y -= 10.0f;*/
			//pVtx += (nCntExplosion * 4);   //�Y���̈ʒu�܂Ői�߂�

				g_aExplosion[nCntExplosion].nPatternAnim++;
				if (g_aExplosion[nCntExplosion].nPatternAnim > 7)
				{
					g_aExplosion[nCntExplosion].bUse = false;
				}
			//���_�o�b�t�@�����b�N
			//g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
			pVtx[nCntExplosion*4].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0.0f);
			pVtx[nCntExplosion*4+1].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f + 0.125f, 0.0f);
			pVtx[nCntExplosion*4+2].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0.0f + 1.0f);
			pVtx[nCntExplosion*4+3].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f + 0.125f, 0.0f + 1.0f);
			//�A�����b�N
			//g_pVtxBuffExplosion->Unlock();
			
		}
	}
	//if ((g_aExplosion[nCntExplosion].nCounterAnim % 3) == 0)
	//{
	//	g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % 8;
	//}

	g_pVtxBuffExplosion  ->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans, mtxView;
	int nCntExplosion;

	pDevice = GetDevice();	// �f�o�C�X�̎擾

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aExplosion[nCntExplosion].mtxWorld);

		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		g_aExplosion[nCntExplosion].mtxWorld._11 = mtxView._11;
		g_aExplosion[nCntExplosion].mtxWorld._12 = mtxView._21;
		g_aExplosion[nCntExplosion].mtxWorld._13 = mtxView._31;
		g_aExplosion[nCntExplosion].mtxWorld._21 = mtxView._12;
		g_aExplosion[nCntExplosion].mtxWorld._22 = mtxView._22;
		g_aExplosion[nCntExplosion].mtxWorld._23 = mtxView._32;
		g_aExplosion[nCntExplosion].mtxWorld._31 = mtxView._13;
		g_aExplosion[nCntExplosion].mtxWorld._32 = mtxView._23;
		g_aExplosion[nCntExplosion].mtxWorld._33 = mtxView._33;


		//// �����𔽉f
		//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aExplosion[nCntExplosion].rot.y, g_aExplosion[nCntExplosion].rot.x, g_aExplosion[nCntExplosion].rot.z);

		//D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntExplosion].pos.x, g_aExplosion[nCntExplosion].pos.y, g_aExplosion[nCntExplosion].pos.z);

		D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntExplosion].mtxWorld);

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureExplosion);

		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// �|���S���̕`��
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntExplosion * 4,//�`����J�n���钸�_�C���f�b�N�X
				2);
		}
	}
	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion ;
	VERTEX_3D*pVtx;
	
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{
			g_aExplosion[nCntExplosion].pos = pos;
			pVtx[0].pos = D3DXVECTOR3(-(EXPLOSION_WIDTH / 2), (EXPLOSION_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((EXPLOSION_WIDTH / 2), (EXPLOSION_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(EXPLOSION_WIDTH / 2), -(EXPLOSION_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((EXPLOSION_WIDTH / 2), -(EXPLOSION_HEIGHT / 2), 0.0f);
			g_aExplosion[nCntExplosion].col = col;
			
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}