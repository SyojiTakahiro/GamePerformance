//*****************************************************************************
//
//     マネージャーの処理[manager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "manager.h"
#include "scene.h"
#include "system.h"
#include "mode.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define MANAGER_NUM_CONTROLLER            (2)        // 使用するコントローラーの数

#ifdef _DEBUG
#define MANAGER_BGM_PLAY                  (false)    // BGMを再生するかどうか

#define MANAGER_DEBUGKEY_DEBUGPROC_CHANGE (DIK_F1)   // デバッグ表示するかしないかを切り替えるキー
#define MANAGER_DEBUGKEY_LIGHTING_CHANGE  (DIK_F2)   // ライティングON/OFFを切り替えるキー
#define MANAGER_DEBUGKEY_WIREFRAME_CHANGE (DIK_F3)   // ワイヤーフレームON/OFFを切り替えるキー
#define MANAGER_DEBUGKEY_UPDATE_STOP      (DIK_F4)   // アップデートを停止するキー
#define MANAGER_DEBUGKEY_MODECHANGE       (DIK_F7)   // 画面遷移をするキー
#define MANAGER_DEBUGKEY_RELEASEALL       (DIK_F11)  // 全てのオブジェクトを破棄するキー
#endif

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
CRenderer         *CManager::m_pRenderer = NULL;     // レンダリングクラス型のポインタ
CLight            *CManager::m_pLight = NULL;        // ライトクラス型のポインタ
CInputKeyboard    *CManager::m_pKeyboard = NULL;     // キーボードクラス型のポインタ
CInputMouse       *CManager::m_pMouse = NULL;        // マウスクラス型のポインタ
CInputJoyStick    *CManager::m_pJoyStick = NULL;     // ジョイスティッククラス型のポインタ
CXInput           *CManager::m_pXInput = NULL;       // XInputクラス型のポインタ
CSound            *CManager::m_pSound = NULL;        // サウンドクラス型のポインタ
CDebugProc        *CManager::m_pDebugProc = NULL;    // デバッグ情報クラス型のポインタ

CFade             *CManager::m_pFade = NULL;         // フェードクラス型のポインタ
CBaseMode         *CManager::m_pModeBase = NULL;     // ベースモードクラスへのポインタ

#ifdef _DEBUG
CManager::MODE    CManager::m_Mode = MODE_TITLE;     // 現在のモード
#else
CManager::MODE    CManager::m_Mode = MODE_TITLE;     // 現在のモード
#endif

#ifdef _DEBUG
bool g_bUpdate = true;                               // アップデートするかしないか
bool g_bLighting = true;                             // ライティングON/OFFを切り替えるフラグ
bool g_bWireFrame = false;                           // ワイヤーフレームで描画するかしないか
#endif

