//*****************************************************************************
//
//     配置物の処理[object.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "object.h"
#include "manager.h"
#include "system.h"
#include "model.h"
#include "motion.h"
#include "modelManager.h"
#include "functionlib.h"
#include "collision.h"
#include "star.h"
#include "mode.h"

//*****************************************************************************
//    マクロ宣言
//*****************************************************************************
#define MAX_OBJECT_MOTION (1)                     // モーションのデータ数
#define OBJECT_MAX_LIFE   (3)                     // オブジェクトの体力

// 値読み込みをする際の目印となる文字列
// 共通
#define POS               "POS = "                // 座標を読み取る合図
#define ROT               "ROT = "                // 向きを読み取る合図
#define MOVE              "MOVE = "               // 移動量を読み取る合図

// その他
#define NUM_MODEL         "NUM_MODEL = "          // 読み込むモデルの数
#define MODEL_FILENAME    "MODEL_FILENAME = "     // 読み込むモデルのファイル名

// 配置物情報
#define MODELSET          "MODELSET"              // 配置物情報読み込み開始の合図
#define END_MODELSET      "END_MODELSET"          // 配置物情報読み込み終了の合図
#define NUM_PARTS         "NUM_PARTS = "          // 配置物のパーツ数

// パーツ情報
#define PARTSSET          "PARTSSET"              // パーツ情報読み込み開始の合図
#define END_PARTSSET      "END_PARTSSET"          // パーツ情報読み込み終了の合図
#define INDEX             "INDEX = "              // 番号を読み取る合図
#define PARENT            "PARENT = "             // 親パーツの番号を読み取る合図
#define MODEL_IDX         "MODEL_IDX = "          // 使用するモデルの番号

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

// モーションの当たり判定情報
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

// 当たり判定情報
#define NUM_COLLISION     "NUM_COLLISION = "      // 当たり判定の数
#define COLLISIONSET      "COLLISIONSET"          // 当たり判定情報読み込み開始の合図
#define END_COLLISIONSET  "END_COLLISIONSET"      // 当たり判定情報読み込み終了の合図
#define RETURN            "RETURN = "             // 当たった際に押し戻すかどうか
#define TYPE              "TYPE = "               // 種類
#define WIDTH             "WIDTH = "              // 横幅
#define HEIGHT            "HEIGHT = "             // 縦幅
#define DEPTH             "DEPTH = "              // 奥行

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//*****************************************************************************
//    CObjectの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CObject::CObject(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_nNumParts = 0;                            // 使用するパーツ数
	m_nNumCollision = 0;                        // 当たり判定の数
	m_apModel = NULL;                           // モデルクラスへのポインタ
	m_pMotionManager = NULL;                    // モーションクラスへのポインタ
	m_apCollision = NULL;                       // コリジョンクラスへのポインタ
	D3DXMatrixIdentity(&m_MtxWorld);            // ワールドマトリックス
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 現在の座標
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 前回の座標
	m_PosStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 生成時の座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 現在の向き
	m_RotStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 生成時の向き
	m_Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 大きさ
	m_RollSpeed = D3DXVECTOR3(0.0f,0.0f,0.0f);  // 向きを回転させるスピード
	m_bBreak = false;                           // 壊せるオブジェクトかどうか
	m_nLife = 0;                                // 体力
}

//=============================================================================
//    デストラクタ
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, int nNumParts, int nNumCollision, CModel **pModel, CMotionManager *pMotionManager, CCollision **pCollision, bool bBreak, int nPriority)
{
	CObject *pObject = NULL;               // 配置物クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pObject == NULL)
		{// メモリが空になっている
			pObject = new CObject(nPriority);
			if (pObject != NULL)
			{// インスタンスを生成できた
				// 各種値の設定
				pObject->SetType(nType);                   // 配置物の種類
				pObject->SetNumParts(nNumParts);           // パーツ数
				pObject->SetNumCollision(nNumCollision);   // 当たり判定の数
				pObject->SetModel(pModel);                 // モデルクラスへのポインタ
				pObject->SetMotionManager(pMotionManager); // モーションクラスへのポインタ
				pObject->SetCollision(pCollision);         // コリジョンクラスへのポインタ
				pObject->SetPos(pos);                      // 現在の座標
				pObject->SetPosOld(pos);                   // 前回の座標
				pObject->SetPosStd(pos);                   // 生成時の座標
				pObject->SetRot(rot);                      // 現在の向き
				pObject->SetRotStd(rot);                   // 生成時の向き
				pObject->SetScale(scale);                  // 大きさ
				pObject->SetRollSpeed(RollSpeed);          // 回転スピード
				pObject->SetBreak(bBreak);                 // 破壊できるかどうか

				if (FAILED(pObject->Init()))
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

	return pObject;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CObject::Init(void)
{
	// 当たり判定を調整する
	CBoxCollider *pBoxCollider = NULL;
	CCircleCollider *pCirclcleCollider = NULL;
	CSphereCollider *pSphereCollider = NULL;
	if (m_apCollision != NULL)
	{// メモリが確保されている
		for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
		{// 当たり判定の数だけ繰り返し
			if (m_apCollision[nCntCol] != NULL)
			{// メモリが確保されている
				if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_BOX)
				{// ボックスコライダーだったら
					pBoxCollider = (CBoxCollider*)m_apCollision[nCntCol];
					pBoxCollider->SetWidth(pBoxCollider->GetWidth() * m_Scale.x);
					pBoxCollider->SetHeight(pBoxCollider->GetHeight() * m_Scale.y);
					pBoxCollider->SetDepth(pBoxCollider->GetDepth() * m_Scale.z);
				}
				else if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_CIRCLE)
				{// サークルコライダーだったら
					pCirclcleCollider = (CCircleCollider*)m_apCollision[nCntCol];
				}
				else if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_SPHERE)
				{// スフィアコライダーだったら
					pSphereCollider = (CSphereCollider*)m_apCollision[nCntCol];
				}
			}
		}
	}

	// 体力を設定
	m_nLife = OBJECT_MAX_LIFE;

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CObject::Uninit(void)
{
	// モデルの破棄
	if (m_apModel != NULL)
	{// メモリが確保されている
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// 使用できるモデル数分繰り返し
			if (m_apModel[nCntModel] != NULL)
			{// ポインタが確保されている
				m_apModel[nCntModel]->Uninit();

				// メモリの開放
				delete m_apModel[nCntModel];
				m_apModel[nCntModel] = NULL;
			}
		}
		// メモリの開放
		delete[] m_apModel;
		m_apModel = NULL;
	}

	// モーションの破棄
	if (m_pMotionManager != NULL)
	{// ポインタが確保されている
		m_pMotionManager->Uninit();

		// メモリの開放
		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}

	// コリジョンの破棄
	if (m_apCollision != NULL)
	{// メモリが確保されている
		for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
		{// 使用できるモデル数分繰り返し
			if (m_apCollision[nCntCol] != NULL)
			{// ポインタが確保されている
				m_apCollision[nCntCol]->Uninit();

				// メモリの開放
				delete m_apCollision[nCntCol];
				m_apCollision[nCntCol] = NULL;
			}
		}
		// メモリの開放
		delete[] m_apCollision;
		m_apCollision = NULL;
	}

	// 死亡フラグを立てる
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CObject::Update(void)
{
	if (m_pMotionManager != NULL)
	{
		m_pMotionManager->Update(m_apModel);
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CObject::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 法線情報を正規化する(スケーリング時に法線情報がいじられるので)
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

		    // ワールドマトリックス計算処理
			SetMtxWorld(pDevice);

			if (m_apModel != NULL)
			{// メモリが確保できた
			    // モデルの描画
				for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
				{// モデルを生成する数だけ繰り返し
				    // モデルの描画処理
					m_apModel[nCntModel]->Draw();
				}
			}

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
			if (m_apCollision != NULL)
			{// メモリが確保できた
				// コリジョンの描画
				for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
				{// 当たり判定の数だけ繰り返し
					m_apCollision[nCntCol]->Draw();
				}
			}

			// 法線を正規化しない
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
		}
	}
}

