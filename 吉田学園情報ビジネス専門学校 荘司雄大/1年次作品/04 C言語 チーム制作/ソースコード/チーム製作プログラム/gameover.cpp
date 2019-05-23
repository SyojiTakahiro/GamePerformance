//=============================================================================
// 
// ゲームオーバーの処理 [gameover.cpp]
// Author : Mikiya Meguro
//
//=============================================================================
#include "gameover.h"
#include "input.h"
#include "fade.h"
#include "gameoverlogo.h"
#include "player.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GAMEOVER_TEXTURENAME		"data\\TEXTURE\\gameover000.jpg"	// テクスチャのファイル名
#define GAMEOVER_TEXTURENAME001		"data\\TEXTURE\\gameover001.jpg"	// テクスチャのファイル名
#define GAMEOVER_TEXTURENAME002		"data\\TEXTURE\\gameover002.png"	// テクスチャのファイル名
#define POS_X						(0)								//左上座標X
#define POS_Y						(0)								//左上座標Y	
#define BG_WIDTH					(SCREEN_WIDTH)					//正方形の横幅座標
#define BG_HEIGHT					(SCREEN_HEIGHT)					//正方形の縦幅座標

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturegameover = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffgameover = NULL;	//頂点バッファへのポインタ
int g_nCntGameOver;
JoyState g_JoyStateGameOver;
//*****************************************************************************
// 初期化
//*****************************************************************************
void InitGameOver(void)
{	//ローカル変数
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	PLAYER *pPlayer = GetPlayer();
	g_JoyStateGameOver.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	if (pPlayer->state == PLAYERSTATE_EAT)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			GAMEOVER_TEXTURENAME,
			&g_pTexturegameover);
	}
	else if (pPlayer->state == PLAYERSTATE_GAUGEZERO)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			GAMEOVER_TEXTURENAME001,
			&g_pTexturegameover);
	}
	else
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			GAMEOVER_TEXTURENAME002,
			&g_pTexturegameover);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffgameover,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffgameover->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(POS_X, POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH, POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(POS_X, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//頂点
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点バッファをアンロックする
	g_pVtxBuffgameover->Unlock();
	//カウントリセット
	g_nCntGameOver = 0;

	InitGameoverLogo();
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitGameOver(void)
{	//テクスチャの破棄
	if (g_pTexturegameover != NULL)
	{
		g_pTexturegameover->Release();
		g_pTexturegameover = NULL;
	}
	//頂点バッファの破棄の破棄
	if (g_pVtxBuffgameover != NULL)
	{
		g_pVtxBuffgameover->Release();
		g_pVtxBuffgameover = NULL;
	}

	UninitGameoverLogo();
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateGameOver(void)
{
	JoyState pJoyState = GetJoystate();
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//エンターキー
	if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_B  && pJoyState.bConnectionJoypad == true) || (state.Gamepad.wButtons & XINPUT_GAMEPAD_A  && pJoyState.bConnectionJoypad == true)
		|| (state.Gamepad.wButtons & XINPUT_GAMEPAD_X  && pJoyState.bConnectionJoypad == true) || (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y  && pJoyState.bConnectionJoypad == true))
	{
		if (g_JoyStateGameOver.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			SetFade(MODE_TITLE);
		}

		g_JoyStateGameOver.nJoypadState = INPUT_JOYSTATE_PUSH;
	}
	g_nCntGameOver++;
	if (g_nCntGameOver >= 600)
	{
		SetFade(MODE_TITLE);
	}

	//ジョイパッドの状態更新
	if (g_JoyStateGameOver.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateGameOver.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateGameOver.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateGameOver.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawGameOver(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffgameover, 0, sizeof(VERTEX_2D));
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturegameover);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//開始する頂点のインデックス
		2); //描画するプリミティブ数

	DrawGameoverLogo();
}