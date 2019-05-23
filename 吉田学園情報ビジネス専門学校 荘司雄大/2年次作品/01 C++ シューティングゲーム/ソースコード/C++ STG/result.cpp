////=============================================================================
////
//// メイン処理 [main.cpp]
//// Author : 
////
////=============================================================================
//#include "main.h"
//#include "renderer.h"
//#include "input.h"
//#include "scene.h"
//#include "scene2D.h"
//#include "manager.h"
//#include "input.h"
//#include "result.h"
//#include "fade.h"
////*****************************************************************************
//// マクロ定義
////*****************************************************************************
//
////静的メンバ変数
//CRenderer *m_pResultRenderer = NULL;
//LPDIRECT3DTEXTURE9 CResult::m_pTexture = NULL;
//
////=============================================================================
//// コンストラクタ
////=============================================================================
//CResult::CResult()
//{
//
//}
//
////=============================================================================
//// デストラクタ
////=============================================================================
//CResult::~CResult()
//{
//	m_pTexture = NULL;
//}
//
////=============================================================================
//// ロード
////=============================================================================
//HRESULT CResult::Load(void)
//{
//	CManager manager;
//
//	//テクスチャの読み込み
//	LPDIRECT3DDEVICE9 pDevice = NULL;
//	pDevice = manager.GetRenderer()->GetDevice();
//
//	// テクスチャの生成
//	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE0, &m_pTexture);
//
//	return S_OK;
//}
//
////=============================================================================
//// アンロード
////=============================================================================
//void CResult::Unload(void)
//{
//	// テクスチャの破棄
//	if (m_pTexture != NULL)
//	{
//		m_pTexture->Release();
//		m_pTexture = NULL;
//	}
//}
//
////=============================================================================
//// 初期化処理
////=============================================================================
//HRESULT CResult::Init(void)
//{
//	//CManager manager;
//
//	//LPDIRECT3DDEVICE9 pDevice = NULL;
//	//if (m_pResultRenderer == NULL)	//エラー起きたら消してみ
//	//{
//	//	pDevice = manager.GetRenderer()->GetDevice();
//	//}
//
//	//// 頂点バッファの生成
//	//pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
//	//	D3DUSAGE_WRITEONLY,
//	//	FVF_VERTEX_2D,
//	//	D3DPOOL_MANAGED,
//	//	&m_pVtxBuff,
//	//	NULL);
//
//	//CManager::SetMode(CManager::MODE_TITLE);
//
//	return S_OK;
//}
//
////=============================================================================
//// 終了処理
////=============================================================================
//void CResult::Uninit(void)
//{
//	if (m_pTexture != NULL)
//	{
//		m_pTexture->Release();
//		m_pTexture = NULL;
//	}
//}
////=============================================================================
//// 更新処理
////=============================================================================
//void CResult::Update(void)
//{
//	CManager manager;
//	CFade *pFade = CManager::GetFade();
//
//	//キーボードの取得
//	CInputKeyboard *pInput;
//	pInput = manager.GetInputKeyboard();
//
//	if (pInput->GetKeyboardTrigger(DIK_RETURN) == true)
//	{
//		pFade->SetFade(CManager::MODE_TITLE);
//	}
//}
//
////=============================================================================
//// 描画処理
////=============================================================================
//void CResult::Draw(void)
//{
//
//}
//
////=============================================================================
//// クリエイト
////=============================================================================
//CResult *CResult::Create(void)
//{
//	CResult *pResult = NULL;
//	if (pResult == NULL)
//	{
//		// 動的確保
//		pResult = new CResult;
//
//		if (pResult != NULL)
//		{
//			// 初期化処理
//			pResult->Init();
//		}
//	}
//	return pResult;
//}