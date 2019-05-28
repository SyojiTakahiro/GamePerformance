//*****************************************************************************
//
//     キャラクター管轄の処理[characterManager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "character.h"
#include "functionlib.h"
#include "model.h"
#include "motion.h"
#include "modelManager.h"
#include "textureManager.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
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
CCharacterManager::CCharacterManager()
{
	// 各種値のクリア
	strcpy(m_aFileName, "\0");    // プレイヤーデータのスクリプトファイル名
	m_pModelManager = NULL;       // モデル管轄クラスへのポインタ
	m_pTextureManager = NULL;     // テクスチャ管轄クラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CCharacterManager::~CCharacterManager()
{

}

//=============================================================================
//    終了処理
//=============================================================================
void CCharacterManager::Uninit(void)
{
	// モデル管轄クラスの破棄
	if (m_pModelManager != NULL)
	{// ポインタが確保されている
		m_pModelManager->Uninit();

		// メモリの開放
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// テクスチャ管轄クラスの破棄
	if (m_pTextureManager != NULL)
	{// ポインタが確保されている
		m_pTextureManager->Uninit();

		// メモリの開放
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    キャラクター情報を読み取る処理
//=============================================================================
CModel **CCharacterManager::ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pAccel, float *pInertia, float *pJumpPower, float *pRivisionRot, float *pGravity, float *pColHeight, float *pColRange)
{
	int nCntParts = 0;
	int nNumParts = 0;
	CModel **apModel = NULL;
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, MOVE) == 0)
		{// 移動量がある
			*pAccel = CFunctionLib::ReadFloat(pStrCur, MOVE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, JUMP) == 0)
		{// ジャンプ量がある
			*pJumpPower = CFunctionLib::ReadFloat(pStrCur, JUMP);
		}
		else if (CFunctionLib::Memcmp(pStrCur, GRAVITY) == 0)
		{// 重力量がある
			*pGravity = CFunctionLib::ReadFloat(pStrCur, GRAVITY);
		}
		else if (CFunctionLib::Memcmp(pStrCur, INERTIA) == 0)
		{// 慣性量がある
			*pInertia = CFunctionLib::ReadFloat(pStrCur, INERTIA);
		}
		else if (CFunctionLib::Memcmp(pStrCur, RIVISION_ANGLE) == 0)
		{// 向きを補正する倍率量がある
			*pRivisionRot = CFunctionLib::ReadFloat(pStrCur, RIVISION_ANGLE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_HEIGHT) == 0)
		{//	当たり判定を取る高さがある
			*pColHeight = CFunctionLib::ReadFloat(pStrCur, COL_HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// 当たり判定を取る範囲がある
			*pColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		if (CFunctionLib::Memcmp(pStrCur, NUM_PARTS) == 0)
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
CModel *CCharacterManager::ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel)
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
CMotion *CCharacterManager::ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion)
{
	int nCntKeySet = 0;                           // キーフレームフレーム情報を読み込んだ回数
	CMotionAttack *pMotionAttack = NULL;          // 攻撃モーションデータクラスへのポインタ
	CMotionCollision **pMotionCollision = NULL;   // モーションの当たり判定データクラスへのポインタ
	int nNumColData = 0;                          // 当たり判定データの数
	CMotionOrbit **pMotionOrbit = NULL;           // モーションの軌跡データクラスへのポインタ
	int nNumOrbitData = 0;                        // 軌跡データの数
	bool bLoop = false;                           // ループするかしないか
	float fBlendMag = 0.0f;                       // ブレンド倍率
	int nNumKey = 0;                              // キーフレームの総数
	int *pPlaybackKey = NULL;                     // 再生フレーム数(複数あるので動的確保)
	D3DXVECTOR3 **pAddPos = NULL;                 // モデルの座標
	D3DXVECTOR3 **pDestRot = NULL;                // モデルの向き
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
		else if (CFunctionLib::Memcmp(pStrCur, ATTACKSET) == 0)
		{// 当たり判定情報読み込み開始の合図だった
			pMotionAttack = ReadMotionAttackSet(pLine, pStrCur, pFile);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISIONSET) == 0)
		{// 当たり判定情報読み込み開始の合図だった
			pMotionCollision = ReadMotionCollisionSet(pLine, pStrCur, pFile, &nNumColData);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ORBITSET) == 0)
		{// 軌跡情報読み込み開始の合図だった
			pMotionOrbit = ReadMotionOrbitSet(pLine, pStrCur, pFile, &nNumOrbitData);
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
		    // 攻撃モーションデータクラスを設定する
			pMotion->SetAttack(pMotionAttack);

			// 当たり判定データクラスを設定する
			pMotion->SetCollision(pMotionCollision);
			pMotion->SetNumColData(nNumColData);

			// 軌跡データクラスを設定する
			pMotion->SetOrbit(pMotionOrbit);
			pMotion->SetNumOrbitData(nNumOrbitData);

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
//    モーションの攻撃モーション情報を読み取る処理
//=============================================================================
CMotionAttack *CCharacterManager::ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile)
{
	CMotionAttack *pMotionAttack = NULL;   // 攻撃モーションデータクラスへのポインタ
	pMotionAttack = new CMotionAttack;     // メモリを確保する
	if (pMotionAttack != NULL)
	{// メモリを確保できた
		while (1)
		{// ループ開始
			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, NEXT_MOTION) == 0)
			{// 次のモーションの番号情報だった
				pMotionAttack->SetNextMotionIdx(CFunctionLib::ReadInt(pStrCur, NEXT_MOTION));
			}
			else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
			{// 軌跡を出し始めるタイミング情報だった
				pMotionAttack->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
			}
			else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
			{// 軌跡をしまうタイミング情報だった
				pMotionAttack->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
			}
			else if (CFunctionLib::Memcmp(pStrCur, END_ATTACKSET) == 0)
			{// 攻撃モーションデータ読み込み終了の合図だった
				break;  // ループ終了
			}
		}
	}

	return pMotionAttack;
}

