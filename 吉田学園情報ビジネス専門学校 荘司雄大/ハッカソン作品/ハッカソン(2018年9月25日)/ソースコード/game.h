//=============================================================================
//
// ゲーム画面処理 [game.h]
// Author : Niwa Hodaka
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

//*****************************************************************************
// 構造体の定義
//*****************************************************************************
// ゲームの状態
typedef enum
{
	GAMESTATE_NONE = 0,   // 何もしていない状態
	GAMESTATE_NONE_RETRY, // 何もしていない状態(リトライ)
	GAMESTATE_NORMAL,     // 通常(プレイ)状態
	GAMESTATE_RETRY,      // やり直し状態
	GAMESTATE_END,        // 終了状態
	GAMESTATE_MAX
}GAMESTATE;

//***************************************************************************** 
// プロトタイプ宣言
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);

#endif