//=============================================================================
//
// 矢印処理 [arrow.h]
// Author : Meguro Mikiya
//
//=============================================================================
#ifndef _ARROW_H_
#define _ARROW_H_

#include "main.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ARROW_NAME000		 "data\\MODEL\\arrow.x"		//テクスチャのファイル名
#define MOVE_ARROW			(2.0f)							//矢印移動量
#define MOVE_SIN_COS_TOP	(0.75f)							//ポリゴン移動量
#define MOVE_SIN_COS_BOT	(0.25f)							//ポリゴン移動量
#define FIELD_MAX_SIZE		(FIELD_SIZE - 10)
#define MAX_ARROW			(1)
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	DWORD nNumMat = 0;						//マテリアル情報の数
	D3DXVECTOR3 pos;						//位置
	D3DXVECTOR3 rot;						//向き
	D3DXMATRIX	mtxWorld;					//ワールドマトリックス
	float fShadow;							//影の大きさ
	int nIdxShadow;							//影のインデックス
	D3DXVECTOR3 DestAngle;					//
	D3DXVECTOR3 DiffAngle;
	D3DXVECTOR3 rotDest;
	int nType;								//矢印の種類
	bool bUse;								//使用しているか
	D3DXVECTOR3 VtxMinArrow, VtxMaxArrow;	//矢印の最小値、最大値
} ARROW;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitArrow(void);
void UninitArrow(void);
void UpdateArrow(void);
void DrawArrow(void);
ARROW *GetArrow(void);

#endif