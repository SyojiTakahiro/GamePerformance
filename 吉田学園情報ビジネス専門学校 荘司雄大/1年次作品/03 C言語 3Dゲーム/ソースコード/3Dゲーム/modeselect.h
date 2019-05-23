//=============================================================================
//
// ポーズメニュー処理 [pauseselect.h]
// Author : 
//
//=============================================================================
#ifndef _MODESELECT_H_
#define _MODESELECT_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	GAMEMODESELECT_NONE = 0,
	GAMEMODESELECT_SINGLEMODE,		//続行状態
	GAMEMODESELECT_2PMODE,			//リトライ状態
	GAMEMODESELECT_TUTORIAL,			//終了
	GAMEMODESELECT_MAX				//状態の総数
}GAMEMODESELECT;						//列挙型

typedef enum
{
	SELECTGAMETYPE_NONE = 0,	//選ばれていない状態
	SELECTGAMETYPE_SELECT,		//選ばれている	状態
	SELECTGAMETYPE_MAX				//状態の総数
}SELECTGAME;

typedef struct
{
	SELECTGAME	selectgame;		//セレクト
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR	col;		//カラー
}MODESELECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitModeSelect(void);
void UninitModeSelect(void);
void UpdateModeSelect(void);
void DrawModeSelect(void);
void SetModeSelect(D3DXVECTOR3 pos, D3DXCOLOR col);
GAMEMODESELECT *GetModeMode(void);
#endif