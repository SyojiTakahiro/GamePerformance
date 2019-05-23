//*****************************************************************************
//
//     ���[�V�����̏���[motion.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MOTION_H_
#define _MOTION_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CModel;

//*****************************************************************************
//    ���[�V�����p�U�����[�V�����f�[�^�N���X�̒�`
//*****************************************************************************
class CMotionAttack
{
public:    // �N�ł��A�N�Z�X�\
	CMotionAttack();
	~CMotionAttack();

	void SetNextMotionIdx(const int nNextMotionIdx) { m_nNextMotionIdx = nNextMotionIdx; }
	void SetStartTiming(const int nStartTiming) { m_nStartTiming = nStartTiming; }
	void SetFinishTiming(const int nFinishTiming) { m_nFinishTiming = nFinishTiming; }

	int GetNextMotionIdx(void) { return m_nNextMotionIdx; }
	int GetStartTiming(void) { return m_nStartTiming; }
	int GetFinishTiming(void) { return m_nFinishTiming; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int m_nNextMotionIdx;    // ���̃��[�V�����̔ԍ�
	int m_nStartTiming;      // ���̃��[�V�����ɔh���ł��鎞�Ԃ��n�܂�^�C�~���O
	int m_nFinishTiming;     // ���̃��[�V�����ɔh���ł��鎞�Ԃ��I���^�C�~���O
};

//*****************************************************************************
//    ���[�V�����p�����蔻��f�[�^�N���X�̒�`
//*****************************************************************************
class CMotionCollision
{
public:    // �N�ł��A�N�Z�X�\
	CMotionCollision();
	~CMotionCollision();

	void SetModelIdx(const int nModelIdx) { m_nModelIdx = nModelIdx; }
	void SetOffsetPos(const D3DXVECTOR3 OffsetPos) { m_OffSetPos = OffsetPos; }
	void SetRange(const float fRange) { m_fRange = fRange; }
	void SetStartTiming(const int nStartTiming) { m_nStartTiming = nStartTiming; }
	void SetFinishTiming(const int nFinishTiming) { m_nFinishTiming = nFinishTiming; }

	int GetModelIdx(void) { return m_nModelIdx; }
	D3DXVECTOR3 GetOffsetPos(void) { return m_OffSetPos; }
	float GetRange(void) { return m_fRange; }
	int GetStartTiming(void) { return m_nStartTiming; }
	int GetFinishTiming(void) { return m_nFinishTiming; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int         m_nModelIdx;      // �����蔻�����郂�f���̔ԍ�
	D3DXVECTOR3 m_OffSetPos;      // �����蔻�����郂�f������̃I�t�Z�b�g����
	float       m_fRange;         // �����蔻������͈�
	int         m_nStartTiming;   // �����蔻������n�߂�^�C�~���O
	int         m_nFinishTiming;  // �����蔻����Ȃ����^�C�~���O
};

//*****************************************************************************
//    ���[�V�����p�O�Ճf�[�^�N���X�̒�`
//*****************************************************************************
class CMotionOrbit
{
public:    // �N�ł��A�N�Z�X�\
	CMotionOrbit();
	~CMotionOrbit();

	void SetModelIdx(const int nModelIdx) { m_nModelIdx = nModelIdx; }
	void SetTexIdx(const int nTexIdx) { m_nTexIdx = nTexIdx; }
	void SetOffsetPos1(const D3DXVECTOR3 OffsetPos) { m_OffSetPos1 = OffsetPos; }
	void SetOffsetPos2(const D3DXVECTOR3 OffsetPos) { m_OffSetPos2 = OffsetPos; }
	void SetOffsetAmp1(const D3DXVECTOR3 OffsetAmp) { m_OffSetAmp1 = OffsetAmp; }
	void SetOffsetAmp2(const D3DXVECTOR3 OffsetAmp) { m_OffSetAmp2 = OffsetAmp; }
	void SetColUp(const D3DXCOLOR ColUp) { m_ColUp = ColUp; }
	void SetColDown(const D3DXCOLOR ColDown) { m_ColDown = ColDown; }
	void SetAlphaDecayUp(const float fAlphaDecayUp) { m_fAlphaDecayUp = fAlphaDecayUp; }
	void SetAlphaDecayDown(const float fAlphaDecayDown) { m_fAlphaDecayDown = fAlphaDecayDown; }
	void SetXBlock(const int nXBlock) { m_nXBlock = nXBlock; }
	void SetYBlock(const int nYBlock) { m_nYBlock = nYBlock; }
	void SetStartTiming(const int nStartTiming) { m_nStartTiming = nStartTiming; }
	void SetFinishTiming(const int nFinishTiming) { m_nFinishTiming = nFinishTiming; }

