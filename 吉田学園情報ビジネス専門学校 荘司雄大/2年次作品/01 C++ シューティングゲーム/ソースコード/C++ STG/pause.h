//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

#define PAUSE_TEXTURE_MAX  (4)          // テクスチャを読み込む数

//*****************************************************************************
// 2Dポリゴンクラス
//*****************************************************************************
class CPause : public CScene
{
public:
	typedef enum
	{
		SELECT_CONTINUE = 0,
		SELECT_RETRY,
		SELECT_QUIT,
		SELECT_MAX
	}SELECT;

	typedef enum
	{
		SELECTTYPE_NONE = 0,	//選ばれていない状態
		SELECTTYPE_SELECT,		//選ばれている	状態
		SELECTTYPE_MAX				//状態の総数
	}SELECTTYPE;

	typedef struct
	{
		SELECTTYPE	select;		//セレクト
		D3DXVECTOR3 pos;		//位置
		D3DXCOLOR	col;		//カラー
	}PAUSESELECT;

	CPause();
	~CPause();
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの開放

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause *Create(void);
private:
	static LPDIRECT3DTEXTURE9	m_apTexture[PAUSE_TEXTURE_MAX];	//共有テクスチャへのポインタ

	CScene2D *m_pCScene2D;
	CScene2D *m_pPauseSelect[SELECT_MAX];           // ロゴのポリゴン

	PAUSESELECT				m_aPauseMenu[SELECT_MAX];					// ポーズメニュー
	SELECT					m_SelectMode;
	int						m_nSelect;
};

#endif