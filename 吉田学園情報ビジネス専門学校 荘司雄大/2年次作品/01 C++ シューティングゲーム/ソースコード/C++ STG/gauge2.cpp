//=============================================================================
//
// スキルゲージ処理 [gauge2.cpp]
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
#include "player.h"
#include "gauge2.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//静的メンバ変数
CRenderer *m_pGauge2Renderer = NULL;
LPDIRECT3DTEXTURE9 CGauge2::m_apTexture[MAX_GAUGE] = {};

float CGauge2::m_fGauge2 = 0;
//=============================================================================
// コンストラクタ
//=============================================================================
CGauge2::CGauge2() : CScene2D(7)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CGauge2::~CGauge2()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CGauge2::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, GAUGE2_TEXTURE0, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, GAUGE2_TEXTURE1, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, GAUGE2_TEXTURE2, &m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CGauge2::Unload(void)
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
HRESULT CGauge2::Init(D3DXVECTOR3 pos)
{
	SetObjType(OBJTYPE_GAUGE2);

	for (int nCntScene = 0; nCntScene < MAX_GAUGE; nCntScene++)
	{
		m_apCScene2D[nCntScene] = new CScene2D(7);
		m_apCScene2D[nCntScene]->Init();
		m_apCScene2D[nCntScene]->BindTexture(m_apTexture[nCntScene]);
	}

	m_fGauge2 = 0.0f;
	m_SERecast = 599;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CGauge2::Uninit(void)
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
void CGauge2::Update(void)
{
	CManager manager;
	//キーボードの取得
	CInputKeyboard *pInput;
	pInput = manager.GetInputKeyboard();

	//サウンドの取得
	CSound *pSound;
	pSound = manager.GetSound();

	//頂点座標の設定･更新
	m_nCntAnim++;

	m_fGauge2 += (0.04f * 1);

	if (m_fGauge2 > 400.0f)
	{
		m_fGauge2 = 400.0f;
		m_SERecast++;
		if (m_SERecast >= 600)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_GAUGE_MAX);
			m_SERecast = 0;
		}
	}

	if (m_fGauge2 <= 0.0f)
	{
		m_fGauge2 = 0.0f;
	}

	for (int nCntScene = 0; nCntScene < MAX_GAUGE; nCntScene++)
	{
		VERTEX_2D *pVtx;
		LPDIRECT3DVERTEXBUFFER9 Vtx;
		Vtx = m_apCScene2D[nCntScene]->GetVtx();

		Vtx->Lock(0, 0, (void**)&pVtx, 0);

		if (nCntScene == 0)
		{
			pVtx[0].pos = D3DXVECTOR3(580.0f - 50, 650.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1000.0f - 50, 650.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(580.0f - 50, 690.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1000.0f - 50, 690.0f, 0.0f);
		}
		else if (nCntScene == 1)
		{
			pVtx[0].pos = D3DXVECTOR3(620.0f - 50, 650.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(m_fGauge2 + 600 - 50, 650.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(620.0f - 50, 690.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(m_fGauge2 + 600 - 50, 690.0f, 0.0f);
		}
		else if (nCntScene == 2)
		{
			pVtx[0].pos = D3DXVECTOR3(600.0f - 50, 640.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1010.0f - 50, 640.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(600.0f - 50, 690.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1010.0f - 50, 690.0f, 0.0f);
		}

		//アンロック
		Vtx->Unlock();
	}

	if (pInput->GetKeyboardTrigger(DIK_F) == true)
	{
		m_fGauge2 -= 250.0f;
	}

	if (pInput->GetKeyboardTrigger(DIK_G) == true)
	{
		m_fGauge2 += 250.0f;
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void CGauge2::Draw(void)
{

}

//=============================================================================
// クリエイト
//=============================================================================
CGauge2 *CGauge2::Create(D3DXVECTOR3 pos)
{
	CGauge2 *pGauge2 = NULL;
	if (pGauge2 == NULL)
	{
		// 動的確保
		pGauge2 = new CGauge2;

		if (pGauge2 != NULL)
		{
			// 初期化処理
			pGauge2->Init(pos);
		}
	}
	return pGauge2;
}