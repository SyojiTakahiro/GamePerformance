//=============================================================================
//
// 影の処理 [shadow.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "shadow.h"
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SHADOW_TEXTURE_NAME   "data/TEXTURE/shadow000.jpg"          // 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9      g_pTextureShadow = NULL;   // テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;   // 頂点バッファへのポインタ
Shadow                  g_aShadow[MAX_SHADOW];     // 影の情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 頂点情報の作成
	MakeVertexShadow(pDevice);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{// 影の総数分繰り返し
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);
		g_aShadow[nCntShadow].width = 0.0f;
		g_aShadow[nCntShadow].depth = 0.0f;
		g_aShadow[nCntShadow].bUse = false;
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{ 
	// テクスチャの開放
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;                // 計算用マトリックス

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{// 影の総数分繰り返し
		if (g_aShadow[nCntShadow].bUse == true)
		{// 影が使用されている
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntShadow * 4, NUM_POLYGON);
		}
	}

	// 合成の設定を戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		SHADOW_TEXTURE_NAME,
		&g_pTextureShadow);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{// 影の総数分繰り返し
		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].width, 0.0f, g_aShadow[nCntShadow].depth);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].width, 0.0f, g_aShadow[nCntShadow].depth);
		pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].width, 0.0f, -g_aShadow[nCntShadow].depth);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].width, 0.0f, -g_aShadow[nCntShadow].depth);

		// 法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラー
		pVtx[0].col = g_aShadow[nCntShadow].col;
		pVtx[1].col = g_aShadow[nCntShadow].col;
		pVtx[2].col = g_aShadow[nCntShadow].col;
		pVtx[3].col = g_aShadow[nCntShadow].col;

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;   // ポインタを進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}
//=============================================================================
// 影の設定
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float width, float depth)
{
	VERTEX_3D *pVtx;
	int nCntShadow;  // 影の番号

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{// 影の総数分繰り返し
		if (g_aShadow[nCntShadow].bUse == false)
		{// 影が使用されていない
			g_aShadow[nCntShadow].pos = pos;       // 位置を設定
			g_aShadow[nCntShadow].rot = rot;       // 向きを設定
			g_aShadow[nCntShadow].width = width;   // 幅を設定
			g_aShadow[nCntShadow].depth = depth;   // 奥行を設定
			g_aShadow[nCntShadow].col = col;       // 色を設定

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].width, 0.0f, g_aShadow[nCntShadow].depth);
			pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].width, 0.0f, g_aShadow[nCntShadow].depth);
			pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].width, 0.0f, -g_aShadow[nCntShadow].depth);
			pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].width, 0.0f, -g_aShadow[nCntShadow].depth);

			// 頂点カラーの設定
			pVtx[0].col = g_aShadow[nCntShadow].col;
			pVtx[1].col = g_aShadow[nCntShadow].col;
			pVtx[2].col = g_aShadow[nCntShadow].col;
			pVtx[3].col = g_aShadow[nCntShadow].col;

			g_aShadow[nCntShadow].bUse = true;     // 使用している状態にする
			break;
		}
		pVtx += 4;   // ポインタを進めておく
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();

	return nCntShadow;
}
//=============================================================================
// 影の座標設定
//=============================================================================
void SetPositionShadow(int IdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[IdxShadow].pos = pos;  // 座標を設定しなおす
}
//=============================================================================
// 影の色設定
//=============================================================================
void SetColShadow(int IdxShadow, D3DXCOLOR col)
{
	g_aShadow[IdxShadow].col = col;  // 色を設定しなおす

	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += IdxShadow * 4;  // ポインタを番号の先頭にもっていく

	// 頂点カラーの設定
	pVtx[0].col = g_aShadow[IdxShadow].col;
	pVtx[1].col = g_aShadow[IdxShadow].col;
	pVtx[2].col = g_aShadow[IdxShadow].col;
	pVtx[3].col = g_aShadow[IdxShadow].col;

	// 頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}
//=============================================================================
// 影の大きさ設定
//=============================================================================
void SetSizeShadow(int IdxShadow, float width, float depth)
{
	g_aShadow[IdxShadow].width = width;  // 幅を設定しなおす
	g_aShadow[IdxShadow].depth = depth;  // 奥行を設定しなおす

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += IdxShadow * 4;    // ポインタを番号の先頭にもっていく

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-g_aShadow[IdxShadow].width, 0.0f, g_aShadow[IdxShadow].depth);
	pVtx[1].pos = D3DXVECTOR3(g_aShadow[IdxShadow].width, 0.0f, g_aShadow[IdxShadow].depth);
	pVtx[2].pos = D3DXVECTOR3(-g_aShadow[IdxShadow].width, 0.0f, -g_aShadow[IdxShadow].depth);
	pVtx[3].pos = D3DXVECTOR3(g_aShadow[IdxShadow].width, 0.0f, -g_aShadow[IdxShadow].depth);

	// 頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}
//=============================================================================
// 影の削除
//=============================================================================
void DeleteShadow(int IdxShadow)
{
	g_aShadow[IdxShadow].bUse = false;  // 使用していない状態にする
}