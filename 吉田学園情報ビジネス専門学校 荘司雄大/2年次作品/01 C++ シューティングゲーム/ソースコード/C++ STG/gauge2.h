//=============================================================================
//
//	スキルゲージ処理 [gauge2.h]
// Author : Takahiro Syoji
//
//=============================================================================
#ifndef _GAUGE2_H_
#define _GAUGE2_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// 2Dポリゴンクラス
//*****************************************************************************
class CGauge2 : public CScene2D
{
public:
	CGauge2();
	~CGauge2();
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの開放

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGauge2 *Create(D3DXVECTOR3 pos);
	static float m_fGauge2;
private:
	static LPDIRECT3DTEXTURE9	m_apTexture[MAX_GAUGE];				//共有テクスチャへのポインタ
	CScene2D *m_apCScene2D[MAX_SCENE];

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
	int m_SERecast;

	int m_nCntAnim;
};

#endif
