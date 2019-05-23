//=============================================================================
//
// 体力のカバー処理 [coverlife.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "coverlife.h"
#include "lifegauge.h"
#include "main.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_COVERLIFE		"data/TEXTURE/GAUGE_COVER.png"		// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexCoverLife(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureCoverLife = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCoverLife = NULL;				// 頂点バッファへのポインタ
LifeGauge				g_aCoverLife[MAX_LIFEGAUGE] = {};			// ゲージの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitCoverLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float fCoverLifePos = 0;		//ゲージの位置

									// 値の初期化
	for (int nCntCoverLife = 0; nCntCoverLife < MAX_LIFEGAUGE; nCntCoverLife++)
	{//ゲージの数だけ繰り返し
		g_aCoverLife[nCntCoverLife].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 125.0f + fCoverLifePos, 50.0f, 0.0f);	//位置の初期化
		g_aCoverLife[nCntCoverLife].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);								//色の初期化
		g_aCoverLife[nCntCoverLife].fWidth = LIFEGAUGE_WIDTH + 30.0f;												//幅の初期化
		g_aCoverLife[nCntCoverLife].fHeight = 40;												//高さの初期化
		g_aCoverLife[nCntCoverLife].nType = nCntCoverLife;														//タイプ
		fCoverLifePos += 250.0f;
	}

	// 頂点情報の設定
	MakeVertexCoverLife(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCoverLife(void)
{
	// テクスチャの開放
	if (g_pTextureCoverLife != NULL)
	{
		g_pTextureCoverLife->Release();
		g_pTextureCoverLife = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffCoverLife != NULL)
	{
		g_pVtxBuffCoverLife->Release();
		g_pVtxBuffCoverLife = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCoverLife(void)
{

}
//=============================================================================
// タイトル画面
//=============================================================================
void DrawCoverLife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffCoverLife, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureCoverLife);

	for (int nCntCoverLife = 0; nCntCoverLife < MAX_LIFEGAUGE; nCntCoverLife++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntCoverLife, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexCoverLife(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_COVERLIFE,
		&g_pTextureCoverLife);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFEGAUGE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCoverLife,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

					   // 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffCoverLife->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCoverLife = 0; nCntCoverLife < MAX_LIFEGAUGE; nCntCoverLife++)
	{
		switch (nCntCoverLife)
		{
		case 0:
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x - g_aCoverLife[nCntCoverLife].fWidth, g_aCoverLife[nCntCoverLife].pos.y - g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x, g_aCoverLife[nCntCoverLife].pos.y - g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x - g_aCoverLife[nCntCoverLife].fWidth, g_aCoverLife[nCntCoverLife].pos.y + g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x, g_aCoverLife[nCntCoverLife].pos.y + g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
			break;
		case 1:
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x, g_aCoverLife[nCntCoverLife].pos.y - g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x + g_aCoverLife[nCntCoverLife].fWidth, g_aCoverLife[nCntCoverLife].pos.y - g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x, g_aCoverLife[nCntCoverLife].pos.y + g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x + g_aCoverLife[nCntCoverLife].fWidth, g_aCoverLife[nCntCoverLife].pos.y + g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			break;
		}
		// 頂点テクスチャ
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = g_aCoverLife[nCntCoverLife].col;
		pVtx[1].col = g_aCoverLife[nCntCoverLife].col;
		pVtx[2].col = g_aCoverLife[nCntCoverLife].col;
		pVtx[3].col = g_aCoverLife[nCntCoverLife].col;

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffCoverLife->Unlock();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetCoverLife(float fWidth, int nType)
{
	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

					   // 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffCoverLife->Lock(0, 0, (void**)&pVtx, 0);

	g_aCoverLife[nType].fWidth -= fWidth;	//幅の減算処理

	for (int nCntCoverLife = 0; nCntCoverLife < MAX_LIFEGAUGE; nCntCoverLife++)
	{//ゲージの数だけ繰り返し
		switch (nCntCoverLife)
		{
		case 0:	//プレイヤー０の場合
				// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x - g_aCoverLife[nCntCoverLife].fWidth, g_aCoverLife[nCntCoverLife].pos.y - g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x, g_aCoverLife[nCntCoverLife].pos.y - g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x - g_aCoverLife[nCntCoverLife].fWidth, g_aCoverLife[nCntCoverLife].pos.y + g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x, g_aCoverLife[nCntCoverLife].pos.y + g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			break;
		case 1:	//プレイヤー１の場合
				// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x, g_aCoverLife[nCntCoverLife].pos.y - g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x + g_aCoverLife[nCntCoverLife].fWidth, g_aCoverLife[nCntCoverLife].pos.y - g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x, g_aCoverLife[nCntCoverLife].pos.y + g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aCoverLife[nCntCoverLife].pos.x + g_aCoverLife[nCntCoverLife].fWidth, g_aCoverLife[nCntCoverLife].pos.y + g_aCoverLife[nCntCoverLife].fHeight, g_aCoverLife[nCntCoverLife].pos.z);
			break;
		}
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffCoverLife->Unlock();
}