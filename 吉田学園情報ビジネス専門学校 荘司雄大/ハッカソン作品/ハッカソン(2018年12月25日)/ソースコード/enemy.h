//*****************************************************************************
//
//     �G�̏���[enemy.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "character.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CMotion;
class CGaugeLife;

//*****************************************************************************
//    �G�l�~�[�Ǌ��N���X�̒�`
//*****************************************************************************
class CEnemyManager : CScene
{
public:    // �N�ł��A�N�Z�X�\
	CEnemyManager(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMYMANAGER);
	~CEnemyManager();

	static CEnemyManager *Create(char *pFileName, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetFileName(char *pFileName);
	void SetModelManager(CModelManager *pModelManager);

	char *GetFileName(void);
	CModelManager *GetModelManager(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void SetEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, char *pSetFileName);
	void CreateEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nLife, const int nAddScore = 1000);
	CModel **ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pGravity, float *pColHeight, float *pColRange);
	CModel *ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel);
	CMotion *ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	void ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);

	char          m_aFileName[256];  // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
	CModelManager *m_pModelManager;  // ���f���Ǌ��N���X�ւ̃|�C���^
	int m_nCntPop;					 // �|�b�v���鏇��
};

//*****************************************************************************
//    �G�l�~�[�N���X�̒�`
//*****************************************************************************
class CEnemy : public CCharacter
{
public:    // �N�ł��A�N�Z�X�\
	typedef enum
	{
		STATE_NORMAL = 0,   // �ʏ���
		STATE_MOVE,         // �ړ����
		STATE_FALL,         // ���������[�V����
		STATE_LANDING,      // ���n���
		STATE_DEATH,        // ���S���
		STATE_MAX
	}STATE;

	CEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemy();

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore = 1000, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife,int nAddScore = 1000);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE State);
	void SetLife(const int nLife);
	void SetAddScore(const int nAddScore);

	STATE GetState(void);
	int GetLife(void);
	int GetAddScore(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Collision(void);
	void Statement(void);
	void Death(void);
	bool CollisionPlayer(D3DXVECTOR3 pos, float fRange);

	STATE       m_State;       // ���
	D3DXVECTOR3 m_Move;        // �ړ���
	int         m_nLife;       // �̗�
	int         m_nAddScore;   // �|�������ɑ�����X�R�A��
	CGaugeLife  *m_pGaugeLife; // �̗̓Q�[�W
	bool		m_bMoveRot;	   // �i�s����
};

#endif