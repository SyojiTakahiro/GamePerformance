//*****************************************************************************
//
//     エミッタの処理[emitter.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "emitter.h"
#include "manager.h"
#include "camera.h"
#include "particle.h"
#include "effectManager.h"
#include "input.h"

#include "debuglog.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************

//*****************************************************************************
//    CEmitterDataの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CEmitterData::CEmitterData()
{
	m_nTexIdx = 0;     // 使用するテクスチャの番号
	m_nType = 0;       // 使用するエフェクトの番号
	m_nLife = 0;       // 寿命
	m_nLap = 0;        // 1回の放出で出すエフェクトの個数
	m_nAppear = 0;     // エフェクトを出す間隔
	m_bLoop = false;   // ループするかしないか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEmitterData::~CEmitterData()
{

}

//*****************************************************************************
//    CEmitterの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CEmitter::CEmitter(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値の設定
	m_pParData = NULL;                      // パーティクルデータへのポインタ
	m_pTexture = NULL;                      // テクスチャへのポインタ
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 座標
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 向き
	D3DXMatrixIdentity(&m_MtxWorld);        // ワールドマトリックス
	m_nTexIdx = 0;                          // 使用するテクスチャ番号
	m_nType = 0;                            // 種類
	m_nLife = 0;                            // エミッタの寿命
	m_nLap = 0;                             // 1回の放出タイミングに出すエフェクトの個数
	m_nAppear = 0;                          // 放出する間隔
	m_nCounter = 0;                         // 放出タイミングを管理するカウンター
	m_bLoop = false;                        // ループするかしないか
}

