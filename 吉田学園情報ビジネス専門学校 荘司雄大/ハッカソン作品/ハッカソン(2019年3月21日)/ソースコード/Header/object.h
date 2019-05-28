//*****************************************************************************
//
//     �I�u�W�F�N�g�̏���[object.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "3DPolygon.h"
#include "3DMesh.h"

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
class CCollision;

//*****************************************************************************
//    �z�u�r���{�[�h�N���X�̒�`
//*****************************************************************************
class CBillboardObject : public CSceneBillboard
{
public:    // �N�ł��A�N�Z�X�\
	//----------------------
	// ���
	//----------------------
	typedef enum
	{
		TYPE_NONE = 0,   // �����Ȃ�
		TYPE_BAZOOKA,    // �o�Y�[�J
		TYPE_HORMING,    // �z�[�~���O
		TYPE_WIRE,       // ���C���[
		TYPE_MAX
	}TYPE;

	CBillboardObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_BILLOBJ);
	~CBillboardObject();

	static CBillboardObject *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, int nType = 0, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, int nType = 0);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void SetType(const int nType);
	void SetTexIdx(const int nTexIdx);
	void SetDrawAddtive(const bool bDrawAddtive);

	int GetType(void);
	int GetTexIdx(void);
	bool GetDrawAddtive(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int   m_nType;         // ��ޔԍ�
	int   m_nTexIdx;       // �g�p���Ă���e�N�X�`���̔ԍ�
	bool  m_bDrawAddtive;  // ���Z�����ŕ`�悷�邩���Ȃ���
};

