//*****************************************************************************
//
//     エネミーの処理[enemy.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "modelManager.h"
#include "motion.h"
#include "gauge.h"
#include "score.h"
#include "game.h"
#include "meshField.h"
#include "object.h"
#include "functionlib.h"
#include "debuglog.h"
#include "player.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
// 敵のデータ設定用マクロ
#define ENEMY_GAUGE_POS (D3DXVECTOR3(0.0f,90.0f,0.0f))    // 敵のゲージオフセット位置


// 値読み込みをする際の目印となる文字列
// 共通
#define POS                "POS = "                // 座標を読み取る合図
#define ROT                "ROT = "                // 向きを読み取る合図
#define MOVE               "MOVE = "               // 移動量を読み取る合図

// その他
#define ENEMY_SET_FILENAME "ENEMY_SET_FILENAME = " // 敵の配置データスクリプトファイル名
#define NUM_MODEL          "NUM_MODEL = "          // 読み込むモデルの数
#define MODEL_FILENAME     "MODEL_FILENAME = "     // 読み込むモデルのファイル名

// キャラクター情報
#define CHARACTERSET       "CHARACTERSET"          // キャラクター情報読み込み開始の合図
#define END_CHARACTERSET   "END_CHARACTERSET"      // キャラクター情報読み込み終了の合図
#define JUMP               "JUMP = "               // ジャンプ量
#define GRAVITY            "GRAVITY = "            // 重力量
#define COL_HEIGHT         "COL_HEIGHT = "         // 当たり判定をとる高さ
#define COL_RANGE          "COL_RANGE = "          // 当たり判定をとる範囲
#define NUM_PARTS          "NUM_PARTS = "          // キャラクターのパーツ数

// パーツ情報
#define PARTSSET           "PARTSSET"              // パーツ情報読み込み開始の合図
#define END_PARTSSET       "END_PARTSSET"          // パーツ情報読み込み終了の合図
#define INDEX              "INDEX = "              // 番号を読み取る合図
#define PARENT             "PARENT = "             // 親パーツの番号を読み取る合図

// モーション情報
#define MOTIONSET          "MOTIONSET"             // モーション情報読み込み開始の合図
#define END_MOTIONSET      "END_MOTIONSET"         // モーション情報読み込み終了の合図
#define LOOP               "LOOP = "               // モーションがループ再生するかどうかを読み取る合図
#define BLEND              "BLEND = "              // モーションブレンドの倍率を読み取る合図
#define NUM_KEY            "NUM_KEY = "            // モーションのキーフレームの数を読み取る合図

// キーフレーム情報
#define KEYSET             "KEYSET"                // キーフレーム情報読み込み開始の合図
#define END_KEYSET         "END_KEYSET"            // キーフレーム情報読み込み終了の合図
#define FRAME              "FRAME = "              // キーフレームの再生フレーム数を読み取る合図

// キー情報
#define KEY                "KEY"                   // キー情報読み込み開始の合図
#define END_KEY            "END_KEY"               // キー情報読み込み終了の合図

// 敵の配置情報
#define ENEMYSET           "ENEMYSET"              // 敵の配置データ読み込み開始の合図
#define END_ENEMYSET       "END_ENEMYSET"          // 敵の配置データ読み込み終了の合図
#define LIFE               "LIFE = "               // 敵の体力
#define SCORE              "SCORE = "              // 敵を倒した時に増えるスコア量

#define ENEMY_SPEED		   (1.5f)				   // 敵の速度

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************

