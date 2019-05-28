//*****************************************************************************
//
//     ���U���g�̏���[result.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mode.h"
#include "manager.h"
#include "system.h"
#include "functionlib.h"
#include "scene.h"
#include "2DPolygon.h"
#include "textureManager.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define RESULT_SYSTEM_FILENAME    "data/TEXT/MODE/result.ini"   // ���U���g�̏��������X�N���v�g�t�@�C����
#define RESULT_MODECHANGE_TIMING  (300)                         // ��ʑJ�ڂ������ōs���^�C�~���O

// �l��ǂݎ��p�X��
#define NUM_TEXTURE                 "NUM_TEXTURE = "            // �ǂݍ��ރe�N�X�`���̐�
#define NUM_MAP                     "NUM_MAP = "                // �ǂݍ��ރ}�b�v�̐�
#define TEXTURE_FILENAME            "TEXTURE_FILENAME = "       // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define EFFECT_FILENAME             "EFFECT_FILENAME = "        // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����
#define MAP_FILENAME                "MAP_FILENAME = "           // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
int CResult::m_nNumPlayer = 0;
int CResult::m_nWinnerIdx = 0;


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CResult::CResult()
{
	m_pBg[0] = NULL;
	m_pBg[1] = NULL;
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
	CResult *pResult = NULL;  // ���U���g�N���X�ւ̃|�C���^
	if (pResult == NULL)
	{// ���������m�ۂł����Ԃł���
		pResult = new CResult;
		if (pResult != NULL)
		{// ���������m�ۂł���
			if (FAILED(pResult->Init()))
				return NULL;
		}
		else
		{// ���������m�ۂł����Ȃ�����
			return NULL;
		}
	}
	else
	{// ���������m�ۂł����ԂłȂ�
		return NULL;
	}

	return pResult;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CResult::Init(void)
{
	CManager::GetSound()->PlaySound(3);

	// ���U���g�����������O������ǂݍ���
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
			int nCntPlayer = 0;                         // �v���C���[�f�[�^��ǂݍ��񂾉�
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // �e�N�X�`���ǂݎ��p
			char aTexFileName[256] = "\0";              // �e�N�X�`���̃t�@�C����
			int nNumMap = 0;                            // �}�b�v�f�[�^�̐�
			int nCntMap = 0;                            // �}�b�v�f�[�^��ǂݍ��񂾐�
			int nPlayerLife[2] = {};                    // �v���C���[�̗̑�
			CTextureManager *pTextureManager = NULL;    // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^

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
									pTextureManager = CTextureManager::Create(nNumTex);
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
									pTextureManager->BindTexture(pTexture, nCntTex);
									pTextureManager->SetFileName(pStr, nCntTex);
									pTexture = NULL;
									nCntTex++;
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}

					// �J�����̐���
					D3DVIEWPORT9 ViewPort;
					ViewPort.X = 0;
					ViewPort.Y = 0;
					ViewPort.Width = SCREEN_WIDTH;
					ViewPort.Height = SCREEN_HEIGHT;
					ViewPort.MaxZ = 1.0f;
					ViewPort.MinZ = 0.0f;
					SetCamera(CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, ViewPort, NULL), 0);


					m_pBg[0] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
					if (m_pBg[0] != NULL)
					{
						m_pBg[0]->BindTexture(pTextureManager->GetTexture(0));
					}

					m_pBg[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
					if (m_pBg[1] != NULL)
					{
						m_pBg[1]->BindTexture(pTextureManager->GetTexture(1 + m_nWinnerIdx));
					}

					SetTextureManager(pTextureManager);

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

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CResult::Uninit(void)
{
	// ���ʂ̏I������
	CBaseMode::Uninit();

	m_nNumPlayer = 0;
	m_nWinnerIdx = 0;

	CManager::GetSound()->StopSound(3);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CResult::Update(void)
{
	// �S�ẴI�u�W�F�N�g���X�V����
	CScene::UpdateAll();

	// ��ʑJ�ڃ`�F�b�N
	ModeChange(CManager::MODE_TITLE, CManager::MODE_SETTING, RESULT_MODECHANGE_TIMING);

	CDebugProc::Print("���U���g���\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CResult::Draw(void)
{

}

//=============================================================================
//    �v���C���[�̐l����ݒ肷��
//=============================================================================
void CResult::SetNumPlayer(const int nNumPlayer)
{
	m_nNumPlayer = nNumPlayer;
}

//=============================================================================
//    �������ق��̃v���C���[�̔ԍ���ݒ肷��
//=============================================================================
void CResult::SetWinnerIdx(const int nWinnerIdx)
{
	m_nWinnerIdx = nWinnerIdx;
}