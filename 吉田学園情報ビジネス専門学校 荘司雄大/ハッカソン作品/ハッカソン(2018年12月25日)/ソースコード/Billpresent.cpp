//*****************************************************************************
//
//     プレゼント用ビルボードの処理[Billpresent.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "Billpresent.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "effectManager.h"

//=============================================================================
//    コンストラクタ
//=============================================================================
CBillPresent::CBillPresent(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// 各種値のクリア
}

//=============================================================================
//    デストラクタ
//=============================================================================
CBillPresent::~CBillPresent()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CBillPresent *CBillPresent::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, int nPriority)
{
	CBillPresent *pBillPresent = NULL;     // プレゼント用ビルボードクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pBillPresent == NULL)
		{// メモリが空になっている
			pBillPresent = new CBillPresent(nPriority);
			if (pBillPresent != NULL)
			{// インスタンスを生成できた
			    // 各種値の代入
				pBillPresent->SetPos(pos);         // ポリゴンの座標
				pBillPresent->SetCol(col);         // ポリゴンの色
				pBillPresent->SetRot(fRot);        // ポリゴンの向き
				pBillPresent->SetWidth(fWidth);    // ポリゴンの幅
				pBillPresent->SetHeight(fHeight);  // ポリゴンの高さ

				if (FAILED(pBillPresent->Init()))
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

	return pBillPresent;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CBillPresent::Init(void)
{
	// 初期化に失敗した
	if (FAILED(CSceneBillboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CBillPresent::Uninit(void)
{
	// 共通の描画処理
	CSceneBillboard::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CBillPresent::Update(void)
{

}

//=============================================================================
//    描画処理
//=============================================================================
void CBillPresent::Draw(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			DWORD Lighting;

			// ライティングの設定を外す
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// 共通の描画処理
			CSceneBillboard::Draw();

			// ライティングの設定を元に戻す
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}
}

//=============================================================================
//    当たり判定処理
//=============================================================================
void CBillPresent::Collision(D3DXVECTOR3 pos)
{
	// 必要な変数を読み込み
	D3DXVECTOR3 BillPos = GetPos();
	float fWidth = GetWidth();
	float fHeight = GetHeight();

	if (pos.x >= BillPos.x - fWidth && pos.x <= BillPos.x + fWidth)
	{// X軸上の中にいる
		if (pos.y >= BillPos.y - fHeight && pos.y <= BillPos.y + fHeight)
		{// Y軸上の中にいる
			if (CManager::GetMode() == CManager::MODE_GAME)
			{
				CManager::GetGame()->CutPresentNum();
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESENT);
				CManager::GetGame()->AddScore();

				CEffectManager *pEffectManager = NULL;
				// エフェクト管轄クラスへのポインタを入手しておく
				if (pEffectManager == NULL)
				{// ポインタが確保できる状態である
					CScene *pScene = NULL;
					CScene *pSceneNext = NULL;
					for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
					{// 優先順位の数だけ繰り返し
						pScene = GetTop(nCntPriority);
						while (pScene != NULL)
						{// メモリが空になるまで繰り返し
							pSceneNext = pScene->GetNext();
							if (pScene->GetObjType() == OBJTYPE_EFFECTMANAGER)
							{// エフェクト管轄クラスへのポインタだった
								pEffectManager = (CEffectManager*)pScene;
								pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_PRESENT);
							}
							pScene = pSceneNext;
						}
					}
				}

				Uninit();
			}
		}
	}
}