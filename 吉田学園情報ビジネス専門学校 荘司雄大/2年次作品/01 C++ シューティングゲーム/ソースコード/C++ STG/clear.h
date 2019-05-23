//=============================================================================
//
// クリア処理 [Clear.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _CLEAR_H_
#define _CLEAR_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// オブジェクトクラス
//*****************************************************************************
class CNumber;

class CClear
{
public:
	CClear();
	~CClear();
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの開放

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CClear *Create(void);

	static void SetClearScore(int nScore);
protected:

private:
	CNumber *m_apNumber[MAX_NUMBER];
	static int m_nScore;

	static LPDIRECT3DTEXTURE9	m_pTexture;				//共有テクスチャへのポインタ
	CScene2D *m_pCScene2D;

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	D3DXVECTOR3 m_pos;
};

#endif