//=============================================================================
//
// 影の処理 [shadow.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define  MAX_SHADOW   (3000)       // 影の総数

//*****************************************************************************
// 影の構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3  pos;          // 位置(中心)
	D3DXVECTOR3  rot;          // 向き
	D3DXMATRIX   mtxWorld;     // ワールドマトリックス
	D3DXCOLOR    col;          // 頂点カラー
	float        width;        // 幅
	float        depth;        // 奥行
	bool         bUse;         // 使用しているかどうか
}Shadow;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float width, float depth);
void SetPositionShadow(int IdxShadow, D3DXVECTOR3 pos);
void SetColShadow(int IdxShadow, D3DXCOLOR col);
void SetSizeShadow(int IdxShadow, float width, float depth);
void DeleteShadow(int IdxShadow);

#endif