//=============================================================================
//
// 敵処理 [enemy.h]
// Author :Takahiro
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_ENEMY	(256)	// 敵の最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	float fWidth;
	float fHeight;
	int nType;
	bool bUse;
	D3DXCOLOR color;
	ENEMYSTATE state;	//敵の状態
	int nCounterState;	//状態管理のカウンター
	int nLife;			//体力
}ENEMY;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType);
void HitEnemy(int nCntEnemy, int nDmage);
bool CollisionEnemy(D3DXVECTOR3 *pePos, D3DXVECTOR3 *pePosOld, float fWidth, float fHeight);
ENEMY *GetEnemy(void);

#endif

