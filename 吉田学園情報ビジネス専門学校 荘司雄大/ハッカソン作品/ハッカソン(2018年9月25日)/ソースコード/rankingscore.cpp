//=============================================================================
//
// ランキングスコア処理 [rankingscore.cpp]
// Author : Niwa Hodaka
//
//=============================================================================
#include "rankingscore.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SCORE		    "data/TEXTURE/number000.png"	// 読み込むテクスチャファイル名
#define TEXTURE_SIZE            (35)                            // テクスチャのサイズ
#define MAX_RANKINGSCORENUMBER  (8)                             // ランキングスコアの桁数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexRankingScore(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRankingScore = NULL;     // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingScore = NULL;	   // 頂点バッファへのポインタ
D3DXVECTOR3             g_RankingScorePos;                 // 頂点座標
int                     g_TextureRankingScorePos;          // テクスチャX座標
RANKINGSCORE            g_aRankingScore[MAX_RANKINGSCORE]; // ランキングスコア

//=============================================================================
// 初期化処理
//=============================================================================
void InitRankingScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nRankingScore = 100000;
	g_RankingScorePos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 350, SCREEN_HEIGHT / 2 - 170, 0.0f);  // 座標の初期化
	for (int nCntRankingScore = 0; nCntRankingScore < MAX_RANKINGSCORE; nCntRankingScore++)
	{// ランキングスコアの数だけ繰り返し
		g_aRankingScore[nCntRankingScore].pos = g_RankingScorePos;                 // ランキングスコア座標を初期化
		g_aRankingScore[nCntRankingScore].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f); // ランキングスコアの色を初期化
		g_aRankingScore[nCntRankingScore].score = nRankingScore;                   // ランキングスコアを初期化
		g_aRankingScore[nCntRankingScore].nCounterCol = 0;                         // カウンターは初期化
		g_aRankingScore[nCntRankingScore].state = RANKINGSCORESTATE_NONE;          // 更新されていない状態に
		nRankingScore -= 20000;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,		// デバイスへのポインタ
		TEXTURE_SCORE,		                // ファイルの名前
		&g_pTextureRankingScore);	        // テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexRankingScore(pDevice);
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitRankingScore(void)
{
	// テクスチャの開放
	if (g_pTextureRankingScore != NULL)
	{
		g_pTextureRankingScore->Release();
		g_pTextureRankingScore = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffRankingScore != NULL)
	{
		g_pVtxBuffRankingScore->Release();
		g_pVtxBuffRankingScore = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRankingScore(void)
{
	// 頂点バッファを生成
	VERTEX_2D *pVtx;
	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRankingScore = 0; nCntRankingScore < MAX_RANKINGSCORE; nCntRankingScore++)
	{// ランキングスコアの数だけ繰り返し
		if (g_aRankingScore[nCntRankingScore].state == RANKINGSCORESTATE_UPDATE)
		{// 更新された状態だったら
			g_aRankingScore[nCntRankingScore].nCounterCol++;
			if (g_aRankingScore[nCntRankingScore].nCounterCol % 2 == 0)
			{// 一定時間がたったら
				if (g_aRankingScore[nCntRankingScore].col.b == 0.0f)
				{// 赤だったら
					g_aRankingScore[nCntRankingScore].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
				else
				{// 白だったら
					g_aRankingScore[nCntRankingScore].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
			}
		}
		else
		{// 通常の状態なら
			g_aRankingScore[nCntRankingScore].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		}
		for (int nCntRankingScore1 = 0; nCntRankingScore1 < MAX_RANKINGSCORENUMBER; nCntRankingScore1++)
		{// スコアの桁数だけ繰り返し
		 // 頂点カラー
			pVtx[0].col = g_aRankingScore[nCntRankingScore].col;
			pVtx[1].col = g_aRankingScore[nCntRankingScore].col;
			pVtx[2].col = g_aRankingScore[nCntRankingScore].col;
			pVtx[3].col = g_aRankingScore[nCntRankingScore].col;
			pVtx += 4;
		}
		// 頂点バッファをアンロックする
		g_pVtxBuffRankingScore->Unlock();
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawRankingScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankingScore, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRankingScore);

	// ポリゴンの描画
	for (int nCntRankingScore2 = 0; nCntRankingScore2 < MAX_RANKINGSCORENUMBER * MAX_RANKINGSCORE; nCntRankingScore2++)
	{// 表示するランキングスコアの桁数だけ繰り返し
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			(nCntRankingScore2 * 4),
			2);
	}
}
//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexRankingScore(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKINGSCORENUMBER * MAX_RANKINGSCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingScore,
		NULL);

	// 頂点バッファを生成
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	for (int nCntRankingScore1 = 0; nCntRankingScore1 < MAX_RANKINGSCORE; nCntRankingScore1++)
	{// ランキングスコアの数だけ繰り返し
		int RankingScore = 1;
		for (int nCntRankingScore2 = 0; nCntRankingScore2 < MAX_RANKINGSCORENUMBER; nCntRankingScore2++)
		{// ランキングスコアの桁数だけ繰り返し
			g_TextureRankingScorePos = g_aRankingScore[nCntRankingScore1].score % (RankingScore * 10) / RankingScore;
			// 頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_RankingScorePos.x - TEXTURE_SIZE, g_RankingScorePos.y - TEXTURE_SIZE, g_RankingScorePos.z);
			pVtx[1].pos = D3DXVECTOR3(g_RankingScorePos.x + TEXTURE_SIZE, g_RankingScorePos.y - TEXTURE_SIZE, g_RankingScorePos.z);
			pVtx[2].pos = D3DXVECTOR3(g_RankingScorePos.x - TEXTURE_SIZE, g_RankingScorePos.y + TEXTURE_SIZE, g_RankingScorePos.z);
			pVtx[3].pos = D3DXVECTOR3(g_RankingScorePos.x + TEXTURE_SIZE, g_RankingScorePos.y + TEXTURE_SIZE, g_RankingScorePos.z);

			// 頂点テクスチャ
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// 頂点カラー
			pVtx[0].col = g_aRankingScore[nCntRankingScore1].col;
			pVtx[1].col = g_aRankingScore[nCntRankingScore1].col;
			pVtx[2].col = g_aRankingScore[nCntRankingScore1].col;
			pVtx[3].col = g_aRankingScore[nCntRankingScore1].col;

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f + (g_TextureRankingScorePos * 0.1f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (g_TextureRankingScorePos * 0.1f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (g_TextureRankingScorePos * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (g_TextureRankingScorePos * 0.1f), 1.0f);

			pVtx += 4;
			g_RankingScorePos.x -= 70;
			RankingScore *= 10;
		}
		g_RankingScorePos.y += 105;
		g_RankingScorePos.x = SCREEN_WIDTH / 2 + 350;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffRankingScore->Unlock();
}
//=============================================================================
// ランキングスコアとゲームスコアの比較
//=============================================================================
void SetRankingScore(int nScore)
{
	// 頂点バッファを生成
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffRankingScore->Lock(0, 0, (void**)&pVtx, 0);

	for(int nCntRankingScore = 0;nCntRankingScore < MAX_RANKINGSCORE;nCntRankingScore++)
	{// ランキングスコアの数だけ繰り返し
		if (g_aRankingScore[nCntRankingScore].score < nScore)
		{// ハイスコアが更新された
			g_aRankingScore[nCntRankingScore].score = nScore;

			int RankingScore = 1;
			// 頂点情報を設定
			for (int nCntRankingScore1 = 0; nCntRankingScore1 < MAX_RANKINGSCORENUMBER; nCntRankingScore1++)
			{// スコアの桁数だけ繰り返し
				g_TextureRankingScorePos = g_aRankingScore[nCntRankingScore].score % (RankingScore * 10) / RankingScore;

				// テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2(0.0f + (g_TextureRankingScorePos * 0.1f), 0.0f);
				pVtx[1].tex = D3DXVECTOR2(0.1f + (g_TextureRankingScorePos * 0.1f), 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f + (g_TextureRankingScorePos * 0.1f), 1.0f);
				pVtx[3].tex = D3DXVECTOR2(0.1f + (g_TextureRankingScorePos * 0.1f), 1.0f);

				RankingScore *= 10;        // 桁をずらすため
				pVtx += 4;
			}
			g_aRankingScore[nCntRankingScore].state = RANKINGSCORESTATE_UPDATE;  // 更新された状態に

			// 頂点バッファをアンロックする
			g_pVtxBuffRankingScore->Unlock();
			break;
		}
		pVtx += 4 * MAX_RANKINGSCORENUMBER; // ランキングスコアの桁数分だけ進める
	}
}
//=============================================================================
// ランキングスコアの取得
//=============================================================================
RANKINGSCORE *GetRankingScore(void)
{
	return &g_aRankingScore[0];
}