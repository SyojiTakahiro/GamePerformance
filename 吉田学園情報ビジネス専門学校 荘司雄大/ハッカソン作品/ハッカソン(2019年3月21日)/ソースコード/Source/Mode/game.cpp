//*****************************************************************************
//
//     �Q�[���̏���[game.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mode.h"
#include "manager.h"
#include "system.h"
#include "functionlib.h"
#include "scene.h"
#include "character.h"
#include "effect.h"
#include "map.h"
#include "textureManager.h"
#include "pause.h"
#include "bullet.h"
#include "object.h"
#include "star.h"
#include "timer.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define PAUSE_PRIORITY         (10)
#define GAME_SYSTEM_FILENAME   "data/TEXT/MODE/game.ini"   // �Q�[���̏��������X�N���v�g�t�@�C����
#define GAME_MODECHANGE_TIMING (300)                       // ��ʑJ�ڂ������ōs���^�C�~���O
#define GAME_MAX_MAP           (3)                         // �}�b�v�f�[�^�̍ő吔

// �l��ǂݎ��p�X��
#define NUM_TEXTURE                 "NUM_TEXTURE = "            // �ǂݍ��ރe�N�X�`���̐�
#define NUM_MAP                     "NUM_MAP = "                // �ǂݍ��ރ}�b�v�̐�
#define TEXTURE_FILENAME            "TEXTURE_FILENAME = "       // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define EFFECT_FILENAME             "EFFECT_FILENAME = "        // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����
#define MAP_FILENAME                "MAP_FILENAME = "           // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����

