//=============================================================================
//
// 必殺弾処理 [bulletSP.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _BULLETSP_H_
#define _BULLETSP_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// 2Dポリゴンクラス
//*****************************************************************************
class CBulletSP : public CScene2D
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		BULLETTYPE_PLAYER,
		BULLETTYPE_MAX
	}BULLETTYPE;

	CBulletSP();
	~CBulletSP();
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの開放

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBulletSP *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size,int life ,BULLETTYPE bulletType);
private:

	static LPDIRECT3DTEXTURE9	m_pTexture;				//共有テクスチャへのポインタ
	D3DXVECTOR3					m_move;					//移動量

	int m_nBulletLife;
	int m_DamageRecast;
	BULLETTYPE m_bulletType;

	/*float					m_fBulletSpin;
	float					m_fBulletScale;
	float					m_fBulletLength;*/
	int m_HitCounter;
	int m_nCounterAnim;			//カウンター
	int m_nPatternAnim;			//パターンNo.
};

#endif