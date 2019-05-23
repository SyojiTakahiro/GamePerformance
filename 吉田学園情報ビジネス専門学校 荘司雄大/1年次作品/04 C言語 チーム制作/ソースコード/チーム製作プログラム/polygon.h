//=============================================================================
//
// ポリゴン処理 [polygon.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _POLYGON_H_
#define _POLYGON_H_

#include "main.h"

//*****************************************************************************
// ポリゴンの構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;							//位置
	D3DXVECTOR3 rot;							//向き
	D3DXMATRIX	mtrxWorld;						//ワールドマトリックス
	bool bUse;
}POLYGON;
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POLYGON_TEXTURENAME		 "data\\TEXTURE\\MapField001.jpg"		//テクスチャのファイル名
#define TEX_POS_X_INIT			(1.0f)							//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)							//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)							//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)							//テクスチャ座標U右
#define TEX_TOP					(0.0f)							//テクスチャ座標V上
#define TEX_BOT					(1.0f)							//テクスチャ座標V下
#define FIELD_SIZE				(3000.0f)						//地面の大きさ
#define MAX_POLYGON				(9)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPolygon(void);
void UninitPolygon(void);
void UpdatePolygon(void);
void DrawPolygon(void);
void SetPolygon(D3DXVECTOR3 pos);

#endif
