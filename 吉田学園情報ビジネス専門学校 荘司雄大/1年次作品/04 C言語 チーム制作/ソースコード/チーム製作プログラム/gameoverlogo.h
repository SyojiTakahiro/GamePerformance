//=============================================================================
// 授業作成
// 背景の処理 [gameoverlogo.h]
// Author : Mikiya Meguro
//
//=============================================================================
#ifndef _GAMEOVERLOGO_H_
#define _GAMEOVERLOGO_H_

#include "main.h"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGameoverLogo(void);
void UninitGameoverLogo(void);
void UpdateGameoverLogo(void);
void DrawGameoverLogo(void);
D3DXVECTOR3 *GetGameoverLogo(void);
void SetGameoverLogo(D3DXVECTOR3 GameoverLogo);
#endif