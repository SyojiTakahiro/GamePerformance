//=============================================================================
//
// �^�C�}�[�̏��� [timer.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
#include "2DPolygon.h"

#define TIMER_DIGIT (6)	//�^�C�}�[�̌���
#define TIMER_WIDTH (20.0f)	//�^�C�}�[�̕�
#define TIMER_HEIGHT (40.0f)	//�^�C�}�[�̍���
//�O���錾
class CNumber;

//*****************************************************************************
// �N���X��`
//*****************************************************************************
class CTimer : public CScene
{
public:
	//**********************************
	// �^�C�}�[�̎��
	//**********************************
	typedef enum
	{
		TIMER_ONEMINUTES = 0,		//1��
		TIMER_TENSECOND,		//10�b
		TIMER_ONESECOND,		//1�b
		TIMER_TENMIRI,			//10�~��
		TIMER_ONEMIRI,			//1�b
		TIMER_MAX,
	}TIMER_TYPE;

	//�����o�֐�
	CTimer();
	~CTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	static void SetTimer(int nOneMinutes, int nTenSeconds, int nOneSeconds, int nTenMiri, int nOneMiri);
	static int GetOneMinute(void) { return m_nOneMinute; }		//1��
	static int GetTenSecond(void) { return m_nTenSecond; }		//10�b
	static int GetOneSecond(void) { return m_nOneSecond; }		//10�b
	static int GetTenMiri(void) { return m_nTenMiri; }			//�~��
	static int GetOneMiri(void) { return m_nOneMiri; }			//�~��

	//�ÓI�����o�֐�
	static CTimer *Create(D3DXVECTOR3 pos);

private:
	bool		m_bUse;			//�g�p���Ă��邩�ǂ���
	int			m_nCntFrame;
	D3DXVECTOR3 m_pos;			//�ʒu
	static int m_nTimer;		//�^�C�}�[
	static int m_nOneMinute;	//1��
	static int m_nTenSecond;	//10�b
	static int m_nOneSecond;	//10�b
	static int m_nTenMiri;		//10�~��
	static int m_nOneMiri;		//1�~��
	static CNumber *m_apNumber[TIMER_DIGIT];		//�^�C�}�[�̃|�C���^
	LPDIRECT3DTEXTURE9	m_pTexture;					// �e�N�X�`���ւ̃|�C���^
protected:
};
#endif