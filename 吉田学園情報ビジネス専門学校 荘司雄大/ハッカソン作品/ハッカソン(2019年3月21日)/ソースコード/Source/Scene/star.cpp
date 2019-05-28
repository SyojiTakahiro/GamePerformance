//*****************************************************************************
//
//     星の処理[star.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "star.h"
#include "manager.h"
#include "system.h"
#include "model.h"
#include "modelManager.h"
#include "object.h"
#include "character.h"
#include "map.h"

//*****************************************************************************
//    マクロ宣言
//*****************************************************************************
#define STAR_HIT_LENGTH  (90.0f)                    // 星の当たり判定をする範囲
#define STAR_MODEL_FILENAME "data/MODEL/star.x"     // 星のモデル名
#define STAR_MAX_LIFE    (240)                      // 星の体力

#define STAR_GRAVITY     (-0.4f)                    // 星の重力
#define STAR_INERTIA     (0.04f)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
CModelManager *CStar::m_pModelManager = NULL;

//=============================================================================
//    コンストラクタ
//=============================================================================
CStar::CStar(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_MtxWorld);
	m_pModel = NULL;
	m_nAttackPlayerIdx = 0;
	m_bGetOk = false;
	m_nLife = STAR_MAX_LIFE;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CStar::~CStar()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CStar *CStar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Move)
{
	CStar *pStar = NULL;               // 弾クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pStar == NULL)
		{// メモリが空になっている
			pStar = new CStar();
			if (pStar != NULL)
			{// インスタンスを生成できた
				pStar->SetPos(pos);
				pStar->SetRot(rot);
				pStar->SetMove(Move);
				if (FAILED(pStar->Init()))
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

	return pStar;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    モデルの読み込み処理
//=============================================================================
HRESULT CStar::Load(void)
{
	LPD3DXMESH pMesh = NULL;                    // メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat = NULL;               // マテリアル情報へのポインタ
	DWORD nNumMat = 0;                          // マテリアル情報の数

	m_pModelManager = CModelManager::Create(1);

	// xファイルの読み込み
	D3DXLoadMeshFromX(STAR_MODEL_FILENAME,
		D3DXMESH_SYSTEMMEM,
		CManager::GetRenderer()->GetDevice(),
		NULL,
		&pBuffMat,
		NULL,
		&nNumMat,
		&pMesh);

	// モデル管轄クラスにポインタを設定する
	m_pModelManager->SetMesh(pMesh, 0);
	m_pModelManager->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, 0);

	return S_OK;
}

//=============================================================================
//    モデルの開放処理
//=============================================================================
void CStar::UnLoad(void)
{
	if (m_pModelManager != NULL)
	{
		m_pModelManager->Uninit();

		delete m_pModelManager;
		m_pModelManager = NULL;
	}
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CStar::Init(void)
{
	// モデルを生成する
	if (m_pModel == NULL)
	{
		m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pModelManager->GetMesh(0), m_pModelManager->GetBuffMat(0), m_pModelManager->GetNumMat(0), m_pModelManager->GetVtxMax(0), m_pModelManager->GetVtxMin(0), m_pModelManager->GetTexture(0));
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CStar::Uninit(void)
{
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();

		delete m_pModel;
		m_pModel = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CStar::Update(void)
{
	m_nLife--;
	if (m_nLife <= 0)
	{
		Uninit();
	}
	else
	{
		// 前回の座標を保存
		m_PosOld = m_Pos;

		// 移動させる
		m_Pos += m_Move;

		// 慣性を付ける
		m_Move.y += STAR_GRAVITY;

		// 移動量に慣性を加える
		m_Move.x += (0.0f - m_Move.x) * STAR_INERTIA;
		m_Move.z += (0.0f - m_Move.z) * STAR_INERTIA;

		if (m_Pos.y <= 0.0f)
		{
			m_Pos.y = 0.0f;
			m_bGetOk = true;
		}
		if (m_Pos.x >= 2000.0f)
		{
			m_Pos.x = 2000.0f;
		}
		else if (m_Pos.x <= -2000.0f)
		{
			m_Pos.x = -2000.0f;
		}
		if (m_Pos.z >= 2000.0f)
		{
			m_Pos.z = 2000.0f;
		}
		else if (m_Pos.z <= -2000.0f)
		{
			m_Pos.z = -2000.0f;
		}

		// 当たり判定を発生させる
		HitDecision();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CStar::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 星のワールドマトリックス計算処理
			SetMtxWorld(pDevice);

			if (m_pModel != NULL)
			{// メモリが確保できた
			    // モデルの描画処理
				m_pModel->Draw();
			}
		}
	}
}

//=============================================================================
//    描画デバイスにワールドマトリックスを設定する処理
//=============================================================================
void CStar::SetMtxWorld(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

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
//    星がプレイヤーに当たったかどうか設定する処理
//=============================================================================
void CStar::HitDecision(void)
{
	bool bDeath = false;
	CScene *pScene = NULL;               // シーンクラスへのポインタ
	CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
	CObject *pObject = NULL;             // 配置物クラスへのポインタ
	CPlayer *pPlayer = NULL;
	for (int nCntPriority = FIELD_PRIORITY; nCntPriority < MAP_PRIORITY + 1; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (bDeath != true)
			{
				if (pScene->GetObjType() == OBJTYPE_PLAYER)
				{// プレイヤーに当たった
					pPlayer = (CPlayer*)pScene;
					bDeath = HitPlayer(pPlayer);
				}
			}
			pScene = pSceneNext;
		}
	}

	if (bDeath == true)
	{// フラグがたっている
		Uninit();
	}
}

//=============================================================================
//    プレイヤーと当たったかどうか判定する処理
//=============================================================================
bool CStar::HitPlayer(CPlayer *pPlayer)
{
	bool bHit = false;
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	float fColRange = pPlayer->GetColRange();
	float fLength = 0.0f;
	float fHitRange = 0.0f;

	// 当たり判定を取る範囲を計算
	fHitRange = sqrtf((fColRange + STAR_HIT_LENGTH) * (fColRange + STAR_HIT_LENGTH));

	// 当たり判定を取る座標と配置物の座標との距離を測る
	fLength = sqrtf((PlayerPos.x - m_Pos.x) * (PlayerPos.x - m_Pos.x) + (PlayerPos.y - m_Pos.y) * (PlayerPos.y - m_Pos.y) + (PlayerPos.z - m_Pos.z) * (PlayerPos.z - m_Pos.z));

	if (fLength <= fHitRange)
	{// 距離が範囲内である
		if (m_bGetOk == true)
		{// 取得できるなら
			bHit = true;
			pPlayer->AddScore();
		}
	}

	return bHit;
}

//=============================================================================
//    座標を設定する処理
//=============================================================================
void CStar::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    向きを設定する処理
//=============================================================================
void CStar::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    移動量を設定する処理
//=============================================================================
void CStar::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    ワールドマトリックスを設定する処理
//=============================================================================
void CStar::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    座標を設定する処理
//=============================================================================
D3DXVECTOR3 CStar::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    向きを設定する処理
//=============================================================================
D3DXVECTOR3 CStar::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    移動量を設定する処理
//=============================================================================
D3DXVECTOR3 CStar::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    ワールドマトリックスを設定する処理
//=============================================================================
D3DXMATRIX CStar::GetMtxWorld(void)
{
	return m_MtxWorld;
}