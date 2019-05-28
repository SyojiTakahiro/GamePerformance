//*****************************************************************************
//
//     ゲームの処理[game.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mode.h"
#include "manager.h"
#include "system.h"
#include "functionlib.h"
#include "scene.h"
#include "character.h"
#include "effect.h"
#include "map.h"
#include "textureManager.h"
#include "pause.h"
#include "bullet.h"
#include "object.h"
#include "star.h"
#include "timer.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define PAUSE_PRIORITY         (10)
#define GAME_SYSTEM_FILENAME   "data/TEXT/MODE/game.ini"   // ゲームの初期化情報スクリプトファイル名
#define GAME_MODECHANGE_TIMING (300)                       // 画面遷移を自動で行うタイミング
#define GAME_MAX_MAP           (3)                         // マップデータの最大数

// 値を読み取るパス名
#define NUM_TEXTURE                 "NUM_TEXTURE = "            // 読み込むテクスチャの数
#define NUM_MAP                     "NUM_MAP = "                // 読み込むマップの数
#define TEXTURE_FILENAME            "TEXTURE_FILENAME = "       // 読み込むテクスチャのファイル名
#define EFFECT_FILENAME             "EFFECT_FILENAME = "        // エフェクトデータのスクリプトファイル名
#define MAP_FILENAME                "MAP_FILENAME = "           // マップデータのスクリプトファイル名

// プレイヤー情報
#define PLAYERSET                   "PLAYERSET"                 // プレイヤー情報読み込み開始の合図
#define END_PLAYERSET               "END_PLAYERSET"             // プレイヤー情報読み込み終了の合図
#define LIFE                        "LIFE = "                   // プレイヤーの体力
#define FILENAME                    "FILENAME = "               // プレイヤーデータのスクリプトファイル名

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
int CGame::m_nNumPlayer = 0;
int CGame::m_nGameMapIdx = 0;
CGame::PLAYER_SETTING CGame::m_PlayerSetting[2];

