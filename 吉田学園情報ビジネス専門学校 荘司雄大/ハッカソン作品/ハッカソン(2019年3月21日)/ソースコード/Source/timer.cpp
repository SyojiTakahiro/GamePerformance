//=============================================================================
//
// タイマーの処理 [timer.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "timer.h"
#include "manager.h"
#include "2DPolygon.h"
#include "mode.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CTimer::m_nTimer = 0;			//タイマー
int CTimer::m_nOneMinute = 0;		//1分
int CTimer::m_nTenSecond = 0;		//10秒
int CTimer::m_nOneSecond = 0;		//1秒
int CTimer::m_nTenMiri = 0;			//10ミリ
int CTimer::m_nOneMiri = 0;			//1ミリ
CNumber *CTimer::m_apNumber[TIMER_DIGIT] = {};	//タイマーのポインタ
//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// グローバル変数
//=============================================================================

//=============================================================================
// タイマーのコンストラクタ
//=============================================================================
CTimer::CTimer() :CScene(6, OBJTYPE_2DPOLYGON)
{
	m_pTexture = NULL;                         // テクスチャへのポインタ
}
//=============================================================================
// タイマーのデストラクタ
//=============================================================================
CTimer::~CTimer()
{

}

//=============================================================================
//タイマーの生成
//=============================================================================
CTimer *CTimer::Create(D3DXVECTOR3 pos)
{
	CTimer *pTimer = NULL;	//タイマーのポインタをNULLにする
	if (pTimer == NULL)
	{//タイマーがNULLの場合
		pTimer = new CTimer;		//シーンの動的確保
		pTimer->m_pos = pos;		//位置の代入
		pTimer->Init();				//初期化処理
	}
	return pTimer;					//値を返す
}

//=============================================================================
// タイマーの初期化処理
//=============================================================================
HRESULT CTimer::Init(void)
{
	//タイマーの初期化
	m_nOneMinute = 1;
	m_nTenSecond = 3;
	m_nOneSecond = 0;
	m_nTenMiri = 0;
	m_nOneMiri = 0;

	m_nCntFrame = 0;	//フレームカウンターの初期化
	m_bUse = true;		//使用している状態にする

	for (int nCntNumber = 0; nCntNumber < TIMER_DIGIT; nCntNumber++)
	{//桁の数だけ繰り返し
		switch (nCntNumber)
		{
		case CTimer::TIMER_ONEMINUTES:
			//1分
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 50.0f, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TIMER_WIDTH, TIMER_HEIGHT, 0.0f, CNumber::STATE_NONE, m_nOneMinute, 3, 6);
			break;

		case CTimer::TIMER_TENSECOND:
			//10秒
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 120.0f, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TIMER_WIDTH, TIMER_HEIGHT, 0.0f, CNumber::STATE_NONE, m_nTenSecond, 3, 6);
			break;
		case CTimer::TIMER_ONESECOND:
			//1秒
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 170.0f, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TIMER_WIDTH, TIMER_HEIGHT, 0.0f, CNumber::STATE_NONE, m_nOneSecond, 3, 6);
			break;
		case CTimer::TIMER_TENMIRI:
			//10ミリ
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 240.0f, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TIMER_WIDTH, TIMER_HEIGHT, 0.0f, CNumber::STATE_NONE, m_nTenMiri, 3, 6);
			break;
		case CTimer::TIMER_ONEMIRI:
			//1ミリ
			//数字の生成
			m_apNumber[nCntNumber] = CNumber::Create(D3DXVECTOR3(m_pos.x + 290.0f, m_pos.y, m_pos.z), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TIMER_WIDTH, TIMER_HEIGHT, 0.0f, CNumber::STATE_NONE, m_nOneMiri, 3, 6);
			break;
		}
	}

	return S_OK;
}

//=============================================================================
// タイマーの終了処理
//=============================================================================
void CTimer::Uninit(void)
{
	for (int nCntTimer = 0; nCntTimer < TIMER_DIGIT; nCntTimer++)
	{//桁の数だけ繰り返し
		if (m_apNumber[nCntTimer] != NULL)
		{
			m_apNumber[nCntTimer]->Uninit();
			m_apNumber[nCntTimer] = NULL;
		}
	}

	Release();
}

