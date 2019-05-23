//=============================================================================
//
// �e���� [bullet.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

//*****************************************************************************
// 2D�|���S���N���X
//*****************************************************************************
class CBullet : public CScene2D
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		BULLETTYPE_PLAYER,
		BULLETTYPE_ENEMY,
		BULLETTYPE_MAX
	}BULLETTYPE;

	CBullet();
	~CBullet();
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̊J��

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBullet *Create(D3DXVECTOR3 pos ,D3DXVECTOR3 move, D3DXCOLOR col,float size,int life, BULLETTYPE bulletType);
private:

	static LPDIRECT3DTEXTURE9	m_pTexture;				//���L�e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3					m_move;					//�ړ���
	D3DXCOLOR					m_col;

	int m_nBulletLife;
	int m_nPlayerLife;
	int m_DamageRecast;
	BULLETTYPE m_bulletType;

	/*float					m_fBulletSpin;
	float					m_fBulletScale;
	float					m_fBulletLength;*/
};

#endif