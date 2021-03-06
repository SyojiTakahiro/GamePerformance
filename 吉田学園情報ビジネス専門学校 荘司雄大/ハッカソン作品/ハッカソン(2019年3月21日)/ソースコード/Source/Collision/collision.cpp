//*****************************************************************************
//
//     コリジョンの処理[collision.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "collision.h"
#include "manager.h"
#include "system.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************


//*****************************************************************************
//     静的メンバ変数
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CCollision::CCollision()
{
	// 各種値のクリア
	m_nType = TYPE_NONE;                    // 種類
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 向き
	D3DXMatrixIdentity(&m_MtxWorld);        // ワールドマトリックス
	m_pParent = NULL;                       // 親のポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CCollision::~CCollision()
{

}

//=============================================================================
//    描画デバイスにワールドマトリックスを設定する処理
//=============================================================================
void CCollision::SetMtxWorld(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxParent; // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_MtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// 親モデルのワールドマトリックスを掛け合わせる
	if (m_pParent != NULL)
	{// 親モデルが存在する
		mtxParent = m_pParent->GetMtxWorld();            // 親からワールドマトリックスを取得する
	}
	else
	{// 親モデルが存在しない
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);  // デバイスからワールドマトリックスを取得する
	}
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    種類を設定する処理
//=============================================================================
void CCollision::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    座標を設定する処理
//=============================================================================
void CCollision::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    向きを設定する処理
//=============================================================================
void CCollision::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ワールドマトリックスを設定する処理
//=============================================================================
void CCollision::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    親のポインタを設定する処理
//=============================================================================
void CCollision::SetParent(CCollision *pParent)
{
	m_pParent = pParent;
}

//=============================================================================
//    種類を取得する処理
//=============================================================================
int CCollision::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    座標を取得する処理
//=============================================================================
D3DXVECTOR3 CCollision::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    向きを取得する処理
//=============================================================================
D3DXVECTOR3 CCollision::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ワールドマトリックスを取得する処理
//=============================================================================
D3DXMATRIX CCollision::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    親のポインタを取得する処理
//=============================================================================
CCollision *CCollision::GetParent(void)
{
	return m_pParent;
}