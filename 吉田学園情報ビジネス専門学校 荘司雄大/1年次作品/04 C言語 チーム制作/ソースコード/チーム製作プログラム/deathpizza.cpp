//=============================================================================
//
// デスピザ処理 [deathpizza.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "deathpizza.h"
#include "shadow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EFFECT_TEXTURENAME	 "data\\TEXTURE\\DEATHPIZZA000.png"	//テクスチャのファイル名
#define TEX_POS_X_INIT			(1.0f)							//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)							//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)							//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)							//テクスチャ座標U右
#define TEX_TOP					(0.0f)							//テクスチャ座標V上
#define TEX_BOT					(1.0f)							//テクスチャ座標V下
#define MAX_EFFECT				(128)							//エフェクトの最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexDeathPizza(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDeathPizza = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9	g_pTextureDeathPizza = NULL;		//テクスチャへのポインタ
DEATHPIZZA g_aDeathPizza[MAX_EFFECT];

//=============================================================================
// 初期化処理
//=============================================================================
void InitDeathPizza(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスの取得
	pDevice = GetDevice();	// デバイスの取得

	//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			EFFECT_TEXTURENAME,
			&g_pTextureDeathPizza);
	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDeathPizza,
		NULL);

	VERTEX_3D *pVtx;
	//頂点バッファをロック
	g_pVtxBuffDeathPizza->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDeathPizza = 0; nCntDeathPizza < MAX_EFFECT; nCntDeathPizza++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-20.0f, 20.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(20.0f, 20.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-20.0f, -20.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(20.0f, -20.0f, 0.0f);

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

	for (int nCntDeathPizza = 0; nCntDeathPizza < MAX_EFFECT; nCntDeathPizza++)
	{
		g_aDeathPizza[nCntDeathPizza].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aDeathPizza[nCntDeathPizza].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aDeathPizza[nCntDeathPizza].fRadius = 0.0f;
		g_aDeathPizza[nCntDeathPizza].fHeght = 0.0f;
		g_aDeathPizza[nCntDeathPizza].fWidth = 0.0f;
		g_aDeathPizza[nCntDeathPizza].bUse = false;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffDeathPizza->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitDeathPizza(void)
{	//テクスチャの破棄
	if (g_pTextureDeathPizza != NULL)
	{
		g_pTextureDeathPizza->Release();
		g_pTextureDeathPizza = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffDeathPizza != NULL)
	{
		g_pVtxBuffDeathPizza->Release();
		g_pVtxBuffDeathPizza = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateDeathPizza(void)
{
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;
	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffDeathPizza->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntDeathPizza = 0; nCntDeathPizza < MAX_EFFECT; nCntDeathPizza++)
	{
		if (g_aDeathPizza[nCntDeathPizza].bUse == true)	//エフェクトが使用されている
		{
			g_aDeathPizza[nCntDeathPizza].pos += g_aDeathPizza[nCntDeathPizza].move;
			//g_aDeathPizza[nCntDeathPizza].nLife -= 1;
			//g_aDeathPizza[nCntDeathPizza].fRadius -= 0.5f;
			g_aDeathPizza[nCntDeathPizza].col.a -= 0.01f;

			if (/*g_aDeathPizza[nCntDeathPizza].nLife <= 0 || */g_aDeathPizza[nCntDeathPizza].col.a <= 0.1f)	//エフェクトのライフが０
			{
				g_aDeathPizza[nCntDeathPizza].bUse = false;
			}
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(-g_aDeathPizza[nCntDeathPizza].fRadius, g_aDeathPizza[nCntDeathPizza].fRadius, 0);
			pVtx[1].pos = D3DXVECTOR3(g_aDeathPizza[nCntDeathPizza].fRadius, g_aDeathPizza[nCntDeathPizza].fRadius, 0);
			pVtx[2].pos = D3DXVECTOR3(-g_aDeathPizza[nCntDeathPizza].fRadius, -g_aDeathPizza[nCntDeathPizza].fRadius, 0);
			pVtx[3].pos = D3DXVECTOR3(g_aDeathPizza[nCntDeathPizza].fRadius, -g_aDeathPizza[nCntDeathPizza].fRadius, 0);

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(g_aDeathPizza[nCntDeathPizza].col.r, g_aDeathPizza[nCntDeathPizza].col.g, g_aDeathPizza[nCntDeathPizza].col.b, g_aDeathPizza[nCntDeathPizza].col.a);
			pVtx[1].col = D3DXCOLOR(g_aDeathPizza[nCntDeathPizza].col.r, g_aDeathPizza[nCntDeathPizza].col.g, g_aDeathPizza[nCntDeathPizza].col.b, g_aDeathPizza[nCntDeathPizza].col.a);
			pVtx[2].col = D3DXCOLOR(g_aDeathPizza[nCntDeathPizza].col.r, g_aDeathPizza[nCntDeathPizza].col.g, g_aDeathPizza[nCntDeathPizza].col.b, g_aDeathPizza[nCntDeathPizza].col.a);
			pVtx[3].col = D3DXCOLOR(g_aDeathPizza[nCntDeathPizza].col.r, g_aDeathPizza[nCntDeathPizza].col.g, g_aDeathPizza[nCntDeathPizza].col.b, g_aDeathPizza[nCntDeathPizza].col.a);

			pVtx += 4;	//頂点データのポインタを4つ分進める
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffDeathPizza->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawDeathPizza(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX  mtxView, mtxTrans;				//計算用マトリックス

	//アルファテスト(透明色を描画しないように)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//Zバッファの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	for (int nCntDeathPizza = 0; nCntDeathPizza < MAX_EFFECT; nCntDeathPizza++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aDeathPizza[nCntDeathPizza].mtrxWorldDeathPizza);
		//ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		//逆行列を設定
		g_aDeathPizza[nCntDeathPizza].mtrxWorldDeathPizza._11 = mtxView._11;
		g_aDeathPizza[nCntDeathPizza].mtrxWorldDeathPizza._12 = mtxView._21;
		g_aDeathPizza[nCntDeathPizza].mtrxWorldDeathPizza._13 = mtxView._31;
		g_aDeathPizza[nCntDeathPizza].mtrxWorldDeathPizza._21 = mtxView._12;
		g_aDeathPizza[nCntDeathPizza].mtrxWorldDeathPizza._22 = mtxView._22;
		g_aDeathPizza[nCntDeathPizza].mtrxWorldDeathPizza._23 = mtxView._32;
		g_aDeathPizza[nCntDeathPizza].mtrxWorldDeathPizza._31 = mtxView._13;
		g_aDeathPizza[nCntDeathPizza].mtrxWorldDeathPizza._32 = mtxView._23;
		g_aDeathPizza[nCntDeathPizza].mtrxWorldDeathPizza._33 = mtxView._33;
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aDeathPizza[nCntDeathPizza].pos.x, g_aDeathPizza[nCntDeathPizza].pos.y, g_aDeathPizza[nCntDeathPizza].pos.z);
		D3DXMatrixMultiply(&g_aDeathPizza[nCntDeathPizza].mtrxWorldDeathPizza, &g_aDeathPizza[nCntDeathPizza].mtrxWorldDeathPizza, &mtxTrans);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aDeathPizza[nCntDeathPizza].mtrxWorldDeathPizza);
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffDeathPizza, 0, sizeof(VERTEX_3D));
		//頂点フォーマットを設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureDeathPizza);

		if (g_aDeathPizza[nCntDeathPizza].bUse == true)	//エフェクトが使用されている
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				0 + (4 * nCntDeathPizza),	//開始する頂点のインデックス
				2); //描画するプリミティブ数
		}

		//テクスチャの設定
		pDevice->SetTexture(0, NULL);
	}
	//レンダーステートの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//Zバッファをデフォルトの設定に戻す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexDeathPizza(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定

}

//=============================================================================
// エフェクトの設定処理
//=============================================================================
void SetDeathPizza(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife)
{
	int nCntDeathPizza;

	VERTEX_3D *pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffDeathPizza->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntDeathPizza = 0; nCntDeathPizza < MAX_EFFECT; nCntDeathPizza++)
	{
		if (g_aDeathPizza[nCntDeathPizza].bUse == false)	//エフェクトが使用されていない
		{
			g_aDeathPizza[nCntDeathPizza].pos = pos;
			g_aDeathPizza[nCntDeathPizza].move = move;
			g_aDeathPizza[nCntDeathPizza].fRadius = fRadius;
			g_aDeathPizza[nCntDeathPizza].nLife = nLife;
			g_aDeathPizza[nCntDeathPizza].col = col;
			g_aDeathPizza[nCntDeathPizza].bUse = true;
			break;
		}
		pVtx += 4;	//頂点データのポインタを4つ分進める	
	}
	//頂点バッファをアンロックする
	g_pVtxBuffDeathPizza->Unlock();
}
