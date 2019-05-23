//=============================================================================
//
// タイトルロゴの処理 [titlelogo.cpp]
// Author : Jukiya Hayakawa
//
//=============================================================================
#include "titlelogo.h"
#include "main.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_TITLELOGO		"data/TEXTURE/titlelogo.png"			// 読み込むテクスチャファイル名
#define TITLELOGO_WIDTH			(250)									// ゲージの幅
#define TITLELOGO_HEIGHT		(250)									// ゲージの高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexTitleLogo(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureTitleLogo = NULL;					// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;					// 頂点バッファへのポインタ
TitleLogo				g_titleLogo;								// ゲージの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_titleLogo.pos = D3DXVECTOR3(SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 - 100 , 0.0f);	//位置の初期化
	g_titleLogo.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);												//色の初期化
	g_titleLogo.fWidth = TITLELOGO_WIDTH;																//幅の初期化
	g_titleLogo.fHeight = TITLELOGO_HEIGHT;																//高さの初期化

	// 頂点情報の設定
	MakeVertexTitleLogo(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTitleLogo(void)
{
	// テクスチャの開放
	if (g_pTextureTitleLogo != NULL)
	{
		g_pTextureTitleLogo->Release();
		g_pTextureTitleLogo = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitleLogo(void)
{

	//if (g_titleLogo.col.a <= 1.0f)
	//{
	//	g_titleLogo.col.a += 0.005f;		//加算処理
	//}
	//else if (g_titleLogo.col.a >= 1.0f)
	//{
	//	g_titleLogo.col.a = 1.0f;
	//}
	//VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	//// 頂点バッファをロックし,頂点データへのポインタを取得
	//g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	//// 頂点座標
	//pVtx[0].pos = D3DXVECTOR3(g_titleLogo.pos.x - g_titleLogo.fWidth, g_titleLogo.pos.y - g_titleLogo.fHeight, g_titleLogo.pos.z);
	//pVtx[1].pos = D3DXVECTOR3(g_titleLogo.pos.x + g_titleLogo.fWidth, g_titleLogo.pos.y - g_titleLogo.fHeight, g_titleLogo.pos.z);
	//pVtx[2].pos = D3DXVECTOR3(g_titleLogo.pos.x - g_titleLogo.fWidth, g_titleLogo.pos.y + g_titleLogo.fHeight, g_titleLogo.pos.z);
	//pVtx[3].pos = D3DXVECTOR3(g_titleLogo.pos.x + g_titleLogo.fWidth, g_titleLogo.pos.y + g_titleLogo.fHeight, g_titleLogo.pos.z);

	//// 頂点カラー
	//pVtx[0].col = g_titleLogo.col;
	//pVtx[1].col = g_titleLogo.col;
	//pVtx[2].col = g_titleLogo.col;
	//pVtx[3].col = g_titleLogo.col;

	//// 頂点バッファをアンロックする
	//g_pVtxBuffTitleLogo->Unlock();
}
//=============================================================================
// タイトル画面
//=============================================================================
void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTitleLogo);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexTitleLogo(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_TITLELOGO,
		&g_pTextureTitleLogo);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITlELOGO,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_titleLogo.pos.x - g_titleLogo.fWidth, g_titleLogo.pos.y - g_titleLogo.fHeight, g_titleLogo.pos.z);
	pVtx[1].pos = D3DXVECTOR3(g_titleLogo.pos.x + g_titleLogo.fWidth, g_titleLogo.pos.y - g_titleLogo.fHeight, g_titleLogo.pos.z);
	pVtx[2].pos = D3DXVECTOR3(g_titleLogo.pos.x - g_titleLogo.fWidth, g_titleLogo.pos.y + g_titleLogo.fHeight, g_titleLogo.pos.z);
	pVtx[3].pos = D3DXVECTOR3(g_titleLogo.pos.x + g_titleLogo.fWidth, g_titleLogo.pos.y + g_titleLogo.fHeight, g_titleLogo.pos.z);

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = g_titleLogo.col;
	pVtx[1].col = g_titleLogo.col;
	pVtx[2].col = g_titleLogo.col;
	pVtx[3].col = g_titleLogo.col;

	// 頂点バッファをアンロックする
	g_pVtxBuffTitleLogo->Unlock();
}