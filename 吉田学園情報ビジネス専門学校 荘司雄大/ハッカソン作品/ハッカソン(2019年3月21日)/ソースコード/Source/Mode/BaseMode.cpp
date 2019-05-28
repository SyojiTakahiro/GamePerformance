//*****************************************************************************
//
//     モードベースの処理[BaseMode.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mode.h"
#include "manager.h"
#include "system.h"
#include "2DPolygon.h"
#include "character.h"
#include "effect.h"
#include "map.h"
#include "textureManager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************

//=============================================================================
//    コンストラクタ
//=============================================================================
CBaseMode::CBaseMode()
{
	// 各種値のクリア
	m_pPress = NULL;          // PRESSENTERクラスへのポインタ
	m_pTextureManager = NULL; // テクスチャ管轄クラスへのポインタ
	m_nModeCounter = 0;       // モードカウンター(主に画面遷移に使用)
	m_bModeChange = false;    // 画面遷移をしているかどうか
	m_pPlayerManager = NULL;  // プレイヤー管轄クラスへのポインタ
	m_pPlayer = NULL;         // プレイヤークラスへのポインタ
	m_pEffectManager = NULL;  // エフェクト管轄クラスへのポインタ
	m_pMap = NULL;            // マップ管轄クラスへのポインタ
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA_NUM; nCntCamera++)
	{// カメラの最大数分繰り返し
		m_pCamera[nCntCamera] = NULL;  // カメラクラスへのポインタ
	}
}

//=============================================================================
//    デストラクタ
//=============================================================================
CBaseMode::~CBaseMode()
{

}

//=============================================================================
//    終了処理
//=============================================================================
void CBaseMode::Uninit(void)
{
	// プレイヤー管轄を開放する
	ReleasePlayerManager();

	// エフェクト管轄クラスを開放する
	ReleaseEffectManager();

	// マップ管轄クラスを開放する
	ReleaseMap();

	// カメラクラスを開放する
	ReleaseCamera();
}

//=============================================================================
//    画面遷移するかどうかチェックする処理
//=============================================================================
void CBaseMode::ModeChange(int nAutoNext, int nPressNext, int nChangeTiming)
{
	if (m_bModeChange == false)
	{// 画面遷移をしていない
		// カウンターを進める
		m_nModeCounter++;

		if (m_nModeCounter >= nChangeTiming)
		{// カウンターが既定の値を超えた
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// フェードの状態が使用されていない状態である
				CManager::GetFade()->SetFade(nAutoNext);
				m_bModeChange = true;
			}
		}
		else
		{// カウンターがまだ既定の値を超えていない
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// フェードの状態が使用されていない状態である
				if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
				{// 決定ボタンが押された
					CManager::GetFade()->SetFade(nPressNext);
					if (m_pPress != NULL)
					{// メモリが確保されている
						m_pPress->FlashStart();
					}
					m_bModeChange = true;
					//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
				}
				else
				{// キーボードの決定ボタンが押されていない
					for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_7; nCntButton++)
					{// 反応するボタン数の数だけ繰り返し
						if (CManager::GetJoyStick()->GetJoyStickDevice()->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
						{// ボタンが押された
							CManager::GetFade()->SetFade(nPressNext);
							if (m_pPress != NULL)
							{// メモリが確保されている
								m_pPress->FlashStart();
							}
							m_bModeChange = true;
							//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
						}
					}
				}
			}
		}
	}
}

//=============================================================================
//    テクスチャ管轄クラスを破棄する
//=============================================================================
void CBaseMode::ReleaseTextureManager(void)
{
	if (m_pTextureManager != NULL)
	{// メモリが確保されている
		m_pTextureManager->Uninit();

		// メモリの開放
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    プレイヤー管轄クラスを破棄する
//=============================================================================
void CBaseMode::ReleasePlayerManager(void)
{
	if (m_pPlayerManager != NULL)
	{// メモリが確保されている
		m_pPlayerManager->Uninit();

		// メモリの開放
		delete m_pPlayerManager;
		m_pPlayerManager = NULL;
	}
}

//=============================================================================
//    エフェクト管轄クラスを破棄する
//=============================================================================
void CBaseMode::ReleaseEffectManager(void)
{
	if (m_pEffectManager != NULL)
	{// メモリが確保されている
		m_pEffectManager->Uninit();

		// メモリの開放
		delete m_pEffectManager;
		m_pEffectManager = NULL;
	}
}

//=============================================================================
//    マップ管轄クラスを破棄する
//=============================================================================
void CBaseMode::ReleaseMap(void)
{
	if (m_pMap != NULL)
	{// メモリが確保されている
		m_pMap->Uninit();

		// メモリの開放
		delete m_pMap;
		m_pMap = NULL;
	}
}

//=============================================================================
//    カメラクラスを破棄する
//=============================================================================
void CBaseMode::ReleaseCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA_NUM; nCntCamera++)
	{// カメラの最大数分繰り返し
		if (m_pCamera[nCntCamera] != NULL)
		{// メモリが確保されている
			m_pCamera[nCntCamera]->Uninit();

			// メモリの開放
			delete 	m_pCamera[nCntCamera];
			m_pCamera[nCntCamera] = NULL;
		}
	}
}

