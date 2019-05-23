//=============================================================================
// 授業作成
// 背景の処理 [rankinglogo.h]
// Author : Mikiya Meguro
//
//=============================================================================
#ifndef _RANKINGLOGO_H_
#define _RANKINGLOGO_H_

#include "main.h"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitRankingLogo(void);
void UninitRankingLogo(void);
void UpdateRankingLogo(void);
void DrawRankingLogo(void);
D3DXVECTOR3 *GetRankingLogo(void);
void SetRankingLogo(D3DXVECTOR3 RankingLogo);
#endif