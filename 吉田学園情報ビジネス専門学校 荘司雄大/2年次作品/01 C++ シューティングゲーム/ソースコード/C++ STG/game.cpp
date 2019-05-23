//=============================================================================
//
// ゲーム処理 [game.cpp]
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
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "bullet.h"
#include "explosion.h"
#include "effect.h"
#include "bar.h"
#include "bg.h"
#include "number.h"
#include "score.h"
#include "gauge.h"
#include "gauge2.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "fade.h"
#include "clear.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//静的メンバ変数
CRenderer *m_pGameRenderer = NULL;
LPDIRECT3DTEXTURE9 CGame::m_pTexture = NULL;

CPlayer *CGame::m_pPlayer = NULL;
CGauge *CGame::m_pGauge = NULL;
CGauge2 *CGame::m_pGauge2 = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CGame::CGame()
{
	m_pPlayer = NULL;
	m_pGauge = NULL;
	m_pGauge2 = NULL;
	m_pScore = NULL;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGame::~CGame()
{
	
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{
	CBar::Create();
	CBg::Create();
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(300, 350, 0.0f),50);

	CEnemy::Create(D3DXVECTOR3(1500, 150, 0.0f),70,10,CEnemy::ENEMYTYPE_1);
	CEnemy::Create(D3DXVECTOR3(1600, 350, 0.0f),70,10,CEnemy::ENEMYTYPE_1);
	CEnemy::Create(D3DXVECTOR3(1500, 550, 0.0f),70,10,CEnemy::ENEMYTYPE_1);
	CEnemy::Create(D3DXVECTOR3(1700, 250, 0.0f),70, 10, CEnemy::ENEMYTYPE_1);
	CEnemy::Create(D3DXVECTOR3(1700, 450, 0.0f),70, 10, CEnemy::ENEMYTYPE_1);

	CEnemy::Create(D3DXVECTOR3(900, SCREEN_HEIGHT + 1000 - 120, 0.0f), 50, 5, CEnemy::ENEMYTYPE_5);
	CEnemy::Create(D3DXVECTOR3(900, -1000, 0.0f), 50, 5, CEnemy::ENEMYTYPE_6);

	CEnemy::Create(D3DXVECTOR3(2800, 200, 0.0f), 50, 20, CEnemy::ENEMYTYPE_2);
	CEnemy::Create(D3DXVECTOR3(2700, 350, 0.0f), 50, 20, CEnemy::ENEMYTYPE_2);
	CEnemy::Create(D3DXVECTOR3(2800, 500, 0.0f), 50, 20, CEnemy::ENEMYTYPE_2);

	CEnemy::Create(D3DXVECTOR3(900, SCREEN_HEIGHT + 3200 - 120, 0.0f), 50, 5, CEnemy::ENEMYTYPE_5);
	CEnemy::Create(D3DXVECTOR3(900, -3200, 0.0f), 50, 5, CEnemy::ENEMYTYPE_6);

	CEnemy::Create(D3DXVECTOR3(3600, 200, 0.0f), 50, 30, CEnemy::ENEMYTYPE_3);
	CEnemy::Create(D3DXVECTOR3(3700, 350, 0.0f), 50, 30, CEnemy::ENEMYTYPE_3);
	CEnemy::Create(D3DXVECTOR3(3600, 500, 0.0f), 50, 30, CEnemy::ENEMYTYPE_3);

	CEnemy::Create(D3DXVECTOR3(900, SCREEN_HEIGHT + 4200 - 120, 0.0f), 50, 5, CEnemy::ENEMYTYPE_5);
	CEnemy::Create(D3DXVECTOR3(900, -4200, 0.0f), 50, 5, CEnemy::ENEMYTYPE_6);

	CEnemy::Create(D3DXVECTOR3(5100, 200, 0.0f), 50, 30, CEnemy::ENEMYTYPE_3);
	CEnemy::Create(D3DXVECTOR3(5200, 350, 0.0f), 50, 20, CEnemy::ENEMYTYPE_2);
	CEnemy::Create(D3DXVECTOR3(5100, 500, 0.0f), 50, 30, CEnemy::ENEMYTYPE_3);

	CEnemy::Create(D3DXVECTOR3(900, SCREEN_HEIGHT + 5500 - 120, 0.0f), 50, 5, CEnemy::ENEMYTYPE_5);
	CEnemy::Create(D3DXVECTOR3(900, -5500, 0.0f), 50, 5, CEnemy::ENEMYTYPE_6);
	
	CEnemy::Create(D3DXVECTOR3(5600 * 5, 150, 0.0f), 50, 2, CEnemy::ENEMYTYPE_4);
	CEnemy::Create(D3DXVECTOR3(5700 * 5, 250, 0.0f), 50, 2, CEnemy::ENEMYTYPE_4);
	CEnemy::Create(D3DXVECTOR3(5600 * 5, 350, 0.0f), 50, 2, CEnemy::ENEMYTYPE_4);
	CEnemy::Create(D3DXVECTOR3(5700 * 5, 450, 0.0f), 50, 2, CEnemy::ENEMYTYPE_4);
	CEnemy::Create(D3DXVECTOR3(5600 * 5, 550, 0.0f), 50, 2, CEnemy::ENEMYTYPE_4);

	CBoss::Create(D3DXVECTOR3(7700, 350, 0.0f), 400, 400, CBoss::BOSSTYPE_1);

	m_pScore = CScore::Create(D3DXVECTOR3(1000, 660, 0.0f));

	m_pGauge = CGauge::Create(D3DXVECTOR3(100, 700, 0.0f));
	m_pGauge2 = CGauge2::Create(D3DXVECTOR3(100, 1100, 0.0f));

	m_nClearState = 0;
	m_SERecast = 599;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGame::Uninit(void)
{
	
	if (m_pGauge != NULL)
	{
		m_pGauge->Uninit();
		m_pGauge = NULL;
	}

	if (m_pGauge2 != NULL)
	{
		m_pGauge2->Uninit();
		m_pGauge2 = NULL;
	}

	if (m_pScore != NULL)
	{
		m_pScore->Uninit();
		m_pScore = NULL;
	}
	CScene::ReleaseAll();
}
//=============================================================================
// 更新処理
//=============================================================================
void CGame::Update(void)
{
	CManager manager;
	CFade *pFade = CManager::GetFade();

	//キーボードの取得
	CInputKeyboard *pInput;
	pInput = manager.GetInputKeyboard();

	//サウンドの取得
	CSound *pSound;
	pSound = manager.GetSound();

	if (CBoss::m_nLife <= 0)
	{
		m_SERecast++;
		if (m_SERecast >= 600)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_BOSS_DOWN);
			m_SERecast = 0;
		}
		m_nClearState++;
	
	}

	if (m_nClearState >= 180)
	{
		CClear::SetClearScore(m_pScore->GetScore());
		pFade->SetFade(CManager::MODE_GAMECLEAR);
		pSound->StopSound(CSound::SOUND_LABEL_BGM002);
		pSound->PlaySound(CSound::SOUND_LABEL_BGM003);
		pSound->StopSound(CSound::SOUND_LABEL_SE_SPSHOT);
		m_nClearState = 0;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
// クリエイト
//=============================================================================
CGame *CGame::Create(void)
{
	CGame *pGame = NULL;
	if (pGame == NULL)
	{
		// 動的確保
		pGame = new CGame;

		if (pGame != NULL)
		{
			// 初期化処理
			pGame->Init();
		}
	}
	return pGame;
}

CPlayer *CGame::GetPlayer(void)
{
	return m_pPlayer;
}

CGauge *CGame::GetGauge(void)
{
	return m_pGauge;
}

CGauge2 *CGame::GetGauge2(void)
{
	return m_pGauge2;
}