//=============================================================================
//    描画デバイスにワールドマトリックスを設定する処理
//=============================================================================
void CObject::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxScale, mtxRot, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 大きさを反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxScale);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    当たり判定処理
//=============================================================================
bool CObject::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand, CScene *pRideObject)
{
	bool bCol = false;   // 当たったかどうか
	CBoxCollider *pBoxCollider = NULL;
	CCircleCollider *pCirclcleCollider = NULL;
	CSphereCollider *pSphereCollider = NULL;

	if (m_apCollision != NULL)
	{// メモリが確保されている
		for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
		{// 当たり判定の数だけ繰り返し
			if (m_apCollision[nCntCol] != NULL)
			{// メモリが確保されている
				if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_BOX)
				{// ボックスコライダーだったら
					pBoxCollider = (CBoxCollider*)m_apCollision[nCntCol];
					if (pBoxCollider->Collision(pPos, pPosOld, pMove, ColRange, pLand) == true)
					{// 箱に当たっている
						bCol = true;
					}
				}
				else if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_CIRCLE)
				{// サークルコライダーだったら
					pCirclcleCollider = (CCircleCollider*)m_apCollision[nCntCol];
					if (pCirclcleCollider->Collision(pPos, pPosOld, pMove, ColRange, pLand) == true)
					{// 箱に当たっている
						bCol = true;
					}
				}
				else if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_SPHERE)
				{// スフィアコライダーだったら
					pSphereCollider = (CSphereCollider*)m_apCollision[nCntCol];
					if (pSphereCollider->Collision(pPos, pPosOld, pMove, ColRange, pLand) == true)
					{// 箱に当たっている
						bCol = true;
					}
				}
			}
		}
	}

	return bCol;
}


//=============================================================================
//    配置物のダメージ処理
//=============================================================================
void CObject::Damage(int nDamage)
{
	if (m_bBreak == true)
	{// 壊れる配置物なら
		m_nLife -= nDamage;

		if (m_nLife <= 0)
		{// 体力がなくなった
			Uninit();
		}
	}
}

//=============================================================================
//    配置物の種類を設定する
//=============================================================================
void CObject::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    パーツ数を設定する
//=============================================================================
void CObject::SetNumParts(const int nNumParts)
{
	m_nNumParts = nNumParts;
}

//=============================================================================
//    当たり判定の数を設定する
//=============================================================================
void CObject::SetNumCollision(const int nNumCollision)
{
	m_nNumCollision = nNumCollision;
}

//=============================================================================
//    モデルクラスへのポインタを設定する
//=============================================================================
void CObject::SetModel(CModel **pModel)
{
	m_apModel = pModel;
}

//=============================================================================
//    モーションクラスへのポインタを設定する
//=============================================================================
void CObject::SetMotionManager(CMotionManager *pMotionManager)
{
	m_pMotionManager = pMotionManager;
}

//=============================================================================
//    コリジョンクラスへのポインタを設定する
//=============================================================================
void CObject::SetCollision(CCollision **pCollision)
{
	m_apCollision = pCollision;
}

//=============================================================================
//    現在の座標を設定する
//=============================================================================
void CObject::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    前回の座標を設定する
//=============================================================================
void CObject::SetPosOld(const D3DXVECTOR3 posold)
{
	m_PosOld = posold;
}

//=============================================================================
//    生成時の座標を設定する
//=============================================================================
void CObject::SetPosStd(const D3DXVECTOR3 posstd)
{
	m_PosStd = posstd;
}

//=============================================================================
//    現在の向きを設定する
//=============================================================================
void CObject::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    生成時の向きを設定する
//=============================================================================
void CObject::SetRotStd(const D3DXVECTOR3 rotstd)
{
	m_RotStd = rotstd;
}

