//=============================================================================
//
// �������� [explosion.cpp]
// Author :
//
//=============================================================================
#include "explosion.h"
#include "bullet.h"

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
int nCounterAnim;	
int nPatternAnim;	
bool bUse;
}Explosion;

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION];
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
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//g_posExplosion = D3DXVECTOR3(600, 600, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME, &g_pTextureExplosion);

	// ���_���̍쐬
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffExplosion,
								NULL);

	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 200, 200, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 200, 200, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 200, 200, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 200, 200, 255);
	
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
	VERTEX_2D*pVtx;

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
	int nCntExplosion;
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	// �|���S���̕`��
	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntExplosion, 2);
		}
	}
}

//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion ;
	VERTEX_2D*pVtx;
	
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{
			g_aExplosion[nCntExplosion].pos = pos;
			pVtx[0].pos = D3DXVECTOR3(-(EXPLOSION_WIDTH / 2), -(EXPLOSION_HEIGHT / 2), 0.0f) + g_aExplosion[nCntExplosion].pos;
			pVtx[1].pos = D3DXVECTOR3((EXPLOSION_WIDTH / 2), -(EXPLOSION_HEIGHT / 2), 0.0f) + g_aExplosion[nCntExplosion].pos;
			pVtx[2].pos = D3DXVECTOR3(-(EXPLOSION_WIDTH / 2), (EXPLOSION_HEIGHT / 2), 0.0f) + g_aExplosion[nCntExplosion].pos;
			pVtx[3].pos = D3DXVECTOR3((EXPLOSION_WIDTH / 2), (EXPLOSION_HEIGHT / 2), 0.0f) + g_aExplosion[nCntExplosion].pos;
			g_aExplosion[nCntExplosion].col = col;
			
			pVtx[0].col = D3DCOLOR_RGBA(255, 69, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 69, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 69, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 69, 0, 255);

			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}