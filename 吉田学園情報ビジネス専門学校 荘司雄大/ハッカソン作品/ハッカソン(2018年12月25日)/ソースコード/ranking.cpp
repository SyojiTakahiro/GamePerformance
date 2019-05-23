//*****************************************************************************
//
//     ランキングの処理[ranking.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "ranking.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "textureManager.h"
#include "functionlib.h"
#include "map.h"

#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define RANKING_SYSTEM_FILENAME    "data/TEXT/MODE/ranking.ini"                // ランキングの初期化情報スクリプトファイル名
#define RANKING_MODECHANGE_TIMING  (600)                                       // 画面遷移を自動で行うタイミング

// 値を読み取るパス名
#define NUM_TEXTURE                "NUM_TEXTURE = "                            // 読み込むテクスチャの数
#define TEXTURE_FILENAME           "TEXTURE_FILENAME = "                       // 読み込むテクスチャのファイル名
#define MAP_FILENAME               "MAP_FILENAME = "                           // マップデータのスクリプトファイル名

// ランキングスコア用のマクロ
#define RANKING_SCORE_POS_INI      (D3DXVECTOR3(1000.0f,180.0f,0.0f))          // ランキングスコアの座標(初期値)
#define RANKING_SCORE_POS_MAX      (1080.0f)                                   // ランキングスコアの座標の最大値
#define RANKING_SCORE_COL_INI      (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))            // ランキングスコアの色(初期値)
#define RANKING_SCORE_WIDTH_INI    (70.0f)                                     // ランキングスコアの幅(初期値)
#define RANKING_SCORE_HEIGHT_INI   (54.0f)                                     // ランキングスコアの高さ(初期値)
#define RANKING_SCORE_MOVE_INI     (D3DXVECTOR3(-15.0f,0.0f,0.0f))             // ランキングスコアの移動量(初期値)

// ランキング順位用のマクロ
#define RANKING_RANK_POS_INI       (D3DXVECTOR3(200.0f,180.0f,0.0f))           // ランキング順位の座標(初期値)
#define RANKING_RANK_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // ランキング順位の色(初期値)
#define RANKING_RANK_WIDTH_INI     (180.0f)                                    // ランキング順位の幅(初期値)
#define RANKING_RANK_HEIGHT_INI    (70.0f)                                     // ランキング順位の高さ(初期値)
#define RANKING_RANK_POS_MAX       (260.0f)                                    // ランキング順位の座標の最大値

// ランキングロゴ用のマクロ
#define RANKING_LOGO_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 2,65.0f,0.0f))  // ランキングロゴの座標(初期値)
#define RANKING_LOGO_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // ランキングロゴの色(初期値)
#define RANKING_LOGO_WIDTH_INI     (400.0f)                                    // ランキングロゴの幅(初期値)
#define RANKING_LOGO_HEIGHT_INI    (80.0f)                                     // ランキングロゴの高さ(初期値)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
CRanking::STATE CRanking::m_State[MAX_RANKING] = {};                           // ランキングスコアの状態
int CRanking::m_nRankingScore[MAX_RANKING] = {100000,80000,60000,40000,20000}; // ランキングのスコア

//=============================================================================
//    コンストラクタ
//=============================================================================
CRanking::CRanking(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数だけ繰り返し
		m_aDigit[nCntRanking] = 0;                            // ランキングスコアの桁数
		m_apNumber[nCntRanking] = NULL;                       // ランキングスコア表示用のポリゴン
		m_Move[nCntRanking] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ランキングスコアの移動量
		m_apRankingRank[nCntRanking] = NULL;                  // ランキング順位表示用のポリゴン
	}
	m_pMap = NULL;              // マップクラスへのポインタ
	m_pTextureManager = NULL;   // テクスチャ管轄クラスへのポインタ
	m_pRankingLogo = NULL;      // ランキングロゴ用表示用のポリゴン
	m_nCounter = 0;             // 遷移を管理するカウンター
}

