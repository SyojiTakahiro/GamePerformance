//*****************************************************************************
//
//     �Q�[�W�̏���[gauge.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GAUGE_H_
#define _GAUGE_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    �Q�[�W�N���X�̒�`
//*****************************************************************************
class CGauge : public CSceneBillboard
{
public:    // �N�ł��A�N�Z�X�\
	CGauge(int nPriority = 3, OBJTYPE objType = OBJTYPE_GAUGE);
	~CGauge();

	static CGauge *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetGaugeRate(const float fGaugeRate);
	void SetRightWidth(const float fRightWidth);

	float GetGaugeRate(void);
	float GetRightWidth(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	float m_fGaugeRate;    // �Q�[�W�̊���
	float m_fRightWidth;   // �Q�[�W�̍����̒���
};

//*****************************************************************************
//    �̗̓Q�[�W�N���X�̒�`
//*****************************************************************************
class CGaugeLife : public CGauge
{
public:    // �N�ł��A�N�Z�X�\
	//----------------------
	// ���
	//----------------------
	typedef enum
	{
		STATE_NONE = 0,
		STATE_CUT,
		STATE_ADD,
		STATE_MAX
	}STATE;

	CGaugeLife(int nPriority = 3, OBJTYPE objType = OBJTYPE_LIFEGAUGE);
	~CGaugeLife();

	static CGaugeLife *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, int nLife, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void CutGauge(const int nCutValue);
	void AddGauge(const int nAddValue);

	void SetGaugePos(const D3DXVECTOR3 pos);
	void SetState(const STATE State);
	void SetLife(const int nLife);

	STATE GetState(void);
	int GetLife(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int             m_nCounter;      // �Q�[�W�̌��炵���Ǘ�����J�E���^�[
	STATE           m_State;         // ���
	int             m_nLife;         // �̗�
	CGauge         *m_pWhiteBill;    // �̗̓Q�[�W�̃t���[��
	CGauge         *m_pRedBill;      // �̗̓Q�[�W�����������ɕ\������Ԃ��Q�[�W
};

#endif