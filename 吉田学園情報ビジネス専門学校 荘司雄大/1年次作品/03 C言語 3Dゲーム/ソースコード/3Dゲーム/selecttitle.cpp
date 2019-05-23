//=============================================================================
// 授業作成
// 背景の処理 [title.cpp]
// Author :
//
//=============================================================================
#include "main.h"
#include "selecttitle.h"
#include "input.h"
#include "modeselect.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BG_TEXTURENAME		"data\\TEXTURE\\block000.jpg"	//テクスチャのファイル名
#define POS_X				(500)							//左上座標X
#define POS_Y				(400)							//左上座標Y	
#define BG_WIDTH			(800)				//正方形の横幅座標
#define BG_HEIGHT			(700)				//正方形の縦幅座標
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
LPDIRECT3DTEXTURE9 g_pTextureSelectTitle = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSelectTitle = NULL;	//頂点バッファへのポインタ
bool bSelectDraw;
//*****************************************************************************
// ポリゴン
//*****************************************************************************
void InitSelectTitle(void)
{	//ローカル変数
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		BG_TEXTURENAME,
		&g_pTextureSelectTitle);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSelectTitle,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffSelectTitle->Lock(0, 0, (void**)&pVtx, 0);
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
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffSelectTitle->Unlock();

	InitModeSelect();

	bSelectDraw = true;
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitSelectTitle(void)
{	//テクスチャの破棄
	if (g_pTextureSelectTitle != NULL)
	{
		g_pTextureSelectTitle->Release();
		g_pTextureSelectTitle = NULL;
	}
	//頂点バッファの破棄の破棄
	if (g_pVtxBuffSelectTitle != NULL)
	{
		g_pVtxBuffSelectTitle->Release();
		g_pVtxBuffSelectTitle = NULL;
	}

	UninitModeSelect();
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateSelectTitle(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		bSelectDraw = true;
	}
	UpdateModeSelect();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawSelectTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスを取得する
	pDevice = GetDevice();
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSelectTitle, 0, sizeof(VERTEX_2D));
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureSelectTitle);
	if (bSelectDraw == true)
	{
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0,	//開始する頂点のインデックス
			2); //描画するプリミティブ数
		DrawModeSelect();
	}
}