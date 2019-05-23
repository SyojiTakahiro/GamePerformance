//=============================================================================
// 
// チュートリアルの処理 [tutorial.cpp]
// Author : Mikiya Meguro
//
//=============================================================================
#include "tutorial.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "pressenter.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TUTORIAL_TEXTURENAME000		"data\\TEXTURE\\Tutorial000.jpg"	//テクスチャのファイル名
#define TUTORIAL_TEXTURENAME001		"data\\TEXTURE\\Tutorial001.jpg"	//テクスチャのファイル名
#define TUTORIAL_TEXTURENAME002		"data\\TEXTURE\\Tutorial002.jpg"	//テクスチャのファイル名
#define TUTORIAL_X				(0)							//左上座標X
#define TUTORIAL_Y				(0)							//左上座標Y	
#define TUTORIAL_WIDTH			(SCREEN_WIDTH)				//正方形の横幅座標
#define TUTORIAL_HEIGHT			(SCREEN_HEIGHT)				//正方形の縦幅座標
#define MAX_TEXTURE				(3)
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTutorial[MAX_TEXTURE] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;					//頂点バッファへのポインタ
JoyState g_JoyStateTutorial;
TUTORIAL g_Tutorial;
//*****************************************************************************
// 初期化
//*****************************************************************************
void InitTutorial(void)
{	//ローカル変数
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	g_JoyStateTutorial.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TUTORIAL_TEXTURENAME000,
		&g_pTextureTutorial[0]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TUTORIAL_TEXTURENAME001,
		&g_pTextureTutorial[1]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TUTORIAL_TEXTURENAME002,
		&g_pTextureTutorial[2]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(TUTORIAL_X, TUTORIAL_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TUTORIAL_WIDTH, TUTORIAL_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TUTORIAL_X, TUTORIAL_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TUTORIAL_WIDTH, TUTORIAL_HEIGHT, 0.0f);
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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();
	//初期化
	g_Tutorial.bUse = true;
	g_Tutorial.nType = 0;

	//プレスエンター
	InitPressEnter();
	SetPressEnter(D3DXVECTOR3(SCREEN_WIDTH / 2, 420.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 200.0f, 50.0f);

}
//=============================================================================
// 終了処理
//=============================================================================
void UninitTutorial(void)
{	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (g_pTextureTutorial[nCnt] != NULL)
		{
			g_pTextureTutorial[nCnt]->Release();
			g_pTextureTutorial[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄の破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}

	UninitPressEnter();
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateTutorial(void)
{
	FADE fade;
	fade = GetFade();
	JoyState pJoyState = GetJoystate();
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//エンターキー 先に進む
	if (GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE || GetKeyboardTrigger(DIK_RIGHT) == true && fade == FADE_NONE
		//|| (state.Gamepad.wButtons & XINPUT_GAMEPAD_B  && pJoyState.bConnectionJoypad == true) && fade == FADE_NONE
		|| (state.Gamepad.wButtons & XINPUT_GAMEPAD_A  && pJoyState.bConnectionJoypad == true) && fade == FADE_NONE 
		|| (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y  && pJoyState.bConnectionJoypad == true)
		|| (state.Gamepad.wButtons & XINPUT_GAMEPAD_X  && pJoyState.bConnectionJoypad == true))
	{
		if (g_JoyStateTutorial.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			//エンターキー
			if (GetKeyboardTrigger(DIK_RETURN) == true || GetKeyboardTrigger(DIK_RIGHT) == true 
				//|| (state.Gamepad.wButtons & XINPUT_GAMEPAD_B  && pJoyState.bConnectionJoypad == true) 
				|| (state.Gamepad.wButtons & XINPUT_GAMEPAD_A  && pJoyState.bConnectionJoypad == true)
				|| (state.Gamepad.wButtons & XINPUT_GAMEPAD_X  && pJoyState.bConnectionJoypad == true) 
				|| (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y  && pJoyState.bConnectionJoypad == true))
			{
				
				if (g_Tutorial.nType < MAX_TEXTURE - 1)
				{
					g_Tutorial.nType++;
				}
				else
				{
					PlaySound(SOUND_LABEL_SE_KLAXON1);
					SetFade(MODE_GAME);
				}
			}
		}

		g_JoyStateTutorial.nJoypadState = INPUT_JOYSTATE_PUSH;
	}
	//前に戻る
	if (GetKeyboardTrigger(DIK_LEFT) == true && fade == FADE_NONE 
		|| (state.Gamepad.wButtons & XINPUT_GAMEPAD_B  && pJoyState.bConnectionJoypad == true) && fade == FADE_NONE)
	{
		if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B  && pJoyState.bConnectionJoypad == true) || GetKeyboardTrigger(DIK_LEFT) == true)
		{
			
				if (g_Tutorial.nType > 0)
				{
					g_Tutorial.nType--;
				}
			
		}
		g_JoyStateTutorial.nJoypadState = INPUT_JOYSTATE_PUSH;
	}
	UpdatePressEnter();

	//ジョイパッドの状態更新
	if (g_JoyStateTutorial.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateTutorial.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateTutorial.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateTutorial.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawTutorial(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTutorial[g_Tutorial.nType]);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//開始する頂点のインデックス
		2); //描画するプリミティブ数

	//DrawPressEnter();
}