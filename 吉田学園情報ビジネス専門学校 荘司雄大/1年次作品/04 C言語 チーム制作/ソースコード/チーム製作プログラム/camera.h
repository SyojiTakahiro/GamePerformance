//=============================================================================
//
// カメラ処理 [camera.h]
// Author :  Meguro Mikiya
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
	D3DXVECTOR3 posV;			//視点
	D3DXVECTOR3 posR;			//注視点
	D3DXVECTOR3 posU;			//上方向ベクトル
	D3DXVECTOR3 posVDest;
	D3DXVECTOR3 posRDest;
	D3DXVECTOR3 rotCamera;		//向き
	D3DXVECTOR3 rotDest;
	D3DXVECTOR3 DiffAngle;
	D3DXMATRIX mtxProjection;	//プロジェクションマトリックス(テレビ)
	D3DXMATRIX mtxView;			//ビューマトリックス(カメラマン)
	int nCounterAngle;			//カメラアングルが変わるカウンター
	D3DVIEWPORT9 Viewport;		//ビューポート
}Camera;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(int nIdxCamera);
Camera *GetCamera(void);
#endif
