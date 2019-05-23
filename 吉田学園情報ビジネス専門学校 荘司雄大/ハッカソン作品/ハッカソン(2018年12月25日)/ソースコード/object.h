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
//    �I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CObject : public CSceneX
{
public:    // �N�ł��A�N�Z�X�\
	CObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECT);
	~CObject();

	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);
	void SetTransParent(float fAlpha);

	void SetDisp(const bool bDisp);
	void SetRollSpeed(const D3DXVECTOR3 RollSpeed);
	void SetPosOld(const D3DXVECTOR3 PosOld);
	void SetModelIdx(const int ModelIdx) { m_ModelIdx = ModelIdx; };
	void SetRotStd(const D3DXVECTOR3 RotStd);

	bool GetDisp(void);
	int GetModelIdx(void) { return m_ModelIdx; };
	D3DXVECTOR3 GetRollSpeed(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetRotStd(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	bool        m_bDisp;        // �`�悷�邩���Ȃ���
	int         m_nDispCounter; // �`����Ǘ�����J�E���^�[
	int         m_ModelIdx;     // �g�p���Ă��郂�f���̔ԍ�
	D3DXVECTOR3 m_PosOld;       // �O��̍��W
	D3DXVECTOR3 m_RollSpeed;    // ��������]������X�s�[�h
	D3DXVECTOR3 m_PosStd;       // ���f���̍��W(������)
	D3DXVECTOR3 m_RotStd;       // ���f���̌���(������)
};

//*****************************************************************************
//    �����I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CObjectMove : public CObject
{
public:    // �N�ł��A�N�Z�X�\
	CObjectMove(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTMOVE);
	~CObjectMove();

	static CObjectMove *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);

	void SetMove(const D3DXVECTOR3 Move);
	void SetMoveCurrent(const D3DXVECTOR3 MoveCurrent);
	void SetCounter(const int nCounter);
	void SetMaxMoveCounter(const int nMaxMoveCounter);
	void SetRivisionMove(const float fRivisionMove);
	void SetPosStd(const D3DXVECTOR3 PosStd);

	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetMoveCurrent(void);
	int GetCounter(void);
	int GetMaxMoveCounter(void);
	float GetRivisionMove(void);
	D3DXVECTOR3 GetPosStd(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3 m_Move;             // �ړ���
	D3DXVECTOR3 m_MoveCurrent;      // ���݂̈ړ���
	int         m_nCounter;         // �ړ��ʂ��Ǘ�����J�E���^�[
	int         m_nMaxMoveCounter;  // �ړ��ʂ��ő�l�ɂȂ鎞��
	float       m_fRivisionMove;    // �ړ��̔{��
	D3DXVECTOR3 m_PosStd;           // ���f���̍��W(������)
};


//*****************************************************************************
//    �S�[���p�I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CObjectGoal : public CObject
{
public:    // �N�ł��A�N�Z�X�\
	CObjectGoal(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTGOAL);
	~CObjectGoal();

	static CObjectGoal *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};
#endif