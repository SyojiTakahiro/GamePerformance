//=============================================================================
//
// モデル処理 [building.h]
// Author : 
//
//=============================================================================
#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME0		 "data//MODEL//P1.x"		//テクスチャのファイル名
#define MODEL_NAME1		 "data//MODEL//P1.x"		//テクスチャのファイル名
#define MODEL_NAME2		 "data//MODEL//box000.x"		//テクスチャのファイル名
#define MOVE_MODEL			(2.0f)							//モデル移動量
#define MOVE_SIN_COS_TOP	(0.75f)							//ポリゴン移動量
#define MOVE_SIN_COS_BOT	(0.25f)							//ポリゴン移動量
#define FIELD_MAX_SIZE		(FIELD_SIZE - 10)
#define MAX_MODEL			(256)
#define MAX_MODEL_TYPE		(3)								//モデル種類の最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 VtxMinBuilding, VtxMaxBuilding;	//モデルの最小値、最大値
} VTX;
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
	D3DXVECTOR3 VtxMinBuilding, VtxMaxBuilding;	//モデルの最小値、最大値
} BUILDING;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBuilding(void);
void UninitBuilding(void);
void UpdateBuilding(void);
void DrawBuilding(void);
BUILDING *GetBuilding(void);
bool CollisionBuilding(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pBuilding*/);
void SetBuilding(D3DXVECTOR3 pos, int nType);
#endif