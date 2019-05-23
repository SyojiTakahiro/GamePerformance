//=============================================================================
//
// �u���b�N���� [block.h]
// Author :Takahiro
//
//=============================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

typedef enum
{
	BLOCKTYPE_NORMAL = 0, //�ʏ�
	BLOCKTYPE_MOVE,		  //����
	BLOCKTYPE_FALL,		  //������
	BLOCKTYPE_MAX
}BLOCKTYPE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 PosOld;
	D3DXVECTOR3 move;
	float fWidth;
	float fHeight;
	BLOCKTYPE type;
	bool bUse;
}Block;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, D3DXCOLOR col,float fWidth, float fHeight, BLOCKTYPE type);
bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove,float fWidth, float fHeight/*,Block **pBlock*/);
bool CollisionBlockE(D3DXVECTOR3 *ePos, D3DXVECTOR3 *ePosOld, D3DXVECTOR3 *eMove, float fWidth, float fHeight/*,Block **pBlock*/);
#endif
