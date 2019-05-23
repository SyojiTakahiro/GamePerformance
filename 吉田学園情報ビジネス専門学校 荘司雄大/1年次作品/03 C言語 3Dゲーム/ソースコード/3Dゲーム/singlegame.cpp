//=============================================================================
//
// 入力処理 [input.cpp]
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
// グローバル変数
//*****************************************************************************
SINGLEGAMESTATE g_GameState = SINGLEGAMESTATE_NONE;
int g_nCounterSingleGameState;

bool g_bPause1;

SELECTMODE	*g_Pause1;
//=============================================================================
// 初期化処理
//=============================================================================
void InitSingleGame(void)
{
	//背景の初期化処理
	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// ポリゴンの初期化処理
	InitMeshField();

	// 壁の初期化処理
	InitMeshWall();

	// 影の初期化処理
	InitShadow();

	// ポリゴンの初期化処理
	InitBullet();

	InitParticle();

	InitExplosion();

	// モデルの初期化処理
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
// 終了処理
//=============================================================================
void UninitSingleGame(void)
{

	//キーボードの終了処理
	/*UninitKeyboard();*/

	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// ポリゴンの終了処理
	UninitMeshField();

	// 壁の終了処理
	UninitMeshWall();

	// 影の終了処理
	UninitShadow();

	// ポリゴンの終了処理
	UninitBullet();

	UninitParticle();

	UninitExplosion();

	// モデルの終了処理
	UninitPlayer();
	UninitCPU();

	UninitBuilding();

	UninitScore();

	UninitLife();

	UninitCPULife();

	UninitPause();
}
//=============================================================================
// 更新処理
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
		// カメラの更新処理
		UpdateCamera();

		// ライトの更新処理
		UpdateLight();

		// ポリゴンの更新処理
		UpdateMeshField();

		// 壁の更新処理

		UpdateMeshWall();

		// 影の更新処理
		UpdateShadow();

		// ポリゴンの更新処理
		UpdateBullet();

		UpdateParticle();

		UpdateExplosion();

		// モデルの更新処理
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
// 描画処理
//=============================================================================
void DrawSingleGame(void)
{
	// カメラの設定
	SetCamera();

	// ポリゴンの描画処理);
	DrawBuilding();

	DrawMeshField();

	// 壁の描画処理
	DrawMeshWall();

	// 影の描画処理
	DrawShadow();

	// ポリゴンの描画処理

	DrawExplosion();
	// モデルの描画処理
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