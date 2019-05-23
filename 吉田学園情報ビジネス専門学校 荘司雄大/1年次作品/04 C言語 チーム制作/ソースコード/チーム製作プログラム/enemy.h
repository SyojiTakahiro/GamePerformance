//=============================================================================
//
// 敵の処理 [enemy.h]
// Author : Meguro Mikiya
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "polygon.h"
#include "player.h"
#include "model.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENEMY_NAME000		 "data\\MODEL\\車\\arisuta.x"		//テクスチャのファイル名
#define ENEMY_NAME001		 "data\\MODEL\\車\\arisuta.x"				//テクスチャのファイル名
#define ENEMY_NAME002		 "data\\MODEL\\車\\arisuta.x"		//テクスチャのファイル名

#define MOVE_ENEMY			(0.2f)							//敵の移動量
#define MOVE_SIN_COS_TOP	(0.75f)							//ポリゴン移動量
#define MOVE_SIN_COS_BOT	(0.25f)							//ポリゴン移動量
#define FIELD_MAX_SIZE		(FIELD_SIZE - 10)
#define MAX_ENEMY			(300)							//敵の配置の最大数
#define MAX_ENEMY_TYPE		(3)								//敵の種類の最大数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
//typedef struct
//{
//	D3DXVECTOR3 VtxMinEnemy, VtxMaxEnemy;	//モデルの最小値、最大値
//} VTX;
typedef struct
{
	DWORD nNumMat = 0;						//マテリアル情報の数
	D3DXVECTOR3 pos;						//位置
	D3DXVECTOR3 posold;						//前回の位置
	D3DXVECTOR3 move;						//移動量
	D3DXVECTOR3 rot;						//向き
	float fDestAngle;						//プレイヤーの角度
	float fDiffAngle;						//差分
	D3DXMATRIX	mtxWorld;					//ワールドマトリックス
	float fShadow;							//影の大きさ
	int nIdxShadow;							//影のインデックス
	int nType;								//モデルの種類
	bool bUse;								//使用しているか
	D3DXVECTOR3 VtxMinEnemy, VtxMaxEnemy;	//モデルの最小値、最大値
} ENEMY;

//typedef enum
//{
//	COLLISION_PLAYER = 0,	//プレイヤー
//	COLLISION_BULLET,		//弾
//} COLLISIONENEMYTYPE;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
ENEMY *GetEnemy(void);
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin);
void SetEnemy(D3DXVECTOR3 pos,int nType);
#endif