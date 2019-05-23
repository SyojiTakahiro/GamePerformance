//=============================================================================
//
// ライフゲージ処理 [gauge.cpp]
// Author : Takahiro Syoji
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "bg.h"
#include "explosion.h"
#include "player.h"
#include "gauge.h"
#include "gauge2.h"
#include "fade.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//静的メンバ変数
CRenderer *m_pGaugeRenderer = NULL;
LPDIRECT3DTEXTURE9 CGauge::m_apTexture[MAX_GAUGE] = {};

float CGauge::m_fGauge = 0;
//=============================================================================
// コンストラクタ
//=============================================================================
CGauge::CGauge() : CScene2D(7)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CGauge::~CGauge()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CGauge::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, GAUGE_TEXTURE0, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, GAUGE_TEXTURE1, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, GAUGE_TEXTURE2, &m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CGauge::Unload(void)
{
	for (int nCntScene = 0; nCntScene < MAX_GAUGE; nCntScene++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntScene] != NULL)
		{
			m_apTexture[nCntScene]->Release();
			m_apTexture[nCntScene] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CGauge::Init(D3DXVECTOR3 pos)
{
	SetObjType(OBJTYPE_GAUGE);

	for (int nCntScene = 0; nCntScene < MAX_GAUGE; nCntScene++)
	{
		m_apCScene2D[nCntScene] = new CScene2D(7);
		m_apCScene2D[nCntScene]->Init();
		m_apCScene2D[nCntScene]->BindTexture(m_apTexture[nCntScene]);
	}

	m_fGauge = 400.0f;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGauge::Uninit(void)
{
	for (int nCntScene = 0; nCntScene < MAX_GAUGE; nCntScene++)
	{
		if (m_apCScene2D[nCntScene] != NULL)
		{
			m_apCScene2D[nCntScene]->Uninit();
			m_apCScene2D[nCntScene] = NULL;
		}
	}
	//リリース
	CScene::Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CGauge::Update(void)
{
	CManager manager;

	//キーボードの取得
	CInputKeyboard *pInput;
	pInput = manager.GetInputKeyboard();

	//サウンドの取得
	CSound *pSound;
	pSound = manager.GetSound();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetInputJoypad();

	//頂点座標の設定･更新
	m_nCntAnim++;


	if (m_fGauge > 400.0f)
	{
		m_fGauge = 400.0f;
	}

	for (int nCntScene = 0; nCntScene < MAX_GAUGE; nCntScene++)
	{
		VERTEX_2D *pVtx;
		LPDIRECT3DVERTEXBUFFER9 Vtx;
		Vtx = m_apCScene2D[nCntScene]->GetVtx();

		Vtx->Lock(0, 0, (void**)&pVtx, 0);

		if (nCntScene == 0)
		{
			pVtx[0].pos = D3DXVECTOR3(80.0f -50, 650.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(520.0f - 50, 650.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(80.0f - 50, 690.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(520.0f - 50, 690.0f, 0.0f);
		}
		else if (nCntScene == 1)
		{
			pVtx[0].pos = D3DXVECTOR3(120.0f - 50, 650.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(m_fGauge+120 - 50, 650.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(120.0f - 50, 690.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(m_fGauge+120 - 50, 690.0f, 0.0f);
		}
		else if (nCntScene == 2)
		{
			pVtx[0].pos = D3DXVECTOR3(100.0f - 50, 640.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(530.0f - 50, 640.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(100.0f - 50, 691.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(530.0f - 50, 691.0f, 0.0f);
		}

		//アンロック
		Vtx->Unlock();
	}

	//ヒール
	if (m_fGauge < 400 && CGauge2::m_fGauge2 > 200)
	{
		if (pInput->GetKeyboardTrigger(DIK_X) == true)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_HEAL);
			m_fGauge += 200.0f;
			CGauge2::m_fGauge2 -= 200.0f;
		}
		if (pInputJoypad != NULL)
		{
			if (pInputJoypad->GetTrigger(8) == true)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_HEAL);
				m_fGauge += 200.0f;
				CGauge2::m_fGauge2 -= 200.0f;
			}
		}
	}

	if (pInput->GetKeyboardTrigger(DIK_Z) == true)
	{
		m_fGauge -= 100.0f;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CGauge::Draw(void)
{

}

//=============================================================================
// クリエイト
//=============================================================================
CGauge *CGauge::Create(D3DXVECTOR3 pos)
{
	CGauge *pGauge = NULL;
	if (pGauge == NULL)
	{
		// 動的確保
		pGauge = new CGauge;

		if (pGauge != NULL)
		{
			// 初期化処理
			pGauge->Init(pos);
		}
	}
	return pGauge;
}

void CGauge::CutLife(float fDamage)
{
	CManager manager;
	CFade *pFade = CManager::GetFade();

	//サウンドの取得
	CSound *pSound;
	pSound = manager.GetSound();

	 m_fGauge -= fDamage;

	 if (CGauge::m_fGauge <= 0)
	 {
		 pFade->SetFade(CManager::MODE_GAMEOVER);
		 pSound->StopSound(CSound::SOUND_LABEL_BGM002);
		 pSound->PlaySound(CSound::SOUND_LABEL_BGM004);
		 pSound->StopSound(CSound::SOUND_LABEL_SE_SPSHOT);
	 }
}