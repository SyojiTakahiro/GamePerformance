//*****************************************************************************
//
//     当たり判定用箱モデルの処理[boxCollider.cpp]
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
CBoxCollider::CBoxCollider()
{
	// 各種値のクリア
	for (int nCntVer = 0; nCntVer < 8; nCntVer++)
	{// 箱の頂点の数だけ繰り返し
		m_Vertex[nCntVer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Vertex[nCntVer].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
	for (int nCntIndex = 0; nCntIndex < 24; nCntIndex++)
	{// 描画に使用するインデックス情報の数だけ繰り返し
		m_wIndex[nCntIndex] = 0;
	}
	m_fWidth = 0.0f;                          // 箱の横幅
	m_fHeight = 0.0f;                         // 箱の高さ
	m_fDepth = 0.0f;                          // 箱の奥行
	m_bReturnFlag = false;                    // 当たった際に戻すかどうか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CBoxCollider::~CBoxCollider()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBoxCollider *CBoxCollider::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth, bool bReturnFlag, int nType)
{
	CBoxCollider *pBoxCollider = NULL;      // 当たり判定用箱モデルクラス型のポインタ
	if (pBoxCollider == NULL)
	{// メモリが空になっている
		pBoxCollider = new CBoxCollider;
		if (pBoxCollider != NULL)
		{// インスタンスを生成できた
		    // 各種値の設定
			pBoxCollider->SetPos(pos);                 // 座標
			pBoxCollider->SetWidth(fWidth);            // 箱の横幅
			pBoxCollider->SetHeight(fHeight);          // 箱の高さ
			pBoxCollider->SetDepth(fDepth);            // 箱の奥行
			pBoxCollider->SetReturnFlag(bReturnFlag);  // 当たった際に戻すかどうか
			pBoxCollider->SetType(nType);              // 種類

			if (FAILED(pBoxCollider->Init()))
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

	return pBoxCollider;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBoxCollider::Init(void)
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
void CBoxCollider::Uninit(void)
{

}

//=============================================================================
//    更新処理
//=============================================================================
void CBoxCollider::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CBoxCollider::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
		    // ワールドマトリックスの設定処理
			SetMtxWorld(pDevice);

#ifdef _DEBUG
			DWORD Culling;
			DWORD Lighting;

			// ライティングしない設定に
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// カリングを外しておく
			pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// 箱の描画
			pDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 8, 12, m_wIndex, D3DFMT_INDEX16, m_Vertex, sizeof(BOX_VERTEX));

			// カリングの設定を戻す
			pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

			// ライティングの設定を戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
#endif
		}
	}
}

//=============================================================================
//    頂点情報生成処理
//=============================================================================
void CBoxCollider::MakeVertex(void)
{
	// 頂点座標を設定(順番は 上側の 左奥 -> 右奥 -> -> 左前 -> 右奥
	//                       下側の 左奥 -> 右奥 -> -> 左前 -> 右奥)
	m_Vertex[0].pos = D3DXVECTOR3(0.0f, m_fHeight, m_fDepth);
	m_Vertex[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth);
	m_Vertex[2].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
	m_Vertex[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	m_Vertex[4].pos = D3DXVECTOR3(0.0f, 0.0f, m_fDepth);
	m_Vertex[5].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth);
	m_Vertex[6].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Vertex[7].pos = D3DXVECTOR3(m_fWidth, 0.0f, 0.0f);

	// 頂点カラーを設定(全て赤色に設定)
	m_Vertex[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[4].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[5].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[6].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[7].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// インデックス情報を設定
	m_wIndex[0] = 0;
	m_wIndex[1] = 1;
	m_wIndex[2] = 1;
	m_wIndex[3] = 3;
	m_wIndex[4] = 3;
	m_wIndex[5] = 2;
	m_wIndex[6] = 2;
	m_wIndex[7] = 0;
	m_wIndex[8] = 0;
	m_wIndex[9] = 4;
	m_wIndex[10] = 1;
	m_wIndex[11] = 5;
	m_wIndex[12] = 2;
	m_wIndex[13] = 6;
	m_wIndex[14] = 3;
	m_wIndex[15] = 7;
	m_wIndex[16] = 4;
	m_wIndex[17] = 5;
	m_wIndex[18] = 5;
	m_wIndex[19] = 7;
	m_wIndex[20] = 7;
	m_wIndex[21] = 6;
	m_wIndex[22] = 6;
	m_wIndex[23] = 4;
}

//=============================================================================
//    当たり判定処理
//=============================================================================
bool CBoxCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand)
{
	bool bHit = false;  // 当たったかどうか
	D3DXVECTOR3 pos = D3DXVECTOR3(GetMtxWorld()._41, GetMtxWorld()._42, GetMtxWorld()._43);

	// Y軸上の判定
	if (pPos->y < pos.y + m_fHeight && pPos->y + ColRange.y > pos.y)
	{// 現在の高さが箱の中にいる
	    // X軸上の判定
		if (pPos->z + ColRange.z > pos.z && pPos->z - ColRange.z < pos.z + m_fDepth)
		{// Z軸上にいるかどうか判定
			if (pPosOld->x + ColRange.x <= pos.x && pPos->x + ColRange.x > pos.x)
			{// 箱の左側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->x = pos.x - ColRange.x;
				}
				bHit = true;
			}
			else if (pPosOld->x - ColRange.x >= pos.x + m_fWidth && pPos->x - ColRange.x < pos.x + m_fWidth)
			{// 箱の右側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->x = pos.x + m_fWidth + ColRange.x;
				}
				bHit = true;
			}
		}
		// Z軸上の判定
		if (pPos->x + ColRange.x > pos.x && pPos->x - ColRange.x < pos.x + m_fWidth)
		{// X軸上にいるかどうか判定
			if (pPosOld->z - ColRange.z >= pos.z + m_fDepth && pPos->z - ColRange.z < pos.z + m_fDepth)
			{// 箱の奥側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->z = pos.z + m_fDepth + ColRange.z;
				}
				bHit = true;
			}
			else if (pPosOld->z + ColRange.z <= pos.z && pPos->z + ColRange.z > pos.z)
			{// 箱の手前側から入った
				if (m_bReturnFlag == true)
				{// 位置を戻すならば
					pPos->z = pos.z - ColRange.z;
				}
				bHit = true;
			}
		}
	}

	if (pPosOld->y >= pos.y + m_fHeight && pPos->y < pos.y + m_fHeight)
	{// 高さが上から箱の内側に入ってしまった
		if (pPos->x + ColRange.x > pos.x && pPos->x - ColRange.x < pos.x + m_fWidth
			&& pPos->z + ColRange.z > pos.z && pPos->z - ColRange.z < pos.z + m_fDepth)
		{// 箱の内側にいる判定だった
			if (m_bReturnFlag == true)
			{// 位置を戻すならば
				pPos->y = pos.y + m_fHeight;
			}
			bHit = true;
			if (pLand != NULL)
			{
				*pLand = true;
			}
		}
	}
	else if (pPosOld->y + ColRange.y <= pos.y && pPos->y + ColRange.y > pos.y)
	{// 高さが下から箱の内側に入ってしまった
		if (pPos->x + ColRange.x > pos.x && pPos->x - ColRange.x < pos.x + m_fWidth
			&& pPos->z + ColRange.z > pos.z && pPos->z - ColRange.z < pos.z + m_fDepth)
		{// 箱の内側にいる判定だった
			if (m_bReturnFlag == true)
			{// 位置を戻すならば
				pPos->y = pos.y - ColRange.y;
				pMove->y = 0.0f;
			}
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
//    箱の横幅設定処理
//=============================================================================
void CBoxCollider::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    箱の高さ設定処理
//=============================================================================
void CBoxCollider::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    箱の奥行設定処理
//=============================================================================
void CBoxCollider::SetDepth(const float fDepth)
{
	m_fDepth = fDepth;
}

//=============================================================================
//    当たった際に戻すかどうか設定処理
//=============================================================================
void CBoxCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    箱の横幅取得処理
//=============================================================================
float CBoxCollider::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    箱の高さ取得処理
//=============================================================================
float CBoxCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    箱の奥行取得処理
//=============================================================================
float CBoxCollider::GetDepth(void)
{
	return m_fDepth;
}

//=============================================================================
//    当たった際に戻すかどうか取得処理
//=============================================================================
bool CBoxCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}