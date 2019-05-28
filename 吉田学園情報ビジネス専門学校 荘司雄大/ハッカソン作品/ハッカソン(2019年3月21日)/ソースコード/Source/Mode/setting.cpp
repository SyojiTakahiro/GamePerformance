//*****************************************************************************
//
//     セッティングの処理[setting.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mode.h"
#include "manager.h"
#include "system.h"
#include "functionlib.h"
#include "scene.h"
#include "2DPolygon.h"
#include "textureManager.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define SETTING_SYSTEM_FILENAME    "data/TEXT/MODE/setting.ini"   // セッティングの初期化情報スクリプトファイル名
#define SETTING_MODECHANGE_TIMING  (300)                          // 画面遷移を自動で行うタイミング

// 値を読み取るパス名
#define NUM_TEXTURE                 "NUM_TEXTURE = "            // 読み込むテクスチャの数
#define TEXTURE_FILENAME            "TEXTURE_FILENAME = "       // 読み込むテクスチャのファイル名


#define MAPSELECT_MAX          (3)
#define MAPSELECT_POS_INI      (D3DXVECTOR3(SCREEN_WIDTH / 2,80.0f,0.0f))       // ポーズ項目の座標(初期値)
#define MAPSELECT_COL_INI      (D3DXCOLOR(0.5f,0.5f,0.5f,1.0f))                 // ポーズ項目の色(初期値)
#define MAPSELECT_WIDTH_INI    (180.0f)                                         // 選択されているポーズ選択項目の幅
#define MAPSELECT_HEIGHT_INI   (180.0f)                                         // 選択されているポーズ選択項目ポーズ選択項目の高さ
#define MAPSELECT_WIDTH_NONE   (150.0f)                                         // 選択されていないポーズ選択項目の幅
#define MAPSELECT_HEIGHT_NONE  (150.0f)                                         // 選択されていないポーズ選択項目の高さ
#define MAPSELECT_COL_NONE     (D3DXCOLOR(0.1f,0.1f,0.1f,1.0f))                 // 選択されていないポーズ選択項目の色(初期値)

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************


//=============================================================================
//    コンストラクタ
//=============================================================================
CSetting::CSetting()
{
	m_nSelectMapIdx = 0;   // ゲームで使用するマップのデータ
	m_pBg = NULL;        // 背景
	m_nOnePlayer = -1;

	m_bJoin[0] = false;
	m_bJoin[1] = false;
	m_pJoin[0] = NULL;
	m_pJoin[1] = NULL;
	m_pMapSelect[0] = NULL;
	m_pMapSelect[1] = NULL;
	m_pMapSelect[2] = NULL;
	m_pJoinPress = NULL;
	m_nNumPlayer = 0;
	m_State = STATE_CHECKJOIN;
}

