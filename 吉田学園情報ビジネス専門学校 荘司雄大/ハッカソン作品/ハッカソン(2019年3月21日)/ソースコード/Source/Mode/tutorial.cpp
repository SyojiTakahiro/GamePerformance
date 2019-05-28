//*****************************************************************************
//
//     チュートリアルの処理[tutorial.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mode.h"
#include "manager.h"
#include "system.h"
#include "functionlib.h"
#include "scene.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define TUTORIAL_SYSTEM_FILENAME    "data/TEXT/MODE/tutorial.ini"   // チュートリアルの初期化情報スクリプトファイル名
#define TUTORIAL_MODECHANGE_TIMING  (300)                           // 画面遷移を自動で行うタイミング

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CTutorial::CTutorial()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CTutorial *CTutorial::Create(void)
{
	CTutorial *pTutorial = NULL;  // チュートリアルクラスへのポインタ
	if (pTutorial == NULL)
	{// メモリが確保できる状態である
		pTutorial = new CTutorial;
		if (pTutorial != NULL)
		{// メモリが確保できた
			if (FAILED(pTutorial->Init()))
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

	return pTutorial;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CTutorial::Init(void)
{
	// チュートリアルの初期化情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(TUTORIAL_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                         // 1行分読み取り用
			char *pStrCur = NULL;                       // 現在の先頭ポインタ
			char *pStr = NULL;                          // 先頭ポインタ保存用

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
							if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
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
void CTutorial::Uninit(void)
{
	// 共通の終了処理
	CBaseMode::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CTutorial::Update(void)
{
	// 全てのオブジェクトを更新する
	CScene::UpdateAll();

	// 画面遷移チェック
	ModeChange(CManager::MODE_TITLE, CManager::MODE_TITLE, TUTORIAL_MODECHANGE_TIMING);

	CDebugProc::Print("チュートリアル画面\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CTutorial::Draw(void)
{

}