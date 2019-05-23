//*****************************************************************************
//
//     �v���C���[�̏���[player.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "character.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CMotionAttack;
class CMotionCollision;
class CMotionOrbit;
class CMotionManager;
class CMotion;
class CModelManager;
class COrbitEffect;
class CEffectManager;

//*****************************************************************************
//    �v���C���[�Ǌ��N���X�̒�`
//*****************************************************************************
class CPlayerManager : CScene
{
public:    // �N�ł��A�N�Z�X�\
	CPlayerManager(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYERMANAGER);
	~CPlayerManager();

	static CPlayerManager *Create(char *pFileName, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetFileName(char *pFileName);
	void SetModelManager(CModelManager *pModelManager);

	char *GetFileName(void);
	CModelManager *GetModelManager(void);

	void SetRespawn(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CreatePlayer(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange);
	CModel **ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pAccel, float *pInertia, float *pJumpPower, float *pRivisionRot, float *pGravity, float *pColHeight, float *pColRange);
	CModel *ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel);
	CMotionAttack *ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile);
	CMotionCollision **ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData);
	CMotionOrbit **ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData);
	CMotion *ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	void ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);

	char          m_aFileName[256];      // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
	CModelManager *m_pModelManager;      // ���f���Ǌ��N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;  // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^


	// �R�s�[�p�ϐ�
	CMotionManager *m_pMotionManager;
	CModel         **m_apModel;
	int m_nNumParts;
	int *m_pNumParent;
	float m_fAccel;
	float m_fInertia;
	float m_fJumpPower;
	float m_fRivisionRot;
	float m_fGravity;
	float m_fColHeight;
	float m_fColRange;
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
		STATE_MOVE,         // �ړ����
		STATE_HOLEOPEN,     // �܂Ō���������
		STATE_FALL,         // ���������[�V����
		STATE_LANDING,      // ���n���
		STATE_DEATH,        // ���S���
		STATE_MAX
	}STATE;

	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, int nPriority = 3);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE State);
	void SetMove(const D3DXVECTOR3 move);
	void SetAccel(const float fAccel);
	void SetInertia(const float fInertia);
	void SetJumpPower(const float fJumpPower);
	void SetRivisionRot(const float fRivisionRot);
	void SetJump(const bool bJump);
	void SetAction(const bool bAction);

	STATE GetState(void);
	D3DXVECTOR3 GetMove(void);
	float GetAccel(void);
	float GetInertia(void);
	float GetJumpPower(void);
	float GetRivisionRot(void);
	bool GetJump(void);
	bool GetAction(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void InputMove(void);
	void Movement(void);
	void Collision(void);
	void Action(void);
	void Statement(void);
	void Landing(void);
	void Death(void);
	void BlockBreak(void);
	void BlockDelete(void);

	bool AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset);
	void CreateEffect(void);
	void CalcOffsetRot(void);

	COrbitEffect    *m_pOrbitEffect;      // �O�ՃG�t�F�N�g�N���X�ւ̃|�C���^
	CEffectManager  *m_pEffectManager;    // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	STATE           m_State;              // ���
	D3DXVECTOR3     m_Move;               // �ړ���
	float           m_fAccel;             // �����x
	float           m_fInertia;           // ����
	float           m_fJumpPower;         // �W�����v��
	float           m_fRivisionRot;       // ������␳����{��
	bool            m_bJump;              // �W�����v���Ă��邩�ǂ���
	bool            m_bAction;            // �A�N�V���������Ă��邩�ǂ���
	bool            m_bOrbit;             // �O�Ղ��o���Ă��邩�ǂ���
	bool            m_bBooking;           // �A�N�V�����̗\�񂪓����Ă��邩�ǂ���
};

#endif