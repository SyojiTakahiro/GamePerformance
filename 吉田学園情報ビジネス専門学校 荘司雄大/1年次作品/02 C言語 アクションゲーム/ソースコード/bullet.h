//=============================================================================
//
// 弾処理 [bullet.h]
// Author :Takahiro
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

typedef enum
{
	BULLETTYPE_PLAYER = 0,
	BULLETTYPE_ENEMY,
	BULLETTYPE_MAX
}BULLETTYPE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight,BULLETTYPE type);

#endif
