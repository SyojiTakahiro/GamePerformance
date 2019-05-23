//=============================================================================
//
// メッシュウォール処理 [MESHWALL.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "meshWall.h"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice);
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall[MAX_MESHWALL] = {};		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureMeshWall[MAX_MESHWALL] = {};		//テクスチャへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshWall[MAX_MESHWALL] = {};		//インデックスバッファへのポインタ
//D3DXVECTOR3 g_aMashWall[nCnt].pos;								//位置
//D3DXVECTOR3 g_aMashWall[nCnt].rot;								//向き
//D3DXMATRIX	g_aMashWall[nCnt].mtrxWorld;						//ワールドマトリックス
int g_nNumVertexMeshWall;								//頂点数
int g_nNumIndexMeshWall;								//インデックス数
int g_nNumPolygonMeshWall;								//ポリゴン数
MESHWALL g_aMashWall[MAX_MESHWALL];
//=============================================================================
// 初期化処理
//=============================================================================
void InitMeshWall(int nIdxMeshWall, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスの取得
	pDevice = GetDevice();	// デバイスの取得

	int nCntXvtx = 0;
	int nCntZvtx = 0;
	int nCntVtx = 0;	
	int nCntIdx = 0;

	g_nNumVertexMeshWall = (X_WALLSIZE + 1) * (Y_WALLSIZE + 1);
	g_nNumIndexMeshWall = ((X_WALLSIZE + 1) * (Y_WALLSIZE + 1) + ((Y_WALLSIZE - 1) * 2) + (X_WALLSIZE + 1) * (Y_WALLSIZE - 1));

	//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			MESHWALL_TEXTURENAME,
			&g_pTextureMeshWall[nIdxMeshWall]);
	// 頂点情報の作成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) *  g_nNumVertexMeshWall,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall[nIdxMeshWall],
		NULL);
	// インデックス情報の作成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexMeshWall,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,	//2byteのデータ
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshWall[nIdxMeshWall],
		NULL);

	VERTEX_3D *pVtx;
	//頂点バッファをロック
	g_pVtxBuffMeshWall[nIdxMeshWall]->Lock(0, 0, (void**)&pVtx, 0);
	
	//座標を中心にする
	//g_aMashWall[nIdxMeshWall].fPos_X = (g_aMashWall[nIdxMeshWall].fXpos * X_WALLSIZE) / 2;
	//g_aMashWall[nIdxMeshWall].fPos_Y = (g_aMashWall[nIdxMeshWall].fYpos * Y_WALLSIZE) / 2;


	for (nCntZvtx = 0; nCntZvtx < Y_WALLSIZE + 1; nCntZvtx++)
	{
		for (nCntXvtx = 0; nCntXvtx < X_WALLSIZE + 1; nCntXvtx++)
		{
			//頂点情報の設定
			pVtx[(nCntZvtx + nCntXvtx) + nCntVtx + g_aMashWall[nIdxMeshWall].nVtxIndex].pos = D3DXVECTOR3(g_aMashWall[nIdxMeshWall].fPos_X + (nCntXvtx * X_POSSIZE), g_aMashWall[nIdxMeshWall].fPos_Y - (nCntZvtx * Y_POSSIZE), 0.0f);
			//法線の設定
			pVtx[(nCntZvtx + nCntXvtx) + nCntVtx + g_aMashWall[nIdxMeshWall].nVtxIndex].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			//頂点カラー
			pVtx[(nCntZvtx + nCntXvtx) + nCntVtx + g_aMashWall[nIdxMeshWall].nVtxIndex].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f);
			//テクスチャ座標
			pVtx[(nCntZvtx + nCntXvtx) + nCntVtx + g_aMashWall[nIdxMeshWall].nVtxIndex].tex = D3DXVECTOR2(0 + (nCntXvtx * 0.5f), 0 + (nCntZvtx * 0.5f));
		}
		//頂点情報の値を保持
		nCntVtx += X_WALLSIZE;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffMeshWall[nIdxMeshWall]->Unlock();
	
	//インデックスデータへのポインタ
	WORD *pIdx;
	//インデックスバッファをロック
	g_pIdxBuffMeshWall[nIdxMeshWall]->Lock(0, 0, (void**)&pIdx, 0);

	

