//*****************************************************************************
//
//     プレゼントの処理[stock.cpp]
//     Auther:Takuto Ishida
//
//*****************************************************************************
#include "present.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "textureManager.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define STOCK_FRAME_TEXTURE	"data/TEXTURE/GAME/UI/stock.png"	// タイマーの枠テクスチャのパス

//=============================================================================
//    コンストラクタ
//=============================================================================
CPresent::CPresent(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_apNumber = NULL;                           // 数字ポリゴンクラスへのポインタ
	m_nDigit = 0;                                // プレゼントの桁数
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // プレゼントの座標
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // プレゼントの色
	m_fWidth = 0.0f;                             // プレゼントの数字ポリゴン１つ分の幅
	m_fHeight = 0.0f;                            // プレゼントの数字ポリゴン１つ分の幅
	m_nPresent = 0;                              // プレゼント
	m_pTexture = NULL;                           // テクスチャへのポインタ
	m_pTextureManager = NULL;					 // テクスチャ管理クラス
	m_pFrame = NULL;							 // 枠
}

//=============================================================================
//    デストラクタ
//=============================================================================
CPresent::~CPresent()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CPresent *CPresent::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPresent, int nPriority)
{
	CPresent *pPresent = NULL;                 // プレゼントクラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pPresent == NULL)
		{// メモリが空になっている
			pPresent = new CPresent(nPriority);
			if (pPresent != NULL)
			{// メモリを確保できた
			 // 各種値の設定
				pPresent->SetPos(pos);                    // プレゼントの座標
				pPresent->SetCol(col);                    // プレゼントの色
				pPresent->SetWidth(fWidth);               // プレゼントの数字ポリゴン１つ分の幅
				pPresent->SetHeight(fHeight);             // プレゼントの数字ポリゴン１つ分の高さ
				pPresent->SetPresent(nPresent);               // プレゼント

				if (FAILED(pPresent->Init()))
				{// 初期化に失敗した
					return NULL;
				}
			}
			else
			{// メモリを確保できなかった
				return NULL;
			}
		}
		else
		{// メモリが空ではない
			return NULL;
		}
	}
	else
	{// これ以上生成できない
		return NULL;
	}

	return pPresent;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CPresent::Init(void)
{
	// 各種値の初期化
	D3DXVECTOR3 NumberPos = m_Pos;   // 座標
	D3DXCOLOR NumberCol = m_Col;     // 色
	float fNumberWidth = m_fWidth;   // 幅
	float fNumberHeight = m_fHeight; // 高さ
	int nNumber = 0;                 // 表示する数字の計算用

	if (m_pFrame == NULL)
	{// メモリが確保できる状態である
		m_pFrame = CScene2D::Create(m_Pos + D3DXVECTOR3(-20.0f,0.0f,0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), m_fWidth * 2.5f, m_fHeight * 1.5f, 0.0f, 5);
		if (m_pFrame != NULL)
		{// メモリを確保できた
			LPDIRECT3DTEXTURE9 pTexture = NULL;
			D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), STOCK_FRAME_TEXTURE, &pTexture);
			m_pTextureManager = CTextureManager::Create(1);

			if (m_pTextureManager != NULL)
			{// テクスチャ管理クラスの生成に成功
				m_pTextureManager->BindTexture(pTexture, 0);
				m_pTextureManager->SetFileName(STOCK_FRAME_TEXTURE, 0);
				m_pFrame->BindTexture(m_pTextureManager->GetTexture(0));
				pTexture = NULL;
			}
		}
	}

	// プレゼントの桁数を計算し、桁数分メモリを確保
	m_nDigit = (int)log10f((float)10) + 1;   // 桁数を求める
	if (m_nDigit <= 0) { m_nDigit = 1; }	         // 0以下のとき1にする

	if (m_apNumber == NULL)
	{// メモリが確保できる状態である
		m_apNumber = new CNumber*[m_nDigit];  // 桁数分メモリ確保
		if (m_apNumber != NULL)
		{// メモリを確保できた
			for (int nCntPresent = 0; nCntPresent < m_nDigit; nCntPresent++)
			{// スコアの桁数分繰り返し
				m_apNumber[nCntPresent] = NULL;   // いったんメモリを空にする
				if (m_apNumber[nCntPresent] == NULL)
				{// メモリが確保できる状態にある
				    // 表示する数字の計算
					nNumber = m_nPresent % ((int)powf(10.0f, (float)nCntPresent) * 10) / (int)powf(10.0f, (float)nCntPresent);

					// 数字ポリゴンのインスタンス生成処理
					m_apNumber[nCntPresent] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, CNumber::STATE_NONE, nNumber, 3, GetPriority());

					// 横に進める
					NumberPos.x -= m_fWidth + (m_fWidth * 0.5f);
				}
			}
		}
	}
	return S_OK;
}

