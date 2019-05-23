//=============================================================================
//
// ポリゴン処理 [meshCylinder.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _MESHCYLINDERSIZE_H_
#define _MESHCYLINDERSIZE_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHCYLINDER_TEXTURENAME		 "data\\TEXTURE\\mesh000.jpg"	//テクスチャのファイル名
#define TEX_POS_X_INIT			(1.0f)								//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)								//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)								//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)								//テクスチャ座標U右
#define TEX_TOP					(0.0f)								//テクスチャ座標V上
#define TEX_BOT					(1.0f)								//テクスチャ座標V下
//#define FIELD_SIZE				(225.0f)							//地面の大きさ
#define X_CYLINDERPOSSIZE		(20)								//Xの大きさ
#define Y_CYLINDERPOSSIZE		(20)								//Zの大きさ
#define X_CYLINDERSIZE			(16)									//Xの幅
#define Y_CYLINDERSIZE			(2)									//Zの幅
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);

#endif
