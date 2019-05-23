//*****************************************************************************
//
//     チュートリアルの処理[tutorial.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "ui.h"
#include "timer.h"
#include "score.h"
#include "functionlib.h"
#include "manager.h"
#include "renderer.h"
#include "textureManager.h"
#include "timer.h"
#include "score.h"
#include "stock.h"
#include "present.h"

#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define GAME_SYSTEM_FILENAME   "data/TEXT/MODE/game.ini"              // ゲームの初期化情報スクリプトファイル名
#define NUMBER_TEXTURE		   "data/TEXTURE/COMMON/number.png"

#define UI_NUM_TEX			 (1)		// テクスチャ数

#define UI_TEX_NUMBER_0		 (0)		// テクスチャ番号
#define UI_TEX_NUMBER_1		 (1)		// テクスチャ番号
#define UI_TEX_NUMBER_2		 (2)		// テクスチャ番号

// タイム用のマクロ
#define UI_TIMER_POS         (D3DXVECTOR3(720.0f,60.0f,0.0f))       // タイマーの座標
#define UI_TIMER_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))       // タイマーの色
#define UI_TIMER_WIDTH       (40.0f)                                // タイマーの数字ポリゴン１つ分の幅
#define UI_TIMER_HEIGHT      (40.0f)                                // タイマーの数字ポリゴン１つ分の高さ
#define UI_TIMER_WARNING     (30 * 60)                              // タイマーが赤くなるタイミング

// スコア用のマクロ
#define UI_SCORE_POS         (D3DXVECTOR3(1200.0f,60.0f,0.0f))      // スコアの座標
#define UI_SCORE_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))       // スコアの色
#define UI_SCORE_WIDTH       (30.0f)                                // スコアの数字ポリゴン１つ分の幅
#define UI_SCORE_HEIGHT      (30.0f)                                // スコアの数字ポリゴン１つ分の高さ

// 残機用のマクロ
#define UI_STOCK_POS         (D3DXVECTOR3(120.0f,60.0f,0.0f))       // 残機の座標
#define UI_STOCK_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))       // 残機の色
#define UI_STOCK_WIDTH       (30.0f)                                // 残機の数字ポリゴン１つ分の幅
#define UI_STOCK_HEIGHT      (30.0f)                                // 残機の数字ポリゴン１つ分の高さ
#define UI_STOCK_NUMBER      (3)								    // 残機数

// プレゼント用のマクロ
#define UI_PRESENT_POS         (D3DXVECTOR3(420.0f,60.0f,0.0f))     // プレゼントの座標
#define UI_PRESENT_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))     // プレゼントの色
#define UI_PRESENT_WIDTH       (30.0f)                              // プレゼントの数字ポリゴン１つ分の幅
#define UI_PRESENT_HEIGHT      (30.0f)                              // プレゼントの数字ポリゴン１つ分の高さ
#define UI_PRESENT_NUMBER      (5)								    // プレゼント数

