//*****************************************************************************
//
//     プレイヤー管轄の処理[playerManager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "character.h"
#include "manager.h"
#include "system.h"
#include "functionlib.h"
#include "model.h"
#include "motion.h"
#include "modelManager.h"
#include "textureManager.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define PLAYER_PRIORITY   (3)                     // プレイヤーの処理優先順位

// 値読み込みをする際の目印となる文字列
// 共通
#define POS               "POS = "                // 座標を読み取る合図
#define ROT               "ROT = "                // 向きを読み取る合図
#define MOVE              "MOVE = "               // 移動量を読み取る合図

// その他
#define NUM_TEXTURE       "NUM_TEXTURE = "        // 読み込むテクスチャの数
#define TEXTURE_FILENAME  "TEXTURE_FILENAME = "   // 読み込むテクスチャのファイル名
#define NUM_MODEL         "NUM_MODEL = "          // 読み込むモデルの数
#define MODEL_FILENAME    "MODEL_FILENAME = "     // 読み込むモデルのファイル名

// キャラクター情報
#define CHARACTERSET      "CHARACTERSET"          // キャラクター情報読み込み開始の合図
#define END_CHARACTERSET  "END_CHARACTERSET"      // キャラクター情報読み込み終了の合図
#define JUMP              "JUMP = "               // ジャンプ量
#define GRAVITY           "GRAVITY = "            // 重力量
#define INERTIA           "INERTIA = "            // 慣性量
#define RIVISION_ANGLE    "RIVISION_ANGLE = "     // 向きを補正する倍率
#define COL_HEIGHT        "COL_HEIGHT = "         // 当たり判定をとる高さ
#define COL_RANGE         "COL_RANGE = "          // 当たり判定をとる範囲
#define NUM_PARTS         "NUM_PARTS = "          // キャラクターのパーツ数

// パーツ情報
#define PARTSSET          "PARTSSET"              // パーツ情報読み込み開始の合図
#define END_PARTSSET      "END_PARTSSET"          // パーツ情報読み込み終了の合図
#define INDEX             "INDEX = "              // 番号を読み取る合図
#define PARENT            "PARENT = "             // 親パーツの番号を読み取る合図

// モーション情報
#define MOTIONSET         "MOTIONSET"             // モーション情報読み込み開始の合図
#define END_MOTIONSET     "END_MOTIONSET"         // モーション情報読み込み終了の合図
#define COLLISIONSET      "COLLISIONSET"          // モーションの当たり判定情報読み込み開始の合図
#define END_COLLISIONSET  "END_COLLISIONSET"      // モーションの当たり判定情報読み込み終了の合図
#define ORBITSET          "ORBITSET"              // モーションの軌跡情報読み込み開始の合図
#define END_ORBITSET      "END_ORBITSET"          // モーションの軌跡情報読み込み終了の合図
#define RANGE             "RANGE = "              // 範囲を読み取る合図
#define XBLOCK            "XBLOCK = "             // 軌跡の横の分割数を読み取る合図
#define ZBLOCK            "ZBLOCK = "             // 軌跡の奥行の分割数を読み取る合図
#define OFFSET            "OFFSET = "             // オフセットを読み取る合図
#define LOOP              "LOOP = "               // モーションがループ再生するかどうかを読み取る合図
#define BLEND             "BLEND = "              // モーションブレンドの倍率を読み取る合図
#define NUM_KEY           "NUM_KEY = "            // モーションのキーフレームの数を読み取る合図

// 攻撃モーション情報
#define ATTACKSET         "ATTACKSET"             // 攻撃モーション情報読み込み開始の合図
#define END_ATTACKSET     "END_ATTACKSET"         // 攻撃モーション情報読み込み終了の合図
#define NEXT_MOTION       "NEXT_MOTION = "        // 次のモーションの番号

// 当たり判定情報
#define NUM_COL           "NUM_COL = "            // モーションの当たり判定情報の数を読み取る合図
#define COLLISION         "COLLISION"             // 当たり判定情報読み込み開始の合図
#define DAMAGE            "DAMAGE = "             // 当たった場合のダメージ
#define END_COLLISION     "END_COLLISION"         // 当たり判定情報読み込み終了の合図

