//=============================================================================
// 授業作成
// 背景の処理 [titlelogo.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "p2winlogo.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLELOGO_TEXTURENAME	"data\\TEXTURE\\p2win.png"//テクスチャのファイル名
#define TEX_LEFT				(0.0f)						//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)						//テクスチャ座標U右
#define TEX_TOP					(0.0f)						//テクスチャ座標V上
#define TEX_BOT					(1.0f)						//テクスチャ座標V下
#define TITLELOGO_POS_X			(350)						//頂点座標Xの移動量
#define TITLELOGO_POS_Y			(100)						//頂点座標Yの移動量

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		 g_pTextureP2winLogo = NULL;					//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	 g_pVtxBuffP2winLogo = NULL;					//頂点バッファへのポインタ
D3DXVECTOR3				 g_P2winLogopos;								//位置
D3DXVECTOR3				 g_P2winLogomove;								//移動量
float					 g_fMoveP2winLogoY;
//*****************************************************************************
// ポリゴン
//*****************************************************************************
void InitP2winLogo(void)
{	//ローカル変数
	g_P2winLogopos = D3DXVECTOR3(640.0f, -30.0f, 0.0f); //位置
	g_P2winLogomove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	 //位置
	g_fMoveP2winLogoY = 1;
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TITLELOGO_TEXTURENAME,
		&g_pTextureP2winLogo);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffP2winLogo,
		NULL);

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffP2winLogo->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_P2winLogopos.x - TITLELOGO_POS_X, g_P2winLogopos.y - TITLELOGO_POS_Y, g_P2winLogopos.z);
	pVtx[1].pos = D3DXVECTOR3(g_P2winLogopos.x + TITLELOGO_POS_X, g_P2winLogopos.y - TITLELOGO_POS_Y, g_P2winLogopos.z);
	pVtx[2].pos = D3DXVECTOR3(g_P2winLogopos.x - TITLELOGO_POS_X, g_P2winLogopos.y + TITLELOGO_POS_Y, g_P2winLogopos.z);
	pVtx[3].pos = D3DXVECTOR3(g_P2winLogopos.x + TITLELOGO_POS_X, g_P2winLogopos.y + TITLELOGO_POS_Y, g_P2winLogopos.z);
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
	g_pVtxBuffP2winLogo->Unlock();
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitP2winLogo(void)
{	//テクスチャの破棄
	if (g_pTextureP2winLogo != NULL)
	{
		g_pTextureP2winLogo->Release();
		g_pTextureP2winLogo = NULL;
	}
	//頂点バッファの破棄の破棄
	if (g_pVtxBuffP2winLogo != NULL)
	{
		g_pVtxBuffP2winLogo->Release();
		g_pVtxBuffP2winLogo = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateP2winLogo(void)
{

	if (g_P2winLogopos.y >= 220.0f)
	{
		g_fMoveP2winLogoY = 0;
	}

	//位置更新
	g_P2winLogomove.y = 2 * g_fMoveP2winLogoY;
	g_P2winLogopos.y += g_P2winLogomove.y;

	//位置の更新
	VERTEX_2D *pVtx;	//頂点情報へのポインタ
						//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffP2winLogo->Lock(0, 0, (void**)&pVtx, 0);
	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(g_P2winLogopos.x - TITLELOGO_POS_X, g_P2winLogopos.y - TITLELOGO_POS_Y, g_P2winLogopos.z);
	pVtx[1].pos = D3DXVECTOR3(g_P2winLogopos.x + TITLELOGO_POS_X, g_P2winLogopos.y - TITLELOGO_POS_Y, g_P2winLogopos.z);
	pVtx[2].pos = D3DXVECTOR3(g_P2winLogopos.x - TITLELOGO_POS_X, g_P2winLogopos.y + TITLELOGO_POS_Y, g_P2winLogopos.z);
	pVtx[3].pos = D3DXVECTOR3(g_P2winLogopos.x + TITLELOGO_POS_X, g_P2winLogopos.y + TITLELOGO_POS_Y, g_P2winLogopos.z);
	//頂点バッファをアンロックする
	g_pVtxBuffP2winLogo->Unlock();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawP2winLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスを取得する
	pDevice = GetDevice();

	//アルファテスト(透明色を描画しないように)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffP2winLogo, 0, sizeof(VERTEX_2D));
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureP2winLogo);
	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,	//開始する頂点のインデックス
		2); //描画するプリミティブ数

			//レンダーステートの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}
//=============================================================================
// タイトルロゴ取得処理
//=============================================================================
D3DXVECTOR3 *GetP2winLogo(void)
{
	return &g_P2winLogopos;
}
//=============================================================================
// タイトルロゴのセット
//=============================================================================
void SetP2winLogo(D3DXVECTOR3 nP2winLogo)
{
	g_P2winLogopos = nP2winLogo;
}