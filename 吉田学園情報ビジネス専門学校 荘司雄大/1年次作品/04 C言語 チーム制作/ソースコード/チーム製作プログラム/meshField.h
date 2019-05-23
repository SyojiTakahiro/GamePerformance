//=============================================================================
//
// ポリゴン処理 [meshField.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MESHFIELD_TEXTURENAME		 "data\\TEXTURE\\field000.jpg"	//テクスチャのファイル名
#define TEX_POS_X_INIT			(1.0f)								//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)								//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)								//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)								//テクスチャ座標U右
#define TEX_TOP					(0.0f)								//テクスチャ座標V上
#define TEX_BOT					(1.0f)								//テクスチャ座標V下
#define X_POSSIZE				(50)								//Xの大きさ
#define Z_POSSIZE				(50)								//Zの大きさ
#define X_FIELD_SIZE			(9)									//Xの幅
#define Z_FIELD_SIZE			(9)									//Zの幅
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

#endif
