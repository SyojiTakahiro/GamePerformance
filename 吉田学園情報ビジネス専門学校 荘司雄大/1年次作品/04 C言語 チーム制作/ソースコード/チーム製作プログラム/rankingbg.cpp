//=============================================================================
// 授業作成
// 背景の処理 [rankingbg.cpp]
// Author : Mikiya Meguro
//
//=============================================================================
#include "main.h"
#include "rankingbg.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_TEXTURENAME		"data\\TEXTURE\\Ranking.jpg"//テクスチャのファイル名
#define POS_X				(0)							//左上座標X
#define POS_Y				(0)							//左上座標Y	
#define BG_WIDTH			(SCREEN_WIDTH)				//正方形の横幅座標
#define BG_HEIGHT			(SCREEN_HEIGHT)				//正方形の縦幅座標
#define COUNTER_ANIM_RESET	(200)						//アニメーションリセットカウント
#define TEX_POS_X_INIT		(1.0f)						//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT		(0.005f)					//テクスチャ座標Vの初期位置
#define TEX_LEFT			(0.0f)						//テクスチャ座標U左
#define TEX_RIGHT			(1.0f)						//テクスチャ座標U右
#define TEX_TOP				(0.0f)						//テクスチャ座標V上
#define TEX_BOT				(1.0f)						//テクスチャ座標V下

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureRankingBG = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingBG = NULL;	//頂点バッファへのポインタ
//*****************************************************************************
// ポリゴン
//*****************************************************************************
void InitRankingBG(void)
{	//ローカル変数
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		BG_TEXTURENAME,
		&g_pTextureRankingBG);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingBG,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffRankingBG->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(POS_X, POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(BG_WIDTH, POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(POS_X, BG_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(BG_WIDTH, BG_HEIGHT, 0.0f);
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
	g_pVtxBuffRankingBG->Unlock();
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitRankingBG(void)
{	//テクスチャの破棄
	if (g_pTextureRankingBG != NULL)
	{
		g_pTextureRankingBG->Release();
		g_pTextureRankingBG = NULL;
	}
	//頂点バッファの破棄の破棄
	if (g_pVtxBuffRankingBG != NULL)
	{
		g_pVtxBuffRankingBG->Release();
		g_pVtxBuffRankingBG = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateRankingBG(void)
{
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawRankingBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankingBG, 0, sizeof(VERTEX_2D));
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRankingBG);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//開始する頂点のインデックス
		2); //描画するプリミティブ数
}