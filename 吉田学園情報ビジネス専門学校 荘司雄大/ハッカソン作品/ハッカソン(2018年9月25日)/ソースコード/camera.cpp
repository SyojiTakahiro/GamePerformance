//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "camera.h"
#include "player.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CAMERA_ANGEL_VIEW  (45.0f)    // カメラの画角
#define CAMERA_MOVE        (10.0f)    // カメラの移動量
#define AROUND_SPEED       (0.02f)    // 回り込み速度初期値
#define AROUND_TIME        (50)       // 回り込み待ち時間初期値
#define CAMERA_UP_DOWN_SPEED (3.0f)

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Camera g_camera;  // カメラの情報

//=============================================================================
// カメラの初期化処理
//=============================================================================
void InitCamera(void)
{

	if (GetMode() == MODE_GAME)
	{
		g_camera.posV = D3DXVECTOR3(0.0f, 300.0f, -500.0f);  // 現在の視点を初期化
		g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 現在の注視点を初期化
	}
	else
	{
		g_camera.posV = D3DXVECTOR3(0.0f, 100.0f, -1000.0f);  // 現在の視点を初期化
		g_camera.posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);     // 現在の注視点を初期化
	}
	g_camera.posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 目的の視点を初期化
	g_camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 目的の注視点を初期化
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);       // ベクトルを初期化
	g_camera.rot  = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // 現在の向きを初期化
	g_camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 目的の向きを初期化
	g_camera.state = CAMERASTATE_NORMAL;                 // 通常の状態に
	g_camera.fWaraparoundSpeed = AROUND_SPEED;           // 回り込み速度を初期化
	g_camera.nWaraparoundTime = AROUND_TIME;             // 回り込み待ち時間を初期化
	g_camera.nCounterTime = 0;                           // 待ち時間カウンターを初期化
	g_camera.posVAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 目的の視点座標を初期化
	g_camera.posRAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 目的の注視点座標を初期化