	int GetModelIdx(void) { return m_nModelIdx; }
	int GetTexIdx(void) { return m_nTexIdx; }
	D3DXVECTOR3 GetOffsetPos1(void) { return m_OffSetPos1; }
	D3DXVECTOR3 GetOffsetPos2(void) { return m_OffSetPos2; }
	D3DXVECTOR3 GetOffsetAmp1(void) { return m_OffSetAmp1; }
	D3DXVECTOR3 GetOffsetAmp2(void) { return m_OffSetAmp2; }
	D3DXCOLOR GetColUp(void) { return m_ColUp; }
	D3DXCOLOR GetColDown(void) { return m_ColDown; }
	float GetAlphaDecayUp(void) { return m_fAlphaDecayUp; }
	float GetAlphaDecayDown(void) { return m_fAlphaDecayDown; }
	int GetXBlock(void) { return m_nXBlock; }
	int GetYBlock(void) { return m_nYBlock; }
	int GetStartTiming(void) { return m_nStartTiming; }
	int GetFinishTiming(void) { return m_nFinishTiming; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int         m_nModelIdx;      // �O�Ղ��o�����f���̔ԍ�
	int         m_nTexIdx;        // �O�Ղɓ\��e�N�X�`���̔ԍ�
	D3DXVECTOR3 m_OffSetPos1;     // �O�Ղ��o�����f������̃I�t�Z�b�g����(1�Ԗ�)
	D3DXVECTOR3 m_OffSetPos2;     // �O�Ղ��o�����f������̃I�t�Z�b�g����(2�Ԗ�)
	D3DXVECTOR3 m_OffSetAmp1;     // �O�Ղ̃I�t�Z�b�g�����l(1�Ԗ�)
	D3DXVECTOR3 m_OffSetAmp2;     // �O�Ղ̃I�t�Z�b�g�����l(2�Ԗ�)
	D3DXCOLOR   m_ColUp;          // �O�Ղ̒��_�㑤�̐F
	D3DXCOLOR   m_ColDown;        // �O�Ղ̒��_�����̐F
	float       m_fAlphaDecayUp;  // �O�Ղ̒��_�㑤�̓����x�����l
	float       m_fAlphaDecayDown;// �O�Ղ̒��_�㑤�̓����x�����l
	int         m_nXBlock;        // �O�Ղ̉��̕�����
	int         m_nYBlock;        // �O�Ղ̏c�̕�����
	int         m_nStartTiming;   // �O�Ղ��o���n�߂�^�C�~���O
	int         m_nFinishTiming;  // �O�Ղ����܂��n�߂�^�C�~���O
};

//*****************************************************************************
//    �L�[�t���[���N���X�̒�`
//*****************************************************************************
class CKeyframe
{
public:    // �N�ł��A�N�Z�X�\
	CKeyframe();
	~CKeyframe();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	void SetAddPos(D3DXVECTOR3 pos);
	void SetDestPos(D3DXVECTOR3 pos);
	void SetDestRot(D3DXVECTOR3 rot);
	void SetDiffRot(D3DXVECTOR3 rotDiff);

	D3DXVECTOR3 GetAddPos(void);
	D3DXVECTOR3 GetDestPos(void);
	D3DXVECTOR3 GetDestRot(void);
	D3DXVECTOR3 GetDiffRot(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3 m_AddPos;       // ���݂̃I�t�Z�b�g�ʒu�ɉ�����l
	D3DXVECTOR3 m_DestPos;      // �ړI�̃I�t�Z�b�g�ʒu
	D3DXVECTOR3 m_DestRot;      // �ړI�̃I�t�Z�b�g����
	D3DXVECTOR3 m_DiffRot;      // �����̌���
};

//*****************************************************************************
//    ���[�V�����N���X�̒�`
//*****************************************************************************
class CMotion
{
public:    // �N�ł��A�N�Z�X�\
	//------------------------
	//  ���[�V�����̏��
	//------------------------
	typedef enum
	{
		STATE_NORMAL = 0,  // �ʏ���
		STATE_STOP,        // ��~���
		STATE_SWITCH,      // �؂�ւ����
		STATE_MAX
	}STATE;

	CMotion();
	~CMotion();

