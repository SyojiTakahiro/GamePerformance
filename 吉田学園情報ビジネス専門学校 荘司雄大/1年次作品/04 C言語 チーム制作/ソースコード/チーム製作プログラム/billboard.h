//=============================================================================
//
// 壁の処理 [billboard.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	//D3DXCOLOR col;					// 色
	D3DXMATRIX	mtrxWorldBillboard;		// ワールドマトリックス
	float fRadius;						// 半径(大きさ)
	float fHeght;						// 高さ
	float fWidth;						// 幅
	int nIdxShadow;						// 影のインデックス
	bool bUse;							// 使用しているかどうか
} BILLBOARD;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBillboard(void);
void UninitBillboard(void);
void UpdateBillboard(void);
void DrawBillboard(void);
void SetBillboard(D3DXVECTOR3 pos, float fHeght, float fWidth);

#endif