//=============================================================================
//    大きさを設定する
//=============================================================================
void CObject::SetScale(const D3DXVECTOR3 scale)
{
	m_Scale = scale;
}

//=============================================================================
//    回転スピードを設定する
//=============================================================================
void CObject::SetRollSpeed(const D3DXVECTOR3 RollSpeed)
{
	m_RollSpeed = RollSpeed;
}

//=============================================================================
//    破壊できるかどうかを設定する
//=============================================================================
void CObject::SetBreak(const bool bBreak)
{
	m_bBreak = bBreak;
}

//=============================================================================
//    配置物の種類を取得する
//=============================================================================
int CObject::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    パーツ数を取得する
//=============================================================================
int CObject::GetNumParts(void)
{
	return m_nNumParts;
}

//=============================================================================
//    当たり判定の数を取得する
//=============================================================================
int CObject::GetNumCollision(void)
{
	return m_nNumCollision;
}

//=============================================================================
//    モデルクラスへのポインタを取得する
//=============================================================================
CModel **CObject::GetModel(void)
{
	return m_apModel;
}

//=============================================================================
//    モーションクラスへのポインタを取得する
//=============================================================================
CMotionManager *CObject::GetMotionManager(void)
{
	return m_pMotionManager;
}

//=============================================================================
//    コリジョンクラスへのポインタを取得する
//=============================================================================
CCollision **CObject::GetCollision(void)
{
	return m_apCollision;
}

//=============================================================================
//    現在の座標を取得する
//=============================================================================
D3DXVECTOR3 CObject::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    前回の座標を取得する
//=============================================================================
D3DXVECTOR3 CObject::GetPosOld(void)
{
	return m_PosOld;
}

//=============================================================================
//    生成時の座標を取得する
//=============================================================================
D3DXVECTOR3 CObject::GetPosStd(void)
{
	return m_PosStd;
}

//=============================================================================
//    現在の向きを取得する
//=============================================================================
D3DXVECTOR3 CObject::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    生成時の向きを取得する
//=============================================================================
D3DXVECTOR3 CObject::GetRotStd(void)
{
	return m_RotStd;
}

//=============================================================================
//    大きさを取得する
//=============================================================================
D3DXVECTOR3 CObject::GetScale(void)
{
	return m_Scale;
}

//=============================================================================
//    回転スピードを取得する
//=============================================================================
D3DXVECTOR3 CObject::GetRollSpeed(void)
{
	return m_RollSpeed;
}

//=============================================================================
//    破壊できるかどうかを取得する
//=============================================================================
bool CObject::GetBreak(void)
{
	return m_bBreak;
}

//=============================================================================
//    コンストラクタ
//=============================================================================
CObjectStar::CObjectStar(int nPriority, OBJTYPE objType) : CObject(nPriority, objType)
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CObjectStar::~CObjectStar()
{

}

