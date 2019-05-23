//=============================================================================
// 授業作成
// 背景の処理 [titlelogo.h]
// Author :
//
//=============================================================================
#ifndef _P1WINLOGO_H_
#define _P1WINLOGO_H_

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitP1winLogo(void);
void UninitP1winLogo(void);
void UpdateP1winLogo(void);
void DrawP1winLogo(void);
D3DXVECTOR3 *GetP1winLogo(void);
void SetP1winLogo(D3DXVECTOR3 P1winLogo);
#endif