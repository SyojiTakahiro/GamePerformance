//=============================================================================
//
// 空の処理 [sky.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "sky.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SKY_TEXTURE_NAME0   "data/TEXTURE/sky000.jpg"  // 読み込むテクスチャファイル名
#define DOME_LENGTH         (100)                      // 縦の長さ
#define DOME_SIDE           (50)                       // 横の長さ
#define DOME_RADIUS         (5000.0f)                  // 半径の長さ
#define DOME_HEIGHT         (3000.0f)                  // 高さの長さ
#define BLOCK_LENGTH        (4)                        // 縦の分割数
#define BLOCK_SIDE          (10)                       // 横の分割数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexSky(LPDIRECT3DDEVICE9 pDevice);
void MakeVertexSkyCover(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexSky(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9      g_pTextureSky = NULL;    // テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSky[2] = {};   // 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffSky = NULL;    // インデックスバッファへのポインタ
Sky                     g_Sky;                   // 空の情報
float                   g_Addtex = 0.0f;

//=============================================================================
// 初期化処理
//=============================================================================
void InitSky(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();   // デバイスの取得

	g_Sky.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 中心座標を初期化
	g_Sky.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // 向きを初期化

	g_Sky.Length = DOME_LENGTH;             // 縦の長さを初期化
	g_Sky.Side = DOME_SIDE;                 // 横の長さを初期化
	g_Sky.XBlock = BLOCK_SIDE;              // 横の分割数を設定
	g_Sky.YBlock = BLOCK_LENGTH;            // 縦の分割数を設定
	g_Sky.nNumVertex = (g_Sky.XBlock + 1) * (g_Sky.YBlock + 1); // 頂点数を設定
	g_Sky.nNumIndex = ((g_Sky.XBlock + 1) * (g_Sky.YBlock + 1)) + (2 * (g_Sky.YBlock - 1)) + (g_Sky.XBlock + 1) * (g_Sky.YBlock - 1);  // インデックス数を設定
	g_Sky.nNumPolygon = ((g_Sky.XBlock * g_Sky.YBlock) * 2) + ((g_Sky.YBlock - 1) * 4);  // 描画するポリゴン数を設定

	// 頂点情報の作成
	MakeVertexSky(pDevice);
	MakeVertexSkyCover(pDevice);

	// インデックス情報の作成
	MakeIndexSky(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSky(void)
{
	// テクスチャの開放
	if (g_pTextureSky != NULL)
	{
		g_pTextureSky->Release();
		g_pTextureSky = NULL;
	}


	// 頂点バッファの開放
	for (int nCntVertex = 0; nCntVertex < 2; nCntVertex++)
	{
		if (g_pVtxBuffSky[nCntVertex] != NULL)
		{
			g_pVtxBuffSky[nCntVertex]->Release();
			g_pVtxBuffSky[nCntVertex] = NULL;
		}
	}

	// インデックスバッファの開放
	if (g_pIdxBuffSky != NULL)
	{
		g_pIdxBuffSky->Release();
		g_pIdxBuffSky = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateSky(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffSky[0]->Lock(0, 0, (void**)&pVtx, 0);

	g_Addtex += 0.00002f;  // 加える分を進める

	for (int nCntV = 0; nCntV < g_Sky.YBlock + 1; nCntV++)
	{//垂直方向の分割数だけ繰り返し
		for (int nCntH = 0; nCntH < g_Sky.XBlock + 1; nCntH++)
		{//水平方向の分割数だけ繰り返し
			// テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(0.0f + (nCntH * (1.0f / g_Sky.XBlock)) + g_Addtex, 0.0f - (nCntV * (1.0f / g_Sky.YBlock)));
		}
		pVtx += g_Sky.XBlock + 1;       // 頂点の先頭の番号を進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffSky[0]->Unlock();

	g_Sky.rot.y += 0.00002f;
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawSky(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;                // 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Sky.mtxWorld);

	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Sky.rot.y, g_Sky.rot.x, g_Sky.rot.z);
	D3DXMatrixMultiply(&g_Sky.mtxWorld, &g_Sky.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Sky.pos.x, g_Sky.pos.y, g_Sky.pos.z);
	D3DXMatrixMultiply(&g_Sky.mtxWorld, &g_Sky.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Sky.mtxWorld);

	//-------------
	//  壁の描画
	//-------------

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffSky[0], 0, sizeof(VERTEX_3D));

	// インデックスバッファをデバイスのデータストリームにバインド
	pDevice->SetIndices(g_pIdxBuffSky);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureSky);


	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_Sky.nNumVertex, 0, g_Sky.nNumPolygon);

	//-------------
	//  蓋の描画
	//-------------

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffSky[1], 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, g_Sky.XBlock);
}

//=============================================================================
// 頂点情報の作成(壁)
//=============================================================================
void MakeVertexSky(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_Sky.nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSky[0],
		NULL);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		SKY_TEXTURE_NAME0,
		&g_pTextureSky);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffSky[0]->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;   // 角度
	float fRadius = 0;  // 半径
	D3DXVECTOR3 pos;    // 座標

	fAngle = 0.0f;                         // 角度は0に
	fRadius = cosf(0.0f) * DOME_RADIUS;    // 半径を設定
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // 座標を設定

	for (int nCntV = 0; nCntV < g_Sky.YBlock + 1; nCntV++)
	{//垂直方向の分割数だけ繰り返し
		for (int nCntH = 0; nCntH < g_Sky.XBlock + 1; nCntH++)
		{//水平方向の分割数だけ繰り返し
			pos.x = sinf(-fAngle * D3DX_PI) * fRadius;
			pos.y = sinf(0.15f * nCntV) * DOME_HEIGHT - 200.0f;
			pos.z = cosf(-fAngle * D3DX_PI) * fRadius;

			// 頂点座標
			pVtx[nCntH].pos = pos;

			// 法線ベクトル
			pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

			// 頂点カラー
			pVtx[nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標
			pVtx[nCntH].tex = D3DXVECTOR2(0.0f + 1.0f / (g_Sky.XBlock / 2), 0.0f + (1.0f * nCntV));

			fAngle += 1.0f / (g_Sky.XBlock / 2);  // 角度を進める

			if (fAngle >= 1.0f)
			{// 倍率が1を超えた
				fAngle = -1.0f;
			}
		}
		fRadius = cosf(0.26f * (nCntV + 1)) * DOME_RADIUS;   // 半径を徐々に減少させる
		fAngle = 0.0f;                                       // 角度は0に
		pVtx += g_Sky.XBlock + 1;                            // 頂点の先頭の番号を進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffSky[0]->Unlock();
}
//=============================================================================
// 頂点情報の作成(蓋)
//=============================================================================
void MakeVertexSkyCover(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (g_Sky.XBlock + 2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSky[1],
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;
	D3DXCOLOR col = D3DXCOLOR(0.0f, 0.0f, 0.33f, 1.0f);

	if (GetMode() == MODE_RANKING)
	{// ランキング画面だったら
		col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffSky[1]->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標
	pVtx[0].pos = D3DXVECTOR3(0.0f, DOME_HEIGHT, 0.0f);

	// 法線ベクトル
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

	// 頂点カラー
	pVtx[0].col = col;

	float fAngle = 0;   // 角度
	float fRadius = 0;  // 半径
	D3DXVECTOR3 pos;    // 座標

	fAngle = 0.0f;                                         // 角度は0に
	fRadius = cosf(0.20f * (g_Sky.YBlock)) * DOME_RADIUS;  // 半径を設定
	pos = D3DXVECTOR3(0.0f, sinf(0.15f * (g_Sky.YBlock)) * DOME_HEIGHT - 200.0f, 0.0f);  // 座標を設定

	for (int nCntVer = 1; nCntVer < (g_Sky.XBlock + 2); nCntVer++)
	{// 水平方向の分割数だけ繰り返し
	    // 頂点座標
		pos.x = sinf(-fAngle * D3DX_PI) * fRadius;
		pos.z = cosf(-fAngle * D3DX_PI) * fRadius;

		pVtx[nCntVer].pos = pos;

		// 法線ベクトル
		pVtx[nCntVer].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

		// 頂点カラー
		pVtx[nCntVer].col = col;

		fAngle += 1.0f / (g_Sky.XBlock / 2);  // 角度を進める

		if (fAngle >= 1.0f)
		{// 倍率が1を超えた
			fAngle = -1.0f;
		}
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffSky[1]->Unlock();
}
//=============================================================================
// インデックス情報の作成
//=============================================================================
void MakeIndexSky(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_Sky.nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSky,
		NULL);

	WORD *pIdx;      // インデックスデータへのポインタ
	int nCntIdx = 0; // インデックス番号

	// インデックスバッファをロックし,インデックスデータへのポインタを取得
	g_pIdxBuffSky->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < g_Sky.YBlock; nCntIdxY++)
	{// 縦の分割数分繰り返し
		for (int nCntIdxX = 0; nCntIdxX < g_Sky.XBlock + 1; nCntIdxX++, nCntIdx++)
		{// 横の分割数+１繰り返し
		    // 上下の二つ分インデックス番号を設定
			pIdx[0] = nCntIdx + g_Sky.XBlock + 1; // 下側
			pIdx[1] = nCntIdx;                    // 上側

			pIdx += 2;  // 2つ分進める
			if (nCntIdxY < g_Sky.YBlock && nCntIdxX == g_Sky.XBlock)
			{// 1 , 下側が縦の分割数の最下層ではない
			 // 2 , 横の分割数分設定が終わった
				pIdx[0] = nCntIdx;                          // 上側
				pIdx[1] = nCntIdx + (g_Sky.XBlock + 1) + 1; // 次の下側

				pIdx += 2; // 2つ分進める
			}
		}
	}

	// インデックスバッファをアンロックする
	g_pIdxBuffSky->Unlock();
}