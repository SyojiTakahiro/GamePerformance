//=============================================================================
//
// チュートリアル [t.cpp]
// Author :Takahiro
//
//=============================================================================
#include "title.h"
#include "t.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_TEXTURENAME		"data/TEXTURE/bg1100.jpg"			//背景画像
#define TITLE_POS_X			(0)					//X座標
#define TITLE_POS_Y			(0)					//Y座標
#define TITLE_WIDTH			(SCREEN_WIDTH)		//画面の幅
#define TITLE_HEIGHT		(SCREEN_HEIGHT)		//画面の高さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureT = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffT = NULL;

//=============================================================================
// メイン関数
//=============================================================================
void InitT(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAME, &g_pTextureT);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffT,
		NULL);

	VERTEX_2D*pVtx;

	//頂点バッファをロック
	g_pVtxBuffT->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, TITLE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, TITLE_POS_Y + TITLE_HEIGHT, 0.0f);

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
	g_pVtxBuffT->Unlock();
}

void UninitT(void)
{
	//テクスチャの破棄
	if (g_pTextureT != NULL)
	{
		g_pTextureT->Release();
		g_pTextureT = NULL;
	}
	//頂点の破棄
	if (g_pVtxBuffT != NULL)
	{
		g_pVtxBuffT->Release();
		g_pVtxBuffT = NULL;
	}
}

void UpdateT(void)
{
	FADE fade;
	fade = GetFade();
	if (GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE)
	{
		SetFade(MODE_GAME);
		PlaySound(SOUND_LABEL_SE_DECIDE);
	}
}

void DrawT(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	pDevice->SetStreamSource(0, g_pVtxBuffT, 0, sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_pTextureT);

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}