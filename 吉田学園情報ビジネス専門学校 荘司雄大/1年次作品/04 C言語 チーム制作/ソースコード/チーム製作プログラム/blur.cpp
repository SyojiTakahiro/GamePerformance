//=============================================================================
//
// ブラーの処理 [gauge.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "blur.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_GAUGE000		"data/TEXTURE/集中1.jpg"	// 読み込むテクスチャファイル名
#define TEXTURE_GAUGE001		"data/TEXTURE/集中2.jpg"		// 読み込むテクスチャフェイル名
#define TEXTURE_GAUGE002		"data/TEXTURE/集中2.jpg"		// 読み込むテクスチャフェイル名
#define TEXTURE_GAUGE003		"data/TEXTURE/集中2.jpg"			// 読み込むテクスチャフェイル名
#define MAX_GAUGE				(3)									// ゲージの最大数	
#define REDUCE_GAUGE			(0.12f)								// ゲージの減る量(毎フレーム)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexBlur(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBlur[MAX_GAUGE] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlur = NULL;				// 頂点バッファへのポインタ
int nCouter;
float fAlpha1;
float fAlpha2;
float fAlpha3;
//=============================================================================
// 初期化処理
//=============================================================================
void InitBlur(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAUGE000, &g_pTextureBlur[0]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAUGE001, &g_pTextureBlur[1]);
	D3DXCreateTextureFromFile(pDevice, TEXTURE_GAUGE002, &g_pTextureBlur[2]);
	//D3DXCreateTextureFromFile(pDevice, TEXTURE_GAUGE003, &g_pTextureBlur[3]);

	// 頂点情報の作成
	MakeVertexBlur(pDevice);

	fAlpha1 = 0;
	fAlpha2 = 0;
	fAlpha3 = 0;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBlur(void)
{
	for (int nCntBlur = 0; nCntBlur < MAX_GAUGE; nCntBlur++)
	{
		// テクスチャの開放
		if (g_pTextureBlur[nCntBlur] != NULL)
		{
			g_pTextureBlur[nCntBlur]->Release();
			g_pTextureBlur[nCntBlur] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBlur != NULL)
	{
		g_pVtxBuffBlur->Release();
		g_pVtxBuffBlur = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlur(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBlur(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBlur, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntBlur = 0; nCntBlur < MAX_GAUGE; nCntBlur++)
	{// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBlur[nCntBlur]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBlur * 4, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexBlur(LPDIRECT3DDEVICE9 pDevice)
{

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GAUGE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlur,
		NULL);

	// 頂点情報の作成
	VERTEX_2D*pVtx;			//頂点情報へのポインタ

							//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBlur->Lock(0, 0, (void**)&pVtx, 0);
	
	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++)
	{
		if (nCntGauge == 0)
		{	// ゲージ
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_WIDTH, 0.0f);

			//頂点カラー設定
			pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.7f);
			pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.7f);
			pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.7f);
			pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.7f);
		}
		else if (nCntGauge == 1)
		{	// 電池
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_WIDTH, 0.0f);

			//頂点カラー設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);
		}
		else if (nCntGauge == 2)
		{	// バッテリーメモリ
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_WIDTH, 0.0f);

			//頂点カラー設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを４つ分進める
	}
	//頂点バッファのアンロック
	g_pVtxBuffBlur->Unlock();
}
