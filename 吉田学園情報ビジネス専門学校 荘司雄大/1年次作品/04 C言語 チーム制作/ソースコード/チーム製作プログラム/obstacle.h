//=============================================================================
//
// 障害物の処理 [obstacle.h]
// Author : 荘司雄大
//
//=============================================================================
#ifndef _OBSTACLE_H_
#define _OBSTACLE_H_

#include "main.h"

//*****************************************************************************
// モデルの構造体
//*****************************************************************************
typedef enum
{
	OBSTACLE_TYPE_PIZZA = 0,			// ピザ屋
	OBSTACLE_TYPE_PIZZA_YOKO,			// ピザ屋 横
	OBSTACLE_TYPE_HOUSE000,				// 家0
	OBSTACLE_TYPE_HOUSE000_YOKO,		// 家0 横
	OBSTACLE_TYPE_HOUSE001,				// 家1
	OBSTACLE_TYPE_HOUSE001_YOKO,		// 家1 横
	OBSTACLE_TYPE_HOUSE002,				// 家2
	OBSTACLE_TYPE_HOUSE003,				// 家3 tatemono001.x
	OBSTACLE_TYPE_HOUSE003_YOKO,		// 家3 横
	OBSTACLE_TYPE_HOUSE004,				// 家4 
	OBSTACLE_TYPE_HOUSE004_YOKO,		// 家4 横
	OBSTACLE_TYPE_HOUSE005,				// 家5
	OBSTACLE_TYPE_HOUSE005_YOKO,		// 家5 横
	OBSTACLE_TYPE_HOUSE006,				// 家6
	OBSTACLE_TYPE_HOUSE006_YOKO,		// 家6 横
	OBSTACLE_TYPE_HOUSE007,				// 家7
	OBSTACLE_TYPE_HOUSE007_YOKO,		// 家7 横
	OBSTACLE_TYPE_HOUSE008,				// 家8
	OBSTACLE_TYPE_HOUSE008_YOKO,		// 家8 横
	OBSTACLE_TYPE_HOUSE009,				// 家9
	OBSTACLE_TYPE_HOUSE009_YOKO,		// 家9 横
	OBSTACLE_TYPE_HAIKYO,				// 廃墟
	OBSTACLE_TYPE_POKKURUZOU,			// ポックル像
	OBSTACLE_TYPE_HOSPITAL,				// 病院
	OBSTACLE_TYPE_HOSPITAL_YOKO,		// 病院 横
	OBSTACLE_TYPE_TEMPLE,				// 神殿
	OBSTACLE_TYPE_TEMPLE_YOKO,			// 神殿 横
	OBSTACLE_TYPE_SKYTREE,				// スカイツリー
	OBSTACLE_TYPE_PIZZASYATOU,			// ピザの斜塔
	OBSTACLE_TYPE_OFFICE000,			// オフィス0
	OBSTACLE_TYPE_OFFICE001,			// オフィス1
	OBSTACLE_TYPE_OFFICE001_YOKO,		// オフィス1 横
	OBSTACLE_TYPE_OFFICE002,			// オフィス2
	OBSTACLE_TYPE_OFFICE002_YOKO,		// オフィス2 横
	OBSTACLE_TYPE_KAMINARIMON,			// 雷門
	OBSTACLE_TYPE_KAMINARIMON_YOKO,		// 雷門 横
	OBSTACLE_TYPE_KOUZISYA,				// 工事車
	OBSTACLE_TYPE_KOUZIHYOUSIKI,		// 標識
	OBSTACLE_TYPE_KOUZIHYOUSIKI_YOKO,	// 標識
	OBSTACLE_TYPE_KOUZIIN,				// 工事員
	OBSTACLE_TYPE_KOUZIIN_YOKO,			// 工事員
	OBSTACLE_TYPE_BRIST,				// バリスタ
	OBSTACLE_TYPE_BRIST_YOKO,			// バリスタ横
	OBSTACLE_TYPE_HAKASEHOUSE,			// 博士
	OBSTACLE_TYPE_HAKASEHOUSE_YOKO,		// 博士横
	OBSTACLE_TYPE_HAKASE,				// 博士家
	OBSTACLE_TYPE_HAKASE_YOKO,			// 博士家横
	OBSTACLE_TYPE_USHI,					// 牛
	OBSTACLE_TYPE_SAKU,					// 柵
	OBSTACLE_TYPE_SAKU_YOKO,			// 柵横
	OBSTACLE_TYPE_TAMAGO,				// たまごっち
	OBSTACLE_TYPE_TAMAGO_YOKO,			// たまごっち横
	OBSTACLE_TYPE_BASTAR,				// バスターソード
	OBSTACLE_TYPE_BASTAR_YOKO,			// バスターソード横
	OBSTACLE_TYPE_FLOUR,				// 花
	OBSTACLE_TYPE_FLOUR_YOKO,			// 花
	OBSTACLE_TYPE_TENGA,				// TENGA
	OBSTACLE_TYPE_KANBAN,				// 看板
	OBSTACLE_TYPE_KAISYA,				// 会社
	OBSTACLE_TYPE_TUTI,					// 土
	OBSTACLE_TYPE_POKUTANK,				// ポックルタンク
	OBSTACLE_TYPE_POPUTANK,				// ポプタンク
	OBSTACLE_TYPE_MAX,
}OBSTACLE_TYPE;



typedef struct
{
	D3DXVECTOR3 pos;							// 位置
	D3DXVECTOR3 rot;							// 向き
	D3DXVECTOR3 move;							// 移動量
	int			nIdxShadow;						// 影
	OBSTACLE_TYPE	nType;						// 種類
	D3DXMATRIX	mtxWorld;						// ワールドマトリックス
	float fDestAngle;							// プレイヤーの角度
	float fDiffAngle;							// 差分
	int nLife;									// ライフ
	bool bUse;									// 使用しているかどうか
	float fLength;								// カメラとモデルとの距離
	float fLengthPlayer;						// プレイヤーとの距離
	D3DXVECTOR3 LengthMax;
	D3DXVECTOR3 LengthMin;
	D3DXVECTOR3	vtxMinObstacle, vtxMaxObstacle;	// モデルの最小値、最大値
	D3DXVECTOR3	vtxMinBike, vtxMaxBike;			// バイク時の当たり判定
}OBSTACLE;

typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 rot;        // 向き
	D3DXVECTOR3 move;		// 移動量
	OBSTACLE_TYPE nType;	// 種類
}OBSTACLE_INFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitObstacle(void);
void UninitObstacle(void);
void UpdateObstacle(void);
void DrawObstacle(void);
OBSTACLE *GetObstacle(void);
void SetObstacle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);
bool CollisionObstacle(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove,D3DXVECTOR3 radius);
bool CollisionObstacleBike(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 radius);

#endif
