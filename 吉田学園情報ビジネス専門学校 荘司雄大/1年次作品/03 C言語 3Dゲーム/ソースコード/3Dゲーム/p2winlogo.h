//=============================================================================
// 授業作成
// 背景の処理 [titlelogo.h]
// Author :
//
//=============================================================================
#ifndef _P2WINLOGO_H_
#define _P2WINLOGO_H_

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitP2winLogo(void);
void UninitP2winLogo(void);
void UpdateP2winLogo(void);
void DrawP2winLogo(void);
D3DXVECTOR3 *GetP2winLogo(void);
void SetP2winLogo(D3DXVECTOR3 P2winLogo);
#endif