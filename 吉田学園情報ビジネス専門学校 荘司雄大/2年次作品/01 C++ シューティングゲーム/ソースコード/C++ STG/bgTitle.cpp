//=============================================================================
//
// タイトル背景処理 [bgTitle.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "bgTitle.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_SPEED0			(0.000f)
#define BG_SPEED1			(-0.006f)
#define BG_SPEED2			(0.000f)

//静的メンバ変数
CRenderer *m_pBgTitleRenderer = NULL;
LPDIRECT3DTEXTURE9 CBgTitle::m_apTexture[MAX_BG] = {};

bool g_bBgTitleDelete;
//=============================================================================
// コンストラクタ
//=============================================================================
CBgTitle::CBgTitle() : CScene(0)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CBgTitle::~CBgTitle()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CBgTitle::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE0, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE1, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURE2, &m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CBgTitle::Unload(void)
{
	for (int nCntScene = 0; nCntScene < MAX_BG; nCntScene++)
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
HRESULT CBgTitle::Init(void)
{
	for (int nCntScene = 0; nCntScene < MAX_BG; nCntScene++)
	{
		m_apCScene2D[nCntScene] = new CScene2D(0);
		m_apCScene2D[nCntScene]->Init();
		m_apCScene2D[nCntScene]->BindTexture(m_apTexture[nCntScene]);

	}

	for (int nCntScene = 0; nCntScene < MAX_BG; nCntScene++)
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
void CBgTitle::Uninit(void)
{
	for (int nCntScene = 0; nCntScene < MAX_BG; nCntScene++)
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
void CBgTitle::Update(void)
{
	//頂点座標の設定･更新
	m_nCntAnim++;

	for (int nCntScene = 0; nCntScene < MAX_BG; nCntScene++)
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
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + (m_nCntAnim * BG_SPEED1));
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + (m_nCntAnim * BG_SPEED1));
			pVtx[2].tex = D3DXVECTOR2(0.0f ,1.0f + (m_nCntAnim * BG_SPEED1));
			pVtx[3].tex = D3DXVECTOR2(1.0f ,1.0f + (m_nCntAnim * BG_SPEED1));
		}
		else if (nCntScene == 2)
		{
			// 頂点情報を設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED2), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED2), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED2), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED2), 1.0f);
		}

		//アンロック
		Vtx->Unlock();
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void CBgTitle::Draw(void)
{

}

//=============================================================================
// クリエイト
//=============================================================================
CBgTitle *CBgTitle::Create(void)
{
	CBgTitle *pBgTitle = NULL;
	if (pBgTitle == NULL)
	{
		// 動的確保
		pBgTitle = new CBgTitle;

		if (pBgTitle != NULL)
		{
			// 初期化処理
			pBgTitle->Init();
		}
	}
	return pBgTitle;
}