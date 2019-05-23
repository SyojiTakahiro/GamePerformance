//=============================================================================
//
// ポリゴン処理 [BG.cpp]
// Author : 
//
//=============================================================================
#include "bg.h"
#include "player.h"
#include "shadow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_NAME	"data/TEXTURE/block000.jpg"

#define MAX_H			(8)
#define MAX_V			(2)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexBG(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBG = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffBG = NULL;

D3DXVECTOR3 g_posBG;
D3DXVECTOR3 g_rotBG;
D3DXMATRIX g_mtxWorldBG;

int g_nNumVertexBG;
int g_nNumIndexBG;
int g_nNumPolygonBG;

//=============================================================================
// 初期化処理
//=============================================================================
void InitBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	pDevice = GetDevice();

	int nCountIdx = 0;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &g_pTextureBG);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MAX_H + 1) * (MAX_V + 1),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBG,
		NULL);

	// インデックスバッファの作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 14,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffBG,
		NULL);

	VERTEX_3D*pVtx;

	//頂点バッファをロック
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(-50, 0, 50);
	pVtx[1].pos = D3DXVECTOR3(0, 0, 50);
	pVtx[2].pos = D3DXVECTOR3(50, 0, 50);
	pVtx[3].pos = D3DXVECTOR3(-50, 0, 0);
	pVtx[4].pos = D3DXVECTOR3(0, 0, 0);
	pVtx[5].pos = D3DXVECTOR3(50, 0, 0);
	pVtx[6].pos = D3DXVECTOR3(-50, 0, -50);
	pVtx[7].pos = D3DXVECTOR3(0, 0, -50);
	pVtx[8].pos = D3DXVECTOR3(50, 0, -50);

	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[4].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[5].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[6].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[7].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[8].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[4].tex = D3DXVECTOR2(0.5f, 0.5f);
	pVtx[5].tex = D3DXVECTOR2(1.0f, 0.5f);
	pVtx[6].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[7].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[8].tex = D3DXVECTOR2(1.0f, 1.0f);

	//アンロック
	g_pVtxBuffBG->Unlock();

	WORD*pIdx;		//ポインタ
	g_pIdxBuffBG->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntV = 0; nCntV < MAX_V + 1; nCntV++)
	{
		for (int nCntH = 0; nCntH < MAX_H + 1; nCntH++)
		{
			pVtx[0].pos.x = sinf(D3DX_PI*0.0f) * 100;
			pVtx[0].pos.y = 100;
			pVtx[0].pos.z = cosf(D3DX_PI*0.0f) * 100;
		}
	}
	/*pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;*/

	g_pIdxBuffBG->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBG(void)
{
	//テクスチャの破棄
	if (g_pTextureBG != NULL)
	{
		g_pTextureBG->Release();
		g_pTextureBG = NULL;
	}
	//頂点の破棄
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}
	//インデックスバッファの破棄
	if (g_pIdxBuffBG != NULL)
	{
		g_pIdxBuffBG->Release();
		g_pIdxBuffBG = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBG(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans;

	pDevice = GetDevice();	// デバイスの取得

							// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtxWorldBG);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotBG.y, g_rotBG.x, g_rotBG.z);

	D3DXMatrixMultiply(&g_mtxWorldBG, &g_mtxWorldBG, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posBG.x, g_posBG.y, g_posBG.z);

	D3DXMatrixMultiply(&g_mtxWorldBG, &g_mtxWorldBG, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldBG);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_3D));

	pDevice->SetIndices(g_pIdxBuffBG);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBG);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 9, 0, 12);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexBG(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定

}