//*****************************************************************************
//
//     当たり判定用球モデルの処理[sphereCollider.cpp]
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
CSphereCollider::CSphereCollider()
{
	// 各種値のクリア
	for (int nCntVer = 0; nCntVer < 8; nCntVer++)
	{// 円筒の頂点の数だけ繰り返し
		m_Vertex[nCntVer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Vertex[nCntVer].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
	for (int nCntIndex = 0; nCntIndex < 24; nCntIndex++)
	{// 描画に使用するインデックス情報の数だけ繰り返し
		m_wIndex[nCntIndex] = 0;
	}
	m_fColRange = 0.0f;                       // 球の半径
	m_bReturnFlag = false;                    // 当たった際に戻すかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSphereCollider::~CSphereCollider()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSphereCollider *CSphereCollider::Create(D3DXVECTOR3 pos, float fColRange, bool bReturnFlag, int nType)
{
	CSphereCollider *pSphereCollider = NULL;      // 当たり判定用球モデルクラス型のポインタ
	if (pSphereCollider == NULL)
	{// メモリが空になっている
		pSphereCollider = new CSphereCollider;
		if (pSphereCollider != NULL)
		{// インスタンスを生成できた
		    // 各種値の設定
			pSphereCollider->SetPos(pos);                 // 座標
			pSphereCollider->SetColRange(fColRange);      // 球の半径
			pSphereCollider->SetReturnFlag(bReturnFlag);  // 当たった際に戻すかどうか
			pSphereCollider->SetType(nType);              // 種類

			if (FAILED(pSphereCollider->Init()))
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

	return pSphereCollider;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSphereCollider::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // 頂点バッファの生成
			MakeVertex();
		}
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSphereCollider::Uninit(void)
{

}

//=============================================================================
//    更新処理
//=============================================================================
void CSphereCollider::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CSphereCollider::Draw(void)
{
#ifdef _DEBUG
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Culling;
			DWORD Lighting;

			// ライティングしない設定に
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// カリングを外しておく
			pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// ワールドマトリックスの設定処理
			SetMtxWorld(pDevice);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// 球の描画
			pDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 8, 12, m_wIndex, D3DFMT_INDEX16, m_Vertex, sizeof(BOX_VERTEX));

			// カリングの設定を戻す
			pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

			// ライティングの設定を戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}

#endif
}

//=============================================================================
//    頂点情報生成処理
//=============================================================================
void CSphereCollider::MakeVertex(void)
{
	//// 頂点座標を設定(順番は 上側の 左奥 -> 右奥 -> -> 左前 -> 右奥
	////                       下側の 左奥 -> 右奥 -> -> 左前 -> 右奥)
	//m_Vertex[0].pos = D3DXVECTOR3(0.0f, m_fHeight, m_fDepth);
	//m_Vertex[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth);
	//m_Vertex[2].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
	//m_Vertex[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	//m_Vertex[4].pos = D3DXVECTOR3(0.0f, 0.0f, m_fDepth);
	//m_Vertex[5].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth);
	//m_Vertex[6].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Vertex[7].pos = D3DXVECTOR3(m_fWidth, 0.0f, 0.0f);

	//// 頂点カラーを設定(全て赤色に設定)
	//m_Vertex[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[4].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[5].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[6].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[7].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	//// インデックス情報を設定
	//m_wIndex[0] = 0;
	//m_wIndex[1] = 1;
	//m_wIndex[2] = 1;
	//m_wIndex[3] = 3;
	//m_wIndex[4] = 3;
	//m_wIndex[5] = 2;
	//m_wIndex[6] = 2;
	//m_wIndex[7] = 0;
	//m_wIndex[8] = 0;
	//m_wIndex[9] = 4;
	//m_wIndex[10] = 1;
	//m_wIndex[11] = 5;
	//m_wIndex[12] = 2;
	//m_wIndex[13] = 6;
	//m_wIndex[14] = 3;
	//m_wIndex[15] = 7;
	//m_wIndex[16] = 4;
	//m_wIndex[17] = 5;
	//m_wIndex[18] = 5;
	//m_wIndex[19] = 7;
	//m_wIndex[20] = 7;
	//m_wIndex[21] = 6;
	//m_wIndex[22] = 6;
	//m_wIndex[23] = 4;
}

//=============================================================================
//    当たり判定処理
//=============================================================================
bool CSphereCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand)
{
	bool bHit = false;  // 当たったかどうか
	D3DXVECTOR3 pos = GetPos();

	return bHit;
}

//=============================================================================
//    球の半径設定処理
//=============================================================================
void CSphereCollider::SetColRange(const float fColRange)
{
	m_fColRange = fColRange;
}

//=============================================================================
//    当たった際に戻すかどうか設定処理
//=============================================================================
void CSphereCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    球の半径取得処理
//=============================================================================
float CSphereCollider::GetColRange(void)
{
	return m_fColRange;
}

//=============================================================================
//    当たった際に戻すかどうか取得処理
//=============================================================================
bool CSphereCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}