#if 0
	for (int nCntIdxZ = 0; nCntIdxZ < Y_WALLSIZE; nCntIdxZ++)
	{
		for (int nCntIdxX = 0; nCntIdxX < X_WALLSIZE + 1; nCntIdxX++, nCntIdx++)
		{
			pIdx[0] = nCntIdx + X_WALLSIZE + 1;
			pIdx[1] = nCntIdx;
			//pIdxを2個分ずらす
			pIdx += 2;
			if (nCntIdxZ < Y_WALLSIZE && nCntIdxX == X_WALLSIZE)
			{
				pIdx[0] = nCntIdx;
				pIdx[1] = nCntIdx + (X_WALLSIZE + 1) + 1;
				//pIdxを2個分ずらす
				pIdx += 2;
			}
		}
	}
#endif

	//インデックス(InitOnly)
#if 1	
	for (int nCntIdxZ = 0; nCntIdxZ < Y_WALLSIZE; nCntIdxZ++)
	{
		for (int nCntIdxX = 0; nCntIdxX < X_WALLSIZE + 1; nCntIdxX++, nCntIdx++)
		{
			pIdx[0 + g_aMashWall[nIdxMeshWall].nIdxIndex] = nCntIdx + X_WALLSIZE + 1;
			pIdx[1 + g_aMashWall[nIdxMeshWall].nIdxIndex] = nCntIdx;
			//pIdxを2個分ずらす
			pIdx += 2;
			if (nCntIdxZ < Y_WALLSIZE && nCntIdxX == X_WALLSIZE)
			{
				pIdx[0 + g_aMashWall[nIdxMeshWall].nIdxIndex] = nCntIdx;
				pIdx[1 + g_aMashWall[nIdxMeshWall].nIdxIndex] = nCntIdx + (X_WALLSIZE + 1) + 1;
				//pIdxを2個分ずらす
				pIdx += 2;
			}
		}
	}
#endif
	//インデックスバッファをアンロック
	g_pIdxBuffMeshWall[nIdxMeshWall]->Unlock();

	g_aMashWall[nIdxMeshWall].pos = pos;
	g_aMashWall[nIdxMeshWall].rot = rot;
	g_aMashWall[nIdxMeshWall].bUse = true;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshWall(void)
{	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		if (g_pTextureMeshWall[nCnt] != NULL)
		{
			g_pTextureMeshWall[nCnt]->Release();
			g_pTextureMeshWall[nCnt] = NULL;
		}
		// 頂点バッファの開放
		if (g_pVtxBuffMeshWall[nCnt] != NULL)
		{
			g_pVtxBuffMeshWall[nCnt]->Release();
			g_pVtxBuffMeshWall[nCnt] = NULL;
		}
		// インデックスバッファの開放
		if (g_pIdxBuffMeshWall[nCnt] != NULL)
		{
			g_pIdxBuffMeshWall[nCnt]->Release();
			g_pIdxBuffMeshWall[nCnt] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateMeshWall(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCnt= 0; nCnt < MAX_MESHWALL; nCnt++)
	{
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aMashWall[nCnt].mtrxWorld); 
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMashWall[nCnt].rot.y, g_aMashWall[nCnt].rot.x, g_aMashWall[nCnt].rot.z);
	D3DXMatrixMultiply(&g_aMashWall[nCnt].mtrxWorld, &g_aMashWall[nCnt].mtrxWorld, &mtxRot);
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_aMashWall[nCnt].pos.x, g_aMashWall[nCnt].pos.y, g_aMashWall[nCnt].pos.z);
	D3DXMatrixMultiply(&g_aMashWall[nCnt].mtrxWorld, &g_aMashWall[nCnt].mtrxWorld, &mtxTrans);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aMashWall[nCnt].mtrxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshWall[nCnt], 0, sizeof(VERTEX_3D));
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshWall[nCnt]);
	//頂点フォーマットを設定
	pDevice->SetFVF(FVF_VERTEX_3D);
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshWall[nCnt]);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0 , g_nNumVertexMeshWall, 0, g_nNumIndexMeshWall - 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定
}

