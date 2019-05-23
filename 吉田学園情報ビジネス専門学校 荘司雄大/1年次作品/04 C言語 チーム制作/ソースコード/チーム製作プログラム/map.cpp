//=============================================================================
// 
// マップの処理 [map.cpp]
// Author : Mikiya Meguro
//
//=============================================================================
#include "Map.h"
#include "input.h"
#include "mapPlayer.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAP_TEXTURENAME		"data\\TEXTURE\\MapField001.jpg"	//テクスチャのファイル名
#define POS_X				(0)							//左上座標X
#define POS_Y				(420)							//左上座標Y	
#define BG_WIDTH			(300)						//正方形の横幅座標
#define BG_HEIGHT			(720)						//正方形の縦幅座標

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMap = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMap = NULL;	//頂点バッファへのポインタ
//*****************************************************************************
// マップ
//*****************************************************************************
void InitMap(void)
{	
	//ローカル変数
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		MAP_TEXTURENAME,
		&g_pTextureMap);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMap,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffMap->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(POS_X, POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(POS_X + BG_WIDTH, POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(POS_X, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(POS_X + BG_WIDTH, BG_HEIGHT, 0.0f);
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
	g_pVtxBuffMap->Unlock();	

	//マッププレイヤー初期化処理
	InitMapPlayer();
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitMap(void)
{	//テクスチャの破棄
	if (g_pTextureMap != NULL)
	{
		g_pTextureMap->Release();
		g_pTextureMap = NULL;
	}
	//頂点バッファの破棄の破棄
	if (g_pVtxBuffMap != NULL)
	{
		g_pVtxBuffMap->Release();
		g_pVtxBuffMap = NULL;
	}
	//マッププレイヤー終了処理
	UninitMapPlayer();

}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateMap(void)
{
	//マッププレイヤー初期化処理
	UpdateMapPlayer();

}
//=============================================================================
// 描画処理
//=============================================================================
void DrawMap(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスを取得する
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMap, 0, sizeof(VERTEX_2D));
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMap);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//開始する頂点のインデックス
		2); //描画するプリミティブ数

	//マッププレイヤー描画処理
	DrawMapPlayer();

}
