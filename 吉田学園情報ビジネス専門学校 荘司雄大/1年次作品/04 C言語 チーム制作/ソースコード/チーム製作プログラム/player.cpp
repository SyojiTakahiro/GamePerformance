//=============================================================================
//
// プレイヤーの処理 [player.cpp]
// Author : Shun Yokomichi
//
//=============================================================================
#include "player.h"
#include "input.h"			// 入力処理
#include "shadow.h"			// 影の処理
#include "camera.h"			// カメラの処理
#include "effect.h"			// エフェクトの処理
#include "explosion.h"		// 爆発の処理
#include "model.h"			// モデルの処理
#include "game.h"			// ゲームの処理
#include "goal.h"			// ゴールの処理
#include "passage.h"		// 通行人の処理
#include "car.h"			// 車の処理
#include "obstacle.h"		// オブジェクトの処理
#include "sound.h"
#include "inputx.h"
#include "particle.h"
#include "deathpizza.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_PLAYER			(2)									// プレイヤーの総数
#define PLAYER_NAME000		"data/MODEL/player.x"				//	プレイヤー(通常)の名前
#define PLAYER_NAME001		"data/MODEL/Pizza.x"				//	プレイヤー(覚醒)の名前
#define MOVE_PLAYER			(2.0f)								// プレイヤーのスピード
#define MOVE_PIZZA			(3.0f)								// Pizzaモードのスピード
#define MAX_OBSTACLE		(100)								// オブジェクトの最大数
#define MAX_OBSTACLE_TYPE   (OBSTACLE_TYPE_MAX )				// オブジェクトのタイプ数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
PLAYER			g_aPlayer[MAX_PLAYER];
int				g_nSwitching;					// モード切替（０ 通常 / 1 覚醒)
int				g_nCntEx;						// 煙を出すカウント
float			g_fAngel;
float			g_fMove;						// プレイヤーの移動速度
bool			g_bRotchang;					// 向きの切り替え
JoyState g_JoyStatePlayer;
//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_nSwitching = 0;
	g_fMove = 0.0f;
	g_fAngel = 0.14f;
	g_bRotchang = true;
	g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	// 初期化
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		// 位置・向きの初期設定
		g_aPlayer[nCntPlayer].pos = D3DXVECTOR3(500.0f, 3.0f, -850.0f);						// posの初期化(現在)
		g_aPlayer[nCntPlayer].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// posの初期化(目的)
		g_aPlayer[nCntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// moveの初期化
		g_aPlayer[nCntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// rot(現在)の初期化(通常 && Pizzaの縦)		
		g_aPlayer[nCntPlayer].rotPizza = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);					// rot(現在)の初期化(Pizzaの横)
		g_aPlayer[nCntPlayer].rotDest = D3DXVECTOR3(0.0f, D3DX_PI, 0.0f);					// rot(目的)の初期化
		g_aPlayer[nCntPlayer].DiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// プレイヤーの差分の初期化
		g_aPlayer[nCntPlayer].vtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);		// 最小値の場合は多めに数値をいれる（比較のため)
		g_aPlayer[nCntPlayer].vtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);		// 最大値の場合は少なめに数値をいれる（比較のため)
		g_aPlayer[nCntPlayer].type = PALYERTYPE_STOP;										// プレイヤーの種類の初期化
		g_aPlayer[nCntPlayer].fXaxis = 0;
		g_aPlayer[nCntPlayer].fZaxis = -40.0f;
		g_aPlayer[nCntPlayer].pMesh = 0;
		g_aPlayer[nCntPlayer].pBuffMat = 0;
		g_aPlayer[nCntPlayer].nNumMat = 0;
		g_aPlayer[nCntPlayer].rotnum = 4;
		g_aPlayer[nCntPlayer].ChangeState = CHANGESTATE_POSSIBLE;
		g_aPlayer[nCntPlayer].PizzaMode = PLAYERPIZZA_HUMAN;
	}
	g_aPlayer[0].state = PLAYERSTATE_NORMAL;
	g_aPlayer[1].state = PLAYERSTATE_AWAKENING;
	g_aPlayer[0].nIdyShadow = SetShadow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 15);
	g_aPlayer[0].bUse = true;
	g_aPlayer[1].bUse = false;


	// Xファイルの読み込み
	D3DXLoadMeshFromX(PLAYER_NAME000, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_aPlayer[0].pBuffMat, NULL, &g_aPlayer[0].nNumMat, &g_aPlayer[0].pMesh);
	D3DXLoadMeshFromX(PLAYER_NAME001, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_aPlayer[1].pBuffMat, NULL, &g_aPlayer[1].nNumMat, &g_aPlayer[1].pMesh);

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		int nNumVtx;		// 頂点数
		DWORD sizeFVF;		// 頂点フォーマットのサイズ
		BYTE *pVtxBuff;		// 頂点バッファへのポインタ

							// 頂点数の取得
		nNumVtx = g_aPlayer[nCntPlayer].pMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_aPlayer[nCntPlayer].pMesh->GetFVF());

		// 頂点バッファをロック
		g_aPlayer[nCntPlayer].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			// 頂点座標の代入

			if (vtx.x < g_aPlayer[nCntPlayer].vtxMin.x)
			{
				g_aPlayer[nCntPlayer].vtxMin.x = vtx.x;
			}
			if (vtx.y < g_aPlayer[nCntPlayer].vtxMin.y)
			{
				g_aPlayer[nCntPlayer].vtxMin.y = vtx.y;
			}
			if (vtx.z < g_aPlayer[nCntPlayer].vtxMin.z)
			{
				g_aPlayer[nCntPlayer].vtxMin.z = vtx.z;
			}
			if (vtx.x > g_aPlayer[nCntPlayer].vtxMax.x)
			{
				g_aPlayer[nCntPlayer].vtxMax.x = vtx.x;
			}
			if (vtx.y > g_aPlayer[nCntPlayer].vtxMax.y)
			{
				g_aPlayer[nCntPlayer].vtxMax.y = vtx.y;
			}
			if (vtx.z > g_aPlayer[nCntPlayer].vtxMax.z)
			{
				g_aPlayer[nCntPlayer].vtxMax.z = vtx.z;
			}
			pVtxBuff += sizeFVF;			// サイズ分のポインタを進める
		}
		g_aPlayer[nCntPlayer].vtxMaxBike = D3DXVECTOR3(g_aPlayer[0].vtxMax.x + 300, g_aPlayer[0].vtxMax.y, g_aPlayer[0].vtxMax.z+ 300);
		g_aPlayer[nCntPlayer].vtxMinBike = g_aPlayer[0].vtxMin;
		// 頂点バッファをアンロック
		g_aPlayer[nCntPlayer].pMesh->UnlockVertexBuffer();
	}

	
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		// メッシュの開放
		if (g_aPlayer[nCntPlayer].pMesh != NULL)
		{
			g_aPlayer[nCntPlayer].pMesh->Release();
			g_aPlayer[nCntPlayer].pMesh = NULL;
		}

		// マテリアルの開放
		if (g_aPlayer[nCntPlayer].pBuffMat != NULL)
		{
			g_aPlayer[nCntPlayer].pBuffMat->Release();
			g_aPlayer[nCntPlayer].pBuffMat = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	// 変数宣言
	Camera *pCamera;
	pCamera = GetCamera();
	GAMESTATE GameState = GetGameState();

	JoyState pJoyState = GetJoystate();
	XINPUT_STATE state;
	XInputGetState(0, &state);

	// 状態を管理
	switch (g_aPlayer[g_nSwitching].state)
	{
	case PLAYERSTATE_NORMAL:
		// 通常状態の場合

		g_nCntEx++;
		if ((g_nCntEx % 30) == 0 && g_aPlayer[0].type == PALYERTYPE_WAIK)
		{
			SetExplosion(D3DXVECTOR3(g_aPlayer[0].pos.x , g_aPlayer[0].pos.y + 20, g_aPlayer[0].pos.z) , D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), 15, 15);
		}
		g_aPlayer[g_nSwitching].rotDest.z = D3DX_PI * 0.0f + pCamera->rotCamera.z;
		break;

	case PLAYERSTATE_AWAKENING:
		// Pizzaモード状態
		g_nCntEx++;
		if (GetKeyboardTrigger(DIK_LSHIFT) == true || GetKeyboardTrigger(DIK_RSHIFT) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y  && pJoyState.bConnectionJoypad == true))
		{
			if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				g_bRotchang = g_bRotchang ? false : true;
			}

			g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
		}
		if (g_bRotchang == true)
		{	// Pizza(縦回転)
			g_nCntEx = 0;
			g_aPlayer[g_nSwitching].rotPizza.z = D3DX_PI * 0.0f + pCamera->rotCamera.z;
			g_aPlayer[g_nSwitching].pos.y = 20.0f;

			if (g_aPlayer[g_nSwitching].type == PALYERTYPE_WAIK)
			{	// プレイヤーが動いている場合
				g_aPlayer[g_nSwitching].rotPizza.x -= 0.3f;
				//g_aPlayer[g_nSwitching].rotPizza.y = 0.0f;
			}
		}
		else
		{	// Pizza(横回転)
			g_nCntEx++;
			g_aPlayer[g_nSwitching].rotPizza.x = 0.0f;
			g_aPlayer[g_nSwitching].rotPizza.z = D3DX_PI * 0.5f + pCamera->rotCamera.z;
			g_aPlayer[g_nSwitching].pos.y = 8.0f;

			if (g_aPlayer[g_nSwitching].type == PALYERTYPE_WAIK)
			{	// プレイヤーが動いている場合
				g_aPlayer[g_nSwitching].rotPizza.y += 0.1f;
			}

			if (g_nCntEx == 120)
			{
				g_bRotchang = true;
			}
		}
		break;
	}

	g_aPlayer[g_nSwitching].posOld = g_aPlayer[g_nSwitching].pos;		//前回の位置を保存
	g_aPlayer[g_nSwitching].type = PALYERTYPE_STOP;

	if (g_aPlayer[0].state == PLAYERSTATE_NORMAL && g_aPlayer[1].state == PLAYERSTATE_AWAKENING)
	{
		// それぞれの条件で動く向きを変える（移動）
		if (GetKeyboardPress(DIK_S) == true || (state.Gamepad.bLeftTrigger & XINPUT_GAMEPAD_LEFT_THUMB && pJoyState.bConnectionJoypad == true))
		{	//S or ↓ で減速
			g_aPlayer[g_nSwitching].rotnum = 6;
			g_aPlayer[g_nSwitching].type = PALYERTYPE_STOP;

			if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_NORMAL && GetKeyboardPress(DIK_D) == true || g_aPlayer[g_nSwitching].state == PLAYERSTATE_NORMAL && GetKeyboardPress(DIK_RIGHT) == true
				|| (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
			{
				g_aPlayer[g_nSwitching].rotDest.y = -D3DX_PI * 0.5f + pCamera->rotCamera.y;
				g_aPlayer[g_nSwitching].rotDest.z = D3DX_PI * 0.15f + pCamera->rotCamera.z;
			}
			else if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_NORMAL && GetKeyboardPress(DIK_A) == true 
				|| GetKeyboardPress(DIK_W) == true || (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true)
				|| (state.Gamepad.bRightTrigger & XINPUT_GAMEPAD_RIGHT_THUMB && pJoyState.bConnectionJoypad == true))
			{
				g_aPlayer[g_nSwitching].rotDest.y = D3DX_PI * 0.5f + pCamera->rotCamera.y;
				g_aPlayer[g_nSwitching].rotDest.z = -D3DX_PI * 0.15f + pCamera->rotCamera.z;
			}

		}
		else if (GetKeyboardPress(DIK_A) == true ||  (state.Gamepad.sThumbLX <= -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{
			g_aPlayer[g_nSwitching].type = PALYERTYPE_WAIK;
			if (GetKeyboardPress(DIK_W) == true ||  (state.Gamepad.bRightTrigger & XINPUT_GAMEPAD_RIGHT_THUMB && pJoyState.bConnectionJoypad == true))
			{	//左下移動
				g_aPlayer[g_nSwitching].rotnum = 0;

				if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_NORMAL)
				{
					g_aPlayer[g_nSwitching].move.x += sinf((-D3DX_PI * 0.25f) + pCamera->rotCamera.y) * g_fMove;
					g_aPlayer[g_nSwitching].move.z += cosf((-D3DX_PI * 0.25f) + pCamera->rotCamera.y) * g_fMove;
					g_aPlayer[g_nSwitching].rotDest.z = -D3DX_PI * 0.15f + pCamera->rotCamera.z;

				}
				else if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_AWAKENING)
				{
					g_aPlayer[g_nSwitching].move.x += sinf((-D3DX_PI * 0.25f) + pCamera->rotCamera.y) * MOVE_PIZZA;
					g_aPlayer[g_nSwitching].move.z += cosf((-D3DX_PI * 0.25f) + pCamera->rotCamera.y) * MOVE_PIZZA;
				}
			}
			else
			{	//左移動
				g_aPlayer[g_nSwitching].rotnum = 1;

				if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_NORMAL)
				{
					g_aPlayer[g_nSwitching].move.x += sinf((-D3DX_PI * 0.5f) + pCamera->rotCamera.y) * g_fMove;
					g_aPlayer[g_nSwitching].move.z += cosf((-D3DX_PI * 0.5f) + pCamera->rotCamera.y) * g_fMove;
					g_aPlayer[g_nSwitching].rotDest.z = -D3DX_PI * 0.15f + pCamera->rotCamera.z;
				}
				else if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_AWAKENING)
				{
					g_aPlayer[g_nSwitching].move.x += sinf((-D3DX_PI * 0.5f) + pCamera->rotCamera.y) * MOVE_PIZZA;
					g_aPlayer[g_nSwitching].move.z += cosf((-D3DX_PI * 0.5f) + pCamera->rotCamera.y) * MOVE_PIZZA;
				}
			}
		}
		else if (GetKeyboardPress(DIK_D) == true  || (state.Gamepad.sThumbLX >= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && pJoyState.bConnectionJoypad == true))
		{
			g_aPlayer[g_nSwitching].type = PALYERTYPE_WAIK;
			if (GetKeyboardPress(DIK_W) == true || GetKeyboardPress(DIK_UP) == true || (state.Gamepad.bRightTrigger & XINPUT_GAMEPAD_RIGHT_THUMB && pJoyState.bConnectionJoypad == true))
			{	//右下移動
				g_aPlayer[g_nSwitching].rotnum = 2;

				if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_NORMAL)
				{
					g_aPlayer[g_nSwitching].move.x += sinf((D3DX_PI * 0.25f) + pCamera->rotCamera.y) * g_fMove;
					g_aPlayer[g_nSwitching].move.z += cosf((D3DX_PI * 0.25f) + pCamera->rotCamera.y) * g_fMove;
					g_aPlayer[g_nSwitching].rotDest.z = D3DX_PI * 0.15f + pCamera->rotCamera.z;
				}
				else if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_AWAKENING)
				{
					g_aPlayer[g_nSwitching].move.x += sinf((D3DX_PI * 0.25f) + pCamera->rotCamera.y) * MOVE_PIZZA;
					g_aPlayer[g_nSwitching].move.z += cosf((D3DX_PI * 0.25f) + pCamera->rotCamera.y) * MOVE_PIZZA;
				}
			}
			else
			{	//右移動
				g_aPlayer[g_nSwitching].rotnum = 3;

				if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_NORMAL)
				{
					g_aPlayer[g_nSwitching].move.x += sinf((D3DX_PI * 0.5f) + pCamera->rotCamera.y) * g_fMove;
					g_aPlayer[g_nSwitching].move.z += cosf((D3DX_PI * 0.5f) + pCamera->rotCamera.y) * g_fMove;
					g_aPlayer[g_nSwitching].rotDest.z = D3DX_PI * 0.15f + pCamera->rotCamera.z;
				}
				else if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_AWAKENING)
				{
					g_aPlayer[g_nSwitching].move.x += sinf((D3DX_PI * 0.5f) + pCamera->rotCamera.y) * MOVE_PIZZA;
					g_aPlayer[g_nSwitching].move.z += cosf((D3DX_PI * 0.5f) + pCamera->rotCamera.y) * MOVE_PIZZA;
				}
			}
		}
		else if (GetKeyboardPress(DIK_W) == true || (state.Gamepad.bRightTrigger & XINPUT_GAMEPAD_RIGHT_THUMB && pJoyState.bConnectionJoypad == true))
		{	// W or ↑ で移動
			g_aPlayer[g_nSwitching].rotnum = 4;
			g_aPlayer[g_nSwitching].type = PALYERTYPE_WAIK;

			if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_NORMAL)
			{
				g_aPlayer[g_nSwitching].move.x += sinf((0) + pCamera->rotCamera.y) * g_fMove;
				g_aPlayer[g_nSwitching].move.z += cosf((0) + pCamera->rotCamera.y) * g_fMove;
			}
			else if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_AWAKENING)
			{
				g_aPlayer[g_nSwitching].move.x += sinf((0) + pCamera->rotCamera.y) * MOVE_PIZZA;
				g_aPlayer[g_nSwitching].move.z += cosf((0) + pCamera->rotCamera.y) * MOVE_PIZZA;
			}
		}
		else if (GetKeyboardPress(DIK_SPACE) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_A && pJoyState.bConnectionJoypad == true))
		{	// SPACEキーでバック移動
			g_aPlayer[g_nSwitching].rotnum = 5;
			g_aPlayer[g_nSwitching].type = PALYERTYPE_WAIK;

			if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_NORMAL)
			{
				g_aPlayer[g_nSwitching].move.x += sinf((D3DX_PI)+pCamera->rotCamera.y) * g_fMove / 2;
				g_aPlayer[g_nSwitching].move.z += cosf((D3DX_PI)+pCamera->rotCamera.y) * g_fMove / 2;
			}
			else if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_AWAKENING)
			{
				g_aPlayer[g_nSwitching].move.x += sinf((D3DX_PI)+pCamera->rotCamera.y) * MOVE_PIZZA / 2;
				g_aPlayer[g_nSwitching].move.z += cosf((D3DX_PI)+pCamera->rotCamera.y) * MOVE_PIZZA / 2;
			}
		}

		// それぞれの角度
		if (g_aPlayer[g_nSwitching].rotnum == 0)
		{	// 左下
			g_aPlayer[g_nSwitching].rotDest.y = D3DX_PI * 0.75f + pCamera->rotCamera.y;
		}
		if (g_aPlayer[g_nSwitching].rotnum == 1)
		{	// 左
			g_aPlayer[g_nSwitching].rotDest.y = D3DX_PI * 0.5f + pCamera->rotCamera.y;
		}
		if (g_aPlayer[g_nSwitching].rotnum == 2)
		{	// 右下
			g_aPlayer[g_nSwitching].rotDest.y = -D3DX_PI * 0.75f + pCamera->rotCamera.y;
		}
		if (g_aPlayer[g_nSwitching].rotnum == 3)
		{	// 右
			g_aPlayer[g_nSwitching].rotDest.y = -D3DX_PI * 0.5f + pCamera->rotCamera.y;
		}
		if (g_aPlayer[g_nSwitching].rotnum == 4)
		{	// 上
			g_aPlayer[g_nSwitching].rotDest.y = D3DX_PI + pCamera->rotCamera.y;
		}
		if (g_aPlayer[g_nSwitching].rotnum == 5)
		{	// バック
			g_aPlayer[g_nSwitching].rotDest.y = D3DX_PI + pCamera->rotCamera.y;
		}

		g_aPlayer[g_nSwitching].DiffAngle.y = g_aPlayer[g_nSwitching].rotDest.y - g_aPlayer[g_nSwitching].rot.y;

		//角度の設定
		if (g_aPlayer[g_nSwitching].DiffAngle.y > D3DX_PI)
		{
			g_aPlayer[g_nSwitching].DiffAngle.y -= D3DX_PI* 2.0f;
		}
		if (g_aPlayer[g_nSwitching].DiffAngle.y < -D3DX_PI)
		{
			g_aPlayer[g_nSwitching].DiffAngle.y += D3DX_PI* 2.0f;
		}

		g_aPlayer[g_nSwitching].rot.y += g_aPlayer[g_nSwitching].DiffAngle.y * 0.2f;

		if (g_aPlayer[g_nSwitching].rot.y > D3DX_PI)
		{
			g_aPlayer[g_nSwitching].rot.y -= D3DX_PI* 2.0f;
		}
		if (g_aPlayer[g_nSwitching].rot.y < -D3DX_PI)
		{
			g_aPlayer[g_nSwitching].rot.y += D3DX_PI* 2.0f;
		}

		if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_NORMAL)
		{	// 通常状態の場合
			g_aPlayer[g_nSwitching].DiffAngle.z = g_aPlayer[g_nSwitching].rotDest.z - g_aPlayer[g_nSwitching].rot.z;

			if (g_aPlayer[g_nSwitching].DiffAngle.z > D3DX_PI)
			{
				g_aPlayer[g_nSwitching].DiffAngle.z -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[g_nSwitching].DiffAngle.z < -D3DX_PI)
			{
				g_aPlayer[g_nSwitching].DiffAngle.z += D3DX_PI * 2.0f;
			}

			g_aPlayer[g_nSwitching].rot.z += g_aPlayer[g_nSwitching].DiffAngle.z * 0.1f;

			if (g_aPlayer[g_nSwitching].rot.z > D3DX_PI)
			{
				g_aPlayer[g_nSwitching].rot.z -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[g_nSwitching].rot.z < -D3DX_PI)
			{
				g_aPlayer[g_nSwitching].rot.z += D3DX_PI * 2.0f;
			}
		}
		if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_AWAKENING && g_bRotchang == true)
		{	// PIzza状態(縦回転)の場合
			g_aPlayer[g_nSwitching].DiffAngle.y = g_aPlayer[g_nSwitching].rotDest.y - g_aPlayer[g_nSwitching].rotPizza.y;

			if (g_aPlayer[g_nSwitching].DiffAngle.y > D3DX_PI)
			{
				g_aPlayer[g_nSwitching].DiffAngle.y -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[g_nSwitching].DiffAngle.y < -D3DX_PI)
			{
				g_aPlayer[g_nSwitching].DiffAngle.y += D3DX_PI * 2.0f;
			}

			g_aPlayer[g_nSwitching].rotPizza.y += g_aPlayer[g_nSwitching].DiffAngle.y * 0.1f;

			if (g_aPlayer[g_nSwitching].rotPizza.y > D3DX_PI)
			{
				g_aPlayer[g_nSwitching].rotPizza.y -= D3DX_PI * 2.0f;
			}
			if (g_aPlayer[g_nSwitching].rotPizza.y < -D3DX_PI)
			{
				g_aPlayer[g_nSwitching].rotPizza.y += D3DX_PI * 2.0f;
			}
		}

		// 速度調整
		if (g_aPlayer[g_nSwitching].type == PALYERTYPE_STOP)
		{
			g_fMove -= 0.017f;
			g_fAngel = 0.02f;
			if (g_fMove <= 0)
			{
				g_fMove = 0.0f;
			}
		}
		else if (g_aPlayer[g_nSwitching].type == PALYERTYPE_WAIK)
		{
			g_fAngel = 0.14f;
			g_fMove += 0.005f;
			if (g_fMove >= MOVE_PLAYER)
			{
				g_fMove = MOVE_PLAYER;
			}
		}
	
		//フィールドの大きさの判定
		if (9000 <= g_aPlayer[g_nSwitching].pos.x)
		{
			g_aPlayer[g_nSwitching].pos.x = 9000 - 1;
		}
		if (-3000 >= g_aPlayer[g_nSwitching].pos.x)
		{
			g_aPlayer[g_nSwitching].pos.x = -3000 + 1;
		}
		if (9000 <= g_aPlayer[g_nSwitching].pos.z)
		{
			g_aPlayer[g_nSwitching].pos.z = 9000 - 1;
		}
		if (-3000 >= g_aPlayer[g_nSwitching].pos.z)
		{
			g_aPlayer[g_nSwitching].pos.z = -3000 + 1;
		}

		//減速
		g_aPlayer[g_nSwitching].move.x += (0.0f - g_aPlayer[g_nSwitching].move.x) * g_fAngel;
		g_aPlayer[g_nSwitching].move.z += (0.0f - g_aPlayer[g_nSwitching].move.z) * g_fAngel;

		//位置更新（移動）
		g_aPlayer[g_nSwitching].pos.x += g_aPlayer[g_nSwitching].move.x;
		g_aPlayer[g_nSwitching].pos.z += g_aPlayer[g_nSwitching].move.z;

		//モデルとの当たり判定
		if (CollisionModel(&g_aPlayer[g_nSwitching].pos, &g_aPlayer[g_nSwitching].posOld, &g_aPlayer[g_nSwitching].move, g_aPlayer[g_nSwitching].vtxMax, g_aPlayer[g_nSwitching].vtxMin) == true)
		{//モデルに着地したとき

		}
		//ゴールとの当たり判定
		if (CollisionGoal(&g_aPlayer[g_nSwitching].pos, &g_aPlayer[g_nSwitching].posOld, &g_aPlayer[g_nSwitching].move, g_aPlayer[g_nSwitching].vtxMax, g_aPlayer[g_nSwitching].vtxMin) == true)
		{//ゴールに入った
			GOAL *pGoal = GetGoal();
			pGoal->nGoalPattern++;
		}
		// 通行人との当り判定
		if (CollisionPassage(&g_aPlayer[g_nSwitching].pos, &g_aPlayer[g_nSwitching].posOld, &g_aPlayer[g_nSwitching].move, g_aPlayer[g_nSwitching].vtxMax, g_aPlayer[g_nSwitching].vtxMin) == true)
		{
			if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_AWAKENING)
			{
				if (GameState != GAMESTATE_GAMEOVER)
				{
					PlaySound(SOUND_LABEL_SE_SAKEBI3);
					SetGameState(GAMESTATE_GAMEOVER);
					g_aPlayer[1].state = PLAYERSTATE_EAT;
					g_aPlayer[1].bUse = false;
					DeleteShadow(g_aPlayer[0].nIdyShadow);
					SetDeathPizza(D3DXVECTOR3(g_aPlayer[g_nSwitching].pos.x, g_aPlayer[g_nSwitching].pos.y + 10, g_aPlayer[g_nSwitching].pos.z),
						D3DXVECTOR3(0, 1, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 30);
					SetParticle(D3DXVECTOR3(g_aPlayer[g_nSwitching].pos.x, g_aPlayer[g_nSwitching].pos.y + 50, g_aPlayer[g_nSwitching].pos.z)
						, D3DXVECTOR3(0.8f, 0.2f, 0.8f), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(0, 0, 0, 0), 20, 30, PARTICLE_TYPE_PIZZA_DEATH);
				}
			}
		}
		// 車の当り判定
		if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_NORMAL || g_aPlayer[g_nSwitching].state == PLAYERSTATE_AWAKENING && g_bRotchang == true)
		{
			if (CollisionCar(&g_aPlayer[g_nSwitching].pos, &g_aPlayer[g_nSwitching].posOld, &g_aPlayer[g_nSwitching].move, g_aPlayer[g_nSwitching].vtxMax, g_aPlayer[g_nSwitching].vtxMin) == true)
			{
				if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_AWAKENING)
				{
					if (GameState != GAMESTATE_GAMEOVER)
					{
						PlaySound(SOUND_LABEL_SE_SAKEBI3);
						SetGameState(GAMESTATE_GAMEOVER);
						g_aPlayer[1].state = PLAYERSTATE_DIE;
						g_aPlayer[1].bUse = false;
						DeleteShadow(g_aPlayer[0].nIdyShadow);
						SetDeathPizza(D3DXVECTOR3(g_aPlayer[g_nSwitching].pos.x, g_aPlayer[g_nSwitching].pos.y + 10, g_aPlayer[g_nSwitching].pos.z),
							D3DXVECTOR3(0, 1, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 30);

						SetParticle(D3DXVECTOR3(g_aPlayer[g_nSwitching].pos.x, g_aPlayer[g_nSwitching].pos.y + 50, g_aPlayer[g_nSwitching].pos.z)
							, D3DXVECTOR3(0.8f, 0.2f, 0.8f), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(0, 0, 0, 0), 20, 30, PARTICLE_TYPE_PIZZA_DEATH);
					}
				}
			}
		}
		//オブジェクトとの当たり判定
		if (CollisionObstacle(&g_aPlayer[g_nSwitching].pos, &g_aPlayer[g_nSwitching].posOld, &g_aPlayer[g_nSwitching].move, g_aPlayer[g_nSwitching].vtxMax) == true)
		{//オブジェクトに当たった

			g_aPlayer[g_nSwitching].ChangeState = CHANGESTATE_IMPOSSIBLE;

			/*if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_AWAKENING)
			{
				g_aPlayer[1].bUse = false;
				DeleteShadow(g_aPlayer[0].nIdyShadow);
				g_aPlayer[g_nSwitching].state = PLAYERSTATE_DIE;
				SetDeathPizza(D3DXVECTOR3(g_aPlayer[g_nSwitching].pos.x, g_aPlayer[g_nSwitching].pos.y + 10, g_aPlayer[g_nSwitching].pos.z),
					D3DXVECTOR3(0, 1, 0), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 20, 30);

				SetParticle(D3DXVECTOR3(g_aPlayer[g_nSwitching].pos.x, g_aPlayer[g_nSwitching].pos.y + 50, g_aPlayer[g_nSwitching].pos.z)
					, D3DXVECTOR3(0.8f, 0.2f, 0.8f), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(0, 0, 0, 0), 20, 30, PARTICLE_TYPE_PIZZA_DEATH);
				if (GameState != GAMESTATE_GAMEOVER)
				{
					SetGameState(GAMESTATE_GAMEOVER);
				}
			}*/
			/*if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_NORMAL)
			{
			g_aPlayer[0].rot.x = -D3DX_PI * 0.15f + pCamera->rotCamera.x;
			g_aPlayer[0].pos.y = 18.0f;
			}*/
		}

		//バイク状態の当たり判定
		if (CollisionObstacleBike(&g_aPlayer[g_nSwitching].pos, &g_aPlayer[g_nSwitching].posOld, &g_aPlayer[g_nSwitching].move, g_aPlayer[0].vtxMaxBike) == true)
		{//オブジェクトに当たった
			//当たっているときは変身できない
			g_aPlayer[g_nSwitching].ChangeState = CHANGESTATE_IMPOSSIBLE;
		}
		// ENTERキーで切替
		if (GetKeyboardTrigger(DIK_RETURN) == true && g_aPlayer[g_nSwitching].ChangeState == CHANGESTATE_POSSIBLE || (state.Gamepad.wButtons & XINPUT_GAMEPAD_X  && pJoyState.bConnectionJoypad == true && g_aPlayer[g_nSwitching].ChangeState == CHANGESTATE_POSSIBLE))
		{
			if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
			{
				//SetEffect(D3DXVECTOR3(g_aPlayer[g_nSwitching].pos.x, g_aPlayer[g_nSwitching].pos.y + 15.0f, g_aPlayer[g_nSwitching].pos.z), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 50, 100);
				g_nSwitching = g_nSwitching ^ 1;										// 状態変化 (0 : 通常	1 : Pizza状態)
				g_aPlayer[g_nSwitching ^ 1].bUse = false;
				g_aPlayer[g_nSwitching].bUse = true;
				g_aPlayer[g_nSwitching].pos = g_aPlayer[g_nSwitching ^ 1].pos;			// posの引継ぎ
				g_aPlayer[g_nSwitching].rot = g_aPlayer[g_nSwitching ^ 1].rot;			// 方向の引継ぎ
				g_aPlayer[g_nSwitching].move = g_aPlayer[g_nSwitching ^ 1].move;		// moveの引き継ぎ
				g_nCntEx = 0;

				if (g_nSwitching == 0)
				{	// ０の場合は、通常状態へ
					g_aPlayer[g_nSwitching].state = PLAYERSTATE_NORMAL;
					g_aPlayer[g_nSwitching].pos.y = 3.0f;
					StopSound(SOUND_LABEL_BGM_PIZZA);
					PlaySound(SOUND_LABEL_BGM_GAME);
					g_aPlayer[0].PizzaMode = PLAYERPIZZA_HUMAN;
					g_aPlayer[1].PizzaMode = PLAYERPIZZA_HUMAN;
				}
				else
				{	// １の場合は、Pizzaモード状態へ
					SetParticle(D3DXVECTOR3(g_aPlayer[g_nSwitching].pos.x, g_aPlayer[g_nSwitching].pos.y + 50, g_aPlayer[g_nSwitching].pos.z)
						, D3DXVECTOR3(0.8f, 0.2f, 0.8f), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(0, 0, 0, 0), 20, 30, PARTICLE_TYPE_PIZZA);
					g_nCntEx = 0;
					g_aPlayer[g_nSwitching].state = PLAYERSTATE_AWAKENING;
					g_aPlayer[0].PizzaMode = PLAYERPIZZA_PIZZA;
					g_aPlayer[1].PizzaMode = PLAYERPIZZA_PIZZA;
					StopSound(SOUND_LABEL_BGM_GAME);
					PlaySound(SOUND_LABEL_BGM_PIZZA);
				}
			}
			g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_PUSH;
		}
		// 影
		SetPositionShadow(g_aPlayer[g_nSwitching].nIdyShadow, D3DXVECTOR3(g_aPlayer[g_nSwitching].pos.x, 0.1f, g_aPlayer[g_nSwitching].pos.z), 15);

		//ジョイパッドの状態更新
		if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_PUSH)
		{
			g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NONE;
		}
		else if (g_JoyStatePlayer.nJoypadState == INPUT_JOYSTATE_NONE)
		{
			g_JoyStatePlayer.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						// 現在のマテリアル保存用
	D3DXMATERIAL *pMat;							// マテリアルデータへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aPlayer[g_nSwitching].mtxWorld);
	if (g_aPlayer[g_nSwitching].state == PLAYERSTATE_AWAKENING)
	{
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[1].rotPizza.y, g_aPlayer[1].rotPizza.x, g_aPlayer[1].rotPizza.z);
		D3DXMatrixMultiply(&g_aPlayer[1].mtxWorld, &g_aPlayer[1].mtxWorld, &mtxRot);
	}
	else
	{
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer[g_nSwitching].rot.y, g_aPlayer[g_nSwitching].rot.x, g_aPlayer[g_nSwitching].rot.z);
		D3DXMatrixMultiply(&g_aPlayer[g_nSwitching].mtxWorld, &g_aPlayer[g_nSwitching].mtxWorld, &mtxRot);
	}

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aPlayer[g_nSwitching].pos.x, g_aPlayer[g_nSwitching].pos.y, g_aPlayer[g_nSwitching].pos.z);
	D3DXMatrixMultiply(&g_aPlayer[g_nSwitching].mtxWorld, &g_aPlayer[g_nSwitching].mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer[g_nSwitching].mtxWorld);

	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_aPlayer[g_nSwitching].pBuffMat->GetBufferPointer();

	if (g_aPlayer[g_nSwitching].bUse == true)
	{
		for (int nCntMat = 0; nCntMat < (int)g_aPlayer[g_nSwitching].nNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, NULL);

			// モデル(パーツ)の描画
			g_aPlayer[g_nSwitching].pMesh->DrawSubset(nCntMat);
		}
	}

	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

}

//=============================================================================
// プレイヤーの取得
//=============================================================================
PLAYER*GetPlayer(void)
{
	return &g_aPlayer[g_nSwitching];
}