// 軌跡情報
#define NUM_ORBIT         "NUM_ORBIT = "          // モーションの軌跡情報の数を読み取る合図
#define ORBIT		      "ORBIT"                 // 軌跡情報読み込み開始の合図
#define END_ORBIT	      "END_ORBIT"             // 軌跡情報読み込み終了の合図
#define TEX_INDEX         "TEX_INDEX = "          // 軌跡に張り付けるテクスチャの番号を読み取る合図
#define OFFSET1           "OFFSET1 = "            // １個目のオフセットを読み取る合図
#define OFFSET2           "OFFSET2 = "            // ２個目のオフセットを読み取る合図
#define OFFSETAMP1        "OFFSETAMP1 = "         // 軌跡のオフセット距離増幅値を読み取る合図(１個目)
#define OFFSETAMP2        "OFFSETAMP2 = "         // 軌跡のオフセット距離増幅値を読み取る合図(２個目)
#define COLUP             "COLUP = "              // 軌跡の頂点上側の色
#define COLDOWN           "COLDOWN = "            // 軌跡の頂点下側の色
#define ALPHA_DECAY_UP    "ALPHA_DECAY_UP = "     // 軌跡の頂点上側の透明度減衰値
#define ALPHA_DECAY_DOWN  "ALPHA_DECAY_DOWN = "   // 軌跡の頂点下側の透明度減衰値
#define START             "START = "              // 軌跡を出し始めるタイミングを読み取る合図
#define FINISH            "FINISH = "             // 軌跡をしまうタイミングを読み取る合図

// キーフレーム情報
#define KEYSET            "KEYSET"                // キーフレーム情報読み込み開始の合図
#define END_KEYSET        "END_KEYSET"            // キーフレーム情報読み込み終了の合図
#define FRAME             "FRAME = "              // キーフレームの再生フレーム数を読み取る合図

// キー情報
#define KEY               "KEY"                   // キー情報読み込み開始の合図
#define END_KEY           "END_KEY"               // キー情報読み込み終了の合図

//*****************************************************************************
//     静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CPlayerManager::CPlayerManager() : CCharacterManager()
{
	// 各種値のクリア
	m_nControllerIdx = 0;   // 使用するコントローラーの番号
}

