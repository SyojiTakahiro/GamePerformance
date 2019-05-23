//=============================================================================
//
// チュートリアル背景処理 [bgTutorial.cpp]
// Author : Takahiro Syoji
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "bgTutorial.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_SPEED0			(0.000f)
#define BG_SPEED1			(0.003f)
#define BG_SPEED2			(0.000f)
#define BG_SPEED3			(0.000f)

#define BGTUTORIAL_TEXTURE		"data/TEXTURE/tutorial.png"

//静的メンバ変数
CRenderer *m_pBgTutorialRenderer = NULL;
LPDIRECT3DTEXTURE9 CBgTutorial::m_apTexture[MAX_GAME_BG] = {};

bool g_bBgTutorialDelete;
//=============================================================================
// コンストラクタ
//=============================================================================
CBgTutorial::CBgTutorial() : CScene(0)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CBgTutorial::~CBgTutorial()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CBgTutorial::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE0, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE1, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, BGTUTORIAL_TEXTURE, &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE3, &m_apTexture[3]);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CBgTutorial::Unload(void)
{
	for (int nCntScene = 0; nCntScene < MAX_GAME_BG; nCntScene++)
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
HRESULT CBgTutorial::Init(void)
{
	for (int nCntScene = 0; nCntScene < MAX_GAME_BG; nCntScene++)
	{
		m_apCScene2D[nCntScene] = new CScene2D(0);
		m_apCScene2D[nCntScene]->Init();
		m_apCScene2D[nCntScene]->BindTexture(m_apTexture[nCntScene]);

	}

	for (int nCntScene = 0; nCntScene < MAX_GAME_BG; nCntScene++)
	{
		VERTEX_2D *pVtx;
		LPDIRECT3DVERTEXBUFFER9 Vtx;
		Vtx = m_apCScene2D[nCntScene]->GetVtx();

		// 頂点情報を設定
		Vtx->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0 + SCREEN_WIDTH, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0 + SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0 + SCREEN_WIDTH, 0 + SCREEN_HEIGHT, 0.0f);

		//アンロック
		Vtx->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBgTutorial::Uninit(void)
{
	for (int nCntScene = 0; nCntScene < MAX_GAME_BG; nCntScene++)
	{
		m_apCScene2D[nCntScene]->Uninit();
		m_apCScene2D[nCntScene] = NULL;
	}
	//リリース
	CScene::Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CBgTutorial::Update(void)
{
	//頂点座標の設定･更新
	m_nCntAnim++;

	for (int nCntScene = 0; nCntScene < MAX_GAME_BG; nCntScene++)
	{
		VERTEX_2D *pVtx;
		LPDIRECT3DVERTEXBUFFER9 Vtx;
		Vtx = m_apCScene2D[nCntScene]->GetVtx();

		Vtx->Lock(0, 0, (void**)&pVtx, 0);

		if (nCntScene == 0)
		{
			// 頂点情報を設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED0), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED0), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED0), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED0), 1.0f);
		}
		else if (nCntScene == 1)
		{
			// 頂点情報を設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED1), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED1), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED1), 1.0f);
		}
		else if (nCntScene == 2)
		{
			// 頂点情報を設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED2), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED2), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED2), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED2), 1.0f);
		}
		else if (nCntScene == 3)
		{
			// 頂点情報を設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED3), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED3), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED3), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED3), 1.0f);
		}

		//アンロック
		Vtx->Unlock();
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void CBgTutorial::Draw(void)
{

}

//=============================================================================
// クリエイト
//=============================================================================
CBgTutorial *CBgTutorial::Create(void)
{
	CBgTutorial *pBgTutorial = NULL;
	if (pBgTutorial == NULL)
	{
		// 動的確保
		pBgTutorial = new CBgTutorial;

		if (pBgTutorial != NULL)
		{
			// 初期化処理
			pBgTutorial->Init();
		}
	}
	return pBgTutorial;
}