//=============================================================================
//    モードカウンターを設定する
//=============================================================================
void CBaseMode::SetModeCounter(const int nModeCounter)
{
	m_nModeCounter = nModeCounter;
}

//=============================================================================
//    PRESSENTERクラスへのポインタを設定する
//=============================================================================
void CBaseMode::SetPress(CScene2DPress *pPress)
{
	m_pPress = pPress;
}

//=============================================================================
//    テクスチャ管轄クラスへのポインタを設定する
//=============================================================================
void CBaseMode::SetTextureManager(CTextureManager *pTextureManager)
{
	m_pTextureManager = pTextureManager;
}

//=============================================================================
//    画面遷移をしているかどうかを設定する
//=============================================================================
void CBaseMode::SetModeChange(const bool bModeChange)
{
	m_bModeChange = bModeChange;
}

//=============================================================================
//    プレイヤー管轄クラスへのポインタを設定する
//=============================================================================
void CBaseMode::SetPlayerManager(CPlayerManager *pPlayerManager)
{
	m_pPlayerManager = pPlayerManager;
}

//=============================================================================
//    プレイヤークラスへのポインタを設定する
//=============================================================================
void CBaseMode::SetPlayer(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;
}

//=============================================================================
//    エフェクト管轄クラスへのポインタを設定する
//=============================================================================
void CBaseMode::SetEffectManager(CEffectManager *pEffectManager)
{
	m_pEffectManager = pEffectManager;
}

//=============================================================================
//    マップ管轄クラスへのポインタを設定する
//=============================================================================
void CBaseMode::SetMap(CMap *pMap)
{
	m_pMap = pMap;
}

//=============================================================================
//    カメラクラスへのポインタを設定する
//=============================================================================
void CBaseMode::SetCamera(CCamera *pCamera, int nIdx)
{
	m_pCamera[nIdx] = pCamera;
}

//=============================================================================
//    モードカウンターを取得する
//=============================================================================
int CBaseMode::GetModeCounter(void)
{
	return m_nModeCounter;
}

//=============================================================================
//    PRESSENTERクラスへのポインタを取得する
//=============================================================================
CScene2DPress *CBaseMode::GetPress(void)
{
	return m_pPress;
}

//=============================================================================
//    テクスチャ管轄クラスへのポインタを取得する
//=============================================================================
CTextureManager *CBaseMode::GetTextureManager(void)
{
	return m_pTextureManager;
}

//=============================================================================
//    画面遷移をしているかどうかを設定する
//=============================================================================
bool CBaseMode::GetModeChange(void)
{
	return m_bModeChange;
}

//=============================================================================
//    プレイヤー管轄クラスへのポインタを取得する
//=============================================================================
CPlayerManager *CBaseMode::GetPlayerManager(void)
{
	return m_pPlayerManager;
}

//=============================================================================
//    プレイヤークラスへのポインタを取得する
//=============================================================================
CPlayer *CBaseMode::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
//    エフェクト管轄クラスへのポインタを取得する
//=============================================================================
CEffectManager *CBaseMode::GetEffectManager(void)
{
	return m_pEffectManager;
}

//=============================================================================
//    マップ管轄クラスへのポインタを取得する
//=============================================================================
CMap *CBaseMode::GetMap(void)
{
	return m_pMap;
}

//=============================================================================
//    カメラクラスへのポインタを取得する
//=============================================================================
CCamera *CBaseMode::GetCamera(int nIdx)
{
	return m_pCamera[nIdx];
}