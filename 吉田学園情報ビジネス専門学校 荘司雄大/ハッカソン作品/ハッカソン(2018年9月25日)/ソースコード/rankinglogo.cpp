//=============================================================================
//
// ランキングロゴ処理 [rankinglogo.cpp]
// Author : Niwa Hodaka
//
//=============================================================================
#include "rankinglogo.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME      "data/TEXTURE/rankinglogo000.png"  // 読み込むテクスチャファイル名
#define TEXTURE_SIZEX     (460)                              // テクスチャを張り付ける幅
#define TEXTURE_SIZEY     (70)                               // テクスチャを張り付ける高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexRankingLogo(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRankingLogo = NULL; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingLogo = NULL; // 頂点バッファへのポインタ
D3DXVECTOR3             g_PosRankingLogo;             // ランキングロゴ座標
D3DXCOLOR				g_colorRankingLogo;		      // ランキングロゴ色

//=============================================================================
// 初期化処理
//=============================================================================
void InitRankingLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 値の初期化
	g_PosRankingLogo = D3DXVECTOR3(SCREEN_WIDTH / 2, 80, 0.0f);
	g_colorRankingLogo = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// 頂点情報の設定
	MakeVertexRankingLogo(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRankingLogo(void)
{
	// テクスチャの破棄
	if (g_pTextureRankingLogo != NULL)
	{
		g_pTextureRankingLogo->Release();
		g_pTextureRankingLogo = NULL;
	}


	// 頂点バッファの開放
	if (g_pVtxBuffRankingLogo != NULL)
	{
		g_pVtxBuffRankingLogo->Release();
		g_pVtxBuffRankingLogo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRankingLogo(void)
{
}
//=============================================================================
// タイトル画面
//=============================================================================
void DrawRankingLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffRankingLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRankingLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexRankingLogo(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME,
		&g_pTextureRankingLogo);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingLogo,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffRankingLogo->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_PosRankingLogo.x - TEXTURE_SIZEX, g_PosRankingLogo.y - TEXTURE_SIZEY, g_PosRankingLogo.z);
	pVtx[1].pos = D3DXVECTOR3(g_PosRankingLogo.x + TEXTURE_SIZEX, g_PosRankingLogo.y - TEXTURE_SIZEY, g_PosRankingLogo.z);
	pVtx[2].pos = D3DXVECTOR3(g_PosRankingLogo.x - TEXTURE_SIZEX, g_PosRankingLogo.y + TEXTURE_SIZEY, g_PosRankingLogo.z);
	pVtx[3].pos = D3DXVECTOR3(g_PosRankingLogo.x + TEXTURE_SIZEX, g_PosRankingLogo.y + TEXTURE_SIZEY, g_PosRankingLogo.z);

	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = g_colorRankingLogo;
	pVtx[1].col = g_colorRankingLogo;
	pVtx[2].col = g_colorRankingLogo;
	pVtx[3].col = g_colorRankingLogo;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点バッファをアンロックする
	g_pVtxBuffRankingLogo->Unlock();
}