//=============================================================================
//
// TITLE [title.cpp]
// Author :Takahiro
//
//=============================================================================
#include "title.h"
#include "titlelogo.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "pressenter.h"
#include "rank.h"
#include "ranking.h"
#include "rankingbg.h"
#include "score.h"
#include "camera.h"
#include "light.h"
#include "meshField.h"
#include "building.h"
#include "modeselect.h"
#include "selecttitle.h"
#include "model.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TITLE_TEXTURENAME		"data/TEXTURE/title.jpg"			//�w�i�摜
#define TITLE_POS_X			(0)					//X���W
#define TITLE_POS_Y			(0)					//Y���W
#define TITLE_WIDTH			(SCREEN_WIDTH)		//��ʂ̕�
#define TITLE_HEIGHT		(SCREEN_HEIGHT)		//��ʂ̍���

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;

//GAMEMODESELECT g_GameState = GAMEMODESELECT_NONE;

int g_nCntRankingTimer = 0;

bool g_bModeSelect;

GAMEMODESELECT	*g_ModeSelect;
//=============================================================================
// ���C���֐�
//=============================================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAME, &g_pTextureTitle);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, TITLE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, TITLE_POS_Y + TITLE_HEIGHT, 0.0f);

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
	g_pVtxBuffTitle->Unlock();

	// �J�����̏���������
	InitCamera();
	// ���C�g�̏���������
	InitLight();

	InitBuilding();

	InitModel();

	InitTitleLogo();

	InitMeshField();

	InitPressEnter();

	InitSelectTitle();

	SetModel(D3DXVECTOR3(200, 0, -160), 0);

	SetModel(D3DXVECTOR3(280, 0, -160), 1);

	g_nCntRankingTimer = 0;

	g_bModeSelect = false;

	PlaySound(SOUND_LABEL_BGM000);
}

void UninitTitle(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}
	//���_�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	// �J�����̏I������
	UninitCamera();
	// ���C�g�̏I������
	UninitLight();

	UninitBuilding();

	UninitModel();

	UninitTitleLogo();

	UninitSelectTitle();

	UninitMeshField();

	UninitPressEnter();
}

void UpdateTitle(void)
{
	FADE fade;
	fade = GetFade();
	g_nCntRankingTimer++;

	//�G���^�[�L�[
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		PlaySound(SOUND_LABEL_SE_PAUSE);
		g_bModeSelect = g_bModeSelect ? false : true;
	}
	g_ModeSelect = GetModeMode();

	//if (GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE)
	//{
	//	UpdateSelectTitle();
	//	/*SetFade(MODE_GAME);
	//	StopSound(SOUND_LABEL_BGM000);
	//	PlaySound(SOUND_LABEL_SE_DECIDE);*/
	//}

	/*if (GetKeyboardTrigger(DIK_O) == true && fade == FADE_NONE)
	{
		SetFade(MODE_SINGLEGAME);
		StopSound(SOUND_LABEL_BGM000);
		PlaySound(SOUND_LABEL_SE_DECIDE);
	}

	if (GetKeyboardTrigger(DIK_P) == true && fade == FADE_NONE)
	{
		SetFade(MODE_T);
		StopSound(SOUND_LABEL_BGM000);
		PlaySound(SOUND_LABEL_SE_DECIDE);
	}*/

	/*if (g_ScreenChangeCnt >= 800)
	{
		if (g_ScreenChange == 0)
		{
			g_ScreenChange = 1;
		}
		else if (g_ScreenChange == 1)
		{
			g_ScreenChange = 0;
		}

		UpdateRankingBG();

		g_ScreenChangeCnt = 0;
	}
	UpdatePressEnter();*/

	/*if (g_nCntRankingTimer >= 600)
	{
		SetScore(0);
		SetFade(MODE_RANKING);
		g_nCntRankingTimer = 0;
	}
*/

	// �J�����̍X�V����
	UpdateCamera();
	// ���C�g�̍X�V����
	UpdateLight();

	UpdateBuilding();

	UpdateModel();

	UpdateMeshField();
	UpdateTitleLogo();
	UpdatePressEnter();

	if (g_bModeSelect == false)
	{
		
	}
	else
	{
		UpdateSelectTitle();
		if (*g_ModeSelect == GAMEMODESELECT_SINGLEMODE)
		{
			g_bModeSelect = g_bModeSelect ? false : true;
			*g_ModeSelect = GAMEMODESELECT_NONE;
		}
	}

}

void DrawTitle(void)
{

	// �J�����̐ݒ�
	SetCamera();

	////�f�o�C�X�̎擾
	//pDevice = GetDevice();

	//pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//pDevice->SetTexture(0, g_pTextureTitle);

	//pDevice->SetFVF(FVF_VERTEX_2D);
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	DrawBuilding();

	DrawModel();

	DrawMeshField();

	DrawTitleLogo();

	DrawPressEnter();

	if (g_bModeSelect == true)
	{
		DrawSelectTitle();
		DrawModeSelect();
	}
}