//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : Niwa Hodaka
//
//=============================================================================
#include "pause.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_POS_X        (0)                         // ポーズの左上Ｘ座標
#define PAUSE_POS_Y        (0)                         // ポーズの左上Ｙ座標
#define PAUSE_WIDTH        (SCREEN_WIDTH)              // ポーズの幅
#define PAUSE_HEIGHT       (SCREEN_HEIGHT)             // ポーズの高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexPause(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePause = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	// 頂点バッファへのポインタ
D3DXCOLOR				g_colorPause;			// ポーズ画面色

//=============================================================================
// 初期化処理
//=============================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 値の初期化
	g_colorPause = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f);

	// 頂点情報の設定
	MakeVertexPause(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPause(void)
{
	// テクスチャの開放
	if (g_pTexturePause != NULL)
	{
		g_pTexturePause->Release();
		g_pTexturePause = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePause(void)
{

}

//=============================================================================
// タイトル画面
//=============================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePause);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexPause(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

					   // 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_WIDTH, PAUSE_HEIGHT, 0.0f);


	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = g_colorPause;
	pVtx[1].col = g_colorPause;
	pVtx[2].col = g_colorPause;
	pVtx[3].col = g_colorPause;

	// 頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}