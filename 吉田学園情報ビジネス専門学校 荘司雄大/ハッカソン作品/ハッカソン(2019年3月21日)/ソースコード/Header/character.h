//*****************************************************************************
//
//     �L�����N�^�[�̏���[character.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CModel;
class CMotionAttack;
class CMotionCollision;
class CMotionOrbit;
class CMotion;
class CMotionManager;
class CModelManager;
class CTextureManager;
class CShadow;
class CCamera;

//*****************************************************************************
//    �L�����N�^�[�N���X�̒�`
//*****************************************************************************
class CCharacter : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CCharacter(int nPriority = 3, OBJTYPE objType = OBJTYPE_CHARACTER);
	~CCharacter();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(LPDIRECT3DDEVICE9 pDevice);
	void SetDefaultPos(void);
	void SetDefaultRot(void);
	void SetModelAlpha(float fAlpha);

	void SetNumParts(const int nNumParts);
	void SetModel(CModel **pModel);
	void SetModel(CModel *pModel, const int nIdx);
	void SetMotionManager(CMotionManager *pMotionManager);
	void SetTextureManager(CTextureManager *pTextureManager);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetPos(const D3DXVECTOR3 pos);
	void SetPosOld(const D3DXVECTOR3 posOld);
	void SetMove(const D3DXVECTOR3 Move);
	void SetRot(const D3DXVECTOR3 rot);
	void SetDestRot(const D3DXVECTOR3 Destrot);
	void SetDiffRot(const D3DXVECTOR3 Diffrot);
	void SetGravity(const float fGravity);
	void SetColHeight(const float fColHeight);
	void SetColRange(const float fColRange);
	void SetJumpPower(const float fJumpPower);
	void SetRivisionRot(const float fRivisionRot);
	void SetAccel(const float fAccel);
	void SetInertia(const float fInertia);
	void SetLand(const bool bLand);
	void SetLandScene(CScene *pLandScene);
	void SetLandSceneOld(CScene *pLandSceneOld);
	void SetLandOffsetPos(const D3DXVECTOR3 LandOffsetPos);
	void SetLandOffsetAdd(const D3DXVECTOR3 LandOffsetAdd);
	void SetLandOffsetRot(const D3DXVECTOR3 LandOffsetRot);
	void SetLandOffsetRotStart(const D3DXVECTOR3 LandOffsetRotStart);
	void SetShadow(CShadow *pShadow);

	int GetNumParts(void);
	CModel **GetModel(void);
	CModel *GetModel(const int nIdx);
	CMotionManager *GetMotionManager(void);
	CTextureManager *GetTextureManager(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetDestRot(void);
	D3DXVECTOR3 GetDiffRot(void);
	float GetGravity(void);
	float GetColHeight(void);
	float GetColRange(void);
	float GetJumpPower(void);
	float GetRivisionRot(void);
	float GetAccel(void);
	float GetInertia(void);
	bool GetLand(void);
	CScene *GetLandScene(void);
	CScene *GetLandSceneOld(void);
	D3DXVECTOR3 GetLandOffsetPos(void);
	D3DXVECTOR3 GetLandOffsetAdd(void);
	D3DXVECTOR3 GetLandOffsetRot(void);
	D3DXVECTOR3 GetLandOffsetRotStart(void);
	CShadow *GetShadow(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void Movement(void);
	void Collision(void);
	void CalcOffsetRot(void);

private:   // ���̃N���X�������A�N�Z�X�\

	int             m_nNumParts;             // �g�p����p�[�c��
	CModel          **m_apModel;             // ���f���N���X�ւ̃|�C���^
	CMotionManager  *m_pMotionManager;       // ���[�V�����N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;      // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	D3DXMATRIX      m_MtxWorld;              // ���[���h�}�g���b�N�X
	D3DXVECTOR3     m_Pos;                   // ���݂̍��W
	D3DXVECTOR3     m_PosOld;                // �O��̍��W
	D3DXVECTOR3     m_Move;                  // �ړ���
	D3DXVECTOR3     m_Rot;                   // ���݂̌���
	D3DXVECTOR3     m_DestRot;               // �ړI�̌���
	D3DXVECTOR3     m_DiffRot;               // �ړI�̌����ƌ��݂̌����̍���
	float           m_fGravity;              // �d��
	float           m_fColHeight;            // �����蔻����Ƃ鍂��
	float           m_fColRange;             // �����蔻������͈�
	float           m_fJumpPower;            // �W�����v��
	float           m_fRivisionRot;          // ������␳����{��
	float           m_fAccel;                // �ړ��X�s�[�h
	float           m_fInertia;              // ����
	bool            m_bLand;                 // �O��I�u�W�F�N�g�ɏ���Ă������ǂ���
	CScene          *m_pLandScene;           // ����Ă���I�u�W�F�N�g�ւ̃|�C���^
	CScene          *m_pLandSceneOld;        // �O�����Ă����I�u�W�F�N�g�ւ̃|�C���^
	D3DXVECTOR3     m_LandOffsetPos;         // �I�u�W�F�N�g����̃I�t�Z�b�g����
	D3DXVECTOR3     m_LandOffsetAdd;         // �I�t�Z�b�g�����ɉ�����l
	D3DXVECTOR3     m_LandOffsetRot;         // �I�u�W�F�N�g����̃I�t�Z�b�g����
	D3DXVECTOR3     m_LandOffsetRotStart;    // ����Ă���̌���
	CShadow         *m_pShadow;              // �e�N���X�ւ̃|�C���^
};

//*****************************************************************************
//    �v���C���[�N���X�̒�`
//*****************************************************************************
class CPlayer : public CCharacter
{
public:    // �N�ł��A�N�Z�X�\
	//--------------------
    //  ��Ԃ̎��
	//--------------------
	typedef enum
	{
		STATE_NORMAL = 0,   // �ʏ���
		STATE_WINNER,       // �������
		STATE_LOSER,        // �s�k���
		STATE_MAX
	}STATE;

	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, int nControllerIdx, int nPriority = 3);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, int nControllerIdx);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Damage(D3DXVECTOR3 BulletMove);
	void AddScore(void);

	void SetState(const STATE State);
	void SetControllerIdx(const int nIdx);
	void SetCamera(CCamera *pCamera);

	STATE GetState(void);
	int GetControllerIdx(void);
	CCamera *GetCamera(void);
	int GetScore(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void InputMove(void);
	void Action(void);
	void Statement(void);
	void Landing(void);
	bool AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset, int nDamage);

	void CreateBoostEffect(void);

	STATE m_State;              // ���
	int   m_nControllerIdx;     // �R���g���[���[�̔ԍ�
	CCamera *m_pCamera;         // �J�����N���X�ւ̃|�C���^
	bool m_bDamage;             // �_���[�W���󂯂����ǂ���
	bool m_bAttack;             // �U�������炤���ǂ���
	int m_nDamageCounter;       // �_���[�W���󂯂����Ԃ��Ǘ�����J�E���^�[
	int m_nNumScore;            // �L�����N�^�[�������W�߂���
};

