//=============================================================================
// 
// リザルトの処理 [fadelogo.cpp]
// Author : Mikiya Meguro
//
//=============================================================================
#include "fadelogo.h"
#include "input.h"
#include "fade.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_TEXTURENAME		"data\\TEXTURE\\NowLoading.png"	//テクスチャのファイル名
#define RESULT_POS_X				(0)							//左上座標X
#define RESULT_POS_Y				(0)							//左上座標Y	
#define RESULT_WIDTH			(SCREEN_WIDTH)				//正方形の横幅座標
#define RESULT_HEIGHT			(SCREEN_HEIGHT)				//正方形の縦幅座標

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureFadeLogo = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFadeLogo = NULL;	//頂点バッファへのポインタ
D3DXCOLOR				g_colorFadeLogo;			// フェード色
//*****************************************************************************
// 初期化
//*****************************************************************************
void InitFadeLogo(void)
{	//ローカル変数	
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;

	pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TITLE_TEXTURENAME,
		&g_pTextureFadeLogo);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFadeLogo,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffFadeLogo->Lock(0, 0, (void**)&pVtx, 0);
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
	g_pVtxBuffFadeLogo->Unlock();
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitFadeLogo(void)
{	//テクスチャの破棄
	if (g_pTextureFadeLogo != NULL)
	{
		g_pTextureFadeLogo->Release();
		g_pTextureFadeLogo = NULL;
	}
	//頂点バッファの破棄の破棄
	if (g_pVtxBuffFadeLogo != NULL)
	{
		g_pVtxBuffFadeLogo->Release();
		g_pVtxBuffFadeLogo = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateFadeLogo(void)
{
	FADE fade = GetFade();
	if (fade != FADE_NONE)
	{//何もしてない状態
		if (fade == FADE_IN)
		{//フェードイン/
			g_colorFadeLogo.a -= 0.02f;	//画面を透明にしていく
			if (g_colorFadeLogo.a <= 0.0f)
			{
				g_colorFadeLogo.a = 0.0f;	//値を入れなおして初期化
				fade = FADE_NONE;	//何もしてない状態
			}
		}
		else if (fade == FADE_OUT)
		{//フェードアウト状態
			g_colorFadeLogo.r = 1.0f;
			g_colorFadeLogo.g = 1.0f;
			g_colorFadeLogo.b = 1.0f;
			g_colorFadeLogo.a += 0.02f;		//画面を不透明に
			if (g_colorFadeLogo.a >= 1.0f)
			{
				g_colorFadeLogo.a = 1.0f;	//値を入れなおして初期化
			}
		}
		VERTEX_2D *pVtx;								//頂点情報へのポインタ	
		g_pVtxBuffFadeLogo->Lock(0, 0, (void**)&pVtx, 0);	//頂点バッファをロックし頂点データのポインタを取得	
		//頂点カラー
		pVtx[0].col = g_colorFadeLogo;
		pVtx[1].col = g_colorFadeLogo;
		pVtx[2].col = g_colorFadeLogo;
		pVtx[3].col = g_colorFadeLogo;
		//頂点バッファをアンロックする
		g_pVtxBuffFadeLogo->Unlock();
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawFadeLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFadeLogo, 0, sizeof(VERTEX_2D));
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFadeLogo);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//開始する頂点のインデックス
		2); //描画するプリミティブ数
}