//*****************************************************************************
//
//     カメラの処理[camera.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "system.h"
#include "manager.h"
#include "mode.h"
#include "character.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
// 動画用カメラのマクロ
#define CAMERA_MOVIE_MODE            (false)                 // 動画用のカメラに切り替える(自分で操作できるカメラにする)
#define CAMERA_MOVIE_MOVE            (6.0f)                  // 動画用カメラの移動量

// カメラ起草情報マクロ
#define CAMERA_ANGEL_VIEW            (30.0f)                 // カメラの画角
#define CAMERA_ZNER                  (10.0f)                 // カメラの一番近い視錐台
#define CAMERA_ZFAR                  (25000.0f)              // カメラの一番遠い視錐台
#define CAMERA_ANGLE_SPEED           (0.01f)                 // カメラをマウスで回転させるスピード倍率
#define CAMERA_MOVE_SPEED            (1.3f)                  // カメラをマウスで移動させるスピード倍率
#define CAMERA_LENGTH_MIN            (40.0f)                 // 視点注視点の距離の最小値
#define CAMERA_LENGTH_MAX            (400.0f)                // 視点注視点の距離の最大値
#define CAMERA_LENGTH_INI            (200.0f)                // カメラ距離の初期値
#define CAMERA_POSY_UP               (30.0f)                 // カメラの高さを上げる補正値
#define CAMERA_POSY_RANGE            (300.0f)                // カメラY座標の範囲
#define CAMERA_POSY_MIN              (50.0f)                 // カメラY座標の最小値

// ゲーム中のカメラ用マクロ
#define CAMERA_ANGLEY_MIN            (-0.8f)                 // カメラ縦移動の角度最小値
#define CAMERA_ANGLEY_MAX            (D3DX_PI * 0.5f)        // カメラ縦移動の角度最大値
#define CAMERA_MOVESPEED_XZ          (0.8f)                  // カメラを動かした時の移動スピード(XZ平面)
#define CAMERA_MOVESPEED_Y           (0.8f)                  // カメラを動かした時の移動スピード(縦軸上)
#define CAMERA_ROLLSPEED_Z           (0.01f)                 // カメラの向きを動かした時の移動スピード(Z軸)
#define CAMERA_ROLLSPEED_Y           (0.025f)                // カメラの向きを動かした時の移動スピード(Y軸)
#define CAMERA_ROT_RIVISION_X        (0.9f)                 // カメラの向きを補正する倍率(X軸)
#define CAMERA_ROT_RIVISION_Y        (0.9f)                 // カメラの向きを補正する倍率(Y軸)
#define CAMERA_ROT_RIVISION_Z        (0.9f)                 // カメラの向きを補正する倍率(Z軸)

// 移動倍率マクロ
#define CAMERA_ANGLE_MOVERIVISION_X  (0.00003f)             // カメラをゲームパッドで動かしたときの横移動の倍率
#define CAMERA_ANGLE_MOVERIVISION_Y  (0.00003f)             // カメラをゲームパッドで動かしたときの縦移動の倍率
#define CAMERA_GAME_LENGTH	         (230.0f)                // ゲーム中のカメラの距離
#define CAMERA_LOCKON_LENGTH         (230.0f)                // ロックオン時のゲーム中のカメラの距離

// タイトルのカメラ用マクロ
#define CAMERA_TITLE_LENGTH_0         (300.0f)               // タイトルの視点注視点間の距離
#define CAMERA_TITLE_POSYUP_0         (70.0f)                // タイトルの視点注視点間(Y軸上)の差
#define CAMERA_TITLEPOSR_MOVESPEED_XZ (0.003f)               // カメラの注視点をタイトル中に動かした時の移動スピード(XZ平面)
#define CAMERA_TITLEPOSR_MOVESPEED_Y  (0.003f)               // カメラの注視点をタイトル中に動かした時の移動スピード(縦軸上)

