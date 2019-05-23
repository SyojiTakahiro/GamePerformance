//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// 2Dポリゴンクラス
//*****************************************************************************
class CEffect : public CScene2D
{
public:
	CEffect();
	~CEffect();
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの開放

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEffect *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius,int life);
private:

	static LPDIRECT3DTEXTURE9	m_pTexture;				//共有テクスチャへのポインタ

	int	  m_nLife;			//
	D3DXVECTOR3 m_posEffect;
	float m_fRadius;			//
	D3DXCOLOR m_col;
};

#endif