//=============================================================================
//    コンストラクタ
//=============================================================================
CManager::CManager()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CManager::~CManager()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	// レンダリングの作成
	if (m_pRenderer == NULL)
	{// メモリを確保できる状態にある
		m_pRenderer = new CRenderer;
		if (m_pRenderer != NULL)
		{// メモリを確保できた
			if (FAILED(m_pRenderer->Init(hWnd, bWindow)))  // FALSEにするとフルスクリーン
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
	}

	// ライトの作成
	if (m_pLight == NULL)
	{// メモリを確保できる状態にある
		m_pLight = new CLight;
		if (m_pLight != NULL)
		{// メモリを確保できた
			if (FAILED(m_pLight->Init()))
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
	}

	// キーボードの作成
	if (m_pKeyboard == NULL)
	{// メモリが確保できる状態にある
		m_pKeyboard = new CInputKeyboard;
		if (m_pKeyboard != NULL)
		{// メモリが確保できた
			if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリを確保できる状態にない
		return E_FAIL;
	}

	// マウスの作成
	if (m_pMouse == NULL)
	{// メモリが確保できる状態にある
		m_pMouse = new CInputMouse;
		if (m_pMouse != NULL)
		{// メモリが確保できた
			m_pMouse->Init(hInstance, hWnd);
		}
	}

	// ジョイスティックの作成
	if (m_pJoyStick == NULL)
	{// メモリが確保できる状態にある
		m_pJoyStick = new CInputJoyStick(MANAGER_NUM_CONTROLLER);
		if (m_pJoyStick != NULL)
		{// メモリが確保できた
			m_pJoyStick->Init(hInstance, hWnd);
		}
	}

	// XInputの作成
	if (m_pXInput == NULL)
	{// メモリが確保できる状態にある
		m_pXInput = new CXInput;
		if (m_pXInput != NULL)
		{// メモリが確保できた
			m_pXInput->Init();
		}
	}

	// サウンドの作成
	if (m_pSound == NULL)
	{// メモリが確保できる状態にある
		m_pSound = new CSound;
		if (m_pSound != NULL)
		{// メモリが確保できた
			if (FAILED(m_pSound->Init(hWnd)))
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリが確保できなかった
		return E_FAIL;
	}

	// デバッグ情報クラスの作成
	if (m_pDebugProc == NULL)
	{// メモリが確保できる状態にある
		m_pDebugProc = new CDebugProc;
		if (m_pDebugProc != NULL)
		{// メモリが確保できた
			m_pDebugProc->Init();
		}
	}

	// モードの設定
	SetMode(m_Mode);

	// フェードの作成
	if (m_pFade == NULL)
	{// メモリが確保できる状態にある
		m_pFade = new CFade;
		if (m_pFade != NULL)
		{// メモリが確保できた
			if (FAILED(m_pFade->Init()))
			{// 初期化に失敗した
				return E_FAIL;
			}
		}
		else
		{// メモリが確保できなかった
			return E_FAIL;
		}
	}
	else
	{// メモリが確保できなかった
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CManager::Uninit(void)
{
	// フェードの破棄
	if (m_pFade != NULL)
	{// メモリが確保されている
	    // フェードの終了処理
		m_pFade->Uninit();

		// フェードの開放
		delete m_pFade;
		m_pFade = NULL;
	}

	// デバッグ情報の破棄
	if (m_pDebugProc != NULL)
	{// メモリが確保されている
	     // デバッグ情報の終了処理
		m_pDebugProc->Uninit();

		// メモリの開放
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	// キーボードの破棄
	if (m_pKeyboard != NULL)
	{// メモリが確保されている
	    // キーボードの終了処理
		m_pKeyboard->Uninit();

		// メモリの開放
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}

	// マウスの破棄
	if (m_pMouse != NULL)
	{// メモリが確保されている
	    // マウスの終了処理
		m_pMouse->Uninit();

		// メモリの開放
		delete m_pMouse;
		m_pMouse = NULL;
	}

	// ジョイスティックの破棄
	if (m_pJoyStick != NULL)
	{// メモリが確保されている
	    // ジョイスティックの終了処理
		m_pJoyStick->Uninit();

		// メモリの開放
		delete m_pJoyStick;
		m_pJoyStick = NULL;
	}

	// XInputの破棄
	if (m_pXInput != NULL)
	{// メモリが確保されている
		// XInputの終了処理
		m_pXInput->Uninit();

		// メモリの開放
		delete m_pXInput;
		m_pXInput = NULL;
	}

	// ライトの破棄
	if (m_pLight != NULL)
	{// メモリが確保されている
	    // ライトの終了処理
		m_pLight->Uninit();

		// メモリの開放
		delete m_pLight;
		m_pLight = NULL;
	}

	// レンダリングの破棄
	if (m_pRenderer != NULL)
	{// メモリが確保されている
		// レンダリングの終了処理
		m_pRenderer->Uninit();

		// メモリの開放
		delete m_pRenderer;
		m_pRenderer = NULL;
	}

	// モードの破棄
	if (m_pModeBase != NULL)
	{// メモリが確保されている
	    // モードの終了処理
		m_pModeBase->Uninit();

		// モードの開放
		delete m_pModeBase;
		m_pModeBase = NULL;
	}

	// シーンのインスタンスをすべて破棄する
	CScene::ReleaseAll();

	// サウンドの破棄
	if (m_pSound != NULL)
	{// メモリが確保されている
	    // サウンドの終了処理
		m_pSound->Uninit();

		// メモリの開放
		delete m_pSound;
		m_pSound = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CManager::Update(void)
{
	// 操作方法を描画
#ifdef _DEBUG  // デバッグ時の処理
	CDebugProc::Print("( 操作方法一覧 )\n");
	CDebugProc::Print("デバッグ表示切替           : [ F1 ]\n");
	CDebugProc::Print("ライティング(ON/OFF)       : [ F2 ]\n");
	CDebugProc::Print("ワイヤーフレーム(ON/OFF)   : [ F3 ]\n");
	CDebugProc::Print("更新を停止する/開始する    : [ F4 ]\n");
	CDebugProc::Print("画面遷移                   : [ F7 ]\n");
	CDebugProc::Print("すべてのオブジェクトを削除 : [ F11 ]\n");

	if (m_pKeyboard != NULL)
	{// キーボードが生成されている
		if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_DEBUGPROC_CHANGE) == true)
		{// キーボードのF1キーが押された
			CDebugProc::SetDisp(CDebugProc::GetDisp() ? false : true);
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_LIGHTING_CHANGE) == true)
		{// キーボードのF2キーが押された
			g_bLighting = g_bLighting ? false : true;
			m_pRenderer->GetDevice()->SetRenderState(D3DRS_LIGHTING, g_bLighting);
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_WIREFRAME_CHANGE) == true)
		{// キーボードのF3キーが押された
			g_bWireFrame = g_bWireFrame ? false : true;
			if (g_bWireFrame == true)
			{
				m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			else
			{
				m_pRenderer->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_UPDATE_STOP) == true)
		{// キーボードのF4キーが押された
			g_bUpdate = g_bUpdate ? false : true;
		}
		else if (m_pKeyboard->GetTrigger(MANAGER_DEBUGKEY_RELEASEALL) == true)
		{// キーボードのF11キーが押された
			CScene::ReleaseAll();
		}

		if (g_bUpdate == false && m_pKeyboard->GetTrigger(DIK_1) == true)
		{
			CScene::UpdateAll();
		}
	}

	if (m_pKeyboard != NULL)
	{// メモリが確保されている
	    // キーボードの更新処理
		m_pKeyboard->Update();
	}

	if (m_pMouse != NULL)
	{// メモリが確保されている
	    // マウスの更新処理
		m_pMouse->Update();
	}

	if (m_pLight != NULL)
	{// メモリが確保されている
	    // ライトの更新処理
		m_pLight->Update();
	}

	if (g_bUpdate == true)
	{// 更新する状態ならば
		if (m_pJoyStick != NULL)
		{// メモリが確保されている
		    // ジョイスティックの更新処理
			m_pJoyStick->Update();
		}

		if (m_pXInput != NULL)
		{// メモリが確保されている
		    // XInputの更新処理
			m_pXInput->Update();
		}

		if (m_pFade != NULL)
		{// メモリが確保されている
		    // フェードの更新処理
			m_pFade->Update();
		}

		if (m_pRenderer != NULL)
		{// メモリが確保されている
		    // レンダリングの更新処理
			m_pRenderer->Update();
		}

		if (m_pModeBase != NULL)
		{// メモリが確保されている
			// モードの更新処理
			m_pModeBase->Update();
		}
	}

#else // リリース時の処理
	if (m_pKeyboard != NULL)
	{// メモリが確保されている
	    // キーボードの更新処理
		m_pKeyboard->Update();
	}

	if (m_pMouse != NULL)
	{// メモリが確保されている
	    // マウスの更新処理
		m_pMouse->Update();
	}

	if (m_pJoyStick != NULL)
	{// メモリが確保されている
	    // ジョイスティックの更新処理
		m_pJoyStick->Update();
	}

	if (m_pXInput != NULL)
	{// メモリが確保されている
	    // XInputの更新処理
		m_pXInput->Update();
	}

	if (m_pFade != NULL)
	{// メモリが確保されている
	    // フェードの更新処理
		m_pFade->Update();
	}

	if (m_pLight != NULL)
	{// メモリが確保されている
	    // ライトの更新処理
		m_pLight->Update();
	}

	if (m_pRenderer != NULL)
	{// メモリが確保されている
	    // レンダリングの更新処理
		m_pRenderer->Update();
	}

	if (m_pModeBase != NULL)
	{// メモリが確保されている
	    // モードの更新処理
		m_pModeBase->Update();
	}
#endif
}

//=============================================================================
//    描画処理
//=============================================================================
void CManager::Draw(void)
{
	if (m_pRenderer != NULL)
	{// メモリが確保されている
		m_pRenderer->Draw();
	}
}

//=============================================================================
//    次のモードを設定する
//=============================================================================
void CManager::SetMode(int mode)
{
	// 前回のモードを終了する
	if (m_pModeBase != NULL)
	{// メモリが確保されている
	    // モードの終了処理
		m_pModeBase->Uninit();

		// モードの開放
		delete m_pModeBase;
		m_pModeBase = NULL;
	}

	// すべてのオブジェクトを破棄
	CScene::ReleaseAll();

	// 現在のモード更新
	m_Mode = (MODE)mode;

	switch (mode)
	{// 次のモードによって処理わけ
	case MODE_TITLE:     // タイトル画面
		if (m_pModeBase == NULL)
		{// メモリが確保されている
			m_pModeBase = CTitle::Create();
		}
		break;
	case MODE_VIEW_MAP:  // マップ紹介画面
		if (m_pModeBase == NULL)
		{// メモリが確保されている
			m_pModeBase = CViewMap::Create();
		}
		break;
	case MODE_TUTORIAL:  // チュートリアル画面
		if (m_pModeBase == NULL)
		{// メモリが確保されている
			m_pModeBase = CTutorial::Create();
		}
		break;
	case MODE_SETTING:  // セッティング画面
		if (m_pModeBase == NULL)
		{// メモリが確保されている
			m_pModeBase = CSetting::Create();
		}
		break;
	case MODE_GAME:      // ゲーム画面
		if (m_pModeBase == NULL)
		{// メモリが確保されている
			m_pModeBase = CGame::Create();
		}
		break;
	case MODE_RESULT:    // リザルト画面
		if (m_pModeBase == NULL)
		{// メモリが確保されている
			m_pModeBase = CResult::Create();
		}
		break;
	}
}

//=============================================================================
//    レンダリングクラスのポインタを取得
//=============================================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//=============================================================================
//    ライトクラスのポインタを取得
//=============================================================================
CLight *CManager::GetLight(void)
{
	return m_pLight;
}

//=============================================================================
//    キーボードクラスのポインタを取得
//=============================================================================
CInputKeyboard *CManager::GetKeyboard(void)
{
	return m_pKeyboard;
}

//=============================================================================
//    マウスクラスのポインタを取得
//=============================================================================
CInputMouse *CManager::GetMouse(void)
{
	return m_pMouse;
}

//=============================================================================
//    ジョイスティッククラスのポインタを取得
//=============================================================================
CInputJoyStick *CManager::GetJoyStick(void)
{
	return m_pJoyStick;
}

//=============================================================================
//    XInputクラスのポインタを取得
//=============================================================================
CXInput *CManager::GetXInput(void)
{
	return m_pXInput;
}

//=============================================================================
//    サウンドクラスのポインタを取得
//=============================================================================
CSound *CManager::GetSound(void)
{
	return m_pSound;
}

//=============================================================================
//    フェードクラスのポインタを取得
//=============================================================================
CFade *CManager::GetFade(void)
{
	return m_pFade;
}

//=============================================================================
//    現在のモードを取得する
//=============================================================================
CManager::MODE CManager::GetMode(void)
{
	return m_Mode;
}

//=============================================================================
//   モードベースクラスのポインタを取得
//=============================================================================
CBaseMode *CManager::GetBaseMode(void)
{
	return m_pModeBase;
}

//=============================================================================
//   タイトルクラスのポインタを取得
//=============================================================================
CTitle *CManager::GetTitle(void)
{
	return (CTitle*)m_pModeBase;
}

//=============================================================================
//   マップ紹介画面クラスのポインタを取得
//=============================================================================
CViewMap *CManager::GetViewMap(void)
{
	return (CViewMap*)m_pModeBase;
}

//=============================================================================
//   チュートリアルクラスのポインタを取得
//=============================================================================
CTutorial *CManager::GetTutorial(void)
{
	return (CTutorial*)m_pModeBase;
}

//=============================================================================
//    セッティングクラスのポインタを取得
//=============================================================================
CSetting *CManager::GetSetting(void)
{
	return (CSetting*)m_pModeBase;
}

//=============================================================================
//    ゲームクラスのポインタを取得
//=============================================================================
CGame *CManager::GetGame(void)
{
	return (CGame*)m_pModeBase;
}

//=============================================================================
//    リザルトクラスのポインタを取得
//=============================================================================
CResult *CManager::GetResult(void)
{
	return (CResult*)m_pModeBase;
}

#ifdef _DEBUG  // デバッグ時の処理

//=============================================================================
//    更新しているかどうかを返す処理
//=============================================================================
bool GetUpdate(void)
{
	return g_bUpdate;
}

#endif