//=============================================================================
//    モーションの当たり判定情報を読み取る処理
//=============================================================================
CMotionCollision **CCharacterManager::ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData)
{
	int nCntCollision = 0;                       // 当たり判定データを読み込んだ回数
	CMotionCollision **pMotionCollision = NULL;  // モーションの当たり判定データクラスへのポインタ

	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, NUM_COL) == 0)
		{// 当たり判定データの数がある
			*pNumColData = CFunctionLib::ReadInt(pStrCur, NUM_COL);
			if (*pNumColData >= 1)
			{// 当たり判定データが１個以上ある
				pMotionCollision = new CMotionCollision*[*pNumColData];
				if (pMotionCollision != NULL)
				{// メモリが確保できた
					for (int nCntCol = 0; nCntCol < *pNumColData; nCntCol++)
					{// 当たり判定データの数だけ繰り返し
						pMotionCollision[nCntCol] = new CMotionCollision;
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
		{// 当たり判定データ読み込み開始の合図だった
			while (1)
			{// ループ開始
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
				if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
				{// 当たり判定を取るモデルの番号情報だった
					pMotionCollision[nCntCollision]->SetModelIdx(CFunctionLib::ReadInt(pStrCur, INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET) == 0)
				{// 当たり判定を取るモデルからのオフセット距離情報だった
					pMotionCollision[nCntCollision]->SetOffsetPos(CFunctionLib::ReadVector3(pStrCur, OFFSET));
				}
				else if (CFunctionLib::Memcmp(pStrCur, RANGE) == 0)
				{// 当たり判定を取る範囲情報だった
					pMotionCollision[nCntCollision]->SetRange(CFunctionLib::ReadFloat(pStrCur, RANGE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
				{// 当たり判定をし始めるタイミング情報だった
					pMotionCollision[nCntCollision]->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
				}
				else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
				{// 当たり判定を終了するタイミング情報だった
					pMotionCollision[nCntCollision]->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
				}
				else if (CFunctionLib::Memcmp(pStrCur, DAMAGE) == 0)
				{// 当たった時のダメージ量情報だった
					pMotionCollision[nCntCollision]->SetDamage(CFunctionLib::ReadInt(pStrCur, DAMAGE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_COLLISION) == 0)
				{// 当たり判定データ読み込み終了の合図だった
					nCntCollision++;
					break;  // ループ終了
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_COLLISIONSET) == 0)
		{// 当たり判定情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return pMotionCollision;
}

//=============================================================================
//    モーションの軌跡情報を読み取る処理
//=============================================================================
CMotionOrbit **CCharacterManager::ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData)
{
	int nCntOrbit = 0;                   // 軌跡データを読み込んだ回数
	CMotionOrbit **pMotionOrbit = NULL;  // モーションの軌跡データクラスへのポインタ

	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, NUM_ORBIT) == 0)
		{// 軌跡データの数がある
			*pNumOrbitData = CFunctionLib::ReadInt(pStrCur, NUM_ORBIT);
			if (*pNumOrbitData >= 1)
			{// 当たり判定データが１個以上ある
				pMotionOrbit = new CMotionOrbit*[*pNumOrbitData];
				if (pMotionOrbit != NULL)
				{// メモリが確保できた
					for (int nCntOr = 0; nCntOr < *pNumOrbitData; nCntOr++)
					{// 当たり判定データの数だけ繰り返し
						pMotionOrbit[nCntOr] = new CMotionOrbit;
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, ORBIT) == 0)
		{// 軌跡データ読み込み開始の合図だった
			while (1)
			{// ループ開始
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
				if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
				{// 軌跡を出すモデルの番号情報だった
					pMotionOrbit[nCntOrbit]->SetModelIdx(CFunctionLib::ReadInt(pStrCur, INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, TEX_INDEX) == 0)
				{// 軌跡に張り付けるテクスチャの番号情報だった
					pMotionOrbit[nCntOrbit]->SetTexIdx(CFunctionLib::ReadInt(pStrCur, TEX_INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET1) == 0)
				{// 軌跡を出すモデルからのオフセット距離情報(１個目)だった
					pMotionOrbit[nCntOrbit]->SetOffsetPos1(CFunctionLib::ReadVector3(pStrCur, OFFSET1));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET2) == 0)
				{// 軌跡を出すモデルからのオフセット距離情報(２個目)だった
					pMotionOrbit[nCntOrbit]->SetOffsetPos2(CFunctionLib::ReadVector3(pStrCur, OFFSET2));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSETAMP1) == 0)
				{// 軌跡のオフセット距離増幅値(1個目)だった
					pMotionOrbit[nCntOrbit]->SetOffsetAmp1(CFunctionLib::ReadVector3(pStrCur, OFFSETAMP1));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSETAMP2) == 0)
				{// 軌跡のオフセット距離増幅値(2個目)だった
					pMotionOrbit[nCntOrbit]->SetOffsetAmp2(CFunctionLib::ReadVector3(pStrCur, OFFSETAMP2));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLUP) == 0)
				{// 軌跡の頂点上側の色だった
					pMotionOrbit[nCntOrbit]->SetColUp(CFunctionLib::ReadVector4(pStrCur, COLUP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLDOWN) == 0)
				{// 軌跡の頂点下側の色だった
					pMotionOrbit[nCntOrbit]->SetColDown(CFunctionLib::ReadVector4(pStrCur, COLDOWN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ALPHA_DECAY_UP) == 0)
				{// 軌跡の頂点上側の透明度減衰値だった
					pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(CFunctionLib::ReadFloat(pStrCur, ALPHA_DECAY_UP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ALPHA_DECAY_DOWN) == 0)
				{// 軌跡の頂点下側の透明度減衰値だった
					pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(CFunctionLib::ReadFloat(pStrCur, ALPHA_DECAY_DOWN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
				{// 軌跡の横の分割数情報だった
					pMotionOrbit[nCntOrbit]->SetXBlock(CFunctionLib::ReadInt(pStrCur, XBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ZBLOCK) == 0)
				{// 軌跡の縦の分割数情報だった
					pMotionOrbit[nCntOrbit]->SetYBlock(CFunctionLib::ReadInt(pStrCur, ZBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
				{// 軌跡を出し始めるタイミング情報だった
					pMotionOrbit[nCntOrbit]->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
				}
				else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
				{// 軌跡をしまうタイミング情報だった
					pMotionOrbit[nCntOrbit]->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_ORBIT) == 0)
				{// 軌跡データ読み込み終了の合図だった
					nCntOrbit++;
					break;  // ループ終了
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_ORBITSET) == 0)
		{// 軌跡情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return pMotionOrbit;
}

//=============================================================================
//    キーフレーム情報を読み取る処理
//=============================================================================
void CCharacterManager::ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
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
void CCharacterManager::ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
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
//    キャラクターデータのスクリプトファイル名を設定する処理
//=============================================================================
void CCharacterManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    モデル管轄クラスへのポインタを設定する処理
//=============================================================================
void CCharacterManager::SetModelManager(CModelManager *pModelManager)
{
	m_pModelManager = pModelManager;
}

//=============================================================================
//    テクスチャ管轄クラスへのポインタを設定する処理
//=============================================================================
void CCharacterManager::SetTextureManager(CTextureManager *pTextureManager)
{
	m_pTextureManager = pTextureManager;
}

//=============================================================================
//    キャラクターデータのスクリプトファイル名を取得する処理
//=============================================================================
char *CCharacterManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    モデル管轄クラスへのポインタを取得する処理
//=============================================================================
CModelManager *CCharacterManager::GetModelManager(void)
{
	return m_pModelManager;
}

//=============================================================================
//    テクスチャ管轄クラスへのポインタを取得する処理
//=============================================================================
CTextureManager *CCharacterManager::GetTextureManager(void)
{
	return m_pTextureManager;
}