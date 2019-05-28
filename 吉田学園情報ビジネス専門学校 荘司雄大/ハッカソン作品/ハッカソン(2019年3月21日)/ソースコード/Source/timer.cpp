//=============================================================================
//
// �^�C�}�[�̏��� [timer.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "timer.h"
#include "manager.h"
#include "2DPolygon.h"
#include "mode.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CTimer::m_nTimer = 0;			//�^�C�}�[
int CTimer::m_nOneMinute = 0;		//1��
int CTimer::m_nTenSecond = 0;		//10�b
int CTimer::m_nOneSecond = 0;		//1�b
int CTimer::m_nTenMiri = 0;			//10�~��
int CTimer::m_nOneMiri = 0;			//1�~��
CNumber *CTimer::m_apNumber[TIMER_DIGIT] = {};	//�^�C�}�[�̃|�C���^
//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �O���[�o���ϐ�
//=============================================================================

//=============================================================================
// �^�C�}�[�̃R���X�g���N�^
//=============================================================================
CTimer::CTimer() :CScene(6, OBJTYPE_2DPOLYGON)
{
	m_pTexture = NULL;                         // �e�N�X�`���ւ̃|�C���^
}
//=============================================================================
// �^�C�}�[�̃f�X�g���N�^
//=============================================================================
CTimer::~CTimer()
{

}

//=============================================================================
//�^�C�}�[�̐���
//=============================================================================
CTimer *CTimer::Create(D3DXVECTOR3 pos)
{
	CTimer *pTimer = NULL;	//�^�C�}�[�̃|�C���^��NULL�ɂ���
	if (pTimer == NULL)
	{//�^�C�}�[��NULL�̏ꍇ
		pTimer = new CTimer;		//�V�[���̓��I�m��
		pTimer->m_pos = pos;		//�ʒu�̑��
		pTimer->Init();				//����������
	}
	return pTimer;					//�l��Ԃ�
}

//=============================================================================
// �^�C�}�[�̏���������
//=============================================================================
HRESULT CTimer::Init(void)
{
	//�^�C�}�[�̏�����
	m_nOneMinute = 1;
	m_nTenSecond = 3;
	m_nOneSecond = 0;
	m_nTenMiri = 0;
	m_nOneMiri = 0;

	m_nCntFrame = 0;	//�t���[���J�E���^�[�̏�����
	m_bUse = true;		//�g�p���Ă����Ԃɂ���

	for (int nCntNumber = 0; nCntNumber < TIMER_DIGIT; nCntNumber++)
	{//���̐������J��Ԃ�
		switch (nCntNumber)
		{
		case CTimer::TIMER_ONEMINUTES:
			//1��
			//�����̐���
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 50.0f, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TIMER_WIDTH, TIMER_HEIGHT, 0.0f, CNumber::STATE_NONE, m_nOneMinute, 3, 6);
			break;

		case CTimer::TIMER_TENSECOND:
			//10�b
			//�����̐���
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 120.0f, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TIMER_WIDTH, TIMER_HEIGHT, 0.0f, CNumber::STATE_NONE, m_nTenSecond, 3, 6);
			break;
		case CTimer::TIMER_ONESECOND:
			//1�b
			//�����̐���
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 170.0f, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TIMER_WIDTH, TIMER_HEIGHT, 0.0f, CNumber::STATE_NONE, m_nOneSecond, 3, 6);
			break;
		case CTimer::TIMER_TENMIRI:
			//10�~��
			//�����̐���
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 240.0f, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TIMER_WIDTH, TIMER_HEIGHT, 0.0f, CNumber::STATE_NONE, m_nTenMiri, 3, 6);
			break;
		case CTimer::TIMER_ONEMIRI:
			//1�~��
			//�����̐���
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 290.0f, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TIMER_WIDTH, TIMER_HEIGHT, 0.0f, CNumber::STATE_NONE, m_nOneMiri, 3, 6);
			break;
		}
	}

	return S_OK;
}

//=============================================================================
// �^�C�}�[�̏I������
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCntTimer = 0; nCntTimer < TIMER_DIGIT; nCntTimer++)
	{//���̐������J��Ԃ�
		if (m_apNumber[nCntTimer] != NULL)
		{
			m_apNumber[nCntTimer]->Uninit();
			m_apNumber[nCntTimer] = NULL;
		}
	}

	Release();
}