//*****************************************************************************
//    �R�N���X�̒�`
//*****************************************************************************
class CMountain : public CMeshCylinder
{
public:    // �N�ł��A�N�Z�X�\
	CMountain(int nPriority = 3, OBJTYPE objType = OBJTYPE_MOUNTAIN);
	~CMountain();
	static CMountain *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeNormal(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};

//*****************************************************************************
//    �z�u���N���X�̒�`
//*****************************************************************************
class CObject : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECT);
	~CObject();

	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, int nNumParts, int nNumCollision, CModel **pModel, CMotionManager *pMotionManager, CCollision **pCollision, bool bBreak = false, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand, CScene *pRideObject);
	void Damage(int nDamage);

	void SetType(const int nType);
	void SetNumParts(const int nNumParts);
	void SetNumCollision(const int nNumCollision);
	void SetModel(CModel **pModel);
	void SetMotionManager(CMotionManager *pMotionManager);
	void SetCollision(CCollision **pCollision);
	void SetPos(const D3DXVECTOR3 pos);
	void SetPosOld(const D3DXVECTOR3 posold);
	void SetPosStd(const D3DXVECTOR3 posstd);
	void SetRot(const D3DXVECTOR3 rot);
	void SetRotStd(const D3DXVECTOR3 rotstd);
	void SetScale(const D3DXVECTOR3 scale);
	void SetRollSpeed(const D3DXVECTOR3 RollSpeed);
	void SetBreak(const bool bBreak);

	int GetType(void);
	int GetNumParts(void);
	int GetNumCollision(void);
	CModel **GetModel(void);
	CMotionManager *GetMotionManager(void);
	CCollision **GetCollision(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetPosStd(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetRotStd(void);
	D3DXVECTOR3 GetScale(void);
	D3DXVECTOR3 GetRollSpeed(void);
	bool GetBreak(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int             m_nType;                 // �z�u���̎��
	int             m_nNumParts;             // �g�p����p�[�c��
	int             m_nNumCollision;         // �����蔻�����鐔
	CMotionManager  *m_pMotionManager;       // ���[�V�����N���X�ւ̃|�C���^
	CModel          **m_apModel;             // ���f���N���X�ւ̃|�C���^
	CCollision      **m_apCollision;         // �����蔻��N���X�ւ̃|�C���^
	D3DXMATRIX      m_MtxWorld;              // ���[���h�}�g���b�N�X
	D3DXVECTOR3     m_Pos;                   // ���݂̍��W
	D3DXVECTOR3     m_PosOld;                // �O��̍��W
	D3DXVECTOR3     m_PosStd;                // �������̍��W
	D3DXVECTOR3     m_Rot;                   // ���݂̌���
	D3DXVECTOR3     m_RotStd;                // �������̌���
	D3DXVECTOR3     m_Scale;                 // �傫��
	D3DXVECTOR3     m_RollSpeed;             // ��������]������X�s�[�h
	bool            m_bBreak;                // �󂹂�I�u�W�F�N�g���ǂ���
	int             m_nLife;                 // �̗�
};


//*****************************************************************************
//    �X�^�[�z�u���N���X�̒�`
//*****************************************************************************
class CObjectStar : public CObject
{
public:    // �N�ł��A�N�Z�X�\
	CObjectStar(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTSTAR);
	~CObjectStar();

	static CObjectStar *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, int nNumParts, int nNumCollision, CModel **pModel, CMotionManager *pMotionManager, CCollision **pCollision, bool bBreak = false, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Death(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

//*****************************************************************************
//    �z�u���ǂݎ��p�N���X�̒�`
//*****************************************************************************
class CObjectManager
{
public:    // �N�ł��A�N�Z�X�\
	CObjectManager();
	~CObjectManager();

	static CObjectManager *Create(char *pFileName, CModelManager *pModelManager);

	HRESULT Init(void);
	void Uninit(void);
	void CreateObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, bool bBreak = false, int nPriority = 3);
	void CreateStarObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, bool bBreak = false, int nPriority = 3);

	void SetFileName(const char *pFileName);
	void SetNumParts(const int nNumParts);
	void SetNumCollision(const int nNumCollision);
	void SetModel(CModel **pModel);
	void SetModelManager(CModelManager *pModelManager);
	void SetMotionManager(CMotionManager *pMotionManager);
	void SetCollision(CCollision **pCollision);

	char *GetFileName(void);
	int GetNumParts(void);
	int GetNumCollision(void);
	CModel **GetModel(void);
	CModelManager *GetModelManager(void);
	CMotionManager *GetMotionManager(void);
	CCollision **GetCollision(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CModel **ReadModelSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts);
	CModel *ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel);
	CMotionAttack *ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile);
	CMotionCollision **ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData);
	CMotionOrbit **ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData);
	CMotion *ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	void ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	CCollision **ReadCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumCollision);
	CCollision *ReadCollision(char *pLine, char *pStrCur, FILE *pFile, CCollision **apCollision);

	char           m_aFileName[256];     // �t�@�C����
	int            m_nNumParts;          // �p�[�c��
	int            m_nNumCollision;      // �����蔻�����鐔
	int            *m_pNumParent;        // �e���f���̔ԍ�
	int            *m_pNumColParent;     // �����蔻��̐e�ԍ�
	int            *m_pNumModelIdx;      // �g�p���郂�f���̔ԍ�
	CModel         **m_apModel;          // ���f���N���X�ւ̃|�C���^
	CModelManager  *m_pModelManager;     // ���f���Ǌ��N���X�ւ̃|�C���^
	CMotionManager *m_pMotionManager;    // ���[�V�����N���X�ւ̃|�C���^
	CCollision     **m_apCollision;      // �R���W�����N���X�ւ̃|�C���^
};

//*****************************************************************************
//    ��N���X�̒�`
//*****************************************************************************
class CSky : public CMeshDome
{
public:    // �N�ł��A�N�Z�X�\
	CSky(int nPriority = 3, OBJTYPE objType = OBJTYPE_SKY);
	~CSky();
	static CSky *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, float fRotSpeed, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, float fRotSpeed);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeNormal(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	float m_fRotSpeed;     // ��]����X�s�[�h
};

#endif