#//=============================================================================
//
// bar処理 [bar.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _BAR_H_
#define _BAR_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// 2Dポリゴンクラス
//*****************************************************************************
class CBar : public CScene
{
public:
	CBar();
	~CBar();
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの開放

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBar *Create(void);
private:
	static LPDIRECT3DTEXTURE9	m_pTexture;				//共有テクスチャへのポインタ
	CScene2D *m_pCScene2D;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	int m_nCntAnim;
};

#endif