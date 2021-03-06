//=============================================================================
//
// 体力のカバーの処理 [coverlife.h]
// Author  Jukiya Hayakawa
//
//=============================================================================
#ifndef _COVERLIFE_H_
#define _COVERLIFE_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_COVERLIFE			(2)								// 体力のカバー総数

//*****************************************************************************
// ゲージの構造体の定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3  pos;                       // 位置
	D3DXCOLOR	 col;						// 色
	float		 fWidth;					// 幅
	float		 fHeight;					// 高さ
	int			 nType;						//種類
}LifeCover;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCoverLife(void);
void UninitCoverLife(void);
void UpdateCoverLife(void);
void DrawCoverLife(void);
#endif