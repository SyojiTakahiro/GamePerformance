//=============================================================================
//
//  [game.h]
// Author :Takahiro
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

typedef enum
{
	GAMESTATE_NONE = 0,
	GAMESTATE_NORMAL,
	GAMESTATE_CLEAR,
	GAMESTATE_END,
	GAMESTATE_MAX
}GAMESTATE;

typedef struct
{
	D3DXVECTOR3 pos;
	float fWidth;
	float fHeight;
}BlockInfo;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
GAMESTATE *GetGameState(void);


#endif

