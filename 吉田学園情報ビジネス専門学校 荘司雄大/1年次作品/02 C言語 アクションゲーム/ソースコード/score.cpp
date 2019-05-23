//=============================================================================
//
// スコア処理 [score.cpp]
// Author : Takahiro
//
//=============================================================================
#include "score.h"
#include "fade.h"
#include "game.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
//void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		// 頂点バッファへのポインタ
int						g_nScore;					// スコア

//=============================================================================
// 初期化処理
//=============================================================================
void InitScore(void)
{
	int nCntScore;
	float fScore;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// スコアの初期化


	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
							TEXTURE_SCORE,			// ファイルの名前
							&g_pTextureScore);		// テクスチャへのポインタ

	// 頂点情報の作成
	//MakeVertexScore(pDevice);

	D3DXCreateTextureFromFile
	(
		pDevice,
		TEXTURE_SCORE,
		&g_pTextureScore
	);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_2D) * 4 * 8,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL
	);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロック
	g_pVtxBuffScore->Lock
	(
		0,
		0,
		(void**)&pVtx,
		0
	);

	for (nCntScore = 0, fScore = 1200; nCntScore < 8; nCntScore++, fScore -= 30)
	{

		//頂点設定
		pVtx[0].pos = D3DXVECTOR3(fScore, 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(fScore + 30.0f, 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(fScore, 80.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(fScore + 30.0f, 80.0f, 0.0f);
		//1.0fで固定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラー設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 0, 255);
		//テクスチャ設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//頂点バッファをアンロック
		pVtx += 4;
	}
	g_pVtxBuffScore->Unlock();
	g_nScore = 0;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitScore(void)
{
	// テクスチャの開放
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateScore(void)
{
	int nMemory;
	int nCntScore;
	nMemory = g_nScore;

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロック
	g_pVtxBuffScore->Lock( 0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < 8; nCntScore++)
	{
		//テクスチャ設定
		pVtx[nCntScore * 4].tex = D3DXVECTOR2((nMemory % 10)*0.1f, 0.0f);
		pVtx[nCntScore * 4 + 1].tex = D3DXVECTOR2((nMemory % 10)*0.1f + 0.1f, 0.0f);
		pVtx[nCntScore * 4 + 2].tex = D3DXVECTOR2((nMemory % 10)*0.1f, 1.0f);
		pVtx[nCntScore * 4 + 3].tex = D3DXVECTOR2((nMemory % 10)*0.1f + 0.1f, 1.0f);
		nMemory /= 10;
	}

	if (g_nScore == 1700)
	{
		GAMESTATE pGameState;
		pGameState = *GetGameState();
		StopSound(SOUND_LABEL_BGM001);
		if (pGameState != GAMESTATE_END)
		{
			
			SetGameState(GAMESTATE_END);
			SetFade(MODE_CLEAR);
		}
	}
	g_pVtxBuffScore->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawScore(void)
{
	int nCntScore;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureScore);

	// ポリゴンの描画
	for (nCntScore = 0; nCntScore < 8; nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
	}
}

////=============================================================================
//// 頂点情報の作成
////=============================================================================
//void MakeVertexScore(LPDIRECT3DDEVICE9 pDevice)
//{
//	// テクスチャの開放
//	
//
//	// 頂点バッファの開放
//	
//
//}

//=============================================================================
// スコアの加算
//=============================================================================
void AddScore(int nValue)
{
	g_nScore += nValue;
}
