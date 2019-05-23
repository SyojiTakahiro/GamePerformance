//=============================================================================
//
// 地面の処理 [field.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"

//*****************************************************************************
// フィールドの構造体
//*****************************************************************************
typedef enum
{
	FIELDSET_TEXTYPE = 0,
	FIELDSET_POS,
	FIELDSET_ROT,
	FIELDSET_SIDE,
	FIELDSET_DEPTH,
	FIELDSET_XBLOCK,
	FIELDSET_ZBLOCK
}FIELDSET;

typedef struct
{
	D3DXVECTOR3 pos;        // 座標
	D3DXVECTOR3 rot;        // 向き
	D3DXMATRIX  mtxWorld;   // ワールドマトリックス
	float       Depth;      // 奥行の長さ
	float       Side;       // 横の長さ
	int         XBlock;     // 横の分割数
	int         ZBlock;     // 縦の分割数
	int         nNumVertex; // 頂点数
	int         nNumIndex;  // インデックス数
	int         nNumPolygon;// ポリゴン数
	int         TexType;    // テクスチャの種類
}Field;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);

#endif