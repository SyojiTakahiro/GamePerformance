//=============================================================================
//
// ���͏��� [input.cpp]
// Author :
//
//=============================================================================
#include "main.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "player.h"
#include "cpu.h"
#include "shadow.h"
#include "bullet.h"
#include "meshField.h"
#include "meshWall.h"
#include "particle.h"
#include "singlegame.h"
#include "pause.h"
#include "pauseselect.h"
#include "fade.h"
#include "sound.h"
#include "p1win.h"
#include "p2win.h"
#include "building.h"
#include "explosion.h"
#include "rank.h"
#include "ranking.h"
#include "rankingbg.h"
#include "score.h"
#include "life.h"
#include "cpulife.h"
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
SINGLEGAMESTATE g_GameState = SINGLEGAMESTATE_NONE;
int g_nCounterSingleGameState;

bool g_bPause1;

SELECTMODE	*g_Pause1;
//=============================================================================
// ����������
//=============================================================================
void InitSingleGame(void)
{
	//�w�i�̏���������
	// �J�����̏���������
	InitCamera();

	// ���C�g�̏���������
	InitLight();

	// �|���S���̏���������
	InitMeshField();

	// �ǂ̏���������
	InitMeshWall();

	// �e�̏���������
	InitShadow();

	// �|���S���̏���������
	InitBullet();

	InitParticle();

	InitExplosion();

	// ���f���̏���������
	InitPlayer();
	InitCPU();

	InitBuilding();

	InitScore();

	InitLife();

	InitCPULife();

	InitPause();

	/*SetBuilding(D3DXVECTOR3(100,0,0),0);

	SetBuilding(D3DXVECTOR3(400, 0, 0), 0);*/

	g_GameState = SINGLEGAMESTATE_NORMAL;
	g_nCounterSingleGameState = 0;
	g_bPause1 = false;
	PlaySound(SOUND_LABEL_BGM001);
}
//=============================================================================
// �I������
//=============================================================================
void UninitSingleGame(void)
{

	//�L�[�{�[�h�̏I������
	/*UninitKeyboard();*/

	// �J�����̏I������
	UninitCamera();

	// ���C�g�̏I������
	UninitLight();

	// �|���S���̏I������
	UninitMeshField();

	// �ǂ̏I������
	UninitMeshWall();

	// �e�̏I������
	UninitShadow();

	// �|���S���̏I������
	UninitBullet();

	UninitParticle();

	UninitExplosion();

	// ���f���̏I������
	UninitPlayer();
	UninitCPU();

	UninitBuilding();

	UninitScore();

	UninitLife();

	UninitCPULife();

	UninitPause();
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateSingleGame(void)
{
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		PlaySound(SOUND_LABEL_SE_PAUSE);
		g_bPause1 = g_bPause1 ? false : true;
	}
	g_Pause1 = GetPauseMode();

	switch (g_GameState)
	{
	case SINGLEGAMESTATE_NORMAL:
		break;
	case SINGLEGAMESTATE_P1WIN:
		g_nCounterSingleGameState++;
		if (g_nCounterSingleGameState >= 60)
		{
			g_GameState = SINGLEGAMESTATE_NONE;
			StopSound(SOUND_LABEL_BGM001);
			//
			SetFade(MODE_TITLE);
		}
	case SINGLEGAMESTATE_P2WIN:
		g_nCounterSingleGameState++;
		if (g_nCounterSingleGameState >= 60)
		{
			g_GameState = SINGLEGAMESTATE_NONE;
			StopSound(SOUND_LABEL_BGM001);
			//
			SetFade(MODE_LOSE);
		}
		break;
	}

	if (g_bPause1 == false)
	{
		// �J�����̍X�V����
		UpdateCamera();

		// ���C�g�̍X�V����
		UpdateLight();

		// �|���S���̍X�V����
		UpdateMeshField();

		// �ǂ̍X�V����

		UpdateMeshWall();

		// �e�̍X�V����
		UpdateShadow();

		// �|���S���̍X�V����
		UpdateBullet();

		UpdateParticle();

		UpdateExplosion();

		// ���f���̍X�V����
		UpdatePlayer();
		UpdateCPU();

		UpdateBuilding();

		UpdateLife();

		UpdateCPULife();

		UpdateScore();
	}
	else
	{
		UpdatePause();
		if (*g_Pause1 == SELECTMODE_CONTINUE)
		{
			g_bPause1 = g_bPause1 ? false : true;
			*g_Pause1 = SELECTMODE_NONE;
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawSingleGame(void)
{
	// �J�����̐ݒ�
	SetCamera();

	// �|���S���̕`�揈��);
	DrawBuilding();

	DrawMeshField();

	// �ǂ̕`�揈��
	DrawMeshWall();

	// �e�̕`�揈��
	DrawShadow();

	// �|���S���̕`�揈��

	DrawExplosion();
	// ���f���̕`�揈��
	DrawPlayer();
	DrawCPU();

	DrawBullet();

	DrawLife();

	DrawCPULife();

	DrawParticle();

	//DrawScore();

	if (g_bPause1 == true)
	{
		DrawPause();
		DrawPauseSelect();
	}
}

void SetSingleGameState(SINGLEGAMESTATE state)
{
	g_GameState = state;
	g_nCounterSingleGameState = 0;
}

SINGLEGAMESTATE GetSingleGameState(void)
{
	return g_GameState;
}