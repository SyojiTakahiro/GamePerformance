//=============================================================================
//
// ポリゴン処理 [shadow.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	D3DXCOLOR col;						// 色
	D3DXMATRIX	mtrxWorld;				// ワールドマトリックス
	float fRadius;						// 半径(大きさ)
	bool bUse;							// 使用しているかどうか
	float fLength;
} SHADOW;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fRadius);
void DeleteShadow(int nIdxShadow);

#endif
