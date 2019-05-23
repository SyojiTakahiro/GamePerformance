//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// カメラの構造体
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;				//視点
	D3DXVECTOR3 posR;				//注視点
	D3DXVECTOR3 posVDest;				//視点
	D3DXVECTOR3 posRDest;				//注視
	D3DXVECTOR3	vecU;				//上方向ベクトル
	D3DXVECTOR3	rot;
	D3DXMATRIX mtxProjection;
	D3DXMATRIX mtxView;
	D3DVIEWPORT9 ViewPort;
	int nCounterAngle;
	float fLength;
}CAMERA;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
CAMERA *GetCamera(void);

#endif
