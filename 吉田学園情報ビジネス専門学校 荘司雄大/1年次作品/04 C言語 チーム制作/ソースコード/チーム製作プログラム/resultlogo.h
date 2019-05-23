//=============================================================================
// 授業作成
// 背景の処理 [resultlogo.h]
// Author : Mikiya Meguro
//
//=============================================================================
#ifndef _RESULTLOGO_H_
#define _RESULTLOGO_H_

#include "main.h"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitResultLogo(void);
void UninitResultLogo(void);
void UpdateResultLogo(void);
void DrawResultLogo(void);
D3DXVECTOR3 *GetResultLogo(void);
void SetResultLogo(D3DXVECTOR3 ResultLogo);
#endif