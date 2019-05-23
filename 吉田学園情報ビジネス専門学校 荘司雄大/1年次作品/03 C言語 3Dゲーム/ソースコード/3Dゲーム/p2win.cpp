//=============================================================================
//
// P2Win [P2Win.cpp]
// Author :Takahiro
//
//=============================================================================
#include "P2Win.h"
#include "p2winlogo.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "meshField.h"
#include "building.h"
#include "player2.h"
#include "model.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define RESULT_TEXTURENAME		"data/TEXTURE/GC.jpg"			//背景画像
#define RESULT_POS_X			(0)					//X座標
#define RESULT_POS_Y			(0)					//Y座標
#define RESULT_WIDTH			(SCREEN_WIDTH)		//画面の幅
#define RESULT_HEIGHT			(SCREEN_HEIGHT)		//画面の高さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureP2Win = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffP2Win = NULL;

//=============================================================================
// メイン関数
//=============================================================================
void InitP2Win(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, RESULT_TEXTURENAME, &g_pTextureP2Win);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffP2Win,
		NULL);

	VERTEX_2D*pVtx;

	//頂点バッファをロック
	g_pVtxBuffP2Win->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(RESULT_POS_X + RESULT_WIDTH, RESULT_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(RESULT_POS_X, RESULT_POS_Y + RESULT_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(RESULT_POS_X + RESULT_WIDTH, RESULT_POS_Y + RESULT_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0, 0);
	pVtx[1].tex = D3DXVECTOR2(1, 0);
	pVtx[2].tex = D3DXVECTOR2(0, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 1);

	//アンロック
	g_pVtxBuffP2Win->Unlock();

	// カメラの初期化処理
	InitCamera();
	// ライトの初期化処理
	InitLight();

	InitBuilding();

	InitModel();

	SetModel(D3DXVECTOR3(250, 0, -160), 1);

	InitMeshField();

	InitP2winLogo();

	PlaySound(SOUND_LABEL_BGM002);
}

void UninitP2Win(void)
{
	//テクスチャの破棄
	if (g_pTextureP2Win != NULL)
	{
		g_pTextureP2Win->Release();
		g_pTextureP2Win = NULL;
	}
	//頂点の破棄
	if (g_pVtxBuffP2Win != NULL)
	{
		g_pVtxBuffP2Win->Release();
		g_pVtxBuffP2Win = NULL;
	}

	// カメラの終了処理
	UninitCamera();
	// ライトの終了処理
	UninitLight();

	UninitBuilding();

	UninitModel();

	UninitMeshField();

	UninitP2winLogo();
}

void UpdateP2Win(void)
{
	FADE fade;
	fade = GetFade();

	if (GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE)
	{
		StopSound(SOUND_LABEL_BGM002);
		SetFade(MODE_TITLE);
	}
	// カメラの更新処理
	UpdateCamera();
	// ライトの更新処理
	UpdateLight();

	UpdateBuilding();

	UpdateModel();

	UpdateMeshField();

	UpdateP2winLogo();
}

void DrawP2Win(void)
{
	// カメラの設定
	SetCamera();

	////デバイスの取得
	//pDevice = GetDevice();

	//pDevice->SetStreamSource(0, g_pVtxBuffP2Win, 0, sizeof(VERTEX_2D));

	//pDevice->SetTexture(0, g_pTextureP2Win);

	//pDevice->SetFVF(FVF_VERTEX_2D);
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	DrawBuilding();

	DrawModel();

	DrawMeshField();

	DrawP2winLogo();
}