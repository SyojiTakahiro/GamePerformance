//=============================================================================
//
// タイム処理 [timer.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "game.h"
#include "result.h"
#include "timer.h"
#include "fade.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define	TEXTURE_TIMER		"data/TEXTURE/number003.png"	// 読み込むテクスチャファイル名
#define TEXTURE_SIZE        (30)                            // テクスチャのサイズ
#define MAX_TIMERNUMBER     (3)                             // スコアの桁数
#define MAX_TIMER			(999)							// タイマーの最大値
//=============================================================================
// プロトタイプ宣言
//=============================================================================
void MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice);

//=============================================================================
// グローバル変数
//=============================================================================
LPDIRECT3DTEXTURE9		g_pTextureTimer = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;		// 頂点バッファへのポインタ
D3DXVECTOR3             g_TimerPos;                 // 頂点座標
TIMERSTATE              g_TimerState;               // タイマーの状態
int                     g_TexturePosTimer;          // テクスチャX座標
int                     g_nTimer;                   // タイマー
int                     g_nCounterTimer;            // タイマーを減らすタイミング
bool					g_bStopTimer;				//タイマーを止める

//=============================================================================
// 初期化処理
//=============================================================================
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nTimer = MAX_TIMER;							// タイマーの初期化
	g_bStopTimer = false;							// 止まっていない状態にする
	g_TimerPos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 50, 50.0f, 0.0f);		// 座標の初期化

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_TIMER,		                        // ファイルの名前
		&g_pTextureTimer);	                        // テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexTimer(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTimer(void)
{
	// テクスチャの開放
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimer(void)
{
	//フェードの取得
	FADE fade;
	fade = GetFade();

	if (g_bStopTimer == TIMERSTATE_COUNT)
	{// タイマーを進める状態だったら
		int time = 1;
		g_nCounterTimer++;
		if (g_nCounterTimer % 60 == 0)
		{// 一秒間たったら
			g_nTimer--;
			if (g_nTimer >= 0)
			{
				VERTEX_2D *pVtx;

				// 頂点バッファをロックし,頂点データへのポインタを取得
				g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

				for (int nCntTimer = 0; nCntTimer < MAX_TIMERNUMBER; nCntTimer++)
				{// タイマーの桁数だけ繰り返し
					g_TexturePosTimer = g_nTimer % (time * 10) / time;

					// テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2(0.0f + (g_TexturePosTimer * 0.1f), 0.0f);
					pVtx[1].tex = D3DXVECTOR2(0.1f + (g_TexturePosTimer * 0.1f), 0.0f);
					pVtx[2].tex = D3DXVECTOR2(0.0f + (g_TexturePosTimer * 0.1f), 1.0f);
					pVtx[3].tex = D3DXVECTOR2(0.1f + (g_TexturePosTimer * 0.1f), 1.0f);

					time *= 10;
					pVtx += 4;
				}

				// 頂点バッファをアンロックする
				g_pVtxBuffTimer->Unlock();
			}
			if (g_nTimer <= 30)
			{
				VERTEX_2D *pVtx;

				// 頂点バッファをロックし,頂点データへのポインタを取得
				g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);
				for (int nCntTimer = 0; nCntTimer < MAX_TIMERNUMBER; nCntTimer++)
				{// タイマーの桁数だけ繰り返し
					// 頂点カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

					pVtx += 4;
				}
				// 頂点バッファをアンロックする
				g_pVtxBuffTimer->Unlock();
			}
			else if (g_nTimer < 0 && fade == FADE_NONE)
			{// タイマーが0になったら
				SetGameState(GAMESTATE_END);
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTimer);

	// ポリゴンの描画
	for (int nCntScore = 0; nCntScore < MAX_TIMERNUMBER; nCntScore++)
	{// 表示するタイマーの桁数だけ繰り返し
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntScore * 4,
			2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexTimer(LPDIRECT3DDEVICE9 pDevice)
{
	int time = 1;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIMERNUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);

	// 頂点バッファを生成
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点情報を設定
	for (int nCntTimer = 0; nCntTimer < MAX_TIMERNUMBER; nCntTimer++)
	{// タイマーの桁数だけ繰り返し
		g_TexturePosTimer = g_nTimer % (time * 10) / time;

		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_TimerPos.x - TEXTURE_SIZE, g_TimerPos.y - TEXTURE_SIZE, g_TimerPos.z);
		pVtx[1].pos = D3DXVECTOR3(g_TimerPos.x + TEXTURE_SIZE, g_TimerPos.y - TEXTURE_SIZE, g_TimerPos.z);
		pVtx[2].pos = D3DXVECTOR3(g_TimerPos.x - TEXTURE_SIZE, g_TimerPos.y + TEXTURE_SIZE, g_TimerPos.z);
		pVtx[3].pos = D3DXVECTOR3(g_TimerPos.x + TEXTURE_SIZE, g_TimerPos.y + TEXTURE_SIZE, g_TimerPos.z);


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

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + (g_TexturePosTimer * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (g_TexturePosTimer * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (g_TexturePosTimer * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (g_TexturePosTimer * 0.1f), 1.0f);

		pVtx += 4;
		g_TimerPos.x -= 50;
		time *= 10;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTimer->Unlock();
}

//=============================================================================
// タイマーの取得
//=============================================================================
void StopTimerState(void)
{
	g_bStopTimer = TIMERSTATE_NONE_COUNT;
}