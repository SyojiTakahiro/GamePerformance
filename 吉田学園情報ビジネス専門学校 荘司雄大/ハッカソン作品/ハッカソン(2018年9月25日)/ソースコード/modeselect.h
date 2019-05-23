//=============================================================================
//
// モードセレクトの処理 [modeselect.h]
// Author : Niwa Hodaka
//
//=============================================================================
#ifndef _MODESELECT_H_
#define _MODESELECT_H_

#include "main.h"

//*************************************
// 項目の状態
//*************************************
typedef enum
{
	MODESTATE_NONE = 0,   // 選択されていない状態
	MODESTATE_SELECT,     // 選択されている状態
	MODESTATE_MAX
}MODESTATE;

typedef enum
{
	MODESTART_GAME,     // ゲーム遷移
	MODESTART_TUTORIAL, // チュートリアル遷移
	MODESTART_RANKING,  // ランキング遷移
	MODESTART_TITLE,    // タイトル遷移
	MODESTART_MAX
}MODESTART;

//*************************************
// 項目の構造体
//*************************************
typedef struct
{
	D3DXCOLOR col;    // 色
	MODESTATE state;  // 選択されているかどうか
}MODESELECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitModeSelect(void);
void UninitModeSelect(void);
void UpdateModeSelect(void);
void DrawModeSelect(void);

#endif