//=============================================================================
//
// パーティクルの処理 [particle.cpp]
// Author :	Takahiro
//
//=============================================================================
#include "particle.h"
#include "player.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_PARTICLE	  "data/TEXTURE/effect000.jpg"    // 読み込むテクスチャファイル名
#define PARTICLE_TEXTURESIZE  (10)                            // ポリゴンのサイズ
#define	MAX_PARTICLE		  (44096)						  // パーティクルの最大数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 move;		// 移動量
	D3DXCOLOR col;			// 色
	int nRadius;			// 半径(大きさ)
	int nLife;				// 表示時間(寿命)
	bool bUse;				// 使用しているかどうか
} PARTICLE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;	 // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	 // 頂点バッファへのポインタ
PARTICLE			    g_aParticle[MAX_PARTICLE];   // パーティクルの情報

//=============================================================================
// 初期化処理
//=============================================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{// パーティクルの数だけ繰り返し
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		g_aParticle[nCntParticle].nRadius = 0;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].bUse = false;
	}
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_PARTICLE,
		&g_pTextureParticle);

	// 頂点情報の作成
	MakeVertexParticle(pDevice);
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
	Player *pPlayer;
	pPlayer = GetPlayer();
	// 頂点情報の作成
	VERTEX_2D *pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	// ポリゴンの描画
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos.x -= pPlayer->move.x;
		// パーティクルの数だけ繰り返し
		if (g_aParticle[nCntParticle].bUse == true)
		{// パーティクルが使用されている
			g_aParticle[nCntParticle].nLife--;
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			g_aParticle[nCntParticle].nRadius--;
			g_aParticle[nCntParticle].col.r -= 0.005f;
			g_aParticle[nCntParticle].col.g -= 0.02f;
			g_aParticle[nCntParticle].col.b -= 0.05f;
			g_aParticle[nCntParticle].col.a -= 0.005f;

			if (g_aParticle[nCntParticle].nLife <= 0)
			{// 一定時間経過
				if (g_aParticle[nCntParticle].nRadius <= 0)
				{// 半径がなくなる
					g_aParticle[nCntParticle].bUse = false;
				}
			}

			// 頂点座標の更新
			pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.z);

			// 色情報の更新
			pVtx[0].col = D3DXCOLOR(g_aParticle[nCntParticle].col);
			pVtx[1].col = D3DXCOLOR(g_aParticle[nCntParticle].col);
			pVtx[2].col = D3DXCOLOR(g_aParticle[nCntParticle].col);
			pVtx[3].col = D3DXCOLOR(g_aParticle[nCntParticle].col);

		}
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureParticle);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ポリゴンの描画
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{// パーティクルの数だけ繰り返し
		if (g_aParticle[nCntParticle].bUse == true)
		{// パーティクルが使用されている
		 // テクスチャの読み込み
			pDevice->SetTexture(0, g_pTextureParticle);

			// パーティクルの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntParticle * 4,
				2);
		}
	}

	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 頂点の作成
//=============================================================================
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{// パーティクルの数だけ繰り返し
	 // 頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.z);

		// 頂点テクスチャ
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}


//=============================================================================
// エフェクト設定
//=============================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, int nRadius, int nLife)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;
	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{// パーティクルの数だけ繰り返し
		if (g_aParticle[nCntParticle].bUse == false)
		{// パーティクルが使用されていない
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].move = move;
			g_aParticle[nCntParticle].col = col;

			// 色情報の設定
			pVtx[0].col = D3DXCOLOR(g_aParticle[nCntParticle].col);
			pVtx[1].col = D3DXCOLOR(g_aParticle[nCntParticle].col);
			pVtx[2].col = D3DXCOLOR(g_aParticle[nCntParticle].col);
			pVtx[3].col = D3DXCOLOR(g_aParticle[nCntParticle].col);

			g_aParticle[nCntParticle].nRadius = nRadius;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.y - g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x - g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].pos.x + g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.y + g_aParticle[nCntParticle].nRadius, g_aParticle[nCntParticle].pos.z);

			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].bUse = true;
			break;
		}
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}