//*****************************************************************************
//    CCameraの処理
//*****************************************************************************
//=============================================================================
//    コンストラクタ
//=============================================================================
CCamera::CCamera()
{
	// 各種値のクリア
	m_pPlayer = NULL;                           // プレイヤークラスへのポインタ
	m_PosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 現在の視点
	m_PosVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 目的の注視点
	m_PosVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 現在の視点と目的の注視点の差分
	m_PosVOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 前回の始点
	m_PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 現在の注視点
	m_PosRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 目的の注視点
	m_PosRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // 現在の注視点と目的の注視点の差分
	m_MoveV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 視点の移動量
	m_MoveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 注視点の移動量
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 現在の向き
	m_RotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 目的の向き
	m_RotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 目的の向きと現在の向きの差分
	m_RotOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 前回の向き
	m_VecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 上方向ベクトル
	D3DXMatrixIdentity(&m_MtxProjection);       // プロジェクションマトリックス
	D3DXMatrixIdentity(&m_MtxView);             // ビューマトリックス
	m_fLength = 0.0f;                           // 距離
	m_bMoving = false;                          // カメラを動かせるかどうか
	m_ViewPort.X = 0;                           // 描画領域左上X座標
	m_ViewPort.Y = 0;                           // 描画領域左上Y座標
	m_ViewPort.Width = 0;                       // 描画領域の幅
	m_ViewPort.Height = 0;                      // 描画領域の高さ
	m_ViewPort.MinZ = 0.0f;                     // 描画領域深度バッファ最小値
	m_ViewPort.MaxZ = 0.0f;                     // 描画領域深度バッファ最大値
}

