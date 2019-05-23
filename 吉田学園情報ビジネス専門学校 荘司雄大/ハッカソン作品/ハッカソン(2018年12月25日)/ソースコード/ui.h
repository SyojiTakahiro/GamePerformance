//*****************************************************************************
//
//     UI管理の処理[ui.h]
//     Auther:Takuto Ishida
//
//*****************************************************************************
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define UI_TIMER_PRIORITY	(5)		// タイマーの処理優先順位
#define UI_SCORE_PRIORITY	(5)		// スコアの処理優先順位
#define UI_STOCK_PRIORITY	(5)		// 残機の処理優先順位
#define UI_PRESENT_PRIORITY	(5)		// プレゼントの処理優先順位

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CTimer;
class CScore;
class CStock;
class CPresent;
class CTextureManager;

//*****************************************************************************
//    チュートリアルクラスの定義
//*****************************************************************************
class CUI
{
public:     // 誰でもアクセス可能
	CUI();
	~CUI();

	static CUI *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	int GetTimer(void);			// タイマーを取得
	void SetTimer(int nTimer);	// タイマーの設定

	void ResetTimer(void);		// タイマーをリセット

	int GetScore(void);			// スコアを取得
	void SetScore(int nScore) ;	// スコアの設定

	int GetStock(void);			// 残機を取得
	void SetStock(int nScore);	// 残機の設定

	int GetPresent(void);			// プレゼントを取得
	void SetPresent(int nScore);	// プレゼントの設定

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	CTextureManager *m_pTextureManager;		// テクスチャ管理クラスのポインタ
	CTimer *m_pTimer;						// タイマークラスのポインタ
	CScore *m_pScore;						// スコアクラスのポインタ
	CStock *m_pStock;						// 残機クラスのポインタ
	CPresent *m_pPresent;					// プレゼントクラスのポインタ
};

#endif