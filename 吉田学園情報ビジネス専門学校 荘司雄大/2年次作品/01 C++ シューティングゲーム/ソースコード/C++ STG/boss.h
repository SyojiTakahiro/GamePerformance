//=============================================================================
//
// ボスエネミー処理 [boss.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _BOSS_H_
#define _BOSS_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// 2Dポリゴンクラス
//*****************************************************************************
class CBoss : public CScene2D
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		BOSSTYPE_1,
		BOSSTYPE_2,
		BOSSTYPE_3,
		BOSSTYPE_BOSS,
		BOSSTYPE_TUTORIAL,
		BULLETTYPE_MAX
	}BOSSTYPE;

	CBoss();
	~CBoss();
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの開放

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBoss *Create(D3DXVECTOR3 pos, float size, int nLife, BOSSTYPE enemyType);
	void SetBossLife(int nLife);
	void CutBossLife(int nDamage);
	int GetBossLife(void);

	static int m_nLife;

private:
	static LPDIRECT3DTEXTURE9	m_apTexture[3];				//共有テクスチャへのポインタ

	float					m_fBossSpin;
	float					m_fBossScale;
	float					m_fBossLength;

	BOSSTYPE m_BossType;

	int m_BulletRecast1;
	int m_BulletRecast2;
	int m_BulletRecast3;
};

#endif