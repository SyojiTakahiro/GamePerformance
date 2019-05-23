//=============================================================================
//
// 背景処理 [bg.cpp]
// Author : Takahiro Syoji
//
//=============================================================================
#include "bg2.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_TEXTURENAME		"data/TEXTURE/bg1000.png"			//背景画像
#define BG_POS_X			(0)					//X座標
#define BG_POS_Y			(0)					//Y座標
#define BG_WIDTH			(SCREEN_WIDTH)		//画面の幅
#define BG_HEIGHT			(SCREEN_HEIGHT)		//画面の高さ
#define BG_SPEED			(0.00005f)

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR2 pos;	//位置
	D3DXVECTOR2 move;	//移動量
	bool bUse;			//使用しているかどうか
}BG2;

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBG2 = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG2 = NULL;

int g_nCntAnim;
int g_nPtnAnim;
//=============================================================================
// メイン関数
//=============================================================================
void InitBG2(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURENAME, &g_pTextureBG2);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG2,
		NULL);

	g_nCntAnim = 0;
	g_nPtnAnim = 0;

	VERTEX_2D*pVtx;

	//頂点バッファをロック
	g_pVtxBuffBG2->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffBG2->Unlock();
}

void UninitBG2(void)
{
	//テクスチャの破棄
	if (g_pTextureBG2 != NULL)
	{
		g_pTextureBG2->Release();
		g_pTextureBG2 = NULL;
	}
	//頂点の破棄
	if (g_pVtxBuffBG2 != NULL)
	{
		g_pVtxBuffBG2->Release();
		g_pVtxBuffBG2 = NULL;
	}
}

void UpdateBG2(void)
{
	g_nCntAnim++;
	VERTEX_2D*pVtx;

	//頂点バッファをロック
	g_pVtxBuffBG2->Lock(0, 0, (void**)&pVtx, 0);


	pVtx[0].tex = D3DXVECTOR2(0.0f + (g_nCntAnim * BG_SPEED), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.25f + (g_nCntAnim * BG_SPEED), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (g_nCntAnim * BG_SPEED), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.25f + (g_nCntAnim * BG_SPEED), 1.0f);

	//アンロック
	g_pVtxBuffBG2->Unlock();
}

void DrawBG2(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	pDevice->SetStreamSource(0, g_pVtxBuffBG2, 0, sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_pTextureBG2);

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}