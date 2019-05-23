//=============================================================================
//
// パーティクル処理 [Particle.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "particle.h"
#include "shadow.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PARTICLE_TEXTURENAME0	 "data\\TEXTURE\\Particle000.png"	//テクスチャのファイル名
#define PARTICLE_TEXTURENAME1	 "data\\TEXTURE\\effect000.jpg"	//テクスチャのファイル名

#define TEX_POS_X_INIT			(1.0f)								//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)								//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)								//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)								//テクスチャ座標U右
#define TEX_TOP					(0.0f)								//テクスチャ座標V上
#define TEX_BOT					(1.0f)								//テクスチャ座標V下
#define MAX_PARTICLE			(7200)								//パーティクルの最大数
#define MAX_TEXTURE				(2)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9	g_pTextureParticle[MAX_TEXTURE] = {};		//テクスチャへのポインタ
PARTICLE g_aParticle[MAX_PARTICLE];

//=============================================================================
// 初期化処理
//=============================================================================
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスの取得
	pDevice = GetDevice();	// デバイスの取得

	//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			PARTICLE_TEXTURENAME0,
			&g_pTextureParticle[0]);
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			PARTICLE_TEXTURENAME1,
			&g_pTextureParticle[1]);
	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_PARTICLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffParticle,
		NULL);

	VERTEX_3D *pVtx;
	//頂点バッファをロック
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
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

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].fHeght = 0.0f;
		g_aParticle[nCntParticle].fWidth = 0.0f;
		g_aParticle[nCntParticle].bUse = false;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitParticle(void)
{	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		if (g_pTextureParticle[nCnt] != NULL)
		{
			g_pTextureParticle[nCnt]->Release();
			g_pTextureParticle[nCnt] = NULL;
		}
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
	//頂点情報へのポインタ
	VERTEX_3D *pVtx;
	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)	//パーティクルが使用されている
		{
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;
			g_aParticle[nCntParticle].nLife -= 1;
			g_aParticle[nCntParticle].fRadius -= 0.5f;
		}
		if (g_aParticle[nCntParticle].nLife <= 0)	//パーティクルのライフが０
		{
			g_aParticle[nCntParticle].bUse = false;
		}
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius,g_aParticle[nCntParticle].fRadius, 0);
		pVtx[1].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius,g_aParticle[nCntParticle].fRadius, 0);
		pVtx[2].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius,-g_aParticle[nCntParticle].fRadius, 0);
		pVtx[3].pos = D3DXVECTOR3(g_aParticle[nCntParticle].fRadius,-g_aParticle[nCntParticle].fRadius, 0);

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX  mtxView, mtxTrans;				//計算用マトリックス

	//アルファテスト(透明色を描画しないように)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//Zバッファの設定
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aParticle[nCntParticle].mtrxWorldParticle);
		//ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		//逆行列を設定
		g_aParticle[nCntParticle].mtrxWorldParticle._11 = mtxView._11;
		g_aParticle[nCntParticle].mtrxWorldParticle._12 = mtxView._21;
		g_aParticle[nCntParticle].mtrxWorldParticle._13 = mtxView._31;
		g_aParticle[nCntParticle].mtrxWorldParticle._21 = mtxView._12;
		g_aParticle[nCntParticle].mtrxWorldParticle._22 = mtxView._22;
		g_aParticle[nCntParticle].mtrxWorldParticle._23 = mtxView._32;
		g_aParticle[nCntParticle].mtrxWorldParticle._31 = mtxView._13;
		g_aParticle[nCntParticle].mtrxWorldParticle._32 = mtxView._23;
		g_aParticle[nCntParticle].mtrxWorldParticle._33 = mtxView._33;
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);
		D3DXMatrixMultiply(&g_aParticle[nCntParticle].mtrxWorldParticle, &g_aParticle[nCntParticle].mtrxWorldParticle, &mtxTrans);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aParticle[nCntParticle].mtrxWorldParticle);
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffParticle, 0, sizeof(VERTEX_3D));
		//頂点フォーマットを設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureParticle[g_aParticle[nCntParticle].nTexType]);

		if (g_aParticle[nCntParticle].bUse == true)	//パーティクルが使用されている
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				0 + (4 * nCntParticle),	//開始する頂点のインデックス
				2); //描画するプリミティブ数
		}
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
void MakeVertexParticle(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定

}

//=============================================================================
// パーティクルの設定処理
//=============================================================================
void SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nLife, PARTICLE_TYPE type)
{
	int nCntParticle;
	int nParticleTotal = 0;	//1回パーティクルの最大数
	int RotRand;
	int RotRandY;
	int ColRandR;
	int ColRandG;
	int ColRandB;
	float ColR = 0;
	float ColG = 0;
	float ColB = 0;
	D3DXCOLOR(0, 0, 0, 0);

	VERTEX_3D *pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffParticle->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)	//パーティクルが使用されていない
		{
			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].fRadius = fRadius;
			g_aParticle[nCntParticle].nLife = nLife;
			g_aParticle[nCntParticle].ParticleType = type; 
			g_aParticle[nCntParticle].nTexType = 0;
			if (g_aParticle[nCntParticle].ParticleType == PARTICLE_TYPE_GOAL)
			{
				g_aParticle[nCntParticle].nTexType = 1;
			}

			RotRand = rand() % 30;
			RotRandY = rand() % 314;
			
			g_aParticle[nCntParticle].move.x += sinf(rot.y + RotRand) * move.x;
			g_aParticle[nCntParticle].move.y += tanf(rot.x + RotRandY) * move.y;
			g_aParticle[nCntParticle].move.z += cosf(rot.y + RotRand) * move.z;
			
			//if (g_aParticle[nCntParticle].move.y <= 0)
			//{
			//	g_aParticle[nCntParticle].move.y *= -1;
			//}

			ColRandR = rand() % 10;
			ColRandG = rand() % 10;
			ColRandB = rand() % 10;

			ColR += ColRandR;
			ColG += ColRandG;
			ColB += ColRandB;

			ColR = ColR / 10;
			ColG = ColG / 10;
			ColB = ColB / 10;

			D3DXCOLOR(ColR, ColG, ColB, 1.0f);

			g_aParticle[nCntParticle].col = col;

			if (type == PARTICLE_TYPE_GOAL)
			{
				//頂点カラー
				pVtx[0].col = D3DXCOLOR(ColR, ColG, ColB, 1.0f);
				pVtx[1].col = D3DXCOLOR(ColR, ColG, ColB, 1.0f);
				pVtx[2].col = D3DXCOLOR(ColR, ColG, ColB, 1.0f);
				pVtx[3].col = D3DXCOLOR(ColR, ColG, ColB, 1.0f);
			}
			if (type == PARTICLE_TYPE_PIZZA_DEATH)
			{
				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
			nParticleTotal++;
			g_aParticle[nCntParticle].bUse = true;

			//1回のパーティクル最大数
			if (nParticleTotal >= 15)
			{
				nParticleTotal = 0;
				break;
			}		
		}
		pVtx += 4;	//頂点データのポインタを4つ分進める	
	}
	//頂点バッファをアンロックする
	g_pVtxBuffParticle->Unlock();
}
