//=============================================================================
//
// モデル処理 [model.h]
// Author : 
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "polygon.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME000		 "data\\MODEL\\P1.x"		//テクスチャのファイル名
#define MODEL_NAME001		 "data\\MODEL\\P2.x"		//テクスチャのファイル名
#define MODEL_NAME002		 "data\\MODEL\\box000.x"		//テクスチャのファイル名

#define MOVE_MODEL1			(2.0f)							//モデル移動量
#define MOVE_SIN_COS_TOP1	(0.75f)							//ポリゴン移動量
#define MOVE_SIN_COS_BOT1	(0.25f)							//ポリゴン移動量
#define FIELD_MAX_SIZE1		(FIELD_SIZE - 10)
#define MAX_MODEL1			(300)							//モデル配置の最大数
#define MAX_MODEL_TYPE1		(2)								//モデル種類の最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 VtxMinModel, VtxMaxModel;	//モデルの最小値、最大値
	D3DXVECTOR3 VtxMinEnemy, VtxMaxEnemy;	//モデルの最小値、最大値

} VTX1;
typedef struct
{
	DWORD nNumMat = 0;						//マテリアル情報の数
	D3DXVECTOR3 pos;						//位置
	D3DXVECTOR3 rot;						//向き
	D3DXMATRIX	mtxWorld;					//ワールドマトリックス
	float fShadow;							//影の大きさ
	int nIdxShadow;							//影のインデックス
	int nType;								//モデルの種類
	bool bUse;								//使用しているか
	D3DXVECTOR3 VtxMinModel, VtxMaxModel;	//モデルの最小値、最大値
} MODEL1;

typedef enum
{
	COLLISION_PLAYER = 0,	//プレイヤー
	COLLISION_ENEMY,
	COLLISION_BULLET,		//弾
	COLLISION_ITEM,
	COLLISION_MAX,
} COLLISIONMODELTYPE;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
MODEL1 *GetModel(void);
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin, COLLISIONMODELTYPE Type);
void SetModel(D3DXVECTOR3 pos,int nType);
#endif