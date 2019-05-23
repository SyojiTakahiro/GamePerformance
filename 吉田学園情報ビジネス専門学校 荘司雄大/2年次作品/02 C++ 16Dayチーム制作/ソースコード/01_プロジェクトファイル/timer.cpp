//*****************************************************************************
//
//     �^�C�}�[�̏���[timer.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "timer.h"
#include "number.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define TIMER_POS_INI         (D3DXVECTOR3(1200.0f,60.0f,0.0f))   // �^�C�}�[�̍��W(�����l)
#define TIMER_COL_INI         (D3DXCOLOR(0.0f,1.0f,1.0f,1.0f))    // �^�C�}�[�̐F(�����l)
#define TIMER_WIDTH_INI       (60.0f)                             // �^�C�}�[�̍��W(�����l)
#define TIMER_HEIGHT_INI      (60.0f)                             // �^�C�}�[�̍���(�����l)
#define TIMER_INI             (180 * 60)                          // �^�C�}�[�̏����l
#define TIMER_WARNING         (40 * 60)                           // �^�C�}�[���Ԃ��Ȃ�^�C�~���O
#define TIMER_NUMBER_PRIORITY (6)                                 // �^�C�}�[�̏����D�揇��

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CTimer::CTimer(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER_MAX; nCntTimer++)
	{// �X�R�A�̌������J��Ԃ�
		m_apNumber[nCntTimer] = NULL;
	}
	m_nTimer = 0;  // �^�C�}�[
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CTimer::~CTimer()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CTimer::Init(void)
{
	// �e��l�̏�����
	D3DXVECTOR3 NumberPos = TIMER_POS_INI;   // ���W
	D3DXCOLOR NumberCol = TIMER_COL_INI;     // �F
	float fNumberWidth = TIMER_WIDTH_INI;    // ��
	float fNumberHeight = TIMER_HEIGHT_INI;  // ����
	int nNumber = 0;                         // �\�����鐔���̌v�Z�p
	m_nTimer = TIMER_INI;                    // �^�C�}�[

	for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER_MAX; nCntTimer++)
	{// �X�R�A�̌������J��Ԃ�
		if (m_apNumber[nCntTimer] == NULL)
		{// ���������m�ۂł����Ԃɂ���
			// �\�����鐔���̌v�Z
			nNumber = (m_nTimer / 60) % ((int)powf(10.0f, (float)nCntTimer) * 10) / (int)powf(10.0f, (float)nCntTimer);

			// �����|���S���̃C���X�^���X��������
			m_apNumber[nCntTimer] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, nNumber, TIMER_NUMBER_PRIORITY);

			// ���ɐi�߂�
			NumberPos.x -= TIMER_WIDTH_INI + (TIMER_WIDTH_INI * 0.5f);
		}
	}
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER_MAX; nCntTimer++)
	{// �^�C�}�[�̌������J��Ԃ�
		if (m_apNumber[nCntTimer] != NULL)
		{// ���������m�ۂ���Ă���
		    // �����|���S���̏I������
			m_apNumber[nCntTimer]->Uninit();
			m_apNumber[nCntTimer] = NULL;
		}
	}

	// �^�C�}�[�̏�������
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CTimer::Update(void)
{
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[�����[�h����������
		CGame *pGame = CManager::GetGame();  // �Q�[���̃C���X�^���X���擾
		if(pGame != NULL)
		{// �Q�[���̃C���X�^���X���擾�ł���
			if (pGame->GetState() == CGame::STATE_CHALLENGE)
			{// �}�b�v�U�����Ȃ��
				if (m_nTimer > 0)
				{// �^�C�}�[���܂�����
					// �^�C�}�[�����炷
					m_nTimer--;
					if (m_nTimer % 60 == 0)
					{// �A�b�v�f�[�g��60��܂����
						int nSetNumber = 0;   // �\�����鐔���̌v�Z�p
						for (int nCntTimer = 0; nCntTimer < TIMER_NUMBER_MAX; nCntTimer++)
						{// �^�C�}�[�̌������J��Ԃ�
							if (m_nTimer == TIMER_WARNING)
							{// �^�C�}�[���Ȃ��Ȃ肻���Ȃ��
								m_apNumber[nCntTimer]->SetVtxBuffCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
								CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WARNING);
							}

							// �\�����鐔���̌v�Z
							nSetNumber = (m_nTimer / 60) % ((int)powf(10.0f, (float)nCntTimer) * 10) / (int)powf(10.0f, (float)nCntTimer);

							// �����|���S���̐����ݒ菈��
							m_apNumber[nCntTimer]->SetNumber(nSetNumber);
						}
					}
				}
				else
				{// �^�C�}�[���Ȃ��Ȃ���
					if (CManager::GetMode() == CManager::MODE_GAME)
					{// �Q�[�����[�h����������
						if (pGame->GetState() != CGame::STATE_END)
						{// �Q�[���I����ԂłȂ�
							pGame->SetState(CGame::STATE_END);
							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_FINISH);
						}
					}
				}
			}
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CTimer::Draw(void)
{

}

//=============================================================================
//    ��������
//=============================================================================
CTimer *CTimer::Create(void)
{
	CTimer *pTimer = NULL;                 // �^�C�}�[�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pTimer == NULL)
		{// ����������ɂȂ��Ă���
			pTimer = new CTimer;
			if (pTimer != NULL)
			{// ���������m�ۂł���
				if (FAILED(pTimer->Init()))
				{// �������Ɏ��s����
					return NULL;
				}
			}
			else
			{// ���������m�ۂł��Ȃ�����
				return NULL;
			}
		}
		else
		{// ����������ł͂Ȃ�
			return NULL;
		}
	}
	else
	{// ����ȏ㐶���ł��Ȃ�
		return NULL;
	}

	return pTimer;
}