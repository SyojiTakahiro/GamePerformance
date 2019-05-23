//=============================================================================
//
// 体力ゲージの処理 [lifegauge.h]
// Author  Jukiya Hayakawa
//
//=============================================================================
#ifndef _LIFEGAUGE_H_
#define _LIFEGAUGE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIFEGAUGE			(2)								// ゲージの総数
#define LIFEGAUGE_WIDTH			(460)							// ゲージの幅
#define LIFEGAUGE_HEIGHT		(25)							// ゲージの高さ

//*****************************************************************************
// ゲージの構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3  pos;                       // 位置
	D3DXCOLOR	 col;						// 色
	float		 fWidth;					// 幅
	float		 fHeight;					// 高さ
	int			 nType;						//プレイヤーの種類
}LifeGauge;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitLifeGauge(void);
void UninitLifeGauge(void);
void UpdateLifeGauge(void);
void DrawLifeGauge(void);
void SetLifeGauge(float fWidth,int nType);
#endif