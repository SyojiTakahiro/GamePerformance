//=============================================================================
//
// フェード処理 [fade.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "fade.h"
#include "fadelogo.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_POLYGON (2)
#define FADE_TEXTURE			"data\\TEXTURE\\背景.jpg"//テクスチャのファイル名
#define FADE_POS_X				(0)							//左上座標X
#define FADE_POS_Y				(0)							//左上座標Y	
#define FADE_WIDTH			(SCREEN_WIDTH)					//正方形の横幅座標
#define FADE_HEIGHT			(SCREEN_HEIGHT)					//正方形の縦幅座標
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureFade = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	// 頂点バッファへのポインタ
FADE					g_fade;					// フェード状態
MODE					g_modeNext;				// 次のモード
D3DXCOLOR				g_colorFade;			// フェード色

//=============================================================================
// 初期化処理
//=============================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		FADE_TEXTURE,
		&g_pTextureFade);

	// 値の初期化
	g_fade = FADE_IN;									//フェードイン状態に
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	//黒い画面に(不透明)

	// 頂点情報の設定
	MakeVertexFade(pDevice);

	InitFadeLogo();
	
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitFade(void)
{
	// テクスチャの開放
	if (g_pTextureFade != NULL)
	{
		g_pTextureFade->Release();
		g_pTextureFade = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
	UninitFadeLogo();
}
 
//=============================================================================
// 更新処理
//=============================================================================
void UpdateFade(void)
{
	if (g_fade != FADE_NONE)
	{//何もしてない状態
		if (g_fade == FADE_IN)
		{//フェードイン/
			g_colorFade.a -= 0.02f;	//画面を透明にしていく
			if (g_colorFade.a <= 0.0f)
			{
				g_colorFade.a = 0.0f;	//値を入れなおして初期化
				g_fade = FADE_NONE;	//何もしてない状態
			}
		}
		else if (g_fade == FADE_OUT)
		{//フェードアウト状態
			g_colorFade.r = 1.0f;
			g_colorFade.g = 1.0f;
			g_colorFade.b = 1.0f;
			g_colorFade.a += 0.02f;		//画面を不透明に
			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;	//値を入れなおして初期化
				g_fade = FADE_IN;		//フェードイン状態に
				SetMode(g_modeNext);
			}
		}
		VERTEX_2D *pVtx;								//頂点情報へのポインタ	
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);	//頂点バッファをロックし頂点データのポインタを取得
		//頂点カラー
		pVtx[0].col = g_colorFade;
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;
		//頂点バッファをアンロックする
		g_pVtxBuffFade->Unlock();
	}
	UpdateFadeLogo();
}

//=============================================================================
// タイトル画面
//=============================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureFade);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	DrawFadeLogo();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexFade(LPDIRECT3DDEVICE9 pDevice)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	//頂点情報の生成
	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(FADE_POS_X, FADE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(FADE_WIDTH, FADE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(FADE_POS_X, FADE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(FADE_WIDTH, FADE_HEIGHT, 0.0f);

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
	g_pVtxBuffFade->Unlock();
}

//=============================================================================
// フェードの設定
//=============================================================================
void SetFade(MODE modeNext)
{
	if (g_fade != FADE_OUT)
	{
		g_fade = FADE_OUT;									//フェードアウト状態に
		g_modeNext = modeNext;
		g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);	//透明に
	}
}

//=============================================================================
// フェードの取得
//=============================================================================
FADE GetFade(void)
{
	return g_fade;
}

