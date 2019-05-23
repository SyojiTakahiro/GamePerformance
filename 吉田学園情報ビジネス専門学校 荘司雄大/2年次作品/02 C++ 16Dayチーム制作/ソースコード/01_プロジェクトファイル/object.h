//*****************************************************************************
//
//     �z�u���̏���[object.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "sceneX.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MAX_BALLOON	(5)		// �o���[���I�u�W�F�N�g�̐�

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CShadow;
class CBalloon;
class CPlayer;

//*****************************************************************************
//    �I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CObject : public CSceneX
{
public:    // �N�ł��A�N�Z�X�\
	CObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECT);
	~CObject();

	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, bool bPush, bool bBreak, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, bool bPush, bool bBreak);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, CShadow *pShadow, float fHeight, float fRange);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange);

	D3DXVECTOR3 GetRollSpeed(void);
	CShadow *GetShadow(void);
	bool GetCollision(void);
	bool GetBreak(void);
	bool GetPush(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPD3DXMESH     m_apMesh;      // ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER   m_apBuffMat;   // �}�e���A�����ւ̃|�C���^
	static DWORD          m_anNumMat;    // �}�e���A�����̐�
	static D3DXVECTOR3    m_aVtxMin;     // ���_���W�̍ŏ��l
	static D3DXVECTOR3    m_aVtxMax;     // ���_���W�̍ő�l

	bool        m_bCollision; // �����蔻�����邩�ǂ���
	bool        m_bPush;      // �����邩�ǂ���
	bool        m_bBreak;     // �󂹂邩�ǂ���
	D3DXVECTOR3 m_RollSpeed;  // ��������]������X�s�[�h
	CShadow     *m_pShadow;   // �e�N���X�ւ̃|�C���^
};

//*****************************************************************************
//    �����I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CObjectMove : public CObject
{
public:    // �N�ł��A�N�Z�X�\
	CObjectMove(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTMOVE);
	~CObjectMove();

	static CObjectMove *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 move, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 move);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, CShadow *pShadow, float fHeight, float fRange);

	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetMoveCurrent(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPD3DXMESH     m_apMesh;      // ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER   m_apBuffMat;   // �}�e���A�����ւ̃|�C���^
	static DWORD          m_anNumMat;    // �}�e���A�����̐�
	static D3DXVECTOR3    m_aVtxMin;     // ���_���W�̍ŏ��l
	static D3DXVECTOR3    m_aVtxMax;     // ���_���W�̍ő�l

	D3DXVECTOR3 m_Move;          // �ړ���
	D3DXVECTOR3 m_MoveCurrent;   // ���݂̈ړ���
	int         m_nMoveCounter;  // �ړ����Ǘ�����J�E���^�[
	float       m_fMoveRivision; // �ړ��ʂ�␳����{��
};


//*****************************************************************************
//    �����o���p�I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CObjectBalloon : public CObject
{
public:    // �N�ł��A�N�Z�X�\
	typedef enum
	{
		BALLOON_TYPE_0 = 0,
		BALLOON_TYPE_1,
		BALLOON_TYPE_2,
		BALLOON_TYPE_3,
		BALLOON_TYPE_4,
		BALLOON_TYPE_MAX
	}BALLOON;

	CObjectBalloon(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTBALLOON);
	~CObjectBalloon();

	static CObjectBalloon *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, BALLOON MapBalloon, char *pNextMapName, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, char *pNextMapName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPD3DXMESH     m_apMesh;      // ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER   m_apBuffMat;   // �}�e���A�����ւ̃|�C���^
	static DWORD          m_anNumMat;    // �}�e���A�����̐�
	static D3DXVECTOR3    m_aVtxMin;     // ���_���W�̍ŏ��l
	static D3DXVECTOR3    m_aVtxMax;     // ���_���W�̍ő�l

	int m_nType;						 //�����o���̊g�傩�k�����̃^�C�v
	int m_nCount;						 //�����o����1�񂾂��o���J�E���^�[
	BALLOON m_MapBalloon;
	CBalloon *m_pBalloon[BALLOON_TYPE_MAX];
	char m_aNextMapName[256];
};


//*****************************************************************************
//    �S�[���p�I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CObjectGoal : public CObject
{
public:    // �N�ł��A�N�Z�X�\
	CObjectGoal(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTGOAL);
	~CObjectGoal();

	static CObjectGoal *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPD3DXMESH     m_apMesh;      // ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER   m_apBuffMat;   // �}�e���A�����ւ̃|�C���^
	static DWORD          m_anNumMat;    // �}�e���A�����̐�
	static D3DXVECTOR3    m_aVtxMin;     // ���_���W�̍ŏ��l
	static D3DXVECTOR3    m_aVtxMax;     // ���_���W�̍ő�l
};


//*****************************************************************************
//    �W�b�v���C���p�I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CObjectTarget : public CObject
{
public:    // �N�ł��A�N�Z�X�\
	typedef enum
	{
		STATE_NONE = 0,  // �������Ă��Ȃ����
		STATE_CARRY,     // �v���C���[���^��ł�����
		STATE_FINISH,    // �^�яI��������
		STATE_MAX
	}STATE;

	CObjectTarget(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTTARGET);
	~CObjectTarget();

	static CObjectTarget *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 PosTarget, int nMaxCarry, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 PosTarget, int nMaxCarry);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, CShadow *pShadow, float fHeight, float fRange, CScene *pScene);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Carry(void);

	static LPD3DXMESH     m_apMesh;      // ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER   m_apBuffMat;   // �}�e���A�����ւ̃|�C���^
	static DWORD          m_anNumMat;    // �}�e���A�����̐�
	static D3DXVECTOR3    m_aVtxMin;     // ���_���W�̍ŏ��l
	static D3DXVECTOR3    m_aVtxMax;     // ���_���W�̍ő�l

	int         m_nMaxCarryTime;     // ���܂ŉ^�Ԃ��̎���
	int         m_nCounter;          // �^�т��Ǘ�����J�E���^�[
	STATE       m_State;             // ���
	D3DXVECTOR3 m_PosTarget;         // �v���C���[���^�Ԉʒu
	D3DXVECTOR3 m_DiffPosTarget;     // �ړI�̈ʒu�Ƃ̍���
	CPlayer     *m_pPlayer;          // �v���C���[�N���X�ւ̃|�C���^
};

#endif
