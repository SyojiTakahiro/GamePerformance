//=============================================================================
//
// 体力ゲージ処理 [lifegauge.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "lifegauge.h"
#include "main.h"
#include "input.h"
#include "damagegauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_LIFEGAUGE		"data/TEXTURE/LIFE_GAUGE.png"		// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexLifeGauge(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureGauge = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;				// 頂点バッファへのポインタ
LifeGauge				g_aLifeGauge[MAX_LIFEGAUGE] = {};			// ゲージの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitLifeGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float fGaugePos = 0;		//ゲージの位置

	// 値の初期化
	for (int nCntGauge = 0 ; nCntGauge < MAX_LIFEGAUGE; nCntGauge++)
	{//ゲージの数だけ繰り返し
		g_aLifeGauge[nCntGauge].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 152 + fGaugePos, 50.0f, 0.0f);	//位置の初期化
		g_aLifeGauge[nCntGauge].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);						//色の初期化
		g_aLifeGauge[nCntGauge].fWidth = LIFEGAUGE_WIDTH;											//幅の初期化
		g_aLifeGauge[nCntGauge].fHeight = LIFEGAUGE_HEIGHT;											//高さの初期化
		g_aLifeGauge[nCntGauge].nType = nCntGauge;												//タイプ
		fGaugePos += 305;
	}

	// 頂点情報の設定
	MakeVertexLifeGauge(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLifeGauge(void)
{
	// テクスチャの開放
	if (g_pTextureGauge != NULL)
	{
		g_pTextureGauge->Release();
		g_pTextureGauge = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLifeGauge(void)
{

}
//=============================================================================
// タイトル画面
//=============================================================================
void DrawLifeGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureGauge);

	for (int nCntGauge = 0; nCntGauge < MAX_LIFEGAUGE; nCntGauge++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntGauge, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexLifeGauge(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_LIFEGAUGE,
		&g_pTextureGauge);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFEGAUGE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_LIFEGAUGE; nCntGauge++)
	{
		switch (nCntGauge)
		{
		case 0:
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x - g_aLifeGauge[nCntGauge].fWidth, g_aLifeGauge[nCntGauge].pos.y - g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x, g_aLifeGauge[nCntGauge].pos.y - g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x - g_aLifeGauge[nCntGauge].fWidth, g_aLifeGauge[nCntGauge].pos.y + g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x, g_aLifeGauge[nCntGauge].pos.y + g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
			break;
		case 1:
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x, g_aLifeGauge[nCntGauge].pos.y - g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x + g_aLifeGauge[nCntGauge].fWidth, g_aLifeGauge[nCntGauge].pos.y - g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x, g_aLifeGauge[nCntGauge].pos.y + g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x + g_aLifeGauge[nCntGauge].fWidth, g_aLifeGauge[nCntGauge].pos.y + g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);

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
		pVtx[0].col = g_aLifeGauge[nCntGauge].col;
		pVtx[1].col = g_aLifeGauge[nCntGauge].col;
		pVtx[2].col = g_aLifeGauge[nCntGauge].col;
		pVtx[3].col = g_aLifeGauge[nCntGauge].col;

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();
}

//=============================================================================
// 体力ゲージの設置処理
//=============================================================================
void SetLifeGauge(float fWidth, int nType)
{
	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	g_aLifeGauge[nType].fWidth -= fWidth;	//幅の減算処理

	for (int nCntGauge = 0; nCntGauge < MAX_LIFEGAUGE; nCntGauge++)
	{//ゲージの数だけ繰り返し
		switch (nCntGauge)
		{
		case 0:	//プレイヤー０の場合
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x - g_aLifeGauge[nCntGauge].fWidth, g_aLifeGauge[nCntGauge].pos.y - g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x, g_aLifeGauge[nCntGauge].pos.y - g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x - g_aLifeGauge[nCntGauge].fWidth, g_aLifeGauge[nCntGauge].pos.y + g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x, g_aLifeGauge[nCntGauge].pos.y + g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			break;
		case 1:	//プレイヤー１の場合
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x, g_aLifeGauge[nCntGauge].pos.y - g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x + g_aLifeGauge[nCntGauge].fWidth, g_aLifeGauge[nCntGauge].pos.y - g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x, g_aLifeGauge[nCntGauge].pos.y + g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aLifeGauge[nCntGauge].pos.x + g_aLifeGauge[nCntGauge].fWidth, g_aLifeGauge[nCntGauge].pos.y + g_aLifeGauge[nCntGauge].fHeight, g_aLifeGauge[nCntGauge].pos.z);
			break;
		}
		SetDamageGaugeState(STATE_MINUS, nCntGauge, g_aLifeGauge[nCntGauge].fWidth);
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffGauge->Unlock();
}