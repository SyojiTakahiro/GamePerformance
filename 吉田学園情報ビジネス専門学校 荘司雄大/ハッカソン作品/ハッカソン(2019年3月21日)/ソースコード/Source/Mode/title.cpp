//*****************************************************************************
//
//     タイトルの処理[title.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mode.h"
#include "manager.h"
#include "system.h"
#include "functionlib.h"
#include "scene.h"
#include "2DPolygon.h"
#include "effect.h"
#include "textureManager.h"


//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define TITLE_SYSTEM_FILENAME    "data/TEXT/MODE/title.ini"   // タイトルの初期化情報スクリプトファイル名
#define TITLE_MODECHANGE_TIMING  (300)                        // 画面遷移を自動で行うタイミング

// 値を読み取るパス名
#define NUM_TEXTURE                 "NUM_TEXTURE = "            // 読み込むテクスチャの数
#define TEXTURE_FILENAME            "TEXTURE_FILENAME = "       // 読み込むテクスチャのファイル名
#define MAP_FILENAME				"MAP_FILENAME = "                             // マップデータのスクリプトファイル名
//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CTitle::CTitle()
{
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		m_pBg[nCnt] = NULL;        // 背景
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CTitle *CTitle::Create(void)
{
	CTitle *pTitle = NULL;  // タイトルクラスへのポインタ
	if (pTitle == NULL)
	{// メモリが確保できる状態である
		pTitle = new CTitle;
		if (pTitle != NULL)
		{// メモリが確保できた
			if (FAILED(pTitle->Init()))
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

	return pTitle;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CTitle::Init(void)
{
	CManager::GetSound()->PlaySound(0);

	// セッティングの初期化情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(TITLE_SYSTEM_FILENAME, "r");
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
			CTextureManager *pTextureManager = NULL;    // テクスチャ管轄クラスへのポインタ
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

					// 背景の生成
					m_pBg[1] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
					m_pBg[0] = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 300, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 600, 320);
					if (m_pBg != NULL)
					{
						m_pBg[0]->BindTexture(pTextureManager->GetTexture(0));
						m_pBg[1]->BindTexture(pTextureManager->GetTexture(1));
					}

					CScene2DPress *pPress = NULL;
					pPress = CScene2DPress::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, 600.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 400.0f, 100.0f);
					if (pPress != NULL)
					{
						pPress->BindTexture(pTextureManager->GetTexture(2));
					}
					SetPress(pPress);

					if (pTextureManager != NULL)
					{
						SetTextureManager(pTextureManager);
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
void CTitle::Uninit(void)
{
	// 共通の終了処理
	CBaseMode::Uninit();

	CManager::GetSound()->StopSound(0);
}

//=============================================================================
//    更新処理
//=============================================================================
void CTitle::Update(void)
{
	// 全てのオブジェクトを更新する
	CScene::UpdateAll();

	// 画面遷移チェック
	ModeChange(CManager::MODE_TITLE, CManager::MODE_SETTING, TITLE_MODECHANGE_TIMING);

	CDebugProc::Print("タイトル画面\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CTitle::Draw(void)
{

}