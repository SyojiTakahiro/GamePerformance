//=============================================================================
//
// 体力の土台のゲージ処理 [lifegauge.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "baselifegauge.h"
#include "lifegauge.h"
#include "main.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_BASELIFEGAUGE		"data/TEXTURE/GAUGE_BASE.png"		// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexBaseLifeGauge(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBaseGauge = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBaseGauge = NULL;				// 頂点バッファへのポインタ
LifeGauge				g_aBaseLifeGauge[MAX_LIFEGAUGE] = {};			// ゲージの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitBaseLifeGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float fBaseGaugePos = 0;		//ゲージの位置

								// 値の初期化
	for (int nCntBaseGauge = 0; nCntBaseGauge < MAX_LIFEGAUGE; nCntBaseGauge++)
	{//ゲージの数だけ繰り返し
		g_aBaseLifeGauge[nCntBaseGauge].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 100.0f + fBaseGaugePos, 50.0f, 0.0f);	//位置の初期化
		g_aBaseLifeGauge[nCntBaseGauge].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);								//色の初期化
		g_aBaseLifeGauge[nCntBaseGauge].fWidth = LIFEGAUGE_WIDTH + 40;												//幅の初期化
		g_aBaseLifeGauge[nCntBaseGauge].fHeight = LIFEGAUGE_HEIGHT + 2;												//高さの初期化
		g_aBaseLifeGauge[nCntBaseGauge].nType = nCntBaseGauge;														//タイプ
		fBaseGaugePos += 200.0f;
	}

	// 頂点情報の設定
	MakeVertexBaseLifeGauge(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBaseLifeGauge(void)
{
	// テクスチャの開放
	if (g_pTextureBaseGauge != NULL)
	{
		g_pTextureBaseGauge->Release();
		g_pTextureBaseGauge = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBaseGauge != NULL)
	{
		g_pVtxBuffBaseGauge->Release();
		g_pVtxBuffBaseGauge = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBaseLifeGauge(void)
{

}
//=============================================================================
// タイトル画面
//=============================================================================
void DrawBaseLifeGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBaseGauge, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBaseGauge);

	for (int nCntBaseGauge = 0; nCntBaseGauge < MAX_LIFEGAUGE; nCntBaseGauge++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBaseGauge, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexBaseLifeGauge(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_BASELIFEGAUGE,
		&g_pTextureBaseGauge);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFEGAUGE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBaseGauge,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

					   // 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffBaseGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBaseGauge = 0; nCntBaseGauge < MAX_LIFEGAUGE; nCntBaseGauge++)
	{
		switch (nCntBaseGauge)
		{
		case 0:
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x - g_aBaseLifeGauge[nCntBaseGauge].fWidth, g_aBaseLifeGauge[nCntBaseGauge].pos.y - g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x, g_aBaseLifeGauge[nCntBaseGauge].pos.y - g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x - g_aBaseLifeGauge[nCntBaseGauge].fWidth, g_aBaseLifeGauge[nCntBaseGauge].pos.y + g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x, g_aBaseLifeGauge[nCntBaseGauge].pos.y + g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
			break;
		case 1:
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x, g_aBaseLifeGauge[nCntBaseGauge].pos.y - g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x + g_aBaseLifeGauge[nCntBaseGauge].fWidth, g_aBaseLifeGauge[nCntBaseGauge].pos.y - g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x, g_aBaseLifeGauge[nCntBaseGauge].pos.y + g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x + g_aBaseLifeGauge[nCntBaseGauge].fWidth, g_aBaseLifeGauge[nCntBaseGauge].pos.y + g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);

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
		pVtx[0].col = g_aBaseLifeGauge[nCntBaseGauge].col;
		pVtx[1].col = g_aBaseLifeGauge[nCntBaseGauge].col;
		pVtx[2].col = g_aBaseLifeGauge[nCntBaseGauge].col;
		pVtx[3].col = g_aBaseLifeGauge[nCntBaseGauge].col;

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBaseGauge->Unlock();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetBaseLifeGauge(float fWidth, int nType)
{
	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

					   // 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffBaseGauge->Lock(0, 0, (void**)&pVtx, 0);

	g_aBaseLifeGauge[nType].fWidth -= fWidth;	//幅の減算処理

	for (int nCntBaseGauge = 0; nCntBaseGauge < MAX_LIFEGAUGE; nCntBaseGauge++)
	{//ゲージの数だけ繰り返し
		switch (nCntBaseGauge)
		{
		case 0:	//プレイヤー０の場合
				// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x - g_aBaseLifeGauge[nCntBaseGauge].fWidth, g_aBaseLifeGauge[nCntBaseGauge].pos.y - g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x, g_aBaseLifeGauge[nCntBaseGauge].pos.y - g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x - g_aBaseLifeGauge[nCntBaseGauge].fWidth, g_aBaseLifeGauge[nCntBaseGauge].pos.y + g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x, g_aBaseLifeGauge[nCntBaseGauge].pos.y + g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			break;
		case 1:	//プレイヤー１の場合
				// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x, g_aBaseLifeGauge[nCntBaseGauge].pos.y - g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x + g_aBaseLifeGauge[nCntBaseGauge].fWidth, g_aBaseLifeGauge[nCntBaseGauge].pos.y - g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x, g_aBaseLifeGauge[nCntBaseGauge].pos.y + g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aBaseLifeGauge[nCntBaseGauge].pos.x + g_aBaseLifeGauge[nCntBaseGauge].fWidth, g_aBaseLifeGauge[nCntBaseGauge].pos.y + g_aBaseLifeGauge[nCntBaseGauge].fHeight, g_aBaseLifeGauge[nCntBaseGauge].pos.z);
			break;
		}
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffBaseGauge->Unlock();
}