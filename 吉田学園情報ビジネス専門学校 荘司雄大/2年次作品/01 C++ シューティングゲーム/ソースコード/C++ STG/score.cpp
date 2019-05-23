//=============================================================================
//
// スコア処理 [Score.cpp]
// Author : Takahiro Syoji
//
//=============================================================================
#include "number.h"
#include "score.h"
#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define SCORE_POS (D3DXVECTOR3(1100, 700, 0.0f))

//=============================================================================
// コンストラクタ
//=============================================================================
CScore::CScore()
{
	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		m_apNumber[nCntScore] = NULL;
	}

	m_nScore = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CScore::Init(void)
{
	int nNumber;

	SetObjType(OBJTYPE_SCORE);
	D3DXVECTOR3 pos;
	pos = m_pos;

	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		m_apNumber[nCntScore] = CNumber::Create(pos);

		// 数字の計算
		nNumber = m_nScore % (int)powf(10.0f, (float)(MAX_NUMBER - nCntScore)) * 10 / (int)powf(10.0f, (float)(MAX_NUMBER - nCntScore));

		m_apNumber[nCntScore]->SetNumber(nNumber);

		pos.x += 30;
	}

	m_nScore = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();

			delete m_apNumber[nCntScore];
			m_apNumber[nCntScore] = NULL;
		}
	}
	//リリース
	Release();
}
//=============================================================================
// 更新処理
//=============================================================================
void CScore::Update(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		m_apNumber[nCntScore]->Draw();
	}
}

//=============================================================================
// クリエイト
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos)
{
	CScore *pScore = NULL;
	if (pScore == NULL)
	{
		// 動的確保
		pScore = new CScore;

		if (pScore != NULL)
		{
			pScore->m_pos = pos;

			// 初期化処理
			pScore->Init();
		}
	}
	return pScore;
}

//=============================================================================
// セットスコア
//=============================================================================
void CScore::AddScore(int nScore)
{
	int nNumber;

	m_nScore += nScore;

	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		// 数字の計算
		nNumber = m_nScore % (int)powf(10.0f, (float)(MAX_NUMBER - nCntScore)) * 10 / (int)powf(10.0f, (float)(MAX_NUMBER - nCntScore));

		m_apNumber[nCntScore]->SetNumber(nNumber);
	}
}

int CScore::GetScore(void)
{
	return m_nScore;
}