//=============================================================================
//    デストラクタ
//=============================================================================
CSetting::~CSetting()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CSetting *CSetting::Create(void)
{
	CSetting *pSetting = NULL;  // セッティングクラスへのポインタ
	if (pSetting == NULL)
	{// メモリが確保できる状態である
		pSetting = new CSetting;
		if (pSetting != NULL)
		{// メモリが確保できた
			if (FAILED(pSetting->Init()))
				return NULL;
		}
		else
		{// メモリを確保できたなかった
			return NULL;
		}
	}
	else
	{// メモリを確保できる状態でない
		return NULL;
	}

	return pSetting;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CSetting::Init(void)
{
	CManager::GetSound()->PlaySound(1);

	// セッティングの初期化情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(SETTING_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		    // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                         // 1行分読み取り用
			char *pStrCur = NULL;                       // 現在の先頭ポインタ
			char *pStr = NULL;                          // 先頭ポインタ保存用
			int nNumTex = 0;                            // テクスチャを読み込む数
			int nCntTex = 0;                            // テクスチャを読み込んだ回数
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ読み取り用
			char aTexFileName[256] = "\0";              // テクスチャのファイル名
			CTextureManager *pTextureManager = NULL;    // テクスチャ管轄クラスへのポインタ

			if (pLine == NULL && pStr == NULL)
			{// メモリが確保できる状態である
				pLine = new char[256];
				pStr = new char[256];
				if (pLine != NULL && pStr != NULL)
				{// メモリを確保できた
				    // 文字列を初期化
					strcpy(pLine, "\0");
					strcpy(pStr, "\0");

					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// スクリプト読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// テクスチャの個数情報があった
								nNumTex = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTex >= 1)
								{// モデル数が1つ以上ある
									pTextureManager = CTextureManager::Create(nNumTex);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// テクスチャのファイルパス名情報があった
							    // テクスチャのファイルパス名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, TEXTURE_FILENAME);

								// テクスチャの読み込み
								D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);
								if (pTexture != NULL)
								{// テクスチャを読み込めた
									pTextureManager->BindTexture(pTexture, nCntTex);
									pTextureManager->SetFileName(pStr, nCntTex);
									pTexture = NULL;
									nCntTex++;
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}

					D3DVIEWPORT9 ViewPort;
					ViewPort.X = 0;
					ViewPort.Y = 0;
					ViewPort.Width = SCREEN_WIDTH;
					ViewPort.Height = SCREEN_HEIGHT;
					ViewPort.MaxZ = 1.0f;
					ViewPort.MinZ = 0.0f;
					SetCamera(CCamera::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 300.0f, ViewPort, NULL), 0);

					// メモリの開放
					if (pStr != NULL)
					{// メモリが確保されている
						delete[] pStr;
						pStr = NULL;
					}
					if (pLine != NULL)
					{// メモリが確保されている
						delete[] pLine;
						pLine = NULL;
					}

					// 背景の生成
					m_pBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f),D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
					if (m_pBg != NULL)
					{
						m_pBg->BindTexture(pTextureManager->GetTexture(0));
					}

					if (pTextureManager != NULL)
					{
						SetTextureManager(pTextureManager);
					}
				}
				else
				{// メモリを確保できなかった
					return E_FAIL;
				}
			}
			else
			{// メモリを確保できる状態でない
				return E_FAIL;
			}
			fclose(pFile);
		}
		else
		{// ファイルポインタを確保できなかった
			return E_FAIL;
		}
	}
	else
	{// ファイルポインタを確保できる状態でない
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CSetting::Uninit(void)
{
	// 共通の終了処理
	CBaseMode::Uninit();

	CManager::GetSound()->StopSound(1);
}

//=============================================================================
//    更新処理
//=============================================================================
void CSetting::Update(void)
{
	// 全てのオブジェクトを更新する
	CScene::UpdateAll();

	if (m_State == STATE_CHECKJOIN)
	{
		CheckJoin();
	}
	else if (m_State == STATE_MAPSELECT)
	{
		MapSelect();
	}

	CDebugProc::Print("セッティング画面\n");
}

//=============================================================================
//    描画処理
//=============================================================================
void CSetting::Draw(void)
{

}

//=============================================================================
//    参加受付状態の処理
//=============================================================================
void CSetting::CheckJoin(void)
{
	CJoyStick *pJoyStick = NULL;

	for (int nCntStick = 0; nCntStick < 2; nCntStick++)
	{
		pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(nCntStick);
		if (pJoyStick != NULL)
		{
			if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_11) == true)
			{
				if (m_bJoin[nCntStick] != true)
				{// 参戦していない
					D3DXVECTOR3 JoinPos = D3DXVECTOR3(960.0f, SCREEN_HEIGHT / 2, 0.0f);
					int nTexIdx = 2;
					CGame::SettingPlayer(nCntStick);
					m_bJoin[nCntStick] = true;
					m_nNumPlayer++;
					if (m_nOnePlayer == -1)
					{
						nTexIdx = 1;
						JoinPos = D3DXVECTOR3(320.0f, SCREEN_HEIGHT / 2, 0.0f);
						m_nOnePlayer = nCntStick;
					}


					m_pJoin[nCntStick] = CScene2D::Create(JoinPos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 300.0f, 340.0f);
					if (m_pJoin[nCntStick] != NULL)
					{
						m_pJoin[nCntStick]->BindTexture(GetTextureManager()->GetTexture(nTexIdx));
					}
				}
				else
				{// もう参戦している
					m_State = STATE_MAPSELECT;
					if (m_pJoin[0] != NULL)
					{
						m_pJoin[0]->Uninit();
						m_pJoin[0] = NULL;
					}
					if (m_pJoin[1] != NULL)
					{
						m_pJoin[1]->Uninit();
						m_pJoin[1] = NULL;
					}

					m_pMapSelect[0] = CScene2DFlash::Create(D3DXVECTOR3(230.0f, SCREEN_HEIGHT / 2, 0.0f), MAPSELECT_COL_INI, MAPSELECT_WIDTH_INI, MAPSELECT_HEIGHT_INI, 0.0f, CScene2DFlash::STATE_SELECT);
					if (m_pMapSelect[0] != NULL)
					{
						m_pMapSelect[0]->BindTexture(GetTextureManager()->GetTexture(3));
					}

					m_pMapSelect[1] = CScene2DFlash::Create(D3DXVECTOR3(640.0f, SCREEN_HEIGHT / 2, 0.0f), MAPSELECT_COL_NONE, MAPSELECT_WIDTH_NONE, MAPSELECT_HEIGHT_NONE, 0.0f, CScene2DFlash::STATE_NONE);
					if (m_pMapSelect[1] != NULL)
					{
						m_pMapSelect[1]->BindTexture(GetTextureManager()->GetTexture(4));
					}

					m_pMapSelect[2] = CScene2DFlash::Create(D3DXVECTOR3(1040.0f, SCREEN_HEIGHT / 2, 0.0f), MAPSELECT_COL_NONE, MAPSELECT_WIDTH_NONE, MAPSELECT_HEIGHT_NONE, 0.0f, CScene2DFlash::STATE_NONE);
					if (m_pMapSelect[2] != NULL)
					{
						m_pMapSelect[2]->BindTexture(GetTextureManager()->GetTexture(5));
					}

					CScene::DeathCheck();
				}
			}
		}
	}
}

