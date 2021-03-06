#//=============================================================================
//
// ゲームオーバー背景処理 [bgGameover.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _BGGAMEOVER_H_
#define _BGGAMEOVER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// 2Dポリゴンクラス
//*****************************************************************************
class CBgGameover : public CScene
{
public:
	CBgGameover();
	~CBgGameover();
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの開放

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBgGameover *Create(void);
private:
	static LPDIRECT3DTEXTURE9	m_apTexture[MAX_BG];				//共有テクスチャへのポインタ
	CScene2D *m_apCScene2D[MAX_SCENE];

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	int m_nCntAnim;
};

#endif