/*	g_camera.rot.y = -GetPlayer()->rot.y;
	g_camera.rotDest.y = -GetPlayer()->rot.y; */                                                          // 目的の向きは最初は同じにする
	g_camera.fLength = 300.0f;  // 距離を計算
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
// カメラ操作の処理
#if 0
     //-----------------
     // 視点移動
     //-----------------
	if (GetKeyboardPress(DIK_LEFT) == true)
	{// Aキーが押された
		if (GetKeyboardPress(DIK_UP) == true)
		{// 同時にWキーが押された
			g_camera.posV.x -= sinf(g_camera.rot.y + (D3DX_PI * 0.75f)) * CAMERA_MOVE;
			g_camera.posV.z -= cosf(g_camera.rot.y + (D3DX_PI * 0.75f)) * CAMERA_MOVE;
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{// 同時にSキーが押された
			g_camera.posV.x -= sinf(g_camera.rot.y + (D3DX_PI * 0.25f)) * CAMERA_MOVE;
			g_camera.posV.z -= cosf(g_camera.rot.y + (D3DX_PI * 0.25f)) * CAMERA_MOVE;
		}
		else
		{// 同時に何も押されていない
			g_camera.posV.x -= sinf(g_camera.rot.y + (D3DX_PI * 0.5f)) * CAMERA_MOVE;
			g_camera.posV.z -= cosf(g_camera.rot.y + (D3DX_PI * 0.5f)) * CAMERA_MOVE;
		}

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fLength;
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{// Dキーが押された
		if (GetKeyboardPress(DIK_UP) == true)
		{// 同時にWキーが押された
			g_camera.posV.x -= sinf(g_camera.rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVE;
			g_camera.posV.z -= cosf(g_camera.rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVE;
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{// 同時にSキーが押された
			g_camera.posV.x -= sinf(g_camera.rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVE;
			g_camera.posV.z -= cosf(g_camera.rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVE;
		}
		else
		{// 同時に何も押されていない
			g_camera.posV.x -= sinf(g_camera.rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVE;
			g_camera.posV.z -= cosf(g_camera.rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVE;
		}

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fLength;
	}
	else if (GetKeyboardPress(DIK_UP) == true)
	{// Wキーが押された
		g_camera.posV.x += sinf(g_camera.rot.y) * CAMERA_MOVE;
		g_camera.posV.z += cosf(g_camera.rot.y) * CAMERA_MOVE;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fLength;
	}
	else if (GetKeyboardPress(DIK_DOWN) == true)
	{// Sキーが押された
		g_camera.posV.x -= sinf(g_camera.rot.y) * CAMERA_MOVE;
		g_camera.posV.z -= cosf(g_camera.rot.y) * CAMERA_MOVE;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fLength;
	}
	//-----------------
	// 視点旋回
	//-----------------
	if (GetKeyboardPress(DIK_Z) == true)
	{// Zキーが押された
		g_camera.rot.y -= D3DX_PI / 120;
		if (g_camera.rot.y <= -D3DX_PI)
		{
			g_camera.rot.y = D3DX_PI;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;

	}
	if (GetKeyboardPress(DIK_C) == true)
	{// Cキーが押された
		g_camera.rot.y += D3DX_PI / 120;
		if (g_camera.rot.y >= D3DX_PI)
		{
			g_camera.rot.y = -D3DX_PI;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;
	}
	//-----------------
	// 注視点旋回
	//-----------------
	if (GetKeyboardPress(DIK_Q) == true)
	{// Qキーが押された
		g_camera.rot.y -= D3DX_PI / 120;
		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y = D3DX_PI;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fLength;
	}
	if (GetKeyboardPress(DIK_E) == true)
	{// Eキーが押された
		g_camera.rot.y += D3DX_PI / 120;
		if (g_camera.rot.y >= D3DX_PI)
		{
			g_camera.rot.y = -D3DX_PI;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * g_camera.fLength;
	}
	//-----------------
	// カメラリセット
	//-----------------
	if (GetKeyboardPress(DIK_G) == true)
	{// SPACEキーが押された
		g_camera.posV = D3DXVECTOR3(0.0f, 130.0f, -250.0f);
		g_camera.posR = D3DXVECTOR3(0.0f, 00.0f, 0.0f);
		g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_camera.rot.y = atan2f(g_camera.posV.x - g_camera.posR.x, g_camera.posV.z - g_camera.posR.z);
		g_camera.fLength = sqrtf((g_camera.posV.x - g_camera.posR.x) * (g_camera.posV.x - g_camera.posR.x) + (g_camera.posV.z - g_camera.posR.z) * (g_camera.posV.z - g_camera.posR.z));
	}

	//-----------------
	// 注視点上昇下降
	//-----------------
	if (GetKeyboardPress(DIK_T) == true)
	{// Tキーが押された
		g_camera.posR.y += CAMERA_UP_DOWN_SPEED;
	}
	if (GetKeyboardPress(DIK_B) == true)
	{// Bキーが押された
		g_camera.posR.y -= CAMERA_UP_DOWN_SPEED;
	}
	//-----------------
	// 視点上昇下降
	//-----------------
	if (GetKeyboardPress(DIK_Y) == true)
	{// Yキーが押された
		g_camera.posV.y += CAMERA_UP_DOWN_SPEED;
	}
	if (GetKeyboardPress(DIK_N) == true)
	{// Nキーが押された
		g_camera.posV.y -= CAMERA_UP_DOWN_SPEED;
	}

	//------------------------
	// ズームイン ズームアウト
	//------------------------
	if (GetKeyboardPress(DIK_U) == true)
	{// Uキーが押された
		if (g_camera.fLength >= 60)
		{
			g_camera.fLength -= 2.0f;
		}
	}
	if (GetKeyboardPress(DIK_M) == true)
	{// Mキーが押された
		if (g_camera.fLength <= 350)
		{
			g_camera.fLength += 2.0f;
		}
	}
#endif

	if (GetMode() == MODE_TITLE)
	{// タイトル中だったら
		g_camera.rot.y += D3DX_PI / 1200;
		if (g_camera.rot.y >= D3DX_PI)
		{
			g_camera.rot.y = -D3DX_PI;
		}
		g_camera.posV.x = g_camera.posR.x + sinf(g_camera.rot.y) * 1000;
		g_camera.posV.z = g_camera.posR.z + cosf(g_camera.rot.y) * 1000;
	}
	else if (GetMode() == MODE_GAME)
	{// ゲーム中だったら
		D3DXVECTOR3 PlayerPos[MAX_PLAYER];  // プレイヤーの座標
		for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
		{// プレイヤーの人数分繰り返し
			PlayerPos[nCntPlayer] = GetPlayer(nCntPlayer)->pos;
		}

		g_camera.posR.x = (PlayerPos[0].x + PlayerPos[1].x) / 2;

		if ((PlayerPos[0].x + PlayerPos[1].x) / 2 >= 150.0f)
		{// プレイヤー同士が離れすぎている
			g_camera.fLength = (PlayerPos[0].x + PlayerPos[1].x) / 2 + 300.0f;
		}

		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fLength;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fLength;
	}
}
//=============================================================================
// カメラの設定処理
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(CAMERA_ANGEL_VIEW),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		9000.0f);

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}
//=============================================================================
// カメラの取得
//=============================================================================
Camera *GetCamera(void)
{
	return &g_camera;
}