//*****************************************************************************
//
//     ���U���g�̏���[result.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "result.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "textureManager.h"
#include "functionlib.h"
#include "map.h"

#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define RESULT_SYSTEM_FILENAME   "data/TEXT/MODE/result.ini"                  // ���U���g�̏��������X�N���v�g�t�@�C����
#define RESULT_MODECHANGE_TIMING (600)                                        // ��ʑJ�ڂ������ōs���^�C�~���O

// �l��ǂݎ��p�X��
#define NUM_TEXTURE              "NUM_TEXTURE = "                             // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME         "TEXTURE_FILENAME = "                        // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define MAP_FILENAME             "MAP_FILENAME = "                            // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����

// PressEnter�p�̃}�N��
#define RESULT_PRESS_POS         (D3DXVECTOR3(SCREEN_WIDTH / 2,600.0f,0.0f))  // PressEnter�̍��W
#define RESULT_PRESS_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // PressEnter�̐F
#define RESULT_PRESS_WIDTH       (400.0f)                                     // PressEnter�̕�
#define RESULT_PRESS_HEIGHT      (100.0f)                                     // PressEnter�̍���

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
CResult::RESULT CResult::m_Result = CResult::RESULT_GAMEOVER;

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CResult::CResult(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pScene = NULL;
	m_nCounter = 0;             // �J�ڂ��Ǘ�����J�E���^�[
	m_pPress = NULL;            // PressEnter�p��2D�|���S���N���X�ւ̃|�C���^
	m_pTextureManager = NULL;   // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
//    ��������
//=============================================================================
CResult *CResult::Create(void)
{
	CResult *pResult = NULL;  // ���U���g�N���X�^�̃|�C���^
	if (pResult == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pResult = new CResult;
		if (pResult != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pResult->Init();
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
	return pResult;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CResult::Init(void)
{
	// ���U���g�̏����������O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(RESULT_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                         // 1�s���ǂݎ��p
			char *pStrCur = NULL;                       // ���݂̐擪�|�C���^
			char *pStr = NULL;                          // �擪�|�C���^�ۑ��p
			int nNumTex = 0;                            // �e�N�X�`����ǂݍ��ސ�
			int nCntTex = 0;                            // �e�N�X�`����ǂݍ��񂾉�
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // �e�N�X�`���ǂݎ��p
			char aTexFileName[256] = "\0";              // �e�N�X�`���̃t�@�C����
			char aMapFileName[256] = "\0";              // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����

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
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// �e�N�X�`���̌���񂪂�����
								nNumTex = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTex >= 1)
								{// ���f������1�ȏ゠��
									m_pTextureManager = CTextureManager::Create(nNumTex);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// �e�N�X�`���̃t�@�C���p�X����񂪂�����
							    // �e�N�X�`���̃t�@�C���p�X����ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, TEXTURE_FILENAME);

								// �e�N�X�`���̓ǂݍ���
								D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);
								if (pTexture != NULL)
								{// �e�N�X�`����ǂݍ��߂�
									m_pTextureManager->BindTexture(pTexture, nCntTex);
									m_pTextureManager->SetFileName(pStr, nCntTex);
									pTexture = NULL;
									nCntTex++;
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// �}�b�v�f�[�^�̃X�N���v�g�t�@�C����������
							    // �}�b�v�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(aMapFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
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

			fclose(pFile);
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

	if (m_pScene == NULL)
	{
		m_pScene = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		if (m_pScene != NULL)
		{// ���������m�ۂł���
			if (m_Result == RESULT_CLEAR)
			{
				m_pScene->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_1));
			}
			else if (m_Result == RESULT_GAMEOVER)
			{
				m_pScene->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_2));
			}
		}
	}

	// PressEnter�̍쐬
	if (m_pPress == NULL)
	{
		m_pPress = CScene2DPress::Create(RESULT_PRESS_POS, RESULT_PRESS_COL, RESULT_PRESS_WIDTH, RESULT_PRESS_HEIGHT, 0.0f, 80, 100, 4, 6);
		if (m_pPress != NULL)
		{// ���������m�ۂł���
			m_pPress->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_0));
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CResult::Uninit(void)
{
	// �e�N�X�`���Ǌ��N���X���J������
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// PressEnter�̔j��
	if (m_pPress != NULL)
	{
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	m_Result = RESULT_GAMEOVER;

	// ���U���g�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CResult::Update(void)
{
	m_nCounter++;  // �J�E���^�[��i�߂�

	if (m_nCounter >= RESULT_MODECHANGE_TIMING)
	{// �J�E���^�[������̒l�𒴂���
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			CManager::GetFade()->SetFade(CManager::MODE_RANKING);
		}
	}
	else
	{// �J�E���^�[���܂�����̒l�𒴂��Ă��Ȃ�
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// ����{�^���������ꂽ
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
				CManager::GetFade()->SetFade(CManager::MODE_RANKING);
				if (m_pPress != NULL)
				{// ���������m�ۂ���Ă���
					m_pPress->FlashStart();
				}
			}
			else
			{// �L�[�{�[�h�̌���{�^����������Ă��Ȃ�
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_7; nCntButton++)
				{// ��������{�^�����̐������J��Ԃ�
					if (CManager::GetJoyStick()->GetJoyStickDevice()->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// �{�^���������ꂽ
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
						CManager::GetFade()->SetFade(CManager::MODE_RANKING);
						if (m_pPress != NULL)
						{// ���������m�ۂ���Ă���
							m_pPress->FlashStart();
						}
					}
				}
			}
		}
	}

	CDebugProc::Print("���U���g���\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CResult::Draw(void)
{

}

//=============================================================================
//    ���ʂ�ݒ肷�鏈��
//=============================================================================
void CResult::SetResult(RESULT result)
{
	m_Result = result;
}

//=============================================================================
//    ���ʂ�ݒ肷�鏈��
//=============================================================================
CResult::RESULT CResult::GetResult(void)
{
	return m_Result;
}