//=============================================================================
//    マップセレクト状態の処理
//=============================================================================
void CSetting::MapSelect(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();                   // キーボードの取得
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(m_nOnePlayer);   // ジョイスティックの取得

	// 選択項目の処理
	if (pKeyboard->GetTrigger(DIK_W) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_23) == TRUE || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_31) == TRUE
		|| pKeyboard->GetRepeat(DIK_W) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_23) == TRUE || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_31) == TRUE)
	{// 上方向の入力がされた
	    // 選択されていた項目の設定
		m_pMapSelect[m_nSelectMapIdx]->SetState(CScene2DFlash::STATE_NONE);   // 状態
		m_pMapSelect[m_nSelectMapIdx]->SetCol(MAPSELECT_COL_NONE);          // 色
		m_pMapSelect[m_nSelectMapIdx]->SetWidth(MAPSELECT_WIDTH_NONE);      // 幅
		m_pMapSelect[m_nSelectMapIdx]->SetHeight(MAPSELECT_HEIGHT_NONE);    // 高さ

		// 次に選択される項目の設定
		m_pMapSelect[(m_nSelectMapIdx + (MAPSELECT_MAX - 1)) % MAPSELECT_MAX]->SetState(CScene2DFlash::STATE_SELECT);  // 状態
		m_pMapSelect[(m_nSelectMapIdx + (MAPSELECT_MAX - 1)) % MAPSELECT_MAX]->SetCol(MAPSELECT_COL_INI);            // 色
		m_pMapSelect[(m_nSelectMapIdx + (MAPSELECT_MAX - 1)) % MAPSELECT_MAX]->SetWidth(MAPSELECT_WIDTH_INI);        // 幅
		m_pMapSelect[(m_nSelectMapIdx + (MAPSELECT_MAX - 1)) % MAPSELECT_MAX]->SetHeight(MAPSELECT_HEIGHT_INI);      // 高さ

		// 番号を設定
		m_nSelectMapIdx = (m_nSelectMapIdx + (MAPSELECT_MAX - 1)) % MAPSELECT_MAX;

		// 音を鳴らす
		//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	}
	else if (pKeyboard->GetTrigger(DIK_S) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_22) == TRUE || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_30) == TRUE
		|| pKeyboard->GetRepeat(DIK_S) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_22) == TRUE || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_30) == TRUE)
	{// 下方向の入力がされた
	    // 選択されていた項目の設定
		m_pMapSelect[m_nSelectMapIdx]->SetState(CScene2DFlash::STATE_NONE);   // 状態
		m_pMapSelect[m_nSelectMapIdx]->SetCol(MAPSELECT_COL_NONE);          // 色
		m_pMapSelect[m_nSelectMapIdx]->SetWidth(MAPSELECT_WIDTH_NONE);      // 幅
		m_pMapSelect[m_nSelectMapIdx]->SetHeight(MAPSELECT_HEIGHT_NONE);    // 高さ

		// 次に選択される項目の設定
		m_pMapSelect[(m_nSelectMapIdx + 1) % MAPSELECT_MAX]->SetState(CScene2DFlash::STATE_SELECT);  // 状態
		m_pMapSelect[(m_nSelectMapIdx + 1) % MAPSELECT_MAX]->SetCol(MAPSELECT_COL_INI);            // 色
		m_pMapSelect[(m_nSelectMapIdx + 1) % MAPSELECT_MAX]->SetWidth(MAPSELECT_WIDTH_INI);        // 幅
		m_pMapSelect[(m_nSelectMapIdx + 1) % MAPSELECT_MAX]->SetHeight(MAPSELECT_HEIGHT_INI);      // 高さ

																							 // 番号を設定
		m_nSelectMapIdx = (m_nSelectMapIdx + 1) % MAPSELECT_MAX;

		// 音を鳴らす
		//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_3) == TRUE)
	{// 決定ボタンが押された
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// フェードの状態が使用されていない状態である
			CGame::SetGameMapIdx(m_nSelectMapIdx);
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
		}
	}
}