//=============================================================================
//    破壊できるかどうかを取得する
//=============================================================================
CObjectStar *CObjectStar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, int nNumParts, int nNumCollision, CModel **pModel, CMotionManager *pMotionManager, CCollision **pCollision, bool bBreak, int nPriority)
{
	CObjectStar *pObjectGoal = NULL;       // ゴール用配置物クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pObjectGoal == NULL)
		{// メモリが空になっている
			pObjectGoal = new CObjectStar(nPriority);
			if (pObjectGoal != NULL)
			{// インスタンスを生成できた
			    // 各種値の設定
				pObjectGoal->SetType(nType);                   // 配置物の種類
				pObjectGoal->SetNumParts(nNumParts);           // パーツ数
				pObjectGoal->SetNumCollision(nNumCollision);   // 当たり判定の数
				pObjectGoal->SetModel(pModel);                 // モデルクラスへのポインタ
				pObjectGoal->SetMotionManager(pMotionManager); // モーションクラスへのポインタ
				pObjectGoal->SetCollision(pCollision);         // コリジョンクラスへのポインタ
				pObjectGoal->SetPos(pos);                      // 現在の座標
				pObjectGoal->SetPosOld(pos);                   // 前回の座標
				pObjectGoal->SetPosStd(pos);                   // 生成時の座標
				pObjectGoal->SetRot(rot);                      // 現在の向き
				pObjectGoal->SetRotStd(rot);                   // 生成時の向き
				pObjectGoal->SetScale(scale);                  // 大きさ
				pObjectGoal->SetRollSpeed(RollSpeed);          // 回転スピード
				pObjectGoal->SetBreak(bBreak);                 // 破壊できるかどうか

				if (FAILED(pObjectGoal->Init()))
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

	return pObjectGoal;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CObjectStar::Init(void)
{
	if (FAILED(CObject::Init()))
	{// 初期化に失敗した
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CObjectStar::Uninit(void)
{
	// 共通の終了処理
	CObject::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CObjectStar::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CObjectStar::Draw(void)
{
	// 共通の終了処理
	CObject::Draw();
}

//=============================================================================
//    死んだときの処理
//=============================================================================
void CObjectStar::Death(void)
{
	// 星を生成する
	for (int nCntStar = 0; nCntStar < 10; nCntStar++)
	{
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		CStar::Create(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(fAngle) * 10.0f, 20.0f, cosf(fAngle) * 10.0f));
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CManager::GetGame()->SetStarObject();
	}
}

//*****************************************************************************
//    CObjectManagerの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CObjectManager::CObjectManager()
{
	// 各種値のクリア
	strcpy(m_aFileName, "\0");   // ファイル名
	m_nNumParts = 0;             // パーツ数
	m_nNumCollision = 0;         // 当たり判定の数
	m_pNumParent = NULL;         // 親モデルの番号
	m_pNumColParent = NULL;      // 当たり判定の親番号
	m_pNumModelIdx = NULL;       // 使用するモデルの番号
	m_apModel = NULL;            // モデルクラスへのポインタ
	m_pModelManager = NULL;      // モデル管轄クラスへのポインタ
	m_pMotionManager = NULL;     // モーションクラスへのポインタ
	m_apCollision = NULL;        // コリジョンクラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CObjectManager::~CObjectManager()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CObjectManager *CObjectManager::Create(char *pFileName, CModelManager *pModelManager)
{
	CObjectManager *pObjectManager = NULL;  // プレイヤー管轄クラス型のポインタ
	if (pObjectManager == NULL)
	{// メモリが空になっている
		pObjectManager = new CObjectManager;
		if (pObjectManager != NULL)
		{// インスタンスを生成できた
			pObjectManager->SetFileName(pFileName);
			pObjectManager->SetModelManager(pModelManager);
			if (FAILED(pObjectManager->Init()))
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

	return pObjectManager;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CObjectManager::Init(void)
{
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
			int nNumModel = 0;                          // 読み込むモデル数
			int nCntModel = 0;                          // モデルを読み込んだ回数を数える
			int nCntMotion = 0;                         // モーションを読み込んだ回数を数える
			CMotion *pMotion[MAX_OBJECT_MOTION] = {};   // モーションクラスへのポインタ
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ情報へのポインタ
			LPD3DXMESH pMesh = NULL;                    // メッシュ情報へのポインタ
			LPD3DXBUFFER pBuffMat = NULL;               // マテリアル情報へのポインタ
			DWORD nNumMat = 0;                          // マテリアル情報の数

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
							if (CFunctionLib::Memcmp(pStrCur, MODELSET) == 0)
							{// モデル情報読み込み開始の合図があった
								m_apModel = ReadModelSet(pLine, pStrCur, pFile, &m_nNumParts);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COLLISIONSET) == 0)
							{// 当たり判定情報読み込み開始の合図があった
								m_apCollision = ReadCollisionSet(pLine, pStrCur, pFile, &m_nNumCollision);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOTIONSET) == 0)
							{// モーション情報読み込み開始の合図があった
								pMotion[nCntMotion] = ReadMotionSet(pLine, pStrCur, pFile, &m_nNumParts, pMotion[nCntMotion], nCntMotion);
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
			if (m_pMotionManager == NULL)
			{// ポインタが確保されている
				m_pMotionManager = CMotionManager::Create(MAX_OBJECT_MOTION, m_nNumParts);
				if (m_pMotionManager != NULL)
				{// モーション管理クラスが作成できた
					for (int nCntMotion = 0; nCntMotion < MAX_OBJECT_MOTION; nCntMotion++)
					{// 状態の数だけ繰りかえし
						m_pMotionManager->SetMotion(pMotion[nCntMotion], nCntMotion);
					}
				}
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
void CObjectManager::Uninit(void)
{
	// モデルの破棄
	if (m_apModel != NULL)
	{// メモリが確保されている
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// 使用できるモデル数分繰り返し
			if (m_apModel[nCntModel] != NULL)
			{// ポインタが確保されている
				m_apModel[nCntModel]->Uninit();

				// メモリの開放
				delete m_apModel[nCntModel];
				m_apModel[nCntModel] = NULL;
			}
		}
		// メモリの開放
		delete[] m_apModel;
		m_apModel = NULL;
	}

	// モーションクラスの破棄
	if (m_pMotionManager != NULL)
	{// メモリが確保されている
		m_pMotionManager->Uninit();

		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}

	// コリジョンクラスの破棄
	if (m_apCollision != NULL)
	{// メモリが確保されている
		for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
		{// 使用できるモデル数分繰り返し
			if (m_apCollision[nCntCol] != NULL)
			{// ポインタが確保されている
				m_apCollision[nCntCol]->Uninit();

				// メモリの開放
				delete m_apCollision[nCntCol];
				m_apCollision[nCntCol] = NULL;
			}
		}
		// メモリの開放
		delete[] m_apCollision;
		m_apCollision = NULL;
	}

	// 親モデルの番号
	if (m_pNumParent != NULL)
	{// メモリが確保されている
		delete[] m_pNumParent;
		m_pNumParent = NULL;
	}

	// 当たり判定の親の番号
	if (m_pNumColParent != NULL)
	{// メモリが確保されている
		delete[] m_pNumColParent;
		m_pNumColParent = NULL;
	}

	// 使用するモデルの番号
	if (m_pNumModelIdx != NULL)
	{// メモリが確保されている
		delete[] m_pNumModelIdx;
		m_pNumModelIdx = NULL;
	}
}

//=============================================================================
//    配置物を生成する処理
//=============================================================================
void CObjectManager::CreateObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, bool bBreak, int nPriority)
{
	CModel **apModelCpy = NULL;                     // モデルクラスコピー用
	CMotionAttack *pMotionAttack = NULL;            // 攻撃モーションデータクラスコピー用
	CMotionCollision **pMotionCollision = NULL;     // モーションの当たり判定データクラスコピー用
	CMotionOrbit **pMotionOrbit = NULL;             // モーションの軌跡データクラスコピー用
	CMotion *pMotionCpy[MAX_OBJECT_MOTION] = {};    // モージョンクラスコピー用
	CMotionManager *pMotionManagerCpy = NULL;       // モーション管轄クラスコピー用
	CCollision **pCollisionCpy = NULL;              // 当たり判定クラスコピー用
	CBoxCollider *pBoxCollider = NULL;              // ボックスコライダークラス
	CCircleCollider *pCircleCollider = NULL;        // サークルコライダークラス
	CSphereCollider *pSphereCollider = NULL;        // スフィアコライダークラス
	CObject *pObject = NULL;                        // 配置物クラスへのポインタ

	if (m_apModel != NULL && m_pMotionManager != NULL)
	{// モデルへのポインタとモーションクラスへのポインタとモデル管轄クラスへのポインタが作成されている
	    // モデル情報をコピーする
		if (apModelCpy == NULL)
		{// メモリが確保されている
			apModelCpy = new CModel*[m_nNumParts];
			if (apModelCpy != NULL)
			{// メモリが確保できた
				for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
				{// パーツ数分繰り返し
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// メモリが確保できる状態である
						apModelCpy[nCntParts] = CModel::Create(m_apModel[nCntParts]->GetPos(), m_apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(m_pNumModelIdx[nCntParts]), m_pModelManager->GetBuffMat(m_pNumModelIdx[nCntParts]), m_pModelManager->GetNumMat(m_pNumModelIdx[nCntParts]), m_pModelManager->GetVtxMax(m_pNumModelIdx[nCntParts]), m_pModelManager->GetVtxMin(m_pNumModelIdx[nCntParts]), m_pModelManager->GetTexture(m_pNumModelIdx[nCntParts]));
						if (apModelCpy[nCntParts] != NULL)
						{// メモリが確保できた
							if (m_pNumParent[nCntParts] != -1)
							{// 親モデルが存在する
								apModelCpy[nCntParts]->SetParent(apModelCpy[m_pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// モーション情報をコピーする
		if (pMotionManagerCpy == NULL && m_pMotionManager != NULL)
		{// メモリが確保できる状態である
			pMotionManagerCpy = CMotionManager::Create(MAX_OBJECT_MOTION, m_nNumParts);
			if (pMotionManagerCpy != NULL)
			{// モーション管理クラスが作成できた
				for (int nCntMotion = 0; nCntMotion < MAX_OBJECT_MOTION; nCntMotion++)
				{// プレイヤーの状態の数だけ繰り返し
					if (pMotionCpy[nCntMotion] == NULL && m_pMotionManager->GetMotion()[nCntMotion] != NULL)
					{// メモリを確保できる状態である
						pMotionCpy[nCntMotion] = CMotion::Create(m_nNumParts, m_pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), m_pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), m_pMotionManager->GetMotion()[nCntMotion]->GetLoop(), m_pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// メモリが確保できた
							for (int nCntKey = 0; nCntKey < m_pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// キーフレームの総数分繰り返し
								for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
								{// キーフレームの総数分繰り返し
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(m_pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(m_pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// 攻撃モーションデータクラスのコピー
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetAttack() != NULL)
							{// 攻撃モーションデータクラスが生成されている
								pMotionAttack = new CMotionAttack;
								if (pMotionAttack != NULL)
								{// メモリが確保できた
								    // 各種値の設定
									pMotionAttack->SetNextMotionIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetNextMotionIdx());
									pMotionAttack->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetStartTiming());
									pMotionAttack->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetFinishTiming());
									pMotionCpy[nCntMotion]->SetAttack(pMotionAttack);
								}
							}

							// 当たり判定データクラスのコピー
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData() >= 1)
							{// 当たり判定データが１個以上ある
								pMotionCollision = new CMotionCollision*[m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData()];
								if (pMotionCollision != NULL)
								{// メモリが確保できた
									for (int nCntCol = 0; nCntCol < m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData(); nCntCol++)
									{// 当たり判定データの数だけ繰り返し
										pMotionCollision[nCntCol] = new CMotionCollision;
										if (pMotionCollision[nCntCol] != NULL)
										{// メモリが確保できた
											pMotionCollision[nCntCol]->SetModelIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetModelIdx());
											pMotionCollision[nCntCol]->SetOffsetPos(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetOffsetPos());
											pMotionCollision[nCntCol]->SetRange(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetRange());
											pMotionCollision[nCntCol]->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetStartTiming());
											pMotionCollision[nCntCol]->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetFinishTiming());
											pMotionCollision[nCntCol]->SetDamage(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetDamage());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetCollision(pMotionCollision);
								pMotionCpy[nCntMotion]->SetNumColData(m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData());
							}

							// 軌跡データクラスのコピー
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData() >= 1)
							{// 当たり判定データが１個以上ある
								pMotionOrbit = new CMotionOrbit*[m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData()];
								if (pMotionOrbit != NULL)
								{// メモリが確保できた
									for (int nCntOrbit = 0; nCntOrbit < m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData(); nCntOrbit++)
									{// 当たり判定データの数だけ繰り返し
										pMotionOrbit[nCntOrbit] = new CMotionOrbit;
										if (pMotionOrbit[nCntOrbit] != NULL)
										{// メモリが確保できた
											pMotionOrbit[nCntOrbit]->SetModelIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetModelIdx());
											pMotionOrbit[nCntOrbit]->SetTexIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetTexIdx());
											pMotionOrbit[nCntOrbit]->SetOffsetPos1(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos1());
											pMotionOrbit[nCntOrbit]->SetOffsetPos2(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos2());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp1(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp1());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp2(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp2());
											pMotionOrbit[nCntOrbit]->SetColUp(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColUp());
											pMotionOrbit[nCntOrbit]->SetColDown(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColDown());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayUp());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayDown());
											pMotionOrbit[nCntOrbit]->SetXBlock(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetXBlock());
											pMotionOrbit[nCntOrbit]->SetYBlock(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetYBlock());
											pMotionOrbit[nCntOrbit]->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetStartTiming());
											pMotionOrbit[nCntOrbit]->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetFinishTiming());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetOrbit(pMotionOrbit);
								pMotionCpy[nCntMotion]->SetNumOrbitData(m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());
							}

							// モーション管轄クラスにポインタを渡す
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// 当たり判定情報をコピーする
		CBoxCollider *pBoxColliderCpy = NULL;           // ボックスコライダークラスコピー用
		CCircleCollider *pCircleColliderCpy = NULL;     // サークルコライダークラスコピー用
		CSphereCollider *pSphereColliderCpy = NULL;     // スフィアコライダークラスコピー用
		if (pCollisionCpy == NULL && m_apCollision != NULL)
		{// メモリが確保できる状態である
			pCollisionCpy = new CCollision*[m_nNumCollision];
			for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
			{// 当たり判定の数だけ繰り返し
				if (m_apCollision[nCntCol] != NULL)
				{// メモリが確保できている
					if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_BOX)
					{// ボックスコライダーだったら
						pBoxCollider = (CBoxCollider*)m_apCollision[nCntCol];
						pBoxColliderCpy = CBoxCollider::Create(pBoxCollider->GetPos(), pBoxCollider->GetWidth(), pBoxCollider->GetHeight(), pBoxCollider->GetDepth(), pBoxCollider->GetReturnFlag(), pBoxCollider->GetType());
						if (pBoxColliderCpy != NULL)
						{// メモリが確保できた
							if (m_pNumColParent[nCntCol] != -1)
							{// 親モデルが存在する
								pBoxColliderCpy->SetParent(pCollisionCpy[m_pNumParent[nCntCol]]);
							}
							pCollisionCpy[nCntCol] = pBoxColliderCpy;
						}
					}
				}
			}
		}

		// 配置物クラスを作成する
		if (pObject == NULL)
		{// メモリが確保できる状態である
			pObject = CObject::Create(pos, rot, scale, RollSpeed, nType, m_nNumParts, m_nNumCollision, apModelCpy, pMotionManagerCpy, pCollisionCpy, bBreak, nPriority);
		}
	}
}

//=============================================================================
//    スター配置物を生成する処理
//=============================================================================
void CObjectManager::CreateStarObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, bool bBreak, int nPriority)
{
	CModel **apModelCpy = NULL;                     // モデルクラスコピー用
	CMotionAttack *pMotionAttack = NULL;            // 攻撃モーションデータクラスコピー用
	CMotionCollision **pMotionCollision = NULL;     // モーションの当たり判定データクラスコピー用
	CMotionOrbit **pMotionOrbit = NULL;             // モーションの軌跡データクラスコピー用
	CMotion *pMotionCpy[MAX_OBJECT_MOTION] = {};    // モージョンクラスコピー用
	CMotionManager *pMotionManagerCpy = NULL;       // モーション管轄クラスコピー用
	CCollision **pCollisionCpy = NULL;              // 当たり判定クラスコピー用
	CBoxCollider *pBoxCollider = NULL;              // ボックスコライダークラス
	CCircleCollider *pCircleCollider = NULL;        // サークルコライダークラス
	CSphereCollider *pSphereCollider = NULL;        // スフィアコライダークラス
	CObjectStar *pObject = NULL;                        // 配置物クラスへのポインタ

	if (m_apModel != NULL && m_pMotionManager != NULL)
	{// モデルへのポインタとモーションクラスへのポインタとモデル管轄クラスへのポインタが作成されている
	 // モデル情報をコピーする
		if (apModelCpy == NULL)
		{// メモリが確保されている
			apModelCpy = new CModel*[m_nNumParts];
			if (apModelCpy != NULL)
			{// メモリが確保できた
				for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
				{// パーツ数分繰り返し
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// メモリが確保できる状態である
						apModelCpy[nCntParts] = CModel::Create(m_apModel[nCntParts]->GetPos(), m_apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(m_pNumModelIdx[nCntParts]), m_pModelManager->GetBuffMat(m_pNumModelIdx[nCntParts]), m_pModelManager->GetNumMat(m_pNumModelIdx[nCntParts]), m_pModelManager->GetVtxMax(m_pNumModelIdx[nCntParts]), m_pModelManager->GetVtxMin(m_pNumModelIdx[nCntParts]), m_pModelManager->GetTexture(m_pNumModelIdx[nCntParts]));
						if (apModelCpy[nCntParts] != NULL)
						{// メモリが確保できた
							if (m_pNumParent[nCntParts] != -1)
							{// 親モデルが存在する
								apModelCpy[nCntParts]->SetParent(apModelCpy[m_pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// モーション情報をコピーする
		if (pMotionManagerCpy == NULL && m_pMotionManager != NULL)
		{// メモリが確保できる状態である
			pMotionManagerCpy = CMotionManager::Create(MAX_OBJECT_MOTION, m_nNumParts);
			if (pMotionManagerCpy != NULL)
			{// モーション管理クラスが作成できた
				for (int nCntMotion = 0; nCntMotion < MAX_OBJECT_MOTION; nCntMotion++)
				{// プレイヤーの状態の数だけ繰り返し
					if (pMotionCpy[nCntMotion] == NULL && m_pMotionManager->GetMotion()[nCntMotion] != NULL)
					{// メモリを確保できる状態である
						pMotionCpy[nCntMotion] = CMotion::Create(m_nNumParts, m_pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), m_pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), m_pMotionManager->GetMotion()[nCntMotion]->GetLoop(), m_pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// メモリが確保できた
							for (int nCntKey = 0; nCntKey < m_pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// キーフレームの総数分繰り返し
								for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
								{// キーフレームの総数分繰り返し
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(m_pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(m_pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// 攻撃モーションデータクラスのコピー
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetAttack() != NULL)
							{// 攻撃モーションデータクラスが生成されている
								pMotionAttack = new CMotionAttack;
								if (pMotionAttack != NULL)
								{// メモリが確保できた
								 // 各種値の設定
									pMotionAttack->SetNextMotionIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetNextMotionIdx());
									pMotionAttack->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetStartTiming());
									pMotionAttack->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetFinishTiming());
									pMotionCpy[nCntMotion]->SetAttack(pMotionAttack);
								}
							}

							// 当たり判定データクラスのコピー
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData() >= 1)
							{// 当たり判定データが１個以上ある
								pMotionCollision = new CMotionCollision*[m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData()];
								if (pMotionCollision != NULL)
								{// メモリが確保できた
									for (int nCntCol = 0; nCntCol < m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData(); nCntCol++)
									{// 当たり判定データの数だけ繰り返し
										pMotionCollision[nCntCol] = new CMotionCollision;
										if (pMotionCollision[nCntCol] != NULL)
										{// メモリが確保できた
											pMotionCollision[nCntCol]->SetModelIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetModelIdx());
											pMotionCollision[nCntCol]->SetOffsetPos(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetOffsetPos());
											pMotionCollision[nCntCol]->SetRange(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetRange());
											pMotionCollision[nCntCol]->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetStartTiming());
											pMotionCollision[nCntCol]->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetFinishTiming());
											pMotionCollision[nCntCol]->SetDamage(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetDamage());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetCollision(pMotionCollision);
								pMotionCpy[nCntMotion]->SetNumColData(m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData());
							}

							// 軌跡データクラスのコピー
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData() >= 1)
							{// 当たり判定データが１個以上ある
								pMotionOrbit = new CMotionOrbit*[m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData()];
								if (pMotionOrbit != NULL)
								{// メモリが確保できた
									for (int nCntOrbit = 0; nCntOrbit < m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData(); nCntOrbit++)
									{// 当たり判定データの数だけ繰り返し
										pMotionOrbit[nCntOrbit] = new CMotionOrbit;
										if (pMotionOrbit[nCntOrbit] != NULL)
										{// メモリが確保できた
											pMotionOrbit[nCntOrbit]->SetModelIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetModelIdx());
											pMotionOrbit[nCntOrbit]->SetTexIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetTexIdx());
											pMotionOrbit[nCntOrbit]->SetOffsetPos1(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos1());
											pMotionOrbit[nCntOrbit]->SetOffsetPos2(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos2());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp1(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp1());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp2(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp2());
											pMotionOrbit[nCntOrbit]->SetColUp(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColUp());
											pMotionOrbit[nCntOrbit]->SetColDown(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColDown());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayUp());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayDown());
											pMotionOrbit[nCntOrbit]->SetXBlock(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetXBlock());
											pMotionOrbit[nCntOrbit]->SetYBlock(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetYBlock());
											pMotionOrbit[nCntOrbit]->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetStartTiming());
											pMotionOrbit[nCntOrbit]->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetFinishTiming());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetOrbit(pMotionOrbit);
								pMotionCpy[nCntMotion]->SetNumOrbitData(m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());
							}

							// モーション管轄クラスにポインタを渡す
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// 当たり判定情報をコピーする
		CBoxCollider *pBoxColliderCpy = NULL;           // ボックスコライダークラスコピー用
		CCircleCollider *pCircleColliderCpy = NULL;     // サークルコライダークラスコピー用
		CSphereCollider *pSphereColliderCpy = NULL;     // スフィアコライダークラスコピー用
		if (pCollisionCpy == NULL && m_apCollision != NULL)
		{// メモリが確保できる状態である
			pCollisionCpy = new CCollision*[m_nNumCollision];
			for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
			{// 当たり判定の数だけ繰り返し
				if (m_apCollision[nCntCol] != NULL)
				{// メモリが確保できている
					if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_BOX)
					{// ボックスコライダーだったら
						pBoxCollider = (CBoxCollider*)m_apCollision[nCntCol];
						pBoxColliderCpy = CBoxCollider::Create(pBoxCollider->GetPos(), pBoxCollider->GetWidth(), pBoxCollider->GetHeight(), pBoxCollider->GetDepth(), pBoxCollider->GetReturnFlag(), pBoxCollider->GetType());
						if (pBoxColliderCpy != NULL)
						{// メモリが確保できた
							if (m_pNumColParent[nCntCol] != -1)
							{// 親モデルが存在する
								pBoxColliderCpy->SetParent(pCollisionCpy[m_pNumParent[nCntCol]]);
							}
							pCollisionCpy[nCntCol] = pBoxColliderCpy;
						}
					}
				}
			}
		}

		// 配置物クラスを作成する
		if (pObject == NULL)
		{// メモリが確保できる状態である
			pObject = CObjectStar::Create(pos, rot, scale, RollSpeed, nType, m_nNumParts, m_nNumCollision, apModelCpy, pMotionManagerCpy, pCollisionCpy, bBreak, nPriority);
		}
	}
}

//=============================================================================
//    配置物情報を読み取る処理
//=============================================================================
CModel **CObjectManager::ReadModelSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts)
{
	int nCntParts = 0;
	CModel **apModel = NULL;
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, NUM_PARTS) == 0)
		{// パーツ数情報がある
			*pNumParts = (CFunctionLib::ReadInt(pStrCur, NUM_PARTS));
			m_pNumParent = new int[*pNumParts];
			m_pNumModelIdx = new int[*pNumParts];
			if (apModel == NULL)
			{// メモリが確保できる状態である
				apModel = new CModel*[*pNumParts];
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARTSSET) == 0)
		{// パーツ情報読み込み開始の合図だった
			if (nCntParts < *pNumParts)
			{// まだパーツ情報を読み込める
				apModel[nCntParts] = ReadPartsSet(pLine, pStrCur, pFile, pNumParts, m_pNumParent, apModel);
				nCntParts++;
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_MODELSET) == 0)
		{// 配置物情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return apModel;
}

//=============================================================================
//    パーツ情報を読み取る処理
//=============================================================================
CModel *CObjectManager::ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel)
{
	int nIdx = 0;                                      // パーツ番号情報
	int nParent = 0;                                   // 親モデルのパーツ番号
	int nModelIdx = 0;
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
			pNumParent[nIdx] = nParent = CFunctionLib::ReadInt(pStrCur, PARENT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, MODEL_IDX) == 0)
		{// モデルの番号情報がある
			m_pNumModelIdx[nIdx] = nModelIdx = CFunctionLib::ReadInt(pStrCur, MODEL_IDX);
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
		pModel = CModel::Create(Pos, Rot, m_pModelManager->GetMesh(m_pNumModelIdx[nIdx]), m_pModelManager->GetBuffMat(m_pNumModelIdx[nIdx]), m_pModelManager->GetNumMat(m_pNumModelIdx[nIdx]), m_pModelManager->GetVtxMax(nIdx), m_pModelManager->GetVtxMin(m_pNumModelIdx[nIdx]), m_pModelManager->GetTexture(m_pNumModelIdx[nIdx]));
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
CMotion *CObjectManager::ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion)
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
CMotionAttack *CObjectManager::ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile)
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
CMotionCollision **CObjectManager::ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData)
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
CMotionOrbit **CObjectManager::ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData)
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
void CObjectManager::ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
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
void CObjectManager::ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
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
//    当たり判定情報を読み取る処理
//=============================================================================
CCollision **CObjectManager::ReadCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumCollision)
{
	int nCntCol = 0;
	CCollision **apCollision = NULL;
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, NUM_COLLISION) == 0)
		{// 当たり判定の数情報がある
			*pNumCollision = (CFunctionLib::ReadInt(pStrCur, NUM_COLLISION));
			m_pNumColParent = new int[*pNumCollision];
			if (apCollision == NULL)
			{// メモリが確保できる状態である
				apCollision = new CCollision*[*pNumCollision];
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
		{// 当たり判定データ情報読み込み開始の合図だった
			if (nCntCol < *pNumCollision)
			{// まだ当たり判定データ情報を読み込める
				apCollision[nCntCol] = ReadCollision(pLine, pStrCur, pFile, apCollision);
				nCntCol++;
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_COLLISIONSET) == 0)
		{// 配置物情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	return apCollision;
}

//=============================================================================
//    当たり判定データを読み取る処理
//=============================================================================
CCollision *CObjectManager::ReadCollision(char *pLine, char *pStrCur, FILE *pFile, CCollision **apCollision)
{
	int nIdx = 0;
	int nType = 0;                                     // 種類
	int nParent = 0;                                   // 親モデルのパーツ番号
	CCollision *pCollision = NULL;                     // 当たり判定クラスへのポインタ
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 当たり判定の座標情報
	bool bReturnFlag = false;                          // 当たった際に押し戻すかどうか
	float fWidth = 0.0f;                               // 横幅
	float fHeight = 0.0f;                              // 縦幅
	float fDepth = 0.0f;                               // 奥行
	while (1)
	{// ループ開始
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
		if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
		{// 番号情報がある
			nIdx = CFunctionLib::ReadInt(pStrCur, INDEX);
		}
		else if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
		{// 種類情報がある
			nType = CFunctionLib::ReadInt(pStrCur, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARENT) == 0)
		{// 親の番号情報がある
			m_pNumColParent[nIdx] = nParent = CFunctionLib::ReadInt(pStrCur, PARENT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// 当たり判定の座標情報がある
			Pos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
		{// 横幅情報がある
			fWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
		{// 縦幅情報がある
			fHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, DEPTH) == 0)
		{// 奥行情報がある
			fDepth = CFunctionLib::ReadFloat(pStrCur, DEPTH);
		}
		else if (CFunctionLib::Memcmp(pStrCur, RETURN) == 0)
		{// 当たった際に押し戻すかどうか情報がある
			bReturnFlag = CFunctionLib::ReadBool(pStrCur, RETURN);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_COLLISION) == 0)
		{// 当たり判定データ情報読み込み終了の合図だった
			break;  // ループ終了
		}
	}

	if (pCollision == NULL)
	{// メモリを確保できる状態である
		if (nType == CCollision::TYPE_BOX)
		{// 箱モデルだったら
			pCollision = CBoxCollider::Create(Pos, fWidth, fHeight, fDepth, bReturnFlag, nType);
		}
		if (pCollision != NULL)
		{// モデルポインタが確保された
			if (nParent != -1)
			{// 親モデルが存在する
				pCollision->SetParent(apCollision[nParent]);
			}
		}
	}

	return pCollision;
}

//=============================================================================
//    ファイル名を取得する
//=============================================================================
void CObjectManager::SetFileName(const char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    パーツ数を取得する
//=============================================================================
void CObjectManager::SetNumParts(const int nNumParts)
{
	m_nNumParts = nNumParts;
}

//=============================================================================
//    当たり判定の数を設定する
//=============================================================================
void CObjectManager::SetNumCollision(const int nNumCollision)
{
	m_nNumCollision = nNumCollision;
}

//=============================================================================
//    モデルクラスへのポインタを設定する
//=============================================================================
void CObjectManager::SetModel(CModel **pModel)
{
	m_apModel = pModel;
}

//=============================================================================
//    モデル管轄クラスへのポインタを設定する
//=============================================================================
void CObjectManager::SetModelManager(CModelManager *pModelManager)
{
	m_pModelManager = pModelManager;
}

//=============================================================================
//    コリジョンクラスへのポインタを設定する
//=============================================================================
void CObjectManager::SetCollision(CCollision **pCollision)
{
	m_apCollision = pCollision;
}

//=============================================================================
//    モーションクラスへのポインタを設定する
//=============================================================================
void CObjectManager::SetMotionManager(CMotionManager *pMotionManager)
{
	m_pMotionManager = pMotionManager;
}

//=============================================================================
//    ファイル名を取得する
//=============================================================================
char *CObjectManager::GetFileName(void)
{
	return &m_aFileName[0];
}

//=============================================================================
//    パーツ数を取得する
//=============================================================================
int CObjectManager::GetNumParts(void)
{
	return m_nNumParts;
}

//=============================================================================
//    当たり判定の数を取得する
//=============================================================================
int CObjectManager::GetNumCollision(void)
{
	return m_nNumCollision;
}

//=============================================================================
//    モデルクラスへのポインタを取得する
//=============================================================================
CModel **CObjectManager::GetModel(void)
{
	return m_apModel;
}

//=============================================================================
//    モデル管轄クラスへのポインタを取得する
//=============================================================================
CModelManager *CObjectManager::GetModelManager(void)
{
	return m_pModelManager;
}

//=============================================================================
//    モーションクラスへのポインタを取得する
//=============================================================================
CMotionManager *CObjectManager::GetMotionManager(void)
{
	return m_pMotionManager;
}

//=============================================================================
//    コリジョンクラスへのポインタを取得する
//=============================================================================
CCollision **CObjectManager::GetCollision(void)
{
	return m_apCollision;
}
