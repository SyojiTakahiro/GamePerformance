//=============================================================================
//
// 爆発処理 [Explosion.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "explosion.h"
#include "shadow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define EXPLOSION_TEXTURENAME	 "data\\TEXTURE\\explosion000.png"//テクスチャのファイル名
#define COUNTER_ANIM_RESET		(7)								//カウンター用
#define TEX_POS_X_INIT			(0.125f)						//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)							//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)							//テクスチャ座標U左
#define TEX_RIGHT				(0.125f)						//テクスチャ座標U右
#define TEX_TOP					(0.0f)							//テクスチャ座標V上
#define TEX_BOT					(1.0f)							//テクスチャ座標V下
#define MAX_EXPLOSION			(128)							//爆発の最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9	g_pTextureExplosion = NULL;		//テクスチャへのポインタ
EXPLOSION g_aExplosion[MAX_EXPLOSION];

//=============================================================================
// 初期化処理
//=============================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスの取得
	pDevice = GetDevice();	// デバイスの取得

	//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			EXPLOSION_TEXTURENAME,
			&g_pTextureExplosion);
	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	VERTEX_3D *pVtx;
	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
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
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		//Uが幅、Vが高さ　(0.0f～1.0f）
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);		//Uが幅、Vが高さ　(0.0f～1.0f）
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		//Uが幅、Vが高さ　(0.0f～1.0f）
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);		//Uが幅、Vが高さ　(0.0f～1.0f）
		//頂点データのポインタを4つ分進める
		pVtx += 4;	
	}

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].fRadius = 0.0f;
		g_aExplosion[nCntExplosion].fHeght = 0.0f;
		g_aExplosion[nCntExplosion].fWidth = 0.0f;
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
		g_aExplosion[nCntExplosion].fTest = 1;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitExplosion(void)
{	//テクスチャの破棄
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateExplosion(void)
{
	VERTEX_3D *pVtx;//頂点情報へのポインタ

					//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{	// 使用される場合
			g_aExplosion[nCntExplosion].nCounterAnim++;			// カウンターの加算

			if ((g_aExplosion[nCntExplosion].nCounterAnim % 20) == 0)
			{
				g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1);

				//テクスチャ設定
				pVtx[nCntExplosion * 4].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim * 0.1f), 0.0f);
				pVtx[nCntExplosion * 4 + 1].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim * 0.1f) + 0.1f, 0.0f);
				pVtx[nCntExplosion * 4 + 2].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim * 0.1f), 1.0f);
				pVtx[nCntExplosion * 4 + 3].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].nPatternAnim * 0.1f) + 0.1f, 1.0f);

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aExplosion[nCntExplosion].fAlpha);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aExplosion[nCntExplosion].fAlpha);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aExplosion[nCntExplosion].fAlpha);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aExplosion[nCntExplosion].fAlpha);

				if (g_aExplosion[nCntExplosion].nPatternAnim > 7)
				{
					g_aExplosion[nCntExplosion].bUse = false;			//使用していない状態
				}
			}

			g_aExplosion[nCntExplosion].pos.y += 0.3f;
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX  mtxView, mtxTrans;				//計算用マトリックス

	//アルファテスト(透明色を描画しないように)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//Zバッファの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aExplosion[nCntExplosion].mtrxWorldExplosion);
		//ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		//逆行列を設定
		g_aExplosion[nCntExplosion].mtrxWorldExplosion._11 = mtxView._11;
		g_aExplosion[nCntExplosion].mtrxWorldExplosion._12 = mtxView._21;
		g_aExplosion[nCntExplosion].mtrxWorldExplosion._13 = mtxView._31;
		g_aExplosion[nCntExplosion].mtrxWorldExplosion._21 = mtxView._12;
		g_aExplosion[nCntExplosion].mtrxWorldExplosion._22 = mtxView._22;
		g_aExplosion[nCntExplosion].mtrxWorldExplosion._23 = mtxView._32;
		g_aExplosion[nCntExplosion].mtrxWorldExplosion._31 = mtxView._13;
		g_aExplosion[nCntExplosion].mtrxWorldExplosion._32 = mtxView._23;
		g_aExplosion[nCntExplosion].mtrxWorldExplosion._33 = mtxView._33;
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntExplosion].pos.x, g_aExplosion[nCntExplosion].pos.y, g_aExplosion[nCntExplosion].pos.z);
		D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtrxWorldExplosion, &g_aExplosion[nCntExplosion].mtrxWorldExplosion, &mtxTrans);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntExplosion].mtrxWorldExplosion);
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));
		//頂点フォーマットを設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureExplosion);

		if (g_aExplosion[nCntExplosion].bUse == true)	//爆発が使用されている
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				0 + (4 * nCntExplosion),	//開始する頂点のインデックス
				2); //描画するプリミティブ数
		}
	}
	//レンダーステートの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//Zバッファをデフォルトの設定に戻す
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	//レンダーステイトを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定

}

//=============================================================================
// 爆発の設定処理
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, float fHeght, float fWidth)
{
	int nCntExplosion;

	VERTEX_3D *pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)	//弾が使用されていない
		{
			g_aExplosion[nCntExplosion].pos = pos;
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(-fWidth, fHeght, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(fWidth, fHeght, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-fWidth, -fHeght, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(fWidth, -fHeght, 0.0f);

			//頂点カラー
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			g_aExplosion[nCntExplosion].col = col;

			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].fTest = 1;

			g_aExplosion[nCntExplosion].bUse = true;
			break;
		}
		pVtx += 4;	//頂点データのポインタを4つ分進める	
	}
	//頂点バッファをアンロックする
	g_pVtxBuffExplosion->Unlock();
}
