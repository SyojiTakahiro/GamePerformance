//*****************************************************************************
//
//     リザルトの処理[result.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "result.h"
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
#define RESULT_SYSTEM_FILENAME   "data/TEXT/MODE/result.ini"                  // リザルトの初期化情報スクリプトファイル名
#define RESULT_MODECHANGE_TIMING (600)                                        // 画面遷移を自動で行うタイミング

// 値を読み取るパス名
#define NUM_TEXTURE              "NUM_TEXTURE = "                             // 読み込むテクスチャの数
#define TEXTURE_FILENAME         "TEXTURE_FILENAME = "                        // 読み込むテクスチャのファイル名
#define MAP_FILENAME             "MAP_FILENAME = "                            // マップデータのスクリプトファイル名

// PressEnter用のマクロ
#define RESULT_PRESS_POS         (D3DXVECTOR3(SCREEN_WIDTH / 2,600.0f,0.0f))  // PressEnterの座標
#define RESULT_PRESS_COL         (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))             // PressEnterの色
#define RESULT_PRESS_WIDTH       (400.0f)                                     // PressEnterの幅
#define RESULT_PRESS_HEIGHT      (100.0f)                                     // PressEnterの高さ

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
CResult::RESULT CResult::m_Result = CResult::RESULT_GAMEOVER;

//=============================================================================
//    コンストラクタ
//=============================================================================
CResult::CResult(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pScene = NULL;
	m_nCounter = 0;             // 遷移を管理するカウンター
	m_pPress = NULL;            // PressEnter用の2Dポリゴンクラスへのポインタ
	m_pTextureManager = NULL;   // テクスチャ管轄クラスへのポインタ
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
	CResult *pResult = NULL;  // リザルトクラス型のポインタ
	if (pResult == NULL)
	{// メモリが確保できる状態にある
		pResult = new CResult;
		if (pResult != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pResult->Init();
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
	return pResult;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CResult::Init(void)
{
	// リザルトの初期化情報を外部から読み込む
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

	if (m_pScene == NULL)
	{
		m_pScene = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		if (m_pScene != NULL)
		{// メモリを確保できた
			if (m_Result == RESULT_CLEAR)
			{
				m_pScene->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_1));
			}
			else if (m_Result == RESULT_GAMEOVER)
			{
				m_pScene->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_2));
			}
		}
	}

	// PressEnterの作成
	if (m_pPress == NULL)
	{
		m_pPress = CScene2DPress::Create(RESULT_PRESS_POS, RESULT_PRESS_COL, RESULT_PRESS_WIDTH, RESULT_PRESS_HEIGHT, 0.0f, 80, 100, 4, 6);
		if (m_pPress != NULL)
		{// メモリを確保できた
			m_pPress->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_0));
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CResult::Uninit(void)
{
	// テクスチャ管轄クラスを開放する
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// メモリの開放
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// PressEnterの破棄
	if (m_pPress != NULL)
	{
		m_pPress->Uninit();
		m_pPress = NULL;
	}

	m_Result = RESULT_GAMEOVER;

	// リザルトオブジェクトの破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CResult::Update(void)
{
	m_nCounter++;  // カウンターを進める

	if (m_nCounter >= RESULT_MODECHANGE_TIMING)
	{// カウンターが既定の値を超えた
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			CManager::GetFade()->SetFade(CManager::MODE_RANKING);
		}
	}
	else
	{// カウンターがまだ既定の値を超えていない
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// 決定ボタンが押された
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
				CManager::GetFade()->SetFade(CManager::MODE_RANKING);
				if (m_pPress != NULL)
				{// メモリが確保されている
					m_pPress->FlashStart();
				}
			}
			else
			{// キーボードの決定ボタンが押されていない
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_7; nCntButton++)
				{// 反応するボタン数の数だけ繰り返し
					if (CManager::GetJoyStick()->GetJoyStickDevice()->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// ボタンが押された
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
						CManager::GetFade()->SetFade(CManager::MODE_RANKING);
						if (m_pPress != NULL)
						{// メモリが確保されている
							m_pPress->FlashStart();
						}
					}
				}
			}
		}
	}

	CDebugProc::Print("リザルト画面\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CResult::Draw(void)
{

}

//=============================================================================
//    結果を設定する処理
//=============================================================================
void CResult::SetResult(RESULT result)
{
	m_Result = result;
}

//=============================================================================
//    結果を設定する処理
//=============================================================================
CResult::RESULT CResult::GetResult(void)
{
	return m_Result;
}