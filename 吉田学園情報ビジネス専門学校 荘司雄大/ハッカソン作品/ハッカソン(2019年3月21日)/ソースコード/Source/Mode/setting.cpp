//*****************************************************************************
//
//     �Z�b�e�B���O�̏���[setting.cpp]
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
#define SETTING_SYSTEM_FILENAME    "data/TEXT/MODE/setting.ini"   // �Z�b�e�B���O�̏��������X�N���v�g�t�@�C����
#define SETTING_MODECHANGE_TIMING  (300)                          // ��ʑJ�ڂ������ōs���^�C�~���O

// �l��ǂݎ��p�X��
#define NUM_TEXTURE                 "NUM_TEXTURE = "            // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME            "TEXTURE_FILENAME = "       // �ǂݍ��ރe�N�X�`���̃t�@�C����


#define MAPSELECT_MAX          (3)
#define MAPSELECT_POS_INI      (D3DXVECTOR3(SCREEN_WIDTH / 2,80.0f,0.0f))       // �|�[�Y���ڂ̍��W(�����l)
#define MAPSELECT_COL_INI      (D3DXCOLOR(0.5f,0.5f,0.5f,1.0f))                 // �|�[�Y���ڂ̐F(�����l)
#define MAPSELECT_WIDTH_INI    (180.0f)                                         // �I������Ă���|�[�Y�I�����ڂ̕�
#define MAPSELECT_HEIGHT_INI   (180.0f)                                         // �I������Ă���|�[�Y�I�����ڃ|�[�Y�I�����ڂ̍���
#define MAPSELECT_WIDTH_NONE   (150.0f)                                         // �I������Ă��Ȃ��|�[�Y�I�����ڂ̕�
#define MAPSELECT_HEIGHT_NONE  (150.0f)                                         // �I������Ă��Ȃ��|�[�Y�I�����ڂ̍���
#define MAPSELECT_COL_NONE     (D3DXCOLOR(0.1f,0.1f,0.1f,1.0f))                 // �I������Ă��Ȃ��|�[�Y�I�����ڂ̐F(�����l)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSetting::CSetting()
{
	m_nSelectMapIdx = 0;   // �Q�[���Ŏg�p����}�b�v�̃f�[�^
	m_pBg = NULL;        // �w�i
	m_nOnePlayer = -1;

	m_bJoin[0] = false;
	m_bJoin[1] = false;
	m_pJoin[0] = NULL;
	m_pJoin[1] = NULL;
	m_pMapSelect[0] = NULL;
	m_pMapSelect[1] = NULL;
	m_pMapSelect[2] = NULL;
	m_pJoinPress = NULL;
	m_nNumPlayer = 0;
	m_State = STATE_CHECKJOIN;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSetting::~CSetting()
{

}

//=============================================================================
//    ��������
//=============================================================================
CSetting *CSetting::Create(void)
{
	CSetting *pSetting = NULL;  // �Z�b�e�B���O�N���X�ւ̃|�C���^
	if (pSetting == NULL)
	{// ���������m�ۂł����Ԃł���
		pSetting = new CSetting;
		if (pSetting != NULL)
		{// ���������m�ۂł���
			if (FAILED(pSetting->Init()))
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

	return pSetting;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CSetting::Init(void)
{
	CManager::GetSound()->PlaySound(1);

	// �Z�b�e�B���O�̏����������O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(SETTING_SYSTEM_FILENAME, "r");
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

					// �w�i�̐���
					m_pBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
					if (m_pBg != NULL)
					{
						m_pBg->BindTexture(pTextureManager->GetTexture(0));
					}

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
void CSetting::Uninit(void)
{
	// ���ʂ̏I������
	CBaseMode::Uninit();

	CManager::GetSound()->StopSound(1);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CSetting::Update(void)
{
	// �S�ẴI�u�W�F�N�g���X�V����
	CScene::UpdateAll();

	if (m_State == STATE_CHECKJOIN)
	{
		CheckJoin();
	}
	else if (m_State == STATE_MAPSELECT)
	{
		MapSelect();
	}

	CDebugProc::Print("�Z�b�e�B���O���\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CSetting::Draw(void)
{

}

//=============================================================================
//    �Q����t��Ԃ̏���
//=============================================================================
void CSetting::CheckJoin(void)
{
	CJoyStick *pJoyStick = NULL;

	for (int nCntStick = 0; nCntStick < 2; nCntStick++)
	{
		pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(nCntStick);
		if (pJoyStick != NULL)
		{
			if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_11) == true)
			{
				if (m_bJoin[nCntStick] != true)
				{// �Q�킵�Ă��Ȃ�
					D3DXVECTOR3 JoinPos = D3DXVECTOR3(960.0f, SCREEN_HEIGHT / 2, 0.0f);
					int nTexIdx = 2;
					CGame::SettingPlayer(nCntStick);
					m_bJoin[nCntStick] = true;
					m_nNumPlayer++;
					if (m_nOnePlayer == -1)
					{
						nTexIdx = 1;
						JoinPos = D3DXVECTOR3(320.0f, SCREEN_HEIGHT / 2, 0.0f);
						m_nOnePlayer = nCntStick;
					}


					m_pJoin[nCntStick] = CScene2D::Create(JoinPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300.0f, 340.0f);
					if (m_pJoin[nCntStick] != NULL)
					{
						m_pJoin[nCntStick]->BindTexture(GetTextureManager()->GetTexture(nTexIdx));
					}
				}
				else
				{// �����Q�킵�Ă���
					m_State = STATE_MAPSELECT;
					if (m_pJoin[0] != NULL)
					{
						m_pJoin[0]->Uninit();
						m_pJoin[0] = NULL;
					}
					if (m_pJoin[1] != NULL)
					{
						m_pJoin[1]->Uninit();
						m_pJoin[1] = NULL;
					}

					m_pMapSelect[0] = CScene2DFlash::Create(D3DXVECTOR3(230.0f, SCREEN_HEIGHT / 2, 0.0f), MAPSELECT_COL_INI, MAPSELECT_WIDTH_INI, MAPSELECT_HEIGHT_INI, 0.0f, CScene2DFlash::STATE_SELECT);
					if (m_pMapSelect[0] != NULL)
					{
						m_pMapSelect[0]->BindTexture(GetTextureManager()->GetTexture(3));
					}

					m_pMapSelect[1] = CScene2DFlash::Create(D3DXVECTOR3(640.0f, SCREEN_HEIGHT / 2, 0.0f), MAPSELECT_COL_NONE, MAPSELECT_WIDTH_NONE, MAPSELECT_HEIGHT_NONE, 0.0f, CScene2DFlash::STATE_NONE);
					if (m_pMapSelect[1] != NULL)
					{
						m_pMapSelect[1]->BindTexture(GetTextureManager()->GetTexture(4));
					}

					m_pMapSelect[2] = CScene2DFlash::Create(D3DXVECTOR3(1040.0f, SCREEN_HEIGHT / 2, 0.0f), MAPSELECT_COL_NONE, MAPSELECT_WIDTH_NONE, MAPSELECT_HEIGHT_NONE, 0.0f, CScene2DFlash::STATE_NONE);
					if (m_pMapSelect[2] != NULL)
					{
						m_pMapSelect[2]->BindTexture(GetTextureManager()->GetTexture(5));
					}

					CScene::DeathCheck();
				}
			}
		}
	}
}

//=============================================================================
//    �}�b�v�Z���N�g��Ԃ̏���
//=============================================================================
void CSetting::MapSelect(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();                   // �L�[�{�[�h�̎擾
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(m_nOnePlayer);   // �W���C�X�e�B�b�N�̎擾

	// �I�����ڂ̏���
	if (pKeyboard->GetTrigger(DIK_W) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_23) == TRUE || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_31) == TRUE
		|| pKeyboard->GetRepeat(DIK_W) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_23) == TRUE || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_31) == TRUE)
	{// ������̓��͂����ꂽ
	    // �I������Ă������ڂ̐ݒ�
		m_pMapSelect[m_nSelectMapIdx]->SetState(CScene2DFlash::STATE_NONE);   // ���
		m_pMapSelect[m_nSelectMapIdx]->SetCol(MAPSELECT_COL_NONE);          // �F
		m_pMapSelect[m_nSelectMapIdx]->SetWidth(MAPSELECT_WIDTH_NONE);      // ��
		m_pMapSelect[m_nSelectMapIdx]->SetHeight(MAPSELECT_HEIGHT_NONE);    // ����

		// ���ɑI������鍀�ڂ̐ݒ�
		m_pMapSelect[(m_nSelectMapIdx + (MAPSELECT_MAX - 1)) % MAPSELECT_MAX]->SetState(CScene2DFlash::STATE_SELECT);  // ���
		m_pMapSelect[(m_nSelectMapIdx + (MAPSELECT_MAX - 1)) % MAPSELECT_MAX]->SetCol(MAPSELECT_COL_INI);            // �F
		m_pMapSelect[(m_nSelectMapIdx + (MAPSELECT_MAX - 1)) % MAPSELECT_MAX]->SetWidth(MAPSELECT_WIDTH_INI);        // ��
		m_pMapSelect[(m_nSelectMapIdx + (MAPSELECT_MAX - 1)) % MAPSELECT_MAX]->SetHeight(MAPSELECT_HEIGHT_INI);      // ����

		// �ԍ���ݒ�
		m_nSelectMapIdx = (m_nSelectMapIdx + (MAPSELECT_MAX - 1)) % MAPSELECT_MAX;

		// ����炷
		//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	}
	else if (pKeyboard->GetTrigger(DIK_S) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_22) == TRUE || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_30) == TRUE
		|| pKeyboard->GetRepeat(DIK_S) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_22) == TRUE || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_30) == TRUE)
	{// �������̓��͂����ꂽ
	    // �I������Ă������ڂ̐ݒ�
		m_pMapSelect[m_nSelectMapIdx]->SetState(CScene2DFlash::STATE_NONE);   // ���
		m_pMapSelect[m_nSelectMapIdx]->SetCol(MAPSELECT_COL_NONE);          // �F
		m_pMapSelect[m_nSelectMapIdx]->SetWidth(MAPSELECT_WIDTH_NONE);      // ��
		m_pMapSelect[m_nSelectMapIdx]->SetHeight(MAPSELECT_HEIGHT_NONE);    // ����

		// ���ɑI������鍀�ڂ̐ݒ�
		m_pMapSelect[(m_nSelectMapIdx + 1) % MAPSELECT_MAX]->SetState(CScene2DFlash::STATE_SELECT);  // ���
		m_pMapSelect[(m_nSelectMapIdx + 1) % MAPSELECT_MAX]->SetCol(MAPSELECT_COL_INI);            // �F
		m_pMapSelect[(m_nSelectMapIdx + 1) % MAPSELECT_MAX]->SetWidth(MAPSELECT_WIDTH_INI);        // ��
		m_pMapSelect[(m_nSelectMapIdx + 1) % MAPSELECT_MAX]->SetHeight(MAPSELECT_HEIGHT_INI);      // ����

																							 // �ԍ���ݒ�
		m_nSelectMapIdx = (m_nSelectMapIdx + 1) % MAPSELECT_MAX;

		// ����炷
		//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_3) == TRUE)
	{// ����{�^���������ꂽ
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			CGame::SetGameMapIdx(m_nSelectMapIdx);
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
		}
	}
}