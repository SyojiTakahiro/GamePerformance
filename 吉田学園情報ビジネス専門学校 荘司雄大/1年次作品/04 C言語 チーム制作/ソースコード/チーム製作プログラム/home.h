//=============================================================================
//
// ホーム処理 [home.h]
// Author : Meguro Mikiya
//
//=============================================================================
#ifndef _HOME_H_
#define _HOME_H_

#include "main.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define HOME_NAME000		 "data\\MODEL\\goal.x"	//テクスチャのファイル名
#define FIELD_MAX_SIZE		(FIELD_SIZE - 10)
#define MAX_HOME			(1)
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
	int nType;								//モデルの種類
	bool bUse;								//使用しているか
	D3DXVECTOR3 VtxMinHome, VtxMaxHome;	//モデルの最小値、最大値
} HOME;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitHome(void);
void UninitHome(void);
void UpdateHome(void);
void DrawHome(void);
HOME *GetHome(void);
bool CollisionHome(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pHome*/);
void SetHome(D3DXVECTOR3 pos,int nType);
#endif