//=============================================================================
//    終了処理
//=============================================================================
void CPresent::Uninit(void)
{
	// 数字ポリゴンの開放処理
	if (m_apNumber != NULL)
	{// メモリが確保されている
		for (int nCntPresent = 0; nCntPresent < m_nDigit; nCntPresent++)
		{// スコアの桁数分繰り返し
			if (m_apNumber[nCntPresent] != NULL)
			{// メモリが確保されている
			 // 数字ポリゴンの終了処理
				m_apNumber[nCntPresent]->Uninit();
				m_apNumber[nCntPresent] = NULL;
			}
		}
		delete[] m_apNumber;
		m_apNumber = NULL;
	}

	// テクスチャ管轄クラスを開放する
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// メモリの開放
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// プレゼントの消去処理
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CPresent::Update(void)
{
}

//=============================================================================
//    描画処理
//=============================================================================
void CPresent::Draw(void)
{

}

//=============================================================================
//    数字テクスチャを設定する処理
//=============================================================================
void CPresent::SetPresentTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
	if (m_pTexture != NULL)
	{// テクスチャへのポインタが取得できた
		if (m_apNumber != NULL)
		{// 数字ポリゴンが生成されている
			for (int nCntPresent = 0; nCntPresent < m_nDigit; nCntPresent++)
			{// プレゼントの桁数分繰り返し
				if (m_apNumber[nCntPresent] != NULL)
				{// 数字ポリゴンが生成されている
					m_apNumber[nCntPresent]->BindTexture(m_pTexture);
				}
			}
		}
	}
}

//=============================================================================
//    プレゼントの座標を設定する処理
//=============================================================================
void CPresent::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    プレゼントの色を設定する処理
//=============================================================================
void CPresent::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
//    プレゼントの数字ポリゴン１つ分の幅を設定する処理
//=============================================================================
void CPresent::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    プレゼントの数字ポリゴン１つ分の高さを設定する処理
//=============================================================================
void CPresent::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    プレゼントを設定する処理
//=============================================================================
void CPresent::SetPresent(const int nPresent)
{
	int nNumber = 0;
	m_nPresent = nPresent;
	if (m_apNumber != NULL)
	{// 数字ポリゴンが生成されている
		for (int nCntPresent = 0; nCntPresent < m_nDigit; nCntPresent++)
		{// 残機の桁数分繰り返し
		    // 表示する数字の計算
			nNumber = m_nPresent % ((int)powf(10.0f, (float)nCntPresent) * 10) / (int)powf(10.0f, (float)nCntPresent);
			m_apNumber[nCntPresent]->SetNumber(nNumber);
		}
	}
}

//=============================================================================
//    プレゼントの座標を取得する処理
//=============================================================================
D3DXVECTOR3 CPresent::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    プレゼントの色を取得する処理
//=============================================================================
D3DXCOLOR CPresent::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    プレゼントの数字ポリゴン１つ分の幅を取得する処理
//=============================================================================
float CPresent::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    プレゼントの数字ポリゴン１つ分の高さを取得する処理
//=============================================================================
float CPresent::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    プレゼントを取得する処理
//=============================================================================
int CPresent::GetPresent(void)
{
	return m_nPresent;
}
