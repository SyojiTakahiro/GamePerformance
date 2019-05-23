//=============================================================================
// 背景の処理 [pressenter.cpp]
// Author : Takahiro
//=============================================================================
#include "main.h"
#include "pressenter.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_TEXTURENAME		"data/TEXTURE/press_enter.png"	//テクスチャのファイル名
#define POS_X				(450)							//左上座標X
#define POS_Y				(450)							//左上座標Y	
#define BG_WIDTH			(800)				//正方形の横幅座標
#define BG_HEIGHT			(510)				//正方形の縦幅座標
#define TEX_POS_X_INIT		(1.0f)						//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT		(0.005f)					//テクスチャ座標Vの初期位置
#define TEX_LEFT			(0.0f)						//テクスチャ座標U左
#define TEX_RIGHT			(1.0f)						//テクスチャ座標U右
#define TEX_TOP				(0.0f)						//テクスチャ座標V上
#define TEX_BOT				(1.0f)						//テクスチャ座標V下

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePressEnter = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPressEnter = NULL;	//頂点バッファへのポインタ
int						g_PressEnterCnt = 0;
float					g_PressEnterChange = 0;
//*****************************************************************************
// ポリゴン
//*****************************************************************************
void InitPressEnter(void)
{	//ローカル変数
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		BG_TEXTURENAME,
		&g_pTexturePressEnter);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPressEnter,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(POS_X, POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH, POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(POS_X, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(TEX_LEFT, TEX_TOP);
	pVtx[1].tex = D3DXVECTOR2(TEX_RIGHT, TEX_TOP);
	pVtx[2].tex = D3DXVECTOR2(TEX_LEFT, TEX_BOT);
	pVtx[3].tex = D3DXVECTOR2(TEX_RIGHT, TEX_BOT);
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
	g_pVtxBuffPressEnter->Unlock();
	//数値初期化
	g_PressEnterCnt = 0;
	g_PressEnterChange = 0;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitPressEnter(void)
{	//テクスチャの破棄
	if (g_pTexturePressEnter != NULL)
	{
		g_pTexturePressEnter->Release();
		g_pTexturePressEnter = NULL;
	}
	//頂点バッファの破棄の破棄
	if (g_pVtxBuffPressEnter != NULL)
	{
		g_pVtxBuffPressEnter->Release();
		g_pVtxBuffPressEnter = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdatePressEnter(void)
{
	g_PressEnterCnt++;

	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);
	
	if (g_PressEnterCnt >= 40)
	{
		if (g_PressEnterChange == 0)
		{
			g_PressEnterChange = 1;
		}
		else if (g_PressEnterChange == 1)
		{
			g_PressEnterChange = 0;
		}
		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_PressEnterChange);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_PressEnterChange);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_PressEnterChange);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_PressEnterChange);

		g_PressEnterCnt = 0;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPressEnter->Unlock();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawPressEnter(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPressEnter, 0, sizeof(VERTEX_2D));
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePressEnter);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//開始する頂点のインデックス
		2); //描画するプリミティブ数
}