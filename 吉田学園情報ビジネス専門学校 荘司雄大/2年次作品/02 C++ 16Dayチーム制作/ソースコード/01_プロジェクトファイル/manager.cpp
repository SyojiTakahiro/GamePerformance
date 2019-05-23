//*****************************************************************************
//
//     �}�l�[�W���[�̏���[manager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "debuglog.h"
#include "scene.h"

#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#ifdef _DEBUG
#define MANAGER_DEBUGKEY_DEBUGPROC_CHANGE (DIK_F1)   // �f�o�b�O�\�����邩���Ȃ�����؂�ւ���L�[
#define MANAGER_DEBUGKEY_LIGHTING_CHANGE  (DIK_F2)   // ���C�e�B���OON/OFF��؂�ւ���L�[
#define MANAGER_DEBUGKEY_WIREFRAME_CHANGE (DIK_F3)   // ���C���[�t���[��ON/OFF��؂�ւ���L�[
#define MANAGER_DEBUGKEY_UPDATE_STOP      (DIK_F4)   // �A�b�v�f�[�g���~����L�[
#define MANAGER_DEBUGKEY_MODECHANGE       (DIK_F7)   // ��ʑJ�ڂ�����L�[
#define MANAGER_DEBUGKEY_RELEASEALL       (DIK_F11)  // �S�ẴI�u�W�F�N�g��j������L�[
#endif

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
CRenderer         *CManager::m_pRenderer = NULL;     // �����_�����O�N���X�^�̃|�C���^
CCamera           *CManager::m_pCamera = NULL;       // �J�����N���X�^�̃|�C���^
CLight            *CManager::m_pLight = NULL;        // ���C�g�N���X�^�̃|�C���^
CInputKeyboard    *CManager::m_pKeyboard = NULL;     // �L�[�{�[�h�N���X�^�̃|�C���^
CInputMouse       *CManager::m_pMouse = NULL;        // �}�E�X�N���X�^�̃|�C���^
CInputJoyStick    *CManager::m_pJoyStick = NULL;     // �W���C�X�e�B�b�N�N���X�^�̃|�C���^
CXInput           *CManager::m_pXInput = NULL;       // XInput�N���X�^�̃|�C���^
CSound            *CManager::m_pSound = NULL;        // �T�E���h�N���X�^�̃|�C���^
CDebugProc        *CManager::m_pDebugProc = NULL;    // �f�o�b�O���N���X�^�̃|�C���^

CFade             *CManager::m_pFade = NULL;          // �t�F�[�h�N���X�^�̃|�C���^
CTitle            *CManager::m_pTitle = NULL;         // �^�C�g���N���X�^�̃|�C���^
CTutorial         *CManager::m_pTutorial = NULL;      // �`���[�g���A���N���X�^�̃|�C���^
CGame             *CManager::m_pGame = NULL;          // �Q�[���N���X�^�̃|�C���^
CResult           *CManager::m_pResult = NULL;        // ���U���g�N���X�^�̃|�C���^
CRanking          *CManager::m_pRanking = NULL;       // �����L���O�N���X�^�̃|�C���^
CManager::MODE    CManager::m_Mode = MODE_TITLE;      // ���݂̃��[�h

