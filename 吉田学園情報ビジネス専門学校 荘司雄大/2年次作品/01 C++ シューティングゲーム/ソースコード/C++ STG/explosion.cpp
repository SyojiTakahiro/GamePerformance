//=============================================================================
//
// 爆発処理 [explosion.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "explosion.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************

//静的メンバ変数
CRenderer *m_pExplosionRenderer = NULL;
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CExplosion::CExplosion() : CScene2D(3)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CExplosion::~CExplosion()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CExplosion::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TEXTURE, &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CExplosion::Unload(void)
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
HRESULT CExplosion::Init(void)
{
	CScene2D::BindTexture(m_pTexture);

	//弾の初期設定
	CScene2D::Init();

	SetExplosion(0);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CExplosion::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CExplosion::Update(void)
{
	m_nCounterAnim++;
	//カウンターの更新
	if ((m_nCounterAnim % 2) == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % 8;
		SetExplosion(m_nPatternAnim);

		//パターン終了時に終了
		if (m_nPatternAnim >= 8 - 1 )
		{
			Uninit();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CExplosion::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// クリエイト
//=============================================================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos,float size)
{
	CExplosion *pExplosion = NULL;
	if (pExplosion == NULL)
	{
		// 動的確保
		pExplosion = new CExplosion;
		if (pExplosion != NULL)
		{
			pExplosion->m_posPolygon = pos;
			pExplosion->SetSize(size);

			// 初期化処理
			pExplosion->Init();
		}
	}
	return pExplosion;
}