//=============================================================================
//
// エフェクトの処理 [effect.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 move;					// 位置
	D3DXCOLOR col;						// 色
	D3DXMATRIX	mtrxWorldEffect;		// ワールドマトリックス
	float fRadius;						// 半径(大きさ)
	float fHeght;						// 高さ
	float fWidth;						// 幅
	float fTest;						// 幅
	int nLife;							// 表示時間(寿命)
	bool bUse;							// 使用しているかどうか
} EFFECT;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife);

#endif
