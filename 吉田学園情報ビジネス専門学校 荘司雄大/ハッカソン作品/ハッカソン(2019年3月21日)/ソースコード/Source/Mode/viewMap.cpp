//*****************************************************************************
//
//     �}�b�v�Љ��ʂ̏���[viewMap.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mode.h"
#include "manager.h"
#include "system.h"
#include "functionlib.h"
#include "scene.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define VIEWMAP_SYSTEM_FILENAME    "data/TEXT/MODE/viewMap.ini"   // �}�b�v�Љ��ʂ̏��������X�N���v�g�t�@�C����
#define VIEWMAP_MODECHANGE_TIMING  (300)                          // ��ʑJ�ڂ������ōs���^�C�~���O

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CViewMap::CViewMap()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CViewMap::~CViewMap()
{

}

//=============================================================================
//    ��������
//=============================================================================
CViewMap *CViewMap::Create(void)
{
	CViewMap *pViewMap = NULL;  // �}�b�v�Љ��ʃN���X�ւ̃|�C���^
	if (pViewMap == NULL)
	{// ���������m�ۂł����Ԃł���
		pViewMap = new CViewMap;
		if (pViewMap != NULL)
		{// ���������m�ۂł���
			if (FAILED(pViewMap->Init()))
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

	return pViewMap;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CViewMap::Init(void)
{
	// �}�b�v�Љ��ʂ̏����������O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(VIEWMAP_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                         // 1�s���ǂݎ��p
			char *pStrCur = NULL;                       // ���݂̐擪�|�C���^
			char *pStr = NULL;                          // �擪�|�C���^�ۑ��p

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
							if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
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
void CViewMap::Uninit(void)
{
	// ���ʂ̏I������
	CBaseMode::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CViewMap::Update(void)
{
	// �S�ẴI�u�W�F�N�g���X�V����
	CScene::UpdateAll();

	// ��ʑJ�ڃ`�F�b�N
	ModeChange(CManager::MODE_TITLE, CManager::MODE_TITLE, VIEWMAP_MODECHANGE_TIMING);

	CDebugProc::Print("�}�b�v�Љ���\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CViewMap::Draw(void)
{

}