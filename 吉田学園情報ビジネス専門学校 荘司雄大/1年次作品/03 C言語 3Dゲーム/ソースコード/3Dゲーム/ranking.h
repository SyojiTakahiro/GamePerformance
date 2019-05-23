//=============================================================================
//
// ランキング処理 [ranking.h]
// Author : 
//
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	//int nCounterAnim;	//アニメーションカウンター
	//int nPatternAnim;	//アニメーションパターンNoを初期化	
	D3DXCOLOR col;		//カラー
}Ranking;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void SetRanking(int aRanking);
#endif