#define GAME_TIMER             "GAME_TIMER = "             // タイマーの秒数

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CUI::CUI()
{
	// 各種値のクリア
	m_pScore = NULL;			// スコアクラスへのポインタ
	m_pTimer = NULL;			// タイマークラスへのポインタ
	m_pStock = NULL;			// 残機クラスへのポインタ
	m_pPresent = NULL;			// プレゼントクラスへのポインタ
	m_pTextureManager = NULL;	// テクスチャ管轄クラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CUI::~CUI()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CUI *CUI::Create(void)
{
	CUI *pUI = NULL;  // チュートリアルクラス型のポインタ
	if (pUI == NULL)
	{// メモリが確保できる状態にある
		pUI = new CUI;
		if (pUI != NULL)
		{// メモリが確保できた
		 // 初期化処理を行う
			pUI->Init();
		}
		else
		{// メモリが確保できなかった
			return NULL;
		}
	}
	else
	{// メモリが確保できる状態にない
		return NULL;
	}

	// インスタンスを返す
	return pUI;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CUI::Init(void)
{
	// ゲームの初期化情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(GAME_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		 // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                         // 1行分読み取り用
			char *pStrCur = NULL;                       // 現在の先頭ポインタ
			char *pStr = NULL;                          // 先頭ポインタ保存用
			int nTimer = 10 * 60;                       // タイマーの秒数
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ読み取り用

			if (pLine == NULL && pStr == NULL)
			{// メモリが確保できる状態である
				pLine = new char[256];
				pStr = new char[256];
				if (pLine != NULL && pStr != NULL)
				{// メモリを確保できた
				 // 文字列を初期化
					strcpy(pLine, "\0");
					strcpy(pStr, "\0");

					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// スクリプト読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, GAME_TIMER) == 0)
							{// タイマーの秒数だった
								nTimer = CFunctionLib::ReadInt(pStrCur, GAME_TIMER);
								nTimer *= 60;
								CTimer::SetInitTimer(nTimer);	// タイマーの初期値を設定
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}

					// テクスチャ管理クラスの生成
					m_pTextureManager = CTextureManager::Create(UI_NUM_TEX);

					if (m_pTextureManager != NULL)
					{
						D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), NUMBER_TEXTURE, &pTexture);
						if (pTexture != NULL)
						{// テクスチャを読み込めた
							m_pTextureManager->BindTexture(pTexture, 0);
							m_pTextureManager->SetFileName(NUMBER_TEXTURE, 0);
							pTexture = NULL;
						}
					}

					// 必要なクラスを作成する
					// タイマー
					m_pTimer = CTimer::Create(UI_TIMER_POS, UI_TIMER_COL, UI_TIMER_WIDTH, UI_TIMER_HEIGHT, nTimer, UI_TIMER_WARNING, UI_TIMER_PRIORITY);
					if (m_pTimer != NULL)
					{// タイマークラスが生成できた
						m_pTimer->SetTimerTexture(m_pTextureManager->GetTexture(UI_TEX_NUMBER_0));
					}

					// スコア
					m_pScore = CScore::Create(UI_SCORE_POS, UI_SCORE_COL, UI_SCORE_WIDTH, UI_SCORE_HEIGHT, UI_SCORE_PRIORITY);
					if (m_pScore != NULL)
					{// スコアクラスが生成できた
						m_pScore->SetScoreTexture(m_pTextureManager->GetTexture(UI_TEX_NUMBER_0));
					}

					// 残機
					m_pStock = CStock::Create(UI_STOCK_POS, UI_STOCK_COL, UI_STOCK_WIDTH, UI_STOCK_HEIGHT, UI_STOCK_NUMBER, UI_STOCK_PRIORITY);
					if (m_pStock != NULL)
					{// 残機クラスが生成できた
						m_pStock->SetStockTexture(m_pTextureManager->GetTexture(UI_TEX_NUMBER_0));
					}

					// プレゼント
					m_pPresent = CPresent::Create(UI_PRESENT_POS, UI_PRESENT_COL, UI_PRESENT_WIDTH, UI_PRESENT_HEIGHT, UI_PRESENT_NUMBER, UI_PRESENT_PRIORITY);
					if (m_pPresent != NULL)
					{// プレゼントクラスが生成できた
						m_pPresent->SetPresentTexture(m_pTextureManager->GetTexture(UI_TEX_NUMBER_0));
					}

					// メモリの開放
					if (pStr != NULL)
					{// メモリが確保されている
						delete[] pStr;
						pStr = NULL;
					}
					if (pLine != NULL)
					{// メモリが確保されている
						delete[] pLine;
						pLine = NULL;
					}
				}
				else
				{// メモリを確保できなかった
					return E_FAIL;
				}
			}
			else
			{// メモリを確保できる状態でない
				return E_FAIL;
			}
		}
		else
		{// ファイルポインタを確保できなかった
			return E_FAIL;
		}
	}
	else
	{// ファイルポインタを確保できる状態でない
		return E_FAIL;
	}
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CUI::Uninit(void)
{
	// テクスチャ管轄クラスを開放する
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// メモリの開放
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// タイマークラスへのポインタを空にする
	if (m_pTimer != NULL)
	{
		m_pTimer = NULL;
	}

	// スコアクラスへのポインタを空にする
	if (m_pScore != NULL)
	{
		m_pScore = NULL;
	}

	// スコアクラスへのポインタを空にする
	if (m_pStock != NULL)
	{
		m_pStock = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CUI::Update(void)
{
}

//=============================================================================
//    タイマーの取得
//=============================================================================
int CUI::GetTimer(void)
{
	return m_pTimer->GetTimer();
}

//=============================================================================
//    タイマーの設定
//=============================================================================
void CUI::SetTimer(int nTimer)
{
	m_pTimer->SetTimer(nTimer);
}

//=============================================================================
//    タイマーのリセット
//=============================================================================
void CUI::ResetTimer(void)
{
	m_pTimer->Reset();
}

//=============================================================================
//    スコアの取得
//=============================================================================
int CUI::GetScore(void)
{
	return m_pScore->GetScore();
}

//=============================================================================
//    スコアの設定
//=============================================================================
void CUI::SetScore(int nScore)
{
	m_pScore->SetScore(nScore);
}

//=============================================================================
//    残機の取得
//=============================================================================
int CUI::GetStock(void)
{
	return m_pStock->GetStock();
}

//=============================================================================
//    残機の設定
//=============================================================================
void CUI::SetStock(int nStock)
{
	m_pStock->SetStock(nStock);
}

//=============================================================================
//    プレゼントの取得
//=============================================================================
int CUI::GetPresent(void)
{
	return m_pPresent->GetPresent();
}

//=============================================================================
//    プレゼントの設定
//=============================================================================
void CUI::SetPresent(int nPresent)
{
	m_pPresent->SetPresent(nPresent);
}