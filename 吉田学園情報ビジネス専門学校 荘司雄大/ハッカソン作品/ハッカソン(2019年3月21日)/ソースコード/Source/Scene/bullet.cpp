//*****************************************************************************
//
//     弾の処理[bullet.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "bullet.h"
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
#define BULLET_HIT_LENGTH  (90.0f)                    // 弾の当たり判定をする範囲
#define BULLET_MODEL_FILENAME "data/MODEL/bullet.x"   // 弾のモデル名
#define BULLET_MAX_LIFE    (120)                      // 弾の体力

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
CModelManager *CBullet::m_pModelManager = NULL;

//=============================================================================
//    コンストラクタ
//=============================================================================
CBullet::CBullet(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_MtxWorld);
	m_pModel = NULL;
	m_nAttackPlayerIdx = 0;
	m_nLife = BULLET_MAX_LIFE;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nAttackPlayerIdx)
{
	CBullet *pBullet = NULL;               // 弾クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pBullet == NULL)
		{// メモリが空になっている
			pBullet = new CBullet();
			if (pBullet != NULL)
			{// インスタンスを生成できた
				pBullet->SetPos(pos);
				pBullet->SetRot(rot);
				pBullet->SetAttackPlayerIdx(nAttackPlayerIdx);
				pBullet->SetMove(move);
				if (FAILED(pBullet->Init()))
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

	return pBullet;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    モデルの読み込み処理
//=============================================================================
HRESULT CBullet::Load(void)
{
	LPD3DXMESH pMesh = NULL;                    // メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat = NULL;               // マテリアル情報へのポインタ
	DWORD nNumMat = 0;                          // マテリアル情報の数

	m_pModelManager = CModelManager::Create(1);

	// xファイルの読み込み
	D3DXLoadMeshFromX(BULLET_MODEL_FILENAME,
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
void CBullet::UnLoad(void)
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
HRESULT CBullet::Init(void)
{
	// 体力を設定
	m_nLife = BULLET_MAX_LIFE;

	// モデルを生成する
	if (m_pModel == NULL)
	{
		m_pModel = CModel::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),m_pModelManager->GetMesh(0), m_pModelManager->GetBuffMat(0), m_pModelManager->GetNumMat(0), m_pModelManager->GetVtxMax(0), m_pModelManager->GetVtxMin(0), m_pModelManager->GetTexture(0));
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBullet::Uninit(void)
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
void CBullet::Update(void)
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

		// 弾を移動させる
		m_Pos += m_Move;

		// 当たり判定を発生させる
		HitDecision();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CBullet::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 弾のワールドマトリックス計算処理
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
void CBullet::SetMtxWorld(LPDIRECT3DDEVICE9 pDevice)
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
//    弾がプレイヤーに当たったかどうか設定する処理
//=============================================================================
void CBullet::HitDecision(void)
{
	bool bDeath = false;
	CScene *pScene = NULL;               // シーンクラスへのポインタ
	CScene *pSceneNext = NULL;           // 次のシーンクラスへのポインタ
	CObject *pObject = NULL;             // 配置物クラスへのポインタ
	CObjectStar *pObjectStar = NULL;
	CPlayer *pPlayer = NULL;
	for (int nCntPriority = FIELD_PRIORITY; nCntPriority < MAP_PRIORITY + 1; nCntPriority++)
	{// 描画優先順位の数だけ繰り返し
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// メモリが空になるまで繰り返し
			pSceneNext = pScene->GetNext();
			if (bDeath != true)
			{
				if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{// 配置物に当たった
					pObject = (CObject*)pScene;
					if (pObject->Collision(&m_Pos, &m_PosOld, &m_Move, D3DXVECTOR3(BULLET_HIT_LENGTH, BULLET_HIT_LENGTH, BULLET_HIT_LENGTH), NULL, this) == true)
					{
						pObject->Damage(1);
						bDeath = true;
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_OBJECTSTAR)
				{// 配置物に当たった
					pObjectStar = (CObjectStar*)pScene;
					if (pObjectStar->Collision(&m_Pos, &m_PosOld, &m_Move, D3DXVECTOR3(BULLET_HIT_LENGTH, BULLET_HIT_LENGTH, BULLET_HIT_LENGTH), NULL, this) == true)
					{
						pObjectStar->Death();
						pObjectStar->Uninit();
						bDeath = true;
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_PLAYER)
				{// プレイヤーに当たった
					pPlayer = (CPlayer*)pScene;
					if (pPlayer->GetControllerIdx() != m_nAttackPlayerIdx)
					{// 自身を打ったプレイヤーではない
						bDeath = HitPlayer(pPlayer);
					}
				}
			}
			pScene = pSceneNext;
		}
	}

	if (bDeath == true)
	{// フラグがたっている
		//CManager::GetSound()->PlaySound(5);
		Uninit();
	}
}

//=============================================================================
//    プレイヤーと当たったかどうか判定する処理
//=============================================================================
bool CBullet::HitPlayer(CPlayer *pPlayer)
{
	bool bHit = false;
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	float fColRange = pPlayer->GetColRange();
	float fLength = 0.0f;
	float fHitRange = 0.0f;

	// 当たり判定を取る範囲を計算
	fHitRange = sqrtf((fColRange + BULLET_HIT_LENGTH) * (fColRange + BULLET_HIT_LENGTH));

	// 当たり判定を取る座標と配置物の座標との距離を測る
	fLength = sqrtf((PlayerPos.x - m_Pos.x) * (PlayerPos.x - m_Pos.x) + (PlayerPos.y - m_Pos.y) * (PlayerPos.y - m_Pos.y) + (PlayerPos.z - m_Pos.z) * (PlayerPos.z - m_Pos.z));

	if (fLength <= fHitRange)
	{// 距離が範囲内である
		bHit = true;
		pPlayer->Damage(m_Move);
	}

	return bHit;
}

//=============================================================================
//    座標を設定する処理
//=============================================================================
void CBullet::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    向きを設定する処理
//=============================================================================
void CBullet::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    移動量を設定する処理
//=============================================================================
void CBullet::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    ワールドマトリックスを設定する処理
//=============================================================================
void CBullet::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    攻撃するプレイヤーの番号を設定する処理
//=============================================================================
void CBullet::SetAttackPlayerIdx(const int nAttackPlayerIdx)
{
	m_nAttackPlayerIdx = nAttackPlayerIdx;
}

//=============================================================================
//    座標を設定する処理
//=============================================================================
D3DXVECTOR3 CBullet::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    向きを設定する処理
//=============================================================================
D3DXVECTOR3 CBullet::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    移動量を設定する処理
//=============================================================================
D3DXVECTOR3 CBullet::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    ワールドマトリックスを設定する処理
//=============================================================================
D3DXMATRIX CBullet::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    攻撃するプレイヤーの番号を取得する処理
//=============================================================================
int CBullet::GetAttackPlayerIdx(void)
{
	return m_nAttackPlayerIdx;
}