//=============================================================================
//
// �G�t�F�N�g���� [effect.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

//*****************************************************************************
// 2D�|���S���N���X
//*****************************************************************************
class CEffect : public CScene2D
{
public:
	CEffect();
	~CEffect();
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̊J��

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius,int life);
private:

	static LPDIRECT3DTEXTURE9	m_pTexture;				//���L�e�N�X�`���ւ̃|�C���^

	int	  m_nLife;			//
	D3DXVECTOR3 m_posEffect;
	float m_fRadius;			//
	D3DXCOLOR m_col;
};

#endif