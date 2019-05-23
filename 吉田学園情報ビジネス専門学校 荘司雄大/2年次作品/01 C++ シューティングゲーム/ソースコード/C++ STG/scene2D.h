//=============================================================================
//
// シーン2D処理 [scene2D.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// 2Dポリゴンクラス
//*****************************************************************************
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority);
	~CScene2D();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScene2D *Create(D3DXVECTOR3 pos,float size);
	void SetPosition(D3DXVECTOR3 pos);
	void SetColor(D3DXCOLOR col);
	void SetExplosion(int nPatternAnim);
	void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius);
	void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius);
	void SetPlayer(int nPatternAnim);
	void SetBulletEX(int nPatternAnim);
	void SetBulletSP(int nPatternAnim);
	D3DXVECTOR3 *GetPosition(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	LPDIRECT3DVERTEXBUFFER9 GetVtx(void);

	void SetSize(float size);
	float GetSize(void);

protected:
	D3DXVECTOR3				m_posPolygon;			// ポリゴンの位置
private:
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// 頂点バッファへのポインタ

	D3DXVECTOR3				m_move;
	float					m_fSpin;
	float					m_fSize;
	float					m_fScale;
	float					m_fLength;
	float					m_fRadius;
};

#endif