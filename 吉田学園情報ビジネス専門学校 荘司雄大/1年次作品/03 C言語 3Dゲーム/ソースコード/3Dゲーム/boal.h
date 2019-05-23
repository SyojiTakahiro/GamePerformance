#pragma once
#ifndef _BOAL_H_
#define _BOAL_H_

#include "main.h"

//*****************************************************************************
// �\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld;
	bool bUse;
}Boal;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBoal(void);
void UninitBoal(void);
void UpdateBoal(void);
void DrawBoal(void);
#endif