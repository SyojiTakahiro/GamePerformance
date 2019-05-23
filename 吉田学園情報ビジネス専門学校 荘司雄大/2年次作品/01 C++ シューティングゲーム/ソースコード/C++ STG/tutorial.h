//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// オブジェクトクラス
//*****************************************************************************
class CTutorial
{
public:
	CTutorial();
	~CTutorial();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTutorial *Create(void);
	static CPlayer *GetPlayer(void);
	static CGauge *GetGauge(void);
	static CGauge2 *GetGauge2(void);
protected:

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;				//共有テクスチャへのポインタ
	CScene2D *m_pCScene2D;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	static CPlayer *m_pPlayer;
	static CGauge *m_pGauge;
	static CGauge2 *m_pGauge2;
};

#endif