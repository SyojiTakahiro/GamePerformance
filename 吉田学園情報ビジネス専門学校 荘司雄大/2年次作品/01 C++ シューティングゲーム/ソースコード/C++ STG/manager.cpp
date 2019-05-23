//=============================================================================
//
// 管理 [manager.cpp]
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


//静的メンバ変数
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

//開始するモード
CManager::MODE CManager::m_mode = MODE_TITLE;
//=============================================================================
// コンストラクタ
//=============================================================================
CManager::CManager()
{
	
}

//=============================================================================
// デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	if (m_pRenderer == NULL)
	{
		//レンダリングクラスの生成
		m_pRenderer = new CRenderer;
		m_pRenderer->Init(hWnd, FALSE);	//FALSE	フルスク

		//テクスチャの読み込み・生成
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

		//2Dポリゴンクラスの生成
		//オブジェクトクラスの生成
	}

	if (m_pInputKeyboard == NULL)
	{
		//キーボードの生成
		m_pInputKeyboard = new CInputKeyboard;
		m_pInputKeyboard->Init(hInstance, hWnd);
	}

	if (m_pInputJoypad == NULL)
	{
		//ジョイパッドの生成
		m_pInputJoypad = new CInputJoypad;
		if (m_pInputJoypad != NULL)
		{
			m_pInputJoypad->Init(hInstance, hWnd);
		}
	}

	if (m_pSound == NULL)
	{
		//キーボードの生成
		m_pSound = new CSound;
		m_pSound->Init(hWnd);
	}

	m_pFade = CFade::Create();
	m_pFade->SetFade(m_mode);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CManager::Uninit(void)
{
	CScene::ReleaseAll();

	//テクスチャの読み込み・生成
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

	//キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();

		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//ジョイパッドの破棄
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();

		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}

	//サウンドの破棄
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = NULL;
	}

	//レンダリングクラスの破棄
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	//フェードクラスの破棄
	if (m_pFade != NULL)
	{
		m_pFade->Uninit();

		delete m_pFade;
		m_pFade = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CManager::Update(void)
{
	//キーボードの更新
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}

	//ジョイパッドの更新
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();
	}

	//各モードの更新
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
// 描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
	//各モードの描画
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
// 取得
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
// 設定
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
