//*****************************************************************************
//
//     プレイヤーの処理[player.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "character.h"
#include "manager.h"
#include "system.h"
#include "3DPolygon.h"
#include "motion.h"
#include "mode.h"
#include "effect.h"
#include "bullet.h"
#include "star.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define PLAYER_SHADOW_PRIORITY       (9)                                // プレイヤーの影の処理優先順位
#define PLAYER_SHADOW_WIDTH          (18.0f)                            // プレイヤーの影の幅
#define PLAYER_SHADOW_HEIGHT         (18.0f)                            // プレイヤーの影の高さ
#define PLAYER_SHADOW_COLOR          (D3DXCOLOR(1.0f,1.0f,1.0f,0.5f))   // プレイヤーの影の色

//=============================================================================
//    コンストラクタ
//=============================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	// 各種値の設定
	m_State = STATE_NORMAL;    // 状態
	m_nControllerIdx = 0;
	m_pCamera = NULL;
	m_bDamage = false;
	m_bAttack = false;
	m_nDamageCounter = 0;
	m_nNumScore = 0;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, int nControllerIdx, int nPriority)
{
	CPlayer *pPlayer = NULL;               // プレイヤークラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pPlayer == NULL)
		{// メモリが空になっている
			pPlayer = new CPlayer(nPriority);
			if (pPlayer != NULL)
			{// インスタンスを生成できた
				if (FAILED(pPlayer->Init(pos, rot, apModel, pMotionManager, pTextureManager, nNumParts, fAccel, fInertia, fJumpPower, fGravity, fRivisionRot, fColHeight, fColRange, nControllerIdx)))
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

	return pPlayer;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, int nControllerIdx)
{
	// 各種値の設定
	SetPos(pos);                        // 現在の座標
	SetRot(rot);                        // 現在の向き
	SetNumParts(nNumParts);             // パーツ数
	SetModel(apModel);                  // モデルクラスへのポインタ
	SetMotionManager(pMotionManager);   // モーション管轄クラスへのポインタ
	SetTextureManager(pTextureManager); // テクスチャ管轄クラスへのポインタ
	SetAccel(fAccel);                   // 加速度
	SetInertia(fInertia);               // 慣性
	SetGravity(fGravity);               // 重力
	SetJumpPower(fJumpPower);           // ジャンプ力
	SetRivisionRot(fRivisionRot);       // 向きを補正する倍率
	SetColHeight(fColHeight);           // 当たり判定を取る高さ
	SetColRange(fColRange);             // 当たり判定を取る範囲
	SetControllerIdx(nControllerIdx);   // コントローラーの番号

	// モデルの位置をニュートラルの位置にする
	SetDefaultPos();

	// モデルの向きをニュートラルの向きにする
	SetDefaultRot();

	// 影を作成する
	CShadow *pShadow = CShadow::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_SHADOW_COLOR, PLAYER_SHADOW_WIDTH, PLAYER_SHADOW_HEIGHT, PLAYER_SHADOW_PRIORITY);
	if (pShadow != NULL)
	{
		SetShadow(pShadow);
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	// 共通の終了処理
	CCharacter::Uninit();
}

//=============================================================================
//    更新処理
//=============================================================================
void CPlayer::Update(void)
{
	// 前回乗っていたオブジェクトへのポインタを保存
	SetLandSceneOld(GetLandScene());

	// 乗っているオブジェクトへのポインタを空にする
	SetLandScene(NULL);

	// 前回の座標を保存
	SetPosOld(GetPos());

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if(CManager::GetGame()->GetState() == CGame::STATE_NORMAL)
		{
			// 移動量計算処理
			InputMove();
		}
	}

	// 移動処理
	Movement();

	if (GetShadow() != NULL)
	{
		GetShadow()->SetPos(D3DXVECTOR3(GetPos().x, 1.0f, GetPos().z));
	}

	// 当たり判定処理
	Collision();

	// アクション処理
	Action();

	// 状態遷移処理
	Statement();

	if (GetMotionManager() != NULL)
	{// モーション管理クラスが生成されている
		GetMotionManager()->Update(GetModel());
	}

	if (m_bDamage == true && m_bAttack == true)
	{
		if (GetPos().y <= 0.0f)
		{
			m_bDamage = false;
		}
	}
	else if (m_bDamage == false && m_bAttack == true)
	{
		m_nDamageCounter--;
		if (m_nDamageCounter <= 0)
		{
			m_bAttack = false;
		}
	}

	CDebugProc::Print("%.1f %.1f %.1f\n", GetPos().x, GetPos().y, GetPos().z);
}

//=============================================================================
//    描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	// 共通の描画処理
	if (m_bAttack == true && m_bDamage == false)
	{
		if (m_nDamageCounter % 2 == 0)
		{
			CCharacter::Draw();
		}
	}
	else
	{
		CCharacter::Draw();
	}
}

