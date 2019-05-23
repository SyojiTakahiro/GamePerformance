//=============================================================================
//
// 車処理 [car.h]
// Author : SHUN YOKOMICHI
//
//=============================================================================
#ifndef _CAR_H_
#define _CAR_H_

#include "main.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAR_NAME000		"data\\MODEL\\車\\anbirance.x"		//MODELのファイル名
#define CAR_NAME001		"data\\MODEL\\車\\car000.x"			//MODELのファイル名
#define CAR_NAME002		"data\\MODEL\\車\\car001.x"			//MODELのファイル名
#define CAR_NAME003		"data\\MODEL\\車\\car002.x"			//MODELのファイル名  // 縦のMODEL

#define CAR_NAME004		"data\\MODEL\\車\\car003.x"			//MODELのファイル名
#define CAR_NAME005		"data\\MODEL\\車\\car004.x"			//MODELのファイル名
#define CAR_NAME006		"data\\MODEL\\車\\car005.x"			//MODELのファイル名
#define CAR_NAME007		"data\\MODEL\\車\\car006.x"			//MODELのファイル名  // 横のMODEL

#define MAX_CAR			(246)								//モデル配置の最大数
#define MAX_CAR_TYPE	(8)									//モデル種類の最大数


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 VtxMinEnemy, VtxMaxEnemy;	// モデルの最小値、最大値
} VTXCAR;
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
} CAR;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCar(void);
void UninitCar(void);
void UpdateCar(void);
void DrawCar(void);
CAR *GetCar(void);
bool CollisionCar(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin);
void SetCar(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPattan, int nActiontime, float fSpeed, bool bChange);
#endif