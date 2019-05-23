//=============================================================================
//
// ランキングランク処理 [rankingrank.cpp]
// Author : Niwa Hodaka
//
//=============================================================================
#include "rankingrank.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME       "data/TEXTURE/ranking_rank.png"   // 読み込むテクスチャファイル名
#define RANNKINGRANK_POS_X    (250)                          // ランキングランクの左上Ｘ座標
#define RANNKINGRANK_POS_Y    (135)                           // ランキングランク左上Ｙ座標
#define RANNKINGRANK_WIDTH    (400)                          // ランキングランクの幅
#define RANNKINGRANK_HEIGHT   (660)                          // ランキングランクの高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexRankingRank(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRankingRank = NULL; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingRank = NULL; // 頂点バッファへのポインタ
D3DXCOLOR				g_colorRankingRank;		      // ランキングランク色.

//=============================================================================
// 初期化処理
//=============================================================================
void InitRankingRank(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 値の初期化
	g_colorRankingRank = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	// 頂点情報の設定
	MakeVertexRankingRank(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRankingRank(void)
{
	// テクスチャの開放
	if (g_pTextureRankingRank != NULL)
	{
		g_pTextureRankingRank->Release();
		g_pTextureRankingRank = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffRankingRank != NULL)
	{
		g_pVtxBuffRankingRank->Release();
		g_pVtxBuffRankingRank = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRankingRank(void)
{
}
//=============================================================================
// タイトル画面
//=============================================================================
void DrawRankingRank(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffRankingRank, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRankingRank);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexRankingRank(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME,
		&g_pTextureRankingRank);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingRank,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

					   // 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffRankingRank->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(RANNKINGRANK_POS_X, RANNKINGRANK_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(RANNKINGRANK_WIDTH, RANNKINGRANK_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(RANNKINGRANK_POS_X, RANNKINGRANK_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(RANNKINGRANK_WIDTH, RANNKINGRANK_HEIGHT, 0.0f);

	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = g_colorRankingRank;
	pVtx[1].col = g_colorRankingRank;
	pVtx[2].col = g_colorRankingRank;
	pVtx[3].col = g_colorRankingRank;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点バッファをアンロックする
	g_pVtxBuffRankingRank->Unlock();
}