#ifdef _DEBUG
CManager::MODE g_Mode = CManager::MODE::MODE_GAME;     // ���݂̃��[�h(�f�o�b�O�N����)
bool g_bUpdate = true;                                 // �A�b�v�f�[�g���邩���Ȃ���
bool g_bLighting = true;                               // ���C�e�B���OON/OFF��؂�ւ���t���O
bool g_bWireFrame = false;                             // ���C���[�t���[���ŕ`�悷�邩���Ȃ���
#endif

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// �����_�����O�̍쐬
	if (m_pRenderer == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pRenderer = new CRenderer;
		if (m_pRenderer != NULL)
		{// ���������m�ۂł���
			if (FAILED(m_pRenderer->Init(hWnd, bWindow)))  // FALSE�ɂ���ƃt���X�N���[��
			{// �������Ɏ��s����
				return E_FAIL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	// �J�����̍쐬
	if (m_pCamera == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pCamera = new CCamera;
		if (m_pCamera != NULL)
		{// ���������m�ۂł���
			if (FAILED(m_pCamera->Init()))
			{// �������Ɏ��s����
				return E_FAIL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	// ���C�g�̍쐬
	if (m_pLight == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pLight = new CLight;
		if (m_pLight != NULL)
		{// ���������m�ۂł���
			if (FAILED(m_pLight->Init()))
			{// �������Ɏ��s����
				return E_FAIL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	// �L�[�{�[�h�̍쐬
	if (m_pKeyboard == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pKeyboard = new CInputKeyboard;
		if (m_pKeyboard != NULL)
		{// ���������m�ۂł���
			if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
			{// �������Ɏ��s����
				return E_FAIL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	// �}�E�X�̍쐬
	if (m_pMouse == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pMouse = new CInputMouse;
		if (m_pMouse != NULL)
		{// ���������m�ۂł���
			m_pMouse->Init(hInstance, hWnd);
		}
	}

	// �W���C�X�e�B�b�N�̍쐬
	if (m_pJoyStick == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pJoyStick = new CInputJoyStick(1);
		if (m_pJoyStick != NULL)
		{// ���������m�ۂł���
			m_pJoyStick->Init(hInstance, hWnd);
		}
	}

	// XInput�̍쐬
	if (m_pXInput == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pXInput = new CXInput;
		if (m_pXInput != NULL)
		{// ���������m�ۂł���
			m_pXInput->Init();
		}
	}

	// �T�E���h�̍쐬
	if (m_pSound == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pSound = new CSound;
		if (m_pSound != NULL)
		{// ���������m�ۂł���
			if (FAILED(m_pSound->Init(hWnd)))
			{// �������Ɏ��s����
				return E_FAIL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł��Ȃ�����
		return E_FAIL;
	}

	// �f�o�b�O���N���X�̍쐬
	if (m_pDebugProc == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pDebugProc = new CDebugProc;
		if (m_pDebugProc != NULL)
		{// ���������m�ۂł���
			m_pDebugProc->Init();
		}
	}

#ifdef _DEBUG
	m_Mode = g_Mode;
#endif

	// ���[�h�̐ݒ�
	SetMode(m_Mode);

	// �e�N�X�`���̓ǂݍ���
	CFade::Load();

	// �t�F�[�h�̍쐬
	if (m_pFade == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pFade = new CFade;
		if (m_pFade != NULL)
		{// ���������m�ۂł���
			if (FAILED(m_pFade->Init()))
			{// �������Ɏ��s����
				return E_FAIL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł��Ȃ�����
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CManager::Uninit(void)
{
	// �e�N�X�`���̊J��
	CFade::UnLoad();

	// �t�F�[�h�̔j��
	if (m_pFade != NULL)
	{// ���������m�ۂ���Ă���
	    // �t�F�[�h�̏I������
		m_pFade->Uninit();

		// �t�F�[�h�̊J��
		delete m_pFade;
		m_pFade = NULL;
	}

	// �f�o�b�O���̔j��
	if (m_pDebugProc != NULL)
	{// ���������m�ۂ���Ă���
	     // �f�o�b�O���̏I������
		m_pDebugProc->Uninit();

		// �������̊J��
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	// �T�E���h�̔j��
	if (m_pSound != NULL)
	{// ���������m�ۂ���Ă���
	    // �T�E���h�̏I������
		m_pSound->Uninit();

		// �������̊J��
		delete m_pSound;
		m_pSound = NULL;
	}

	// �L�[�{�[�h�̔j��
	if (m_pKeyboard != NULL)
	{// ���������m�ۂ���Ă���
	    // �L�[�{�[�h�̏I������
		m_pKeyboard->Uninit();

		// �������̊J��
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}

	// �}�E�X�̔j��
	if (m_pMouse != NULL)
	{// ���������m�ۂ���Ă���
	    // �}�E�X�̏I������
		m_pMouse->Uninit();

		// �������̊J��
		delete m_pMouse;
		m_pMouse = NULL;
	}

	// �W���C�X�e�B�b�N�̔j��
	if (m_pJoyStick != NULL)
	{// ���������m�ۂ���Ă���
	    // �W���C�X�e�B�b�N�̏I������
		m_pJoyStick->Uninit();

		// �������̊J��
		delete m_pJoyStick;
		m_pJoyStick = NULL;
	}

	// XInput�̔j��
	if (m_pXInput != NULL)
	{// ���������m�ۂ���Ă���
		// XInput�̏I������
		m_pXInput->Uninit();

		// �������̊J��
		delete m_pXInput;
		m_pXInput = NULL;
	}

	// ���C�g�̔j��
	if (m_pLight != NULL)
	{// ���������m�ۂ���Ă���
	    // ���C�g�̏I������
		m_pLight->Uninit();

		// �������̊J��
		delete m_pLight;
		m_pLight = NULL;
	}

	// �J�����̔j��
	if (m_pCamera != NULL)
	{// ���������m�ۂ���Ă���
	    // �J�����̏I������
		m_pCamera->Uninit();

		// �������̊J��
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// �����_�����O�̔j��
	if (m_pRenderer != NULL)
	{// ���������m�ۂ���Ă���
		// �����_�����O�̏I������
		m_pRenderer->Uninit();

		// �������̊J��
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// �V�[���̃C���X�^���X�����ׂĔj������
	CScene::ReleaseAll();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CManager::Update(void)
{
	// ������@��`��
#ifdef _DEBUG  // �f�o�b�O���̏���
	CDebugProc::Print("( ������@�ꗗ )\n");
	CDebugProc::Print("�f�o�b�O�\���ؑ�           : [ F1 ]\n");
	CDebugProc::Print("���C�e�B���O(ON/OFF)       : [ F2 ]\n");
	CDebugProc::Print("���C���[�t���[��(ON/OFF)   : [ F3 ]\n");
	CDebugProc::Print("�X�V���~����/�J�n����    : [ F4 ]\n");
	CDebugProc::Print("��ʑJ��                   : [ F7 ]\n");
	CDebugProc::Print("���ׂẴI�u�W�F�N�g���폜 : [ F11 ]\n");

	if (m_pKeyboard != NULL)
	{// �L�[�{�[�h����������Ă���
		if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_DEBUGPROC_CHANGE) == true)
		{// �L�[�{�[�h��F1�L�[�������ꂽ
			CDebugProc::SetDisp(CDebugProc::GetDisp() ? false : true);
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_LIGHTING_CHANGE) == true)
		{// �L�[�{�[�h��F2�L�[�������ꂽ
			g_bLighting = g_bLighting ? false : true;
			m_pRenderer->GetDevice()->SetRenderState(D3DRS_LIGHTING, g_bLighting);
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_WIREFRAME_CHANGE) == true)
		{// �L�[�{�[�h��F3�L�[�������ꂽ
			g_bWireFrame = g_bWireFrame ? false : true;
			if (g_bWireFrame == true)
			{
				m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			else
			{
				m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_UPDATE_STOP) == true)
		{// �L�[�{�[�h��F4�L�[�������ꂽ
			g_bUpdate = g_bUpdate ? false : true;
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_MODECHANGE) == true)
		{// �L�[�{�[�h��F7�L�[�������ꂽ
			if (m_pFade->GetState() == CFade::STATE_NONE)
			{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
				switch (m_Mode)
				{// ���݂̃��[�h�ɂ���ď����킯
				case MODE_TITLE:   // �^�C�g�����
					m_pFade->SetFade(MODE_TUTORIAL);
					break;
				case MODE_TUTORIAL: // �`���[�g���A�����
					m_pFade->SetFade(MODE_GAME);
					break;
				case MODE_GAME:    // �Q�[�����
					m_pFade->SetFade(MODE_RESULT);
					break;
				case MODE_RESULT:  // ���U���g���
					m_pFade->SetFade(MODE_RANKING);
					break;
				case MODE_RANKING: // �����L���O���
					m_pFade->SetFade(MODE_TITLE);
					break;
				}
			}
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_RELEASEALL) == true)
		{// �L�[�{�[�h��F11�L�[�������ꂽ
			CScene::ReleaseAll();
		}
	}

	if (m_pKeyboard != NULL)
	{// ���������m�ۂ���Ă���
	    // �L�[�{�[�h�̍X�V����
		m_pKeyboard->Update();
	}

	if (m_pMouse != NULL)
	{// ���������m�ۂ���Ă���
	    // �}�E�X�̍X�V����
		m_pMouse->Update();
	}

	if (m_pCamera != NULL)
	{// ���������m�ۂ���Ă���
	    // �J�����̍X�V����
		m_pCamera->Update();
	}

	if (g_bUpdate == true)
	{// �X�V�����ԂȂ��
		if (m_pJoyStick != NULL)
		{// ���������m�ۂ���Ă���
		    // �W���C�X�e�B�b�N�̍X�V����
			m_pJoyStick->Update();
		}

		if (m_pXInput != NULL)
		{// ���������m�ۂ���Ă���
		    // XInput�̍X�V����
			m_pXInput->Update();
		}

		if (m_pFade != NULL)
		{// ���������m�ۂ���Ă���
		    // �t�F�[�h�̍X�V����
			m_pFade->Update();
		}

		if (m_pLight != NULL)
		{// ���������m�ۂ���Ă���
		    // ���C�g�̍X�V����
			m_pLight->Update();
		}

		if (m_pRenderer != NULL)
		{// ���������m�ۂ���Ă���
		    // �����_�����O�̍X�V����
			m_pRenderer->Update();
		}
	}

#else // �����[�X���̏���
	if (m_pKeyboard != NULL)
	{// ���������m�ۂ���Ă���
	    // �L�[�{�[�h�̍X�V����
		m_pKeyboard->Update();
	}

	if (m_pMouse != NULL)
	{// ���������m�ۂ���Ă���
	    // �}�E�X�̍X�V����
		m_pMouse->Update();
	}

	if (m_pJoyStick != NULL)
	{// ���������m�ۂ���Ă���
	    // �W���C�X�e�B�b�N�̍X�V����
		m_pJoyStick->Update();
	}

	if (m_pXInput != NULL)
	{// ���������m�ۂ���Ă���
	    // XInput�̍X�V����
		m_pXInput->Update();
	}

	if (m_pFade != NULL)
	{// ���������m�ۂ���Ă���
	    // �t�F�[�h�̍X�V����
		m_pFade->Update();
	}

	if (m_pLight != NULL)
	{// ���������m�ۂ���Ă���
	    // ���C�g�̍X�V����
		m_pLight->Update();
	}

	if (m_pCamera != NULL)
	{// ���������m�ۂ���Ă���
	    // �J�����̍X�V����
		m_pCamera->Update();
	}

	if (m_pRenderer != NULL)
	{// ���������m�ۂ���Ă���
	    // �����_�����O�̍X�V����
		m_pRenderer->Update();
	}
#endif
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{// ���������m�ۂ���Ă���
		m_pRenderer->Draw();
	}
}

//=============================================================================
//    ���̃��[�h��ݒ肷��
//=============================================================================
void CManager::SetMode(MODE mode)
{
	switch (m_Mode)
	{// ���݂̃��[�h�ɂ���ď����킯
	case MODE_TITLE:   // �^�C�g�����
		if (m_pTitle != NULL)
		{// ���������m�ۂ���Ă���
		    // �^�C�g���̏I������
			m_pTitle = NULL;
			StopBgm(CSound::SOUND_LABEL_BGM000);
		}
		break;
	case MODE_TUTORIAL: // �`���[�g���A�����
		if (m_pTutorial != NULL)
		{// ���������m�ۂ���Ă���
		    // �`���[�g���A���̏I������
			m_pTutorial = NULL;
			StopBgm(CSound::SOUND_LABEL_BGM001);
		}
		break;
	case MODE_GAME:    // �Q�[�����
		if (m_pGame != NULL)
		{// ���������m�ۂ���Ă���
		    // �|�[�Y�̉���
			m_pRenderer->SetPause(false);

			// �Q�[���̏I������
			m_pGame = NULL;
			StopBgm(CSound::SOUND_LABEL_BGM002);
		}
		break;
	case MODE_RESULT:  // ���U���g���
		if (m_pResult != NULL)
		{// ���������m�ۂ���Ă���
			// ���U���g�̏I������
			m_pResult = NULL;
			StopBgm(CSound::SOUND_LABEL_BGM003);
		}
		break;
	case MODE_RANKING: // �����L���O���
		if (m_pRanking != NULL)
		{// ���������m�ۂ���Ă���
		    // �����L���O�̏I������
			m_pRanking = NULL;
			StopBgm(CSound::SOUND_LABEL_BGM004);
		}
		break;
	}

	// ���ׂẴI�u�W�F�N�g��j��
	CScene::ReleaseAll();

	switch (mode)
	{// ���݂̃��[�h�ɂ���ď����킯
	case MODE_TITLE:   // �^�C�g�����
		if (m_pTitle == NULL)
		{// ���������m�ۂ���Ă���
			m_pTitle = CTitle::Create();
			PlayBgm(CSound::SOUND_LABEL_BGM000);
		}
		break;
	case MODE_TUTORIAL:  // �`���[�g���A�����
		if (m_pTutorial == NULL)
		{// ���������m�ۂ���Ă���
			m_pTutorial = CTutorial::Create();
			PlayBgm(CSound::SOUND_LABEL_BGM001);
		}
		break;
	case MODE_GAME:      // �Q�[�����
		if (m_pGame == NULL)
		{// ���������m�ۂ���Ă���
			m_pGame = CGame::Create();
			PlayBgm(CSound::SOUND_LABEL_BGM002);
		}
		break;
	case MODE_RESULT:    // ���U���g���
		if (m_pResult == NULL)
		{// ���������m�ۂ���Ă���
			m_pResult = CResult::Create();
			PlayBgm(CSound::SOUND_LABEL_BGM003);
		}
		break;
	case MODE_RANKING:   // �����L���O���
		if (m_pRanking == NULL)
		{// ���������m�ۂ���Ă���
			m_pRanking = CRanking::Create();
			PlayBgm(CSound::SOUND_LABEL_BGM004);
		}
		break;
	}

	// ���݂̃��[�h�X�V
	m_Mode = mode;
}

//=============================================================================
//    �����_�����O�N���X�̃|�C���^���擾
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
//    �J�����N���X�̃|�C���^���擾
//=============================================================================
CCamera *CManager::GetCamera(void)
{
	return m_pCamera;
}

//=============================================================================
//    ���C�g�N���X�̃|�C���^���擾
//=============================================================================
CLight *CManager::GetLight(void)
{
	return m_pLight;
}

//=============================================================================
//    �L�[�{�[�h�N���X�̃|�C���^���擾
//=============================================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	return m_pKeyboard;
}

//=============================================================================
//    �}�E�X�N���X�̃|�C���^���擾
//=============================================================================
CInputMouse *CManager::GetMouse(void)
{
	return m_pMouse;
}

//=============================================================================
//    �W���C�X�e�B�b�N�N���X�̃|�C���^���擾
//=============================================================================
CInputJoyStick *CManager::GetJoyStick(void)
{
	return m_pJoyStick;
}

//=============================================================================
//    XInput�N���X�̃|�C���^���擾
//=============================================================================
CXInput *CManager::GetXInput(void)
{
	return m_pXInput;
}

//=============================================================================
//    �T�E���h�N���X�̃|�C���^���擾
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

//=============================================================================
//    �t�F�[�h�N���X�̃|�C���^���擾
//=============================================================================
CFade *CManager::GetFade(void)
{
	return m_pFade;
}

//=============================================================================
//    ���݂̃��[�h���擾����
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_Mode;
}

//=============================================================================
//   �^�C�g���N���X�̃|�C���^���擾
//=============================================================================
CTitle *CManager::GetTitle(void)
{
	return m_pTitle;
}

//=============================================================================
//   �`���[�g���A���N���X�̃|�C���^���擾
//=============================================================================
CTutorial *CManager::GetTutorial(void)
{
	return m_pTutorial;
}

//=============================================================================
//    �Q�[���N���X�̃|�C���^���擾
//=============================================================================
CGame *CManager::GetGame(void)
{
	return m_pGame;
}

//=============================================================================
//    ���U���g�N���X�̃|�C���^���擾
//=============================================================================
CResult *CManager::GetResult(void)
{
	return m_pResult;
}

//=============================================================================
//    �����L���O�N���X�̃|�C���^���擾
//=============================================================================
CRanking *CManager::GetRanking(void)
{
	return m_pRanking;
}

//=============================================================================
//    BGM�̍Đ�
//=============================================================================
void CManager::PlayBgm(CSound::SOUND_LABEL soundLabel)
{
	if (m_pSound != NULL)
	{// �T�E���h�N���X����������Ă���
		m_pSound->PlaySound(soundLabel);
	}
}

//=============================================================================
//    BGM�̒�~
//=============================================================================
void CManager::StopBgm(CSound::SOUND_LABEL soundLabel)
{
	if (m_pSound != NULL)
	{// �T�E���h�N���X����������Ă���
		m_pSound->StopSound(soundLabel);
	}
}

#ifdef _DEBUG  // �f�o�b�O���̏���

//=============================================================================
//    �X�V���Ă��邩�ǂ�����Ԃ�����
//=============================================================================
bool GetUpdate(void)
{
	return g_bUpdate;
}

#endif