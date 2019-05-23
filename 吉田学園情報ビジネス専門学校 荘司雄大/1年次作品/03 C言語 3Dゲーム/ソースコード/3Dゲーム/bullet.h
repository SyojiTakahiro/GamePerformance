//=============================================================================
//
// �e���� [bullet.h]
// Author :Takahiro
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//*****************************************************************************
// �\����
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
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 posOld;	// ���_���W
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
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight,BULLETTYPE type);
Bullet *GetBullet(void);

#endif