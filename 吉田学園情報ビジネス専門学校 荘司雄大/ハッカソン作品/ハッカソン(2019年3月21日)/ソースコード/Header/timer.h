//=============================================================================
//
// タイマーの処理 [timer.h]
// Author : Jukiya Hayakawa
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"
#include "2DPolygon.h"

#define TIMER_DIGIT (6)	//タイマーの桁数
#define TIMER_WIDTH (20.0f)	//タイマーの幅
#define TIMER_HEIGHT (40.0f)	//タイマーの高さ
//前方宣言
class CNumber;

//*****************************************************************************
// クラス定義
//*****************************************************************************
class CTimer : public CScene
{
public:
	//**********************************
	// タイマーの種類
	//**********************************
	typedef enum
	{
		TIMER_ONEMINUTES = 0,		//1分
		TIMER_TENSECOND,		//10秒
		TIMER_ONESECOND,		//1秒
		TIMER_TENMIRI,			//10ミリ
		TIMER_ONEMIRI,			//1秒
		TIMER_MAX,
	}TIMER_TYPE;

	//メンバ関数
	CTimer();
	~CTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	static void SetTimer(int nOneMinutes, int nTenSeconds, int nOneSeconds, int nTenMiri, int nOneMiri);
	static int GetOneMinute(void) { return m_nOneMinute; }		//1分
	static int GetTenSecond(void) { return m_nTenSecond; }		//10秒
	static int GetOneSecond(void) { return m_nOneSecond; }		//10秒
	static int GetTenMiri(void) { return m_nTenMiri; }			//ミリ
	static int GetOneMiri(void) { return m_nOneMiri; }			//ミリ

	//静的メンバ関数
	static CTimer *Create(D3DXVECTOR3 pos);

private:
	bool		m_bUse;			//使用しているかどうか
	int			m_nCntFrame;
	D3DXVECTOR3 m_pos;			//位置
	static int m_nTimer;		//タイマー
	static int m_nOneMinute;	//1分
	static int m_nTenSecond;	//10秒
	static int m_nOneSecond;	//10秒
	static int m_nTenMiri;		//10ミリ
	static int m_nOneMiri;		//1ミリ
	static CNumber *m_apNumber[TIMER_DIGIT];		//タイマーのポインタ
	LPDIRECT3DTEXTURE9	m_pTexture;					// テクスチャへのポインタ
protected:
};
#endif