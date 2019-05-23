#pragma once
#ifndef _BOAL_H_
#define _BOAL_H_

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
}Boal;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBoal(void);
void UninitBoal(void);
void UpdateBoal(void);
void DrawBoal(void);
#endif