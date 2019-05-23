//=============================================================================
//
// �p�[�e�B�N������ [particle.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

#define MAX_PARTICLE (30)

//*****************************************************************************
// 2D�|���S���N���X
//*****************************************************************************
class CParticle : public CScene2D
{
public:
	CParticle();
	~CParticle();
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̊J��

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int life);
private:

	static LPDIRECT3DTEXTURE9	m_pTexture;				//���L�e�N�X�`���ւ̃|�C���^

	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_posParticle;
	D3DXCOLOR m_col;
	float m_fRadius;		//
	int	  m_nLife;
};

#endif