//*****************************************************************************
//
//     デバッグ情報の処理[debuglog.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "system.h"
#include "manager.h"

//*****************************************************************************
//    静的メンバ変数宣言
//*****************************************************************************
LPD3DXFONT CDebugProc::m_pFont = NULL;                 // フォントへのポインタ
char       CDebugProc::m_aStr[MAX_DEBUG_LOG] = "";     // 文字列格納用変数
#ifdef _DEBUG
bool       CDebugProc::m_bDisp = false;                 // 描画するかしないか
#else
bool       CDebugProc::m_bDisp = false;                // 描画するかしないか
#endif

//=============================================================================
//    コンストラクタ
//=============================================================================
CDebugProc::CDebugProc()
{

}

//=============================================================================
//    デストラクタ
//=============================================================================
CDebugProc::~CDebugProc()
{

}

//=============================================================================
//    初期化処理
//=============================================================================
void CDebugProc::Init(void)
{
	// レンダリングクラス型のポインタ
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// レンダリングクラスが生成されている
	    // デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// デバイスが取得できた
			// デバッグ情報表示用フォントの生成
			//D3DXCreateFont(m_pD3DDevice, 18, 0, FW_HEAVY, 0, FALSE, SHIFTJIS_CHARSET,
			//	OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
			D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
				OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
		}
	}

	// 文字列をクリアする
	ZeroMemory(&m_aStr, sizeof(m_aStr));
}

//=============================================================================
//    終了処理
//=============================================================================
void CDebugProc::Uninit(void)
{
	// デバッグ情報表示用フォントの破棄
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================================================================
//    更新処理
//=============================================================================
void CDebugProc::Update(void)
{
}

//=============================================================================
//    表示するデバッグ情報を文字列に格納する処理
//=============================================================================
void CDebugProc::Print(char *fmt, ...)
{
	if (m_bDisp == true)
	{// 描画する状態である
		va_list list;   // 可変長引数(... の引数を受け取ることができる)
		char Str[256];  // 文字列格納用変数

		va_start(list, fmt);       // 可変長引数リストを初期化(fmt以下の引数を登録)
		vsprintf(Str, fmt, list);  // 文字列を標準出力にのっとって書き出し
		va_end(list);              // 引数リストをクリア
		strcat(m_aStr, Str);       // 格納した文字列をメンバ変数の後ろにくっつける
	}
}

//=============================================================================
//    文字列を画面に表示する処理
//=============================================================================
void CDebugProc::Draw(void)
{
	if (m_bDisp == true)
	{// 描画する状態である
		if (m_pFont != NULL)
		{// フォントへのポインタが生成されている
			RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; 	// 描画する領域(左上X座標,左上Y座標,右下X座標,右下Y座標)

			// デバッグ情報描画
			m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
		}
	}
	// 文字列をクリアする
	ZeroMemory(&m_aStr, sizeof(m_aStr));
}

//=============================================================================
//    描画するかしないかを設定する処理
//=============================================================================
void CDebugProc::SetDisp(bool bDisp)
{
	m_bDisp = bDisp;
}

//=============================================================================
//    描画するかしないかを取得する処理
//=============================================================================
bool CDebugProc::GetDisp(void)
{
	return m_bDisp;
}