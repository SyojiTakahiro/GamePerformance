//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author :  Meguro Mikiya
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"
#include "main.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MOVE_CAMERA			(17.3f)						//モデル移動量
#define MOVE_SIN_COS_TOP_CAMERA	(0.75f)						//カメラ移動量
#define MOVE_SIN_COS_BOT_CAMERA	(0.25f)						//カメラ移動量
#define MAX_CAMERA (4)
//*****************************************************************************
// グローバル変数
//*****************************************************************************
Camera g_Camera[MAX_CAMERA];	//カメラの情報
float fLength;
//=============================================================================
// カメラの初期化処理
//=============================================================================
void InitCamera(void)
{
	//ビューポートの初期化
	g_Camera[0].Viewport.X = 0;
	g_Camera[0].Viewport.Y = 0;
	g_Camera[0].Viewport.Width = SCREEN_WIDTH;
	g_Camera[0].Viewport.Height = SCREEN_HEIGHT;
	g_Camera[0].Viewport.MinZ = 0.0f;
	g_Camera[0].Viewport.MaxZ = 1.0f;

	g_Camera[0].posV = D3DXVECTOR3(0.0f, 70.0f, -200.0f);
	g_Camera[0].posR = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
	g_Camera[0].posU = D3DXVECTOR3(0.0f, 0.1f, 0.0f);
	g_Camera[0].rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera[0].nCounterAngle = 0;
	fLength = sqrtf(
		//Xの距離を求める　Xの距離の2乗
		(g_Camera[0].posR.x - g_Camera[0].posV.x) * (g_Camera[0].posR.x - g_Camera[0].posV.x)
		//Zの距離を求める　Zの距離の2乗
		+ (g_Camera[0].posR.z - g_Camera[0].posV.z) * (g_Camera[0].posR.z - g_Camera[0].posV.z));

	//ビューポートの初期化
	g_Camera[1].Viewport.X = 0;
	g_Camera[1].Viewport.Y = 420;
	g_Camera[1].Viewport.Width = 300;
	g_Camera[1].Viewport.Height = SCREEN_HEIGHT;
	g_Camera[1].Viewport.MinZ = 0.0f;
	g_Camera[1].Viewport.MaxZ = 1.0f;

	g_Camera[1].posV = D3DXVECTOR3(0.0f, 6000.0f, 0.0f);
	g_Camera[1].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera[1].posU = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	g_Camera[1].rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera[1].nCounterAngle = 0;


	//現在のモードを取得
	MODE Mode = GetMode();
#if 0
	switch (Mode)
	{
	case MODE_GAME:
		//ビューポートの初期化
		g_Camera[1].Viewport.X = 0;
		g_Camera[1].Viewport.Y = 420;
		g_Camera[1].Viewport.Width = 300;
		g_Camera[1].Viewport.Height = SCREEN_HEIGHT;
		g_Camera[1].Viewport.MinZ = 0.0f;
		g_Camera[1].Viewport.MaxZ = 1.0f;

		g_Camera[1].posV = D3DXVECTOR3(0.0f, 6000.0f, 0.0f);
		g_Camera[1].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera[1].posU = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		g_Camera[1].rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera[1].nCounterAngle = 0;
	}
	//fLength = sqrtf(
	//	//Xの距離を求める　Xの距離の2乗
	//	(g_Camera[1].posR.x - g_Camera[1].posV.x) * (g_Camera[1].posR.x - g_Camera[1].posV.x)
	//	//Zの距離を求める　Zの距離の2乗
	//	+ (g_Camera[1].posR.z - g_Camera[1].posV.z) * (g_Camera[1].posR.z - g_Camera[1].posV.z));
#endif
	
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// カメラの更新処理
//=============================================================================
void UpdateCamera(void)
{

	PLAYER *pPlayer;
	//プレイヤーの取得
	pPlayer = GetPlayer();
	//現在のモードを取得
	MODE Mode = GetMode();

	switch (Mode)
	{
	case MODE_GAME:
		//最低限のカメラ平行移動

		//デバックカメラ
#if 0
		//任意のキーA
		if (GetKeyboardPress(DIK_LEFT) == true)
		{
			if (GetKeyboardPress(DIK_UP) == true)
			{//左奥移動
			 //カメラの移動	カメラの移動する角度(カメラの向き + 角度) * 移動量
				g_Camera[0].posV.x -= sinf(g_Camera[0].rotCamera.y + D3DX_PI * 0.75f) * MOVE_CAMERA;
				g_Camera[0].posV.z -= cosf(g_Camera[0].rotCamera.y + D3DX_PI * 0.75f) * MOVE_CAMERA;
				//カメラの注視点の移動 カメラの位置 + カメラの角度 * カメラの距離
				g_Camera[0].posR.x = g_Camera[0].posV.x + sinf(g_Camera[0].rotCamera.y) * fLength;
				g_Camera[0].posR.z = g_Camera[0].posV.z + cosf(g_Camera[0].rotCamera.y) * fLength;
			}
			else if (GetKeyboardPress(DIK_DOWN) == true)
			{//左手前移動
			 //カメラの移動	カメラの移動する角度(カメラの向き + 角度) * 移動量
				g_Camera[0].posV.x -= sinf(g_Camera[0].rotCamera.y + D3DX_PI * 0.25f) * MOVE_CAMERA;
				g_Camera[0].posV.z -= cosf(g_Camera[0].rotCamera.y + D3DX_PI * 0.25f) * MOVE_CAMERA;
				//カメラの注視点の移動 カメラの位置 + カメラの角度 * カメラの距離
				g_Camera[0].posR.x = g_Camera[0].posV.x + sinf(g_Camera[0].rotCamera.y) * fLength;
				g_Camera[0].posR.z = g_Camera[0].posV.z + cosf(g_Camera[0].rotCamera.y) * fLength;
			}
			else
			{
				//カメラの移動	カメラの移動する角度(カメラの向き + 角度) * 移動量
				g_Camera[0].posV.x -= sinf(g_Camera[0].rotCamera.y + D3DX_PI * 0.5f) * MOVE_CAMERA;
				g_Camera[0].posV.z -= cosf(g_Camera[0].rotCamera.y + D3DX_PI * 0.5f) * MOVE_CAMERA;
				//カメラの注視点の移動 カメラの位置 + カメラの角度 * カメラの距離
				g_Camera[0].posR.x = g_Camera[0].posV.x + sinf(g_Camera[0].rotCamera.y) * fLength;
				g_Camera[0].posR.z = g_Camera[0].posV.z + cosf(g_Camera[0].rotCamera.y) * fLength;
			}
		}
		//任意のキーD
		else if (GetKeyboardPress(DIK_RIGHT) == true)
		{
			if (GetKeyboardPress(DIK_UP) == true)
			{//右奥移動
			 //カメラの移動	カメラの移動する角度(カメラの向き + 角度) * 移動量
				g_Camera[0].posV.x -= sinf(g_Camera[0].rotCamera.y - D3DX_PI * 0.75f) * MOVE_CAMERA;
				g_Camera[0].posV.z -= cosf(g_Camera[0].rotCamera.y - D3DX_PI * 0.75f) * MOVE_CAMERA;
				//カメラの注視点の移動 カメラの位置 + カメラの角度 * カメラの距離
				g_Camera[0].posR.x = g_Camera[0].posV.x + sinf(g_Camera[0].rotCamera.y) * fLength;
				g_Camera[0].posR.z = g_Camera[0].posV.z + cosf(g_Camera[0].rotCamera.y) * fLength;
			}
			else if (GetKeyboardPress(DIK_DOWN) == true)
			{//右手前移動
			 //カメラの移動	カメラの移動する角度(カメラの向き + 角度) * 移動量
				g_Camera[0].posV.x -= sinf(g_Camera[0].rotCamera.y - D3DX_PI * 0.25f) * MOVE_CAMERA;
				g_Camera[0].posV.z -= cosf(g_Camera[0].rotCamera.y - D3DX_PI * 0.25f) * MOVE_CAMERA;
				//カメラの注視点の移動 カメラの位置 + カメラの角度 * カメラの距離
				g_Camera[0].posR.x = g_Camera[0].posV.x + sinf(g_Camera[0].rotCamera.y) * fLength;
				g_Camera[0].posR.z = g_Camera[0].posV.z + cosf(g_Camera[0].rotCamera.y) * fLength;
			}
			else
			{
				//カメラの移動	カメラの移動する角度(カメラの向き + 角度) * 移動量
				g_Camera[0].posV.x += sinf(g_Camera[0].rotCamera.y + D3DX_PI * 0.5f) * MOVE_CAMERA;
				g_Camera[0].posV.z += cosf(g_Camera[0].rotCamera.y + D3DX_PI * 0.5f) * MOVE_CAMERA;
				//カメラの注視点の移動 カメラの位置 + カメラの角度 * カメラの距離
				g_Camera[0].posR.x = g_Camera[0].posV.x + sinf(g_Camera[0].rotCamera.y) * fLength;
				g_Camera[0].posR.z = g_Camera[0].posV.z + cosf(g_Camera[0].rotCamera.y) * fLength;
			}
		}
		else if (GetKeyboardPress(DIK_UP) == true)
		{//奥移動
		 //カメラの移動	カメラの移動する角度(カメラの向き + 角度) * 移動量
			g_Camera[0].posV.x += sinf(g_Camera[0].rotCamera.y) * MOVE_CAMERA;
			g_Camera[0].posV.z += cosf(g_Camera[0].rotCamera.y) * MOVE_CAMERA;
			//カメラの注視点の移動 カメラの位置 + カメラの角度 * カメラの距離
			g_Camera[0].posR.x = g_Camera[0].posV.x + sinf(g_Camera[0].rotCamera.y) * fLength;
			g_Camera[0].posR.z = g_Camera[0].posV.z + cosf(g_Camera[0].rotCamera.y) * fLength;


		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{//手前移動
		 //カメラの移動	カメラの移動する角度(カメラの向き + 角度) * 移動量
			g_Camera[0].posV.x -= sinf(g_Camera[0].rotCamera.y) * MOVE_CAMERA;
			g_Camera[0].posV.z -= cosf(g_Camera[0].rotCamera.y) * MOVE_CAMERA;
			//カメラの注視点の移動 カメラの位置 + カメラの角度 * カメラの距離
			g_Camera[0].posR.x = g_Camera[0].posV.x + sinf(g_Camera[0].rotCamera.y) * fLength;
			g_Camera[0].posR.z = g_Camera[0].posV.z + cosf(g_Camera[0].rotCamera.y) * fLength;
		}

		//任意のキーV
		if (GetKeyboardPress(DIK_V) == true)
		{
			g_Camera[0].rotCamera.y -= 0.05f;
			// -3.14を下回った
			if (g_Camera[0].rotCamera.y < -D3DX_PI)
			{
				// -3.14を 3.14にする
				g_Camera[0].rotCamera.y += D3DX_PI * 2.0f;
			}
			g_Camera[0].posV.x = g_Camera[0].posR.x + sinf(g_Camera[0].rotCamera.y + D3DX_PI)* fLength;
			g_Camera[0].posV.z = g_Camera[0].posR.z + cosf(g_Camera[0].rotCamera.y + D3DX_PI)* fLength;

		}
		//任意のキーB
		if (GetKeyboardPress(DIK_B) == true)
		{
			g_Camera[0].rotCamera.y += 0.05f;
			// 3.14を上回った
			if (g_Camera[0].rotCamera.y > D3DX_PI)
			{
				// 3.14を -3.14にする
				g_Camera[0].rotCamera.y -= D3DX_PI * 2.0f;
			}
			g_Camera[0].posV.x = g_Camera[0].posR.x + sinf(g_Camera[0].rotCamera.y + D3DX_PI)* fLength;
			g_Camera[0].posV.z = g_Camera[0].posR.z + cosf(g_Camera[0].rotCamera.y + D3DX_PI)* fLength;

	}
		//任意のキーQ
		if (GetKeyboardPress(DIK_Q) == true)
		{
			g_Camera[0].rotCamera.y -= 0.05f;
			// -3.14を下回った
			if (g_Camera[0].rotCamera.y < -D3DX_PI)
			{
				// -3.14を 3.14にする
				g_Camera[0].rotCamera.y += D3DX_PI * 2.0f;
			}
			g_Camera[0].posR.x = g_Camera[0].posV.x + sinf(g_Camera[0].rotCamera.y)* fLength;
			g_Camera[0].posR.z = g_Camera[0].posV.z + cosf(g_Camera[0].rotCamera.y)* fLength;

		}
		//任意のキーE
		if (GetKeyboardPress(DIK_E) == true)
		{
			g_Camera[0].rotCamera.y += 0.05f;
			// 3.14を上回った
			if (g_Camera[0].rotCamera.y > D3DX_PI)
			{
				// 3.14を -3.14にする
				g_Camera[0].rotCamera.y -= D3DX_PI * 2.0f;
			}
			g_Camera[0].posR.x = g_Camera[0].posV.x + sinf(g_Camera[0].rotCamera.y)* fLength;
			g_Camera[0].posR.z = g_Camera[0].posV.z + cosf(g_Camera[0].rotCamera.y)* fLength;

		}

		//任意のキーT
		if (GetKeyboardPress(DIK_T) == true)
		{
			g_Camera[0].rotCamera.x += 2.5f;
			g_Camera[0].posR.y = g_Camera[0].rotCamera.x;
		}
		//任意のキーG
		if (GetKeyboardPress(DIK_G) == true)
		{
			g_Camera[0].rotCamera.x -= 2.5f;
			g_Camera[0].posR.y = g_Camera[0].rotCamera.x;
		}
		//任意のキーY
		if (GetKeyboardPress(DIK_Y) == true)
		{
			g_Camera[0].posV.y += 2.0f;
		}
		//任意のキーN
		if (GetKeyboardPress(DIK_H) == true)
		{
			g_Camera[0].posV.y -= 2.0f;
		}
#endif // 0

		//ゲームカメラ
#if 1
		g_Camera[0].posRDest.x = pPlayer->pos.x - sinf(pPlayer->rot.y) * 10;
		//g_Camera[0].posRDest.y = pPlayer->pos.y - sinf(pPlayer->rot.x) * 30;
		g_Camera[0].posRDest.z = pPlayer->pos.z - cosf(pPlayer->rot.y) * 30;
		//
		g_Camera[0].posVDest.x = pPlayer->pos.x - sinf(g_Camera[0].rotCamera.y) * fLength / 1.5f;
		//g_Camera[0].posVDest.y = pPlayer->pos.y - sinf(g_Camera[0].rotCamera.x) * fLength;
		g_Camera[0].posVDest.z = pPlayer->pos.z - cosf(g_Camera[0].rotCamera.y) * fLength / 1.5f;

		////減速
		g_Camera[0].posR.x += (g_Camera[0].posRDest.x - g_Camera[0].posR.x) * 0.2f + pPlayer->move.x;
		//g_Camera[0].posR.y += (g_Camera[0].posRDest.y - g_Camera[0].posR.y) * 0.2f;
		g_Camera[0].posR.z += (g_Camera[0].posRDest.z - g_Camera[0].posR.z) * 0.2f + pPlayer->move.z;

		g_Camera[0].posV.x += ((g_Camera[0].posVDest.x - g_Camera[0].posV.x) * 0.2f);
		//g_Camera[0].posV.y += ((g_Camera[0].posVDest.y - g_Camera[0].posV.y) * 0.2f) + 20;
		g_Camera[0].posV.z += ((g_Camera[0].posVDest.z - g_Camera[0].posV.z) * 0.2f);

		if (pPlayer->type == PALYERTYPE_WAIK)
		{
			//カメラをプレイヤーの後ろにする
			g_Camera[0].rotDest.y = (pPlayer->rot.y + D3DX_PI);
			//カメラとプレイヤーの距離を求める		
			//g_Camera[0].DiffAngle.x = g_Camera[0].rotDest.x - g_Camera[0].rotCamera.x;
			g_Camera[0].DiffAngle.y = g_Camera[0].rotDest.y - g_Camera[0].rotCamera.y;
			//g_Camera[0].DiffAngle.z = g_Camera[0].rotDest.z - g_Camera[0].rotCamera.z;

			//角度の設定
			if (g_Camera[0].DiffAngle.y > D3DX_PI)
			{
				g_Camera[0].DiffAngle.y -= D3DX_PI* 2.0f;
			}
			if (g_Camera[0].DiffAngle.y < -D3DX_PI)
			{
				g_Camera[0].DiffAngle.y += D3DX_PI* 2.0f;
			}
			//
			g_Camera[0].rotCamera.y += g_Camera[0].DiffAngle.y * 0.03f;

			if (g_Camera[0].rotCamera.y > D3DX_PI)
			{
				g_Camera[0].rotCamera.y -= D3DX_PI* 2.0f;
			}
			if (g_Camera[0].rotCamera.y < -D3DX_PI)
			{
				g_Camera[0].rotCamera.y += D3DX_PI* 2.0f;
			}

		}

		//上からカメラ
		g_Camera[1].posV = D3DXVECTOR3(pPlayer->pos.x, 6000.0f, pPlayer->pos.z - 900.0f);
		g_Camera[1].posR = D3DXVECTOR3(pPlayer->pos.x ,0.0f, pPlayer->pos.z - 900.0f);
#endif
		break;

	case MODE_TITLE:
		//カウンターの加算
		g_Camera[0].nCounterAngle++;
		if (g_Camera[0].nCounterAngle < 300)
		{		
			g_Camera[0].rotCamera.y = 0.0f * D3DX_PI;
			g_Camera[0].posV = D3DXVECTOR3(300.0f, 150.0f, 0.0f);
			g_Camera[0].posR = D3DXVECTOR3(330, 150, -610);
		}
		if (g_Camera[0].nCounterAngle == 300)
		{
			g_Camera[0].rotCamera.y = 0.0f * D3DX_PI;
			g_Camera[0].posV = D3DXVECTOR3(1000.0f, 1500.0f, -2000.0f);
			g_Camera[0].posR = D3DXVECTOR3(1000.0f, 1000.0f, 3000.0f);	
		}
		if (g_Camera[0].nCounterAngle == 600)
		{
			g_Camera[0].rotCamera.y = 0.3f * D3DX_PI;
			g_Camera[0].posV = D3DXVECTOR3(7200.0f, 500.0f, 7500.0f);
			g_Camera[0].posR = D3DXVECTOR3(7200.0f, 480.0f, 7500.0f);

		}
		if (g_Camera[0].nCounterAngle == 900)
		{
			g_Camera[0].rotCamera.y = 0.0f * D3DX_PI;
			g_Camera[0].posV = D3DXVECTOR3(1900.0f, 300.0f, 6600.0f);
			g_Camera[0].posR = D3DXVECTOR3(1900.0f, 450.0f, 6600.0f);
		}
		if (g_Camera[0].nCounterAngle == 1200)
		{
			g_Camera[0].nCounterAngle = 0;
			g_Camera[0].rotCamera.y = 0.0f * D3DX_PI;
		}

		g_Camera[0].rotCamera.y += 0.003f;
		// 3.14を上回った
		if (g_Camera[0].rotCamera.y > D3DX_PI)
		{
			// 3.14を -3.14にする
			g_Camera[0].rotCamera.y -= D3DX_PI * 2.0f;
		}
		g_Camera[0].posV.x = g_Camera[0].posR.x + sinf(g_Camera[0].rotCamera.y + D3DX_PI)* fLength;
		g_Camera[0].posV.z = g_Camera[0].posR.z + cosf(g_Camera[0].rotCamera.y + D3DX_PI)* fLength;
		break;
	}
}

//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(int nIdxCamera)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得


												//　ビューポートの設定
	pDevice->SetViewport(&g_Camera[nIdxCamera].Viewport);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera[nIdxCamera].mtxProjection);

	if (nIdxCamera == 0)
	{
		// プロジェクションマトリックスを作成
		D3DXMatrixPerspectiveFovLH(&g_Camera[nIdxCamera].mtxProjection,
			D3DXToRadian(45.0f),
			(float)g_Camera[nIdxCamera].Viewport.Width / (float)g_Camera[nIdxCamera].Viewport.Height,
			10.0f,
			6000.0f);
	}
	if (nIdxCamera == 1)
	{
		// プロジェクションマトリックスを作成
		D3DXMatrixPerspectiveFovLH(&g_Camera[nIdxCamera].mtxProjection,
			D3DXToRadian(30.0f),
			(float)g_Camera[nIdxCamera].Viewport.Width  / (float)g_Camera[nIdxCamera].Viewport.Height,
			10.0f,
			6000.0f);
	}
	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera[nIdxCamera].mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera[nIdxCamera].mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&g_Camera[nIdxCamera].mtxView,
		&g_Camera[nIdxCamera].posV,
		&g_Camera[nIdxCamera].posR,
		&g_Camera[nIdxCamera].posU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera[nIdxCamera].mtxView);
}

//=============================================================================
// カメラの取得
//=============================================================================
Camera *GetCamera(void)
{
	return &g_Camera[0];
}