//=============================================================================
//
// タイトル処理 [title.cpp]
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
#include "title.h"
#include "fade.h"
#include "bgTitle.h"
#include "pause.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//静的メンバ変数
CRenderer *m_pTitleRenderer = NULL;
LPDIRECT3DTEXTURE9 CTitle::m_pTexture = NULL;

CPause *CTitle::m_pSelect = NULL;
//=============================================================================
// コンストラクタ
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CTitle::~CTitle()
{
	
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	CBgTitle::Create();

	CManager manager;

	//サウンドの取得
	CSound *pSound;
	pSound = manager.GetSound();

	pSound->PlaySound(CSound::SOUND_LABEL_BGM000);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CTitle::Uninit(void)
{
	CScene::ReleaseAll();
}
//=============================================================================
// 更新処理
//=============================================================================
void CTitle::Update(void)
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
		pFade->SetFade(CManager::MODE_TUTORIAL);
		pSound->StopSound(CSound::SOUND_LABEL_BGM000);
		pSound->PlaySound(CSound::SOUND_LABEL_BGM001);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
	}

	if (pInputJoypad != NULL)
	{
		if (pInputJoypad->GetTrigger(3) == true||
			pInputJoypad->GetTrigger(11) == true)
		{
			pFade->SetFade(CManager::MODE_TUTORIAL);
			pSound->StopSound(CSound::SOUND_LABEL_BGM000);
			pSound->PlaySound(CSound::SOUND_LABEL_BGM001);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CTitle::Draw(void)
{
	
}

//=============================================================================
// クリエイト
//=============================================================================
CTitle *CTitle::Create(void)
{
	CTitle *pTitle = NULL;
	if (pTitle == NULL)
	{
		// 動的確保
		pTitle = new CTitle;

		if (pTitle != NULL)
		{
			// 初期化処理
			pTitle->Init();
		}
	}
	return pTitle;
}