//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define  MAX_MODEL   (20)       // プレイヤーモデルの総数
#define  MAX_MOTION  (20)       // モーションの総数
#define  MAX_PLAYER  (2)        // プレイヤーの人数

//*****************************************************************************
// プレイヤーの構造体
//*****************************************************************************
typedef enum
{
	PLAYERSTATUS_MOVEMENT = 0,  // 移動力
	PLAYERSTATUS_JUMPPOWER,     // ジャンプ力
	PLAYERSTATUS_MAX
}PLAYERSTATUS;

typedef enum
{
	PARTSSET_INDEX = 0,   // パーツ番号
	PARTSSET_PARENT,      // 親モデルの番号
	PARTSSET_OFFSET_POS,  // 親モデルからの距離
	PARTSSET_OFFSET_ROT,  // 親モデルからの向き
	PARTSSET_MAX
}PARTSSET;


typedef enum
{
	WEAK_ATTACK_NUMBER_0 = 0,
	WEAK_ATTACK_NUMBER_1,
	WEAK_ATTACK_NUMBER_2,
	WEAK_ATTACK_NUMBER_MAX
}WEAK_ATTACK_NUMBER;

typedef enum
{
	PLAYERSTATE_NORMAL = 0,    // 通常の状態
	PLAYERSTATE_MOVE,          // 移動している状態
	PLAYERSTATE_WEAK_ATTACK_0, // 弱攻撃1発目状態
	PLAYERSTATE_WEAK_ATTACK_1, // 弱攻撃2発目状態
	PLAYERSTATE_WEAK_ATTACK_2, // 弱攻撃3発目状態
	PLAYERSTATE_STORONG_ATTACK,// 強攻撃状態
	PLAYERSTATE_DAMAGE,        // ダメージ状態
	PLAYERSTATE_DAMAGE_FLY,    // ぶっ飛び状態
	PLAYERSTATE_DAMAGE_DOWN,   // ダウン状態
	PLAYERSTATE_DAMAGE_GETUP,  // 起き上がり状態
	PLAYERSTATE_JUMP,          // ジャンプしている状態
	PLAYERSTATE_ULTIMATE_START,// 必殺技を開始している状態
	PLAYERSTATE_ULTIMATE,      // 必殺技をしている状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef enum
{
	MOTIONSTATE_NORMAL = 0,  // 通常の状態
	MOTIONSTATE_STOP,        // モーションが終わった状態
	MOTIONSTATE_SWITCH,      // モーション切り替え状態
	MOTIONSTATE_MAX
}MOTIONSTATE;

typedef struct
{
	LPD3DXMESH   pMesh = NULL;            // メッシュ情報へのポインタ
	LPDIRECT3DTEXTURE9* pTexture = NULL;  // テクスチャのポインタ
	LPD3DXBUFFER pBuffMat = NULL;         // マテリアル情報へのポインタ
	DWORD        nNumMat = NULL;          // マテリアル情報の数
	D3DXMATRIX   mtxWorld;                // ワールドマトリックス
	D3DXVECTOR3  vtxMin, vtxMax;          // プレイヤーの最小値, 最大値
	D3DXVECTOR3  pos;                     // 現在の位置
	D3DXVECTOR3  posStd;                  // 基準の位置
	D3DXVECTOR3  posDiff;                 // 差分の位置
	D3DXVECTOR3  DiffAngle;               // 差分の向き
	D3DXVECTOR3  rot;                     // 現在の向き
	int nIdxModelParent;                  // 親モデルのインデックス
}Model_Player;

typedef struct
{
	D3DXVECTOR3  posAdd[MAX_MODEL];       // 基準の位置に加える値
	D3DXVECTOR3  DestAngle[MAX_MODEL];    // 目的の向き
	int nPlayBackKey;                     // 再生フレーム数
}KeyFrame_Player;

typedef struct
{
	KeyFrame_Player Key[MAX_MODEL];      // キーフレームの情報
	bool bLoop;                          // ループするかしないか
	int nNumKey;                         // キーフレーム数
}Motion_Player;

typedef struct
{
	D3DXVECTOR3  pos;                       // 現在の位置
	D3DXVECTOR3  posold;                    // 前回の位置
	D3DXVECTOR3  move;                      // 移動量
	D3DXVECTOR3  rot;                       // 現在の向き
	D3DXVECTOR3 DiffAngle;                  // 差分
	D3DXVECTOR3 DestAngle;                  // 目的の向き
	D3DXMATRIX   mtxWorld;                  // ワールドマトリックス
	PLAYERSTATE state;                      // 状態
	D3DXVECTOR3 radius;                     // 当たり判定を取る距離
	float fMovement;                        // 移動力
	float fJumpPower;                       // ジャンプ力
	int nIdxShadow;                         // 使用している影の番号
	D3DXCOLOR ShadowCol;                    // 影の色
	float ShadowWidth;                      // 影の幅
	float ShadowDepth;                      // 影の奥行
	bool bJump;                             // ジャンプできるかできないか
	bool bWeakAction;                       // 弱攻撃してるかしていないか
	bool bStorongAction;                    // 強攻撃してるかしていないか
	bool bUltimate;                         // 必殺攻撃しているかどうか
	bool bHit;                              // 攻撃がヒットしたかどうか
	bool bDown;                             // ダウンしているかどうか
	bool bControll;                         // 操作できるかどうか
	int nStateMentCounter;                  // 状態を管理するカウンター
	int wAttackNumber;                      // 弱攻撃の番号
	int nLife;                              // 体力
	Model_Player aModel[MAX_MODEL];         // モデルの総数
	Motion_Player aMotion[PLAYERSTATE_MAX]; // モーションの情報
	MOTIONSTATE mState;                     // モーションの状態
	int nKey;                               // 現在のキーフレーム
	int nMotionCounter;                     // モーションカウンター
	int nBlendCounter;                      // モーションブレンドカウンター
}Player;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(int nCntPlayer);

#endif