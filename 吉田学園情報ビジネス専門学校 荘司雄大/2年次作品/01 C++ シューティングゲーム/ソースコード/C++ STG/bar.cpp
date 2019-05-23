//=============================================================================
//
// bar処理 [bar.cpp]
// Author : Takahiro Syoji
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "bar.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_SPEED0			(0.000f)
#define BG_TEXTURE_BAR		"data/TEXTURE/bar.png"

//静的メンバ変数
CRenderer *m_pBarRenderer = NULL;
LPDIRECT3DTEXTURE9 CBar::m_pTexture = NULL;

bool g_bBarDelete;
//=============================================================================
// コンストラクタ
//=============================================================================
CBar::CBar() : CScene(2)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CBar::~CBar()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CBar::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE_BAR, &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CBar::Unload(void)
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
HRESULT CBar::Init(void)
{

	m_pCScene2D = new CScene2D(4);
	m_pCScene2D->Init();
	m_pCScene2D->BindTexture(m_pTexture);



	VERTEX_2D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 Vtx;
	Vtx = m_pCScene2D->GetVtx();

	// 頂点情報を設定
	Vtx->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0 + SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, 0 + SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0 + SCREEN_WIDTH, 0 + SCREEN_HEIGHT, 0.0f);

	//アンロック
	Vtx->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBar::Uninit(void)
{
	if (m_pCScene2D != NULL)
	{
		m_pCScene2D->Uninit();
		m_pCScene2D = NULL;
	}
	//リリース
	CScene::Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CBar::Update(void)
{
	//頂点座標の設定･更新
	m_nCntAnim++;

	VERTEX_2D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 Vtx;
	Vtx = m_pCScene2D->GetVtx();

	Vtx->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED0), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED0), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED0), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED0), 1.0f);


	//アンロック
	Vtx->Unlock();

}

//=============================================================================
// 描画処理
//=============================================================================
void CBar::Draw(void)
{

}

//=============================================================================
// クリエイト
//=============================================================================
CBar *CBar::Create(void)
{
	CBar *pBar = NULL;
	if (pBar == NULL)
	{
		// 動的確保
		pBar = new CBar;

		if (pBar != NULL)
		{
			// 初期化処理
			pBar->Init();
		}
	}
	return pBar;
}