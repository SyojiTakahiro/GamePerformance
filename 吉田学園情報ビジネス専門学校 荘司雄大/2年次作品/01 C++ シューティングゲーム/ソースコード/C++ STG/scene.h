//=============================================================================
//
// �V�[������ [scene.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �I�u�W�F�N�g�N���X
//*****************************************************************************

class CScene
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,
		OBJTYPE_ENEMY,
		OBJTYPE_BOSS,
		OBJTYPE_BULLET,
		OBJTYPE_EXBULLET,
		OBJTYPE_SCORE,
		OBJTYPE_GAUGE,
		OBJTYPE_GAUGE2,
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority = 3);
	virtual ~CScene();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	void SetObjType(OBJTYPE objType);
	OBJTYPE GetObjType(void);

// �ÓI�����o�֐�
	static CScene *GetScene(int nPriority,int nIdx);
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

protected:
	void Release(void);

private:
	static CScene *m_apScene[MAX_PRI][MAX_SCENE];
	static int m_nNumAll;
	int m_nID;	//�������g�̔ԍ�
	OBJTYPE m_objType; //�I�u�W�F�N�g�̎��
	int m_nPriority;
};

#endif