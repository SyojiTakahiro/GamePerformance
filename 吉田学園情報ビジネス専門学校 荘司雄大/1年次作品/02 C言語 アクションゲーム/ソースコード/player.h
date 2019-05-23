//=============================================================================
//
// プレイヤーの表示処理 [player.h]
// Author : Takahiro Syoji
//
//=============================================================================
#include "main.h"
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_PLAYER	(2)	// プレイヤーの最大数

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
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	float fWidth;
	float fHeight;
	int nType;
	bool bUse;
	PLAYERSTATE state;	//プレイヤーの状態
	int nCounterState;	//状態管理のカウンター
	int nLife;			//体力
	int DirectionMove;
	BULLETKIND BulletKind;
	bool bDisp;
	bool bJump;
	//Block *pBlock;
}Player;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void HitPlayer(int nDmage);
Player *GetPlayer(void);
#endif