// �v���C���[���
#define PLAYERSET                   "PLAYERSET"                 // �v���C���[���ǂݍ��݊J�n�̍��}
#define END_PLAYERSET               "END_PLAYERSET"             // �v���C���[���ǂݍ��ݏI���̍��}
#define LIFE                        "LIFE = "                   // �v���C���[�̗̑�
#define FILENAME                    "FILENAME = "               // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
int CGame::m_nNumPlayer = 0;
int CGame::m_nGameMapIdx = 0;
CGame::PLAYER_SETTING CGame::m_PlayerSetting[2];

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGame::CGame()
{
	m_State = STATE_NORMAL;
	m_bPause = false;
	m_pPause = NULL;
	m_nStateCounter = 0;
	m_nPauseController = 0;

	m_pPlayer[0] = NULL;
	m_pPlayer[1] = NULL;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGame *CGame::Create(void)
{
	CGame *pGame = NULL;  // �Q�[���N���X�ւ̃|�C���^
	if (pGame == NULL)
	{// ���������m�ۂł����Ԃł���
		pGame = new CGame;
		if (pGame != NULL)
		{// ���������m�ۂł���
			if (FAILED(pGame->Init()))
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

	return pGame;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGame::Init(void)
{

	CManager::GetSound()->PlaySound(2);

	CShadow::Load();
	CBullet::Load();
	CStar::Load();

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
			int nNumTex = 0;                            // �e�N�X�`����ǂݍ��ސ�
			int nCntTex = 0;                            // �e�N�X�`����ǂݍ��񂾉�
			int nCntPlayer = 0;                         // �v���C���[�f�[�^��ǂݍ��񂾉�
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // �e�N�X�`���ǂݎ��p
			char aTexFileName[256] = "\0";              // �e�N�X�`���̃t�@�C����
			char aPlayerFileName[2][256];               // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
			char aEffectFileName[256];                  // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����
			int nNumMap = 0;                            // �}�b�v�f�[�^�̐�
			int nCntMap = 0;                            // �}�b�v�f�[�^��ǂݍ��񂾐�
			char aMapFileName[GAME_MAX_MAP][256];       // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����
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
							else if (CFunctionLib::Memcmp(pStrCur, PLAYERSET) == 0)
							{// �v���C���[�f�[�^�ǂݍ��݊J�n�̍��}������
								while (1)
								{// ���[�v�J�n
									pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
									if (CFunctionLib::Memcmp(pStrCur, LIFE) == 0)
									{// �̗͓ǂݍ��݂̍��}������
										nPlayerLife[nCntPlayer] = CFunctionLib::ReadInt(pStrCur, LIFE);
									}
									else if (CFunctionLib::Memcmp(pStrCur, FILENAME) == 0)
									{// �v���C���[�f�[�^�̃X�N���v�g�t�@�C����������
									    // �v���C���[�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
										pStr = CFunctionLib::ReadString(pStrCur, pStr, FILENAME);
										strcpy(aPlayerFileName[nCntPlayer], pStr);
									}
									else if (CFunctionLib::Memcmp(pStrCur, END_PLAYERSET) == 0)
									{// �v���C���[���ǂݍ��ݏI���̍��}������
										nCntPlayer++;
										break;
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, EFFECT_FILENAME) == 0)
							{// �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����������
							    // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, EFFECT_FILENAME);
								strcpy(aEffectFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MAP) == 0)
							{// �}�b�v�̌���񂪂�����
								nNumMap = CFunctionLib::ReadInt(pStrCur, NUM_MAP);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// �}�b�v�f�[�^�̃X�N���v�g�t�@�C����������
							    // �}�b�v�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(aMapFileName[nCntMap], pStr);
								nCntMap++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}

					// �e��N���X�𐶐�����
					// �v���C���[�Ǌ��N���X
					for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
					{// �v���C���[�̐������J��Ԃ�
						SetPlayerManager(CPlayerManager::Create(aPlayerFileName[nCntPlayer], m_PlayerSetting[nCntPlayer].nControllerIdx));
					}

					// �G�t�F�N�g�Ǌ��N���X
					SetEffectManager(CEffectManager::Create(aEffectFileName));

					// �}�b�v�Ǌ��N���X
					SetMap(CMap::Create(aMapFileName[0], GetEffectManager()));

					// ���X�g����v���C���[�N���X�փ|�C���^���擾���Ă���
					CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
					CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
					int nNumPlayer = 0;
					CPlayer *pPlayer[2];
					D3DXVECTOR3 Position[2] = { D3DXVECTOR3(0.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, 0.0f, 1000.0f) };
					D3DXVECTOR3 Rotation[2] = { D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) };
					for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
					{// �`��D�揇�ʂ̐������J��Ԃ�
						pScene = CScene::GetTop(nCntPriority);
						while (pScene != NULL)
						{// ����������ɂȂ�܂ŌJ��Ԃ�
							pSceneNext = pScene->GetNext();
							if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
							{// �v���C���[�N���X�^�̃|�C���^��������
								m_pPlayer[nNumPlayer] = (CPlayer*)pScene;
								pPlayer[nNumPlayer] = (CPlayer*)pScene;
								pPlayer[nNumPlayer]->SetPos(Position[nNumPlayer]);
								pPlayer[nNumPlayer]->SetRot(Rotation[nNumPlayer]);
								pPlayer[nNumPlayer]->SetDestRot(Rotation[nNumPlayer]);
								nNumPlayer++;
								if (nNumPlayer >= m_nNumPlayer)
								{
									break;
								}
							}
							pScene = pSceneNext;
						}
					}

					// �J�����𐶐�����
					//int nCntCamera = 0;
					//D3DVIEWPORT9 ViewPort;
					//ViewPort.X = 0;
					//ViewPort.Y = 0;
					//ViewPort.Height = SCREEN_HEIGHT;
					//ViewPort.MaxZ = 1.0f;
					//ViewPort.MinZ = 0.0f;
					//if (m_nNumPlayer == 2)
					//{// �K�v�ȃv���C���[���Q�l������
					//	ViewPort.Width = SCREEN_WIDTH / 2;
					//}
					//else if (m_nNumPlayer == 1)
					//{// �K�v�ȃv���C���[���P�l������
					//	ViewPort.Width = SCREEN_WIDTH;
					//}

					//SetCamera(CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, ViewPort, pPlayer[nCntCamera]), nCntCamera);
					//nCntCamera++;

					//if (m_nNumPlayer == 2)
					//{// �K�v�ȃv���C���[���Q�l������
					//	ViewPort.X = SCREEN_WIDTH / 2;
					//	SetCamera(CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, ViewPort, pPlayer[nCntCamera]), nCntCamera);
					//	nCntCamera++;
					//}

					int nCntCamera = 0;
					D3DVIEWPORT9 ViewPort;
					ViewPort.X = 0;
					ViewPort.Y = 0;
					ViewPort.Width = SCREEN_WIDTH;
					ViewPort.MaxZ = 1.0f;
					ViewPort.MinZ = 0.0f;
					if (m_nNumPlayer == 2)
					{// �K�v�ȃv���C���[���Q�l������
						ViewPort.Height = SCREEN_HEIGHT / 2;
					}
					else if (m_nNumPlayer == 1)
					{// �K�v�ȃv���C���[���P�l������
						ViewPort.Height = SCREEN_HEIGHT;
					}

					SetCamera(CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, ViewPort, pPlayer[nCntCamera]), nCntCamera);
					nCntCamera++;

					if (m_nNumPlayer == 2)
					{// �K�v�ȃv���C���[���Q�l������
						ViewPort.Y = SCREEN_HEIGHT / 2;
						SetCamera(CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, ViewPort, pPlayer[nCntCamera]), nCntCamera);
						nCntCamera++;
					}

					// �e�N�X�`���Ǌ��N���X��ݒ肷��
					if (pTextureManager != NULL)
					{
						SetTextureManager(pTextureManager);
					}

					// �I�u�W�F�N�g�z�u
					SetStarObject();

					// �^�C�}�[�𐶐�
					CTimer *pTimer = NULL;
					pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 150.0f, 60.0f,0.0));
					if (pTimer != NULL)
					{
						pTimer->BindTexture(pTextureManager->GetTexture(4));
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

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGame::Uninit(void)
{
	CShadow::UnLoad();
	CBullet::UnLoad();
	CStar::UnLoad();

	// ���ʂ̏I������
	CBaseMode::Uninit();

	if (m_bPause == false)
	{
		// �v���C���[�̐l����߂��Ă���
		CResult::SetNumPlayer(m_nNumPlayer);
		m_nNumPlayer = 0;
	}

	CManager::GetSound()->StopSound(2);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGame::Update(void)
{
	// �J�������X�V����
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA_NUM; nCntCamera++)
	{// �J�����̐������J��Ԃ�
		if (GetCamera(nCntCamera) != NULL)
		{// ���������m�ۂ���Ă���
		    // �J�����̍X�V����
			GetCamera(nCntCamera)->Update();
		}
	}

	switch (m_State)
	{// ��Ԃɂ���ď����킯
	case STATE_NORMAL:
		Normal();
		break;
	case STATE_END:
		End();
		break;
	}

	CDebugProc::Print("�Q�[�����\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
//    �ʏ��Ԃ̏���
//=============================================================================
void CGame::Normal(void)
{
	CManager::MODE mode = CManager::GetMode();  // ���݂̃��[�h���擾

	if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
	{// �t�F�[�h���Ă��Ȃ�������
		if (CManager::GetKeyboard()->GetTrigger(DIK_P) == true)
		{// �|�[�YON/OFF
			m_bPause = m_bPause ? false : true;
			if (m_pPause == NULL && m_bPause == true)
			{// �|�[�Y���Ƀ��������m�ۂ���Ă��Ȃ�
				m_pPause = CPause::Create(PAUSE_PRIORITY);
				//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPEN);
			}
			else if (m_pPause != NULL)
			{// �|�[�Y�������Ƀ��������m�ۂ���Ă���
				ReleasePause();
			}
		}
		for (int nCntStick = 0; nCntStick < 2; nCntStick++)
		{
			if (CManager::GetJoyStick()->GetJoyStickDevice(nCntStick)->GetTrigger(CJoyStick::DIJS_BUTTON_11) == TRUE)
			{
				if (m_pPause == NULL && m_bPause == true)
				{// �|�[�Y���Ƀ��������m�ۂ���Ă��Ȃ�
					m_pPause = CPause::Create(PAUSE_PRIORITY);
					m_nPauseController = nCntStick;
					//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPEN);
				}
				else if (m_pPause != NULL)
				{// �|�[�Y�������Ƀ��������m�ۂ���Ă���
					ReleasePause();
				}
			}
		}
	}

	if (m_bPause == false)
	{// �|�[�Y���łȂ�
	    // �V�[���N���X�̍X�V����
		CScene::UpdateAll();
	}
	else
	{// �|�[�Y���ł���
		if (m_pPause != NULL)
		{// ���������m�ۂ���Ă���
		    // �|�[�Y�̍X�V����
			m_pPause->Update();
			CScene::DeathCheck();
		}
	}
}

//=============================================================================
//    �I����Ԃ̏���
//=============================================================================
void CGame::End(void)
{
	// �S�ẴI�u�W�F�N�g���X�V����
	CScene::UpdateAll();

	m_nStateCounter++;
	if (m_nStateCounter > 120)
	{// �I����ԂɂȂ��Ă��炠����x������
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h���Ă��Ȃ�������
			CManager::GetFade()->SetFade(CManager::MODE_RESULT);
		}
	}
}

//=============================================================================
//    �|�[�Y�N���X���J�����鏈��
//=============================================================================
void CGame::ReleasePause(void)
{
	if (m_pPause != NULL)
	{// �|�[�Y�������Ƀ��������m�ۂ���Ă���
	    // �|�[�Y�̏I������
		m_pPause->Uninit();

		// �������̊J��
		delete m_pPause;
		m_pPause = NULL;
	}
}

//=============================================================================
//    �v���C���[����ݒ肷�鏈��
//=============================================================================
void CGame::SettingPlayer(const int nControllerIdx)
{
	// �v���C���[���g�p�����Ԃ�
	m_PlayerSetting[m_nNumPlayer].bUse = true;
	m_PlayerSetting[m_nNumPlayer].nControllerIdx = nControllerIdx;

	// �v���C���[�̐l���𑝂₷
	m_nNumPlayer++;
}

//=============================================================================
//    ���p�̃I�u�W�F�N�g���o������
//=============================================================================
void CGame::SetStarObject(void)
{
	if (GetMap() != NULL)
	{
		if (GetMap()->GetObjectManager(2) != NULL)
		{
			D3DXVECTOR3 ObjPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ObjPos.x = rand() % 3600 - 1800.0f;
			ObjPos.z = rand() % 3600 - 1800.0f;
			D3DXVECTOR3 ObjRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 ObjScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			GetMap()->GetObjectManager(2)->CreateStarObject(ObjPos, ObjRot, ObjScale, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, true, OBJECT_PRIORITY);
		}
	}
}

//=============================================================================
//    �}�b�v�̔ԍ���ݒ肷�鏈��
//=============================================================================
void CGame::SetGameMapIdx(const int nGameMapIdx)
{
	m_nGameMapIdx = nGameMapIdx;
}

//=============================================================================
//    �v���C���[�̐l����ݒ肷�鏈��
//=============================================================================
void CGame::SetNumPlayer(const int nNumPlayer)
{
	m_nNumPlayer = nNumPlayer;
}

//=============================================================================
//    ��Ԃ�ݒ肷�鏈��
//=============================================================================
void CGame::SetState(const CGame::STATE state)
{
	m_State = state;
	m_nStateCounter = 0;

	if (state == STATE_END)
	{
		// ���s�̐ݒ�
		if (m_pPlayer[0] != NULL && m_pPlayer[1] != NULL)
		{
			if (m_pPlayer[0]->GetScore() <= m_pPlayer[1]->GetScore())
			{
				CResult::SetWinnerIdx(1);
			}
		}
	}
}

//=============================================================================
//    �|�[�Y�����ǂ����ݒ肷�鏈��
//=============================================================================
void CGame::SetPause(const bool bPause)
{
	m_bPause = bPause;
}

//=============================================================================
//     �|�[�Y���������R���g���[���[�̔ԍ���ݒ肷�鏈��
//=============================================================================
void CGame::SetPauseController(const int nPauseController)
{
	m_nPauseController = nPauseController;
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
CGame::STATE CGame::GetState(void)
{
	return m_State;
}

//=============================================================================
//     �|�[�Y�����ǂ����擾���鏈��
//=============================================================================
bool CGame::GetPause(void)
{
	return m_bPause;
}

//=============================================================================
//     �|�[�Y���������R���g���[���[�̔ԍ����擾���鏈��
//=============================================================================
int CGame::GetPauseController(void)
{
	return m_nPauseController;
}