//=============================================================================
// �^�C�}�[�̍X�V����
//=============================================================================
void CTimer::Update(void)
{
	//�t���[���J�E���^�[�̉��Z
	m_nCntFrame--;


	if (m_bUse == true)
	{//�g�p���Ă����ꍇ
		m_nOneMiri--;	//1�~�������Z

		if (m_nOneMiri % 6 == 0)
		{//6�t���[�����Ƃ�10�~�����Z
			if (m_nTenMiri < 0 && m_nTenSecond < 0 && m_nTenSecond < 0 && m_nOneMinute < 0)
			{
				m_nOneMiri = 0;
			}
			else
			{
				m_nTenMiri--;
			}
		}

		if (m_nTenMiri < 0)
		{
			m_nTenMiri = 9;	//10�~���̏�����
			if (m_nTenSecond < 0 && m_nTenSecond < 0 && m_nOneMinute < 0)
			{
				m_nTenMiri = 0;
			}
			else
			{

			m_nOneSecond--;	//1�b�̉��Z
			}
		}

		if (m_nOneSecond < 0)
		{
			m_nOneSecond = 9;	//1�b�̏�����

				m_nTenSecond--;		//10�b�̉��Z

		}

		if (m_nTenSecond < 0)
		{
			m_nTenSecond = 5;	//10�b�̏�����
			if (m_nOneMinute > 0)
			{
				m_nOneMinute--;	//1���̉��Z
			}
		}

		if (m_nOneMinute < 0)
		{
			m_nOneMinute = 0;	//1����������
		}

		if (m_nOneMinute <= 0 && m_nTenSecond <= 0 && m_nOneSecond <= 0 && m_nTenMiri <= 0)
		{//�S�Ẵ^�C�}�[��0�ȉ��ɂȂ����ꍇ
			m_bUse = false;		//�g�p���Ă��Ȃ���Ԃɂ���
			m_nOneMinute = 0;	//0�Œ�

			//�X�R�A�̐ݒu����

			// ��ʑJ��
			if (CManager::GetMode() == CManager::MODE_GAME)
			{
				if (CManager::GetGame()->GetState() != CGame::STATE_END)
				{
					CManager::GetGame()->SetState(CGame::STATE_END);
				}
			}
		}
	m_apNumber[0]->SetNumber(m_nOneMinute);	//1���̐����ݒu����
	m_apNumber[1]->SetNumber(m_nTenSecond);	//10�b�̐����ݒu����
	m_apNumber[2]->SetNumber(m_nOneSecond);	//1�b�̐����ݒu����
	m_apNumber[3]->SetNumber(m_nTenMiri);	//10�~���̐����ݒu����
	m_apNumber[4]->SetNumber(m_nOneMiri);	//1�~���̐����ݒu����
	}
}

//=============================================================================
// �^�C�}�[�̕`�揈��
//=============================================================================
void CTimer::Draw(void)
{
	for (int nCntTimer = 0; nCntTimer < TIMER_DIGIT; nCntTimer++)
	{//���̐������J��Ԃ�
		if (m_apNumber[nCntTimer] != NULL)
		{
			m_apNumber[nCntTimer]->Draw();
		}
	}

}

//=============================================================================
//    �e�N�X�`���ݒ菈��
//=============================================================================
void CTimer::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;

	for (int nCntTimer = 0; nCntTimer < TIMER_DIGIT; nCntTimer++)
	{
		if (m_apNumber[nCntTimer] != NULL)
		{
			m_apNumber[nCntTimer]->BindTexture(m_pTexture);
		}
	}
}
//=============================================================================
//   �^�C�}�[�̐ݒu����
//=============================================================================
void CTimer::SetTimer(int nOneMinutes, int nTenSeconds, int nOneSeconds, int nTenMiri, int nOneMiri)
{
	//�^�C�}�[����
	m_nOneMinute = nOneMinutes;
	m_nTenSecond = nTenSeconds;
	m_nOneSecond = nOneSeconds;
	m_nTenMiri = nTenMiri;
	m_nOneMiri = nOneMiri;
}
