//*****************************************************************************
//
//     �V�[���̏���[scene.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE_H_
#define _SCENE_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define MAX_PRIORITY_NUM (8)      // �����̗D�揇�ʂ̍ő吔
#define MAX_SCENE        (5000)   // �V�[������鐔

//*****************************************************************************
//    �V�[���N���X�̒�`
//*****************************************************************************
class CScene
{
public:    // �N�ł��A�N�Z�X�\
	//------------------------
	//  �I�u�W�F�N�g�̎��
	//------------------------
	typedef enum
	{
		OBJTYPE_NONE = -1,        // ����
		OBJTYPE_TITLE,            // �^�C�g��
		OBJTYPE_TUTORIAL,         // �`���[�g���A��
		OBJTYPE_GAME,             // �Q�[��
		OBJTYPE_RESULT,           // ���U���g
		OBJTYPE_RANKING,          // �����L���O
		OBJTYPE_PAUSE,            // �|�[�Y
		OBJTYPE_NUMBER,           // �����|���S��(2D)
		OBJTYPE_LOGO,             // �����|���S��(2D)
		OBJTYPE_2DPOLYGON,        // 2D�|���S��
		OBJTYPE_TIMER,            // �^�C�}�[
		OBJTYPE_3DPOLYGON,        // 3D�|���S��
		OBJTYPE_MODEL,            // ���f��
		OBJTYPE_BILLBOARD,        // �r���{�[�h
		OBJTYPE_MESH,             // ���b�V��
		OBJTYPE_CYLINDER,         // �V�����_�[
		OBJTYPE_DOME,             // �h�[��
		OBJTYPE_EFFECT,           // �G�t�F�N�g
		OBJTYPE_SHADOW,           // �e
		OBJTYPE_PLAYER,           // �v���C���[
		OBJTYPE_BULLET,           // �e
		OBJTYPE_FIELD,            // �t�B�[���h
		OBJTYPE_WATER,            // ��
		OBJTYPE_OBJECT,           // �z�u��
		OBJTYPE_OBJECTMOVE,       // �����z�u��
		OBJTYPE_OBJECTBALLOON,    // �����o�����o��z�u��
		OBJTYPE_BALLOON,	      // �����o��
		OBJTYPE_OBJECTGOAL,	      // �S�[���p�̃I�u�W�F�N�g
		OBJTYPE_OBJECTTARGET,     // �W�b�v���C���p�̃I�u�W�F�N�g
		OBJTYPE_PARTICLEMANAGER,  // �p�[�e�B�N���Ǘ��p
		OBJTYPE_PARTICLEEMITTER,  // �p�[�e�B�N���G�~�b�^
		OBJTYPE_PARTICLE,         // �p�[�e�B�N��
		OBJTYPE_MAX
	}OBJTYPE;

	CScene(int nPriority = 3, OBJTYPE objType = OBJTYPE_NONE);
	virtual ~CScene();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static int GetNumAll(void);

	static void UpdateAll(void);
	static void DrawAll(void);
	static void ReleaseAll(void);
	static void DeathCheck(void);

	static CScene *GetScene(int nPriority);

	void SetObjType(OBJTYPE ObjType);
	OBJTYPE GetObjType(void);
	CScene *GetNext(void);
	CScene *GetPrev(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void Release(void);

private:   // ���̃N���X�������A�N�Z�X�\

	static int    m_nNumAll;                      // �V�[���̍ő吔
	static CScene *m_apTop[MAX_PRIORITY_NUM];     // �擪�̃V�[���N���X�ւ̃|�C���^(�J�n�ʒu���킩��Ȃ��Ȃ�̂ŕۑ�����)
	static CScene *m_apCur[MAX_PRIORITY_NUM];     // ���݂̍Ō���V�[���N���X�ւ̃|�C���^(�V�����V�[���N���X�ւ̃|�C���^��ǉ����邽�߂ɕۑ�)
	CScene        *m_pPrev;                       // �����̑O�̃V�[���N���X�ւ̃|�C���^
	CScene        *m_pNext;                       // �����̎��̃V�[���N���X�ւ̃|�C���^
	bool          m_bDeath;                       // ���S�t���O
	OBJTYPE       m_ObjType;                      // �I�u�W�F�N�g�̎��
	int           m_nPriority;                    // �D�揇�ʂ̔ԍ�
};

#endif