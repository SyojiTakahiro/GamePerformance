//=============================================================================
//
// タイトル処理 [title.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// オブジェクトクラス
//*****************************************************************************

class CTitle
{
public:
	CTitle();
	~CTitle();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTitle *Create(void);
protected:
	
private:
	static LPDIRECT3DTEXTURE9	m_pTexture;				//共有テクスチャへのポインタ
	CScene2D *m_pCScene2D;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	static CPause *m_pSelect;

	bool m_bSelect;
};

#endif