//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "particle.h"

//静的メンバ変数
CRenderer *m_pParticleRenderer = NULL;
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CParticle::CParticle() : CScene2D(5)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CParticle::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE, &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CParticle::Unload(void)
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
HRESULT CParticle::Init(void)
{
	CScene2D::BindTexture(m_pTexture);

	//パーティクルの初期設定
	CScene2D::Init();

	SetParticle(m_posParticle,m_move, m_col, m_fRadius);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CParticle::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CParticle::Update(void)
{
	//ポジションの取得
	D3DXVECTOR3 pos;
	pos = *GetPosition();

	pos -= m_move;

	m_nLife--;
	m_fRadius = m_fRadius;

	m_col.a -= 0.03f;

	SetParticle(pos,m_move, m_col, m_fRadius);

	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CParticle::Draw(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene2D::Draw();

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// クリエイト
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int life)
{
	CParticle *pParticle = NULL;
	if (pParticle == NULL)
	{
		// 動的確保
		pParticle = new CParticle;
		if (pParticle != NULL)
		{
			pParticle->m_posParticle = pos;
			pParticle->m_move = move;
			pParticle->m_col = col;
			pParticle->m_fRadius = fRadius;
			pParticle->m_nLife = life;

			// 初期化処理
			pParticle->Init();
		}
	}
	return pParticle;
}