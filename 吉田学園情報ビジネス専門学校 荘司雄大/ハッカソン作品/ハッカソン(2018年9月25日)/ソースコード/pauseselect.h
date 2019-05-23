//=============================================================================
//
// ポーズ項目処理 [pauseselect.h]
// Author : Niwa Hodaka
//
//=============================================================================
#ifndef _PAUSESELECT_H_
#define _PAUSESELECT_H_

#include "main.h"

//*************************************
// 項目の状態
//*************************************
typedef enum
{
	PAUSESELECT_NONE = 0,   // 選択されていない状態
	PAUSESELECT_SELECT,     // 選択されている状態
	PAUSESELECT_MAX
}PAUSESELECT;

typedef enum
{    
	RESTART_CONTINUE,  // コンティニュー
	RESTART_RETRY,     // ゲームリトライ
	RESTART_QUIT,      // タイトル遷移
	RESTART_MAX
}RESTART;

//*************************************
// 項目の構造体
//*************************************
typedef struct
{
	D3DXCOLOR col;       // 色
	PAUSESELECT select;  // 選択されているかどうか
}PAUSE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPauseSelect(void);
void UninitPauseSelect(void);
void UpdatePauseSelect(void);
void DrawPauseSelect(void);

RESTART GetRestart(void);

#endif
