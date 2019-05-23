//=============================================================================
//
// モデル処理 [PLAYER2.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER2_H_
#define _PLAYER2_H_

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
	PLAYER2STATE_APPEAR = 0,
	PLAYER2STATE_NORMAL,
	PLAYER2STATE_DAMAGE,
	PLAYER2STATE_DEATH,
	PLAYER2STATE_MAX
}PLAYER2STATE;

typedef struct
{
	LPD3DXMESH pMesh;
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nIdxModelParent;
}MODEL2;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;					//向き
	float fWidth;
	float fHeight;
	int nType;
	bool bUse;
	PLAYER2STATE state;	//プレイヤーの状態
	int nCounterState;	//状態管理のカウンター
	int nLife;			//体力
	int DirectionMove;
	D3DXMATRIX mtxWorld;
	bool bDisp;
	bool bJump;
	MODEL2 aModel[2];
	D3DXVECTOR3 VtxMin, VtxMax;	//モデルの最小値、最大値
}PLAYER2;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer2(void);
void UninitPlayer2(void);
void UpdatePlayer2(void);
void DrawPlayer2(void);
PLAYER2 *GetPlayer2(void);
bool CollisionPlayer2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pModel*/);

#endif
