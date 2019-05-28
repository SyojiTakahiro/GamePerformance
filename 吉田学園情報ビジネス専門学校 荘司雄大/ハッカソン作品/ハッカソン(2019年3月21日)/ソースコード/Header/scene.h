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
#define MAX_PRIORITY_NUM (14)      // �����̗D�揇�ʂ̍ő吔
#define MAX_SCENE        (20000)   // �V�[�������鐔

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
		OBJTYPE_NONE = -1,     // ����
		OBJTYPE_2DPOLYGON,     // 2D�|���S��
		OBJTYPE_3DPOLYGON,     // 3D�|���S��
		OBJTYPE_BILLBOARD,     // �r���{�[�h
		OBJTYPE_MESHFIELD,     // ���b�V���t�B�[���h
		OBJTYPE_MESHWALL,      // ���b�V���E�H�[��
		OBJTYPE_MESHCYLINDER,  // ���b�V���V�����_�[
		OBJTYPE_MESHDOME,      // ���b�V���h�[��
		OBJTYPE_MESHSPHERE,    // ���b�V���X�t�B�A
		OBJTYPE_MESHORBIT,     // ���b�V���I�[�r�b�g
		OBJTYPE_MESHRING,      // ���b�V�������O
		OBJTYPE_CHARACTER,     // �L�����N�^�[
		OBJTYPE_PLAYER,        // �v���C���[
		OBJTYPE_ENEMY,         // �G
		OBJTYPE_BILLOBJ,       // �z�u�r���{�[�h
		OBJTYPE_MOUNTAIN,      // �R
		OBJTYPE_OBJECT,        // �z�u��
		OBJTYPE_OBJECTSTAR,    // �X�^�[�p�̔z�u��
		OBJTYPE_SKY,           // ��
		OBJTYPE_EMITTER,       // �G�~�b�^
		OBJTYPE_PAREMITTER,    // �p�[�e�B�N���G�~�b�^
		OBJTYPE_RINGEMITTER,   // �����O�G�~�b�^
		OBJTYPE_PARTICLE,      // �p�[�e�B�N��
		OBJTYPE_RINGEFFECT,    // �����O�G�t�F�N�g
		OBJTYPE_STAR,          // �X�^�[
		OBJTYPE_BULLET,        // �e
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
	static CScene *GetTop(int nPriority);

	void SetObjType(OBJTYPE ObjType);
	OBJTYPE GetObjType(void);
	CScene *GetNext(void);
	CScene *GetPrev(void);
	int GetPriority(void);

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