//=============================================================================
// 授業作成
// 背景の処理 [pressenter.h]
// Author : Mikiya Meguro
//
//=============================================================================
#ifndef _PRESSENTER_H_
#define _PRESSENTER_H_

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR col;			//カラー
	int		PressEnterCnt;
	float	PressEnterChange;
	bool	bUse;
	float	fWIdth;
	float	fHeight;
	bool	bDraw;
}PRESSENTER;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPressEnter(void);
void UninitPressEnter(void);
void UpdatePressEnter(void);
void DrawPressEnter(void);
void SetPressEnter(D3DXVECTOR3 pos, D3DXCOLOR col,float fWIdth, float fHeight );
PRESSENTER *GetPressEnter(void);

#endif