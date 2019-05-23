//=============================================================================
//
// モデル処理 [model.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// モデルの構造体
//*****************************************************************************
typedef enum
{
	MODELSET_POS = 0,
	MODELSET_ROT,
	MODELSET_MAX
}MODELSET;

typedef struct
{
	D3DXMATRIX mtxWorld;    // ワールドマトリックス
	D3DXVECTOR3 pos;        // 座標
	D3DXVECTOR3 rot;        // 向き
}Vertex_Model;

typedef struct
{
	D3DXVECTOR3  pos;                       // 位置
	D3DXVECTOR3  rot;                       // 向き
	D3DXVECTOR3  move;                      // 移動量
	D3DXMATRIX   mtxWorld;                  // ワールドマトリックス
	D3DXVECTOR3	vtxMinObstacle, vtxMaxObstacle;	// モデルの最小値、最大値
	Vertex_Model Vertex[4];                 // 頂点のワールドマトリックス
}Model;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 radius);

#endif