//=============================================================================
//    入力による移動量計算処理
//=============================================================================
void CPlayer::InputMove(void)
{
	// 各種値の取得
	D3DXVECTOR3 DestRot = GetDestRot();   // 目的の向き
	D3DXVECTOR3 Move = GetMove();         // 移動量
	float fAccel = GetAccel();            // 移動スピード

	// キーボードを取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// コントローラーを取得
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(m_nControllerIdx);

	// カメラを取得
	CCamera *pCamera = m_pCamera;

	//if (pKeyboard->GetPress(DIK_A) == true)
	//{// 左方向の入力がされた
	//	if (pKeyboard->GetPress(DIK_W) == true)
	//	{// 同時に上方向の入力がされた
	//	    // 移動処理
	//		Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * fAccel;
	//		Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * fAccel;

	//		// 目的の角度変更
	//		DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
	//	}
	//	else if (pKeyboard->GetPress(DIK_S) == true)
	//	{// 同時に下方向の入力がされた
	//	    // 移動処理
	//		Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * fAccel;
	//		Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * fAccel;

	//		// 目的の角度変更
	//		DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
	//	}
	//	else
	//	{// 何も押されてない
	//	    // 移動処理
	//		Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * fAccel;
	//		Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * fAccel;

	//		// 目的の角度変更
	//		DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
	//	}
	//}
	//else if (pKeyboard->GetPress(DIK_D) == true)
	//{// 右方向の入力がされた
	//	if (pKeyboard->GetPress(DIK_W) == true)
	//	{// 同時に上方向の入力がされた
	//	    // 移動処理
	//		Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * fAccel;
	//		Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * fAccel;

	//		// 目的の角度変更
	//		DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.75f);
	//	}
	//	else if (pKeyboard->GetPress(DIK_S) == true)
	//	{// 同時に下方向の入力がされた
	//	    // 移動処理
	//		Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * fAccel;
	//		Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * fAccel;

	//		// 目的の角度変更
	//		DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.25f);
	//	}
	//	else
	//	{// 何も押されてない
	//	    // 移動処理
	//		Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * fAccel;
	//		Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * fAccel;

	//		// 目的の角度変更
	//		DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.5f);
	//	}
	//}
	//else if (pKeyboard->GetPress(DIK_W) == true)
	//{// 上方向の入力がされた
	//    // 移動処理
	//	Move.x += sinf(pCamera->GetRot().y) * fAccel;
	//	Move.z += cosf(pCamera->GetRot().y) * fAccel;

	//	// 目的の角度変更
	//	DestRot.y = pCamera->GetRot().y + D3DX_PI;
	//}
	//else if (pKeyboard->GetPress(DIK_S) == true)
	//{// 下方向の入力がされた
	//    // 移動処理
	//	Move.x += sinf(pCamera->GetRot().y + D3DX_PI) * fAccel;
	//	Move.z += cosf(pCamera->GetRot().y + D3DX_PI) * fAccel;

	//	// 目的の角度変更
	//	DestRot.y = pCamera->GetRot().y;
	//}
	//else
	//{
		if (pJoyStick != NULL)
		{// ジョイスティックが取得できた
			if (pJoyStick->GetLeftAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisX() <= -DI_STICK_AXIS_MIN
				|| pJoyStick->GetLeftAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisY() <= -DI_STICK_AXIS_MIN)
			{// 左アナログ入力がされている
			    // スティックの倒された長さの比率を計算
				float fLengthX = (float)(sqrt(pJoyStick->GetLeftAxisX() * pJoyStick->GetLeftAxisX() + pJoyStick->GetLeftAxisY() * pJoyStick->GetLeftAxisY()) / DI_JOY_STICK_INPUT);

				// コサインカーブで比率を補正
				float fRivision = cosf(sqrtf(2) - fLengthX);
				if (fRivision < 1.0)
				{// 倍率が1を超えていない
					fRivision += 0.09f;  // 足りない分を補正
					if (fRivision >= 1.0f)
					{// 倍率が1を超えてしまった
						fRivision = 1.0f;
					}
				}

				// 移動量計算処理
				//Move.x += sinf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (fAccel * fRivision);
				//Move.z -= cosf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (fAccel * fRivision);

				// 目的の角度変更
				float fValue = 0.0f;
				if (pJoyStick->GetLeftAxisX() >= DI_STICK_AXIS_MIN)
				{
					fValue = 0.05f;
				}
				if (pJoyStick->GetLeftAxisX() <= -DI_STICK_AXIS_MIN)
				{
					fValue = -0.05f;
				}

				DestRot.y += fValue * fRivision;
				if (DestRot.y > D3DX_PI)
				{
					DestRot.y -= D3DX_PI * 2.0f;
				}
				if (DestRot.y < -D3DX_PI)
				{
					DestRot.y += D3DX_PI *2.0f;
				}
			}
			else if (pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_28) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_29) == true
				|| pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_30) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_31) == true)
			{// 十字キー入力がされている
			    // 移動量計算処理
				Move.x += sinf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (fAccel);
				Move.z += cosf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (fAccel);

				// 目的の角度変更
				DestRot.y = pCamera->GetRot().y + (pJoyStick->GetRadian() - D3DX_PI);
			}

			if (pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_3) == true)
			{
				Move.x += sinf(pCamera->GetRot().y) * (fAccel);
				Move.z += cosf(pCamera->GetRot().y) * (fAccel);

				// ブーストのエフェクト生成
				CreateBoostEffect();
			}
		}
	//}

	// 各種値の設定
	SetDestRot(DestRot);    // 目的の向き
	SetMove(Move);          // 移動量
}


