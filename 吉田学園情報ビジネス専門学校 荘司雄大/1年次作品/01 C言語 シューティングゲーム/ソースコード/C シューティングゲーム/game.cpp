//=============================================================================
//
// ���͏��� [input.cpp]
// Author :
//
//=============================================================================
#include "game.h"
#include "bg.h"
#include "bg2.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "effect.h"
#include "input.h"
#include "main.h"
#include "player.h"
#include "fade.h"
#include "sound.h"
#include "pause.h"

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
GAMESTATE g_GameState = GAMESTATE_NONE;
int g_nCounterGameState;

bool g_bPause;
//=============================================================================
// ����������
//=============================================================================
void InitGame(void)
{
	//�w�i�̏���������
	InitBG();
	
	InitBG2();

	InitBullet();

	InitEffect();

	InitExplosion();

	InitEnemy();

	InitPause();
	//�G�̔z�u
	SetEnemy(D3DXVECTOR3(1000, 350, 0.0f), 4);


	//�|���S���̏���������
	InitPlayer();

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
	/*UninitKeyboard();*/
	UninitBG();
	UninitBG2();
	UninitBullet();
	UninitEffect();
	UninitExplosion();
	UninitEnemy();
	UninitPlayer();
	UninitPause();
}
//=============================================================================
// �X�V����
//=============================================================================
void UpdateGame(void)
{
	/*UpdateKeyboard();*/
	/*UpdateBG();
	UpdateBullet();
	UpdateEffect();
	UpdateExplosion();
	UpdateEnemy();
	UpdatePlayer();*/

	switch (g_GameState)
	{
		case GAMESTATE_NORMAL:
			break;
		case GAMESTATE_END:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 60)
			{
				g_GameState = GAMESTATE_NONE;
				StopSound(SOUND_LABEL_BGM001);
				//
				SetFade(MODE_RESULT);
			}
		case GAMESTATE_CLEAR:
			g_nCounterGameState++;
			if (g_nCounterGameState >= 60)
			{
				g_GameState = GAMESTATE_NONE;
				StopSound(SOUND_LABEL_BGM001);
				//
				SetFade(MODE_CLEAR);
			}
		break;
	}
	if (GetKeyboardTrigger(DIK_P) == true)
	{
		g_bPause = (g_bPause ? false : true);
	}
	if(g_bPause == false)
	{
		UpdateBG();
		UpdateBG2();
		UpdateBullet();
		UpdateEffect();
		UpdateExplosion();
		UpdateEnemy();
		UpdatePlayer();
	}
	else
	{
		UpdatePause();
	}
}
//=============================================================================
// �`�揈��
//=============================================================================
void DrawGame(void)
{
	DrawBG();

	DrawBG2();

	DrawBullet();

	DrawEffect();

	DrawEnemy();
	
	DrawPlayer();
	
	DrawExplosion();
	
	if (g_bPause == true)
	{
		DrawPause();
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