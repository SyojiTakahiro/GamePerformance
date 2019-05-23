//*****************************************************************************
//
//     残機の処理[stock.cpp]
//     Auther:Takuto Ishida
//
//*****************************************************************************
#include "stock.h"
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
CStock::CStock(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// 各種値のクリア
	m_apNumber = NULL;                           // 数字ポリゴンクラスへのポインタ
	m_nDigit = 0;                                // 残機の桁数
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 残機の座標
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // 残機の色
	m_fWidth = 0.0f;                             // 残機の数字ポリゴン１つ分の幅
	m_fHeight = 0.0f;                            // 残機の数字ポリゴン１つ分の幅
	m_nStock = 0;                                // 残機
	m_pTexture = NULL;                           // テクスチャへのポインタ
	m_pTextureManager = NULL;					 // テクスチャ管理クラス
	m_pFrame = NULL;							 // 枠
}

//=============================================================================
//    デストラクタ
//=============================================================================
CStock::~CStock()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CStock *CStock::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nStock, int nPriority)
{
	CStock *pStock = NULL;                 // 残機クラス型のポインタ
	int nIdxScene = CScene::GetNumAll();   // シーンクラスの生成した数を取得
	if (nIdxScene < MAX_SCENE)
	{// まだ生成できる
		if (pStock == NULL)
		{// メモリが空になっている
			pStock = new CStock(nPriority);
			if (pStock != NULL)
			{// メモリを確保できた
			 // 各種値の設定
				pStock->SetPos(pos);                    // 残機の座標
				pStock->SetCol(col);                    // 残機の色
				pStock->SetWidth(fWidth);               // 残機の数字ポリゴン１つ分の幅
				pStock->SetHeight(fHeight);             // 残機の数字ポリゴン１つ分の高さ
				pStock->SetStock(nStock);               // 残機

				if (FAILED(pStock->Init()))
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

	return pStock;
}

//=============================================================================
//    初期化処理
//=============================================================================
HRESULT CStock::Init(void)
{
	// 各種値の初期化
	D3DXVECTOR3 NumberPos = m_Pos;   // 座標
	D3DXCOLOR NumberCol = m_Col;     // 色
	float fNumberWidth = m_fWidth;   // 幅
	float fNumberHeight = m_fHeight; // 高さ
	int nNumber = 0;                 // 表示する数字の計算用

	if (m_pFrame == NULL)
	{// メモリが確保できる状態である
		m_pFrame = CScene2D::Create(m_Pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), m_fWidth * 2.5f, m_fHeight * 1.5f, 0.0f, 5);
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

	// 残機の桁数を計算し、桁数分メモリを確保
	m_nDigit = (int)log10f((float)m_nStock) + 1;   // 桁数を求める
	if (m_nDigit <= 0) { m_nDigit = 1; }	              // 0以下のとき1にする

	if (m_apNumber == NULL)
	{// メモリが確保できる状態である
		m_apNumber = new CNumber*[m_nDigit];  // 桁数分メモリ確保
		if (m_apNumber != NULL)
		{// メモリを確保できた
			for (int nCntStock = 0; nCntStock < m_nDigit; nCntStock++)
			{// スコアの桁数分繰り返し
				m_apNumber[nCntStock] = NULL;   // いったんメモリを空にする
				if (m_apNumber[nCntStock] == NULL)
				{// メモリが確保できる状態にある
				 // 表示する数字の計算
					nNumber = m_nStock % ((int)powf(10.0f, (float)nCntStock) * 10) / (int)powf(10.0f, (float)nCntStock);

					// 数字ポリゴンのインスタンス生成処理
					m_apNumber[nCntStock] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, CNumber::STATE_NONE, nNumber, 3, GetPriority());

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
void CStock::Uninit(void)
{
	// 数字ポリゴンの開放処理
	if (m_apNumber != NULL)
	{// メモリが確保されている
		for (int nCntStock = 0; nCntStock < m_nDigit; nCntStock++)
		{// スコアの桁数分繰り返し
			if (m_apNumber[nCntStock] != NULL)
			{// メモリが確保されている
			 // 数字ポリゴンの終了処理
				m_apNumber[nCntStock]->Uninit();
				m_apNumber[nCntStock] = NULL;
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

	// 残機の消去処理
	Release();
}

//=============================================================================
//    更新処理
//=============================================================================
void CStock::Update(void)
{
	if(m_nStock <= 0)
	{// 残機がなくなった
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// ゲームモード中だったら
			CGame *pGame = CManager::GetGame();
			if (pGame->GetState() != CGame::STATE_END)
			{// ゲーム終了状態でない
				pGame->SetState(CGame::STATE_END);
				//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_FINISH);
			}
		}
	}
}

//=============================================================================
//    描画処理
//=============================================================================
void CStock::Draw(void)
{

}

//=============================================================================
//    数字テクスチャを設定する処理
//=============================================================================
void CStock::SetStockTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
	if (m_pTexture != NULL)
	{// テクスチャへのポインタが取得できた
		if (m_apNumber != NULL)
		{// 数字ポリゴンが生成されている
			for (int nCntStock = 0; nCntStock < m_nDigit; nCntStock++)
			{// 残機の桁数分繰り返し
				if (m_apNumber[nCntStock] != NULL)
				{// 数字ポリゴンが生成されている
					m_apNumber[nCntStock]->BindTexture(m_pTexture);
				}
			}
		}
	}
}

//=============================================================================
//    残機の座標を設定する処理
//=============================================================================
void CStock::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    残機の色を設定する処理
//=============================================================================
void CStock::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
//    残機の数字ポリゴン１つ分の幅を設定する処理
//=============================================================================
void CStock::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    残機の数字ポリゴン１つ分の高さを設定する処理
//=============================================================================
void CStock::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    残機を設定する処理
//=============================================================================
void CStock::SetStock(const int nStock)
{
	m_nStock = nStock;
	if (m_apNumber != NULL)
	{// 数字ポリゴンが生成されている
		for (int nCntStock = 0; nCntStock < m_nDigit; nCntStock++)
		{// 残機の桁数分繰り返し
			m_apNumber[nCntStock]->SetNumber(nStock);
		}
	}
}

//=============================================================================
//    残機の座標を取得する処理
//=============================================================================
D3DXVECTOR3 CStock::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    残機の色を取得する処理
//=============================================================================
D3DXCOLOR CStock::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    残機の数字ポリゴン１つ分の幅を取得する処理
//=============================================================================
float CStock::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    残機の数字ポリゴン１つ分の高さを取得する処理
//=============================================================================
float CStock::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    残機を取得する処理
//=============================================================================
int CStock::GetStock(void)
{
	return m_nStock;
}
