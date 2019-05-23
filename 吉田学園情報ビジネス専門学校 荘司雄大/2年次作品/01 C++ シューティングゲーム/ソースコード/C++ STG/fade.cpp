//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "fade.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FADE_POS_X			(0)					//X座標
#define FADE_POS_Y			(0)					//Y座標
#define FADE_WIDTH			(SCREEN_WIDTH)		//画面の幅
#define FADE_HEIGHT			(SCREEN_HEIGHT)		//画面の高さ

//*****************************************************************************
// 静的メンバ
//*****************************************************************************
CFade::FADE CFade::m_fade = CFade::FADE_IN;

//=============================================================================
// コンストラクタ
//=============================================================================
CFade::CFade()
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CFade::~CFade()
{

}


//=============================================================================
// クリエイト
//=============================================================================
CFade *CFade::Create()
{
	CFade *pFade = NULL;
	if (pFade == NULL)
	{
		// 動的確保
		pFade = new CFade;
		if (pFade != NULL)
		{
			// 初期化処理
			pFade->Init();
		}
	}
	return pFade;
}

//=============================================================================
// 初期化処理
//=============================================================================
void CFade::Init(void)
{
	CManager manager;
	VERTEX_2D*pVtx;

	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// 頂点バッファの情報を設定
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(FADE_POS_X, FADE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(FADE_POS_X + FADE_WIDTH, FADE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(FADE_POS_X, FADE_POS_Y + FADE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(FADE_POS_X + FADE_WIDTH, FADE_POS_Y + FADE_HEIGHT, 0.0f);

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

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// 値の初期化
	m_fade = FADE_IN;
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
}

//=============================================================================
// 終了処理
//=============================================================================
void CFade::Uninit(void)
{
	// テクスチャの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// 頂点バッファの開放
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void CFade::Update(void)
{
	CManager manager;

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			m_colorFade.a -= 1.0f / 30;
			if (m_colorFade.a <= 0.0f)
			{
				m_colorFade.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_colorFade.a += 1.0f /30;
			if (m_colorFade.a >= 1.0f)
			{
				m_colorFade.a = 1.0f;
				m_fade = FADE_IN;
				//
				manager.SetMode(m_modeNext);
			}
		}
		VERTEX_2D*pVtx;
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx[0].col = m_colorFade;
		pVtx[1].col = m_colorFade;
		pVtx[2].col = m_colorFade;
		pVtx[3].col = m_colorFade;
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// タイトル画面
//=============================================================================
void CFade::Draw(void)
{
	CManager manager;

	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// フェードの設定
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	if (m_fade != FADE_OUT)
	{
		m_fade = FADE_OUT;
		m_modeNext = modeNext;
		m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// フェードの取得
//=============================================================================
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}

