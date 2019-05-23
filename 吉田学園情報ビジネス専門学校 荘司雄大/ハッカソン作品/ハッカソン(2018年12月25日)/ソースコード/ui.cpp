//*****************************************************************************
//
//     �`���[�g���A���̏���[tutorial.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "ui.h"
#include "timer.h"
#include "score.h"
#include "functionlib.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "timer.h"
#include "score.h"
#include "stock.h"
#include "present.h"

#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define GAME_SYSTEM_FILENAME   "data/TEXT/MODE/game.ini"              // �Q�[���̏��������X�N���v�g�t�@�C����
#define NUMBER_TEXTURE		   "data/TEXTURE/COMMON/number.png"

#define UI_NUM_TEX			 (1)		// �e�N�X�`����

#define UI_TEX_NUMBER_0		 (0)		// �e�N�X�`���ԍ�
#define UI_TEX_NUMBER_1		 (1)		// �e�N�X�`���ԍ�
#define UI_TEX_NUMBER_2		 (2)		// �e�N�X�`���ԍ�

// �^�C���p�̃}�N��
#define UI_TIMER_POS         (D3DXVECTOR3(720.0f,60.0f,0.0f))       // �^�C�}�[�̍��W
#define UI_TIMER_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))       // �^�C�}�[�̐F
#define UI_TIMER_WIDTH       (40.0f)                                // �^�C�}�[�̐����|���S���P���̕�
#define UI_TIMER_HEIGHT      (40.0f)                                // �^�C�}�[�̐����|���S���P���̍���
#define UI_TIMER_WARNING     (30 * 60)                              // �^�C�}�[���Ԃ��Ȃ�^�C�~���O

// �X�R�A�p�̃}�N��
#define UI_SCORE_POS         (D3DXVECTOR3(1200.0f,60.0f,0.0f))      // �X�R�A�̍��W
#define UI_SCORE_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))       // �X�R�A�̐F
#define UI_SCORE_WIDTH       (30.0f)                                // �X�R�A�̐����|���S���P���̕�
#define UI_SCORE_HEIGHT      (30.0f)                                // �X�R�A�̐����|���S���P���̍���

// �c�@�p�̃}�N��
#define UI_STOCK_POS         (D3DXVECTOR3(120.0f,60.0f,0.0f))       // �c�@�̍��W
#define UI_STOCK_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))       // �c�@�̐F
#define UI_STOCK_WIDTH       (30.0f)                                // �c�@�̐����|���S���P���̕�
#define UI_STOCK_HEIGHT      (30.0f)                                // �c�@�̐����|���S���P���̍���
#define UI_STOCK_NUMBER      (3)								    // �c�@��

// �v���[���g�p�̃}�N��
#define UI_PRESENT_POS         (D3DXVECTOR3(420.0f,60.0f,0.0f))     // �v���[���g�̍��W
#define UI_PRESENT_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))     // �v���[���g�̐F
#define UI_PRESENT_WIDTH       (30.0f)                              // �v���[���g�̐����|���S���P���̕�
#define UI_PRESENT_HEIGHT      (30.0f)                              // �v���[���g�̐����|���S���P���̍���
#define UI_PRESENT_NUMBER      (5)								    // �v���[���g��

