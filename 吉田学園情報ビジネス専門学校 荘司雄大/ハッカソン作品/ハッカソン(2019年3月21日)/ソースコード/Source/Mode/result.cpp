//*****************************************************************************
//
//     リザルトの処理[result.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mode.h"
#include "manager.h"
#include "system.h"
#include "functionlib.h"
#include "scene.h"
#include "2DPolygon.h"
#include "textureManager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define RESULT_SYSTEM_FILENAME    "data/TEXT/MODE/result.ini"   // リザルトの初期化情報スクリプトファイル名
#define RESULT_MODECHANGE_TIMING  (300)                         // 画面遷移を自動で行うタイミング

// 値を読み取るパス名
#define NUM_TEXTURE                 "NUM_TEXTURE = "            // 読み込むテクスチャの数
#define NUM_MAP                     "NUM_MAP = "                // 読み込むマップの数
#define TEXTURE_FILENAME            "TEXTURE_FILENAME = "       // 読み込むテクスチャのファイル名
#define EFFECT_FILENAME             "EFFECT_FILENAME = "        // エフェクトデータのスクリプトファイル名
#define MAP_FILENAME                "MAP_FILENAME = "           // マップデータのスクリプトファイル名

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
int CResult::m_nNumPlayer = 0;
int CResult::m_nWinnerIdx = 0;


//=============================================================================
//    コンストラクタ
//=============================================================================
CResult::CResult()
{
	m_pBg[0] = NULL;
	m_pBg[1] = NULL;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CResult *CResult::Create(void)
{
	CResult *pResult = NULL;  // リザルトクラスへのポインタ
	if (pResult == NULL)
	{// メモリが確保できる状態である
		pResult = new CResult;
		if (pResult != NULL)
		{// メモリが確保できた
			if (FAILED(pResult->Init()))
				return NULL;
		}
		else
		{// メモリを確保できたなかった
			return NULL;
		}
	}
	else
	{// メモリを確保できる状態でない
		return NULL;
	}

	return pResult;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	CManager::GetSound()->PlaySound(3);

	// リザルト初期化情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(RESULT_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                         // 1行分読み取り用
			char *pStrCur = NULL;                       // 現在の先頭ポインタ
			char *pStr = NULL;                          // 先頭ポインタ保存用
			int nNumTex = 0;                            // テクスチャを読み込む数
			int nCntTex = 0;                            // テクスチャを読み込んだ回数
			int nCntPlayer = 0;                         // プレイヤーデータを読み込んだ回数
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ読み取り用
			char aTexFileName[256] = "\0";              // テクスチャのファイル名
			int nNumMap = 0;                            // マップデータの数
			int nCntMap = 0;                            // マップデータを読み込んだ数
			int nPlayerLife[2] = {};                    // プレイヤーの体力
			CTextureManager *pTextureManager = NULL;    // テクスチャ管轄クラスへのポインタ

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
									pTextureManager = CTextureManager::Create(nNumTex);
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
									pTextureManager->BindTexture(pTexture, nCntTex);
									pTextureManager->SetFileName(pStr, nCntTex);
									pTexture = NULL;
									nCntTex++;
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}

					// カメラの生成
					D3DVIEWPORT9 ViewPort;
					ViewPort.X = 0;
					ViewPort.Y = 0;
					ViewPort.Width = SCREEN_WIDTH;
					ViewPort.Height = SCREEN_HEIGHT;
					ViewPort.MaxZ = 1.0f;
					ViewPort.MinZ = 0.0f;
					SetCamera(CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, ViewPort, NULL), 0);


					m_pBg[0] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
					if (m_pBg[0] != NULL)
					{
						m_pBg[0]->BindTexture(pTextureManager->GetTexture(0));
					}

					m_pBg[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
					if (m_pBg[1] != NULL)
					{
						m_pBg[1]->BindTexture(pTextureManager->GetTexture(1 + m_nWinnerIdx));
					}

					SetTextureManager(pTextureManager);

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

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// 共通の終了処理
	CBaseMode::Uninit();

	m_nNumPlayer = 0;
	m_nWinnerIdx = 0;

	CManager::GetSound()->StopSound(3);
}

//=============================================================================
//    更新処理
//=============================================================================
void CResult::Update(void)
{
	// 全てのオブジェクトを更新する
	CScene::UpdateAll();

	// 画面遷移チェック
	ModeChange(CManager::MODE_TITLE, CManager::MODE_SETTING, RESULT_MODECHANGE_TIMING);

	CDebugProc::Print("リザルト画面\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CResult::Draw(void)
{

}

//=============================================================================
//    プレイヤーの人数を設定する
//=============================================================================
void CResult::SetNumPlayer(const int nNumPlayer)
{
	m_nNumPlayer = nNumPlayer;
}

//=============================================================================
//    勝ったほうのプレイヤーの番号を設定する
//=============================================================================
void CResult::SetWinnerIdx(const int nWinnerIdx)
{
	m_nWinnerIdx = nWinnerIdx;
}