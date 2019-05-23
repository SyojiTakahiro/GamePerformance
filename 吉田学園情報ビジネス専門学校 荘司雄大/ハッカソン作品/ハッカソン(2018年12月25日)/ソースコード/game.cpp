//*****************************************************************************
//
//     ゲームの処理[game.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "textureManager.h"
#include "pause.h"
#include "map.h"
#include "functionlib.h"
#include "player.h"
#include "enemy.h"
#include "effectManager.h"
#include "timer.h"
#include "score.h"
#include "scene2D.h"
#include "ranking.h"
#include "meshField.h"
#include "sky.h"
#include "mountain.h"
#include "object.h"
#include "ui.h"
#include "Billpresent.h"

#include "input.h"
#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define GAME_SYSTEM_FILENAME      "data/TEXT/MODE/game.ini"              // ゲームの初期化情報スクリプトファイル名
#define GAME_MODECHANGE_TIMING    (60)                                   // 画面遷移を自動で行うタイミング
#define GAME_PLAYERRESPAWN_TIMING (180)                                  // プレイヤーをリスポーンさせるタイミング
#define GAME_PLAYER_STOCK         (3)                                    // プレイヤーの残機数
#define GAME_MAPCLEAR_CHANGETIMING (120)                                 // マップを切り替えるタイミング

// 値を読み取るパス名
#define NUM_TEXTURE            "NUM_TEXTURE = "            // 読み込むテクスチャの数
#define TEXTURE_FILENAME       "TEXTURE_FILENAME = "       // 読み込むテクスチャのファイル名
#define PLAYER_FILENAME        "PLAYER_FILENAME = "        // プレイヤーデータのスクリプトファイル名
#define ENEMY_FILENAME         "ENEMY_FILENAME = "         // エネミーデータのスクリプトファイル名
#define EFFECT_FILENAME        "EFFECT_FILENAME = "        // エフェクトデータのスクリプトファイル名
#define MAP_FILENAME           "MAP_FILENAME = "           // マップデータのスクリプトファイル名
#define GAME_TIMER             "GAME_TIMER = "             // タイマーの秒数

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CGame::CGame(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_pUi = NULL;
	m_pMap = NULL;             // マップクラスへのポインタ
	m_State = STATE_NORMAL;    // 状態
	m_nStateCounter = 0;       // 状態を管理するカウンター
	m_pTextureManager = NULL;  // テクスチャ管轄クラスへのポインタ
	m_nNumStock = 0;           // プレイヤーの残機数
	m_nNumPresent = 0;
	m_nNumStageClear = 0;      // 現在のクリアしたステージ数

	m_pMesh = NULL;                            // メッシュ情報へのポインタ
	m_pBuffMat = NULL;                         // マテリアル情報へのポインタ
	m_nNumMat = 0;                             // マテリアル情報の数
	m_VtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);    // 頂点座標の最大値へのポインタ
	m_VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 頂点座標の最小値へのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//    ゴール用のモデルを読み込む処理
//=============================================================================
void CGame::LoadGoalModel(void)
{

}

