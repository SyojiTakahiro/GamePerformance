//=============================================================================
//
// ビルボード処理 [Billboard.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "billboard.h"
#include "shadow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BILLBOARD_TEXTURENAME	 "data\\TEXTURE\\tree000.png"	//テクスチャのファイル名
#define TEX_POS_X_INIT			(1.0f)							//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)							//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)							//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)							//テクスチャ座標U右
#define TEX_TOP					(0.0f)							//テクスチャ座標V上
#define TEX_BOT					(1.0f)							//テクスチャ座標V下
#define MAX_BILLBOARD			(128)							//ビルボードの最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9	g_pTextureBillboard = NULL;		//テクスチャへのポインタ
BILLBOARD g_aBillboard[MAX_BILLBOARD];

//=============================================================================
// 初期化処理
//=============================================================================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスの取得
	pDevice = GetDevice();	// デバイスの取得

	//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			BILLBOARD_TEXTURENAME,
			&g_pTextureBillboard);
	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BILLBOARD,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBillboard,
		NULL);

	VERTEX_3D *pVtx;
	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-20.0f, 50.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(20.0f, 50.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-20.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(20.0f, 0.0f, 0.0f);

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(TEX_LEFT, TEX_TOP);
		pVtx[1].tex = D3DXVECTOR2(TEX_RIGHT, TEX_TOP);
		pVtx[2].tex = D3DXVECTOR2(TEX_LEFT, TEX_BOT);
		pVtx[3].tex = D3DXVECTOR2(TEX_RIGHT, TEX_BOT);
		//頂点データのポインタを4つ分進める
		pVtx += 4;	
	}

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		g_aBillboard[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].fRadius = 0.0f;
		g_aBillboard[nCntBillboard].fHeght = 0.0f;
		g_aBillboard[nCntBillboard].fWidth = 0.0f;
		g_aBillboard[nCntBillboard].bUse = false;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBillboard(void)
{	//テクスチャの破棄
	if (g_pTextureBillboard != NULL)
	{
		g_pTextureBillboard->Release();
		g_pTextureBillboard = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBillboard(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX  mtxView, mtxTrans;				//計算用マトリックス

	//アルファテスト(透明色を描画しないように)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	for (int nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aBillboard[nCntBillboard].mtrxWorldBillboard);
		//ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		//逆行列を設定
		g_aBillboard[nCntBillboard].mtrxWorldBillboard._11 = mtxView._11;
		g_aBillboard[nCntBillboard].mtrxWorldBillboard._12 = mtxView._21;
		g_aBillboard[nCntBillboard].mtrxWorldBillboard._13 = mtxView._31;
		g_aBillboard[nCntBillboard].mtrxWorldBillboard._21 = mtxView._12;
		g_aBillboard[nCntBillboard].mtrxWorldBillboard._22 = mtxView._22;
		g_aBillboard[nCntBillboard].mtrxWorldBillboard._23 = mtxView._32;
		g_aBillboard[nCntBillboard].mtrxWorldBillboard._31 = mtxView._13;
		g_aBillboard[nCntBillboard].mtrxWorldBillboard._32 = mtxView._23;
		g_aBillboard[nCntBillboard].mtrxWorldBillboard._33 = mtxView._33;
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aBillboard[nCntBillboard].pos.x, g_aBillboard[nCntBillboard].pos.y, g_aBillboard[nCntBillboard].pos.z);
		D3DXMatrixMultiply(&g_aBillboard[nCntBillboard].mtrxWorldBillboard, &g_aBillboard[nCntBillboard].mtrxWorldBillboard, &mtxTrans);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard[nCntBillboard].mtrxWorldBillboard);
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));
		//頂点フォーマットを設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBillboard);

		if (g_aBillboard[nCntBillboard].bUse == true)	//壁が使用されている
		{
			// ビルボードの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				0 + (4 * nCntBillboard),	//開始する頂点のインデックス
				2); //描画するプリミティブ数
		}
	}
	//レンダーステートの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定

}

//=============================================================================
// 壁の設定処理
//=============================================================================
void SetBillboard(D3DXVECTOR3 pos, float fHeght, float fWidth)
{
	int nCntBillboard;
	VERTEX_3D *pVtx;//頂点情報へのポインタ
					//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntBillboard = 0; nCntBillboard < MAX_BILLBOARD; nCntBillboard++)
	{
		if (g_aBillboard[nCntBillboard].bUse == false)	//壁が使用されていない
		{
			g_aBillboard[nCntBillboard].pos = pos;
			g_aBillboard[nCntBillboard].fHeght= fHeght;
			g_aBillboard[nCntBillboard].fWidth = fWidth;
			//　影のセット
			//g_aBillboard[nCntBillboard].nIdxShadow = SetShadow(pos, D3DXVECTOR3(0, 0, 0), 20.0f);

			g_aBillboard[nCntBillboard].bUse = true;
			break;
		}
		pVtx += 4;	//頂点データのポインタを4つ分進める	
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBillboard->Unlock();
}
