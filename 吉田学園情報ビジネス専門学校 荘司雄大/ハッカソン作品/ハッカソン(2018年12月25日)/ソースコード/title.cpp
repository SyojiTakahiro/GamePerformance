//*****************************************************************************
//
//     �^�C�g���̏���[title.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "title.h"
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
#define TITLE_SYSTEM_FILENAME   "data/TEXT/MODE/title.ini"                    // �^�C�g���̏��������X�N���v�g�t�@�C����
#define TITLE_MODECHANGE_TIMING (600)                                         // ��ʑJ�ڂ������ōs���^�C�~���O

// �l��ǂݎ��p�X��
#define NUM_TEXTURE             "NUM_TEXTURE = "                              // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME        "TEXTURE_FILENAME = "                         // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define MAP_FILENAME            "MAP_FILENAME = "                             // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����

// �^�C�g�����S�p�̃}�N��
#define TITLE_LOGO_POS          (D3DXVECTOR3(SCREEN_WIDTH / 2,250.0f,0.0f))   // �^�C�g�����S�̍��W
#define TITLE_LOGO_COL          (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))              // �^�C�g�����S�̐F
#define TITLE_LOGO_WIDTH        (500.0f)                                      // �^�C�g�����S�̕�
#define TITLE_LOGO_HEIGHT       (230.0f)                                      // �^�C�g�����S�̍���

// PressEnter�p�̃}�N��
#define TITLE_PRESS_POS          (D3DXVECTOR3(SCREEN_WIDTH / 2,600.0f,0.0f))  // PressEnter�̍��W
#define TITLE_PRESS_COL          (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // PressEnter�̐F
#define TITLE_PRESS_WIDTH        (400.0f)                                     // PressEnter�̕�
#define TITLE_PRESS_HEIGHT       (100.0f)                                     // PressEnter�̍���

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CTitle::CTitle(int nPriority, OBJTYPE objType) : CScene(nPriority,objType)
{
	// �e��l�̃N���A
	m_pMap = NULL;              // �}�b�v�N���X�ւ̃|�C���^
	m_nCounter = 0;             // �J�ڂ��Ǘ�����J�E���^�[
	m_pTitleLogo = NULL;        // �^�C�g�����S�p��2D�|���S���N���X�ւ̃|�C���^
	m_pPress = NULL;            // PressEnter�p��2D�|���S���N���X�ւ̃|�C���^
	m_pTextureManager = NULL;   // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
//    ��������
//=============================================================================
CTitle *CTitle::Create(void)
{
	CTitle *pTitle = NULL;  // �^�C�g���N���X�^�̃|�C���^
	if (pTitle == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pTitle = new CTitle;
		if (pTitle != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pTitle->Init();
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
	return pTitle;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	// �^�C�g���̏����������O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(TITLE_SYSTEM_FILENAME, "r");
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

					// �}�b�v�̐���
					m_pMap = CMap::Create(aMapFileName);

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

	// PressEnter�̍쐬
	if (m_pPress == NULL)
	{
		m_pPress = CScene2DPress::Create(TITLE_PRESS_POS, TITLE_PRESS_COL, TITLE_PRESS_WIDTH, TITLE_PRESS_HEIGHT, 0.0f, 80, 100, 4, 6);
		if (m_pPress != NULL)
		{// ���������m�ۂł���
			m_pPress->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_0));
		}
	}

	// �^�C�g�����S�̍쐬
	if (m_pTitleLogo == NULL)
	{
		m_pTitleLogo = CScene2D::Create(TITLE_LOGO_POS, TITLE_LOGO_COL, TITLE_LOGO_WIDTH, TITLE_LOGO_HEIGHT,0.0f,6);
		if (m_pTitleLogo != NULL)
		{// ���������m�ۂł���
			m_pTitleLogo->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_1));
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CTitle::Uninit(void)
{
	// �}�b�v�N���X���J������
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();

		delete m_pMap;
		m_pMap = NULL;
	}

	// �e�N�X�`���Ǌ��N���X���J������
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// �^�C�g�����S�̔j��
	if (m_pTitleLogo != NULL)
	{
		m_pTitleLogo->Uninit();
		m_pTitleLogo = NULL;
	}

	// PressEnter�̔j��
	if (m_pPress != NULL)
	{
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	// �^�C�g���I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CTitle::Update(void)
{
	m_nCounter++;  // �J�E���^�[��i�߂�

	if (m_nCounter >= TITLE_MODECHANGE_TIMING)
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
				CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
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
						CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
						if (m_pPress != NULL)
						{// ���������m�ۂ���Ă���
							m_pPress->FlashStart();
						}
					}
				}
			}
		}
	}

	CDebugProc::Print("�^�C�g�����\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}