//*****************************************************************************
//     CEnemyManagerの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CEnemyManager::CEnemyManager(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値の設定
	m_nCntPop = 0;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEnemyManager::~CEnemyManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CEnemyManager *CEnemyManager::Create(char *pFileName, int nPriority)
{
	CEnemyManager *pEnemyManager = NULL;         // エネミー管轄クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pEnemyManager == NULL)
		{// メモリが空になっている
			pEnemyManager = new CEnemyManager(nPriority);
			if (pEnemyManager != NULL)
			{// インスタンスを生成できた
				pEnemyManager->SetFileName(pFileName);
				if (FAILED(pEnemyManager->Init()))
				{// 初期化に失敗した
					return NULL;
				}
			}
			else
			{// インスタンスを生成できなかった
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// これ以上シーンを作成できない
		return NULL;
	}

	return pEnemyManager;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CEnemyManager::Init(void)
{
	// エネミーのパーツ情報を抜き出す
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                         // 1行分読み取り用
			char *pStrCur = NULL;                       // 現在の先頭ポインタ
			char *pStr = NULL;                          // 先頭ポインタ保存用
			char aSetFileName[256] = "\0";              // 敵の配置データスクリプトファイル名
			int nNumModel = 0;                          // 読み込むモデル数
			int nNumParts = 0;                          // 読み込むパーツ数
			int nCntModel = 0;                          // モデルを読み込んだ回数を数える
			int nCntParts = 0;                          // パーツ情報を読み込んだ回数を数える
			int nCntMotion = 0;                         // モーションを読み込んだ回数を数える
			int *pNumParent = NULL;                     // 親モデルの番号
			CMotionManager *pMotionManager = NULL;      // モーション管理クラスへのポインタ
			CMotion *pMotion[CEnemy::STATE_MAX] = {};   // モーションクラスへのポインタ
			CModel **apModel = NULL;                    // モデルクラスへのポインタ
			LPD3DXMESH pMesh = NULL;                    // メッシュ情報へのポインタ
			LPD3DXBUFFER pBuffMat = NULL;               // マテリアル情報へのポインタ
			DWORD nNumMat = 0;                          // マテリアル情報の数
			float fGravity = 0.0f;                      // 敵の重力情報
			float fColHeight = 0.0f;                    // 当たり判定を取る高さ
			float fColRange = 0.0f;                     // 当たり判定を取る範囲

			if (pLine == NULL && pStr == NULL)
			{// メモリが確保できる状態である
				pLine = new char[256];
				pStr = new char[256];
				strcpy(pLine, "\0");
				strcpy(pStr, "\0");
				if (pLine != NULL && pStr != NULL)
				{// メモリが確保されている
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// スクリプト読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, ENEMY_SET_FILENAME) == 0)
							{// 敵の配置データスクリプトファイルのファイルパス名を読み込んだ
								pStr = CFunctionLib::ReadString(pStrCur, pStr, ENEMY_SET_FILENAME);
								strcpy(aSetFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
							{// モデル数を読み込むなら
								strcpy(pStr, pStrCur);
								nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
								if (nNumModel >= 1)
								{// モデル数が1つ以上ある
									pNumParent = new int[nNumModel];
									m_pModelManager = CModelManager::Create(nNumModel);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
							{// モデルのファイルパス名を読み込んだ
								if (m_pModelManager != NULL && nCntModel < nNumModel)
								{// モデル管轄クラスへのポインタが確保されている
									// モデルのファイルパス名を読み取る
									pStr = CFunctionLib::ReadString(pStrCur, pStr, MODEL_FILENAME);

									// xファイルの読み込み
									D3DXLoadMeshFromX(pStr,
										D3DXMESH_SYSTEMMEM,
										CManager::GetRenderer()->GetDevice(),
										NULL,
										&pBuffMat,
										NULL,
										&nNumMat,
										&pMesh);

									// モデル管轄クラスにポインタを設定する
									m_pModelManager->SetMesh(pMesh, nCntModel);
									m_pModelManager->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);

									// 使用した変数を初期化しておく
									pMesh = NULL;
									pBuffMat = NULL;
									nNumMat = 0;

									nCntModel++;  // モデルを読み込んだ回数を増やす
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, CHARACTERSET) == 0)
							{// キャラクター情報読み込み開始の合図があった
								apModel = ReadCharacterSet(pLine, pStrCur, pFile, &nNumParts, pNumParent, &fGravity, &fColHeight, &fColRange);
								nCntParts++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOTIONSET) == 0)
							{// モーション情報読み込み開始の合図があった
								pMotion[nCntMotion] = ReadMotionSet(pLine, pStrCur, pFile, &nNumParts, pMotion[nCntMotion], nCntMotion);
								nCntMotion++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
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

			// 必要なクラスを作成しておく
			// モーション管轄クラス
			if (pMotionManager == NULL)
			{// ポインタが確保されている
				pMotionManager = CMotionManager::Create(CEnemy::STATE_MAX, nNumParts);
				if (pMotionManager != NULL)
				{// モーション管理クラスが作成できた
					for (int nCntMotion = 0; nCntMotion < CEnemy::STATE_MAX; nCntMotion++)
					{// 状態の数だけ繰りかえし
						pMotionManager->SetMotion(pMotion[nCntMotion], nCntMotion);
					}
				}
			}

			// 敵を配置する
			SetEnemy(apModel, pMotionManager, nNumParts, pNumParent, fGravity, fColHeight, fColHeight, aSetFileName);

			// 読み取り用のポインタを開放しておく
			if (apModel != NULL)
			{// メモリが確保されている
				for (int nCntModel = 0; nCntModel < nNumParts; nCntModel++)
				{// 使用できるモデル数分繰り返し
					if (apModel[nCntModel] != NULL)
					{// ポインタが確保されている
						apModel[nCntModel]->Uninit();

						// メモリの開放
						delete apModel[nCntModel];
						apModel[nCntModel] = NULL;
					}
				}
				// メモリの開放
				delete[] apModel;
				apModel = NULL;
			}

			// モーションの破棄
			if (pMotionManager != NULL)
			{// ポインタが確保されている
				pMotionManager->Uninit();

				// メモリの開放
				delete pMotionManager;
				pMotionManager = NULL;
			}

			// 親モデルの番号
			if (pNumParent != NULL)
			{// メモリが確保されている
				delete[] pNumParent;
				pNumParent = NULL;
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
void CEnemyManager::Uninit(void)
{
	// モデル管轄クラスの破棄
	if (m_pModelManager != NULL)
	{// メモリが確保されている
	    // 終了処理
		m_pModelManager->Uninit();

		// メモリの開放
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// 自身のポインタを破棄
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CEnemyManager::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CEnemyManager::Draw(void)
{

}

//=============================================================================
//    敵を配置する処理
//=============================================================================
void CEnemyManager::SetEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, char *pSetFileName)
{
	// 敵の配置情報を抜き出す
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(pSetFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                                    // 1行分読み取り用
			char *pStrCur = NULL;                                  // 現在の先頭ポインタ
			char *pStr = NULL;                                     // 先頭ポインタ保存用
			int nEnemyLife = 0;                                    // 敵の体力
			int nEnemyAddScore = 0;                                // 敵を倒した時に増えるスコア量
			D3DXVECTOR3 EnemyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 敵の配置座標
			D3DXVECTOR3 EnemyRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 敵の向き
			int nCntEnemy = 0;

			if (pLine == NULL && pStr == NULL)
			{// メモリが確保できる状態である
				pLine = new char[256];
				pStr = new char[256];
				strcpy(pLine, "\0");
				strcpy(pStr, "\0");
				if (pLine != NULL && pStr != NULL)
				{// メモリが確保されている
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// スクリプト読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, ENEMYSET) == 0)
							{// 敵の配置情報読み込み開始の合図だった
								while (1)
								{// ループ開始
									pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
									if (CFunctionLib::Memcmp(pStrCur, LIFE) == 0)
									{// 体力がある
										nEnemyLife = CFunctionLib::ReadInt(pStrCur, LIFE);
									}
									else if (CFunctionLib::Memcmp(pStrCur, SCORE) == 0)
									{// 敵の座標情報がある
										nEnemyAddScore = CFunctionLib::ReadInt(pStrCur, SCORE);
									}
									else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
									{// 敵の座標情報がある
										EnemyPos = CFunctionLib::ReadVector3(pStrCur, POS);
									}
									else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
									{// 敵の向き情報がある
										EnemyRot = CFunctionLib::ReadVector3(pStrCur, ROT);
									}
									else if (CFunctionLib::Memcmp(pStrCur, END_ENEMYSET) == 0)
									{// 敵の配置情報読み込み終了の合図だった
										// エネミークラスを作成する
										CreateEnemy(apModel, pMotionManager, nNumParts, pNumParent, fGravity, fColHeight, fColRange, EnemyPos, EnemyRot, nEnemyLife, nEnemyAddScore);
										nCntEnemy++;
										break;  // ループ終了
									}
								}
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
			}
			fclose(pFile);
		}
	}
}

//=============================================================================
//    エネミークラスを作製する処理
//=============================================================================
void CEnemyManager::CreateEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nLife, const int nAddScore)
{
	CModel **apModelCpy = NULL;                    // モデルクラスコピー用
	CMotion *pMotionCpy[CEnemy::STATE_MAX] = {};   // モージョンクラスコピー用
	CMotionManager *pMotionManagerCpy = NULL;      // モーション管轄クラスコピー用
	CEnemy *pEnemy = NULL;                         // エネミークラスへのポインタ

	if (apModel != NULL && pMotionManager != NULL && m_pModelManager != NULL)
	{// モデルへのポインタとモーションクラスへのポインタとモデル管轄クラスへのポインタが作成されている
	    // モデル情報をコピーする
		if (apModelCpy == NULL)
		{// メモリが確保されている
			apModelCpy = new CModel*[nNumParts];
			if (apModelCpy != NULL)
			{// メモリが確保できた
				for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
				{// パーツ数分繰り返し
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// メモリが確保できる状態である
						apModelCpy[nCntParts] = CModel::Create(apModel[nCntParts]->GetPos(), apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(nCntParts), m_pModelManager->GetBuffMat(nCntParts), m_pModelManager->GetNumMat(nCntParts), m_pModelManager->GetVtxMax(nCntParts), m_pModelManager->GetVtxMin(nCntParts), m_pModelManager->GetTexture(nCntParts));
						if (apModelCpy[nCntParts] != NULL)
						{// メモリが確保できた
							if (pNumParent[nCntParts] != -1)
							{// 親モデルが存在する
								apModelCpy[nCntParts]->SetParent(apModelCpy[pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// モーション情報をコピーする
		if (pMotionManagerCpy == NULL)
		{// メモリが確保できる状態である
			pMotionManagerCpy = CMotionManager::Create(CEnemy::STATE_MAX, nNumParts);
			if (pMotionManagerCpy != NULL)
			{// モーション管理クラスが作成できた
				for (int nCntMotion = 0; nCntMotion < CEnemy::STATE_MAX; nCntMotion++)
				{// 敵の状態の数だけ繰り返し
					if (pMotionCpy[nCntMotion] == NULL)
					{// メモリを確保できる状態である
						pMotionCpy[nCntMotion] = CMotion::Create(nNumParts, pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), pMotionManager->GetMotion()[nCntMotion]->GetLoop(), pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// メモリが確保できた
							for (int nCntKey = 0; nCntKey < pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// キーフレームの総数分繰り返し
								for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
								{// キーフレームの総数分繰り返し
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// モーション管轄クラスにポインタを渡す
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// エネミークラスを作成する
		if (pEnemy == NULL)
		{// メモリが確保できる状態である
			pEnemy = CEnemy::Create(pos, rot, apModelCpy, pMotionManagerCpy, nNumParts, fGravity, fColHeight, fColRange, nLife, nAddScore);
		}
	}
}

//=============================================================================
//    エネミー情報を読み取る処理
//=============================================================================
CModel **CEnemyManager::ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pGravity, float *pColHeight, float *pColRange)
{
	int nCntParts = 0;
	int nNumParts = 0;
	CModel **apModel = NULL;
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, GRAVITY) == 0)
		{// 重力量がある
			*pGravity = CFunctionLib::ReadFloat(pStrCur, GRAVITY);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_HEIGHT) == 0)
		{//	当たり判定を取る高さがある
			*pColHeight = CFunctionLib::ReadFloat(pStrCur, COL_HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// 当たり判定を取る範囲がある
			*pColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_PARTS) == 0)
		{// パーツ数情報がある
			*pNumParts = (CFunctionLib::ReadInt(pStrCur, NUM_PARTS));
			if (apModel == NULL)
			{// メモリが確保できる状態である
				apModel = new CModel*[*pNumParts];
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARTSSET) == 0)
		{// パーツ情報読み込み開始の合図だった
			if (nCntParts < *pNumParts)
			{// まだパーツ情報を読み込める
				apModel[nCntParts] = ReadPartsSet(pLine, pStrCur, pFile, pNumParts, pNumParent, apModel);
				nCntParts++;
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_CHARACTERSET) == 0)
		{// キャラクター情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return apModel;
}

//=============================================================================
//    パーツ情報を読み取る処理
//=============================================================================
CModel *CEnemyManager::ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel)
{
	int nIdx = 0;                                      // パーツ番号情報
	int nParent = 0;                                   // 親モデルのパーツ番号
	CModel *pModel = NULL;                             // モデルクラスへのポインタ
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // モデルの座標情報
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // モデルの向き情報
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
		{// パーツ番号情報がある
			nIdx = CFunctionLib::ReadInt(pStrCur, INDEX);
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARENT) == 0)
		{// 親モデルのパーツ番号情報がある
			pNumParent[nIdx] = CFunctionLib::ReadInt(pStrCur, PARENT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// モデルの座標情報がある
			Pos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// モデルの向き情報がある
			Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_PARTSSET) == 0)
		{// パーツ情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	if (pModel == NULL)
	{// メモリを確保できる状態である
		pModel = CModel::Create(Pos, Rot, m_pModelManager->GetMesh(nIdx), m_pModelManager->GetBuffMat(nIdx), m_pModelManager->GetNumMat(nIdx), m_pModelManager->GetVtxMax(nIdx), m_pModelManager->GetVtxMin(nIdx), m_pModelManager->GetTexture(nIdx));
		if (pModel != NULL)
		{// モデルポインタが確保された
			if (nParent != -1)
			{// 親モデルが存在する
				pModel->SetParent(apModel[nParent]);
			}
		}
	}

	return pModel;
}

//=============================================================================
//    モーション情報を読み取る処理
//=============================================================================
CMotion *CEnemyManager::ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion)
{
	int nCntKeySet = 0;              // キーフレームフレーム情報を読み込んだ回数
	bool bLoop = false;              // ループするかしないか
	float fBlendMag = 0.0f;          // ブレンド倍率
	int nNumKey = 0;                 // キーフレームの総数
	int *pPlaybackKey = NULL;        // 再生フレーム数(複数あるので動的確保)
	D3DXVECTOR3 **pAddPos = NULL;    // モデルの座標
	D3DXVECTOR3 **pDestRot = NULL;   // モデルの向き
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, LOOP) == 0)
		{// ループするかしないかの情報がある
			bLoop = CFunctionLib::ReadBool(pStrCur, LOOP);
		}
		else if (CFunctionLib::Memcmp(pStrCur, BLEND) == 0)
		{// ブレンド倍率の情報がある
			fBlendMag = CFunctionLib::ReadFloat(pStrCur, BLEND);
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_KEY) == 0)
		{// キーフレームの総数の情報がある
			nNumKey = (CFunctionLib::ReadInt(pStrCur, NUM_KEY));
			if (nNumKey >= 1)
			{// 1個以上キーフレームがある
			 // 必要な変数を動的に確保する
			 // 再生フレーム数
				pPlaybackKey = new int[nNumKey];

				// 座標
				if (pAddPos == NULL)
				{// メモリが確保できる状態である
					pAddPos = new D3DXVECTOR3*[nNumKey];
					if (pAddPos != NULL)
					{// メモリが確保できた
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// キーフレームの総数分繰り返し
							pAddPos[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}

				if (pDestRot == NULL)
				{// メモリが確保できる状態である
				 // 向き
					pDestRot = new D3DXVECTOR3*[nNumKey];
					if (pDestRot != NULL)
					{// メモリが確保できた
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// キーフレームの総数分繰り返し
							pDestRot[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, KEYSET) == 0)
		{// キーフレーム情報読み込み開始の合図だった
			ReadKeySet(pLine, pStrCur, pFile, nCntKeySet, pPlaybackKey, pAddPos[nCntKeySet], pDestRot[nCntKeySet]);
			nCntKeySet++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_MOTIONSET) == 0)
		{// モーション情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	if (pMotion == NULL)
	{// メモリが確保できる状態である
		pMotion = CMotion::Create(*pNumParts, nNumKey, fBlendMag, bLoop, pPlaybackKey);
		if (pMotion != NULL)
		{// メモリが確保できた
			for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
			{// キーフレームの総数分繰り返し
				for (int nCntParts = 0; nCntParts < *pNumParts; nCntParts++)
				{// キーフレームの総数分繰り返し
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pAddPos[nCntKey][nCntParts]);
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pDestRot[nCntKey][nCntParts]);
				}
			}
		}
	}

	// メモリを確保した分開放する
	// 再生フレーム数
	if (pPlaybackKey != NULL)
	{// メモリが確保されている
		delete[] pPlaybackKey;
		pPlaybackKey = NULL;
	}

	// 座標
	if (pAddPos != NULL)
	{// メモリが確保できた
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// キーフレームの総数分繰り返し
			delete[] pAddPos[nCntKey];
			pAddPos[nCntKey] = NULL;
		}
		delete[] pAddPos;
		pAddPos = NULL;
	}

	// 向き
	if (pDestRot != NULL)
	{// メモリが確保できた
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// キーフレームの総数分繰り返し
			delete[] pDestRot[nCntKey];
			pDestRot[nCntKey] = NULL;
		}
		delete[] pDestRot;
		pDestRot = NULL;
	}

	return pMotion;
}

//=============================================================================
//    キーフレーム情報を読み取る処理
//=============================================================================
void CEnemyManager::ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	int nCntKey = 0;
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, FRAME) == 0)
		{// 再生フレーム数がある
			pPlaybackKey[nCntKeySet] = CFunctionLib::ReadInt(pStrCur, FRAME);
		}
		if (CFunctionLib::Memcmp(pStrCur, KEY) == 0)
		{// キー情報読み込み開始の合図だった
			ReadKey(pLine, pStrCur, pFile, nCntKey, AddPos, DestRot);
			nCntKey++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEYSET) == 0)
		{// キーフレーム情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    キー情報を読み取る処理
//=============================================================================
void CEnemyManager::ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// 座標がある
			AddPos[nCntKey] = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// 向きがある
			DestRot[nCntKey] = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEY) == 0)
		{// キー情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}
}

//=============================================================================
//    スクリプトファイル名を設定する処理
//=============================================================================
void CEnemyManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    モデル管轄クラスへのポインタを設定する処理
//=============================================================================
void CEnemyManager::SetModelManager(CModelManager *pModelManager)
{
	m_pModelManager = pModelManager;
}

//=============================================================================
//    スクリプトファイル名を取得する処理
//=============================================================================
char *CEnemyManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    モデル管轄クラスへのポインタを取得する処理
//=============================================================================
CModelManager *CEnemyManager::GetModelManager(void)
{
	return m_pModelManager;
}

//*****************************************************************************
//     CEnemyの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CEnemy::CEnemy(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	// 各種値の設定
	m_State = STATE_NORMAL;                  // 状態
	m_nLife = 0;                             // 体力
	m_nAddScore = 0;                         // 倒した時に増えるスコア量
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 移動量
	m_pGaugeLife = NULL;                     // 体力ゲージ
	m_bMoveRot = false;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore, int nPriority)
{
	CEnemy *pEnemy = NULL;                 // エネミークラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pEnemy == NULL)
		{// メモリが空になっている
			pEnemy = new CEnemy(nPriority);
			if (pEnemy != NULL)
			{// インスタンスを生成できた
				if (FAILED(pEnemy->Init(pos, rot, apModel, pMotionManager, nNumParts, fGravity, fColHeight, fColRange, nLife, nAddScore)))
				{// 初期化に失敗した
					return NULL;
				}
			}
			else
			{// インスタンスを生成できなかった
				return NULL;
			}
		}
		else
		{// インスタンスを生成できなかった
			return NULL;
		}
	}
	else
	{// これ以上シーンを作成できない
		return NULL;
	}

	return pEnemy;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore)
{
	// 各種値の設定
	SetPos(pos);                       // 現在の座標
	SetRot(rot);                       // 現在の向き
	SetNumParts(nNumParts);            // パーツ数
	SetModel(apModel);                 // モデルクラスへのポインタ
	SetMotionManager(pMotionManager);  // モーション管轄クラスへのポインタ
	SetGravity(fGravity);              // 重力
	SetColHeight(fColHeight);          // 当たり判定を取る高さ
	SetColRange(fColRange);            // 当たり判定を取る範囲
	SetLife(nLife);                    // 体力
	SetAddScore(nAddScore);            // 倒した時に増えるスコア量

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CEnemy::Uninit(void)
{
	// 体力ゲージの開放
	if (m_pGaugeLife != NULL)
	{
		m_pGaugeLife->Uninit();
		m_pGaugeLife = NULL;
	}

	// 共通の終了処理
	CCharacter::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CEnemy::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = GetPosOld();
	D3DXVECTOR3 rot = GetRot();

	posOld = GetPos();
	SetPosOld(posOld);

	if (m_State != STATE_DEATH)
	{
		CScene *pScene = CScene::GetTop(3);

		while (pScene != NULL)
		{
			CScene *pNextScene = pScene->GetNext();

			if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
			{
				CPlayer *pPlayer = (CPlayer*)pScene;
				D3DXVECTOR3 posPlayer = pPlayer->GetPos();

				if (CollisionPlayer(posPlayer, 30) && pPlayer->GetState() != CPlayer::STATE_DEATH)
				{// プレイヤーとの当たり判定
					pPlayer->SetState(CPlayer::STATE_DEATH);
					pPlayer->GetMotionManager()->SwitchMotion(pPlayer->GetModel(), pPlayer->GetState());
					break;
				}
			}
			pScene = pNextScene;
		}

		if (GetLand())
		{// 地上
			STATE state = GetState();
			SetState(STATE_MOVE);

			if (state != STATE_MOVE)
			{// 移動モーション
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			if (m_bMoveRot)
			{// 右向き
				rot.y = D3DX_PI * -0.5f;
				SetRot(rot);
				m_Move.x = ENEMY_SPEED;

				if (CManager::GetGame()->GetNumStageClear() == 0)
				{// ステージ１
					if (pos.x >= 500.0f)
					{
						pos.x = 500.0f;
						m_bMoveRot = false;
					}
				}
				else if (CManager::GetGame()->GetNumStageClear() == 1)
				{// ステージ2
					if (pos.x >= 1000.0f)
					{
						pos.x = 1000.0f;
						m_bMoveRot = false;
					}
				}
			}
			else
			{// 左向き
				rot.y = D3DX_PI * 0.5f;
				SetRot(rot);

				m_Move.x = -ENEMY_SPEED;

				if (CManager::GetGame()->GetNumStageClear() == 0)
				{// ステージ１
					if (pos.x <= -500.0f)
					{
						pos.x = -500.0f;
						m_bMoveRot = true;
					}
				}
				else if (CManager::GetGame()->GetNumStageClear() == 1)
				{// ステージ2
					if (pos.x <= -1000.0f)
					{
						m_bMoveRot = true;
					}
				}
			}
		}
		else
		{
			STATE state = GetState();

			SetState(STATE_FALL);

			if (state != STATE_FALL)
			{// 落下モーション
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}

			m_Move.x = 0.0f;
		}

		m_Move.y += GetGravity();
		pos += m_Move;

		SetPos(pos);

		// 当たり判定処理
		Collision();

		// 状態遷移処理
		Statement();
	}
	else
	{
		Death();
	}

	if (GetMotionManager() != NULL)
	{// モーション管理クラスが生成されている
		GetMotionManager()->Update(GetModel());
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CEnemy::Draw(void)
{
	// 共通の描画処理
	CCharacter::Draw();
}

//=============================================================================
//    敵が死んだときの処理
//=============================================================================
void CEnemy::Death(void)
{
	if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
	{// 死んだときのモーションが終わった
		Uninit();
	}
}

//=============================================================================
//    当たり判定処理
//=============================================================================
void CEnemy::Collision(void)
{
	bool bLand = false;   // 乗っているかどうか

	// 各種値の取得
	D3DXVECTOR3 pos = GetPos();           // 現在の座標

	// 配置物と地面へのポインタ取得する
	CScene *pScene = NULL;               // シーンクラスへのポインタ
	CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
	CObject *pObject = NULL;             // 配置物クラスへのポインタ
	CMeshField *pField = NULL;           // 地面クラスへのポインタ
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_MESHFIELD)
			{// 地面クラスへのポインタだったら
				pField = (CMeshField*)pScene;
				if (pField != NULL)
				{// 地面が取得できた
					float fFieldHeight = pField->GetPolyHeight(pos);
					if (pos.y < fFieldHeight)
					{// 座標が地面より下になった
						m_Move.y = 0.0f;
						pos.y = fFieldHeight;
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// 配置物だったら
				pObject = (CObject*)pScene;
				if (pObject != NULL)
				{// 配置物が取得できた
					if (pObject->Collision(&pos, &GetPosOld(), &m_Move, 70.0f, 10.0f, this) == true)
					{// 配置物の上に乗っている
						if (GetLand() == false)
						{// 着地していなかったら
							SetLand(true);
						}

						if (GetLandScene() == NULL && GetLand() == true)
						{// 何かに乗っていた
							pos.y = pObject->GetPos().y + pObject->GetVtxMax().y;
						}
						SetLandScene((CScene*)pObject);
						bLand = true;
					}
				}
			}
			// 次のオブジェクトへのポインタを取得
			pScene = pSceneNext;
		}
	}

	// 各種値の設定
	SetPos(pos);         // 現在の座標
	SetLand(bLand);      // オブジェクトに乗っているかどうか
}

//=========================================================================================================================
// 当たり判定
//=========================================================================================================================
bool CEnemy::CollisionPlayer(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// 相手のXYの範囲
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - GetMtxWorld()._41;						// Xの差
	float fLengthY = pos.y - GetMtxWorld()._42;						// Yの差
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2);		// XYの差の二乗

	if (fLength >= fLengthTotal)
	{// 相手と自分の判定
		bHit = true;
	}

	return bHit;
}

//=============================================================================
//    状態遷移処理
//=============================================================================
void CEnemy::Statement(void)
{

}

//=============================================================================
//    状態を設定する処理
//=============================================================================
void CEnemy::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    体力を設定する処理
//=============================================================================
void CEnemy::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    倒した時に増えるスコア量を設定する処理
//=============================================================================
void CEnemy::SetAddScore(const int nAddScore)
{
	m_nAddScore = nAddScore;
}

//=============================================================================
//    状態を取得する処理
//=============================================================================
CEnemy::STATE CEnemy::GetState(void)
{
	return m_State;
}

//=============================================================================
//    体力を取得する処理
//=============================================================================
int CEnemy::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    倒した時に増えるスコア量を取得する処理
//=============================================================================
int CEnemy::GetAddScore(void)
{
	return m_nAddScore;
}