//=============================================================================
//
// スコア処理 [time.h]
// Author : Mikiya Meguro
//
//=============================================================================
#ifndef _TIME_H_
#define _TIME_H_

#include "main.h"


typedef struct
{
	int				nTime;				// タイム
	D3DXVECTOR3		TimePos;			//タイマーの位置
	int				nTimeCounter;		//タイムカウンター
	bool			bTimerStop;			//タイマーストップ

}TIME;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitTime(void);
void UninitTime(void);
void UpdateTime(void);
void DrawTime(void);
TIME *GetTime(void);

#endif
