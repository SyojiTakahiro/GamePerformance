//=============================================================================
//
//  [game.h]
// Author :
//
//=============================================================================
#ifndef _SINGLEGAME_H_
#define _SINGLEGAME_H_

#include "main.h"

typedef enum
{
	SINGLEGAMESTATE_NONE = 0,
	SINGLEGAMESTATE_NORMAL,
	SINGLEGAMESTATE_P1WIN,
	SINGLEGAMESTATE_P2WIN,
	SINGLEGAMESTATE_MAX
}SINGLEGAMESTATE;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitSingleGame(void);
void UninitSingleGame(void);
void UpdateSingleGame(void);
void DrawSingleGame(void);
void SetSingleGameState(SINGLEGAMESTATE state);
SINGLEGAMESTATE GetSingleGameState(void);


#endif
