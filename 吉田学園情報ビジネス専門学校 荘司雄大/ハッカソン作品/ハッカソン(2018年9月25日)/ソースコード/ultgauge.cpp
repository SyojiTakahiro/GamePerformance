//=============================================================================
//
// 必殺ゲージ処理 [ultgauge.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "ultgauge.h"
#include "main.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_GAUGE		"data/TEXTURE/ULT_GAUGE.png"		// 読み込むテクスチャファイル名
#define ULTGAUGE_HEIGHT		(13)								// ゲージの高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexUltGauge(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureUltGauge = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUltGauge = NULL;				// 頂点バッファへのポインタ
UltGauge				g_aUltGauge[MAX_ULTGAUGE] = {};			// ゲージの情報
int						g_aCountCol[MAX_ULTGAUGE] = {};			// 色の情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitUltGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float fUltGaugePos = 0;		//ゲージの位置

								// 値の初期化
	for (int nCntUltGauge = 0; nCntUltGauge < MAX_ULTGAUGE; nCntUltGauge++)
	{//ゲージの数だけ繰り返し
		g_aUltGauge[nCntUltGauge].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 140 + fUltGaugePos, 100, 0.0f);	//位置の初期化
		g_aUltGauge[nCntUltGauge].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);									//色の初期化
		g_aUltGauge[nCntUltGauge].fWidth = 0.0f;															//幅の初期化
		g_aUltGauge[nCntUltGauge].fHeight = ULTGAUGE_HEIGHT;															//高さの初期化
		g_aUltGauge[nCntUltGauge].nType = nCntUltGauge;														//タイプ
		g_aUltGauge[nCntUltGauge].state = ULTGAUGE_NOMAL;														//状態の初期化
		fUltGaugePos += 280.0f;
	}

	// 頂点情報の設定
	MakeVertexUltGauge(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitUltGauge(void)
{
	// テクスチャの開放
	if (g_pTextureUltGauge != NULL)
	{
		g_pTextureUltGauge->Release();
		g_pTextureUltGauge = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffUltGauge != NULL)
	{
		g_pVtxBuffUltGauge->Release();
		g_pVtxBuffUltGauge = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateUltGauge(void)
{
	for (int nCntUltGauge = 0; nCntUltGauge < MAX_ULTGAUGE; nCntUltGauge++)
	{//ゲージの数だけ繰り返し
		switch (g_aUltGauge[nCntUltGauge].state)
		{
		case ULTGAUGE_NOMAL:	//通常状態の場合
			g_aUltGauge[nCntUltGauge].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//白色の代入
			break;
		case ULTGAUGE_READY:	//使用可能状態
			g_aCountCol[nCntUltGauge]++;											//カウント
			if (g_aCountCol[nCntUltGauge] % 4 == 0)
			{// 一定時間が立った場合
				if (g_aUltGauge[nCntUltGauge].col.b == 0.0f)
				{// 赤の場合
					g_aUltGauge[nCntUltGauge].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				else
				{// 白の場合
					g_aUltGauge[nCntUltGauge].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
			}
			break;
		case ULTGAUGE_USE:		//使用状態の場合
			g_aUltGauge[nCntUltGauge].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		//赤色にする

			if (g_aUltGauge[nCntUltGauge].fWidth >= 0)
			{//幅が０以上の場合
				//減算処理
				g_aUltGauge[nCntUltGauge].fWidth -= 1.0f;
			}
			else if (g_aUltGauge[nCntUltGauge].fWidth <= 0)
			{//幅が０以下の場合
				//幅の固定
				g_aUltGauge[nCntUltGauge].fWidth = 0;
				//通常状態にする
				g_aUltGauge[nCntUltGauge].state = ULTGAUGE_NOMAL;
			}
			break;
		}
	}
	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffUltGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUltGauge = 0; nCntUltGauge < MAX_ULTGAUGE; nCntUltGauge++)
	{//ゲージの数だけ繰り返し
		switch (nCntUltGauge)
		{
		case 0:	//プレイヤー０の場合
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x - g_aUltGauge[nCntUltGauge].fWidth, g_aUltGauge[nCntUltGauge].pos.y - g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x, g_aUltGauge[nCntUltGauge].pos.y - g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x - g_aUltGauge[nCntUltGauge].fWidth, g_aUltGauge[nCntUltGauge].pos.y + g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x, g_aUltGauge[nCntUltGauge].pos.y + g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			break;
		case 1:	//プレイヤー１の場合
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x, g_aUltGauge[nCntUltGauge].pos.y - g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x + g_aUltGauge[nCntUltGauge].fWidth, g_aUltGauge[nCntUltGauge].pos.y - g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x, g_aUltGauge[nCntUltGauge].pos.y + g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x + g_aUltGauge[nCntUltGauge].fWidth, g_aUltGauge[nCntUltGauge].pos.y + g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			break;
		}
		// 頂点カラー
		pVtx[0].col = g_aUltGauge[nCntUltGauge].col;
		pVtx[1].col = g_aUltGauge[nCntUltGauge].col;
		pVtx[2].col = g_aUltGauge[nCntUltGauge].col;
		pVtx[3].col = g_aUltGauge[nCntUltGauge].col;

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffUltGauge->Unlock();

}
//=============================================================================
// タイトル画面
//=============================================================================
void DrawUltGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffUltGauge, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureUltGauge);

	for (int nCntUltGauge = 0; nCntUltGauge < MAX_ULTGAUGE; nCntUltGauge++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntUltGauge, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexUltGauge(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_GAUGE,
		&g_pTextureUltGauge);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ULTGAUGE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUltGauge,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffUltGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUltGauge = 0; nCntUltGauge < MAX_ULTGAUGE; nCntUltGauge++)
	{
		switch (nCntUltGauge)
		{
		case 0:
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x - g_aUltGauge[nCntUltGauge].fWidth, g_aUltGauge[nCntUltGauge].pos.y - g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x, g_aUltGauge[nCntUltGauge].pos.y - g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x - g_aUltGauge[nCntUltGauge].fWidth, g_aUltGauge[nCntUltGauge].pos.y + g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x, g_aUltGauge[nCntUltGauge].pos.y + g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
			break;
		case 1:
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x, g_aUltGauge[nCntUltGauge].pos.y - g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x + g_aUltGauge[nCntUltGauge].fWidth, g_aUltGauge[nCntUltGauge].pos.y - g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x, g_aUltGauge[nCntUltGauge].pos.y + g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x + g_aUltGauge[nCntUltGauge].fWidth, g_aUltGauge[nCntUltGauge].pos.y + g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);

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
		pVtx[0].col = g_aUltGauge[nCntUltGauge].col;
		pVtx[1].col = g_aUltGauge[nCntUltGauge].col;
		pVtx[2].col = g_aUltGauge[nCntUltGauge].col;
		pVtx[3].col = g_aUltGauge[nCntUltGauge].col;

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffUltGauge->Unlock();
}

//=============================================================================
// 必殺ゲージの設置処理
//=============================================================================
void SetUltGauge(float fWidth, int nType)
{
	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	g_aUltGauge[nType].fWidth += fWidth;	//幅の加算処理

	if (g_aUltGauge[nType].fWidth >= ULTGAUGE_WIDTH)
	{//ゲージが最大値まで達した場合
		g_aUltGauge[nType].fWidth = ULTGAUGE_WIDTH;	//ゲージを固定させる
		g_aUltGauge[nType].state = ULTGAUGE_READY;		//使用可能状態にする
	}

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffUltGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntUltGauge = 0; nCntUltGauge < MAX_ULTGAUGE; nCntUltGauge++)
	{//ゲージの数だけ繰り返し
		switch (nCntUltGauge)
		{
		case 0:	//プレイヤー０の場合
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x - g_aUltGauge[nCntUltGauge].fWidth, g_aUltGauge[nCntUltGauge].pos.y - g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x, g_aUltGauge[nCntUltGauge].pos.y - g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x - g_aUltGauge[nCntUltGauge].fWidth, g_aUltGauge[nCntUltGauge].pos.y + g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x, g_aUltGauge[nCntUltGauge].pos.y + g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			break;
		case 1:	//プレイヤー１の場合
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x, g_aUltGauge[nCntUltGauge].pos.y - g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x + g_aUltGauge[nCntUltGauge].fWidth, g_aUltGauge[nCntUltGauge].pos.y - g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x, g_aUltGauge[nCntUltGauge].pos.y + g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aUltGauge[nCntUltGauge].pos.x + g_aUltGauge[nCntUltGauge].fWidth, g_aUltGauge[nCntUltGauge].pos.y + g_aUltGauge[nCntUltGauge].fHeight, g_aUltGauge[nCntUltGauge].pos.z);
			break;
		}
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffUltGauge->Unlock();
}

//=============================================================================
// 必殺ゲージ使用処理
//=============================================================================
void UseUltGauge(int nType)
{
	if (g_aUltGauge[nType].state == ULTGAUGE_READY)
	{//使用可能状態の場合
		//使用状態にする
		g_aUltGauge[nType].state = ULTGAUGE_USE;
	}
}

//=============================================================================
// 必殺ゲージ取得処理
//=============================================================================
ULTGAUGE_STATE GetUltGaugeState(int nType)
{
	return g_aUltGauge[nType].state;
}
float GetUltGaugeWidth(int nType)
{
	return g_aUltGauge[nType].fWidth;
}
void SetUltGaugeState(ULTGAUGE_STATE state, int nType)
{
	g_aUltGauge[nType].state = state;
}