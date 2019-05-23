//=============================================================================
//
// PressEnter処理 [pressenter.cpp]
// Author : Niwa Hodaka
//
//=============================================================================
#include "pressenter.h"
#include "fade.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME       "data/TEXTURE/pressbutton.png"  // 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexPressEnter(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePressEnter = NULL;	   // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPressEnter = NULL;	   // 頂点バッファへのポインタ
D3DXVECTOR3             g_PressEnterPos(0.0f, 0.0f, 0.0f); // 座標
D3DXCOLOR               g_PressEnterCol;                   // 色
PRESSENTERSTATE         g_PressEnterState;                 // 状態
int                     g_SizePressEnterX;                 // サイズX
int                     g_SizePressEnterY;                 // サイズY
int                     g_nCounterPressEnter;              // Drawするタイミング管理カウンター
bool                    g_bDisp;                           // 描画するかしないか

//=============================================================================
// 初期化処理
//=============================================================================
void InitPressEnter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_PressEnterState = PRESSENTERSTATE_NONE;             // 押されていない状態にする
	g_PressEnterCol = D3DXCOLOR(1.0f, 1.0f, 0.0f,1.0f);   // 色を初期化
	g_PressEnterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 座標を初期化
	g_nCounterPressEnter = 0;                             // カウンターは0にする
	g_bDisp = false;                                      // 表示はしない

	// 頂点情報の設定
	MakeVertexPressEnter(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPressEnter(void)
{
	// テクスチャの開放
	if (g_pTexturePressEnter != NULL)
	{
		g_pTexturePressEnter->Release();
		g_pTexturePressEnter = NULL;
	}

	// 頂点バッファの開放
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
	if (g_nCounterPressEnter >= 100)
	{
		g_nCounterPressEnter = 0;
	}

	if (g_bDisp == true)
	{// 描画する状態ならば
		g_nCounterPressEnter++;
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// ENTERキーが押された
			g_PressEnterState = PRESSENTERSTATE_PUSH;  // 押されている状態にする
		}
		else
		{
			for (int nCntPad = 0; nCntPad < 2; nCntPad++)
			{
				if (GetXInputTrigger(nCntPad, XIJS_BUTTON_11) == TRUE)
				{
					g_PressEnterState = PRESSENTERSTATE_PUSH;  // 押されている状態にする
				}
			}
		}
	}

	FADE fade;
	fade = GetFade();

	if (fade == FADE_IN)
	{// フェード状態だったら
		g_PressEnterState = PRESSENTERSTATE_NONE;
		g_nCounterPressEnter = 0;
	}
}
//=============================================================================
// タイトル画面
//=============================================================================
void DrawPressEnter(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPressEnter, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePressEnter);

	// ポリゴンの描画
	if (g_bDisp == true)
	{// 描画する状態だったら
		if (g_PressEnterState == PRESSENTERSTATE_NONE)
		{// 押されていない状態だったら
			if (g_nCounterPressEnter / 80 == 0)
			{// カウンターが一定の値になったら
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
			}
		}
		else if (g_PressEnterState == PRESSENTERSTATE_PUSH)
		{// 押されている状態だったら
			if (g_nCounterPressEnter % 4 == 0)
			{// カウンターが一定の値になったら
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
			}
		}
	}
}
//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexPressEnter(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME,
		&g_pTexturePressEnter);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPressEnter,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_PressEnterPos.x - g_SizePressEnterX, g_PressEnterPos.y - g_SizePressEnterY, g_PressEnterPos.z);
	pVtx[1].pos = D3DXVECTOR3(g_PressEnterPos.x + g_SizePressEnterX, g_PressEnterPos.y - g_SizePressEnterY, g_PressEnterPos.z);
	pVtx[2].pos = D3DXVECTOR3(g_PressEnterPos.x - g_SizePressEnterX, g_PressEnterPos.y + g_SizePressEnterY, g_PressEnterPos.z);
	pVtx[3].pos = D3DXVECTOR3(g_PressEnterPos.x + g_SizePressEnterX, g_PressEnterPos.y + g_SizePressEnterY, g_PressEnterPos.z);

	// 頂点テクスチャ
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = g_PressEnterCol;
	pVtx[1].col = g_PressEnterCol;
	pVtx[2].col = g_PressEnterCol;
	pVtx[3].col = g_PressEnterCol;

	// テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	// 頂点バッファをアンロックする
	g_pVtxBuffPressEnter->Unlock();
}
//=============================================================================
// 座標の設定
//=============================================================================
void SetPressEnter(D3DXVECTOR3 pos,int xsize,int ysize)
{
	g_PressEnterPos = pos;
	g_SizePressEnterX = xsize;
	g_SizePressEnterY = ysize;

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_PressEnterPos.x - g_SizePressEnterX, g_PressEnterPos.y - g_SizePressEnterY, g_PressEnterPos.z);
	pVtx[1].pos = D3DXVECTOR3(g_PressEnterPos.x + g_SizePressEnterX, g_PressEnterPos.y - g_SizePressEnterY, g_PressEnterPos.z);
	pVtx[2].pos = D3DXVECTOR3(g_PressEnterPos.x - g_SizePressEnterX, g_PressEnterPos.y + g_SizePressEnterY, g_PressEnterPos.z);
	pVtx[3].pos = D3DXVECTOR3(g_PressEnterPos.x + g_SizePressEnterX, g_PressEnterPos.y + g_SizePressEnterY, g_PressEnterPos.z);

	// 頂点バッファをアンロックする
	g_pVtxBuffPressEnter->Unlock();

	g_bDisp = true;
}
//=============================================================================
// 状態の取得
//=============================================================================
PRESSENTERSTATE *GetPressEnter(void)
{
	return &g_PressEnterState;
}
//=============================================================================
// 描画状態の取得
//=============================================================================
bool *GetDispPressEnter(void)
{
	return &g_bDisp;
}
//=============================================================================
// 描画するかしないか
//=============================================================================
void DispPressEnter(void)
{
	g_bDisp = true;
}