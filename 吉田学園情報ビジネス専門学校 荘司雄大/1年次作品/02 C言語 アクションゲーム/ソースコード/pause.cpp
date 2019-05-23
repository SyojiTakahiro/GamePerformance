//=============================================================================
//
// ポーズ処理 [bg.cpp]
// Author : Takahiro Syoji
//
//=============================================================================
#include "pause.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PAUSE_TEXTURENAME	"data/TEXTURE/bg400.png"	//背景画像
#define PAUSE_POS_X			(0)					//X座標
#define PAUSE_POS_Y			(0)					//Y座標
#define PAUSE_WIDTH			(SCREEN_WIDTH)		//画面の幅
#define PAUSE_HEIGHT		(SCREEN_HEIGHT)		//画面の高さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTexturePause = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;

//=============================================================================
// メイン関数
//=============================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PAUSE_TEXTURENAME, &g_pTexturePause);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	VERTEX_2D*pVtx;

	//頂点バッファをロック
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(PAUSE_POS_X + PAUSE_WIDTH, PAUSE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(PAUSE_POS_X, PAUSE_POS_Y + PAUSE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(PAUSE_POS_X + PAUSE_WIDTH, PAUSE_POS_Y + PAUSE_HEIGHT, 0.0f);

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
	g_pVtxBuffPause->Unlock();
}

void UninitPause(void)
{
	//テクスチャの破棄
	if (g_pTexturePause != NULL)
	{
		g_pTexturePause->Release();
		g_pTexturePause = NULL;
	}
	//頂点の破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

void UpdatePause(void)
{

}

void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	pDevice->SetTexture(0, g_pTexturePause);

	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}