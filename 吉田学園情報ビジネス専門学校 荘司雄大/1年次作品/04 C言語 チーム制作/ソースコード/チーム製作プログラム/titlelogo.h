//=============================================================================
// 授業作成
// 背景の処理 [titlelogo.h]
// Author : Mikiya Meguro
//
//=============================================================================
#ifndef _TITLELOGO_H_
#define _TITLELOGO_H_

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitTitleLogo(void);
void UninitTitleLogo(void);
void UpdateTitleLogo(void);
void DrawTitleLogo(void);
D3DXVECTOR3 *GetTitleLogo(void);
void SetTitleLogo(D3DXVECTOR3 TitleLogo);
#endif