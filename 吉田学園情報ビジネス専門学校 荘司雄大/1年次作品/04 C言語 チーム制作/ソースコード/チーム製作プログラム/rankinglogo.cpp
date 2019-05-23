//=============================================================================
// 授業作成
// 背景の処理 [rankingbg.cpp]
// Author : Mikiya Meguro
//
//=============================================================================
#include "main.h"
#include "rankinglogo.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_TEXTURENAME		"data\\TEXTURE\\ranking_logo.png"//テクスチャのファイル名
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
#define RANKINGLOGO_POS_X		(400)			//頂点座標Xの移動量
#define RANKINGLOGO_POS_Y		(50)			//頂点座標Yの移動量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		 g_pTextureRankingLogo = NULL;					//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	 g_pVtxBuffRankingLogo = NULL;					//頂点バッファへのポインタ
D3DXVECTOR3				 g_Rankingpos;//位置
D3DXVECTOR3				 g_Rankingmove;//移動量
float					 g_fMoveRankingY = 1;
//*****************************************************************************
// ポリゴン
//*****************************************************************************
void InitRankingLogo(void)
{	//ローカル変数
	g_Rankingpos = D3DXVECTOR3(640.0f, 60.0f, 0.0f); //位置
	g_Rankingmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 //位置
	g_fMoveRankingY = 1;
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		BG_TEXTURENAME,
		&g_pTextureRankingLogo);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingLogo,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffRankingLogo->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_Rankingpos.x - RANKINGLOGO_POS_X, g_Rankingpos.y - RANKINGLOGO_POS_Y, g_Rankingpos.z);
	pVtx[1].pos = D3DXVECTOR3(g_Rankingpos.x + RANKINGLOGO_POS_X, g_Rankingpos.y - RANKINGLOGO_POS_Y, g_Rankingpos.z);
	pVtx[2].pos = D3DXVECTOR3(g_Rankingpos.x - RANKINGLOGO_POS_X, g_Rankingpos.y + RANKINGLOGO_POS_Y, g_Rankingpos.z);
	pVtx[3].pos = D3DXVECTOR3(g_Rankingpos.x + RANKINGLOGO_POS_X, g_Rankingpos.y + RANKINGLOGO_POS_Y, g_Rankingpos.z);
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
	g_pVtxBuffRankingLogo->Unlock();
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitRankingLogo(void)
{	//テクスチャの破棄
	if (g_pTextureRankingLogo != NULL)
	{
		g_pTextureRankingLogo->Release();
		g_pTextureRankingLogo = NULL;
	}
	//頂点バッファの破棄の破棄
	if (g_pVtxBuffRankingLogo != NULL)
	{
		g_pVtxBuffRankingLogo->Release();
		g_pVtxBuffRankingLogo = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateRankingLogo(void)
{

	//if (g_Rankingpos.y >= 220.0f)
	//{
	//	g_fMoveRankingY = 0;
	//}

	////位置更新
	//g_Rankingmove.y = 2 * g_fMoveRankingY;
	//g_Rankingpos.y += g_Rankingmove.y;

	////位置の更新
	//VERTEX_2D *pVtx;	//頂点情報へのポインタ
	////頂点バッファをロックし頂点データのポインタを取得
	//g_pVtxBuffRankingLogo->Lock(0, 0, (void**)&pVtx, 0);
	////頂点座標
	//pVtx[0].pos = D3DXVECTOR3(g_Rankingpos.x - RANKINGLOGO_POS_X, g_Rankingpos.y - RANKINGLOGO_POS_Y, g_Rankingpos.z);
	//pVtx[1].pos = D3DXVECTOR3(g_Rankingpos.x + RANKINGLOGO_POS_X, g_Rankingpos.y - RANKINGLOGO_POS_Y, g_Rankingpos.z);
	//pVtx[2].pos = D3DXVECTOR3(g_Rankingpos.x - RANKINGLOGO_POS_X, g_Rankingpos.y + RANKINGLOGO_POS_Y, g_Rankingpos.z);
	//pVtx[3].pos = D3DXVECTOR3(g_Rankingpos.x + RANKINGLOGO_POS_X, g_Rankingpos.y + RANKINGLOGO_POS_Y, g_Rankingpos.z);
	////頂点バッファをアンロックする
	//g_pVtxBuffRankingLogo->Unlock();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawRankingLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankingLogo, 0, sizeof(VERTEX_2D));
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRankingLogo);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//開始する頂点のインデックス
		2); //描画するプリミティブ数
}
//=============================================================================
// タイトルロゴ取得処理
//=============================================================================
D3DXVECTOR3 *GetRankingLogo(void)
{
	return &g_Rankingpos;
}
//=============================================================================
// タイトルロゴのセット
//=============================================================================
void SetRankingLogo(D3DXVECTOR3 nRankingLogo)
{
	g_Rankingpos = nRankingLogo;
}