//=============================================================================
//
// マップの処理 [mapplayer.h]
// Author : Mikiya Meguro
//
//=============================================================================
#ifndef _MAPPLAYER_H_
#define _MAPPLAYER_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posOld;	//前回の位置
	D3DXVECTOR3 move;	//位置
	D3DXCOLOR col;		//カラー
	int nMapBorder;		//マップの境界線に到達した数
}MAPPLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitMapPlayer(void);
void UninitMapPlayer(void);
void UpdateMapPlayer(void);
void DrawMapPlayer(void);

#endif