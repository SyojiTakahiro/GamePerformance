//=============================================================================
//
// 爆発の処理 [Explosion.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXCOLOR col;						// 色
	D3DXMATRIX	mtrxWorldExplosion;		// ワールドマトリックス
	float fRadius;						// 半径(大きさ)
	float fHeght;						// 高さ
	float fWidth;						// 幅
	float fTest;						// 幅
	int nCounterAnim;					// アニメーションカウンター
	int nPatternAnim;					// アニメーションパターンNoを初期化
	float fAlpha;						// 透明度
	bool bUse;							// 使用しているかどうか
} EXPLOSION;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fHeght, float fWidth);

#endif
