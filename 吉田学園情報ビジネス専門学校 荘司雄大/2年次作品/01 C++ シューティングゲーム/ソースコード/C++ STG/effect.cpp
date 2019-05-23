//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "effect.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//静的メンバ変数
CRenderer *m_pEffectRenderer = NULL;
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CEffect::CEffect() : CScene2D(2)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CEffect::Load(void)
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
void CEffect::Unload(void)
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
HRESULT CEffect::Init(void)
{
	CScene2D::BindTexture(m_pTexture);

	//弾の初期設定
	CScene2D::Init();

	SetEffect(m_posEffect,m_col, m_fRadius);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CEffect::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CEffect::Update(void)
{
	m_nLife -= 3;
	m_fRadius -= 1.0f;

	if (m_fRadius <= 0)
	{
		m_fRadius = 0.0f;
	}

	SetEffect(m_posEffect, m_col, m_fRadius);

	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CEffect::Draw(void)
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
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius,int life)
{
	CEffect *pEffect = NULL;
	if (pEffect == NULL)
	{
		// 動的確保
		pEffect = new CEffect;
		if (pEffect != NULL)
		{
			pEffect->m_posEffect = pos;
			pEffect->m_col = col;
			pEffect->m_fRadius = fRadius;
			pEffect->m_nLife = life;

			// 初期化処理
			pEffect->Init();
		}
	}
	return pEffect;
}