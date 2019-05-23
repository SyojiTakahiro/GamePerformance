//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// 2Dポリゴンクラス
//*****************************************************************************
class CPlayer : public CScene2D
{
public:
	CPlayer();
	~CPlayer();
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの開放

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayer *Create(D3DXVECTOR3 pos,float size);

private:
	static LPDIRECT3DTEXTURE9	m_pTexture;				//共有テクスチャへのポインタ

	float					m_fPlayerSpin;
	float					m_fPlayerScale;
	float					m_fPlayerLength;
	int						m_BulletRecast;
	int						m_SlashRecast;
	int m_nCounterAnim;			//カウンター
	int m_nPatternAnim;			//パターンNo.

};

#endif