#define GAME_TIMER             "GAME_TIMER = "             // �^�C�}�[�̕b��

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CUI::CUI()
{
	// �e��l�̃N���A
	m_pScore = NULL;			// �X�R�A�N���X�ւ̃|�C���^
	m_pTimer = NULL;			// �^�C�}�[�N���X�ւ̃|�C���^
	m_pStock = NULL;			// �c�@�N���X�ւ̃|�C���^
	m_pPresent = NULL;			// �v���[���g�N���X�ւ̃|�C���^
	m_pTextureManager = NULL;	// �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CUI::~CUI()
{

}

//=============================================================================
//    ��������
//=============================================================================
CUI *CUI::Create(void)
{
	CUI *pUI = NULL;  // �`���[�g���A���N���X�^�̃|�C���^
	if (pUI == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pUI = new CUI;
		if (pUI != NULL)
		{// ���������m�ۂł���
		 // �������������s��
			pUI->Init();
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return NULL;
	}

	// �C���X�^���X��Ԃ�
	return pUI;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CUI::Init(void)
{
	// �Q�[���̏����������O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(GAME_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		 // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                         // 1�s���ǂݎ��p
			char *pStrCur = NULL;                       // ���݂̐擪�|�C���^
			char *pStr = NULL;                          // �擪�|�C���^�ۑ��p
			int nTimer = 10 * 60;                       // �^�C�}�[�̕b��
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // �e�N�X�`���ǂݎ��p

			if (pLine == NULL && pStr == NULL)
			{// ���������m�ۂł����Ԃł���
				pLine = new char[256];
				pStr = new char[256];
				if (pLine != NULL && pStr != NULL)
				{// ���������m�ۂł���
				 // �������������
					strcpy(pLine, "\0");
					strcpy(pStr, "\0");

					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, GAME_TIMER) == 0)
							{// �^�C�}�[�̕b��������
								nTimer = CFunctionLib::ReadInt(pStrCur, GAME_TIMER);
								nTimer *= 60;
								CTimer::SetInitTimer(nTimer);	// �^�C�}�[�̏����l��ݒ�
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}

					// �e�N�X�`���Ǘ��N���X�̐���
					m_pTextureManager = CTextureManager::Create(UI_NUM_TEX);

					if (m_pTextureManager != NULL)
					{
						D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), NUMBER_TEXTURE, &pTexture);
						if (pTexture != NULL)
						{// �e�N�X�`����ǂݍ��߂�
							m_pTextureManager->BindTexture(pTexture, 0);
							m_pTextureManager->SetFileName(NUMBER_TEXTURE, 0);
							pTexture = NULL;
						}
					}

					// �K�v�ȃN���X���쐬����
					// �^�C�}�[
					m_pTimer = CTimer::Create(UI_TIMER_POS, UI_TIMER_COL, UI_TIMER_WIDTH, UI_TIMER_HEIGHT, nTimer, UI_TIMER_WARNING, UI_TIMER_PRIORITY);
					if (m_pTimer != NULL)
					{// �^�C�}�[�N���X�������ł���
						m_pTimer->SetTimerTexture(m_pTextureManager->GetTexture(UI_TEX_NUMBER_0));
					}

					// �X�R�A
					m_pScore = CScore::Create(UI_SCORE_POS, UI_SCORE_COL, UI_SCORE_WIDTH, UI_SCORE_HEIGHT, UI_SCORE_PRIORITY);
					if (m_pScore != NULL)
					{// �X�R�A�N���X�������ł���
						m_pScore->SetScoreTexture(m_pTextureManager->GetTexture(UI_TEX_NUMBER_0));
					}

					// �c�@
					m_pStock = CStock::Create(UI_STOCK_POS, UI_STOCK_COL, UI_STOCK_WIDTH, UI_STOCK_HEIGHT, UI_STOCK_NUMBER, UI_STOCK_PRIORITY);
					if (m_pStock != NULL)
					{// �c�@�N���X�������ł���
						m_pStock->SetStockTexture(m_pTextureManager->GetTexture(UI_TEX_NUMBER_0));
					}

					// �v���[���g
					m_pPresent = CPresent::Create(UI_PRESENT_POS, UI_PRESENT_COL, UI_PRESENT_WIDTH, UI_PRESENT_HEIGHT, UI_PRESENT_NUMBER, UI_PRESENT_PRIORITY);
					if (m_pPresent != NULL)
					{// �v���[���g�N���X�������ł���
						m_pPresent->SetPresentTexture(m_pTextureManager->GetTexture(UI_TEX_NUMBER_0));
					}

					// �������̊J��
					if (pStr != NULL)
					{// ���������m�ۂ���Ă���
						delete[] pStr;
						pStr = NULL;
					}
					if (pLine != NULL)
					{// ���������m�ۂ���Ă���
						delete[] pLine;
						pLine = NULL;
					}
				}
				else
				{// ���������m�ۂł��Ȃ�����
					return E_FAIL;
				}
			}
			else
			{// ���������m�ۂł����ԂłȂ�
				return E_FAIL;
			}
		}
		else
		{// �t�@�C���|�C���^���m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// �t�@�C���|�C���^���m�ۂł����ԂłȂ�
		return E_FAIL;
	}
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CUI::Uninit(void)
{
	// �e�N�X�`���Ǌ��N���X���J������
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// �^�C�}�[�N���X�ւ̃|�C���^����ɂ���
	if (m_pTimer != NULL)
	{
		m_pTimer = NULL;
	}

	// �X�R�A�N���X�ւ̃|�C���^����ɂ���
	if (m_pScore != NULL)
	{
		m_pScore = NULL;
	}

	// �X�R�A�N���X�ւ̃|�C���^����ɂ���
	if (m_pStock != NULL)
	{
		m_pStock = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CUI::Update(void)
{
}

//=============================================================================
//    �^�C�}�[�̎擾
//=============================================================================
int CUI::GetTimer(void)
{
	return m_pTimer->GetTimer();
}

//=============================================================================
//    �^�C�}�[�̐ݒ�
//=============================================================================
void CUI::SetTimer(int nTimer)
{
	m_pTimer->SetTimer(nTimer);
}

//=============================================================================
//    �^�C�}�[�̃��Z�b�g
//=============================================================================
void CUI::ResetTimer(void)
{
	m_pTimer->Reset();
}

//=============================================================================
//    �X�R�A�̎擾
//=============================================================================
int CUI::GetScore(void)
{
	return m_pScore->GetScore();
}

//=============================================================================
//    �X�R�A�̐ݒ�
//=============================================================================
void CUI::SetScore(int nScore)
{
	m_pScore->SetScore(nScore);
}

//=============================================================================
//    �c�@�̎擾
//=============================================================================
int CUI::GetStock(void)
{
	return m_pStock->GetStock();
}

//=============================================================================
//    �c�@�̐ݒ�
//=============================================================================
void CUI::SetStock(int nStock)
{
	m_pStock->SetStock(nStock);
}

//=============================================================================
//    �v���[���g�̎擾
//=============================================================================
int CUI::GetPresent(void)
{
	return m_pPresent->GetPresent();
}

//=============================================================================
//    �v���[���g�̐ݒ�
//=============================================================================
void CUI::SetPresent(int nPresent)
{
	m_pPresent->SetPresent(nPresent);
}