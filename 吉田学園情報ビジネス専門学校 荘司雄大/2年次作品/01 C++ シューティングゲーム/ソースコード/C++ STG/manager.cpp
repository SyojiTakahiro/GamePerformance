//=============================================================================
//
// �Ǘ� [manager.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "bullet.h"
#include "bulletSP.h"
#include "bulletEX.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "bar.h"
#include "bg.h"
#include "bgTutorial.h"
#include "bgTitle.h"
#include "bgClear.h"
#include "bgGameover.h"
#include "number.h"
#include "score.h"
#include "gauge.h"
#include "gauge2.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "clear.h"
#include "gameover.h"
#include "fade.h"
#include "sound.h"
#include "pause.h"


//�ÓI�����o�ϐ�
CRenderer *CManager::m_pRenderer = NULL;
CInputKeyboard *CManager::m_pInputKeyboard = NULL;
CInputJoypad *CManager::m_pInputJoypad = NULL;
CSound *CManager::m_pSound = NULL;

CTitle *CManager::m_pTitle = NULL;
CGame *CManager::m_pGame = NULL;
CTutorial *CManager::m_pTutorial = NULL;
CResult *CManager::m_pResult = NULL;
CClear *CManager::m_pClear = NULL;
CGameover *CManager::m_pGameover = NULL;
CFade *CManager::m_pFade = NULL;

//�J�n���郂�[�h
CManager::MODE CManager::m_mode = MODE_TITLE;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CManager::CManager()
{
	
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	if (m_pRenderer == NULL)
	{
		//�����_�����O�N���X�̐���
		m_pRenderer = new CRenderer;
		m_pRenderer->Init(hWnd, FALSE);	//FALSE	�t���X�N

		//�e�N�X�`���̓ǂݍ��݁E����
		CBar::Load();
		CBgTutorial::Load();
		CBg::Load();
		CBgTitle::Load();
		CBgClear::Load();
		CBgGameover::Load();
		CEffect::Load();
		CParticle::Load();
		CPlayer::Load();
		CEnemy::Load();
		CBoss::Load();
		CBullet::Load();
		CBulletSP::Load();
		CBulletEX::Load();
		CExplosion::Load();
		CNumber::Load();
		CGauge::Load();
		CGauge2::Load();
		CPause::Load();

		//2D�|���S���N���X�̐���
		//�I�u�W�F�N�g�N���X�̐���
	}

	if (m_pInputKeyboard == NULL)
	{
		//�L�[�{�[�h�̐���
		m_pInputKeyboard = new CInputKeyboard;
		m_pInputKeyboard->Init(hInstance, hWnd);
	}

	if (m_pInputJoypad == NULL)
	{
		//�W���C�p�b�h�̐���
		m_pInputJoypad = new CInputJoypad;
		if (m_pInputJoypad != NULL)
		{
			m_pInputJoypad->Init(hInstance, hWnd);
		}
	}

	if (m_pSound == NULL)
	{
		//�L�[�{�[�h�̐���
		m_pSound = new CSound;
		m_pSound->Init(hWnd);
	}

	m_pFade = CFade::Create();
	m_pFade->SetFade(m_mode);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CManager::Uninit(void)
{
	CScene::ReleaseAll();

	//�e�N�X�`���̓ǂݍ��݁E����
	CBar::Unload();
	CBgTutorial::Unload();
	CBg::Unload();
	CBgTitle::Unload();
	CBgClear::Unload();
	CBgGameover::Unload();
	CEffect::Unload();
	CParticle::Unload();
	CPlayer::Unload();
	CEnemy::Unload();
	CBoss::Unload();
	CBullet::Unload();
	CBulletSP::Unload();
	CBulletEX::Unload();
	CExplosion::Unload();
	CNumber::Unload();
	CGauge::Unload();
	CGauge2::Unload();
	CPause::Unload();

	//�L�[�{�[�h�̔j��
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();

		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//�W���C�p�b�h�̔j��
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();

		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	//�T�E���h�̔j��
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = NULL;
	}

	//�����_�����O�N���X�̔j��
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//�t�F�[�h�N���X�̔j��
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();

		delete m_pFade;
		m_pFade = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void CManager::Update(void)
{
	//�L�[�{�[�h�̍X�V
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	//�W���C�p�b�h�̍X�V
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();
	}

	//�e���[�h�̍X�V
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Update();
		break;
	case MODE_TUTORIAL:
		m_pTutorial->Update();
		break;
	case MODE_GAME:
		m_pGame->Update();
		break;
	case MODE_GAMECLEAR:
		m_pClear->Update();
		break;
	case MODE_GAMEOVER:
		m_pGameover->Update();
		break;
	}

	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}

	if (m_pFade != NULL)
	{
		m_pFade->Update();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
	//�e���[�h�̕`��
	switch (m_mode)
	{
	case MODE_TITLE:
		m_pTitle->Draw();
		break;
	case MODE_TUTORIAL:
		m_pTutorial->Draw();
		break;
	case MODE_GAME:
		m_pGame->Draw();
		break;
	case MODE_GAMECLEAR:
		m_pClear->Draw();
		break;
	case MODE_GAMEOVER:
		m_pGameover->Draw();
		break;
	}
}

//=============================================================================
// �擾
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

CFade *CManager::GetFade(void)
{
	return m_pFade;
}

CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

CInputJoypad *CManager::GetInputJoypad(void)
{
	return m_pInputJoypad;
}

CSound *CManager::GetSound(void)
{
	return m_pSound;
}

CManager::MODE CManager::GetMode(void)
{
	return m_mode;
}

CClear *CManager::GetClear(void)
{
	return m_pClear;
}

//=============================================================================
// �ݒ�
//=============================================================================
void CManager::SetMode(MODE mode)
{
	switch (m_mode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle->Uninit();
			delete m_pTitle;
			m_pTitle = NULL;
		}
		break;
	case MODE_TUTORIAL:
		if (m_pTutorial != NULL)
		{
			m_pTutorial->Uninit();
			delete m_pTutorial;
			m_pTutorial = NULL;
		}
		break;
	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame->Uninit();
			delete m_pGame;
			m_pGame = NULL;
		}
		break;
	case MODE_GAMECLEAR:
		if (m_pClear != NULL)
		{
			m_pClear->Uninit();
			delete m_pClear;
			m_pClear = NULL;
		}
		break;
	case MODE_GAMEOVER:
		if (m_pGameover != NULL)
		{
			m_pGameover->Uninit();
			delete m_pGameover;
			m_pGameover = NULL;
		}
		break;
	}
	switch (mode)
	{
	case MODE_TITLE:
		m_pTitle = CTitle::Create();
		break;
	case MODE_TUTORIAL:
		m_pTutorial = CTutorial::Create();
		break;
	case MODE_GAME:
		m_pGame = CGame::Create();
		break;
	case MODE_GAMECLEAR:
		m_pClear = CClear::Create();
		break;
	case MODE_GAMEOVER:
		m_pGameover = CGameover::Create();
		break;

	}
	m_mode = mode;
}