//=============================================================================
//    デストラクタ
//=============================================================================
CEmitter::~CEmitter()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CEmitter *CEmitter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTexIdx, int nType, float fRangeMax, float fRangeMin, int nLife, int nLap, int nAppear, bool bLoop, CParData *pParData, LPDIRECT3DTEXTURE9 pTexture, int nPriority)
{
	CEmitter *pEmitter = NULL;             // エミッタクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pEmitter == NULL)
		{// メモリが空になっている
			pEmitter = new CEmitter(nPriority);
			if (pEmitter != NULL)
			{// インスタンスを生成できた
				// 各種値の設定
				pEmitter->SetParData(pParData);
				pEmitter->BindTexture(pTexture);
				pEmitter->SetPos(pos);
				pEmitter->SetRot(rot);
				pEmitter->SetTexIdx(nTexIdx);
				pEmitter->SetType(nType);
				pEmitter->SetRangeMax(fRangeMax);
				pEmitter->SetRangeMin(fRangeMin);
				pEmitter->SetLife(nLife);
				pEmitter->SetLap(nLap);
				pEmitter->SetAppear(nAppear);
				pEmitter->SetLoop(bLoop);

				if (FAILED(pEmitter->Init()))
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

	return pEmitter;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CEmitter::Init(void)
{
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CEmitter::Uninit(void)
{
	// 自身のポインタを削除
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CEmitter::Update(void)
{
	if (m_bLoop != true)
	{// ループしない設定ならば
		m_nLife--;  // 寿命を減らす
	}

	if (m_nLife <= 0 && m_bLoop == false)
	{// 寿命が尽きてループしない設定ならば
		Uninit();
	}
	else
	{// 寿命がまだある
		m_nCounter++;  // カウンターを増やす
		if (m_nCounter % m_nAppear == 0)
		{// 放出するタイミングになったら
			for (int nCntParticle = 0; nCntParticle < m_nLap; nCntParticle++)
			{// 1回の放出で出す個数分繰り返し
				SetParticle();
			}
		}

		// 移動処理
		//Movement();
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CEmitter::Draw(void)
{

}

//=============================================================================
//    移動処理
//=============================================================================
void CEmitter::Movement(void)
{
	// キーボードの取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// カメラを取得
	CCamera *pCamera = CManager::GetCamera();

	if (pKeyboard->GetPress(DIK_A) == true)
	{// 左方向の入力がされた
		if (pKeyboard->GetPress(DIK_W) == true)
		{// 同時に上方向の入力がされた
		    // 移動処理
			m_Pos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * EMITTER_MOVEMENT;
			m_Pos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * EMITTER_MOVEMENT;
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// 同時に下方向の入力がされた
		    // 移動処理
			m_Pos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * EMITTER_MOVEMENT;
			m_Pos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * EMITTER_MOVEMENT;
		}
		else
		{// 何も押されてない
		    // 移動処理
			m_Pos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * EMITTER_MOVEMENT;
			m_Pos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * EMITTER_MOVEMENT;
		}
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{// 右方向の入力がされた
		if (pKeyboard->GetPress(DIK_W) == true)
		{// 同時に上方向の入力がされた
		    // 移動処理
			m_Pos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * EMITTER_MOVEMENT;
			m_Pos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * EMITTER_MOVEMENT;
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// 同時に下方向の入力がされた
		    // 移動処理
			m_Pos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * EMITTER_MOVEMENT;
			m_Pos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * EMITTER_MOVEMENT;
		}
		else
		{// 何も押されてない
		    // 移動処理
			m_Pos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * EMITTER_MOVEMENT;
			m_Pos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * EMITTER_MOVEMENT;
		}
	}
	else if (pKeyboard->GetPress(DIK_W) == true)
	{// 上方向の入力がされた
	    // 移動処理
		m_Pos.x += sinf(pCamera->GetRot().y) * EMITTER_MOVEMENT;
		m_Pos.z += cosf(pCamera->GetRot().y) * EMITTER_MOVEMENT;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{// 下方向の入力がされた
	    // 移動処理
		m_Pos.x += sinf(pCamera->GetRot().y + D3DX_PI) * EMITTER_MOVEMENT;
		m_Pos.z += cosf(pCamera->GetRot().y + D3DX_PI) * EMITTER_MOVEMENT;
	}

	if (pKeyboard->GetPress(DIK_R) == true)
	{// 縦方向の入力がされた
	    // 移動処理
		m_Pos.y += EMITTER_MOVEMENT;
	}
	else if (pKeyboard->GetPress(DIK_V) == true)
	{// 縦方向の入力がされた
	    // 移動処理
		m_Pos.y -= EMITTER_MOVEMENT;
	}

	if (pKeyboard->GetPress(DIK_Q) == true)
	{// Qキーの入力がされた
	    // 回転処理
		m_Rot.y += EMITTER_ROTATION;
		if (m_Rot.y > D3DX_PI)
		{// 円周率を超えた
			m_Rot.y -= D3DX_PI * 2.0f;
		}
	}
	else if (pKeyboard->GetPress(DIK_E) == true)
	{// Eキーの入力がされた
	    // 回転処理
		m_Rot.y -= EMITTER_ROTATION;
		if (m_Rot.y < -D3DX_PI)
		{// 円周率を超えた
			m_Rot.y += D3DX_PI * 2.0f;
		}
	}
}

//=============================================================================
//    パーティクルをセットする処理
//=============================================================================
void CEmitter::SetParticle(void)
{
	// 生成に必要なデータを作成
	D3DXVECTOR3 pos = m_Pos;                                  // 座標
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 移動量
	D3DXVECTOR3 ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 移動量の変化量
	D3DXVECTOR3 Range = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // 生成する範囲
	float fMaxRot = 0.0f;                                     // 生成時の向き(最大値)
	float fMinRot = 0.0f;                                     // 生成時の向き(最小値)
	D3DXCOLOR InitCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // 初期化時の色
	float fWidth = 0.0f;                                      // 幅
	float fHeight = 0.0f;                                     // 高さ
	float fRot = 0.0f;                                        // 向き
	D3DXCOLOR ChangeCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // 色の変化量
	float fGravity = 0.0f;                                    // 重力
	float fSpread = 0.0f;                                     // 半径の変化量
	float fChangeRot = 0.0f;                                  // 向きの変化量
	float fBouncing = 0.0f;                                   // バウンド量
	int nLife = 0;                                            // 体力
	int nRotPattern = 0;                                      // 回転のパターン
	bool bCollision = false;                                  // 当たり判定をするかしないか
	bool bDrawAddtive = false;                                // 加算合成で描画するかしないか
	bool bLighting = false;                                   // ライティングするかしないか

	// データを作成する
	if (m_pParData != NULL)
	{// メモリが確保されている
		// データから代入する部分はデータから引っ張ってくる
		fMaxRot = D3DXToRadian(m_pParData->GetMaxRot());
		fMinRot = D3DXToRadian(m_pParData->GetMinRot());
		Range = m_pParData->GetRange();
		ChangeMove = m_pParData->GetChangeMove();
		InitCol = m_pParData->GetInitCol();
		ChangeCol = m_pParData->GetChangeCol();
		fSpread = m_pParData->GetSpread();
		fGravity = m_pParData->GetGravity();
		fBouncing = m_pParData->GetBouncing();
		fChangeRot = m_pParData->GetRotSpeed();
		nRotPattern = m_pParData->GetRotPattern();
		bCollision = m_pParData->GetCollision();
		bDrawAddtive = m_pParData->GetDrawAddtive();
		bLighting = m_pParData->GetLighting();

		// ランダムに出力するところだけ計算する
		float fRandom = 0.0f;
		float fRange = 0.0f;
		float fAngle = 0.0f;

		// 向き
		fMaxRot *= 100.0f;
		fMinRot *= 100.0f;
		fRandom = (fMaxRot - fMinRot);
		if (fRandom > 0 && fRandom >= 0.1f)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fMinRot + m_Rot.y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRot = fAngle;

		// 座標の計算
		float fMag = D3DXToRadian(m_pParData->GetRangeMag());

		// X方向
		fRange = (D3DX_PI * 2.0f) - (fMag - (D3DX_PI * 2.0f));
		if (fRange > 0 && fRange >= 0.0001f)
		{
			fRandom = (float)(rand() % ((int)fRange * 100));
			pos.x += sinf(fRandom / 100.0f) * Range.x;
		}

		// Y方向
		fRange = (D3DX_PI * 2.0f) - (fMag - (D3DX_PI * 2.0f));
		if (fRange > 0 && fRange >= 0.0001f)
		{
			fRandom = (float)(rand() % ((int)fRange * 100));
			pos.y += sinf(fRandom / 100.0f) * Range.y;
		}

		// Z方向
		fRange = (D3DX_PI * 2.0f) - (fMag - (D3DX_PI * 2.0f));
		if (fRange > 0 && fRange >= 0.0001f)
		{
			fRandom = (float)(rand() % ((int)fRange * 100));
			pos.z += cosf(fRandom / 100.0f) * Range.z;
		}

		// 移動量の計算
		D3DXVECTOR3 MaxMove = m_pParData->GetMaxMove();
		D3DXVECTOR3 MinMove = m_pParData->GetMinMove();

		// X方向
		// 角度の算出
		float fAngleMax = D3DXToRadian(m_fRangeMax);
		float fAngleMin = D3DXToRadian(m_fRangeMin);
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 0.1f)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + m_Rot.y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.x - MinMove.x);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.x;
			Move.x = sinf(fAngle) * fRandom;
		}
		else
		{
			Move.x = MinMove.x;
		}


		// Y方向
		// 角度の算出
		fAngleMax = D3DXToRadian(m_fRangeMax);
		fAngleMin = D3DXToRadian(m_fRangeMin);
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 1)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + m_Rot.y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.y - MinMove.y);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.y;
			Move.y = sinf(fAngle) * fRandom;
		}
		else
		{
			Move.y = MinMove.y;
		}

		// Z方向
		// 角度の算出
		fAngleMax = D3DXToRadian(m_fRangeMax);
		fAngleMin = D3DXToRadian(m_fRangeMin);
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 1)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + m_Rot.y;
		if (fAngle > D3DX_PI)
		{// 角度が円周率を超えている
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// 角度が円周率を超えている
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.z - MinMove.z);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.z;
			Move.z = cosf(fAngle) * fRandom;
		}
		else
		{
			Move.z = MinMove.z;
		}

		// 大きさ
		// 幅
		float MaxWidth = m_pParData->GetMaxWidth();
		float MinWidth = m_pParData->GetMinWidth();
		fRange = MaxWidth - MinWidth;
		if (fRange > 0 && fRange >= 1)
		{
			fWidth = rand() % (int)fRange + MinWidth;
		}
		else
		{
			fWidth = MinWidth;
		}

		// 高さ
		float MaxHeight = m_pParData->GetMaxHeight();
		float MinHeight = m_pParData->GetMinHeight();
		fRange = MaxHeight - MinHeight;
		if (fRange > 0 && fRange >= 1)
		{
			fHeight = rand() % (int)fRange + MinHeight;
		}
		else
		{
			fHeight = MinHeight;
		}

		// 寿命
		int MaxLife = m_pParData->GetMaxLife();
		int MinLife = m_pParData->GetMinLife();
		fRange = (float)(MaxLife - MinLife);
		if (fRange > 0 && fRange >= 1)
		{
			nLife = rand() % (int)fRange + MinLife;
		}
		else
		{
			nLife = MinLife;
		}

		// パーティクルの生成
		CParticle *pParticle = CParticle::Create(pos, Move, ChangeMove, InitCol, fWidth, fHeight, fRot, ChangeCol, fGravity, fSpread, fChangeRot, fBouncing, nLife, nRotPattern, bCollision, bDrawAddtive, bLighting);
		if (pParticle != NULL)
		{// パーティクルの生成に成功した
			pParticle->BindTexture(m_pTexture);
		}
	}
}

//=============================================================================
//    パーティクルデータへのポインタ設定処理
//=============================================================================
void CEmitter::SetParData(CParData *pParData)
{
	m_pParData = pParData;
}

//=============================================================================
//    テクスチャへのポインタ設定処理
//=============================================================================
void CEmitter::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    座標設定処理
//=============================================================================
void CEmitter::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    向き設定処理
//=============================================================================
void CEmitter::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ワールドマトリックス設定処理
//=============================================================================
void CEmitter::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    使用するテクスチャ番号設定処理
//=============================================================================
void CEmitter::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    種類設定処理
//=============================================================================
void CEmitter::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    エフェクトを生成する範囲の最大値設定処理
//=============================================================================
void CEmitter::SetRangeMax(const float fRangeMax)
{
	m_fRangeMax = fRangeMax;
}

//=============================================================================
//    エフェクトを生成する範囲の最小値設定処理
//=============================================================================
void CEmitter::SetRangeMin(const float fRangeMin)
{
	m_fRangeMin = fRangeMin;
}

//=============================================================================
//    寿命設定処理
//=============================================================================
void CEmitter::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    1回の放出で生成するエフェクトの個数設定処理
//=============================================================================
void CEmitter::SetLap(const int nLap)
{
	m_nLap = nLap;
}

//=============================================================================
//    放出する間隔設定処理
//=============================================================================
void CEmitter::SetAppear(const int nAppear)
{
	m_nAppear = nAppear;
}

//=============================================================================
//    ループするかしないか設定処理
//=============================================================================
void CEmitter::SetLoop(const bool bLoop)
{
	m_bLoop = bLoop;
}

//=============================================================================
//    座標取得処理
//=============================================================================
D3DXVECTOR3 CEmitter::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    向き取得処理
//=============================================================================
D3DXVECTOR3 CEmitter::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ワールドマトリックス取得処理
//=============================================================================
D3DXMATRIX CEmitter::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    使用するテクスチャ番号取得処理
//=============================================================================
int CEmitter::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    種類取得処理
//=============================================================================
int CEmitter::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    エフェクトを生成する範囲の最大値取得処理
//=============================================================================
float CEmitter::GetRangeMax(void)
{
	return m_fRangeMax;
}

//=============================================================================
//    エフェクトを生成する範囲の最小値取得処理
//=============================================================================
float CEmitter::GetRangeMin(void)
{
	return m_fRangeMin;
}

//=============================================================================
//    寿命取得処理
//=============================================================================
int CEmitter::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    1回の放出で生成するエフェクトの個数取得処理
//=============================================================================
int CEmitter::GetLap(void)
{
	return m_nLap;
}

//=============================================================================
//    放出する間隔取得処理
//=============================================================================
int CEmitter::GetAppear(void)
{
	return m_nAppear;
}

//=============================================================================
//    ループするかしないか取得処理
//=============================================================================
bool CEmitter::GetLoop(void)
{
	return m_bLoop;
}