//=============================================================================
//
// メッシュフィールド処理 [meshfield.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "meshField.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMeshfield(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshfield = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9			g_pTextureMeshfield = NULL;	//テクスチャへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;		//インデックスバッファへのポインタ
D3DXVECTOR3 g_posMeshfield;								//位置
D3DXVECTOR3 g_rotMeshfield;								//向き
D3DXMATRIX	g_mtrxWorldMeshfield;						//ワールドマトリックス
int g_nNumVertexMeshField;								//頂点数
int g_nNumIndexMeshField;								//インデックス数
int g_nNumPolygonMeshField;								//ポリゴン数
//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスの取得
	pDevice = GetDevice();	// デバイスの取得

	int nCntXvtx = 0;
	int nCntZvtx = 0;
	int nCntVtx = 0;
	float fPos_X = 0;
	float fPos_Z = 0;
	float fXpos = -X_POSSIZE;
	float fZpos = Z_POSSIZE;
	//int nIdxX = 0;
	//int nIdxY = 0;
	int nCntIdx = 0;

	g_nNumVertexMeshField = (X_FIELD_SIZE + 1) * (Z_FIELD_SIZE + 1);
	g_nNumIndexMeshField = ((X_FIELD_SIZE + 1) * (Z_FIELD_SIZE + 1) + ((Z_FIELD_SIZE - 1) * 2) + (X_FIELD_SIZE + 1) * (Z_FIELD_SIZE - 1));

	//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			MESHFIELD_TEXTURENAME,
			&g_pTextureMeshfield);
			// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) *  g_nNumVertexMeshField,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshfield,
		NULL);

	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexMeshField,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,	//2byteのデータ
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);


	VERTEX_3D *pVtx;
	//頂点バッファをロック
	g_pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);
	
	//座標を中心にする
	fPos_X = (fXpos * X_FIELD_SIZE) / 2;
	fPos_Z = (fZpos * Z_FIELD_SIZE) / 2;

	for (nCntZvtx = 0; nCntZvtx < Z_FIELD_SIZE + 1; nCntZvtx++)
	{
		for (nCntXvtx = 0; nCntXvtx < X_FIELD_SIZE + 1; nCntXvtx++)
		{
			//頂点情報の設定
			pVtx[(nCntZvtx + nCntXvtx) + nCntVtx].pos = D3DXVECTOR3(fPos_X + (nCntXvtx * X_POSSIZE), 0.0f, fPos_Z -(nCntZvtx * Z_POSSIZE) );
			//法線の設定
			pVtx[(nCntZvtx + nCntXvtx) + nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			//頂点カラー
			pVtx[(nCntZvtx + nCntXvtx )+ nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			//テクスチャ座標
			pVtx[(nCntZvtx + nCntXvtx )+ nCntVtx].tex = D3DXVECTOR2(0 + (nCntXvtx * 0.5f), 0 + (nCntZvtx * 0.5f));
		}
		//頂点情報の値を保持
		nCntVtx += X_FIELD_SIZE;	
	}

	//頂点バッファをアンロックする

	g_pVtxBuffMeshfield->Unlock();
	
	//インデックスデータへのポインタ
	WORD *pIdx;
	//インデックスバッファをロック
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxZ = 0; nCntIdxZ < Z_FIELD_SIZE; nCntIdxZ++)
	{
		for (int nCntIdxX = 0; nCntIdxX < X_FIELD_SIZE + 1;nCntIdxX++,nCntIdx++)
		{
			pIdx[0] = nCntIdx + X_FIELD_SIZE + 1;
			pIdx[1] = nCntIdx;
			//pIdxを2個分ずらす
			pIdx += 2;
			if (nCntIdxZ < Z_FIELD_SIZE && nCntIdxX == X_FIELD_SIZE)
			{
				pIdx[0] = nCntIdx;
				pIdx[1] = nCntIdx + (X_FIELD_SIZE + 1) + 1;
				//pIdxを2個分ずらす
				pIdx += 2;
			}
		}
	}

	//インデックス(途中)

	//for (int nCnt = 0; nCnt < g_nNumIndexMeshField ; nCnt++)
	//{
	//	if (nCnt < X_FIELD_SIZE + 1)
	//	{
	//		pIdx[0 + (nCnt * 2)] = X_FIELD_SIZE + 1 + nCnt;
	//		pIdx[1 + (nCnt * 2)] = (X_FIELD_SIZE + 1 + nCnt) - (X_FIELD_SIZE + 1);
	//	}
	//	else if (nCnt > X_FIELD_SIZE + 1)
	//	{
	//		pIdx[0 + (nCnt * 2)] = (X_FIELD_SIZE  + 1)* 2 + nIdxX;
	//		pIdx[1 + (nCnt * 2)] = X_FIELD_SIZE  + 1 + nIdxX;
	//		nIdxX++;
	//	}
	//	if (nCnt == X_FIELD_SIZE)
	//	{
	//		nCnt++;
	//		pIdx[0 + (nCnt * 2)] = X_FIELD_SIZE;
	//		pIdx[1 + (nCnt * 2)] = X_FIELD_SIZE + (X_FIELD_SIZE * X_FIELD_SIZE);
	//		nIdxY = 1;
	//	}
	//}
	//pIdx[0] = 3;
	//pIdx[1] = 0;
	//pIdx[2] = 4;
	//pIdx[3] = 1;
	//pIdx[4] = 5;
	//pIdx[5] = 2;
	//pIdx[6] = 2;
	//pIdx[7] = 6;
	//pIdx[8] = 6;
	//pIdx[9] = 3;
	//pIdx[10] = 7;
	//pIdx[11] = 4;
	//pIdx[12] = 8;
	//pIdx[13] = 5;
	//インデックスバッファをロックする

	g_pIdxBuffMeshField->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshField(void)
{	//テクスチャの破棄
	if (g_pTextureMeshfield != NULL)
	{
		g_pTextureMeshfield->Release();
		g_pTextureMeshfield = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffMeshfield != NULL)
	{
		g_pVtxBuffMeshfield->Release();
		g_pVtxBuffMeshfield = NULL;
	}
	// インデックスバッファの開放
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshField(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_mtrxWorldMeshfield);
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotMeshfield.y, g_rotMeshfield.x, g_rotMeshfield.z);
	D3DXMatrixMultiply(&g_mtrxWorldMeshfield, &g_mtrxWorldMeshfield, &mtxRot);
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_posMeshfield.x, g_posMeshfield.y, g_posMeshfield.z);
	D3DXMatrixMultiply(&g_mtrxWorldMeshfield, &g_mtrxWorldMeshfield, &mtxTrans);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_mtrxWorldMeshfield);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshField);
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshfield);

	//// ポリゴンの描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
	//	0,	//開始する頂点のインデックス
	//	g_nNumVertexMeshField - 2); //描画するプリミティブ数
	
	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexMeshField, 0, g_nNumIndexMeshField - 2);
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshfield(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定

}

