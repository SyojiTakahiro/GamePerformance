//=============================================================================
//
// 敵処理 [enemy.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

#define MAX_ENEMY			(7)

//*****************************************************************************
// 2Dポリゴンクラス
//*****************************************************************************
class CEnemy : public CScene2D
{
public:
	typedef enum
	{
		OBJTYPE_NONE = 0,
		ENEMYTYPE_1,
		ENEMYTYPE_2,
		ENEMYTYPE_3,
		ENEMYTYPE_4,
		ENEMYTYPE_5,
		ENEMYTYPE_6,
		ENEMYTYPE_TUTORIAL,
		BULLETTYPE_MAX
	}ENEMYTYPE;

	CEnemy();
	~CEnemy();
	static HRESULT Load(void);	//テクスチャの読み込み
	static void Unload(void);	//テクスチャの開放

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CEnemy *Create(D3DXVECTOR3 pos,float size,int nLife,ENEMYTYPE enemyType);
	void SetLife(int nLife);
	void CutLife(int nDamage);
	int GetLife(void);

private:
	static LPDIRECT3DTEXTURE9	m_apTexture[MAX_ENEMY];				//共有テクスチャへのポインタ

	float					m_fEnemySpin;
	float					m_fEnemyScale;
	float					m_fEnemyLength;

	ENEMYTYPE m_enemyType;

	int m_BulletRecast;
	int m_nLife;
};

#endif