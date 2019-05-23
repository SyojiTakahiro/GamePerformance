//=============================================================================
//
// �e�ؒf���� [bulletEX.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _BULLETEX_H_
#define _BULLETEX_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

//*****************************************************************************
// 2D�|���S���N���X
//*****************************************************************************
class CBulletEX : public CScene2D
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		BULLETTYPE_PLAYER,
		BULLETTYPE_MAX
	}BULLETTYPE;

	CBulletEX();
	~CBulletEX();
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̊J��

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBulletEX *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size, int life, BULLETTYPE bulletType);
private:

	static LPDIRECT3DTEXTURE9	m_pTexture;				//���L�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3					m_move;					//�ړ���

	int m_nBulletLife;
	int m_DamageRecast;
	BULLETTYPE m_bulletType;

	/*float					m_fBulletSpin;
	float					m_fBulletScale;
	float					m_fBulletLength;*/

	int m_nCounterAnim;			//�J�E���^�[
	int m_nPatternAnim;			//�p�^�[��No.
};

#endif