//=============================================================================
//    デストラクタ
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CRanking *CRanking::Create(void)
{
	CRanking *pRanking = NULL;  // ランキングクラス型のポインタ
	if (pRanking == NULL)
	{// メモリが確保できる状態にある
		pRanking = new CRanking;
		if (pRanking != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pRanking ->Init();
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
	return pRanking;
}

//=============================================================================
//    ランキングスコアの初期化
//=============================================================================
void CRanking::RankingScoreInit(void)
{

}

//=============================================================================
//    ランキングスコアの設定
//=============================================================================
void CRanking::SetRankingScore(int nScore)
{
	int nScoreChange = nScore; // 入れ替えるスコア
	int nScoreOld = 0;         // ランキングスコアの一時的な格納場所
	bool bSet = false;         // ランキングが更新されたかどうか

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングの桁数分繰り返し
		if (m_nRankingScore[nCntRanking] < nScoreChange)
		{// 登録されているスコアより入れ替えるスコアが大きい
			nScoreOld = m_nRankingScore[nCntRanking];    // 元のスコアを格納
			m_nRankingScore[nCntRanking] = nScoreChange; // スコア更新
			nScoreChange = nScoreOld;                    // 元のスコアを入れ替える対象に
			if (bSet == false)
			{// 更新されていなかったら
				m_State[nCntRanking] = STATE_UPDATE;     // 更新された状態に
				bSet = true;                             // 更新した状態に
			}
		}
	}
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CRanking::Init(void)
{
	// ランキングの初期化情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(RANKING_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                         // 1行分読み取り用
			char *pStrCur = NULL;                       // 現在の先頭ポインタ
			char *pStr = NULL;                          // 先頭ポインタ保存用
			int nNumTex = 0;                            // テクスチャを読み込む数
			int nCntTex = 0;                            // テクスチャを読み込んだ回数
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ読み取り用
			char aTexFileName[256] = "\0";              // テクスチャのファイル名
			char aMapFileName[256] = "\0";              // マップデータのスクリプトファイル名

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
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// テクスチャの個数情報があった
								nNumTex = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTex >= 1)
								{// モデル数が1つ以上ある
									m_pTextureManager = CTextureManager::Create(nNumTex);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// テクスチャのファイルパス名情報があった
							    // テクスチャのファイルパス名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, TEXTURE_FILENAME);

								// テクスチャの読み込み
								D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);
								if (pTexture != NULL)
								{// テクスチャを読み込めた
									m_pTextureManager->BindTexture(pTexture, nCntTex);
									m_pTextureManager->SetFileName(pStr, nCntTex);
									pTexture = NULL;
									nCntTex++;
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// マップデータのスクリプトファイル名だった
							    // マップデータのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(aMapFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}

					// マップの生成
					m_pMap = CMap::Create(aMapFileName);

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
			fclose(pFile);
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

	// ランキングスコアの生成
	D3DXVECTOR3 NumberPos = RANKING_SCORE_POS_INI;     // 座標
	D3DXCOLOR NumberCol = RANKING_SCORE_COL_INI;       // 色
	float fNumberWidth = RANKING_SCORE_WIDTH_INI;      // 幅
	float fNumberHeight = RANKING_SCORE_HEIGHT_INI;    // 高さ
	int nNumber = 0;                                   // 表示する数字の計算用
	CNumber::STATE NumberState = CNumber::STATE_NONE;  // 状態

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数だけ繰り返し
		// 状態の設定
		if (m_State[nCntRanking] == STATE_UPDATE)
		{// 更新された状態だったら
			NumberState = CNumber::STATE_FLASH;
		}

		// 桁数の計算
		m_aDigit[nCntRanking] = (int)log10f((float)m_nRankingScore[nCntRanking]) + 1;  // 桁数を求める
		if (m_aDigit[nCntRanking] <= 0) { m_aDigit[nCntRanking] = 1; }	               // 0以下のとき1にする
		m_apNumber[nCntRanking] = new CNumber*[m_aDigit[nCntRanking]];		           // 桁数分メモリ確保
		if (m_apNumber[nCntRanking] != NULL)
		{// メモリが確保されている
			for (int nCntScore = 0; nCntScore < m_aDigit[nCntRanking]; nCntScore++)
			{// ランキングスコアの桁数分繰り返し
				// 表示する数字の計算
				nNumber = m_nRankingScore[nCntRanking] % ((int)powf(10.0f, (float)nCntScore) * 10) / (int)powf(10.0f, (float)nCntScore);

				// 数字ポリゴンのインスタンス生成処理
				m_apNumber[nCntRanking][nCntScore] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, NumberState, nNumber,3,6);
				if (m_apNumber[nCntRanking][nCntScore] != NULL)
				{// メモリを確保できた
					m_apNumber[nCntRanking][nCntScore]->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_0));
				}

				// 横に進める
				NumberPos.x -= RANKING_SCORE_WIDTH_INI + (RANKING_SCORE_WIDTH_INI * 0.5f);
			}
		}
		// 下に進める
		NumberPos = RANKING_SCORE_POS_INI;
		//NumberPos.x += 500.0f * (nCntRanking + 1);
		NumberPos.y += (RANKING_SCORE_HEIGHT_INI + (RANKING_SCORE_HEIGHT_INI * 1.2f)) * (nCntRanking + 1);

		// 状態を戻しておく
		NumberState = CNumber::STATE_NONE;
	}

	// ランキング順位ポリゴンのインスタンス生成
	D3DXVECTOR3 RankPos = RANKING_RANK_POS_INI;   // 座標
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングの桁数分繰り返し
		if (m_apRankingRank[nCntRanking] == NULL)
		{// メモリが確保できる状態にある
			m_apRankingRank[nCntRanking] = CScene2D::Create(RankPos, RANKING_RANK_COL_INI, RANKING_RANK_WIDTH_INI, RANKING_RANK_HEIGHT_INI,0.0f,6);
			if (m_apRankingRank[nCntRanking] != NULL)
			{// メモリを確保できた
				m_apRankingRank[nCntRanking]->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_1));

				// テクスチャ座標をずらす
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apRankingRank[nCntRanking]->GetVtxBuff();
				if(pVtxBuff != NULL)
				{// 頂点バッファを取得できた
				    // 頂点情報
					VERTEX_2D *pVtx;

					// 頂点バッファをロックし,頂点データへのポインタを取得
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// テクスチャ座標
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + (0.2f * nCntRanking));
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + (0.2f * nCntRanking));
					pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f + (0.2f * nCntRanking));
					pVtx[3].tex = D3DXVECTOR2(1.0f, 0.2f + (0.2f * nCntRanking));

					// 頂点バッファをアンロックする
					pVtxBuff->Unlock();
				}
			}
			RankPos = RANKING_RANK_POS_INI;
			//RankPos.x += 500.0f * (nCntRanking + 1);
			RankPos.y += (RANKING_RANK_HEIGHT_INI + (RANKING_RANK_HEIGHT_INI * 0.75f)) * (nCntRanking + 1);
		}
	}

	// ランキングロゴの生成
	if (m_pRankingLogo == NULL)
	{
		m_pRankingLogo = CScene2D::Create(RANKING_LOGO_POS_INI, RANKING_LOGO_COL_INI, RANKING_LOGO_WIDTH_INI, RANKING_LOGO_HEIGHT_INI, 0.0f, 6);
		if (m_pRankingLogo != NULL)
		{// メモリを確保できた
			m_pRankingLogo->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_2));
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CRanking::Uninit(void)
{
	// マップクラスを開放する
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();

		delete m_pMap;
		m_pMap = NULL;
	}

	// テクスチャ管轄クラスを開放する
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// メモリの開放
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// ランキングスコアの破棄
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングスコアの数だけ繰り返し
		if (m_apNumber[nCntRanking] != NULL)
		{// メモリが確保されている
			for (int nCntDigit = 0; nCntDigit < m_aDigit[nCntRanking]; nCntDigit++)
			{// ランキングスコアの桁数分繰り返し
				if (m_apNumber[nCntRanking][nCntDigit] != NULL)
				{// メモリが確保されている
					m_apNumber[nCntRanking][nCntDigit]->Uninit();
					m_apNumber[nCntRanking][nCntDigit] = NULL;
				}
			}
			delete m_apNumber[nCntRanking];
			m_apNumber[nCntRanking] = NULL;
		}

		// 状態を戻しておく
		m_State[nCntRanking] = STATE_NONE;
	}

	// ランキングロゴの破棄
	if (m_pRankingLogo != NULL)
	{
		m_pRankingLogo->Uninit();
		m_pRankingLogo = NULL;
	}

	// ランキング順位の破棄
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// ランキングの桁数分繰り返し
		if (m_apRankingRank[nCntRanking] != NULL)
		{// メモリが確保されている
			m_apRankingRank[nCntRanking]->Uninit();
			m_apRankingRank[nCntRanking] = NULL;
		}
	}

	// ランキングオブジェクトの破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CRanking::Update(void)
{
	m_nCounter++;  // カウンターを進める

	if (m_nCounter >= RANKING_MODECHANGE_TIMING)
	{// カウンターが既定の値を超えた
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}
	else
	{// カウンターがまだ既定の値を超えていない
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// 決定ボタンが押された
				CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			}
			else
			{// キーボードの決定ボタンが押されていない
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_7; nCntButton++)
				{// 反応するボタン数の数だけ繰り返し
					if (CManager::GetJoyStick()->GetJoyStickDevice()->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// ボタンが押された
						CManager::GetFade()->SetFade(CManager::MODE_TITLE);
					}
				}
			}
		}
	}

	CDebugProc::Print("ランキング画面\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CRanking::Draw(void)
{

}