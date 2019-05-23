//=============================================================================
//
// 壁の処理 [wall.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 rot;					// 向き
	//D3DXCOLOR col;					// 色
	D3DXMATRIX	mtrxWorld;				// ワールドマトリックス
	float fRadius;						// 半径(大きさ)
	float fHeght;						// 高さ
	float fWidth;						// 幅
	bool bUse;							// 使用しているかどうか
} WALL;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeght, float fWidth);

#endif
