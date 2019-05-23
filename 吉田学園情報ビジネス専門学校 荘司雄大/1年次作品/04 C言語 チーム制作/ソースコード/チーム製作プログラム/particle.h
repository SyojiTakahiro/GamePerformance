//=============================================================================
//
// パーティクルの処理 [particle.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	PARTICLE_TYPE_PIZZA = 0,			// 
	PARTICLE_TYPE_PIZZA_DEATH,			// ピザ　死
	PARTICLE_TYPE_GOAL,					// ゴール
	PARTICLE_TYPE_MAX,
}PARTICLE_TYPE;

typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 move;					// 移動量
	D3DXVECTOR3 rot;					// 向き
	D3DXCOLOR col;						// 色
	D3DXMATRIX	mtrxWorldParticle;		// ワールドマトリックス
	float fRadius;						// 半径(大きさ)
	float fHeght;						// 高さ
	float fWidth;						// 幅
	int nLife;							// 表示時間(寿命)
	bool bUse;							// 使用しているかどうか
	PARTICLE_TYPE ParticleType;			// パーティクルの種類
	int nTexType;						// パーティクルのテクスチャの種類
}PARTICLE;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nLife , PARTICLE_TYPE type);

#endif
