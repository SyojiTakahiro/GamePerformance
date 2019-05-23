//=============================================================================
//
// �A�C�e�� [item.h]
// Author :Takahiro
//
//=============================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	//D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fWidth;
	float fHeight;
	int nCounterAnim;
	int nPatternAnim;

	bool bUse;
}Item;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);
void SetItem(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight);
bool CollisionItem(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, float fWidth, float fHeight);
Item *GetItem(void);
#endif