//=============================================================================
//    アクション処理
//=============================================================================
void CPlayer::Action(void)
{
	// コントローラーを取得
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(m_nControllerIdx);

	if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_7) == true)
	{
		D3DXVECTOR3 BulletPos = D3DXVECTOR3(0.0f, 30.0f, -30.0f);
		D3DXVec3TransformCoord(&BulletPos, &BulletPos, &GetMtxWorld());
		D3DXVECTOR3 BulletMove = D3DXVECTOR3(sinf(GetRot().y - D3DX_PI) * 40.0f, 0.0f, cosf(GetRot().y - D3DX_PI) * 40.0f);
		CBullet::Create(BulletPos, D3DXVECTOR3(GetRot().x, GetRot().y - D3DX_PI, GetRot().z), BulletMove, m_nControllerIdx);
	}
	else if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_6) == true)
	{
		D3DXVECTOR3 BulletPos = D3DXVECTOR3(0.0f, 30.0f, -30.0f);
		D3DXVec3TransformCoord(&BulletPos, &BulletPos, &GetMtxWorld());
		D3DXVECTOR3 BulletMove = D3DXVECTOR3(sinf(GetRot().y) * 20.0f, 0.0f, cosf(GetRot().y) * 20.0f);
		CBullet::Create(BulletPos, D3DXVECTOR3(GetRot().x, GetRot().y, GetRot().z), BulletMove, m_nControllerIdx);
	}
}

//=============================================================================
//    状態遷移処理
//=============================================================================
void CPlayer::Statement(void)
{
	// 各種値の取得
	D3DXVECTOR3 Move = GetMove();

	//if (Move.x <= 0.1f && Move.x >= -0.1f
	//	&& Move.z <= 0.1f && Move.z >= -0.1f)
	//{// 移動していない
	//	if (m_State != STATE_NORMAL)
	//	{// 通常状態ではない
	//	    // 状態を通常の状態に
	//		m_State = STATE_NORMAL;

	//		// モーション切り替え処理
	//		GetMotionManager()->SwitchMotion(GetModel(), m_State);
	//	}
	//}
	//else
	//{// 移動している
	//	if (m_State != STATE_MOVE)
	//	{// 歩く状態ではない
	//		m_State = STATE_MOVE;

	//		// モーション切り替え処理
	//		GetMotionManager()->SwitchMotion(GetModel(), m_State);
	//	}
	//}
}