//=============================================================================
// タイマーの更新処理
//=============================================================================
void CTimer::Update(void)
{
	//フレームカウンターの加算
	m_nCntFrame--;


	if (m_bUse == true)
	{//使用していた場合
		m_nOneMiri--;	//1ミリを加算

		if (m_nOneMiri % 6 == 0)
		{//6フレームごとに10ミリ加算
			if (m_nTenMiri < 0 && m_nTenSecond < 0 && m_nTenSecond < 0 && m_nOneMinute < 0)
			{
				m_nOneMiri = 0;
			}
			else
			{
				m_nTenMiri--;
			}
		}

		if (m_nTenMiri < 0)
		{
			m_nTenMiri = 9;	//10ミリの初期化
			if (m_nTenSecond < 0 && m_nTenSecond < 0 && m_nOneMinute < 0)
			{
				m_nTenMiri = 0;
			}
			else
			{

			m_nOneSecond--;	//1秒の加算
			}
		}

		if (m_nOneSecond < 0)
		{
			m_nOneSecond = 9;	//1秒の初期化

				m_nTenSecond--;		//10秒の加算

		}

		if (m_nTenSecond < 0)
		{
			m_nTenSecond = 5;	//10秒の初期化
			if (m_nOneMinute > 0)
			{
				m_nOneMinute--;	//1分の加算
			}
		}

		if (m_nOneMinute < 0)
		{
			m_nOneMinute = 0;	//1分を初期化
		}

		if (m_nOneMinute <= 0 && m_nTenSecond <= 0 && m_nOneSecond <= 0 && m_nTenMiri <= 0)
		{//全てのタイマーが0以下になった場合
			m_bUse = false;		//使用していない状態にする
			m_nOneMinute = 0;	//0固定

			//スコアの設置処理

			// 画面遷移
			if (CManager::GetMode() == CManager::MODE_GAME)
			{
				if (CManager::GetGame()->GetState() != CGame::STATE_END)
				{
					CManager::GetGame()->SetState(CGame::STATE_END);
				}
			}
		}
	m_apNumber[0]->SetNumber(m_nOneMinute);	//1分の数字設置処理
	m_apNumber[1]->SetNumber(m_nTenSecond);	//10秒の数字設置処理
	m_apNumber[2]->SetNumber(m_nOneSecond);	//1秒の数字設置処理
	m_apNumber[3]->SetNumber(m_nTenMiri);	//10ミリの数字設置処理
	m_apNumber[4]->SetNumber(m_nOneMiri);	//1ミリの数字設置処理
	}
}

//=============================================================================
// タイマーの描画処理
//=============================================================================
void CTimer::Draw(void)
{
	for (int nCntTimer = 0; nCntTimer < TIMER_DIGIT; nCntTimer++)
	{//桁の数だけ繰り返し
		if (m_apNumber[nCntTimer] != NULL)
		{
			m_apNumber[nCntTimer]->Draw();
		}
	}

}

//=============================================================================
//    テクスチャ設定処理
//=============================================================================
void CTimer::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;

	for (int nCntTimer = 0; nCntTimer < TIMER_DIGIT; nCntTimer++)
	{
		if (m_apNumber[nCntTimer] != NULL)
		{
			m_apNumber[nCntTimer]->BindTexture(m_pTexture);
		}
	}
}
//=============================================================================
//   タイマーの設置処理
//=============================================================================
void CTimer::SetTimer(int nOneMinutes, int nTenSeconds, int nOneSeconds, int nTenMiri, int nOneMiri)
{
	//タイマーを代入
	m_nOneMinute = nOneMinutes;
	m_nTenSecond = nTenSeconds;
	m_nOneSecond = nOneSeconds;
	m_nTenMiri = nTenMiri;
	m_nOneMiri = nOneMiri;
}
