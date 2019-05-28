//*****************************************************************************
//
//     �^�C�g���̏���[title.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mode.h"
#include "manager.h"
#include "system.h"
#include "functionlib.h"
#include "scene.h"
#include "2DPolygon.h"
#include "effect.h"
#include "textureManager.h"


//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define TITLE_SYSTEM_FILENAME    "data/TEXT/MODE/title.ini"   // �^�C�g���̏��������X�N���v�g�t�@�C����
#define TITLE_MODECHANGE_TIMING  (300)                        // ��ʑJ�ڂ������ōs���^�C�~���O

// �l��ǂݎ��p�X��
#define NUM_TEXTURE                 "NUM_TEXTURE = "            // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME            "TEXTURE_FILENAME = "       // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define MAP_FILENAME				"MAP_FILENAME = "                             // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����
//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		m_pBg[nCnt] = NULL;        // �w�i
	}
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
	CTitle *pTitle = NULL;  // �^�C�g���N���X�ւ̃|�C���^
	if (pTitle == NULL)
	{// ���������m�ۂł����Ԃł���
		pTitle = new CTitle;
		if (pTitle != NULL)
		{// ���������m�ۂł���
			if (FAILED(pTitle->Init()))
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

	return pTitle;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	CManager::GetSound()->PlaySound(0);

	// �Z�b�e�B���O�̏����������O������ǂݍ���
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
			CTextureManager *pTextureManager = NULL;    // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
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

					// �w�i�̐���
					m_pBg[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
					m_pBg[0] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 300, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 600, 320);
					if (m_pBg != NULL)
					{
						m_pBg[0]->BindTexture(pTextureManager->GetTexture(0));
						m_pBg[1]->BindTexture(pTextureManager->GetTexture(1));
					}

					CScene2DPress *pPress = NULL;
					pPress = CScene2DPress::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 400.0f, 100.0f);
					if (pPress != NULL)
					{
						pPress->BindTexture(pTextureManager->GetTexture(2));
					}
					SetPress(pPress);

					if (pTextureManager != NULL)
					{
						SetTextureManager(pTextureManager);
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
void CTitle::Uninit(void)
{
	// ���ʂ̏I������
	CBaseMode::Uninit();

	CManager::GetSound()->StopSound(0);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CTitle::Update(void)
{
	// �S�ẴI�u�W�F�N�g���X�V����
	CScene::UpdateAll();

	// ��ʑJ�ڃ`�F�b�N
	ModeChange(CManager::MODE_TITLE, CManager::MODE_SETTING, TITLE_MODECHANGE_TIMING);

	CDebugProc::Print("�^�C�g�����\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}