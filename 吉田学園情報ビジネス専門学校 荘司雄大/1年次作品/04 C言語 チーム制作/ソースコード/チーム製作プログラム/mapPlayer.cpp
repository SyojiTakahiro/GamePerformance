//=============================================================================
// 
// マッププレイヤープレイヤーの処理 [mapplayer.cpp]
// Author : Mikiya Meguro
//
//=============================================================================
#include "mapplayer.h"
#include "input.h"
#include "player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAPPLAYER_TEXTURENAME		"data\\TEXTURE\\ぽっくる.png"	//テクスチャのファイル名
#define POS_X				(150)						//左上座標X
#define POS_Y				(80)						//左上座標Y	
#define MAPPLAYER_WIDTH		(30)						//正方形の横幅座標
#define MAPPLAYER_HEIGHT	(30)						//正方形の縦幅座標
#define MINIMAP				(150)
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureMapPlayer = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMapPlayer = NULL;	//頂点バッファへのポインタ
MAPPLAYER	g_MapPlayer;
//*****************************************************************************
// マッププレイヤー
//*****************************************************************************
void InitMapPlayer(void)
{	
	//ローカル変数
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	PLAYER *pPlayer = GetPlayer();
	D3DXVECTOR3 mapPlayer = pPlayer->pos / 10;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		MAPPLAYER_TEXTURENAME,
		&g_pTextureMapPlayer);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMapPlayer,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffMapPlayer->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(mapPlayer.x /*+ MAPPLAYER_WIDTH*/, mapPlayer.z, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(mapPlayer.x /*+ MAPPLAYER_WIDTH*/ + MAPPLAYER_WIDTH, mapPlayer.z, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(mapPlayer.x /*+ MAPPLAYER_WIDTH*/, mapPlayer.z + MAPPLAYER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(mapPlayer.x /*+ MAPPLAYER_WIDTH*/ + MAPPLAYER_WIDTH, mapPlayer.z + MAPPLAYER_HEIGHT, 0.0f);
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
	g_pVtxBuffMapPlayer->Unlock();	

	g_MapPlayer.nMapBorder = 1;
	g_MapPlayer.move = D3DXVECTOR3(0.0f,0.0f,0.0f);
	g_MapPlayer.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_MapPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_MapPlayer.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitMapPlayer(void)
{	//テクスチャの破棄
	if (g_pTextureMapPlayer != NULL)
	{
		g_pTextureMapPlayer->Release();
		g_pTextureMapPlayer = NULL;
	}
	//頂点バッファの破棄の破棄
	if (g_pVtxBuffMapPlayer != NULL)
	{
		g_pVtxBuffMapPlayer->Release();
		g_pVtxBuffMapPlayer = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateMapPlayer(void)
{
	PLAYER *pPlayer = GetPlayer();

	D3DXVECTOR3 mapPlayer = pPlayer->pos / 20;
	//マップの位置分下に
	mapPlayer.z -= 420;

	//フィールドの大きさの判定
	if (MINIMAP - MAPPLAYER_WIDTH / 2 <= mapPlayer.x )
	{
		g_MapPlayer.nMapBorder += 1;
		mapPlayer.x += (-MINIMAP + MAPPLAYER_WIDTH / 2) * 2;
	}
	if (-MINIMAP + MAPPLAYER_WIDTH / 2 >= mapPlayer.x)
	{
		g_MapPlayer.nMapBorder -= 1;
		mapPlayer.x += (MINIMAP + MAPPLAYER_WIDTH / 2) * 2;
	}
	//if (-MINIMAP + MAPPLAYER_HEIGHT / 2 >= mapPlayer.z)
	//{
	//	mapPlayer.z = -MINIMAP + MAPPLAYER_HEIGHT / 2;
	//}
	//if (MINIMAP - MAPPLAYER_HEIGHT / 2 <= mapPlayer.z)
	//{
	//	mapPlayer.z = MINIMAP - MAPPLAYER_HEIGHT / 2;
	//}

	VERTEX_2D *pVtx;//頂点情報へのポインタ
	
	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffMapPlayer->Lock(0, 0, (void**)&pVtx, 0);
	
	////頂点座標
	//pVtx[0].pos = D3DXVECTOR3(POS_X + mapPlayer.x, POS_Y - mapPlayer.z, 0.0f);
	//pVtx[1].pos = D3DXVECTOR3(POS_X + MAPPLAYER_WIDTH + mapPlayer.x, POS_Y - mapPlayer.z, 0.0f);
	//pVtx[2].pos = D3DXVECTOR3(POS_X + mapPlayer.x, MAPPLAYER_HEIGHT - mapPlayer.z, 0.0f);
	//pVtx[3].pos = D3DXVECTOR3(POS_X + MAPPLAYER_WIDTH + mapPlayer.x, MAPPLAYER_HEIGHT - mapPlayer.z, 0.0f);
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(mapPlayer.x - MAPPLAYER_WIDTH + 150, -mapPlayer.z - MAPPLAYER_HEIGHT + 150, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(mapPlayer.x + MAPPLAYER_WIDTH + 150, -mapPlayer.z - MAPPLAYER_HEIGHT + 150, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(mapPlayer.x - MAPPLAYER_WIDTH + 150, -mapPlayer.z + MAPPLAYER_HEIGHT + 150, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(mapPlayer.x + MAPPLAYER_WIDTH + 150, -mapPlayer.z + MAPPLAYER_HEIGHT + 150, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffMapPlayer->Unlock();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawMapPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスを取得する
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMapPlayer, 0, sizeof(VERTEX_2D));
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMapPlayer);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//開始する頂点のインデックス
		2); //描画するプリミティブ数
}
