//=============================================================================
//
// 体力ゲージ処理 [lifegauge.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "Rush.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_RUSH		"data/TEXTURE/ultlogo.png"		// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexRushGauge(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRush = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRush = NULL;				// 頂点バッファへのポインタ
bool g_bRushDisp = false;
//=============================================================================
// 初期化処理
//=============================================================================
void InitRush(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の設定
	MakeVertexRushGauge(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRush(void)
{
	// テクスチャの開放
	if (g_pTextureRush != NULL)
	{
		g_pTextureRush->Release();
		g_pTextureRush = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffRush != NULL)
	{
		g_pVtxBuffRush->Release();
		g_pVtxBuffRush = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRush(void)
{

}
//=============================================================================
// タイトル画面
//=============================================================================
void DrawRush(void)
{
	if (g_bRushDisp == true)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffRush, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRush);

			// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexRushGauge(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_RUSH,
		&g_pTextureRush);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRush,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffRush->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, 400.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 200.0f, 400.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 200.0f, 600.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 200.0f, 600.0f, 0.0f);

	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	g_pVtxBuffRush->Unlock();
}

void SetRush(bool b)
{
	g_bRushDisp = b;
}