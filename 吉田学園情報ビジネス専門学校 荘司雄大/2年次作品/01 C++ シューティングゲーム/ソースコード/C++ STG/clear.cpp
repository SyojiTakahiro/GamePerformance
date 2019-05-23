//=============================================================================
//
// クリア処理 [Clear.cpp]
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
#include "clear.h"
#include "fade.h"
#include "bgClear.h"
#include "number.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//静的メンバ変数
CRenderer *m_pClearRenderer = NULL;
LPDIRECT3DTEXTURE9 CClear::m_pTexture = NULL;
int CClear::m_nScore = 0;     

//=============================================================================
// コンストラクタ
//=============================================================================
CClear::CClear()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CClear::~CClear()
{
	m_pTexture = NULL;
}

//=============================================================================
// ロード
//=============================================================================
HRESULT CClear::Load(void)
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
void CClear::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ハイスコアの設定
//=============================================================================
void CClear::SetClearScore(int nScore)
{
	m_nScore = nScore;
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CClear::Init(void)
{
	CBgClear::Create();

	int nNumber;

	D3DXVECTOR3 pos;
	pos = D3DXVECTOR3(SCREEN_WIDTH /2 - 240 / 2,500,0);

	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		m_apNumber[nCntScore] = CNumber::Create(pos);

		// 数字の計算
		nNumber = m_nScore % (int)powf(10.0f, (float)(MAX_NUMBER - nCntScore)) * 10 / (int)powf(10.0f, (float)(MAX_NUMBER - nCntScore));

		m_apNumber[nCntScore]->SetNumber(nNumber);

		pos.x += 30;
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CClear::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();

			delete m_apNumber[nCntScore];
			m_apNumber[nCntScore] = NULL;
		}
	}

	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CClear::Update(void)
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
		pSound->StopSound(CSound::SOUND_LABEL_BGM003);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
	}

	if (pInputJoypad != NULL)
	{
		if (pInputJoypad->GetTrigger(3) == true ||
			pInputJoypad->GetTrigger(11) == true)
		{
			pFade->SetFade(CManager::MODE_TITLE);
			pSound->StopSound(CSound::SOUND_LABEL_BGM003);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CClear::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		m_apNumber[nCntScore]->Draw();
	}
}

//=============================================================================
// クリエイト
//=============================================================================
CClear *CClear::Create(void)
{
	CClear *pResult = NULL;
	if (pResult == NULL)
	{
		// 動的確保
		pResult = new CClear;

		if (pResult != NULL)
		{
			// 初期化処理
			pResult->Init();
		}
	}
	return pResult;
}