//=============================================================================
//
// ｐレイヤーの表示処理 [player.h]
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
	PLAYERSTATE_APPEAR=0,
	PLAYERSTATE_NORMAL,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_DEATH,
	PLAYERSTATE_MAX
}PLAYERSTATE;


typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	int nType;
	bool bUse;
	PLAYERSTATE state;	//プレイヤーの状態
	int nCounterState;	//状態管理のカウンター
	int nLife;			//体力
	bool bDisp;
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