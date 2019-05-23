//=============================================================================
//
// パーティクルの処理 [particl.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "particle.h"
#include "camera.h"
#include "shadow.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PARTICLE_TEXTURE_NAME   "data/TEXTURE/effect000.jpg"     // 読み込むテクスチャファイル名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9      g_pTextureParticle = NULL;  // テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;  // 頂点バッファへのポインタ
Particle                g_aParticle[MAX_PARTICLE];  // パーティクルの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 頂点情報の作成
	MakeVertexParticle(pDevice);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{// パーティクルの総数分繰り返し
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 位置を初期化
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 移動量を初期化
		g_aParticle[nCntParticle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // 向きを初期化
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // 色を初期化
		g_aParticle[nCntParticle].fRadius = 0.0f;                          // 半径を初期化
		g_aParticle[nCntParticle].bUse = false;                            // 使用していない状態にする
		g_aParticle[nCntParticle].nIdxShadow = 0;                          // 影の番号を初期化
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{
	// テクスチャの開放
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateParticle(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{// パーティクルの総数分繰り返し
		if (g_aParticle[nCntParticle].bUse == true)
		{// パーティクルが使用されている
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move; // 位置更新
			g_aParticle[nCntParticle].nLife--;          // 寿命を削る
			g_aParticle[nCntParticle].fRadius -= 0.01f; // 半径を減らす
			g_aParticle[nCntParticle].col.r -= 0.01f;   // 赤を減らす
			g_aParticle[nCntParticle].col.g -= 0.01f;   // 緑を減らす
			g_aParticle[nCntParticle].col.b -= 0.01f;   // 青を減らす
			g_aParticle[nCntParticle].col.a -= 0.01f;   // 透明度を減らす

			// 大きさを更新
			pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

			// 頂点カラーを更新
			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;

			// 影の位置更新
			SetPositionShadow(g_aParticle[nCntParticle].nIdxShadow, D3DXVECTOR3(g_aParticle[nCntParticle].pos.x, 0.0f, g_aParticle[nCntParticle].pos.z));

			if (g_aParticle[nCntParticle].nLife <= 0 || g_aParticle[nCntParticle].fRadius <= 0 || g_aParticle[nCntParticle].col.a <= 0)
			{// 寿命がなくなった
				g_aParticle[nCntParticle].bUse = false;              // 使用していない状態に
				DeleteShadow(g_aParticle[nCntParticle].nIdxShadow);  // 影も消す
			}
		}
		pVtx += 4;   // ポインタを進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxView, mtxTrans;               // 計算用マトリックス

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE); // 深度バッファの書き込み設定を無効に
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);// すべて描画する

	// アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // 有効に
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);               // 透明度が0
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // より大きいものを描画する

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{// パーティクルの総数分繰り返し
		if (g_aParticle[nCntParticle].bUse == true)
		{// パーティクルが使用されている
		    // ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aParticle[nCntParticle].mtxWorld);

			// 向きをカメラの正面に設定
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			g_aParticle[nCntParticle].mtxWorld._11 = mtxView._11;
			g_aParticle[nCntParticle].mtxWorld._12 = mtxView._21;
			g_aParticle[nCntParticle].mtxWorld._13 = mtxView._31;
			g_aParticle[nCntParticle].mtxWorld._21 = mtxView._12;
			g_aParticle[nCntParticle].mtxWorld._22 = mtxView._22;
			g_aParticle[nCntParticle].mtxWorld._23 = mtxView._32;
			g_aParticle[nCntParticle].mtxWorld._31 = mtxView._13;
			g_aParticle[nCntParticle].mtxWorld._32 = mtxView._23;
			g_aParticle[nCntParticle].mtxWorld._33 = mtxView._33;

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
			D3DXMatrixMultiply(&g_aParticle[nCntParticle].mtxWorld, &g_aParticle[nCntParticle].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aParticle[nCntParticle].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureParticle);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * 4, NUM_POLYGON);
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	// Zテストを無効にする(デフォルトの値に)
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);      // 深度バッファへの書き込みを有効に
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL); // 以下のものを描画する
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		PARTICLE_TEXTURE_NAME,
		&g_pTextureParticle);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{// パーティクルの総数分繰り返し
	    // 頂点座標
		pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

		// 法線ベクトル
		pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
		pVtx[1].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
		pVtx[2].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
		pVtx[3].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

		// 頂点カラー
		pVtx[0].col = g_aParticle[nCntParticle].col;
		pVtx[1].col = g_aParticle[nCntParticle].col;
		pVtx[2].col = g_aParticle[nCntParticle].col;
		pVtx[3].col = g_aParticle[nCntParticle].col;

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;   // ポインタを進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}
//=============================================================================
// エフェクトの設定
//=============================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 move, float radius, int life)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{// パーティクルの総数分繰り返し
		if (g_aParticle[nCntParticle].bUse == false)
		{// パーティクルが使用されていない
			g_aParticle[nCntParticle].pos = pos;         // 位置を設定
			g_aParticle[nCntParticle].move = move;       // 移動量を設定
			g_aParticle[nCntParticle].col = col;         // 色を設定
			g_aParticle[nCntParticle].fRadius = radius;  // 半径を設定
			g_aParticle[nCntParticle].nLife = life;      // 寿命を設定

			// 大きさ設定
			pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, 0.0f);

			// 色設定
			pVtx[0].col = g_aParticle[nCntParticle].col;
			pVtx[1].col = g_aParticle[nCntParticle].col;
			pVtx[2].col = g_aParticle[nCntParticle].col;
			pVtx[3].col = g_aParticle[nCntParticle].col;

			g_aParticle[nCntParticle].nIdxShadow = SetShadow(D3DXVECTOR3(g_aParticle[nCntParticle].pos.x, 0.0f, g_aParticle[nCntParticle].pos.z), g_aParticle[nCntParticle].rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.05f), 5, 5); // 影の番号を設定
			g_aParticle[nCntParticle].bUse = true;
			break;
		}
		pVtx += 4;  // ポインタを進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}