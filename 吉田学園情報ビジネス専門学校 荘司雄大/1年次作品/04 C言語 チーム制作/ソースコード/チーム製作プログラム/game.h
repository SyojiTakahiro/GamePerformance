//=============================================================================
//
// ゲームの処理 [game.h]
// Author : Mikiya Meguro
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"
#include "player.h"
#include "input.h"
#include "time.h"
//#include "item.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	GAMESTATE_NONE = 0,		//何もない状態
	GAMESTATE_TUTORIAL,		//チュートリアル状態
	GAMESTATE_NORMAL,		//通常状態
	GAMESTATE_GAMECLEAR,	//終了状態
	GAMESTATE_GAMEOVER,		//ゲームオーバー状態
	GAMESTATE_RANKING,		//ランキング状態
	GAMESTATE_MAX			//状態の総数
}GAMESTATE;				//列挙型

//typedef struct
//{
//	D3DXVECTOR3 pos;		//頂点座標
//	float	fWidth;			//幅
//	float	fHeight;		//高さ
//	BLOCKSTATE BlockType;	//ブロックタイプ
//	int		moveCounter;
//}BlockInfo;

//typedef struct
//{
//	D3DXVECTOR3 pos;		//頂点座標
//	D3DXCOLOR col;			//カラー
//	ITEMTYPE ItemType;		//アイテムタイプ
//}ItemInfo;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);
void SetGaugeState(bool bChange);

#endif
