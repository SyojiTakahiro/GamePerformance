//=============================================================================
//
//	�X�L���Q�[�W���� [gauge2.h]
// Author : Takahiro Syoji
//
//=============================================================================
#ifndef _GAUGE2_H_
#define _GAUGE2_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

//*****************************************************************************
// 2D�|���S���N���X
//*****************************************************************************
class CGauge2 : public CScene2D
{
public:
	CGauge2();
	~CGauge2();
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̊J��

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGauge2 *Create(D3DXVECTOR3 pos);
	static float m_fGauge2;
private:
	static LPDIRECT3DTEXTURE9	m_apTexture[MAX_GAUGE];				//���L�e�N�X�`���ւ̃|�C���^
	CScene2D *m_apCScene2D[MAX_SCENE];

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
	int m_SERecast;

	int m_nCntAnim;
};

#endif
