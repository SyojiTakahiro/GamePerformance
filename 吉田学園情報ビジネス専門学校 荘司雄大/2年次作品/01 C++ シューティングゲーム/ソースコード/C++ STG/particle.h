//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

#define MAX_PARTICLE (30)

//*****************************************************************************
// 2Dポリゴンクラス
//*****************************************************************************
class CParticle : public CScene2D
{
public:
	CParticle();
	~CParticle();
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの開放

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int life);
private:

	static LPDIRECT3DTEXTURE9	m_pTexture;				//共有テクスチャへのポインタ

	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_posParticle;
	D3DXCOLOR m_col;
	float m_fRadius;		//
	int	  m_nLife;
};

#endif