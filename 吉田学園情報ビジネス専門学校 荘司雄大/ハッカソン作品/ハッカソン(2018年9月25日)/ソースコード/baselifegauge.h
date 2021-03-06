//=============================================================================
//
// 体力の土台のゲージの処理 [basegauge.h]
// Author  Jukiya Hayakawa
//
//=============================================================================
#ifndef _BASELIFEGAUGE_H_
#define _BASELIFEGAUGE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BASEGAUGE			(2)								// ベースゲージの総数

//*****************************************************************************
// ゲージの構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3  pos;                       // 位置
	D3DXCOLOR	 col;						// 色
	float		 fWidth;					// 幅
	float		 fHeight;					// 高さ
	int			 nType;						//種類
}BaseLifeGauge;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBaseLifeGauge(void);
void UninitBaseLifeGauge(void);
void UpdateBaseLifeGauge(void);
void DrawBaseLifeGauge(void);
#endif