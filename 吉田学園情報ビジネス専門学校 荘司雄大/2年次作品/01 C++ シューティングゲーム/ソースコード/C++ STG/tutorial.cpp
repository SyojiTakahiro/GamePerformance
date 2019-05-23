//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "tutorial.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "bgTutorial.h"
#include "number.h"
#include "score.h"
#include "gauge.h"
#include "gauge2.h"
#include "title.h"
#include "result.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//静的メンバ変数
CRenderer *m_pTutorialRenderer = NULL;
LPDIRECT3DTEXTURE9 CTutorial::m_pTexture = NULL;

CPlayer *CTutorial::m_pPlayer = NULL;
CGauge *CTutorial::m_pGauge = NULL;
CGauge2 *CTutorial::m_pGauge2 = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTutorial::Init(void)
{

	CBgTutorial::Create();
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(300, 350, 0.0f), 50);

	CEnemy::Create(D3DXVECTOR3(1000, 350, 0.0f), 100, 999,CEnemy::ENEMYTYPE_TUTORIAL);

	CScore::Create(D3DXVECTOR3(1000, 660, 0.0f));

	m_pGauge = CGauge::Create(D3DXVECTOR3(100, 700, 0.0f));
	m_pGauge2 = CGauge2::Create(D3DXVECTOR3(100, 1100, 0.0f));
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTutorial::Uninit(void)
{
	CScene::ReleaseAll();
}
//=============================================================================
// 更新処理
//=============================================================================
void CTutorial::Update(void)
{
	CManager manager;
	CFade *pFade = CManager::GetFade();

	//サウンドの取得
	CSound *pSound;
	pSound = manager.GetSound();

	//キーボードの取得
	CInputKeyboard *pInput;
	pInput = manager.GetInputKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetInputJoypad();

	if (pInput->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		pFade->SetFade(CManager::MODE_GAME);
		pSound->StopSound(CSound::SOUND_LABEL_BGM001);
		pSound->PlaySound(CSound::SOUND_LABEL_BGM002);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
	}

	if (pInputJoypad != NULL)
	{
		if (pInputJoypad->GetTrigger(11) == true)
		{
			pFade->SetFade(CManager::MODE_GAME);
			pSound->StopSound(CSound::SOUND_LABEL_BGM001);
			pSound->PlaySound(CSound::SOUND_LABEL_BGM002);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTutorial::Draw(void)
{

}

//=============================================================================
// クリエイト
//=============================================================================
CTutorial *CTutorial::Create(void)
{
	CTutorial *pTutorial = NULL;
	if (pTutorial == NULL)
	{
		// 動的確保
		pTutorial = new CTutorial;

		if (pTutorial != NULL)
		{
			// 初期化処理
			pTutorial->Init();
		}
	}
	return pTutorial;
}

CPlayer *CTutorial::GetPlayer(void)
{
	return m_pPlayer;
}

CGauge *CTutorial::GetGauge(void)
{
	return m_pGauge;
}

CGauge2 *CTutorial::GetGauge2(void)
{
	return m_pGauge2;
}