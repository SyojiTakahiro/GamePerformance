//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : Niwa Hodaka
//
//=============================================================================
#include "ranking.h"
#include "rankingscore.h"
#include "rankingrank.h"
#include "rankinglogo.h"
#include "rankingscore.h"
#include "fade.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int        g_nCounterRanking;          // タイトル画面に遷移するタイミング

//=============================================================================
// 初期化処理
//=============================================================================
void InitRanking(void)
{
	// 順位初期化処理
	InitRankingRank();

	// ロゴ初期化処理
	InitRankingLogo();

	g_nCounterRanking = 0;  // カウンターを0に
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitRanking(void)
{
	// 順位終了処理
	UninitRankingRank();

	// ロゴ終了処理
	UninitRankingLogo();
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateRanking(void)
{
	// 順位更新処理
	UpdateRankingRank();

	// ロゴ更新処理
	UpdateRankingLogo();

	// スコア更新処理
	UpdateRankingScore();

	FADE fade;
	fade = GetFade();

	g_nCounterRanking++;

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

	// ランキングスコアの取得
	RANKINGSCORE *rankingscore;
	rankingscore = GetRankingScore();

	for (int nCntRankingScore = 0; nCntRankingScore < MAX_RANKINGSCORE; nCntRankingScore++, rankingscore++)
	{// ランキングスコアの数だけ繰り返し
		if (rankingscore->state == RANKINGSCORESTATE_UPDATE)
		{// 更新状態になっていたら
			rankingscore->state = RANKINGSCORESTATE_NONE;
			rankingscore->col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawRanking(void)
{
	// 順位描画処理
	DrawRankingRank();

	// ロゴ描画処理
	DrawRankingLogo();

	// スコア描画処理
	DrawRankingScore();
}