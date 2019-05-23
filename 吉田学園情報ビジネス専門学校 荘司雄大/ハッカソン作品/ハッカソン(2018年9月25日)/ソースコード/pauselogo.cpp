//=============================================================================
//
// ポーズロゴ処理 [pauselogo.cpp]
// Author : Niwa Hodaka
//
//=============================================================================
#include "pauselogo.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME       "data/TEXTURE/pause100.png"    // 読み込むテクスチャファイル名
#define PAUSELOGO_POS_X    (460)                          // ポーズロゴの左上Ｘ座標
#define PAUSELOGO_POS_Y    (180)                          // ポーズロゴ左上Ｙ座標
#define PAUSELOGO_WIDTH    (820)                          // ポーズロゴの幅
#define PAUSELOGO_HEIGHT   (540)                          // ポーズロゴの高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexPauseLogo(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePauseLogo = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPauseLogo = NULL;	// 頂点バッファへのポインタ
D3DXCOLOR				g_colorPauseLogo;		    // ポーズロゴ色

//=============================================================================
// 初期化処理
//=============================================================================
void InitPauseLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 値の初期化
	g_colorPauseLogo = D3DXCOLOR(0.5f, 0.25f, 0.112f, 1.0f);

	// 頂点情報の設定
	MakeVertexPauseLogo(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPauseLogo(void)
{
	// テクスチャの開放
	if (g_pTexturePauseLogo != NULL)
	{
		g_pTexturePauseLogo->Release();
		g_pTexturePauseLogo = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffPauseLogo != NULL)
	{
		g_pVtxBuffPauseLogo->Release();
		g_pVtxBuffPauseLogo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePauseLogo(void)
{
}
//=============================================================================
// タイトル画面
//=============================================================================
void DrawPauseLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPauseLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePauseLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexPauseLogo(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME,
		&g_pTexturePauseLogo);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPauseLogo,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffPauseLogo->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(PAUSELOGO_POS_X, PAUSELOGO_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSELOGO_WIDTH, PAUSELOGO_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSELOGO_POS_X, PAUSELOGO_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSELOGO_WIDTH, PAUSELOGO_HEIGHT, 0.0f);

	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = g_colorPauseLogo;
	pVtx[1].col = g_colorPauseLogo;
	pVtx[2].col = g_colorPauseLogo;
	pVtx[3].col = g_colorPauseLogo;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点バッファをアンロックする
	g_pVtxBuffPauseLogo->Unlock();
}