//=============================================================================
//
// �G���� [enemy.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

#define MAX_ENEMY			(7)

//*****************************************************************************
// 2D�|���S���N���X
//*****************************************************************************
class CEnemy : public CScene2D
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		ENEMYTYPE_1,
		ENEMYTYPE_2,
		ENEMYTYPE_3,
		ENEMYTYPE_4,
		ENEMYTYPE_5,
		ENEMYTYPE_6,
		ENEMYTYPE_TUTORIAL,
		BULLETTYPE_MAX
	}ENEMYTYPE;

	CEnemy();
	~CEnemy();
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̊J��

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEnemy *Create(D3DXVECTOR3 pos,float size,int nLife,ENEMYTYPE enemyType);
	void SetLife(int nLife);
	void CutLife(int nDamage);
	int GetLife(void);

private:
	static LPDIRECT3DTEXTURE9	m_apTexture[MAX_ENEMY];				//���L�e�N�X�`���ւ̃|�C���^

	float					m_fEnemySpin;
	float					m_fEnemyScale;
	float					m_fEnemyLength;

	ENEMYTYPE m_enemyType;

	int m_BulletRecast;
	int m_nLife;
};

#endif