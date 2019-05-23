//=============================================================================
//
// 爆発処理 [explosion.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// 2Dポリゴンクラス
//*****************************************************************************
class CExplosion : public CScene2D
{
public:
	CExplosion();
	~CExplosion();
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの開放

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CExplosion *Create(D3DXVECTOR3 pos,float size);
private:

	static LPDIRECT3DTEXTURE9	m_pTexture;				//共有テクスチャへのポインタ

	int m_nCounterAnim;			//カウンター
	int m_nPatternAnim;			//パターンNo.
};

#endif