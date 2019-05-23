//=============================================================================
//
// 通行人処理 [passage.h]
// Author : SHUN YOKOMICHI
//
//=============================================================================
#ifndef _PASSAGE_H_
#define _PASSAGE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PASSAGE_NAME000		 "data\\MODEL\\Passage\\Passage001.x"	//MODELのファイル名
							//"data\\MODEL\\Bill\\pokkuruzou.x"
#define MAX_PASSAGE			(128)							//モデル配置の最大数
#define MAX_PASSAGE_TYPE	(1)								//モデル種類の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 VtxMinEnemy, VtxMaxEnemy;	// モデルの最小値、最大値
} VTXPASSAGE;
typedef struct
{
	DWORD nNumMat = 0;						// マテリアル情報の数
	D3DXVECTOR3 pos;						// 位置
	D3DXVECTOR3 move;						// 移動
	D3DXVECTOR3 rot;						// 向き(現在)
	D3DXVECTOR3 rotDest;					// 向き(目的)
	D3DXMATRIX	mtxWorld;					// ワールドマトリックス
	float fShadow;							// 影の大きさ
	float fSpeed;							// 移動速度
	float fDestAngle;						//通行人の角度
	int nActiontime;						// 行動時間
	int nIdxShadow;							// 影のインデックス
	int nType;								// モデルの種類
	int nCntTime;							// 移動するモデルの時間
	int nPattan;							// 行動( 0 : 横移動 / 1 : 縦行動)
	bool bUse;								// 使用しているか
	bool bChange;							// 行動の変化
	D3DXVECTOR3 VtxMinEnemy, VtxMaxEnemy;	// モデルの最小値、最大値
	float fLength;
} PASSAGE;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPassage(void);
void UninitPassage(void);
void UpdatePassage(void);
void DrawPassage(void);
PASSAGE *GetPassage(void);
bool CollisionPassage(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin);
void SetPassage(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPattan, int nActiontime, float fSpeed, bool bChange);
#endif