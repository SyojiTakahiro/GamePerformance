//=============================================================================
//
// ゲージの処理 [gauge.h]
// Author  Jukiya Hayakawa
//
//=============================================================================
#ifndef _ULTGAUGE_H_
#define _ULTGAUGE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_ULTGAUGE			(2)								// ゲージの総数
#define ULTGAUGE_WIDTH		(230)								// ゲージの幅

//*****************************************************************************
// ゲージの構造体の定義
//*****************************************************************************
typedef enum
{
	ULTGAUGE_NOMAL = 0,			//通常状態
	ULTGAUGE_READY,				//使用可能状態
	ULTGAUGE_USE,				//使用状態
	ULTGAUGE_MAX,
}ULTGAUGE_STATE;
typedef struct
{
	D3DXVECTOR3  pos;                       // 位置
	D3DXCOLOR	 col;						// 色
	float		 fWidth;					// 幅
	float		 fHeight;					// 高さ
	int			 nType;						// プレイヤーの種類
	ULTGAUGE_STATE state;					// 状態

}UltGauge;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitUltGauge(void);
void UninitUltGauge(void);
void UpdateUltGauge(void);
void DrawUltGauge(void);
void SetUltGauge(float fWidth, int nType);
void SetUltGaugeState(ULTGAUGE_STATE state, int nType);
float GetUltGaugeWidth(int nType);
void UseUltGauge(int nType);
ULTGAUGE_STATE GetUltGaugeState(int nType);
#endif