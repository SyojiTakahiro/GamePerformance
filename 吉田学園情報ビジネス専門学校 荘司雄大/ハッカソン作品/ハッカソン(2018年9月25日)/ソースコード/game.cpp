//=============================================================================
//
// ゲーム画面処理 [game.cpp]
// Author : Niwa Hodaka
//
//=============================================================================
#include "game.h"
#include "fade.h"
#include "result.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "pause.h"
#include "pauseselect.h"
#include "pauselogo.h"
#include "rankingscore.h"
#include "sound.h"


#include "sky.h"
#include "field.h"
#include "shadow.h"
#include "player.h"
#include "effect.h"
#include "lifegauge.h"
#include "baselifegauge.h"
#include "coverlife.h"
#include "damagegauge.h"
#include "ultgauge.h"
#include "timer.h"
#include "model.h"
#include "Rush.h"

//=============================================================================
// 構造体定義
//=============================================================================

//=============================================================================
// プロトタイプ宣言
//=============================================================================

//=============================================================================
// グローバル変数宣言
//=============================================================================
GAMESTATE g_GameState = GAMESTATE_NONE;   // ゲームの状態
int       g_nCounterGameState;            // ゲームの状態管理カウンター
bool      g_bPuase;                       // ポーズ中かどうか

//=============================================================================
// ゲーム画面初期化処理
//=============================================================================
void InitGame(void)
{
	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// 空の初期化処理
	InitSky();

	// 地面の初期化処理
	InitField();

	// 影の初期化処理
	InitShadow();

	// プレイヤーの初期化処理
	InitPlayer();

	// エフェクトの初期化処理
	InitEffect();

	// 体力ゲージの初期化処理
	InitLifeGauge();

	// 体力ゲージ(ベース)の更新処理
	InitBaseLifeGauge();

	// 体力ゲージ(カバー)の初期化処理
	InitCoverLife();

	// 体力ゲージ(ダメージ)の初期化処理
	InitDamageGauge();

	// 必殺ゲージの初期化処理
	InitUltGauge();

	// タイマーの初期化処理
	InitTimer();

	// モデルの初期化処理
	InitModel();

	InitRush();

	// ポーズ初期化処理
	InitPause();

	// ポーズロゴ初期化処理
	InitPauseLogo();

	// ポーズ項目初期化処理
	InitPauseSelect();

	g_GameState = GAMESTATE_NORMAL;
	g_nCounterGameState = 0;
	g_bPuase = false;
}
//=============================================================================
// ゲーム画面終了処理
//=============================================================================
void UninitGame(void)
{
	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// 空の終了処理
	UninitSky();

	// モデルの終了処理
	UninitModel();

	// 地面の終了処理
	UninitField();

	// 影の終了処理
	UninitShadow();

	// プレイヤーの終了処理
	UninitPlayer();

	// エフェクトの終了処理
	UninitEffect();

	// 体力ゲージ(ベース)の更新処理
	UninitBaseLifeGauge();

	// 体力ゲージの終了処理
	UninitLifeGauge();

	// 体力ゲージ(カバー)の終了処理
	UninitCoverLife();

	// 体力ゲージ(ダメージ)の終了処理
	UninitDamageGauge();

	// 必殺ゲージの終了処理
	UninitUltGauge();

	// タイマーの終了処理
	UninitTimer();

	// モデルの終了処理
	UninitModel();

	UninitRush();

	// ポーズ終了処理
	UninitPause();

	// ポーズロゴ終了処理
	UninitPauseLogo();

	// ポーズ項目終了処理
	UninitPauseSelect();
}
//=============================================================================
// ゲーム画面更新処理
//=============================================================================
void UpdateGame(void)
{
	FADE fade;
	fade = GetFade();

	if (GetXInputTrigger(0, XIJS_BUTTON_4) == TRUE && fade == FADE_NONE && g_GameState == GAMESTATE_NORMAL || GetKeyboardTrigger(DIK_P) == true && fade == FADE_NONE && g_GameState == GAMESTATE_NORMAL)
	{// ポーズON/OFF
		g_bPuase = g_bPuase ? false : true;
	}

	if (g_bPuase == false)
	{// ポーズOFF
	    // カメラの更新処理
		UpdateCamera();

		// ライトの更新処理
		UpdateLight();

		// モデルの終了処理
		UpdateModel();

		// 空の更新処理
		UpdateSky();

		// 地面の更新処理
		UpdateField();

		// 影の更新処理
		UpdateShadow();

		// プレイヤーの更新処理
		UpdatePlayer();

		// エフェクトの更新処理
		UpdateEffect();

		// 体力ゲージ(ベース)の更新処理
		UpdateBaseLifeGauge();

		// 体力ゲージの更新処理
		UpdateLifeGauge();

		// 体力ゲージ(カバー)の更新処理
		UpdateCoverLife();

		// 体力ゲージ(ダメージ)の更新処理
		UpdateDamageGauge();

		// 必殺ゲージの更新処理
		UpdateUltGauge();

		// タイマーの更新処理
		UpdateTimer();

		// モデルの更新処理
		UpdateModel();

		UpdateRush();

#ifdef _DEBUG
		if (GetKeyboardTrigger(DIK_F2) && g_GameState == GAMESTATE_NORMAL)
		{// デバック用
			g_GameState = GAMESTATE_END;
		}
#endif

		switch (g_GameState)
		{
		case GAMESTATE_NORMAL:  // 通常状態
			break;

		case GAMESTATE_RETRY:   // やり直し状態
			g_nCounterGameState++;
			if (g_nCounterGameState >= 120)
			{// 一定時間たったら
				g_GameState = GAMESTATE_NONE_RETRY;
				SetFade(MODE_GAME);
			}
			break;

		case GAMESTATE_END:     //終了状態
			g_nCounterGameState++;
			if (g_nCounterGameState >= 120)
			{// 一定時間たったら
				g_GameState = GAMESTATE_NONE;
				SetFade(MODE_RESULT);
			}
			break;
		}
	}
	else
	{
		// ポーズ更新処理
		UpdatePause();

		// ポーズロゴ更新処理
		UpdatePauseLogo();

		// ポーズ項目更新処理
		UpdatePauseSelect();

		if (GetXInputTrigger(0,XIJS_BUTTON_11) == TRUE || GetKeyboardTrigger(DIK_RETURN) == true)
		{// 決定ボタンが押された
			RESTART restart;
			restart = GetRestart();
			FADE fade;
			fade = GetFade();

			if (fade == FADE_NONE)
			{// フェード状態でない
				switch (restart)
				{
				case RESTART_CONTINUE:  // ゲーム再開
					break;
				case RESTART_RETRY:     // ゲームやり直し
					g_GameState = GAMESTATE_NONE;
					SetFade(MODE_GAME);
					return; // これ以上ゲームはUpdateしない
					break;
				case RESTART_QUIT:      // タイトル遷移
					g_GameState = GAMESTATE_NONE;
					SetFade(MODE_TITLE);
					return; // これ以上ゲームはUpdateしない
					break;
				}
				g_bPuase = g_bPuase ? false : true; // ポーズは解除
			}
		}
	}
}
//=============================================================================
// ゲーム画面描画処理
//=============================================================================
void DrawGame(void)
{
	// カメラの設定
	SetCamera();

	// 空の描画処理
	DrawSky();

	// 地面の描画処理
	DrawField();

	// 影の描画処理
	DrawShadow();

	// モデルの更新処理
	DrawModel();

	// プレイヤーの描画処理
	DrawPlayer();

	// エフェクトの描画処理
	DrawEffect();

	// 体力ゲージ(ベース)の描画処理
	DrawBaseLifeGauge();

	// 体力ゲージ(ダメージ)の描画処理
	DrawDamageGauge();

	// 体力ゲージの描画処理
	DrawLifeGauge();

	// 体力ゲージ(カバー)の描画処理
	DrawCoverLife();

	// 必殺ゲージの描画処理
	DrawUltGauge();

	// タイマーの更新処理
	DrawTimer();
	DrawRush();

	if (g_bPuase == true)
	{// ポーズ中なら
		// ポーズ描画処理
		DrawPause();

		// ポーズロゴ描画処理
		DrawPauseLogo();

		// ポーズ項目描画処理
		DrawPauseSelect();
	}
}
//=============================================================================
// ゲーム画面設定処理
//=============================================================================
void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	g_nCounterGameState = 0;
}
//=============================================================================
// ゲーム画面取得処理
//=============================================================================
GAMESTATE GetGameState(void)
{
	return g_GameState;
}