	static CMotion *Create(int nNumModel, int nNumKey, float fBlendMag, bool bLoop, int *pPlaybackKey);
	HRESULT Init(int nNumModel, int nNumKey, float fBlendMag, bool bLoop, int *pPlaybackKey);
	void Uninit(int nNumModel);
	void Update(CModel **pModel, int nNumModel, int *pKey, int *pCounter, int *pBlendCounter, int *pAllCounter);

	CMotionAttack *GetAttack(void);
	CMotionCollision **GetCollision(void);
	int GetNumColData(void);
	CMotionOrbit **GetOrbit(void);
	int GetNumOrbitData(void);
	CKeyframe ***GetKeyframe(void);
	STATE GetState(void);
	int GetNumKey(void);
	bool GetLoop(void);
	int GetPlayBackKey(int nIdx);
	int *GetPlayBackKey(void);
	int GetAllPlayBackKey(void);
	float GetBlendMag(void);

	void SetAttack(CMotionAttack *pAttack);
	void SetCollision(CMotionCollision **pCollision);
	void SetNumColData(const int nNumColData);
	void SetOrbit(CMotionOrbit **pOrbit);
	void SetNumOrbitData(const int nNumOrbitData);
	void SetKeyFrame(const int nIdxModel, const int nIdxKey, CKeyframe *pKeyFrame);
	void SetState(STATE state);
	void SetNumKey(const int nNumKey);
	void SetLoop(const bool bLoop);
	void SetPlayBackKey(const int nPlayBackKey, int nIdx);
	void SetPlayBackKey(int *pPlayBackKey);
	void SetAllPlayBackKey(int nAllPlayBackKey);
	void SetBlendMag(const float fBlendMag);
	void SwitchKey(CModel **pModel, int nNumModel, int nKey);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CMotionAttack    *m_pMotionAttack;    // ���[�V�����U�����[�V�����f�[�^�N���X�ւ̃|�C���^
	CMotionCollision **m_pCollision;      // ���[�V�����̓����蔻��f�[�^�N���X�ւ̃|�C���^
	int              m_nNumColData;       // �����蔻��f�[�^�̐�
	CMotionOrbit     **m_pOrbit;          // ���[�V�����̋O�Ճf�[�^�N���X�ւ̃|�C���^
	int              m_nNumOrbitData;     // �O�Ճf�[�^�̐�
	CKeyframe        ***m_pKeyframe;      // �L�[�t���[���̏��(���̃��[�V�����̃L�[�t���[���̐��ƃ��f���̐������I�Ɋm��)
	STATE            m_State;             // ���[�V�����̏��
	int              *m_pPlaybackKey;     // �Đ��t���[���̑���
	int              m_nAllPlaybackKey;   // �Đ��t���[���̑���
	int              m_nNumKey;           // �L�[�t���[���̐�
	float            m_fBlendMag;         // ���[�V�����u�����h�{��
	bool             m_bLoop;             // ���[�v���邩���Ȃ���
};

//*****************************************************************************
//    ���[�V�����Ǘ��N���X�̒�`
//*****************************************************************************
class CMotionManager
{
public:    // �N�ł��A�N�Z�X�\
	CMotionManager();
	~CMotionManager();

	static CMotionManager *Create(int nNumMotion, int nNumModel);
	HRESULT Init(int nNumMotion, int nNumModel);
	void Uninit(void);
	void Update(CModel **pModel);

	void SetMotion(CMotion *pMotion, int nIdx);
	void SetKey(int nKey);
	void SetAllCounter(int nAllCounter);
	void SetNumber(int nNumber);

	CMotion **GetMotion(void);
	int GetKey(void);
	int GetCounter(void);
	int GetAllCounter(void);
	int GetNumModel(void);
	int GetNumber(void);

	void SwitchMotion(CModel **pModel, int nNumber);

	void Save(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CMotion **m_pMotion;     // ���[�V�����N���X�ւ̃|�C���^
	int     m_nKey;          // ���݂̃L�[�t���[��
	int     m_nCounter;      // ���݂̍Đ��t���[����
	int     m_nAllCounter;   // ���݂̑��Đ��t���[����
	int     m_nBlendCounter; // �u�����h���Ǘ�����J�E���^�[
	int     m_nNumAll;       // ���[�V�����̑���
	int     m_nNumber;       // ���݂̃��[�V�����ԍ�
	int     m_nNumberOld;    // �O��̃��[�V�����ԍ�
	int     m_nNumModel;     // ���f���̐�
};

#endif