//=============================================================================
//
// ポックル処理 [pokuru.h]
// Author : Meguro Mikiya
//
//=============================================================================
#ifndef _POKURU_H_
#define _POKURU_H_

#include "main.h"
#include "polygon.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define POKURU_NAME		 "data\\MODEL\\ポックル\\karada.x"		//テクスチャのファイル名
#define POKURU_NAME1	 "data\\MODEL\\ポックル\\kao.x"		//テクスチャのファイル名
#define POKURU_NAME2	 "data\\MODEL\\ポックル\\migiude.x"		//テクスチャのファイル名
#define POKURU_NAME3	 "data\\MODEL\\ポックル\\migite.x"		//テクスチャのファイル名
#define POKURU_NAME4	 "data\\MODEL\\ポックル\\hidariude.x"		//テクスチャのファイル名
#define POKURU_NAME5	 "data\\MODEL\\ポックル\\hidarite.x"		//テクスチャのファイル名
#define POKURU_NAME6	 "data\\MODEL\\ポックル\\migimomo.x"		//テクスチャのファイル名
#define POKURU_NAME7	 "data\\MODEL\\ポックル\\migiashi.x"		//テクスチャのファイル名
#define POKURU_NAME8	 "data\\MODEL\\ポックル\\hidarimomo.x"		//テクスチャのファイル名
#define POKURU_NAME9	 "data\\MODEL\\ポックル\\hidariashi.x"		//テクスチャのファイル名
#define POKURU_NAME10	 "data\\MODEL\\ポックル\\99_hat.x"		//テクスチャのファイル名
#define POKURU_NAME11	 "data\\MODEL\\矢印.x"			//テクスチャのファイル名

#define MOVE_POKURU			(0.3f)							//ポックル移動量
#define MOVE_SIN_COS_TOP	(0.75f)							//ポリゴン移動量
#define MOVE_SIN_COS_BOT	(0.25f)							//ポリゴン移動量
#define FIELD_MAX_SIZE		(FIELD_SIZE - 10)
#define POKURU_PARTS		(10)							//ポックルパーツ数
//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef enum
{
	POKURUSTATE_APPEAR = 0,	//無敵
	POKURUSTATE_NORMAL,		//通常状態
	POKURUSTATE_STOP,		//通常状態
	POKURUSTATE_MOVE,		//通常状態
	POKURUSTATE_DAMAGE,		//ダメージ状態
	POKURUSTATE_DEATH,		//死亡
	POKURUSTATE_MAX			//状態の総数
}POKURUSTATE;				//列挙型

typedef struct
{
	LPD3DXMESH pMesh;			//メッシュ情報へのポインタ
	LPD3DXBUFFER pBuffMat;		//マテリアル情報へのポインタ
	DWORD nNumMat;				//マテリアル情報の数
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//向き
	int nIdxModelParent;		//親モデルのインデックス
}Model;

typedef struct
{
	DWORD nNumMatPokuru = 0;			//マテリアル情報の数
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 posold;					//前回の位置
	D3DXVECTOR3 move;					//移動量
	D3DXVECTOR3 rot;					//向き
	D3DXMATRIX	mtxWorld;				//ワールドマトリックス
	float fShadow;						//影の大きさ
	int nIdxShadow;						//影のインデックス
	float fDestAngle;					//ポックルの角度
	float fDiffAngle;					//差分
	float fWidth;
	float fHeight;
	POKURUSTATE state;					//ポックルの状態
	D3DXVECTOR3 VtxMin, VtxMax;			//ポックル最小値、最大値
	float fLength;
	Model aModel[POKURU_PARTS];			//モデルのパーツ数
} POKURU;


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPokuru(void);
void UninitPokuru(void);
void UpdatePokuru(void);
void DrawPokuru(void);
POKURU *GetPokuru(void);

#endif
