//=============================================================================
//
// �������� [explosion.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

//*****************************************************************************
// 2D�|���S���N���X
//*****************************************************************************
class CExplosion : public CScene2D
{
public:
	CExplosion();
	~CExplosion();
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̊J��

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CExplosion *Create(D3DXVECTOR3 pos,float size);
private:

	static LPDIRECT3DTEXTURE9	m_pTexture;				//���L�e�N�X�`���ւ̃|�C���^

	int m_nCounterAnim;			//�J�E���^�[
	int m_nPatternAnim;			//�p�^�[��No.
};

#endif