//=============================================================================
// 
// リザルトの処理 [result.cpp]
// Author : Mikiya Meguro
//
//=============================================================================
#include "result.h"
#include "input.h"
#include "fade.h"
//#include "score.h"
//#include "time.h"
#include "resultlogo.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_TEXTURENAME		"data\\TEXTURE\\Gameclear.jpg"	//テクスチャのファイル名
#define RESULT_POS_X				(0)							//左上座標X
#define RESULT_POS_Y				(0)							//左上座標Y	
#define RESULT_WIDTH			(SCREEN_WIDTH)				//正方形の横幅座標
#define RESULT_HEIGHT			(SCREEN_HEIGHT)				//正方形の縦幅座標

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureResult = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;	//頂点バッファへのポインタ
int g_nCounterGameClear;
JoyState g_JoyStateResult;
//*****************************************************************************
// 初期化
//*****************************************************************************
void InitResult(void)
{	//ローカル変数	
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	
	g_nCounterGameClear = 0;
	g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TITLE_TEXTURENAME,
		&g_pTextureResult);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(RESULT_WIDTH, RESULT_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(RESULT_WIDTH, RESULT_HEIGHT, 0.0f);
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
	g_pVtxBuffResult->Unlock();
	//カウントリセット
	g_nCounterGameClear = 0;

	InitResultLogo();

}
//=============================================================================
// 終了処理
//=============================================================================
void UninitResult(void)
{	//テクスチャの破棄
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}
	//頂点バッファの破棄の破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}

	UninitResultLogo();
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateResult(void)
{
	JoyState pJoyState = GetJoystate();
	XINPUT_STATE state;
	XInputGetState(0, &state);

	//エンターキー
	if (GetKeyboardTrigger(DIK_RETURN) == true || (state.Gamepad.wButtons & XINPUT_GAMEPAD_B  && pJoyState.bConnectionJoypad == true) || (state.Gamepad.wButtons & XINPUT_GAMEPAD_A  && pJoyState.bConnectionJoypad == true)
		|| (state.Gamepad.wButtons & XINPUT_GAMEPAD_X  && pJoyState.bConnectionJoypad == true) || (state.Gamepad.wButtons & XINPUT_GAMEPAD_Y  && pJoyState.bConnectionJoypad == true))
	{
		if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_NOTPUSH)
		{
			SetFade(MODE_RANKING);
		}

		g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_PUSH;
	}
	g_nCounterGameClear++;
	if (g_nCounterGameClear >= 600)
	{
		SetFade(MODE_RANKING);
	}

	//ジョイパッドの状態更新
	if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_PUSH)
	{
		g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NONE;
	}
	else if (g_JoyStateResult.nJoypadState == INPUT_JOYSTATE_NONE)
	{
		g_JoyStateResult.nJoypadState = INPUT_JOYSTATE_NOTPUSH;
	}
	UpdateResultLogo();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResult);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//開始する頂点のインデックス
		2); //描画するプリミティブ数

	//DrawResultLogo();

}