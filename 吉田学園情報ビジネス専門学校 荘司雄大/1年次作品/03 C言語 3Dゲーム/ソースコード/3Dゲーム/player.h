//=============================================================================
//
// モデル処理 [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_PLAYER	(1)	// プレイヤーの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	PLAYERSTATE_APPEAR = 0,
	PLAYERSTATE_NORMAL,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_DEATH,
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef enum
{//弾
	BULLETKIND_NOMAL = 0,	//通常状態
	BULLETKIND_DIFFUSION,	//拡散
	BULLETKIND_MAX
} BULLETKIND;

typedef struct
{
	LPD3DXMESH pMesh;
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nIdxModelParent;
}MODEL;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;					//向き
	int nType;
	bool bUse;
	PLAYERSTATE state;	//プレイヤーの状態
	int nCounterState;	//状態管理のカウンター
	int nLife;			//体力
	int DirectionMove;
	BULLETKIND BulletKind;
	bool bDisp;
	bool bJump;
	D3DXMATRIX mtxWorld;
	MODEL aModel[2];
	D3DXVECTOR3 VtxMin, VtxMax;	//モデルの最小値、最大値
}PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pModel*/);


#endif
