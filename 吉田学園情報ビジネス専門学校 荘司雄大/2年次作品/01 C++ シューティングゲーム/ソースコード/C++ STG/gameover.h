//=============================================================================
//
// �Q�[���I�[�o�[���� [gameover.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _GAMEOVER_H_
#define _GAMEOVER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

//*****************************************************************************
// �I�u�W�F�N�g�N���X
//*****************************************************************************

class CGameover
{
public:
	CGameover();
	~CGameover();
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̊J��

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGameover *Create(void);
protected:

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;				//���L�e�N�X�`���ւ̃|�C���^
	CScene2D *m_pCScene2D;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

};

#endif