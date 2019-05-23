//=============================================================================
//
// カメラ処理 [camera.h]
// Author : Hodaka Niwa
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// カメラの構造体
//*****************************************************************************
typedef enum
{
	CAMERASTATE_NORMAL = 0, // 通常の状態
	CAMERASTATE_MOVE,       // 移動している状態
	CAMERASTATE_SPIN,       // 旋回している状態
	CAMERASTATE_MAX
}CAMERASTATE;
typedef struct
{
	D3DXVECTOR3 posV;             // 現在の視点
  	D3DXVECTOR3 posR;             // 現在の注視点
	D3DXVECTOR3 posVDest;         // 目的の視点
	D3DXVECTOR3 posRDest;         // 目的の注視点
	D3DXVECTOR3 posVAdd;          // 目的の座標
	D3DXVECTOR3 posRAdd;          // 目的の座標
	D3DXVECTOR3 vecU;             // 上方向ベクトル
	D3DXMATRIX  mtxProjection;    // プロジェクションマトリックス
	D3DXMATRIX  mtxView;          // ビューマトリックス
	D3DXVECTOR3 rot;              // 現在の向き
	D3DXVECTOR3 rotDest;          // 目的の向き
	CAMERASTATE state;            // 状態
	float       rotDiff;          // 向きの差分
	float       fLength;          // 距離
	float       fWaraparoundSpeed;// 回り込み速度
	int         nWaraparoundTime; // 回り込み待ち時間
	int         nCounterTime;     // 待ち時間カウンター
}Camera;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
Camera *GetCamera(void);

#endif
