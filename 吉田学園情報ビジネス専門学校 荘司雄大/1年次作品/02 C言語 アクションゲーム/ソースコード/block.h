//=============================================================================
//
// ブロック処理 [block.h]
// Author :Takahiro
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************

typedef enum
{
	BLOCKTYPE_NORMAL = 0, //通常
	BLOCKTYPE_MOVE,		  //動く
	BLOCKTYPE_FALL,		  //落ちる
	BLOCKTYPE_MAX
}BLOCKTYPE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 PosOld;
	D3DXVECTOR3 move;
	float fWidth;
	float fHeight;
	BLOCKTYPE type;
	bool bUse;
}Block;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, D3DXCOLOR col,float fWidth, float fHeight, BLOCKTYPE type);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove,float fWidth, float fHeight/*,Block **pBlock*/);
bool CollisionBlockE(D3DXVECTOR3 *ePos, D3DXVECTOR3 *ePosOld, D3DXVECTOR3 *eMove, float fWidth, float fHeight/*,Block **pBlock*/);
#endif
