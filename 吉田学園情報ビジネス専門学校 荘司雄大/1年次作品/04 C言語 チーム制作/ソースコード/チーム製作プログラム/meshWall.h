//=============================================================================
//
// ポリゴン処理 [meshWall.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHWALL_TEXTURENAME		 "data\\TEXTURE\\wall003.jpg"	//テクスチャのファイル名
#define TEX_POS_X_INIT			(1.0f)								//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)								//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)								//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)								//テクスチャ座標U右
#define TEX_TOP					(0.0f)								//テクスチャ座標V上
#define TEX_BOT					(1.0f)								//テクスチャ座標V下
#define X_POSSIZE				(50)								//Xの大きさ
#define Y_POSSIZE				(50)								//Zの大きさ
#define X_WALLSIZE				(120)									//Xの幅
#define Y_WALLSIZE			 	(2)									//Zの幅
#define MAX_MESHWALL			(4)									//メッシュオールの最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 rot;					//向き
	D3DXMATRIX	mtrxWorld;				//ワールドマトリックス
	
	float fHeght;						// 高さ
	float fWidth;						// 幅
	int nVtxIndex = 0;
	int nIdxIndex = 0;
	float fPos_X = 0;
	float fPos_Y = 0;
	float fXpos = 0;
	float fYpos = 0;
	bool bUse;							// 使用しているかどうか
} MESHWALL;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshWall(int nIdxShadow , D3DXVECTOR3 pos , D3DXVECTOR3 rot);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);

#endif
