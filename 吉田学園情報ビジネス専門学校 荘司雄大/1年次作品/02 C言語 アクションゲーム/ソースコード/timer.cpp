//=============================================================================
//
// タイマー処理 [timer.cpp]
// Author : Takahiro
//
//=============================================================================
#include "timer.h"
#include "game.h"
#include "fade.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TIMER		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureTimer = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;		// 頂点バッファへのポインタ
int						g_nTimer = 30;		//
int						g_FrameTimer = 0;

//=============================================================================
// 初期化処理
//=============================================================================
void InitTimer(void)
{
	int nCntTimer;
	float fTimer;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// タイマーの初期化
	g_nTimer = 30;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
		TEXTURE_TIMER,								// ファイルの名前
		&g_pTextureTimer);							// テクスチャへのポインタ

	// 頂点情報の作成
	//MakeVertexScore(pDevice);

	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_TIMER,
		&g_pTextureTimer
	);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL
	);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロック
	g_pVtxBuffTimer->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntTimer = 0, fTimer = 900; nCntTimer < 3; nCntTimer++, fTimer -= 30)
	{

		//頂点設定
		pVtx[0].pos = D3DXVECTOR3(fTimer, 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fTimer + 30.0f, 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fTimer, 80.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fTimer + 30.0f, 80.0f, 0.0f);
		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラー設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		//テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//頂点バッファをアンロック
		pVtx += 4;
	}
	g_pVtxBuffTimer->Unlock();
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

	// 頂点バッファの開放
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
	int nCntTimer;
	g_FrameTimer++;
	int nAnswer;
	int nNmber = 1;

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	if (g_FrameTimer >= 60)
	{
		g_nTimer--;
		g_FrameTimer = 0;
	}

	//頂点バッファをロック
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < 3; nCntTimer++)
	{
		nAnswer = g_nTimer / nNmber;

		//テクスチャ設定
		pVtx[nCntTimer * 4].tex = D3DXVECTOR2((nAnswer % 10)*0.1f, 0.0f);
		pVtx[nCntTimer * 4 + 1].tex = D3DXVECTOR2((nAnswer % 10)*0.1f + 0.1f, 0.0f);
		pVtx[nCntTimer * 4 + 2].tex = D3DXVECTOR2((nAnswer % 10)*0.1f, 1.0f);
		pVtx[nCntTimer * 4 + 3].tex = D3DXVECTOR2((nAnswer % 10)*0.1f + 0.1f, 1.0f);
		nNmber *= 10;

	}

	if (g_nTimer <= 0)
	{
		g_nTimer = 0;
		GAMESTATE pGameState;
		pGameState = *GetGameState();

		StopSound(SOUND_LABEL_BGM001);

		if (pGameState != GAMESTATE_END)
		{
			SetGameState(GAMESTATE_END);
			SetFade(MODE_RESULT);
		}
	}
	g_pVtxBuffTimer->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTimer(void)
{
	int nCntTimer;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTimer);

	// ポリゴンの描画
	for (nCntTimer = 0; nCntTimer < 3; nCntTimer++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTimer * 4, 2);
	}
}

