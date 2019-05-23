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
	void Update(CModel **pModel, int nNumModel, int *pKey, int *pCounter, int *pBlendCounter);

	CKeyframe ***GetKeyframe(void);
	STATE GetState(void);
	int GetNumKey(void);
	bool GetLoop(void);
	int GetPlayBackKey(int nIdx);
	float GetBlendMag(void);

	void SetState(STATE state);
	void SwitchKey(CModel **pModel, int nNumModel, int nKey);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CKeyframe   ***m_pKeyframe;    // �L�[�t���[���̏��(���̃��[�V�����̃L�[�t���[���̐��ƃ��f���̐������I�Ɋm��)
	STATE       m_State;           // ���[�V�����̏��
	int         *m_pPlaybackKey;   // �Đ��t���[���̑���
	int         m_nNumKey;         // �L�[�t���[���̐�
	float       m_fBlendMag;       // ���[�V�����u�����h�{��
	bool        m_bLoop;           // ���[�v���邩���Ȃ���
};

//*****************************************************************************
//    ���[�V�����Ǘ��N���X�̒�`
//*****************************************************************************
class CMotionManager
{
public:    // �N�ł��A�N�Z�X�\
	CMotionManager();
	~CMotionManager();

	static CMotionManager *Create(int nNumMotion, int nNumModel, char *aFileName);
	HRESULT Init(int nNumMotion, int nNumModel, char *aFileName);
	void Uninit(void);
	void Update(CModel **pModel);

	void SetKey(int nKey);
	void SetNumber(int nNumber);

	CMotion **GetMotion(void);
	int GetKey(void);
	int GetCounter(void);
	int GetNumModel(void);
	int GetNumber(void);

	void SwitchMotion(CModel **pModel, int nNumber);

	void Save(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CMotion **m_pMotion;     // ���[�V�����N���X�ւ̃|�C���^
	int     m_nKey;          // ���݂̃L�[�t���[��
	int     m_nCounter;      // ���݂̍Đ��t���[����
	int     m_nBlendCounter; // �u�����h���Ǘ�����J�E���^�[
	int     m_nNumAll;       // ���[�V�����̑���
	int     m_nNumber;       // ���݂̃��[�V�����ԍ�
	int     m_nNumberOld;    // �O��̃��[�V�����ԍ�
	int     m_nNumModel;     // ���f���̐�
};

#endif