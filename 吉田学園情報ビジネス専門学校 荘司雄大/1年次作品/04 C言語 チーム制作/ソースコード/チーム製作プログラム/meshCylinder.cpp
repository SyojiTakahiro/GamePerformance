//=============================================================================
//
// メッシュシリンダー処理 [meshCylinder.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "meshCylinder.h"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureMeshCylinder = NULL;		//テクスチャへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;		//インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshCylinder;								//位置
D3DXVECTOR3 g_rotMeshCylinder;								//向き
D3DXMATRIX	g_mtrxWorldMeshCylinder;						//ワールドマトリックス
int g_nNumVertexMeshCylinder;								//頂点数
int g_nNumIndexMeshCylinder;								//インデックス数
int g_nNumPolygonMeshCylinder;								//ポリゴン数
//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスの取得
	pDevice = GetDevice();	// デバイスの取得

	int nCntH = 0;
	int nCntV = 0;
	int nCntVtx = 0;
	float fPos_X = 0;
	float fPos_Y = 0;
	float fXpos = -X_CYLINDERPOSSIZE;
	float fYpos = Y_CYLINDERPOSSIZE;
	int nCntIdx = 0;
	float fCylinder = 0.0f;
	int  nCylinder = 0;

	g_nNumVertexMeshCylinder = (X_CYLINDERSIZE + 1) * (Y_CYLINDERSIZE + 1);
	g_nNumIndexMeshCylinder = ((X_CYLINDERSIZE + 1) * (Y_CYLINDERSIZE + 1) + ((Y_CYLINDERSIZE - 1) * 2) + (X_CYLINDERSIZE + 1) * (Y_CYLINDERSIZE - 1));
	/*g_nNumPolygonMeshCylinder = ((X_CYLINDERSIZE * 2) * 2) + (Y_CYLINDERPOSSIZE * 2);*/
	//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			MESHCYLINDER_TEXTURENAME,
			&g_pTextureMeshCylinder);
			// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) *  g_nNumVertexMeshCylinder,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshCylinder,
		NULL);

	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexMeshCylinder,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,	//2byteのデータ
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL);


	VERTEX_3D *pVtx;
	//頂点バッファをロック
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);
	
	//座標を中心にする
	fPos_X = (fXpos * X_CYLINDERSIZE) / 2;
	fPos_Y = (fYpos * Y_CYLINDERSIZE) / 2;

	for (nCntV = 0; nCntV < Y_CYLINDERSIZE + 1; nCntV++)
	{
		for (nCntH = 0; nCntH < X_CYLINDERSIZE + 1; nCntH++)
		{

			//頂点情報の設定
			pVtx[(nCntV + nCntH) + nCntVtx].pos.x = sinf(-fCylinder * D3DX_PI) * fXpos;
			pVtx[(nCntV + nCntH) + nCntVtx].pos.y = 20.0f - nCylinder;
			pVtx[(nCntV + nCntH) + nCntVtx].pos.z = cosf(fCylinder * D3DX_PI) * fXpos;

			//法線の設定
			pVtx[(nCntV + nCntH) + nCntVtx].nor = D3DXVECTOR3(sinf(fCylinder), cosf(fCylinder), sinf(fCylinder));
			D3DXVec3Normalize(&pVtx[(nCntV + nCntH) + nCntVtx].nor, &pVtx[(nCntV + nCntH) + nCntVtx].nor);
			//頂点カラー
			pVtx[(nCntV + nCntH) + nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標
			pVtx[(nCntV + nCntH) + nCntVtx].tex = D3DXVECTOR2(0 + (nCntH * 0.5f), 0 + (nCntV * 0.5f));

			//角度1を上回る
			if (fCylinder >= 1.0f)
			{
				fCylinder = -1.0f;
			}
			//Xの分割数/2で1を割る
			fCylinder += 1.0f / (X_CYLINDERSIZE / 2);

		}
		//頂点情報の値を保持
		nCntVtx += X_CYLINDERSIZE;
		nCylinder += 20;
		fCylinder = 0.0f;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshCylinder->Unlock();
	
	//インデックスデータへのポインタ
	WORD *pIdx;
	//インデックスバッファをロック
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxZ = 0; nCntIdxZ < Y_CYLINDERSIZE; nCntIdxZ++)
	{
		for (int nCntIdxX = 0; nCntIdxX < X_CYLINDERSIZE + 1;nCntIdxX++,nCntIdx++)
		{
			pIdx[0] = nCntIdx + X_CYLINDERSIZE + 1;
			pIdx[1] = nCntIdx;
			//pIdxを2個分ずらす
			pIdx += 2;
			if (nCntIdxZ < Y_CYLINDERSIZE && nCntIdxX == X_CYLINDERSIZE)
			{
				pIdx[0] = nCntIdx;
				pIdx[1] = nCntIdx + (X_CYLINDERSIZE + 1) + 1;
				//pIdxを2個分ずらす
				pIdx += 2;
			}
		}
	}

	//インデックス(途中)

	//for (int nCnt = 0; nCnt < g_nNumIndexMeshCylinder ; nCnt++)
	//{
	//	if (nCnt < X_CYLINDERSIZE + 1)
	//	{
	//		pIdx[0 + (nCnt * 2)] = X_CYLINDERSIZE + 1 + nCnt;
	//		pIdx[1 + (nCnt * 2)] = (X_CYLINDERSIZE + 1 + nCnt) - (X_CYLINDERSIZE + 1);
	//	}
	//	else if (nCnt > X_CYLINDERSIZE + 1)
	//	{
	//		pIdx[0 + (nCnt * 2)] = (X_CYLINDERSIZE  + 1)* 2 + nIdxX;
	//		pIdx[1 + (nCnt * 2)] = X_CYLINDERSIZE  + 1 + nIdxX;
	//		nIdxX++;
	//	}
	//	if (nCnt == X_CYLINDERSIZE)
	//	{
	//		nCnt++;
	//		pIdx[0 + (nCnt * 2)] = X_CYLINDERSIZE;
	//		pIdx[1 + (nCnt * 2)] = X_CYLINDERSIZE + (X_CYLINDERSIZE * X_CYLINDERSIZE);
	//		nIdxY = 1;
	//	}
	//}

	//インデックスバッファをロックする
	g_pIdxBuffMeshCylinder->Unlock();

	g_posMeshCylinder = D3DXVECTOR3(100.0f,20.0f,0.0f);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshCylinder(void)
{	//テクスチャの破棄
	if (g_pTextureMeshCylinder != NULL)
	{
		g_pTextureMeshCylinder->Release();
		g_pTextureMeshCylinder = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffMeshCylinder != NULL)
	{
		g_pVtxBuffMeshCylinder->Release();
		g_pVtxBuffMeshCylinder = NULL;
	}
	// インデックスバッファの開放
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshCylinder(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshCylinder(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtrxWorldMeshCylinder);
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshCylinder.y, g_rotMeshCylinder.x, g_rotMeshCylinder.z);
	D3DXMatrixMultiply(&g_mtrxWorldMeshCylinder, &g_mtrxWorldMeshCylinder, &mtxRot);
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshCylinder.x, g_posMeshCylinder.y, g_posMeshCylinder.z);
	D3DXMatrixMultiply(&g_mtrxWorldMeshCylinder, &g_mtrxWorldMeshCylinder, &mtxTrans);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtrxWorldMeshCylinder);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//// ポリゴンの描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
	//	0,	//開始する頂点のインデックス
	//	g_nNumVertexMeshCylinder - 2); //描画するプリミティブ数
	
	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexMeshCylinder, 0, g_nNumIndexMeshCylinder - 2);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshCylinder(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定

}

