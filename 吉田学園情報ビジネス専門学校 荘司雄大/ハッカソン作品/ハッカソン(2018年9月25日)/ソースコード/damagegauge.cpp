//=============================================================================
//
// ダメージゲージ処理 [damagegauge.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "damagegauge.h"
#include "lifegauge.h"
#include "main.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_DAMAGEGAUGE		"data/TEXTURE/DAMAGE_GAUGE.png"		// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexDamageGauge(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureDamageGauge = NULL;				// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDamageGauge = NULL;				// 頂点バッファへのポインタ
DamageGauge				g_aDamageGauge[MAX_DAMAGEGAUGE] = {};			// ゲージの情報
float					g_aCutWidth[MAX_DAMAGEGAUGE] = {};
//=============================================================================
// 初期化処理
//=============================================================================
void InitDamageGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float fGaugePos = 0;		//ゲージの位置

								// 値の初期化
	for (int nCntGauge = 0; nCntGauge < MAX_DAMAGEGAUGE; nCntGauge++)
	{//ゲージの数だけ繰り返し
		g_aDamageGauge[nCntGauge].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 152 + fGaugePos, 50.0f, 0.0f);	//位置の初期化
		g_aDamageGauge[nCntGauge].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);						//色の初期化
		g_aDamageGauge[nCntGauge].fWidth = LIFEGAUGE_WIDTH;											//幅の初期化
		g_aDamageGauge[nCntGauge].fHeight = LIFEGAUGE_HEIGHT;											//高さの初期化
		g_aDamageGauge[nCntGauge].nType = nCntGauge;												//タイプ
		fGaugePos += 305;
	}

	// 頂点情報の設定
	MakeVertexDamageGauge(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDamageGauge(void)
{
	// テクスチャの開放
	if (g_pTextureDamageGauge != NULL)
	{
		g_pTextureDamageGauge->Release();
		g_pTextureDamageGauge = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffDamageGauge != NULL)
	{
		g_pVtxBuffDamageGauge->Release();
		g_pVtxBuffDamageGauge = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDamageGauge(void)
{
	for (int nCntDamageGauge = 0; nCntDamageGauge < MAX_DAMAGEGAUGE; nCntDamageGauge++)
	{//ゲージの数だけ繰り返し
		switch (g_aDamageGauge[nCntDamageGauge].state)
		{
		case STATE_NOMAL:		//通常状態の場合
			break;
		case STATE_MINUS:		//減算状態の場合
			g_aDamageGauge[nCntDamageGauge].fWidth -= 0.5f;	//減算処理
			if (g_aDamageGauge[nCntDamageGauge].fWidth <= g_aCutWidth[nCntDamageGauge])
			{//幅が一致した場合
				g_aDamageGauge[nCntDamageGauge].fWidth = g_aCutWidth[nCntDamageGauge];	//幅の固定
			}
			break;
		}
	}
		VERTEX_2D *pVtx;   // 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点データへのポインタを取得
		g_pVtxBuffDamageGauge->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntGauge = 0; nCntGauge < MAX_DAMAGEGAUGE; nCntGauge++)
		{//ゲージの数だけ繰り返し
			switch (nCntGauge)
			{
			case 0:	//プレイヤー０の場合
					// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x - g_aDamageGauge[nCntGauge].fWidth, g_aDamageGauge[nCntGauge].pos.y - g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
				pVtx[1].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x, g_aDamageGauge[nCntGauge].pos.y - g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
				pVtx[2].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x - g_aDamageGauge[nCntGauge].fWidth, g_aDamageGauge[nCntGauge].pos.y + g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
				pVtx[3].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x, g_aDamageGauge[nCntGauge].pos.y + g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
				break;
			case 1:	//プレイヤー１の場合
					// 頂点座標
				pVtx[0].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x, g_aDamageGauge[nCntGauge].pos.y - g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
				pVtx[1].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x + g_aDamageGauge[nCntGauge].fWidth, g_aDamageGauge[nCntGauge].pos.y - g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
				pVtx[2].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x, g_aDamageGauge[nCntGauge].pos.y + g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
				pVtx[3].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x + g_aDamageGauge[nCntGauge].fWidth, g_aDamageGauge[nCntGauge].pos.y + g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
				break;
			}
			pVtx += 4;
		}
	// 頂点バッファをアンロックする
	g_pVtxBuffDamageGauge->Unlock();
}
//=============================================================================
// タイトル画面
//=============================================================================
void DrawDamageGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffDamageGauge, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureDamageGauge);

	for (int nCntGauge = 0; nCntGauge < MAX_DAMAGEGAUGE; nCntGauge++)
	{
		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntGauge, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexDamageGauge(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_DAMAGEGAUGE,
		&g_pTextureDamageGauge);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_DAMAGEGAUGE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDamageGauge,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

					   // 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffDamageGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_DAMAGEGAUGE; nCntGauge++)
	{
		switch (nCntGauge)
		{
		case 0:
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x - g_aDamageGauge[nCntGauge].fWidth, g_aDamageGauge[nCntGauge].pos.y - g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x, g_aDamageGauge[nCntGauge].pos.y - g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x - g_aDamageGauge[nCntGauge].fWidth, g_aDamageGauge[nCntGauge].pos.y + g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x, g_aDamageGauge[nCntGauge].pos.y + g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
			break;
		case 1:
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x, g_aDamageGauge[nCntGauge].pos.y - g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x + g_aDamageGauge[nCntGauge].fWidth, g_aDamageGauge[nCntGauge].pos.y - g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x, g_aDamageGauge[nCntGauge].pos.y + g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aDamageGauge[nCntGauge].pos.x + g_aDamageGauge[nCntGauge].fWidth, g_aDamageGauge[nCntGauge].pos.y + g_aDamageGauge[nCntGauge].fHeight, g_aDamageGauge[nCntGauge].pos.z);

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
		pVtx[0].col = g_aDamageGauge[nCntGauge].col;
		pVtx[1].col = g_aDamageGauge[nCntGauge].col;
		pVtx[2].col = g_aDamageGauge[nCntGauge].col;
		pVtx[3].col = g_aDamageGauge[nCntGauge].col;

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffDamageGauge->Unlock();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void SetDamageGaugeState(DAMAGE_STATE state, int nType,float fWidth)
{
	g_aDamageGauge[nType].state = state;
	g_aCutWidth[nType] = fWidth;
}