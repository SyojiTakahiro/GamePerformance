//=============================================================================
//
// �N���A���� [Clear.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _CLEAR_H_
#define _CLEAR_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

//*****************************************************************************
// �I�u�W�F�N�g�N���X
//*****************************************************************************
class CNumber;

class CClear
{
public:
	CClear();
	~CClear();
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̊J��

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CClear *Create(void);

	static void SetClearScore(int nScore);
protected:

private:
	CNumber *m_apNumber[MAX_NUMBER];
	static int m_nScore;

	static LPDIRECT3DTEXTURE9	m_pTexture;				//���L�e�N�X�`���ւ̃|�C���^
	CScene2D *m_pCScene2D;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 m_pos;
};

#endif