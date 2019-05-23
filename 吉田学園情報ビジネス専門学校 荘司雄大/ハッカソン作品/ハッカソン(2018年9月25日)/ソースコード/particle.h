//=============================================================================
//
// パーティクルの処理 [particle.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//*****************************************************************************
// パーティクルの構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3   pos;              // 位置(中心)
	D3DXVECTOR3   move;             // 移動量
	D3DXVECTOR3   rot;              // 向き
	D3DXMATRIX    mtxWorld;         // ワールドマトリックス
	D3DXCOLOR     col;              // 色
	float         fRadius;          // 幅
	int           nLife;            // 寿命
	int           nIdxShadow;       // 使用している影の番号
	bool          bUse;             // 使用しているかどうか
}Particle;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define  MAX_PARTICLE   (256)        // パーティクルの総数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 move, float radius, int life);

#endif
