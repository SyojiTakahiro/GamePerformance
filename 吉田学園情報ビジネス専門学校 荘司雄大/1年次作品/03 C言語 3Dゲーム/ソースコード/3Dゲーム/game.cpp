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
#include "player2.h"
#include "shadow.h"
#include "bullet.h"
#include "meshField.h"
#include "meshWall.h"
#include "particle.h"
#include "game.h"
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
#include "life2.h"
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMESTATE g_GameState = GAMESTATE_NONE;
int g_nCounterGameState;

bool g_bPause;

SELECTMODE	*g_Pause;
//=============================================================================
// ����������
//=============================================================================
void InitGame(void)
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
	InitPlayer2();

	InitBuilding();

	InitScore();

	InitLife();

	InitLife2();

	InitPause();

	/*SetBuilding(D3DXVECTOR3(100,0,0),0);

	SetBuilding(D3DXVECTOR3(400, 0, 0), 0);*/

	g_GameState = GAMESTATE_NORMAL;
	g_nCounterGameState = 0;
	g_bPause = false;
	PlaySound(SOUND_LABEL_BGM001);
}
//=============================================================================
// �I������
//=============================================================================
void UninitGame(void)
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
	UninitPlayer2();

	UninitBuilding();

	UninitScore();

	UninitLife();

	UninitLife2();

	UninitPause();
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		PlaySound(SOUND_LABEL_SE_PAUSE);
		g_bPause = g_bPause ? false : true;
	}
	g_Pause = GetPauseMode();

	switch (g_GameState)
	{
		case GAMESTATE_NORMAL:
			break;
		case GAMESTATE_P1WIN:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 60)
			{
				g_GameState = GAMESTATE_NONE;
				StopSound(SOUND_LABEL_BGM001);
				//
				SetFade(MODE_P1WIN);
			}
		case GAMESTATE_P2WIN:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 60)
			{
				g_GameState = GAMESTATE_NONE;
				StopSound(SOUND_LABEL_BGM001);
				//
				SetFade(MODE_P2WIN);
			}
		case GAMESTATE_LOSE:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 60)
			{
				g_GameState = GAMESTATE_NONE;
				StopSound(SOUND_LABEL_BGM001);
				//
				SetFade(MODE_LOSE);
			}
		break;
	}
	
	if(g_bPause == false)
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
		UpdatePlayer2();

		//UpdateBuilding();

		UpdateLife();

		UpdateLife2();

		UpdateScore();
	}
	else
	{
		UpdatePause();
		if (*g_Pause == SELECTMODE_CONTINUE)
		{
			g_bPause = g_bPause ? false : true;
			*g_Pause = SELECTMODE_NONE;
		}
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
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
	DrawPlayer2();

	DrawBullet();

	DrawLife();

	DrawLife2();

	DrawParticle();

	//DrawScore();
	
	if (g_bPause == true)
	{
		DrawPause();
		DrawPauseSelect();
	}
}

void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	g_nCounterGameState = 0;
}

GAMESTATE GetGameState(void)
{
	return g_GameState;
}