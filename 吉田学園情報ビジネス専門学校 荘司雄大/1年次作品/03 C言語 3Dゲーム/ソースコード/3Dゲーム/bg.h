//=============================================================================
//
// ポリゴン処理 [polygon.h]
// Author : 
//
//=============================================================================
#ifndef _BG_H_
#define _BG_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	bool bUse;
}BG;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBG(void);
void UninitBG(void);
void UpdateBG(void);
void DrawBG(void);

#endif