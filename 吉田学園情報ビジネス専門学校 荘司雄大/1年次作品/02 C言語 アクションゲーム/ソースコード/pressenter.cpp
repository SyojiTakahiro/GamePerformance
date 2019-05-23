//=============================================================================
// �w�i�̏��� [pressenter.cpp]
// Author : Takahiro
//=============================================================================
#include "main.h"
#include "pressenter.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_TEXTURENAME		"data/TEXTURE/press_enter.png"	//�e�N�X�`���̃t�@�C����
#define POS_X				(450)							//������WX
#define POS_Y				(450)							//������WY	
#define BG_WIDTH			(800)				//�����`�̉������W
#define BG_HEIGHT			(510)				//�����`�̏c�����W
#define TEX_POS_X_INIT		(1.0f)						//�e�N�X�`�����WU�̏����ʒu
#define TEX_POS_Y_INIT		(0.005f)					//�e�N�X�`�����WV�̏����ʒu
#define TEX_LEFT			(0.0f)						//�e�N�X�`�����WU��
#define TEX_RIGHT			(1.0f)						//�e�N�X�`�����WU�E
#define TEX_TOP				(0.0f)						//�e�N�X�`�����WV��
#define TEX_BOT				(1.0f)						//�e�N�X�`�����WV��

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePressEnter = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPressEnter = NULL;	//���_�o�b�t�@�ւ̃|�C���^
int						g_PressEnterCnt = 0;
float					g_PressEnterChange = 0;
//*****************************************************************************
// �|���S��
//*****************************************************************************
void InitPressEnter(void)
{	//���[�J���ϐ�
	VERTEX_2D *pVtx;//���_���ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		BG_TEXTURENAME,
		&g_pTexturePressEnter);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPressEnter,
		NULL);

	//���_�o�b�t�@�����b�N�����_�f�[�^�̃|�C���^���擾
	g_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);
	//���_���W
	pVtx[0].pos = D3DXVECTOR3(POS_X, POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH, POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(POS_X, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);
	//�e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(TEX_LEFT, TEX_TOP);
	pVtx[1].tex = D3DXVECTOR2(TEX_RIGHT, TEX_TOP);
	pVtx[2].tex = D3DXVECTOR2(TEX_LEFT, TEX_BOT);
	pVtx[3].tex = D3DXVECTOR2(TEX_RIGHT, TEX_BOT);
	//���_
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPressEnter->Unlock();
	//���l������
	g_PressEnterCnt = 0;
	g_PressEnterChange = 0;
}
//=============================================================================
// �I������
//=============================================================================
void UninitPressEnter(void)
{	//�e�N�X�`���̔j��
	if (g_pTexturePressEnter != NULL)
	{
		g_pTexturePressEnter->Release();
		g_pTexturePressEnter = NULL;
	}
	//���_�o�b�t�@�̔j���̔j��
	if (g_pVtxBuffPressEnter != NULL)
	{
		g_pVtxBuffPressEnter->Release();
		g_pVtxBuffPressEnter = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdatePressEnter(void)
{
	g_PressEnterCnt++;

	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^
	//���_�o�b�t�@�����b�N�����_�f�[�^�̃|�C���^���擾
	g_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);
	
	if (g_PressEnterCnt >= 40)
	{
		if (g_PressEnterChange == 0)
		{
			g_PressEnterChange = 1;
		}
		else if (g_PressEnterChange == 1)
		{
			g_PressEnterChange = 0;
		}
		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_PressEnterChange);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_PressEnterChange);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_PressEnterChange);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_PressEnterChange);

		g_PressEnterCnt = 0;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPressEnter->Unlock();
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawPressEnter(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾����
	pDevice = GetDevice();
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPressEnter, 0, sizeof(VERTEX_2D));
	//���_�t�H�[�}�b�g��ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePressEnter);
	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//�J�n���钸�_�̃C���f�b�N�X
		2); //�`�悷��v���~�e�B�u��
}