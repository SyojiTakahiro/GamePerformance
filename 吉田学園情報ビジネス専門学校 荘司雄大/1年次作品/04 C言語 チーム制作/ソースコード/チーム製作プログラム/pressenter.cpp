//=============================================================================
// 授業作成
// 背景の処理 [pressenter.cpp]
// Author : Mikiya Meguro
//
//=============================================================================
#include "main.h"
#include "pressenter.h"
#include "input.h"
#include "Player.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define ENTER_TEXTURENAME		"data\\TEXTURE\\enter.png"	//テクスチャのファイル名
#define ENTER_X					(450)						//左上座標X
#define ENTER_Y					(650)						//左上座標Y	
#define ENTER_WIDTH				(800)						//正方形の横幅座標
#define ENTER_HEIGHT			(710)						//正方形の縦幅座標
#define TEX_ENTER_X_INIT		(1.0f)						//テクスチャ座標Uの初期位置
#define TEX_ENTER_Y_INIT		(0.005f)					//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)						//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)						//テクスチャ座標U右
#define TEX_TOP					(0.0f)						//テクスチャ座標V上
#define TEX_BOT					(1.0f)						//テクスチャ座標V下
#define MAX_PRESSENTER			(1)
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePressEnter = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPressEnter = NULL;	//頂点バッファへのポインタ
PRESSENTER	g_PressEnter[MAX_PRESSENTER];
//*****************************************************************************
// ポリゴン
//*****************************************************************************
void InitPressEnter(void)
{	//ローカル変数
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	for (int nCntPressEnter = 0; nCntPressEnter < MAX_PRESSENTER; nCntPressEnter++)
	{
		g_PressEnter[nCntPressEnter].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_PressEnter[nCntPressEnter].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_PressEnter[nCntPressEnter].PressEnterCnt = 0;
		g_PressEnter[nCntPressEnter].PressEnterChange = 0;
		g_PressEnter[nCntPressEnter].bUse = false;
		g_PressEnter[nCntPressEnter].bDraw = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		ENTER_TEXTURENAME,
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
	pVtx[0].pos = D3DXVECTOR3(ENTER_X, ENTER_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(ENTER_WIDTH, ENTER_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(ENTER_X, ENTER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(ENTER_WIDTH, ENTER_HEIGHT, 0.0f);
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
	//g_PressEnterCnt = 0;
	//g_PressEnterChange = 0;
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
	
	VERTEX_2D *pVtx;			//頂点情報へのポインタ
	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);
	
	for (int nCntPressEnter = 0; nCntPressEnter < MAX_PRESSENTER; nCntPressEnter++)
	{
		g_PressEnter[nCntPressEnter].PressEnterCnt++;

		if (g_PressEnter[nCntPressEnter].PressEnterCnt >= 30)
		{
			if (g_PressEnter[nCntPressEnter].PressEnterChange == 0)
			{
				g_PressEnter[nCntPressEnter].PressEnterChange = 1;
			}
			else if (g_PressEnter[nCntPressEnter].PressEnterChange == 1)
			{
				g_PressEnter[nCntPressEnter].PressEnterChange = 0;
			}
			//頂点カラー
			pVtx[0].col = D3DXCOLOR(g_PressEnter[nCntPressEnter].col.r, g_PressEnter[nCntPressEnter].col.g, g_PressEnter[nCntPressEnter].col.b, g_PressEnter[nCntPressEnter].PressEnterChange);
			pVtx[1].col = D3DXCOLOR(g_PressEnter[nCntPressEnter].col.r, g_PressEnter[nCntPressEnter].col.g, g_PressEnter[nCntPressEnter].col.b, g_PressEnter[nCntPressEnter].PressEnterChange);
			pVtx[2].col = D3DXCOLOR(g_PressEnter[nCntPressEnter].col.r, g_PressEnter[nCntPressEnter].col.g, g_PressEnter[nCntPressEnter].col.b, g_PressEnter[nCntPressEnter].PressEnterChange);
			pVtx[3].col = D3DXCOLOR(g_PressEnter[nCntPressEnter].col.r, g_PressEnter[nCntPressEnter].col.g, g_PressEnter[nCntPressEnter].col.b, g_PressEnter[nCntPressEnter].PressEnterChange);

			g_PressEnter[nCntPressEnter].PressEnterCnt = 0;
		}
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_PressEnter[nCntPressEnter].pos.x - g_PressEnter[nCntPressEnter].fWIdth, g_PressEnter[nCntPressEnter].pos.y - g_PressEnter[nCntPressEnter].fHeight, g_PressEnter[nCntPressEnter].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_PressEnter[nCntPressEnter].pos.x + g_PressEnter[nCntPressEnter].fWIdth, g_PressEnter[nCntPressEnter].pos.y - g_PressEnter[nCntPressEnter].fHeight, g_PressEnter[nCntPressEnter].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_PressEnter[nCntPressEnter].pos.x - g_PressEnter[nCntPressEnter].fWIdth, g_PressEnter[nCntPressEnter].pos.y + g_PressEnter[nCntPressEnter].fHeight, g_PressEnter[nCntPressEnter].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_PressEnter[nCntPressEnter].pos.x + g_PressEnter[nCntPressEnter].fWIdth, g_PressEnter[nCntPressEnter].pos.y + g_PressEnter[nCntPressEnter].fHeight, g_PressEnter[nCntPressEnter].pos.z);
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

	//アルファテスト(透明色を描画しないように)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPressEnter, 0, sizeof(VERTEX_2D));
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturePressEnter);
	
	for (int nCntPressEnter = 0; nCntPressEnter < MAX_PRESSENTER; nCntPressEnter++)
	{
		if (g_PressEnter[nCntPressEnter].bDraw == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				0,	//開始する頂点のインデックス
				2); //描画するプリミティブ数
		}
	}
	//レンダーステートの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}
//=============================================================================
// プレスエンターの設定処理
//=============================================================================
void SetPressEnter(D3DXVECTOR3 pos, D3DXCOLOR col, float fWIdth, float fHeight)
{
	int nCntPressEnter;

	VERTEX_2D *pVtx;//頂点情報へのポインタ
					//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffPressEnter->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntPressEnter = 0; nCntPressEnter < MAX_PRESSENTER; nCntPressEnter++)
	{
		if (g_PressEnter[nCntPressEnter].bUse == false)	//アイテムが使用されていない
		{

			g_PressEnter[nCntPressEnter].pos = pos;
			g_PressEnter[nCntPressEnter].fWIdth = fWIdth;
			g_PressEnter[nCntPressEnter].fHeight = fHeight;

			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_PressEnter[nCntPressEnter].pos.x - g_PressEnter[nCntPressEnter].fWIdth, g_PressEnter[nCntPressEnter].pos.y - g_PressEnter[nCntPressEnter].fHeight, g_PressEnter[nCntPressEnter].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_PressEnter[nCntPressEnter].pos.x + g_PressEnter[nCntPressEnter].fWIdth, g_PressEnter[nCntPressEnter].pos.y - g_PressEnter[nCntPressEnter].fHeight, g_PressEnter[nCntPressEnter].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_PressEnter[nCntPressEnter].pos.x - g_PressEnter[nCntPressEnter].fWIdth, g_PressEnter[nCntPressEnter].pos.y + g_PressEnter[nCntPressEnter].fHeight, g_PressEnter[nCntPressEnter].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_PressEnter[nCntPressEnter].pos.x + g_PressEnter[nCntPressEnter].fWIdth, g_PressEnter[nCntPressEnter].pos.y + g_PressEnter[nCntPressEnter].fHeight, g_PressEnter[nCntPressEnter].pos.z);

			g_PressEnter[nCntPressEnter].col = col;

			//頂点カラー
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;
			g_PressEnter[nCntPressEnter].bDraw = true;
			g_PressEnter[nCntPressEnter].bUse = true;
			break;
		}
		pVtx += 4;	//頂点データのポインタを4つ分進める	
	}
	//頂点バッファをアンロックする
	g_pVtxBuffPressEnter->Unlock();
}
//=============================================================================
// プレスエンターの取得
//=============================================================================
PRESSENTER *GetPressEnter(void)
{
	return &g_PressEnter[0];
}