//=============================================================================
//    デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CCamera *CCamera::Create(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot, float fLength, D3DVIEWPORT9 ViewPort, CPlayer *pPlayer)
{
	CCamera *pCamera = NULL;  // カメラクラス型のポインタ
	if (pCamera == NULL)
	{// メモリが空になっている
		pCamera = new CCamera;
		if (pCamera != NULL)
		{// インスタンスを生成できた
			if (FAILED(pCamera->Init(posV, posR, rot, fLength, ViewPort, pPlayer)))
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

	return pCamera;  	// インスタンスのアドレスを返す
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CCamera::Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot, float fLength, D3DVIEWPORT9 ViewPort, CPlayer *pPlayer)
{
	// 各種値の初期化
	m_PosV = posV;                           // 視点
	m_PosR = posR;                           // 注視点
	m_Rot = rot;                             // 向き
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // 上方向ベクトル
	m_bMoving = CAMERA_MOVIE_MODE;           // カメラを自分で操作できるかどうか

	// 視点と注視点との距離を計算
	m_fLength = fLength;
	m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
	m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
	m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;

	// ビューポート設定
	m_ViewPort = ViewPort;

	// 追尾対象を設定
	m_pPlayer = pPlayer;
	if (m_pPlayer != NULL)
	{
		m_pPlayer->SetCamera(this);
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
//    更新処理
//=============================================================================
void CCamera::Update(void)
{

#if _DEBUG  // デバッグ時の処理
	if (GetUpdate() == false)
	{// 更新中だったら
		CInputMouse *pMouse = CManager::GetMouse();

		if (pMouse != NULL)
		{// マウスが取得できた
			if (pMouse->GetMouseAxisZ() != 0)
			{// マウスのホイールボタンがスクロールされた
				m_fLength += 0.1f * pMouse->GetMouseAxisZ();
				if (m_fLength < CAMERA_LENGTH_MIN)
				{// 距離が一定の値より近くなった
					m_fLength = CAMERA_LENGTH_MIN;
				}
				else if (m_fLength > CAMERA_LENGTH_MAX)
				{// 距離が一定の値より遠くなった
					m_fLength = CAMERA_LENGTH_MAX;
				}

				// 視点の計算
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}

			if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE && pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
			{// マウスの左ボタンと右ボタンが同時に押された
				if (pMouse->GetMouseAxisX() != 0 || pMouse->GetMouseAxisY() != 0)
				{// マウスが動かされている
					// 同時に押されたときの位置から今のマウスカーソルの位置から視点注視点を移動させる向きを計算
					float fAngle = atan2f(((float)pMouse->GetPoint().x - (float)pMouse->GetPointOld().x), ((float)pMouse->GetPoint().y - (float)pMouse->GetPointOld().y));
					fAngle -= m_Rot.y;
					if (fAngle < -D3DX_PI)
					{// 向きが円周率を超えた
						fAngle += D3DX_PI * 2.0f;
					}
					if (fAngle > D3DX_PI)
					{// 向きが円周率を超えた
						fAngle -= D3DX_PI * 2.0f;
					}

					// マウスのX軸,Y軸から移動量を計算
					float fMovement = sqrtf((float)pMouse->GetMouseAxisX() * (float)pMouse->GetMouseAxisX() + (float)pMouse->GetMouseAxisY() * (float)pMouse->GetMouseAxisY());
					fMovement *= CAMERA_MOVE_SPEED;

					// 視点を移動させる
					m_PosR.x += sinf(fAngle) * fMovement;
					m_PosR.z -= cosf(fAngle) * fMovement;

					// 注視点を移動させる
					m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
					m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
				}
			}
			else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE)
			{// マウスの左クリックのみが押された
				// 向きをマウスの速度に合わせて回転させる
				// 縦軸
				m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
				if (m_Rot.y > D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.y -= D3DX_PI * 2.0f;
				}
				if (m_Rot.y < -D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.y += D3DX_PI * 2.0f;
				}

				// 横軸
				m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
				if (m_Rot.z < -D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.z = -D3DX_PI;
				}
				if (m_Rot.z > 0.0f)
				{// 向きが円周率を超えた
					m_Rot.z = 0.0f;
				}

				// 視点の位置を計算する
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}
			else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
			{// マウスの右クリックのみが押された
				// 向きをマウスの速度に合わせて回転させる
				// 縦軸
				m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
				if (m_Rot.y > D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.y -= D3DX_PI * 2.0f;
				}
				if (m_Rot.y < -D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.y += D3DX_PI * 2.0f;
				}

				// 横軸
				m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
				if (m_Rot.z < -D3DX_PI)
				{// 向きが円周率を超えた
					m_Rot.z = -D3DX_PI;
				}
				if (m_Rot.z > 0.0f)
				{// 向きが円周率を超えた
					m_Rot.z = 0.0f;
				}

				// 注視点の位置を計算する
				m_PosR.x = m_PosV.x + sinf(m_Rot.y) * m_fLength;
				m_PosR.y = m_PosV.y - cosf(m_Rot.z) * m_fLength;
				m_PosR.z = m_PosV.z + cosf(m_Rot.y) * m_fLength;
			}
		}
		// 操作方法表示
		CDebugProc::Print("\n( カメラの操作方法 )\n");
		CDebugProc::Print("[ カメラの視点移動   ]   : マウス左クリック\n");
		CDebugProc::Print("[ カメラの注視点移動 ]   : マウス右クリック\n");
		CDebugProc::Print("[ カメラの移動       ]   : マウス左右同時クリック\n");
		CDebugProc::Print("[ カメラの拡大縮小   ]   : マウスホイール\n");
	}
	else
	{// 更新中ならば
		// 前回情報を保存
		m_PosVOld = m_PosV;   // 視点
		m_RotOld = m_Rot;     // 向き

		if (CManager::GetMode() == CManager::MODE_TITLE)
		{// タイトル画面ならば
			Title();
		}
		else if (CManager::GetMode() == CManager::MODE_VIEW_MAP)
		{// マップ紹介画面ならば
			ViewMap();
		}
		else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{// チュートリアル画面ならば
			Tutorial();
		}
		else if (CManager::GetMode() == CManager::MODE_SETTING)
		{// セッティング画面ならば
			Setting();
		}
		else if (CManager::GetMode() == CManager::MODE_GAME)
		{// ゲーム画面ならば
			Game();
		}
		else if (CManager::GetMode() == CManager::MODE_RESULT)
		{// リザルト画面ならば
			Result();
		}
	}

	CDebugProc::Print("\nカメラの視点   : ( %.1f %.1f %.1f )\n", m_PosV.x, m_PosV.y, m_PosV.z);
	CDebugProc::Print("カメラの注視点 : ( %.1f %.1f %.1f )\n", m_PosR.x, m_PosR.y, m_PosR.z);
	CDebugProc::Print("カメラの向き   : ( %.2f %.2f %.2f )\n\n", m_Rot.x, m_Rot.y, m_Rot.z);

#else
if (CManager::GetMode() == CManager::MODE_TITLE)
{// タイトル画面ならば
	Title();
}
else if (CManager::GetMode() == CManager::MODE_VIEW_MAP)
{// マップ紹介画面ならば
	ViewMap();
}
else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
{// チュートリアル画面ならば
	Tutorial();
}
else if (CManager::GetMode() == CManager::MODE_SETTING)
{// セッティング画面ならば
	Setting();
}
else if (CManager::GetMode() == CManager::MODE_GAME)
{// ゲーム画面ならば
	Game();
}
else if (CManager::GetMode() == CManager::MODE_RESULT)
{// リザルト画面ならば
	Result();
}
#endif
}

//=============================================================================
//    カメラの設定
//=============================================================================
void CCamera::SetCamera(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが取得できた
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// デバイスの取得
		if (pDevice != NULL)
		{// デバイスが取得できた
			// ビューポート(描画領域)の設定
			pDevice->SetViewport(&m_ViewPort);

		    // プロジェクションマトリックスの初期化
			D3DXMatrixIdentity(&m_MtxProjection);

			// プロジェクションマトリックスを作成
			D3DXMatrixPerspectiveFovLH(&m_MtxProjection,
				D3DXToRadian(CAMERA_ANGEL_VIEW),
				(float)m_ViewPort.Width / (float)m_ViewPort.Height,
				CAMERA_ZNER,
				CAMERA_ZFAR);

			// プロジェクションマトリックスの設定
			pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProjection);

			// ビューマトリックスの初期化
			D3DXMatrixIdentity(&m_MtxView);

			// ビューマトリックスを作成
			D3DXMatrixLookAtLH(&m_MtxView,
				&m_PosV,
				&m_PosR,
				&m_VecU);

			// ビューマトリックスの設定
			pDevice->SetTransform(D3DTS_VIEW, &m_MtxView);
		}
	}
}

//=============================================================================
//    タイトル画面のカメラ処理
//=============================================================================
void CCamera::Title(void)
{

}

//=============================================================================
//    マップ紹介画面のカメラ処理
//=============================================================================
void CCamera::ViewMap(void)
{

}

//=============================================================================
//    チュートリアル画面のカメラ処理
//=============================================================================
void CCamera::Tutorial(void)
{

}

//=============================================================================
//    セッティング画面のカメラ処理
//=============================================================================
void CCamera::Setting(void)
{

}

//=============================================================================
//    ゲーム画面のカメラ処理
//=============================================================================
void CCamera::Game(void)
{
	// 前回の視点座標を保存
	m_PosVOld = m_PosV;

	//  カメラの距離を設定
	m_fLength = CAMERA_GAME_LENGTH;
	//if (m_pPlayer != NULL)
	//{// プレイヤークラスが取得できた
	//    // ジョイパッドを取得
	//	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(m_pPlayer->GetControllerIdx());
	//	if (pJoyStick != NULL)
	//	{// ジョイスティックを取得できた
	//		if (pJoyStick->GetRightAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetRightAxisX() <= -DI_STICK_AXIS_MIN
	//			|| pJoyStick->GetRightAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetRightAxisY() <= -DI_STICK_AXIS_MIN)
	//		{// 右アナログ入力がされている
	//			float RightAxisX = pJoyStick->GetRightAxisX();
	//			float RightAxisY = pJoyStick->GetRightAxisY();

	//			// 目的の角度変更
	//			m_RotDest.y += RightAxisX * CAMERA_ANGLE_MOVERIVISION_X;
	//			m_RotDest.z += RightAxisY * CAMERA_ANGLE_MOVERIVISION_Y;

	//			if (m_RotDest.y > D3DX_PI)
	//			{// 向きが円周率を超えた
	//				m_RotDest.y -= D3DX_PI * 2.0f;
	//			}
	//			if (m_RotDest.y < -D3DX_PI)
	//			{// 向きが円周率を超えた
	//				m_RotDest.y += D3DX_PI * 2.0f;
	//			}

	//			if (m_RotDest.z < CAMERA_ANGLEY_MIN)
	//			{// 向きが最大値を超えた
	//				m_RotDest.z = CAMERA_ANGLEY_MIN;
	//			}
	//			if (m_RotDest.z > CAMERA_ANGLEY_MAX)
	//			{// 向きが最小値を超えた
	//				m_RotDest.z = CAMERA_ANGLEY_MAX;
	//			}
	//		}
	//	}
	//	if (CManager::GetKeyboard()->GetPress(DIK_LEFT) == true)
	//	{
	//		m_RotDest.y -= CAMERA_ROLLSPEED_Y;
	//		if (m_RotDest.y > D3DX_PI)
	//		{// 向きが円周率を超えた
	//			m_RotDest.y -= D3DX_PI * 2.0f;
	//		}
	//		if (m_RotDest.y < -D3DX_PI)
	//		{// 向きが円周率を超えた
	//			m_RotDest.y += D3DX_PI * 2.0f;
	//		}
	//	}
	//	else if (CManager::GetKeyboard()->GetPress(DIK_RIGHT) == true)
	//	{
	//		m_RotDest.y += CAMERA_ROLLSPEED_Y;
	//		if (m_RotDest.y > D3DX_PI)
	//		{// 向きが円周率を超えた
	//			m_RotDest.y -= D3DX_PI * 2.0f;
	//		}
	//		if (m_RotDest.y < -D3DX_PI)
	//		{// 向きが円周率を超えた
	//			m_RotDest.y += D3DX_PI * 2.0f;
	//		}
	//	}
	//	if (CManager::GetKeyboard()->GetPress(DIK_UP) == true)
	//	{
	//		m_RotDest.z -= CAMERA_ROLLSPEED_Z;
	//		if (m_RotDest.z < CAMERA_ANGLEY_MIN)
	//		{// 向きが最大値を超えた
	//			m_RotDest.z = CAMERA_ANGLEY_MIN;
	//		}
	//		if (m_RotDest.z > CAMERA_ANGLEY_MAX)
	//		{// 向きが最小値を超えた
	//			m_RotDest.z = CAMERA_ANGLEY_MAX;
	//		}
	//	}
	//	else if (CManager::GetKeyboard()->GetPress(DIK_DOWN) == true)
	//	{
	//		m_RotDest.z += CAMERA_ROLLSPEED_Z;
	//		if (m_RotDest.z < CAMERA_ANGLEY_MIN)
	//		{// 向きが最大値を超えた
	//			m_RotDest.z = CAMERA_ANGLEY_MIN;
	//		}
	//		if (m_RotDest.z > CAMERA_ANGLEY_MAX)
	//		{// 向きが最小値を超えた
	//			m_RotDest.z = CAMERA_ANGLEY_MAX;
	//		}
	//	}
	//}

	if (m_pPlayer != NULL)
	{// プレイヤークラスが取得できた
	    // プレイヤーの情報を保存
		D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
		D3DXVECTOR3 PlayerRot = m_pPlayer->GetRot();

		// 目的の向きを設定
		m_RotDest.y = PlayerRot.y - D3DX_PI;

		// 目的の注視点の座標を設定
		// 向きの計算
		CalcRot();

		// 注視点をプレイヤーの位置にする
		m_PosRDest.x = PlayerPos.x - sinf(m_Rot.y + D3DX_PI) * m_fLength;
		m_PosRDest.y = PlayerPos.y + sinf(m_Rot.z + D3DX_PI) * m_fLength;
		m_PosRDest.z = PlayerPos.z - cosf(m_Rot.y + D3DX_PI) * m_fLength;

		// 注視点を少し上げる
		m_PosRDest.y = CAMERA_POSY_UP + 20.0f;

		// 視点の目的位置を計算する
		m_PosVDest.x = PlayerPos.x - sinf(m_Rot.y) * 300.0f;
		m_PosVDest.y = PlayerPos.y - sinf(m_Rot.z + D3DX_PI) * 30.0f + 130.0f;
		m_PosVDest.z = PlayerPos.z - cosf(m_Rot.y) * 300.0f;

		// 現在の視点と目的の視点の差分を計算
		m_PosVDiff = m_PosVDest - m_PosV;

		// 現在の注視点と目的の注視点の差分を計算
		m_PosRDiff = m_PosRDest - m_PosR;

		// 現在の視点を目的の視点に近づける
		m_PosV.x += m_PosVDiff.x * CAMERA_MOVESPEED_XZ;
		m_PosV.y += m_PosVDiff.y * CAMERA_MOVESPEED_Y;
		m_PosV.z += m_PosVDiff.z * CAMERA_MOVESPEED_XZ;

		// 現在の注視点を目的の注視点に近づける
		m_PosR.x += m_PosRDiff.x * CAMERA_MOVESPEED_XZ;
		m_PosR.y += m_PosRDiff.y * CAMERA_MOVESPEED_Y;
		m_PosR.z += m_PosRDiff.z * CAMERA_MOVESPEED_XZ;
	}
}

//=============================================================================
//    リザルト画面のカメラ処理
//=============================================================================
void CCamera::Result(void)
{

}

//=============================================================================
//    カメラの動き処理(動画用)
//=============================================================================
void CCamera::MovieCamera(void)
{
	// キーボードを取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetPress(DIK_J) == true)
	{// 左方向の入力がされた
		if (pKeyboard->GetPress(DIK_I) == true)
		{// 同時に上方向の入力がされた
		    // 移動処理
			m_PosR.x += sinf(m_Rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z += cosf(m_Rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVIE_MOVE;

			// 視点の計算
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else if (pKeyboard->GetPress(DIK_K) == true)
		{// 同時に下方向の入力がされた
		    // 移動処理
			m_PosR.x += sinf(m_Rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z += cosf(m_Rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVIE_MOVE;

			// 視点の計算
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else
		{// 何も押されてない
		    // 移動処理
			m_PosR.x += sinf(m_Rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z += cosf(m_Rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVIE_MOVE;

			// 視点の計算
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
	}
	else if (pKeyboard->GetPress(DIK_L) == true)
	{// 右方向の入力がされた
		if (pKeyboard->GetPress(DIK_I) == true)
		{// 同時に上方向の入力がされた
		    // 移動処理
			m_PosR.x -= sinf(m_Rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z -= cosf(m_Rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVIE_MOVE;

			// 視点の計算
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else if (pKeyboard->GetPress(DIK_K) == true)
		{// 同時に下方向の入力がされた
		    // 移動処理
			m_PosR.x -= sinf(m_Rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z -= cosf(m_Rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVIE_MOVE;

			// 視点の計算
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else
		{// 何も押されてない
		    // 移動処理
			m_PosR.x -= sinf(m_Rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z -= cosf(m_Rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVIE_MOVE;

			// 視点の計算
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
	}
	else if (pKeyboard->GetPress(DIK_I) == true)
	{// 上方向の入力がされた
	    // 移動処理
		m_PosR.x += sinf(m_Rot.y) * CAMERA_MOVIE_MOVE;
		m_PosR.z += cosf(m_Rot.y) * CAMERA_MOVIE_MOVE;

		// 視点の計算
		m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
		m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
		m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
	}
	else if (pKeyboard->GetPress(DIK_K) == true)
	{// 下方向の入力がされた
	    // 移動処理
		m_PosR.x += sinf(m_Rot.y + D3DX_PI) * CAMERA_MOVIE_MOVE;
		m_PosR.z += cosf(m_Rot.y + D3DX_PI) * CAMERA_MOVIE_MOVE;

		// 視点の計算
		m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
		m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
		m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
	}
	else if (pKeyboard->GetPress(DIK_U) == true)
	{// 上方向の入力がされた
	    // 移動処理
		m_PosR.y += 2.0f;

		// 視点の計算
		m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
		m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
		m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
	}
	else if (pKeyboard->GetPress(DIK_O) == true)
	{// 下方向の入力がされた
	    // 移動処理
		m_PosR.y -= 2.0f;

		// 視点の計算
		m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
		m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
		m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
	}

	CInputMouse *pMouse = CManager::GetMouse();

	if (pMouse != NULL)
	{// マウスが取得できた
		if (pMouse->GetMouseAxisZ() != 0)
		{// マウスのホイールボタンがスクロールされた
			m_fLength += 0.1f * pMouse->GetMouseAxisZ();
			if (m_fLength < CAMERA_LENGTH_MIN)
			{// 距離が一定の値より近くなった
				m_fLength = CAMERA_LENGTH_MIN;
			}
			else if (m_fLength > CAMERA_LENGTH_MAX)
			{// 距離が一定の値より遠くなった
				m_fLength = CAMERA_LENGTH_MAX;
			}

			// 視点の計算
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}

		if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE && pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
		{// マウスの左ボタンと右ボタンが同時に押された
			if (pMouse->GetMouseAxisX() != 0 || pMouse->GetMouseAxisY() != 0)
			{// マウスが動かされている
			 // 同時に押されたときの位置から今のマウスカーソルの位置から視点注視点を移動させる向きを計算
				float fAngle = atan2f(((float)pMouse->GetPoint().x - (float)pMouse->GetPointOld().x), ((float)pMouse->GetPoint().y - (float)pMouse->GetPointOld().y));
				fAngle -= m_Rot.y;
				if (fAngle < -D3DX_PI)
				{// 向きが円周率を超えた
					fAngle += D3DX_PI * 2.0f;
				}
				if (fAngle > D3DX_PI)
				{// 向きが円周率を超えた
					fAngle -= D3DX_PI * 2.0f;
				}

				// マウスのX軸,Y軸から移動量を計算
				float fMovement = sqrtf((float)pMouse->GetMouseAxisX() * (float)pMouse->GetMouseAxisX() + (float)pMouse->GetMouseAxisY() * (float)pMouse->GetMouseAxisY());
				fMovement *= CAMERA_MOVE_SPEED;

				// 視点を移動させる
				m_PosR.x += sinf(fAngle) * fMovement;
				m_PosR.z -= cosf(fAngle) * fMovement;

				// 注視点を移動させる
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}
		}
		else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE)
		{// マウスの左クリックのみが押された
		    // 向きをマウスの速度に合わせて回転させる
		    // 縦軸
			m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
			if (m_Rot.y > D3DX_PI)
			{// 向きが円周率を超えた
				m_Rot.y -= D3DX_PI * 2.0f;
			}
			if (m_Rot.y < -D3DX_PI)
			{// 向きが円周率を超えた
				m_Rot.y += D3DX_PI * 2.0f;
			}

			// 横軸
			m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
			if (m_Rot.z < -D3DX_PI)
			{// 向きが円周率を超えた
				m_Rot.z = -D3DX_PI;
			}
			if (m_Rot.z > 0.0f)
			{// 向きが円周率を超えた
				m_Rot.z = 0.0f;
			}

			// 視点の位置を計算する
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
		{// マウスの右クリックのみが押された
		 // 向きをマウスの速度に合わせて回転させる
		    // 縦軸
			m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
			if (m_Rot.y > D3DX_PI)
			{// 向きが円周率を超えた
				m_Rot.y -= D3DX_PI * 2.0f;
			}
			if (m_Rot.y < -D3DX_PI)
			{// 向きが円周率を超えた
				m_Rot.y += D3DX_PI * 2.0f;
			}

			// 横軸
			m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
			if (m_Rot.z < -D3DX_PI)
			{// 向きが円周率を超えた
				m_Rot.z = -D3DX_PI;
			}
			if (m_Rot.z > 0.0f)
			{// 向きが円周率を超えた
				m_Rot.z = 0.0f;
			}

			// 注視点の位置を計算する
			m_PosR.x = m_PosV.x + sinf(m_Rot.y) * m_fLength;
			m_PosR.y = m_PosV.y - cosf(m_Rot.z) * m_fLength;
			m_PosR.z = m_PosV.z + cosf(m_Rot.y) * m_fLength;
		}
	}
}

//=============================================================================
//    カメラの現在の向きを計算する処理
//=============================================================================
void CCamera::CalcRot(void)
{
	// 差分を計算
	m_RotDiff = m_RotDest - m_Rot;

	// 差分チェック
	// X軸
	if (m_RotDiff.x > D3DX_PI)
	{// 円周率を超えた
		m_RotDiff.x -= D3DX_PI * 2.0f;
	}
	if (m_RotDiff.x < -D3DX_PI)
	{// 円周率を超えた
		m_RotDiff.x += D3DX_PI * 2.0f;
	}

	// Y軸
	if (m_RotDiff.y > D3DX_PI)
	{// 円周率を超えた
		m_RotDiff.y -= D3DX_PI * 2.0f;
	}
	if (m_RotDiff.y < -D3DX_PI)
	{// 円周率を超えた
		m_RotDiff.y += D3DX_PI * 2.0f;
	}

	// Z軸
	if (m_Rot.z > D3DX_PI)
	{// 円周率を超えた
		m_Rot.z -= D3DX_PI * 2.0f;
	}
	if (m_Rot.z < -D3DX_PI)
	{// 円周率を超えた
		m_Rot.z += D3DX_PI * 2.0f;
	}

	// 向きに加算する
	m_Rot.x += m_RotDiff.x * CAMERA_ROT_RIVISION_X;
	m_Rot.y += m_RotDiff.y * CAMERA_ROT_RIVISION_Y;
	m_Rot.z += m_RotDiff.z * CAMERA_ROT_RIVISION_Z;

	// 現在の向きチェック
	// X軸
	if (m_Rot.x > D3DX_PI)
	{// 円周率を超えた
		m_Rot.x -= D3DX_PI * 2.0f;
	}
	if (m_Rot.x < -D3DX_PI)
	{// 円周率を超えた
		m_Rot.x += D3DX_PI * 2.0f;
	}

	// Y軸
	if (m_Rot.y > D3DX_PI)
	{// 円周率を超えた
		m_Rot.y -= D3DX_PI * 2.0f;
	}
	if (m_Rot.y < -D3DX_PI)
	{// 円周率を超えた
		m_Rot.y += D3DX_PI * 2.0f;
	}

	// Z軸
	if (m_Rot.z > D3DX_PI)
	{// 円周率を超えた
		m_Rot.z -= D3DX_PI * 2.0f;
	}
	if (m_Rot.z < -D3DX_PI)
	{// 円周率を超えた
		m_Rot.z += D3DX_PI * 2.0f;
	}
}

//=============================================================================
//    カメラの現在の視点の設定処理
//=============================================================================
void CCamera::SetPosV(const D3DXVECTOR3 posV)
{
	m_PosV = posV;
}

//=============================================================================
//    カメラの目的の視点の設定処理
//=============================================================================
void CCamera::SetPosVDest(const D3DXVECTOR3 posVDest)
{
	m_PosVDest = posVDest;
}

//=============================================================================
//    カメラの現在の視点と目的の視点の差分設定処理
//=============================================================================
void CCamera::SetPosVDiff(const D3DXVECTOR3 posVDiff)
{
	m_PosVDiff = posVDiff;
}

//=============================================================================
//    カメラの前回の視点の設定処理
//=============================================================================
void CCamera::SetPosVOld(const D3DXVECTOR3 posVOld)
{
	m_PosVOld = posVOld;
}

//=============================================================================
//    カメラの現在の注視点の設定処理
//=============================================================================
void CCamera::SetPosR(const D3DXVECTOR3 posR)
{
	m_PosR = posR;
}

//=============================================================================
//    カメラの目的の注視点の設定処理
//=============================================================================
void CCamera::SetPosRDest(const D3DXVECTOR3 posRDest)
{
	m_PosRDest = posRDest;
}

//=============================================================================
//    カメラの現在の注視点と目的の注視点の差分設定処理
//=============================================================================
void CCamera::SetPosRDiff(const D3DXVECTOR3 posRDiff)
{
	m_PosRDiff = posRDiff;
}

//=============================================================================
//    カメラの視点の移動量の取得処理
//=============================================================================
void CCamera::SetMoveV(const D3DXVECTOR3 MoveV)
{
	m_MoveV = MoveV;
}

//=============================================================================
//    カメラの注視点の移動量の取得処理
//=============================================================================
void CCamera::SetMoveR(const D3DXVECTOR3 MoveR)
{
	m_MoveR = MoveR;
}

//=============================================================================
//    カメラの現在の向きの設定処理
//=============================================================================
void CCamera::SetRot(const D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//=============================================================================
//    カメラの目的の向きの設定処理
//=============================================================================
void CCamera::SetRotDest(const D3DXVECTOR3 RotDest)
{
	m_RotDest = RotDest;
}

//=============================================================================
//    カメラの目的の向きと現在の向きの差分の設定処理
//=============================================================================
void CCamera::SetRotDiff(const D3DXVECTOR3 RotDiff)
{
	m_RotDiff = RotDiff;
}

//=============================================================================
//    カメラの前回の向きの設定処理
//=============================================================================
void CCamera::SetRotOld(const D3DXVECTOR3 RotOld)
{
	m_RotOld = RotOld;
}

//=============================================================================
//    カメラの向きの設定処理
//=============================================================================
void CCamera::SetMtxProjection(const D3DXMATRIX MtxProjection)
{
	m_MtxProjection = MtxProjection;
}

//=============================================================================
//    カメラの向きの設定処理
//=============================================================================
void CCamera::SetMtxView(const D3DXMATRIX MtxView)
{
	m_MtxView = MtxView;
}

//=============================================================================
//    カメラの現在の視点の取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetPosV(void)
{
	return m_PosV;
}

//=============================================================================
//    カメラの目的の視点の取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetPosVDest(void)
{
	return m_PosVDest;
}

//=============================================================================
//    カメラの現在の視点と目的の視点の差分取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetPosVDiff(void)
{
	return m_PosVDiff;
}

//=============================================================================
//    カメラの前回の視点の取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetPosVOld(void)
{
	return m_PosVOld;
}

//=============================================================================
//    カメラの現在の注視点の取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetPosR(void)
{
	return m_PosR;
}

//=============================================================================
//    カメラの目的の注視点の取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetPosRDest(void)
{
	return m_PosRDest;
}

//=============================================================================
//    カメラの現在の注視点と目的の注視点の差分取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetPosRDiff(void)
{
	return m_PosRDiff;
}

//=============================================================================
//    カメラの視点の移動量の取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetMoveV(void)
{
	return m_MoveV;
}

//=============================================================================
//    カメラの注視点の移動量の取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetMoveR(void)
{
	return m_MoveR;
}

//=============================================================================
//    カメラの現在の向きの取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    カメラの目的の向きの取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetRotDest(void)
{
	return m_RotDest;
}

//=============================================================================
//    カメラの目的の向きと現在の向きの差分の取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetRotDiff(void)
{
	return m_RotDiff;
}

//=============================================================================
//    カメラの前回の向きの取得処理
//=============================================================================
D3DXVECTOR3 CCamera::GetRotOld(void)
{
	return m_RotOld;
}

//=============================================================================
//    カメラのプロジェクションマトリックス取得処理
//=============================================================================
D3DXMATRIX CCamera::GetMtxProjection(void)
{
	return m_MtxProjection;
}

//=============================================================================
//    カメラのビューマトリックスの取得処理
//=============================================================================
D3DXMATRIX CCamera::GetMtxView(void)
{
	return m_MtxView;
}