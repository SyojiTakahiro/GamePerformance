//=============================================================================
//
// PressEnter処理 [pressenter.h]
// Author : Niwa Hodaka
//
//=============================================================================
#ifndef _PRESSENTER_H_
#define _PRESSENTER_H_

#include "main.h"

//*************************************
// PressEnterの状態
//*************************************
typedef enum
{
	PRESSENTERSTATE_NONE = 0,
    PRESSENTERSTATE_PUSH,
	PRESSENTERSTATE_MAX
}PRESSENTERSTATE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPressEnter(void);
void UninitPressEnter(void);
void UpdatePressEnter(void);
void DrawPressEnter(void);
void SetPressEnter(D3DXVECTOR3 pos,int xsize, int ysize);
bool *GetDispPressEnter(void);
PRESSENTERSTATE *GetPressEnter(void);
void DispPressEnter(void);

#endif
