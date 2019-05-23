//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
//静的メンバ変数
CRenderer *m_pPauseRenderer = NULL;
LPDIRECT3DTEXTURE9 CPause::m_apTexture[PAUSE_TEXTURE_MAX] = {};

bool g_bPauseDelete;
//=============================================================================
// コンストラクタ
//=============================================================================
CPause::CPause() : CScene(5)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CPause::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURE, &m_apTexture[0]);

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_CONTINUE, &m_apTexture[1]);

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_RETRY, &m_apTexture[2]);

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TEXTURE_QUIT, &m_apTexture[3]);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CPause::Unload(void)
{
		// テクスチャの破棄
	for (int nCntTex = 0; nCntTex < PAUSE_TEXTURE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CPause::Init(void)
{
	m_pCScene2D = new CScene2D(6);
	m_pCScene2D->Init();
	m_pCScene2D->BindTexture(m_apTexture[0]);

	VERTEX_2D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 Vtx;
	Vtx = m_pCScene2D->GetVtx();

	// 頂点情報を設定
	Vtx->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(400, 100, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(900, 100, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(400, 600, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(900, 600, 0.0f);

	//アンロック
	Vtx->Unlock();

	// CONTINUEポリゴンの生成
	m_pPauseSelect[SELECT_CONTINUE] = new CScene2D(7);
	m_pPauseSelect[SELECT_CONTINUE]->Init();
	m_pPauseSelect[SELECT_CONTINUE]->BindTexture(m_apTexture[1]);

	// 頂点情報を設定
	m_pPauseSelect[SELECT_CONTINUE]->GetVtx()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(500, 150, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(800, 150, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(500, 250, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(800, 250, 0.0f);

	//アンロック
	m_pPauseSelect[SELECT_CONTINUE]->GetVtx()->Unlock();

	// RETRYポリゴンの生成
	m_pPauseSelect[SELECT_RETRY] = new CScene2D(7);
	m_pPauseSelect[SELECT_RETRY]->Init();
	m_pPauseSelect[SELECT_RETRY]->BindTexture(m_apTexture[2]);

	// 頂点情報を設定
	m_pPauseSelect[SELECT_RETRY]->GetVtx()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(500, 300, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(800, 300, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(500, 400, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(800, 400, 0.0f);

	//アンロック
	m_pPauseSelect[SELECT_RETRY]->GetVtx()->Unlock();

	// QUITポリゴンの生成
	m_pPauseSelect[SELECT_QUIT] = new CScene2D(7);
	m_pPauseSelect[SELECT_QUIT]->Init();
	m_pPauseSelect[SELECT_QUIT]->BindTexture(m_apTexture[3]);

	// 頂点情報を設定
	m_pPauseSelect[SELECT_QUIT]->GetVtx()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(500, 450, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(800, 450, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(500, 550, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(800, 550, 0.0f);

	//アンロック
	m_pPauseSelect[SELECT_QUIT]->GetVtx()->Unlock();

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPause::Uninit(void)
{
	if (m_pCScene2D != NULL)
	{
		m_pCScene2D->Uninit();
		m_pCScene2D = NULL;
	}

	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{
		if (m_pPauseSelect[nCntSelect] != NULL)
		{
			m_pPauseSelect[nCntSelect]->Uninit();
			m_pPauseSelect[nCntSelect] = NULL;
		}
	}

	//リリース
	CScene::Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CPause::Update(void)
{
	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{
		if (m_pPauseSelect[nCntSelect] != NULL)
		{
			m_pPauseSelect[nCntSelect]->Update();
		}
	}

	CManager manager;

	CFade *pFade;
	pFade = CManager::GetFade();

	//サウンドの取得
	CSound *pSound;
	pSound = manager.GetSound();

	//キーボードの取得
	CInputKeyboard *pInput;
	pInput = manager.GetInputKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetInputJoypad();

	if (pInput->GetKeyboardTrigger(DIK_DOWN) == true)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
		m_aPauseMenu[m_nSelect].select = SELECTTYPE_NONE;
		m_nSelect = (m_nSelect + 1) % 3;
		m_aPauseMenu[m_nSelect].select = SELECTTYPE_SELECT;
	}
	if (pInput->GetKeyboardTrigger(DIK_UP) == true)
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
		m_aPauseMenu[m_nSelect].select = SELECTTYPE_NONE;
		m_nSelect = (m_nSelect + 2) % 3;
		m_aPauseMenu[m_nSelect].select = SELECTTYPE_SELECT;
	}
	if (pInput->GetKeyboardTrigger(DIK_S) == true )
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
		m_aPauseMenu[m_nSelect].select = SELECTTYPE_NONE;
		m_nSelect = (m_nSelect + 1) % 3;
		m_aPauseMenu[m_nSelect].select = SELECTTYPE_SELECT;
	}
	if (pInput->GetKeyboardTrigger(DIK_W) == true )
	{
		pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
		m_aPauseMenu[m_nSelect].select = SELECTTYPE_NONE;
		m_nSelect = (m_nSelect + 2) % 3;
		m_aPauseMenu[m_nSelect].select = SELECTTYPE_SELECT;
	}


	if (pInput->GetKeyboardTrigger(DIK_P) == true)
	{
		m_nSelect = 0;
		m_aPauseMenu[m_nSelect].select = SELECTTYPE_SELECT;
		m_aPauseMenu[1].select = SELECTTYPE_NONE;
		m_aPauseMenu[2].select = SELECTTYPE_NONE;
	}

	//ジョイパッド
	if (pInputJoypad != NULL)
	{
		if (pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_29) == true)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
			m_aPauseMenu[m_nSelect].select = SELECTTYPE_NONE;
			m_nSelect = (m_nSelect + 1) % 3;
			m_aPauseMenu[m_nSelect].select = SELECTTYPE_SELECT;
		}
		if (pInputJoypad->GetTrigger(CInputJoypad::DIJS_BUTTON_28) == true)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
			m_aPauseMenu[m_nSelect].select = SELECTTYPE_NONE;
			m_nSelect = (m_nSelect + 2) % 3;
			m_aPauseMenu[m_nSelect].select = SELECTTYPE_SELECT;
		}


		if (pInputJoypad->GetTrigger(11) == true)
		{
			m_nSelect = 0;
			m_aPauseMenu[m_nSelect].select = SELECTTYPE_SELECT;
			m_aPauseMenu[1].select = SELECTTYPE_NONE;
			m_aPauseMenu[2].select = SELECTTYPE_NONE;
		}
	}

	for (int nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		if (m_aPauseMenu[nCnt].select == SELECTTYPE_SELECT)
		{//選択中の色	
			m_aPauseMenu[nCnt].col = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f);
		}
		else
		{//未選択の色
			m_aPauseMenu[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		//頂点カラー
		m_pPauseSelect[nCnt]->SetColor(m_aPauseMenu[nCnt].col);
	}

	//エンターキー
	if (pInput->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		if (m_nSelect == 0)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_CANCEL);
			m_SelectMode = SELECT_CONTINUE;
		}
		else if (m_nSelect == 1)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
			m_SelectMode = SELECT_RETRY;
		}
		else if (m_nSelect == 2)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
			m_SelectMode = SELECT_QUIT;
		}

		switch (m_SelectMode)
		{
		case SELECT_CONTINUE:
			break;

		case SELECT_RETRY:
			if (pFade->GetFade() == CFade::FADE_NONE)
			{
				pFade->SetFade(CManager::MODE_GAME);
				pSound->StopSound(CSound::SOUND_LABEL_BGM002);
				pSound->PlaySound(CSound::SOUND_LABEL_BGM002);
			}
			break;
		case SELECT_QUIT:
			if (pFade->GetFade() == CFade::FADE_NONE)
			{
				pFade->SetFade(CManager::MODE_TITLE);
				pSound->StopSound(CSound::SOUND_LABEL_BGM002);
			}
			break;
		
			break;
		}
		// ポーズの開放
		CManager::GetRenderer()->ReleasePause();
	}

	//JOYPAD
	if (pInputJoypad != NULL)
	{
		if (pInputJoypad->GetTrigger(3) == true)
		{
			if (m_nSelect == 0)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_CANCEL);
				m_SelectMode = SELECT_CONTINUE;
			}
			else if (m_nSelect == 1)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
				m_SelectMode = SELECT_RETRY;
			}
			else if (m_nSelect == 2)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
				m_SelectMode = SELECT_QUIT;
			}

			switch (m_SelectMode)
			{
			case SELECT_CONTINUE:
				break;

			case SELECT_RETRY:
				if (pFade->GetFade() == CFade::FADE_NONE)
				{
					pFade->SetFade(CManager::MODE_GAME);
					pSound->StopSound(CSound::SOUND_LABEL_BGM002);
					pSound->PlaySound(CSound::SOUND_LABEL_BGM002);
				}
				break;
			case SELECT_QUIT:
				if (pFade->GetFade() == CFade::FADE_NONE)
				{
					pFade->SetFade(CManager::MODE_TITLE);
					pSound->StopSound(CSound::SOUND_LABEL_BGM002);
				}
				break;

				break;
			}
			// ポーズの開放
			CManager::GetRenderer()->ReleasePause();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CPause::Draw(void)
{
	for (int nCntSelect = 0; nCntSelect < SELECT_MAX; nCntSelect++)
	{
		if (m_pPauseSelect[nCntSelect] != NULL)
		{
			m_pPauseSelect[nCntSelect]->Draw();
		}
	}
}

//=============================================================================
// クリエイト
//=============================================================================
CPause *CPause::Create(void)
{
	CPause *pPause = NULL;
	if (pPause == NULL)
	{
		// 動的確保
		pPause = new CPause;

		if (pPause != NULL)
		{
			// 初期化処理
			pPause->Init();
		}
	}
	return pPause;
}