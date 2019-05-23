//=============================================================================
//
// ゲームオーバー処理 [gameover.cpp]
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
#include "fade.h"
#include "gameover.h"
#include "bgGameover.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//静的メンバ変数
CRenderer *m_pGameOverRenderer = NULL;
LPDIRECT3DTEXTURE9 CGameover::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CGameover::CGameover()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CGameover::~CGameover()
{
	m_pTexture = NULL;
}

//=============================================================================
// ロード
//=============================================================================
HRESULT CGameover::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE0, &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CGameover::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGameover::Init(void)
{
	CBgGameover::Create();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGameover::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CGameover::Update(void)
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
		pFade->SetFade(CManager::MODE_TITLE);
		pSound->StopSound(CSound::SOUND_LABEL_BGM004);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
	}

	if (pInputJoypad != NULL)
	{
		if (pInputJoypad->GetTrigger(3) == true ||
			pInputJoypad->GetTrigger(11) == true)
		{
			pFade->SetFade(CManager::MODE_TITLE);
			pSound->StopSound(CSound::SOUND_LABEL_BGM004);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGameover::Draw(void)
{

}

//=============================================================================
// クリエイト
//=============================================================================
CGameover *CGameover::Create(void)
{
	CGameover *pResult = NULL;
	if (pResult == NULL)
	{
		// 動的確保
		pResult = new CGameover;

		if (pResult != NULL)
		{
			// 初期化処理
			pResult->Init();
		}
	}
	return pResult;
}