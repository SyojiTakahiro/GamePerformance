//=============================================================================
// 
// タイムフレームの処理 [TimeFrame.cpp]
// Author : Mikiya Meguro
//
//=============================================================================
#include "main.h"
#include "timeframe.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TIME_TEXTURENAME	"data\\TEXTURE\\frame_timer.png"	//テクスチャのファイル名
#define TF_X				(540)								//左上座標X
#define TF_Y				(5)									//左上座標Y	
#define TF_WIDTH			(740)								//正方形の横幅座標
#define TF_HEIGHT			(80)								//正方形の縦幅座標
#define TEX_LEFT			(0.0f)								//テクスチャ座標U左
#define TEX_RIGHT			(1.0f)								//テクスチャ座標U右
#define TEX_TOP				(0.0f)								//テクスチャ座標V上
#define TEX_BOT				(1.0f)								//テクスチャ座標V下

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTimeFrame = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimeFrame = NULL;	//頂点バッファへのポインタ
//*****************************************************************************
// 初期化
//*****************************************************************************
void InitTimeFrame(void)
{	//ローカル変数
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TIME_TEXTURENAME,
		&g_pTextureTimeFrame);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeFrame,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffTimeFrame->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(TF_X, TF_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TF_WIDTH, TF_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TF_X, TF_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TF_WIDTH, TF_HEIGHT, 0.0f);
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
	g_pVtxBuffTimeFrame->Unlock();
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitTimeFrame(void)
{	//テクスチャの破棄
	if (g_pTextureTimeFrame != NULL)
	{
		g_pTextureTimeFrame->Release();
		g_pTextureTimeFrame = NULL;
	}
	//頂点バッファの破棄の破棄
	if (g_pVtxBuffTimeFrame != NULL)
	{
		g_pVtxBuffTimeFrame->Release();
		g_pVtxBuffTimeFrame = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateTimeFrame(void)
{
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawTimeFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimeFrame, 0, sizeof(VERTEX_2D));
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTimeFrame);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//開始する頂点のインデックス
		2); //描画するプリミティブ数
}