//=============================================================================
//    ダメージを受けたときの処理
//=============================================================================
void CPlayer::Damage(D3DXVECTOR3 BulletMove)
{
	if (m_bAttack == false)
	{// ダメージをくらう状態ならば
		m_bAttack = true;
		m_bDamage = true;
		m_nDamageCounter = 180;

		D3DXVECTOR3 Move = BulletMove;
		Move.y = 20.0f;
		SetMove(Move);

		for (int nCntStar = 0; nCntStar < 10; nCntStar++)
		{
			m_nNumScore--;
			if (m_nNumScore <= 0)
			{
				m_nNumScore = 0;
			}
			else
			{
				float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
				CStar::Create(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(fAngle) * 10.0f, 20.0f, cosf(fAngle) * 10.0f));
			}
		}
	}
}

//=============================================================================
//    ブーストのエフェクトを生成する
//=============================================================================
void CPlayer::CreateBoostEffect(void)
{
	// エフェクトを出す位置を設定
	D3DXVECTOR3 EffectPos;
	EffectPos = D3DXVECTOR3(0.0f,45.0f,50.0f);
	D3DXVec3TransformCoord(&EffectPos, &EffectPos, &GetMtxWorld());
	CManager::GetBaseMode()->GetEffectManager()->SetEffect(EffectPos, GetRot(), 0);

	EffectPos = D3DXVECTOR3(30.0f, 60.0f, 50.0f);
	D3DXVec3TransformCoord(&EffectPos, &EffectPos, &GetMtxWorld());
	CManager::GetBaseMode()->GetEffectManager()->SetEffect(EffectPos, GetRot(), 0);

	EffectPos = D3DXVECTOR3(-30.0f, 60.0f, 50.0f);
	D3DXVec3TransformCoord(&EffectPos, &EffectPos, &GetMtxWorld());
	CManager::GetBaseMode()->GetEffectManager()->SetEffect(EffectPos, GetRot(), 0);

	EffectPos = D3DXVECTOR3(0.0f, 45.0f, 60.0f);
	D3DXVec3TransformCoord(&EffectPos, &EffectPos, &GetMtxWorld());
	CManager::GetBaseMode()->GetEffectManager()->SetEffect(EffectPos, GetRot(), 0);

	EffectPos = D3DXVECTOR3(30.0f, 60.0f, 60.0f);
	D3DXVec3TransformCoord(&EffectPos, &EffectPos, &GetMtxWorld());
	CManager::GetBaseMode()->GetEffectManager()->SetEffect(EffectPos, GetRot(), 0);

	EffectPos = D3DXVECTOR3(-30.0f, 60.0f, 60.0f);
	D3DXVec3TransformCoord(&EffectPos, &EffectPos, &GetMtxWorld());
	CManager::GetBaseMode()->GetEffectManager()->SetEffect(EffectPos, GetRot(), 0);
}

//=============================================================================
//    スコアを加算する
//=============================================================================
void CPlayer::AddScore(void)
{
	m_nNumScore++;
}

//=============================================================================
//    状態を設定する
//=============================================================================
void CPlayer::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    コントローラーの番号を設定する
//=============================================================================
void CPlayer::SetControllerIdx(const int nIdx)
{
	m_nControllerIdx = nIdx;
}

//=============================================================================
//    カメラクラスへのポインタを設定する
//=============================================================================
void CPlayer::SetCamera(CCamera *pCamera)
{
	m_pCamera = pCamera;
}

//=============================================================================
//    状態を取得する
//=============================================================================
CPlayer::STATE CPlayer::GetState(void)
{
	return m_State;
}

//=============================================================================
//    コントローラーの番号を取得する
//=============================================================================
int CPlayer::GetControllerIdx(void)
{
	return m_nControllerIdx;
}

//=============================================================================
//    カメラクラスへのポインタを取得する
//=============================================================================
CCamera *CPlayer::GetCamera(void)
{
	return m_pCamera;
}

int CPlayer::GetScore(void)
{
	return m_nNumScore;
}