//*****************************************************************************
//    �G�N���X�̒�`
//*****************************************************************************
class CEnemy : public CCharacter
{
public:    // �N�ł��A�N�Z�X�\
	//--------------------
	//  ���
	//--------------------
	typedef enum
	{
		TYPE_NORMAL = 0,   // �ʏ�̎��
		TYPE_MAX
	}TYPE;

	CEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemy();

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, int nPriority = 3);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};

//*****************************************************************************
//    �L�����N�^�[�Ǌ��N���X�̒�`
//*****************************************************************************
class CCharacterManager
{
public:    // �N�ł��A�N�Z�X�\
	CCharacterManager();
	~CCharacterManager();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	void SetFileName(char *pFileName);
	void SetModelManager(CModelManager *pModelManager);
	void SetTextureManager(CTextureManager *pTextureManager);

	char *GetFileName(void);
	CModelManager *GetModelManager(void);
	CTextureManager *GetTextureManager(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	CModel **ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pAccel, float *pInertia, float *pJumpPower, float *pRivisionRot, float *pGravity, float *pColHeight, float *pColRange);
	CModel *ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel);
	CMotionAttack *ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile);
	CMotionCollision **ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData);
	CMotionOrbit **ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData);
	CMotion *ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	void ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);

private:   // ���̃N���X�������A�N�Z�X�\
	char            m_aFileName[256];    // �L�����N�^�[�f�[�^�̃X�N���v�g�t�@�C����
	CModelManager   *m_pModelManager;    // ���f���Ǌ��N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;  // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
};

//*****************************************************************************
//    �v���C���[�Ǌ��N���X�̒�`
//*****************************************************************************
class CPlayerManager : public CCharacterManager
{
public:    // �N�ł��A�N�Z�X�\
	CPlayerManager();
	~CPlayerManager();

	static CPlayerManager *Create(char *pFileName, int nControllerIdx = 0);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetControllerIdx(const int nControllerIdx);

	int GetControllerIdx(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreatePlayer(CModel **apModel, CModelManager *pModelManager , CTextureManager *pTextureManager, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange);

	int m_nControllerIdx;
};

//*****************************************************************************
//    �G�l�~�[�Ǌ��N���X�̒�`
//*****************************************************************************
class CEnemyManager : public CCharacterManager
{
public:    // �N�ł��A�N�Z�X�\
	CEnemyManager();
	~CEnemyManager();

	static CEnemyManager *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreateEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange);
};

//*****************************************************************************
//    �G�l�~�[�Ǌ��܂Ƃ߃N���X�̒�`
//*****************************************************************************
class CEnemyAssembly
{
public:    // �N�ł��A�N�Z�X�\
	CEnemyAssembly();
	~CEnemyAssembly();

	void SetEnemyManager(CEnemyManager *pEnemyManager, int nIdx);
	CEnemyManager *GetEnemyManager(int nIdx);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CEnemyManager *m_pEnemyManager[CEnemy::TYPE_MAX];   // �G�Ǌ��N���X�ւ̃|�C���^
};

#endif