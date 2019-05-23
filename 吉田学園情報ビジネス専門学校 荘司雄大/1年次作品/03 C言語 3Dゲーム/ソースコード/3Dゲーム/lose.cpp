//=============================================================================
//
// Lose [Lose.cpp]
// Author :Takahiro
//
//=============================================================================
#include "lose.h"
#include "p2winlogo.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "meshField.h"
#include "building.h"
#include "player2.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define RESULT_TEXTURENAME		"data/TEXTURE/GC.jpg"			//�w�i�摜
#define RESULT_POS_X			(0)					//X���W
#define RESULT_POS_Y			(0)					//Y���W
#define RESULT_WIDTH			(SCREEN_WIDTH)		//��ʂ̕�
#define RESULT_HEIGHT			(SCREEN_HEIGHT)		//��ʂ̍���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureLose = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLose = NULL;

//=============================================================================
// ���C���֐�
//=============================================================================
void InitLose(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME, &g_pTextureLose);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLose,
		NULL);

	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffLose->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(RESULT_POS_X + RESULT_WIDTH, RESULT_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y + RESULT_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(RESULT_POS_X + RESULT_WIDTH, RESULT_POS_Y + RESULT_HEIGHT, 0.0f);

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
	g_pVtxBuffLose->Unlock();

	// �J�����̏���������
	InitCamera();
	// ���C�g�̏���������
	InitLight();

	InitBuilding();

	SetBuilding(D3DXVECTOR3(250, 0, -160), 1);

	InitMeshField();

	//InitP2winLogo();

	PlaySound(SOUND_LABEL_BGM002);
}

void UninitLose(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureLose != NULL)
	{
		g_pTextureLose->Release();
		g_pTextureLose = NULL;
	}
	//���_�̔j��
	if (g_pVtxBuffLose != NULL)
	{
		g_pVtxBuffLose->Release();
		g_pVtxBuffLose = NULL;
	}

	// �J�����̏I������
	UninitCamera();
	// ���C�g�̏I������
	UninitLight();

	UninitBuilding();

	UninitMeshField();

	//UninitP2winLogo();
}

void UpdateLose(void)
{
	FADE fade;
	fade = GetFade();

	if (GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE)
	{
		StopSound(SOUND_LABEL_BGM002);
		SetFade(MODE_TITLE);
	}
	// �J�����̍X�V����
	UpdateCamera();
	// ���C�g�̍X�V����
	UpdateLight();

	UpdateBuilding();

	UpdateMeshField();

	//UpdateP2winLogo();
}

void DrawLose(void)
{
	// �J�����̐ݒ�
	SetCamera();

	////�f�o�C�X�̎擾
	//pDevice = GetDevice();

	//pDevice->SetStreamSource(0, g_pVtxBuffLose, 0, sizeof(VERTEX_2D));

	//pDevice->SetTexture(0, g_pTextureLose);

	//pDevice->SetFVF(FVF_VERTEX_2D);
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	DrawBuilding();

	DrawMeshField();

	//DrawP2winLogo();
}