//=============================================================================
//    生成処理
//=============================================================================
CGame *CGame::Create(void)
{
	CGame *pGame = NULL;  // ゲームクラス型のポインタ
	if (pGame == NULL)
	{// メモリが確保できる状態にある
		pGame = new CGame;
		if (pGame != NULL)
		{// メモリが確保できた
		    // 初期化処理を行う
			pGame->Init();
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
	return pGame;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CGame::Init(void)
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
			int nNumTex = 0;                            // テクスチャを読み込む数
			int nCntTex = 0;                            // テクスチャを読み込んだ回数
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ読み取り用
			char aTexFileName[256] = "\0";              // テクスチャのファイル名
			char aPlayerFileName[256] = "\0";           // プレイヤーデータのスクリプトファイル名
			char aEnemyFileName[256] = "\0";            // エネミーデータのスクリプトファイル名
			char aEffectFileName[256] = "\0";           // エフェクトデータのスクリプトファイル名
			char aMapFileName[256] = "\0";              // マップデータのスクリプトファイル名
			int nTimer = 10 * 60;                       // タイマーの秒数

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
							else if (CFunctionLib::Memcmp(pStrCur, PLAYER_FILENAME) == 0)
							{// プレイヤーデータのスクリプトファイル名だった
							    // プレイヤーデータのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, PLAYER_FILENAME);
								strcpy(aPlayerFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ENEMY_FILENAME) == 0)
							{// エネミーデータのスクリプトファイル名だった
							    // エネミーデータのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, ENEMY_FILENAME);
								strcpy(aEnemyFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, EFFECT_FILENAME) == 0)
							{// エフェクトデータのスクリプトファイル名だった
							    // エフェクトデータのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, EFFECT_FILENAME);
								strcpy(aEffectFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// マップデータのスクリプトファイル名だった
							    // マップデータのスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(aMapFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, GAME_TIMER) == 0)
							{// タイマーの秒数だった
								nTimer = CFunctionLib::ReadInt(pStrCur, GAME_TIMER);
								nTimer *= 60;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}


					// 必要なクラスを作成する
					// UI
					m_pUi = CUI::Create();

					// マップ
					m_pMap = CMap::Create(aMapFileName);

					// エフェクト管轄クラス
					CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
					//pEffectManager->SetEffect(D3DXVECTOR3(-100.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_FIRE);

					// プレイヤー
					CPlayerManager::Create(aPlayerFileName);

					// エネミー
					CEnemyManager::Create(aEnemyFileName);

					// プレゼント
					CBillPresent *pBillPresent = NULL;
					pBillPresent = CBillPresent::Create(D3DXVECTOR3(-250.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f,5);
					pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));

					pBillPresent = CBillPresent::Create(D3DXVECTOR3(150.0f, 225.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f,5);
					pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));

					pBillPresent = CBillPresent::Create(D3DXVECTOR3(450.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f,5);
					pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));

					pBillPresent = CBillPresent::Create(D3DXVECTOR3(-350.0f, 525.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f,5);
					pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));

					pBillPresent = CBillPresent::Create(D3DXVECTOR3(350.0f, 525.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
					pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));

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


	// 各種値の設定
	m_nNumStock = GAME_PLAYER_STOCK;  // プレイヤーの残機数
	m_nNumPresent = 5;

	// ゴール用のモデルを読み込む
	D3DXLoadMeshFromX("data/MODEL/OBJECT/tonakai.x",
		D3DXMESH_SYSTEMMEM,
		CManager::GetRenderer()->GetDevice(),
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	// 最小値と最大値を初期化
	m_VtxMin = D3DXVECTOR3(10000000.0f, 10000000.0f, 10000000.0f);
	m_VtxMax = D3DXVECTOR3(-10000000.0f, -10000000.0f, -10000000.0f);

	int nNumVtx;     // 頂点数
	DWORD sizeFVF;   // 頂点フォーマットのサイズ
	BYTE *pVtxBuff;  // 頂点バッファへのポインタ

	// 頂点数を獲得
	nNumVtx = m_pMesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// 頂点バッファをロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// すべての頂点を比較してモデルの最小値,最大値を抜き出す
		D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // 頂点座標の代入

		if (Vtx.x < m_VtxMin.x)
		{// 取り出したX座標の最小値が今までのよりも小さい
			m_VtxMin.x = Vtx.x;  // 最小値更新
		}
		if (Vtx.y < m_VtxMin.y)
		{// 取り出したY座標の最小値が今までのよりも小さい
			m_VtxMin.y = Vtx.y;  // 最小値更新
		}
		if (Vtx.z < m_VtxMin.z)
		{// 取り出したZ座標の最小値が今までのよりも小さい
			m_VtxMin.z = Vtx.z;  // 最小値更新
		}
		if (Vtx.x > m_VtxMax.x)
		{// 取り出したX座標の最大値が今までのよりも大きい
			m_VtxMax.x = Vtx.x;  // 最大値更新
		}
		if (Vtx.y > m_VtxMax.y)
		{// 取り出したY座標の最大値が今までのよりも大きい
			m_VtxMax.y = Vtx.y;  // 最大値更新
		}
		if (Vtx.z > m_VtxMax.z)
		{// 取り出したZ座標の最大値が今までのよりも大きい
			m_VtxMax.z = Vtx.z;  // 最大値更新
		}

		pVtxBuff += sizeFVF;  // サイズ分ポインタを進める
	}
	// 頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CGame::Uninit(void)
{
	// UIクラスを開放する
	if (m_pUi != NULL)
	{
		m_pUi->Uninit();

		delete m_pUi;
		m_pUi = NULL;
	}

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

	// ゴール用のモデルを開放する
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	// ゲームオブジェクトの破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CGame::Update(void)
{
	switch (m_State)
	{// 状態によって処理わけ
	case STATE_NORMAL:    // 通常状態
		break;
	case STATE_MAPCHANGE:     // マップ切り替え状態
		m_nStateCounter++;
		if (m_nStateCounter >= GAME_MAPCLEAR_CHANGETIMING)
		{// カウンターが既定の値に達した
			CDebugProc::Print("マップ切り替え中\n");
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// フェードの状態が使用されていない状態である
				CManager::GetFade()->SetFade(CFade::TYPE_SWITCHMAP);
			}
		}
		break;
	case STATE_PLAYER_DEATH:       // プレイヤーが死んだ状態
		m_nStateCounter++;
		if (m_nStateCounter >= GAME_PLAYERRESPAWN_TIMING)
		{// カウンターが既定の値に達した
			CPlayerManager *pPlayerManager = NULL;
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// 優先順位の数だけ繰り返し
				pScene = GetTop(nCntPriority);
				while (pScene != NULL)
				{// メモリが空になるまで繰り返し
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == OBJTYPE_PLAYERMANAGER)
					{// プレイヤー管轄クラスへのポインタだった
						pPlayerManager = (CPlayerManager*)pScene;
						pPlayerManager->SetRespawn();
						m_State = STATE_NORMAL;
						m_nStateCounter = 0;
						m_nNumStock--;
					}
					pScene = pSceneNext;
				}
			}
		}
		break;
	case STATE_END:       // 終了状態
		m_nStateCounter++;
		if (m_nStateCounter >= GAME_MODECHANGE_TIMING)
		{// カウンターが既定の値に達した
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// フェードの状態が使用されていない状態である
				CManager::GetFade()->SetFade(CManager::MODE_RESULT);
				CRanking::SetRankingScore(m_pUi->GetScore());
			}
		}
		break;
	}

	CDebugProc::Print("ゲーム画面\n");
	CDebugProc::Print("\n");
	CDebugProc::Print("( *** 操作方法 *** )\n");
	CDebugProc::Print("( ++ キーボード ++ )\n");
	CDebugProc::Print("( ポーズ )                     : [ P ]\n");
	CDebugProc::Print("( 移動 )                       : [ W ] [ A ] [ S ] [ D ]\n");
	CDebugProc::Print("( カメラ操作 )                 : [ ↑ ] [ ← ] [ ↓ ] [ → ]\n");
	CDebugProc::Print("( ジャンプ )                   : [ ENTER ]\n");
	CDebugProc::Print("( アクション(3段階まで派生) )  : [ W ]\n");
	CDebugProc::Print("\n");

	CDebugProc::Print("( ++ ゲームパッド(ELECOM社製の場合) ++ )\n");
	CDebugProc::Print("( ポーズ )                     : [ STARTボタン ]\n");
	CDebugProc::Print("( 移動 )                       : [ 左アナログスティック ] [ 十字キー ]\n");
	CDebugProc::Print("( カメラ操作 )                 : [ 右アナログスティック ]\n");
	CDebugProc::Print("( ジャンプ )                   : [ Aボタン ]\n");
	CDebugProc::Print("( アクション(3段階まで派生) )  : [ Bボタン ]\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
//    マップをクリアしたかどうかを設定する
//=============================================================================
void CGame::SetMapClear(bool bMapClear)
{
	m_bMapClear = bMapClear;
}

//=============================================================================
//    マップを切り替える処理
//=============================================================================
void CGame::SetMap(void)
{
	m_nNumStageClear++;
	m_State = STATE_NORMAL;  // 状態を戻しておく
	m_nStateCounter = 0;

	// マップの破棄
	if (m_pMap != NULL)
	{// メモリが確保されている
		m_pMap->Uninit();

		// メモリの開放
		delete m_pMap;
		m_pMap = NULL;
	}

	// 配置物等の破棄
	CObject *pObject = NULL;
	CObjectMove *pObjectMove = NULL;
	CObjectGoal *pObjectGoal = NULL;
	CMeshField *pMeshField = NULL;
	CSky *pSky = NULL;
	CMountain *pMountain = NULL;
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	CEnemy *pEnemy = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 優先順位の数だけ繰り返し
		pScene = GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// 配置物へのポインタだった
				pObject = (CObject*)pScene;
				pObject->Uninit();
			}
			else if (pScene->GetObjType() == OBJTYPE_OBJECTMOVE)
			{// 動く配置物へのポインタだった
				pObjectMove = (CObjectMove*)pScene;
				pObjectMove->Uninit();
			}
			else if (pScene->GetObjType() == OBJTYPE_OBJECTGOAL)
			{// ゴール用配置物へのポインタだった
				pObjectGoal = (CObjectGoal*)pScene;
				pObjectGoal->Uninit();
			}
			else if (pScene->GetObjType() == OBJTYPE_MESHFIELD)
			{// 地面へのポインタだった
				pMeshField = (CMeshField*)pScene;
				pMeshField->Uninit();
			}
			else if (pScene->GetObjType() == OBJTYPE_SKY)
			{// 空へのポインタだった
				pSky = (CSky*)pScene;
				pSky->Uninit();
			}
			else if (pScene->GetObjType() == OBJTYPE_MOUNTAIN)
			{// 山へのポインタだった
				pMountain = (CMountain*)pScene;
				pMountain->Uninit();
			}
			else if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// 動く配置物へのポインタだった
				pEnemy = (CEnemy*)pScene;
				pEnemy->Uninit();
			}
			pScene = pSceneNext;
		}
	}

	CScene::DeathCheck();

	// マップの生成
	if (m_pMap == NULL)
	{// メモリが確保できる状態にある
		if (m_nNumStageClear == 1)
		{
			m_pMap = CMap::Create("data/TEXT/MAP/map_game_1.txt");

			CEnemyManager::Create("data/TEXT/CHARACTER/enemy.txt");

			// プレゼントを再配置する
			// プレゼント
			CBillPresent *pBillPresent = NULL;
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-650.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(750.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-650.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(500.0f, 225.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-350.0f, 225.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-800.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(150.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-300.0f, 525.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(100.0f, 525.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(400.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));


			m_nNumPresent = 10;
			m_pUi->SetPresent(m_nNumPresent);
		}
		else if (m_nNumStageClear == 2)
		{
			m_pMap = CMap::Create("data/TEXT/MAP/map_game_2.txt");

			CBillPresent *pBillPresent = NULL;
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-900.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(800.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(400.0f, 225.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-700.0f, 225.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-800.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(800.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(700.0f, 525.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-150.0f, 525.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(400.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(200.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));

			// プレゼントを再配置する
			m_nNumPresent = 10;
			m_pUi->SetPresent(m_nNumPresent);
		}
	}

	// タイマーを戻しておく
	if (m_pUi != NULL)
	{
		m_pUi->SetTimer(120 * 60);
	}

	m_bMapClear = false;    // マップを未クリア状態にする
}

//=============================================================================
//    ポーズクラスにテクスチャを設定する処理
//=============================================================================
void CGame::BindPauseTexture(CPause *pPause)
{
	if (pPause != NULL)
	{// メモリが確保されている
		// ポーズ背景用のテクスチャを設定する
		pPause->GetPauseBg()->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_0));

		// 選択項目用のテクスチャを設定する
		int nCntScene2DFlash = 0;
		for (int nCntTex = TEX_NUMBER_1; nCntTex < TEX_NUMBER_1 + 3; nCntTex++, nCntScene2DFlash++)
		{// テクスチャを読み込む回数繰り返し
			pPause->GetScene2DFlash(nCntScene2DFlash)->BindTexture(m_pTextureManager->GetTexture(nCntTex));
		}
	}
}

//=============================================================================
//    プレゼントの数を減らす処理
//=============================================================================
void CGame::CutPresentNum(void)
{
	m_nNumPresent--;
	if (m_pUi != NULL)
	{
		m_pUi->SetPresent(m_nNumPresent);
	}

	if (m_nNumPresent <= 0)
	{// 残りプレゼントはもうない
		CObjectGoal *pObjectGoal = NULL;
		if (m_nNumStageClear == 0)
		{
			pObjectGoal = CObjectGoal::Create(D3DXVECTOR3(450.0f, 75.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			if (pObjectGoal != NULL)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GOAL);
				pObjectGoal->BindModel(m_pMesh, m_pBuffMat, m_nNumMat, m_VtxMax, m_VtxMin, NULL);
			}
		}
		else if (m_nNumStageClear == 1)
		{
			pObjectGoal = CObjectGoal::Create(D3DXVECTOR3(900.0f, 75.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			if (pObjectGoal != NULL)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GOAL);
				pObjectGoal->BindModel(m_pMesh, m_pBuffMat, m_nNumMat, m_VtxMax, m_VtxMin, NULL);
			}
		}
		else if (m_nNumStageClear == 2)
		{
			pObjectGoal = CObjectGoal::Create(D3DXVECTOR3(950.0f, 225.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			if (pObjectGoal != NULL)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GOAL);
				pObjectGoal->BindModel(m_pMesh, m_pBuffMat, m_nNumMat, m_VtxMax, m_VtxMin, NULL);
			}
		}
	}
}

//=============================================================================
//    状態を設定する処理
//=============================================================================
void CGame::SetState(STATE state)
{
	m_State = state;
	if (m_State == STATE_PLAYER_DEATH)
	{
		m_pUi->SetStock(m_pUi->GetStock() - 1);
	}
}

//=============================================================================
//    状態を取得する処理
//=============================================================================
CGame::STATE CGame::GetState(void)
{
	return m_State;
}

//=============================================================================
//    現在のステージクリア数を取得する処理
//=============================================================================
int CGame::GetNumStageClear(void)
{
	return m_nNumStageClear;
}

//=============================================================================
//    プレイヤーの残機数を取得する処理
//=============================================================================
int CGame::GetNumStock(void)
{
	return m_nNumStock;
}

//=============================================================================
//    スコアを増やす処理
//=============================================================================
void CGame::AddScore(void)
{
	if (m_pUi != NULL)
	{
		m_pUi->SetScore(m_pUi->GetScore() + 5000);
	}
}