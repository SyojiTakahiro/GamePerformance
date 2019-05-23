//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : Takahiro Syoji
//
//=============================================================================
#include "bg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_TEXTURENAME		"data/TEXTURE/mountain000.png"			//背景画像
#define BG_POS_X			(5500)					//X座標
#define BG_POS_Y			(650)					//Y座標
#define BG_WIDTH			(100)		//画面の幅
#define BG_HEIGHT			(100)		//画面の高さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBG = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;

//=============================================================================
// メイン関数
//=============================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME, &g_pTextureBG);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	VERTEX_2D*pVtx;

	//頂点バッファをロック
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_POS_X + BG_WIDTH, BG_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(BG_POS_X, BG_POS_Y + BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_POS_X + BG_WIDTH, BG_POS_Y + BG_HEIGHT, 0.0f);

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
	g_pVtxBuffBG->Unlock();
}

void UninitBG(void)
{
	//テクスチャの破棄
	if (g_pTextureBG != NULL)
	{
		g_pTextureBG->Release();
		g_pTextureBG = NULL;
	}
	//頂点の破棄
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
}

void UpdateBG(void)
{

}

void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_pTextureBG);

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}