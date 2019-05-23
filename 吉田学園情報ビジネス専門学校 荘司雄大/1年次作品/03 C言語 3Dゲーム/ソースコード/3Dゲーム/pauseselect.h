//=============================================================================
//
// ポーズメニュー処理 [pauseselect.h]
// Author : 
//
//=============================================================================
#ifndef _PAUSESELECT_H_
#define _PAUSESELECT_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	SELECTMODE_NONE = 0,
	SELECTMODE_CONTINUE,		//続行状態
	SELECTMODE_RETRY,			//リトライ状態
	SELECTMODE_QUIT,			//終了
	SELECTMODE_MAX				//状態の総数
}SELECTMODE;						//列挙型

typedef enum
{
	SELECTTYPE_NONE = 0,	//選ばれていない状態
	SELECTTYPE_SELECT,		//選ばれている	状態
	SELECTTYPE_MAX				//状態の総数
}SELECT;

typedef struct
{
	SELECT		select;		//セレクト
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR	col;		//カラー
}PAUSESELECT;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPauseSelect(void);
void UninitPauseSelect(void);
void UpdatePauseSelect(void);
void DrawPauseSelect(void);
void SetPauseSelect(D3DXVECTOR3 pos, D3DXCOLOR col);
SELECTMODE *GetPauseMode(void);
#endif