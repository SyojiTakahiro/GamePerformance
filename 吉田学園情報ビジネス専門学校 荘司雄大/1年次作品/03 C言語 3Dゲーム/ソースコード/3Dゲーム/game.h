//=============================================================================
//
//  [game.h]
// Author :
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

typedef enum
{
	GAMESTATE_NONE =0,
	GAMESTATE_NORMAL,
	GAMESTATE_P1WIN,
	GAMESTATE_P2WIN,
	GAMESTATE_LOSE,
	GAMESTATE_MAX
}GAMESTATE;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGameState(void);


#endif