//=============================================================================
//    コンストラクタ
//=============================================================================
CGame::CGame()
{
	m_State = STATE_NORMAL;
	m_bPause = false;
	m_pPause = NULL;
	m_nStateCounter = 0;
	m_nPauseController = 0;

	m_pPlayer[0] = NULL;
	m_pPlayer[1] = NULL;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGame *CGame::Create(void)
{
	CGame *pGame = NULL;  // ゲームクラスへのポインタ
	if (pGame == NULL)
	{// メモリが確保できる状態である
		pGame = new CGame;
		if (pGame != NULL)
		{// メモリが確保できた
			if (FAILED(pGame->Init()))
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

	return pGame;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGame::Init(void)
{

	CManager::GetSound()->PlaySound(2);

	CShadow::Load();
	CBullet::Load();
	CStar::Load();

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
			int nNumTex = 0;                            // テクスチャを読み込む数
			int nCntTex = 0;                            // テクスチャを読み込んだ回数
			int nCntPlayer = 0;                         // プレイヤーデータを読み込んだ回数
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ読み取り用
			char aTexFileName[256] = "\0";              // テクスチャのファイル名
			char aPlayerFileName[2][256];               // プレイヤーデータのスクリプトファイル名
			char aEffectFileName[256];                  // エフェクトデータのスクリプトファイル名
			int nNumMap = 0;                            // マップデータの数
			int nCntMap = 0;                            // マップデータを読み込んだ数
			char aMapFileName[GAME_MAX_MAP][256];       // マップデータのスクリプトファイル名
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
							else if (CFunctionLib::Memcmp(pStrCur, PLAYERSET) == 0)
							{// プレイヤーデータ読み込み開始の合図だった
								while (1)
								{// ループ開始
									pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
									if (CFunctionLib::Memcmp(pStrCur, LIFE) == 0)
									{// 体力読み込みの合図だった
										nPlayerLife[nCntPlayer] = CFunctionLib::ReadInt(pStrCur, LIFE);
									}
									else if (CFunctionLib::Memcmp(pStrCur, FILENAME) == 0)
									{// プレイヤーデータのスクリプトファイル名だった
									    // プレイヤーデータのスクリプトファイル名を読み取る
										pStr = CFunctionLib::ReadString(pStrCur, pStr, FILENAME);
										strcpy(aPlayerFileName[nCntPlayer], pStr);
									}
									else if (CFunctionLib::Memcmp(pStrCur, END_PLAYERSET) == 0)
									{// プレイヤー情報読み込み終了の合図だった
										nCntPlayer++;
										break;
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, EFFECT_FILENAME) == 0)
							{// エフェクトデータのスクリプトファイル名だった
							    // エフェクトデータのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, EFFECT_FILENAME);
								strcpy(aEffectFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MAP) == 0)
							{// マップの個数情報があった
								nNumMap = CFunctionLib::ReadInt(pStrCur, NUM_MAP);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// マップデータのスクリプトファイル名だった
							    // マップデータのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(aMapFileName[nCntMap], pStr);
								nCntMap++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}

					// 各種クラスを生成する
					// プレイヤー管轄クラス
					for (int nCntPlayer = 0; nCntPlayer < m_nNumPlayer; nCntPlayer++)
					{// プレイヤーの数だけ繰り返し
						SetPlayerManager(CPlayerManager::Create(aPlayerFileName[nCntPlayer], m_PlayerSetting[nCntPlayer].nControllerIdx));
					}

					// エフェクト管轄クラス
					SetEffectManager(CEffectManager::Create(aEffectFileName));

					// マップ管轄クラス
					SetMap(CMap::Create(aMapFileName[0], GetEffectManager()));

					// リストからプレイヤークラスへポインタを取得しておく
					CScene *pScene = NULL;               // シーンクラスへのポインタ
					CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
					int nNumPlayer = 0;
					CPlayer *pPlayer[2];
					D3DXVECTOR3 Position[2] = { D3DXVECTOR3(0.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, 0.0f, 1000.0f) };
					D3DXVECTOR3 Rotation[2] = { D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f) };
					for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
					{// 描画優先順位の数だけ繰り返し
						pScene = CScene::GetTop(nCntPriority);
						while (pScene != NULL)
						{// メモリが空になるまで繰り返し
							pSceneNext = pScene->GetNext();
							if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
							{// プレイヤークラス型のポインタだったら
								m_pPlayer[nNumPlayer] = (CPlayer*)pScene;
								pPlayer[nNumPlayer] = (CPlayer*)pScene;
								pPlayer[nNumPlayer]->SetPos(Position[nNumPlayer]);
								pPlayer[nNumPlayer]->SetRot(Rotation[nNumPlayer]);
								pPlayer[nNumPlayer]->SetDestRot(Rotation[nNumPlayer]);
								nNumPlayer++;
								if (nNumPlayer >= m_nNumPlayer)
								{
									break;
								}
							}
							pScene = pSceneNext;
						}
					}

					// カメラを生成する
					//int nCntCamera = 0;
					//D3DVIEWPORT9 ViewPort;
					//ViewPort.X = 0;
					//ViewPort.Y = 0;
					//ViewPort.Height = SCREEN_HEIGHT;
					//ViewPort.MaxZ = 1.0f;
					//ViewPort.MinZ = 0.0f;
					//if (m_nNumPlayer == 2)
					//{// 必要なプレイヤーが２人だった
					//	ViewPort.Width = SCREEN_WIDTH / 2;
					//}
					//else if (m_nNumPlayer == 1)
					//{// 必要なプレイヤーが１人だった
					//	ViewPort.Width = SCREEN_WIDTH;
					//}

					//SetCamera(CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, ViewPort, pPlayer[nCntCamera]), nCntCamera);
					//nCntCamera++;

					//if (m_nNumPlayer == 2)
					//{// 必要なプレイヤーが２人だった
					//	ViewPort.X = SCREEN_WIDTH / 2;
					//	SetCamera(CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, ViewPort, pPlayer[nCntCamera]), nCntCamera);
					//	nCntCamera++;
					//}

					int nCntCamera = 0;
					D3DVIEWPORT9 ViewPort;
					ViewPort.X = 0;
					ViewPort.Y = 0;
					ViewPort.Width = SCREEN_WIDTH;
					ViewPort.MaxZ = 1.0f;
					ViewPort.MinZ = 0.0f;
					if (m_nNumPlayer == 2)
					{// 必要なプレイヤーが２人だった
						ViewPort.Height = SCREEN_HEIGHT / 2;
					}
					else if (m_nNumPlayer == 1)
					{// 必要なプレイヤーが１人だった
						ViewPort.Height = SCREEN_HEIGHT;
					}

					SetCamera(CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, ViewPort, pPlayer[nCntCamera]), nCntCamera);
					nCntCamera++;

					if (m_nNumPlayer == 2)
					{// 必要なプレイヤーが２人だった
						ViewPort.Y = SCREEN_HEIGHT / 2;
						SetCamera(CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, ViewPort, pPlayer[nCntCamera]), nCntCamera);
						nCntCamera++;
					}

					// テクスチャ管轄クラスを設定する
					if (pTextureManager != NULL)
					{
						SetTextureManager(pTextureManager);
					}

					// オブジェクト配置
					SetStarObject();

					// タイマーを生成
					CTimer *pTimer = NULL;
					pTimer = CTimer::Create(D3DXVECTOR3(SCREEN_WIDTH / 2 - 150.0f, 60.0f,0.0));
					if (pTimer != NULL)
					{
						pTimer->BindTexture(pTextureManager->GetTexture(4));
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

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CGame::Uninit(void)
{
	CShadow::UnLoad();
	CBullet::UnLoad();
	CStar::UnLoad();

	// 共通の終了処理
	CBaseMode::Uninit();

	if (m_bPause == false)
	{
		// プレイヤーの人数を戻しておく
		CResult::SetNumPlayer(m_nNumPlayer);
		m_nNumPlayer = 0;
	}

	CManager::GetSound()->StopSound(2);
}

//=============================================================================
//    更新処理
//=============================================================================
void CGame::Update(void)
{
	// カメラを更新する
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA_NUM; nCntCamera++)
	{// カメラの数だけ繰り返し
		if (GetCamera(nCntCamera) != NULL)
		{// メモリが確保されている
		    // カメラの更新処理
			GetCamera(nCntCamera)->Update();
		}
	}

	switch (m_State)
	{// 状態によって処理わけ
	case STATE_NORMAL:
		Normal();
		break;
	case STATE_END:
		End();
		break;
	}

	CDebugProc::Print("ゲーム画面\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
//    通常状態の処理
//=============================================================================
void CGame::Normal(void)
{
	CManager::MODE mode = CManager::GetMode();  // 現在のモードを取得

	if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
	{// フェードしていなかったら
		if (CManager::GetKeyboard()->GetTrigger(DIK_P) == true)
		{// ポーズON/OFF
			m_bPause = m_bPause ? false : true;
			if (m_pPause == NULL && m_bPause == true)
			{// ポーズ中にメモリが確保されていない
				m_pPause = CPause::Create(PAUSE_PRIORITY);
				//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPEN);
			}
			else if (m_pPause != NULL)
			{// ポーズ解除時にメモリが確保されている
				ReleasePause();
			}
		}
		for (int nCntStick = 0; nCntStick < 2; nCntStick++)
		{
			if (CManager::GetJoyStick()->GetJoyStickDevice(nCntStick)->GetTrigger(CJoyStick::DIJS_BUTTON_11) == TRUE)
			{
				if (m_pPause == NULL && m_bPause == true)
				{// ポーズ中にメモリが確保されていない
					m_pPause = CPause::Create(PAUSE_PRIORITY);
					m_nPauseController = nCntStick;
					//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPEN);
				}
				else if (m_pPause != NULL)
				{// ポーズ解除時にメモリが確保されている
					ReleasePause();
				}
			}
		}
	}

	if (m_bPause == false)
	{// ポーズ中でない
	    // シーンクラスの更新処理
		CScene::UpdateAll();
	}
	else
	{// ポーズ中である
		if (m_pPause != NULL)
		{// メモリが確保されている
		    // ポーズの更新処理
			m_pPause->Update();
			CScene::DeathCheck();
		}
	}
}

//=============================================================================
//    終了状態の処理
//=============================================================================
void CGame::End(void)
{
	// 全てのオブジェクトを更新する
	CScene::UpdateAll();

	m_nStateCounter++;
	if (m_nStateCounter > 120)
	{// 終了状態になってからある程度たった
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードしていなかったら
			CManager::GetFade()->SetFade(CManager::MODE_RESULT);
		}
	}
}

//=============================================================================
//    ポーズクラスを開放する処理
//=============================================================================
void CGame::ReleasePause(void)
{
	if (m_pPause != NULL)
	{// ポーズ解除時にメモリが確保されている
	    // ポーズの終了処理
		m_pPause->Uninit();

		// メモリの開放
		delete m_pPause;
		m_pPause = NULL;
	}
}

//=============================================================================
//    プレイヤー情報を設定する処理
//=============================================================================
void CGame::SettingPlayer(const int nControllerIdx)
{
	// プレイヤーを使用する状態に
	m_PlayerSetting[m_nNumPlayer].bUse = true;
	m_PlayerSetting[m_nNumPlayer].nControllerIdx = nControllerIdx;

	// プレイヤーの人数を増やす
	m_nNumPlayer++;
}

//=============================================================================
//    星用のオブジェクトを出す処理
//=============================================================================
void CGame::SetStarObject(void)
{
	if (GetMap() != NULL)
	{
		if (GetMap()->GetObjectManager(2) != NULL)
		{
			D3DXVECTOR3 ObjPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			ObjPos.x = rand() % 3600 - 1800.0f;
			ObjPos.z = rand() % 3600 - 1800.0f;
			D3DXVECTOR3 ObjRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 ObjScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			GetMap()->GetObjectManager(2)->CreateStarObject(ObjPos, ObjRot, ObjScale, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, true, OBJECT_PRIORITY);
		}
	}
}

//=============================================================================
//    マップの番号を設定する処理
//=============================================================================
void CGame::SetGameMapIdx(const int nGameMapIdx)
{
	m_nGameMapIdx = nGameMapIdx;
}

//=============================================================================
//    プレイヤーの人数を設定する処理
//=============================================================================
void CGame::SetNumPlayer(const int nNumPlayer)
{
	m_nNumPlayer = nNumPlayer;
}

//=============================================================================
//    状態を設定する処理
//=============================================================================
void CGame::SetState(const CGame::STATE state)
{
	m_State = state;
	m_nStateCounter = 0;

	if (state == STATE_END)
	{
		// 勝敗の設定
		if (m_pPlayer[0] != NULL && m_pPlayer[1] != NULL)
		{
			if (m_pPlayer[0]->GetScore() <= m_pPlayer[1]->GetScore())
			{
				CResult::SetWinnerIdx(1);
			}
		}
	}
}

//=============================================================================
//    ポーズ中かどうか設定する処理
//=============================================================================
void CGame::SetPause(const bool bPause)
{
	m_bPause = bPause;
}

//=============================================================================
//     ポーズを押したコントローラーの番号を設定する処理
//=============================================================================
void CGame::SetPauseController(const int nPauseController)
{
	m_nPauseController = nPauseController;
}

//=============================================================================
//    状態を取得する処理
//=============================================================================
CGame::STATE CGame::GetState(void)
{
	return m_State;
}

//=============================================================================
//     ポーズ中かどうか取得する処理
//=============================================================================
bool CGame::GetPause(void)
{
	return m_bPause;
}

//=============================================================================
//     ポーズを押したコントローラーの番号を取得する処理
//=============================================================================
int CGame::GetPauseController(void)
{
	return m_nPauseController;
}