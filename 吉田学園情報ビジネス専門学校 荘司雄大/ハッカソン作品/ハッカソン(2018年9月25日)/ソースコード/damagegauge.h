//=============================================================================
//
// ダメージゲージの処理 [damagegauge.h]
// Author  Jukiya Hayakawa
//
//=============================================================================
#ifndef _DAMAGEGAUGE_H_
#define _DAMAGEGAUGE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DAMAGEGAUGE			(2)								// ゲージの総数

//*****************************************************************************
// ゲージの構造体の定義
//*****************************************************************************
typedef enum
{
	STATE_NOMAL = 0,
	STATE_MINUS,
	STATE_MAX,
}DAMAGE_STATE;
typedef struct
{
	D3DXVECTOR3  pos;                       // 位置
	D3DXCOLOR	 col;						// 色
	float		 fWidth;					// 幅
	float		 fHeight;					// 高さ
	int			 nType;						//プレイヤーの種類
	DAMAGE_STATE state;						//状態
}DamageGauge;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitDamageGauge(void);
void UninitDamageGauge(void);
void UpdateDamageGauge(void);
void DrawDamageGauge(void);
void SetDamageGaugeState(DAMAGE_STATE state,int nType,float fWidth);
#endif