//=============================================================================
//    デストラクタ
//=============================================================================
CPlayerManager::~CPlayerManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CPlayerManager *CPlayerManager::Create(char *pFileName, int nControllerIdx)
{
	CPlayerManager *pPlayerManager = NULL;  // プレイヤー管轄クラス型のポインタ
	if (pPlayerManager == NULL)
	{// メモリが空になっている
		pPlayerManager = new CPlayerManager;
		if (pPlayerManager != NULL)
		{// インスタンスを生成できた
			pPlayerManager->SetFileName(pFileName);
			pPlayerManager->SetControllerIdx(nControllerIdx);
			if (FAILED(pPlayerManager->Init()))
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

	return pPlayerManager;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CPlayerManager::Init(void)
{
	// プレイヤーのパーツ情報を抜き出す
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(GetFileName(), "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                         // 1行分読み取り用
			char *pStrCur = NULL;                       // 現在の先頭ポインタ
			char *pStr = NULL;                          // 先頭ポインタ保存用
			int nNumTexture = 0;                        // 読み込むテクスチャの数
			int nNumModel = 0;                          // 読み込むモデル数
			int nNumParts = 0;                          // 読み込むパーツ数
			int nCntTex = 0;                            // テクスチャを読み込んだ回数を数える
			int nCntModel = 0;                          // モデルを読み込んだ回数を数える
			int nCntParts = 0;                          // パーツ情報を読み込んだ回数を数える
			int nCntMotion = 0;                         // モーションを読み込んだ回数を数える
			int *pNumParent = NULL;                     // 親モデルの番号
			CMotionManager *pMotionManager = NULL;      // モーション管理クラスへのポインタ
			CTextureManager *pTextureManager = NULL;    // テクスチャ管轄クラスへのポインタ
			CModelManager *pModelManager = NULL;        // モデル管轄クラスへのポインタ
			CMotion *pMotion[CPlayer::STATE_MAX] = {};  // モーションクラスへのポインタ
			CModel **apModel = NULL;                    // モデルクラスへのポインタ
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ情報へのポインタ
			LPD3DXMESH pMesh = NULL;                    // メッシュ情報へのポインタ
			LPD3DXBUFFER pBuffMat = NULL;               // マテリアル情報へのポインタ
			DWORD nNumMat = 0;                          // マテリアル情報の数
			float fAccel = 0.0f;                        // プレイヤーの加速度情報
			float fInertia = 0.0f;                      // プレイヤーの慣性情報
			float fGravity = 0.0f;                      // プレイヤーの重力情報
			float fJumpPower = 0.0f;                    // プレイヤーのジャンプ量情報
			float fRivisionRot = 0.0f;                  // プレイヤーの向きを補正する倍率情報
			float fColHeight = 0.0f;                    // プレイヤーの当たり判定を取る高さ情報
			float fColRange = 0.0f;                     // プレイヤーの当たり判定を取る範囲情報

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
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// 読み込むテクスチャ数だった
								strcpy(pStr, pStrCur);
								nNumTexture = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTexture >= 1)
								{// 読み込むテクスチャ数が1つ以上ある
									pTextureManager = CTextureManager::Create(nNumTexture);
									SetTextureManager(pTextureManager);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// テクスチャのファイルパス名を読み込んだ
								if (pTextureManager != NULL && nCntTex < nNumTexture)
								{// テクスチャ管轄クラスへのポインタが確保されている
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
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
							{// 読み込むモデル数だった
								strcpy(pStr, pStrCur);
								nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
								if (nNumModel >= 1)
								{// モデル数が1つ以上ある
									pNumParent = new int[nNumModel];
									pModelManager = CModelManager::Create(nNumModel);
									SetModelManager(pModelManager);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
							{// モデルのファイルパス名を読み込んだ
								if (pModelManager != NULL && nCntModel < nNumModel)
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
									pModelManager->SetMesh(pMesh, nCntModel);
									pModelManager->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);

									// 使用した変数を初期化しておく
									pMesh = NULL;
									pBuffMat = NULL;
									nNumMat = 0;

									nCntModel++;  // モデルを読み込んだ回数を増やす
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, CHARACTERSET) == 0)
							{// キャラクター情報読み込み開始の合図があった
								apModel = ReadCharacterSet(pLine, pStrCur, pFile, &nNumParts, pNumParent, &fAccel, &fInertia, &fJumpPower, &fRivisionRot, &fGravity, &fColHeight, &fColRange);
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
				pMotionManager = CMotionManager::Create(CPlayer::STATE_MAX, nNumParts);
				if (pMotionManager != NULL)
				{// モーション管理クラスが作成できた
					for (int nCntMotion = 0; nCntMotion < CPlayer::STATE_MAX; nCntMotion++)
					{// 状態の数だけ繰りかえし
						pMotionManager->SetMotion(pMotion[nCntMotion], nCntMotion);
					}
				}
			}

			// プレイヤークラスを作成する
			CreatePlayer(apModel, pModelManager, pTextureManager, pMotionManager, nNumParts, pNumParent, fAccel, fInertia, fJumpPower, fRivisionRot, fGravity, fColHeight, fColRange);

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
void CPlayerManager::Uninit(void)
{
	// 共通の終了処理
	CCharacterManager::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CPlayerManager::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CPlayerManager::Draw(void)
{

}

//=============================================================================
//    プレイヤークラスを作製する処理
//=============================================================================
void CPlayerManager::CreatePlayer(CModel **apModel, CModelManager *pModelManager, CTextureManager *pTextureManager, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange)
{
	CModel **apModelCpy = NULL;                     // モデルクラスコピー用
	CMotionAttack *pMotionAttack = NULL;            // 攻撃モーションデータクラスコピー用
	CMotionCollision **pMotionCollision = NULL;     // モーションの当たり判定データクラスコピー用
	CMotionOrbit **pMotionOrbit = NULL;             // モーションの軌跡データクラスコピー用
	CMotion *pMotionCpy[CPlayer::STATE_MAX] = {};   // モージョンクラスコピー用
	CMotionManager *pMotionManagerCpy = NULL;       // モーション管轄クラスコピー用
	CPlayer *pPlayer = NULL;                        // プレイヤークラスへのポインタ

	if (apModel != NULL && pMotionManager != NULL && pModelManager != NULL)
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
						apModelCpy[nCntParts] = CModel::Create(apModel[nCntParts]->GetPos(), apModel[nCntParts]->GetRot(), pModelManager->GetMesh(nCntParts), pModelManager->GetBuffMat(nCntParts), pModelManager->GetNumMat(nCntParts), pModelManager->GetVtxMax(nCntParts), pModelManager->GetVtxMin(nCntParts), pModelManager->GetTexture(nCntParts));
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
			pMotionManagerCpy = CMotionManager::Create(CPlayer::STATE_MAX, nNumParts);
			if (pMotionManagerCpy != NULL)
			{// モーション管理クラスが作成できた
				for (int nCntMotion = 0; nCntMotion < CPlayer::STATE_MAX; nCntMotion++)
				{// プレイヤーの状態の数だけ繰り返し
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

							// 攻撃モーションデータクラスのコピー
							if (pMotionManager->GetMotion()[nCntMotion]->GetAttack() != NULL)
							{// 攻撃モーションデータクラスが生成されている
								pMotionAttack = new CMotionAttack;
								if (pMotionAttack != NULL)
								{// メモリが確保できた
								 // 各種値の設定
									pMotionAttack->SetNextMotionIdx(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetNextMotionIdx());
									pMotionAttack->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetStartTiming());
									pMotionAttack->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetFinishTiming());
									pMotionCpy[nCntMotion]->SetAttack(pMotionAttack);
								}
							}

							// 当たり判定データクラスのコピー
							if (pMotionManager->GetMotion()[nCntMotion]->GetNumColData() >= 1)
							{// 当たり判定データが１個以上ある
								pMotionCollision = new CMotionCollision*[pMotionManager->GetMotion()[nCntMotion]->GetNumColData()];
								if (pMotionCollision != NULL)
								{// メモリが確保できた
									for (int nCntCol = 0; nCntCol < pMotionManager->GetMotion()[nCntMotion]->GetNumColData(); nCntCol++)
									{// 当たり判定データの数だけ繰り返し
										pMotionCollision[nCntCol] = new CMotionCollision;
										if (pMotionCollision[nCntCol] != NULL)
										{// メモリが確保できた
											pMotionCollision[nCntCol]->SetModelIdx(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetModelIdx());
											pMotionCollision[nCntCol]->SetOffsetPos(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetOffsetPos());
											pMotionCollision[nCntCol]->SetRange(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetRange());
											pMotionCollision[nCntCol]->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetStartTiming());
											pMotionCollision[nCntCol]->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetFinishTiming());
											pMotionCollision[nCntCol]->SetDamage(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetDamage());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetCollision(pMotionCollision);
								pMotionCpy[nCntMotion]->SetNumColData(pMotionManager->GetMotion()[nCntMotion]->GetNumColData());
							}

							// 軌跡データクラスのコピー
							if (pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData() >= 1)
							{// 当たり判定データが１個以上ある
								pMotionOrbit = new CMotionOrbit*[pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData()];
								if (pMotionOrbit != NULL)
								{// メモリが確保できた
									for (int nCntOrbit = 0; nCntOrbit < pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData(); nCntOrbit++)
									{// 当たり判定データの数だけ繰り返し
										pMotionOrbit[nCntOrbit] = new CMotionOrbit;
										if (pMotionOrbit[nCntOrbit] != NULL)
										{// メモリが確保できた
											pMotionOrbit[nCntOrbit]->SetModelIdx(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetModelIdx());
											pMotionOrbit[nCntOrbit]->SetTexIdx(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetTexIdx());
											pMotionOrbit[nCntOrbit]->SetOffsetPos1(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos1());
											pMotionOrbit[nCntOrbit]->SetOffsetPos2(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos2());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp1(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp1());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp2(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp2());
											pMotionOrbit[nCntOrbit]->SetColUp(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColUp());
											pMotionOrbit[nCntOrbit]->SetColDown(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColDown());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayUp());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayDown());
											pMotionOrbit[nCntOrbit]->SetXBlock(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetXBlock());
											pMotionOrbit[nCntOrbit]->SetYBlock(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetYBlock());
											pMotionOrbit[nCntOrbit]->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetStartTiming());
											pMotionOrbit[nCntOrbit]->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetFinishTiming());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetOrbit(pMotionOrbit);
								pMotionCpy[nCntMotion]->SetNumOrbitData(pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());
							}

							// モーション管轄クラスにポインタを渡す
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// プレイヤークラスを作成する
		if (pPlayer == NULL)
		{// メモリが確保できる状態である
			pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), apModelCpy, pMotionManagerCpy, pTextureManager, nNumParts, fAccel, fInertia, fJumpPower, fGravity, fRivisionRot, fColHeight, fColRange, m_nControllerIdx, PLAYER_PRIORITY);
		}
	}
}


//=============================================================================
//    使用するコントローラーの番号を設定する
//=============================================================================
void CPlayerManager::SetControllerIdx(const int nControllerIdx)
{
	m_nControllerIdx = nControllerIdx;
}

//=============================================================================
//    使用するコントローラーの番号を設定する
//=============================================================================
int CPlayerManager::GetControllerIdx(void)
{
	return m_nControllerIdx;
}