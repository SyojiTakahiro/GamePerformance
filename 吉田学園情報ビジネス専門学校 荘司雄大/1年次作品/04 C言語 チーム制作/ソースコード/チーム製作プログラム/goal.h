//=============================================================================
//
// ゴール処理 [goal.h]
// Author : Meguro Mikiya
//
//=============================================================================
#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GOAL_NAME000		 "data\\MODEL\\goal.x"	//テクスチャのファイル名
#define FIELD_MAX_SIZE		(FIELD_SIZE - 10)
#define MAX_GOAL			(1)
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	GOALSTATE_NORMAL = 0,		// 通常状態
	GOALSTATE_CHECKPOINT,
	GOALSTATE_GOAL,
	GOALSTATE_MAX,			// 状態の総数
}GOALSTATE;

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
	D3DXVECTOR3 VtxMinGoal, VtxMaxGoal;		//モデルの最小値、最大値
	int nCntGoal;
	int nGoalPattern;
	int nCounterState;
	GOALSTATE state;
	float fLength;
} GOAL;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGoal(void);
void UninitGoal(void);
void UpdateGoal(void);
void DrawGoal(void);
GOAL *GetGoal(void);
bool CollisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pGoal*/);
void SetGoal(D3DXVECTOR3 pos,int nType);
#endif