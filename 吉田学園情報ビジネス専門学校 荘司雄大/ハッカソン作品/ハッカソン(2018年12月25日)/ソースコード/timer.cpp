//*****************************************************************************
//
//     �^�C�}�[�̏���[timer.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "timer.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "textureManager.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define TIMER_FRAME_TEXTURE	"data/TEXTURE/GAME/UI/timer.png"	// �^�C�}�[�̘g�e�N�X�`���̃p�X

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
int CTimer::m_nInitTimer = 0;

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CTimer::CTimer(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_apNumber = NULL;                           // �����|���S���N���X�ւ̃|�C���^
	m_nDigit = 0;                                // �^�C�}�[�̌���
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �^�C�}�[�̍��W
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // �^�C�}�[�̐F
	m_fWidth = 0.0f;                             // �^�C�}�[�̐����|���S���P���̕�
	 m_fHeight = 0.0f;                           // �^�C�}�[�̐����|���S���P���̕�
	m_nTimer = 0;                                // �^�C�}�[
	m_nWarningTime = 0;                          // �^�C�}�[���Ԃ��Ȃ�^�C�~���O
	m_pTexture = NULL;                           // �e�N�X�`���ւ̃|�C���^
	m_pTextureManager = NULL;					 // �e�N�X�`���Ǘ��N���X
	m_pFrame = NULL;							 // �g
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CTimer::~CTimer()
{

}

//=============================================================================
//    ��������
//=============================================================================
CTimer *CTimer::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nTimer, int nWarningTime, int nPriority)
{
	CTimer *pTimer = NULL;                 // �^�C�}�[�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pTimer == NULL)
		{// ����������ɂȂ��Ă���
			pTimer = new CTimer(nPriority);
			if (pTimer != NULL)
			{// ���������m�ۂł���
				// �e��l�̐ݒ�
				pTimer->SetPos(pos);                    // �^�C�}�[�̍��W
				pTimer->SetCol(col);                    // �^�C�}�[�̐F
				pTimer->SetWidth(fWidth);               // �^�C�}�[�̐����|���S���P���̕�
				pTimer->SetHeight(fHeight);             // �^�C�}�[�̐����|���S���P���̍���
				pTimer->SetTimer(nTimer);               // �^�C�}�[
				pTimer->SetWarningTime(nWarningTime);   // �^�C�}�[���Ԃ��Ȃ�^�C�~���O

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

//=============================================================================
//    ����������
//=============================================================================
HRESULT CTimer::Init(void)
{
	// �e��l�̏�����
	D3DXVECTOR3 NumberPos = m_Pos;   // ���W
	D3DXCOLOR NumberCol = m_Col;     // �F
	float fNumberWidth = m_fWidth;   // ��
	float fNumberHeight = m_fHeight; // ����
	int nNumber = 0;                 // �\�����鐔���̌v�Z�p

	if (m_pFrame == NULL)
	{// ���������m�ۂł����Ԃł���
		m_pFrame = CScene2D::Create(m_Pos + D3DXVECTOR3(-m_fWidth - 10.0f,0.0f,0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), m_fWidth * 3.0f, m_fHeight * 1.5f, 0.0f, 5);
		if (m_pFrame != NULL)
		{// ���������m�ۂł���
			LPDIRECT3DTEXTURE9 pTexture = NULL;
			D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), TIMER_FRAME_TEXTURE, &pTexture);
			m_pTextureManager = CTextureManager::Create(1);

			if (m_pTextureManager != NULL)
			{// �e�N�X�`���Ǘ��N���X�̐����ɐ���
				m_pTextureManager->BindTexture(pTexture, 0);
				m_pTextureManager->SetFileName(TIMER_FRAME_TEXTURE, 0);
				m_pFrame->BindTexture(m_pTextureManager->GetTexture(0));
				pTexture = NULL;
			}
		}
	}

	// �^�C�}�[�̌������v�Z���A���������������m��
	m_nDigit = (int)log10f((float)(m_nTimer / 60)) + 1;   // ���������߂�
	if (m_nDigit <= 0) { m_nDigit = 1; }	              // 0�ȉ��̂Ƃ�1�ɂ���

	if (m_apNumber == NULL)
	{// ���������m�ۂł����Ԃł���
		m_apNumber = new CNumber*[m_nDigit];  // �������������m��
		if (m_apNumber != NULL)
		{// ���������m�ۂł���
			for (int nCntTimer = 0; nCntTimer < m_nDigit; nCntTimer++)
			{// �X�R�A�̌������J��Ԃ�
				m_apNumber[nCntTimer] = NULL;   // �������񃁃�������ɂ���
				if (m_apNumber[nCntTimer] == NULL)
				{// ���������m�ۂł����Ԃɂ���
					// �\�����鐔���̌v�Z
					nNumber = (m_nTimer / 60) % ((int)powf(10.0f, (float)nCntTimer) * 10) / (int)powf(10.0f, (float)nCntTimer);

					// �����|���S���̃C���X�^���X��������
					m_apNumber[nCntTimer] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, CNumber::STATE_NONE, nNumber, 3, GetPriority());

					// ���ɐi�߂�
					NumberPos.x -= m_fWidth + (m_fWidth * 0.5f);
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CTimer::Uninit(void)
{
	// �����|���S���̊J������
	if(m_apNumber != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntTimer = 0; nCntTimer < m_nDigit; nCntTimer++)
		{// �X�R�A�̌������J��Ԃ�
			if (m_apNumber[nCntTimer] != NULL)
			{// ���������m�ۂ���Ă���
			    // �����|���S���̏I������
				m_apNumber[nCntTimer]->Uninit();
				m_apNumber[nCntTimer] = NULL;
			}
		}
		delete[] m_apNumber;
		m_apNumber = NULL;
	}

	// �e�N�X�`���Ǌ��N���X���J������
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// �^�C�}�[�̏�������
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CTimer::Update(void)
{
	if (CManager::GetGame()->GetState() == CGame::STATE_NORMAL)
	{
		if (m_nTimer > 0)
		{// �^�C�}�[���܂�����
			// �^�C�}�[�����炷
			m_nTimer--;
			if (m_nTimer % 60 == 0)
			{// �A�b�v�f�[�g��60��܂����
				if (m_apNumber != NULL)
				{// �����|���S������������Ă���
					int nSetNumber = 0;   // �\�����鐔���̌v�Z�p
					for (int nCntTimer = 0; nCntTimer < m_nDigit; nCntTimer++)
					{// �^�C�}�[�̌������J��Ԃ�
						if (m_nTimer == m_nWarningTime)
						{// �^�C�}�[���Ȃ��Ȃ肻���Ȃ��
							//m_apNumber[nCntTimer]->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
							//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WARNING);
						}

						// �\�����鐔���̌v�Z
						nSetNumber = (m_nTimer / 60) % ((int)powf(10.0f, (float)nCntTimer) * 10) / (int)powf(10.0f, (float)nCntTimer);

						// �����|���S���̐����ݒ菈��
						m_apNumber[nCntTimer]->SetNumber(nSetNumber);
					}
				}
			}
		}
		else
		{// �^�C�}�[���Ȃ��Ȃ���
			if (CManager::GetMode() == CManager::MODE_GAME)
			{// �Q�[�����[�h����������
				CGame *pGame = CManager::GetGame();
				if (pGame->GetState() != CGame::STATE_END)
				{// �Q�[���I����ԂłȂ�
					pGame->SetState(CGame::STATE_END);
					//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_FINISH);
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
//    �����e�N�X�`����ݒ肷�鏈��
//=============================================================================
void CTimer::SetTimerTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
	if (m_pTexture != NULL)
	{// �e�N�X�`���ւ̃|�C���^���擾�ł���
		if (m_apNumber != NULL)
		{// �����|���S������������Ă���
			for (int nCntTimer = 0; nCntTimer < m_nDigit; nCntTimer++)
			{// �^�C�}�[�̌������J��Ԃ�
				if (m_apNumber[nCntTimer] != NULL)
				{// �����|���S������������Ă���
					m_apNumber[nCntTimer]->BindTexture(m_pTexture);
				}
			}
		}
	}
}

//=============================================================================
//    �^�C�}�[�̍��W��ݒ肷�鏈��
//=============================================================================
void CTimer::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �^�C�}�[�̐F��ݒ肷�鏈��
//=============================================================================
void CTimer::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
//    �^�C�}�[�̐����|���S���P���̕���ݒ肷�鏈��
//=============================================================================
void CTimer::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    �^�C�}�[�̐����|���S���P���̍�����ݒ肷�鏈��
//=============================================================================
void CTimer::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    �^�C�}�[��ݒ肷�鏈��
//=============================================================================
void CTimer::SetTimer(const int nTimer)
{
	m_nTimer = nTimer;
	int nNumber = 0;
	if (m_apNumber != NULL)
	{// �����|���S������������Ă���
		for (int nCntTimer = 0; nCntTimer < m_nDigit; nCntTimer++)
		{// �^�C�}�[�̌������J��Ԃ�
			if (m_apNumber[nCntTimer] != NULL)
			{// �����|���S������������Ă���
			    // �\�����鐔���̌v�Z
				nNumber = (m_nTimer / 60) % ((int)powf(10.0f, (float)nCntTimer) * 10) / (int)powf(10.0f, (float)nCntTimer);
				m_apNumber[nCntTimer]->SetNumber(nNumber);
			}
		}
	}
}

//=============================================================================
//    �^�C�}�[���Ԃ��Ȃ�^�C�~���O��ݒ肷�鏈��
//=============================================================================
void CTimer::SetWarningTime(const int nWarningTime)
{
	m_nWarningTime = nWarningTime;
}

//=============================================================================
//    �^�C�}�[�̍��W���擾���鏈��
//=============================================================================
D3DXVECTOR3 CTimer::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �^�C�}�[�̐F���擾���鏈��
//=============================================================================
D3DXCOLOR CTimer::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    �^�C�}�[�̐����|���S���P���̕����擾���鏈��
//=============================================================================
float CTimer::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    �^�C�}�[�̐����|���S���P���̍������擾���鏈��
//=============================================================================
float CTimer::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    �^�C�}�[���擾���鏈��
//=============================================================================
int CTimer::GetTimer(void)
{
	return m_nTimer;
}

//=============================================================================
//    �^�C�}�[�̐Ԃ��Ȃ�^�C�~���O���擾���鏈��
//=============================================================================
int CTimer::GetWarningTime(void)
{
	return m_nWarningTime;
}