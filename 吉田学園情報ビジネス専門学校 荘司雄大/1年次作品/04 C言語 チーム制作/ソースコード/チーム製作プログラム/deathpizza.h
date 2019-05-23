//=============================================================================
//
// ピザ死亡時の処理 [deathpizza.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _DEATHPIZZA_H_
#define _DEATHPIZZA_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 move;					// 位置
	D3DXCOLOR col;						// 色
	D3DXMATRIX	mtrxWorldDeathPizza;	// ワールドマトリックス
	float fRadius;						// 半径(大きさ)
	float fHeght;						// 高さ
	float fWidth;						// 幅
	int nLife;							// 表示時間(寿命)
	bool bUse;							// 使用しているかどうか
} DEATHPIZZA;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitDeathPizza(void);
void UninitDeathPizza(void);
void UpdateDeathPizza(void);
void DrawDeathPizza(void);
void SetDeathPizza(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife);

#endif
