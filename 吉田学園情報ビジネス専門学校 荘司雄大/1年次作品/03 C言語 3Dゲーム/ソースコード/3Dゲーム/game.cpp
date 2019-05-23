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
// グローバル変数
//*****************************************************************************
GAMESTATE g_GameState = GAMESTATE_NONE;
int g_nCounterGameState;

bool g_bPause;

SELECTMODE	*g_Pause;
//=============================================================================
// 初期化処理
//=============================================================================
void InitGame(void)
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
// 終了処理
//=============================================================================
void UninitGame(void)
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
	UninitPlayer2();

	UninitBuilding();

	UninitScore();

	UninitLife();

	UninitLife2();

	UninitPause();
}
//=============================================================================
// 更新処理
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
// 描画処理
//=============================================================================
void DrawGame(void)
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