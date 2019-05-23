//=============================================================================
//
// 壁の処理 [bullet.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	BULLETTYPE_PLAYER = 0,	//プレイヤー状態
	BULLETTYPE_ENEMY,		//エネミー状態
	BULLETTYPE_BOSS,		//ボス状態
	BULLETTYPE_MAX			//状態の総数
}BULLETTYPE;

typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 move;					// 移動量
	D3DXVECTOR3 posold;					//前回の位置
	D3DXCOLOR col;						// 色
	D3DXMATRIX	mtrxWorldBullet;		// ワールドマトリックス
	float fRadius;						// 半径(大きさ)
	int nIdxShadow;						// 影のインデックス
	float fHeght;						// 高さ
	float fWidth;						// 幅
	BULLETTYPE type;					// 弾の種類
	int nLife;							// 寿命
	bool bUse;							// 使用しているかどうか
	bool bShadowUse;					// 使用しているかどうか
	D3DXVECTOR3 VtxMin, VtxMax;			//プレイヤーの最小値、最大値

} BULLET;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeght, float fWidth);

#endif
