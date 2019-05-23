//=============================================================================
//
// 弾処理 [bullet.h]
// Author :Takahiro
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//*****************************************************************************
// 構造体
//*****************************************************************************
typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_PLAYER2,
	BULLETTYPE_CPU,
	BULLETTYPE_MAX
}BULLETTYPE;

typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 posOld;	// 頂点座標
	D3DXVECTOR3 move;
	D3DXMATRIX mtxWorld;
	BULLETTYPE type;
	int nLife;
	int bType;
	float fWidth;
	float fHeight;
	int nIdxShadow;
	bool bUse;
	D3DXVECTOR3 VtxMin, VtxMax;
}Bullet;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight,BULLETTYPE type);
Bullet *GetBullet(void);

#endif