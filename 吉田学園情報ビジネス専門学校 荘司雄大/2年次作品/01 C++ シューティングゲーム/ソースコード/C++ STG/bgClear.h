#//=============================================================================
//
// �N���A�w�i���� [bgClear.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _BGCLEAR_H_
#define _BGCLEAR_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

//*****************************************************************************
// 2D�|���S���N���X
//*****************************************************************************
class CBgClear : public CScene
{
public:
	CBgClear();
	~CBgClear();
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̊J��

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBgClear *Create(void);
private:
	static LPDIRECT3DTEXTURE9	m_apTexture[MAX_BG];				//���L�e�N�X�`���ւ̃|�C���^
	CScene2D *m_apCScene2D[MAX_SCENE];

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	int m_nCntAnim;
};

#endif