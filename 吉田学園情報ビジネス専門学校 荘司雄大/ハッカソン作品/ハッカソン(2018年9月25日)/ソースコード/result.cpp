//=============================================================================
//
// 結果画面処理 [result.cpp]
// Author : Niwa Hodaka
//
//=============================================================================
#include "result.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_PLAYER1			"data\\TEXTURE\\result00.png"	// ゲームオーバーテクスチャ
#define RESULT_PLAYER2			"data\\TEXTURE\\result01.png"	// ゲームクリアテクスチャ
#define RESULT_POS_X			(0)								// 背景の左上X座標
#define RESULT_POS_Y			(0)								// 背景の左上Y座標
#define RESULT_WIDTH			(SCREEN_WIDTH)					// 背景の幅
#define RESULT_HEIGHT			(SCREEN_HEIGHT)					// 背景の高さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
RESULT g_result = RESULT_PLAYER2_WIN;								// 結果画面を切り替える変数
int    g_nCounterResult;											// タイトル画面に遷移するタイミング
LPDIRECT3DTEXTURE9  g_pTextureResult[2] = {};						// テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;					// 頂点バッファへのポインタ

//=============================================================================
// 初期化処理
//=============================================================================
void InitResult(void)
{
	g_nCounterResult = 0;		// カウンターを0に

	LPDIRECT3DDEVICE9 pDevice;	// デバイスへのポインタ

								// デバイスの取得
	pDevice = GetDevice();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		RESULT_PLAYER1,
		&g_pTextureResult[0]);
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		RESULT_PLAYER2,
		&g_pTextureResult[1]);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,	// 確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	VERTEX_2D *pVtx;	// 頂点情報へのポインタ

	// 頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResult = 0; nCntResult < 2; nCntResult++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(RESULT_WIDTH, RESULT_POS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(RESULT_WIDTH, RESULT_HEIGHT, 0.0f);

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	// 頂点バッファをアンロック
	g_pVtxBuffResult->Unlock();
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{
	int nCntResult;

	for (nCntResult = 0; nCntResult < 2; nCntResult++)
	{
		// テクスチャの破棄
		if (g_pTextureResult[nCntResult] != NULL)
		{
			g_pTextureResult[nCntResult]->Release();
			g_pTextureResult[nCntResult] = NULL;
		}
	}
	// 頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	FADE fade;
	g_nCounterResult++;

	fade = GetFade();
	if (GetFade() == FADE_NONE)
	{// フェード状態ではない
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// 画面遷移
			SetFade(MODE_TITLE);
		}

		for (int nCntPad = 0; nCntPad < 2; nCntPad++)
		{
			for (int nCntButton = XIJS_BUTTON_4; nCntButton < XIJS_BUTTON_16; nCntButton++)
			{// 反応するボタン数の数だけ繰り返し
				if (GetXInputTrigger(nCntPad, (XIJS_BUTTON)nCntButton) == TRUE)
				{
					SetFade(MODE_TITLE);
				}
			}
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9	pDevice;	// デバイスへのポインタ

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResult[g_result]);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//=============================================================================
// 設定処理
//=============================================================================
void SetResult(RESULT result)
{
	g_result = result;
}
//=============================================================================
// 取得処理
//=============================================================================
RESULT GetResult(void)
{
	return g_result;
}