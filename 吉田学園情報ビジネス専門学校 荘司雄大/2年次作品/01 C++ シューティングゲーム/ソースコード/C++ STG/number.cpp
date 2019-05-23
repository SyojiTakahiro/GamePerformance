//=============================================================================
//
// 数字処理 [number.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "number.h"
#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
//静的メンバ変数
//CRenderer *m_pRenderer = NULL;
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

bool g_bNumberDelete;
//=============================================================================
// コンストラクタ
//=============================================================================
CNumber::CNumber()
{
	m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
}

//=============================================================================
// デストラクタ
//=============================================================================
CNumber::~CNumber() 
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CNumber::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	if (m_pTexture == NULL)
	{
		D3DXCreateTextureFromFile(pDevice, NUMBER_TEXTURE, &m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CNumber::Unload(void)
{
	//リリース
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CNumber::Init(void)
{
	CManager manager;

	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;

	// 頂点情報を設定
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_posNumber.x + sinf(-D3DX_PI*0.75f) * 30;
	pVtx[1].pos.x = m_posNumber.x + sinf(D3DX_PI*0.75f) * 30;
	pVtx[2].pos.x = m_posNumber.x + sinf(-D3DX_PI*0.25f) * 30;
	pVtx[3].pos.x = m_posNumber.x + sinf(D3DX_PI*0.25f) * 30;

	pVtx[0].pos.y = m_posNumber.y + cosf(-D3DX_PI*0.75f) * 30;
	pVtx[1].pos.y = m_posNumber.y + cosf(D3DX_PI*0.75f) * 30;
	pVtx[2].pos.y = m_posNumber.y + cosf(-D3DX_PI*0.25f) * 30;
	pVtx[3].pos.y = m_posNumber.y + cosf(D3DX_PI*0.25f) * 30;

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0, 0);
	pVtx[1].tex = D3DXVECTOR2(1, 0);
	pVtx[2].tex = D3DXVECTOR2(0, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 1);

	//アンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CNumber::Uninit(void)
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void CNumber::Update(void)
{
	
}

//=============================================================================
// 描画処理
//=============================================================================
void CNumber::Draw(void)
{
	CManager manager;

	LPDIRECT3DDEVICE9 pDevice = NULL;

	CRenderer *pRenderer;
	pRenderer = manager.GetRenderer();
	pDevice = pRenderer->GetDevice();
	//or
	//pDevice = manager.GetRenderer()->GetDevice();


	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,  0, 2);
}

//=============================================================================
// クリエイト
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos)
{
	CNumber *pNumber = NULL;
	if (pNumber == NULL)
	{
		// 動的確保
		pNumber = new CNumber;

		if (pNumber != NULL)
		{
			pNumber->m_posNumber = pos;
			// 初期化処理
			pNumber->Init();
		}
	}
	return pNumber;
}

//=============================================================================
// セットナンバー
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D *pVtx;

	// 頂点情報を設定
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// テクスチャ設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 1.0f);

	//アンロック
	m_pVtxBuff->Unlock();
}