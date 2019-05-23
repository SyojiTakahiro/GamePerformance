//=============================================================================
//
// プレイヤーの処理 [playerl.h]
// Author : Shun Yokomicho
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_SIN_COS_TOP	(0.75f)							//ポリゴン移動量
#define MOVE_SIN_COS_BOT	(0.25f)							//ポリゴン移動量
#define FIELD_MAX_SIZE		(FIELD_SIZE - 10)
#define PLAYER_PARTS		(10)							//プレイヤーパーツ数

typedef enum
{
	PLAYERSTATE_NORMAL = 0,		// 通常状態
	PLAYERSTATE_AWAKENING,		// Pizzaモード状態
	PLAYERSTATE_EAT,			// Pizzaが食べられる状態
	PLAYERSTATE_GAUGEZERO,		// Gaugeが0の場合
	PLAYERSTATE_DIE,			// プレイヤーが壁にぶつかる場合
	PLAYERSTATE_MAX,			// 状態の総数
}PLAYERSTATE;

typedef enum
{
	CHANGESTATE_POSSIBLE = 0,	// 変身可能状態
	CHANGESTATE_IMPOSSIBLE,		// 変身不可状態
	CHANGESTATE_MAX,			// 状態の総数
}CHANGESTATE;

typedef enum
{
	PLAYERPIZZA_HUMAN = 0,	// 人間状態
	PLAYERPIZZA_PIZZA,		// ピザ状態
	PLAYERPIZZA_MAX,		// 状態の総数
}PLAYERPIZZA;

typedef enum
{
	PALYERTYPE_STOP = 0,
	PALYERTYPE_WAIK,
	PALYERTYPE_MAX
}PALYERTYPE;

typedef struct
{
	D3DXVECTOR3  pos;			// 現在の位置
	D3DXVECTOR3  posOld;		// 過去の位置
	D3DXVECTOR3  move;			// 移動
	D3DXVECTOR3	 rot;			// 通常状態 && Pizza状態縦の向き(現在)
	D3DXVECTOR3	 rotPizza;		// Pizza状態横の向き(現在)
	D3DXVECTOR3	 rotDest;		// 向き(目的)
	D3DXVECTOR3	 rotPizzaDest;	// Pizza状態横の向き(目的)
	D3DXVECTOR3	 DiffAngle;		// 向き(プレイヤーの向きの差分) 
	D3DXVECTOR3  DiffAnglePizza;// 向き(Pizzaの向きの差分)
	D3DXVECTOR3  vtxMin;		// モデルの最小値
	D3DXVECTOR3  vtxMax;		// モデルの最大値
	D3DXMATRIX	 mtxWorld;		// ワールドマトリックス
	PLAYERSTATE  state;			// プレイヤーの状態
	PALYERTYPE	type;			// プレイヤーの種類
	LPD3DXMESH	 pMesh; 		//メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat;		// マテリアル情報へのポインタ
	DWORD		nNumMat;		// マテリアルの情報
	int		nIdyShadow;			// 影の番号
	int rotnum;					// プレイヤーの向きの記憶
	float fXaxis;				// X軸
	float fZaxis;				// Z軸
	bool bUse;					// 使われているかどうか
	CHANGESTATE ChangeState;	// 変身可能かどうか
	D3DXVECTOR3  vtxMinBike;	// バイク時の最小値
	D3DXVECTOR3  vtxMaxBike;	// バイク時の最大値
	PLAYERPIZZA PizzaMode;
}PLAYER;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);

#endif