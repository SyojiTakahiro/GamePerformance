//=============================================================================
//
// モデル処理 [player.h]
// Author : 
//
//=============================================================================
#ifndef _CPU_H_
#define _CPU_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_CPU	(1)	// プレイヤーの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	CPUSTATE_APPEAR = 0,
	CPUSTATE_NORMAL,
	CPUSTATE_DAMAGE,
	CPUSTATE_DEATH,
	CPUSTATE_MAX
}CPUSTATE;

typedef enum
{//弾
	CPUBULLETKIND_NOMAL = 0,	//通常状態
	CPUBULLETKIND_DIFFUSION,	//拡散
	CPUBULLETKIND_MAX
} CPUBULLETKIND;

typedef struct
{
	LPD3DXMESH pMesh;
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nIdxModelParent;
}CPUMODEL;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;					//向き
	int nType;
	bool bUse;
	CPUSTATE state;	//プレイヤーの状態
	int nCounterState;	//状態管理のカウンター
	int nLife;			//体力
	int DirectionMove;
	CPUBULLETKIND BulletKind;
	bool bDisp;
	bool bJump;
	D3DXMATRIX mtxWorld;
	CPUMODEL aModel[2];
	D3DXVECTOR3 VtxMin, VtxMax;	//モデルの最小値、最大値
}CPU;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCPU(void);
void UninitCPU(void);
void UpdateCPU(void);
void DrawCPU(void);
CPU *GetCPU(void);
bool CollisionCPU(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pModel*/);


#endif