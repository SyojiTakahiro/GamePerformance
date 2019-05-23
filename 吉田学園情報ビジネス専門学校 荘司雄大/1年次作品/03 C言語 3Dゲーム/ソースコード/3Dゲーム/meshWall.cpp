//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : 
//
//=============================================================================
#include "MeshWall.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_Z			(0)					//8
#define MAX_X			(0)				//10
#define MAX_Y			(1)
#define MAX_VERTICAL	(1)					//垂直方向の分割数
#define MAX_LEVEL		(1)					//水平方向の分割数
#define MAX_WALL		(4)
#define	TEXTURE_NAME	"data/TEXTURE/block005.jpg"
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexMeshWall(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureMeshWall = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;	//頂点バッファのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshWall = NULL;	//インデックスバッファのポインタ
int g_nNumVertexMeshWall;							//頂点数
int g_nNumIndexMeshWall;							//インデックス数
int g_nNumPolygonMeshWall;							//ポリゴン数
MeshWall g_Wall[MAX_WALL];
//=============================================================================
// 初期化処
//=============================================================================
void InitMeshWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		// デバイスの取得

	int nCntIdx_z, nCntIdx_x,nCountIdx = 0; 

	for (int nCount = 0; nCount < MAX_WALL; nCount++)
	{
		if (nCount == 0)
		{
			g_Wall[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Wall[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f) * D3DX_PI;
		}
		if (nCount == 1)
		{
			g_Wall[nCount].pos = D3DXVECTOR3(400.0f, 0.0f, 0.0f);
			g_Wall[nCount].rot = D3DXVECTOR3(0.0f, 0.5f, 0.0f) * D3DX_PI;
		}
		if (nCount == 2)
		{
			g_Wall[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, -400.0f);
			g_Wall[nCount].rot = D3DXVECTOR3(0.0f, 1.0f, 0.0f) * D3DX_PI;
		}
		if (nCount == 3)
		{
			g_Wall[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, -400.0f);
			g_Wall[nCount].rot = D3DXVECTOR3(0.0f, -0.5f, 0.0f) * D3DX_PI;
		}
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,TEXTURE_NAME,&g_pTextureMeshWall);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MAX_Y + 1)*(MAX_X + 1),
		D3DUSAGE_WRITEONLY, 
		FVF_VERTEX_3D, 
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshWall, 
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (((MAX_X + 1) * 2) * MAX_Y + (2 * (MAX_Y - 1))), 
		D3DUSAGE_WRITEONLY, 
		D3DFMT_INDEX16, 
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshWall,
		NULL);

	//ポインタ
	VERTEX_3D*pVtx;					//頂点情報へのポインタ
	WORD*pIdx;						//インデックスデータへのポインタ
	//バッファをロック
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	//
	for (nCntIdx_z = 0; nCntIdx_z < MAX_Y + 1; nCntIdx_z++)
	{
		for (nCntIdx_x = 0; nCntIdx_x < MAX_X + 1; nCntIdx_x++)
		{
			pVtx[0].pos = D3DXVECTOR3(0.0f + (nCntIdx_x * 50.0f), 0.0f - (nCntIdx_z * 50.0f), 0);
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[0].tex = D3DXVECTOR2(nCntIdx_x * 1.0f, nCntIdx_z * 1.0f);
			pVtx += 1;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshWall->Unlock();

	//インデックスバッファをロックする
	g_pIdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスの設定
	for (nCntIdx_z = 0; nCntIdx_z < MAX_Y ; nCntIdx_z++)
	{
		for (nCntIdx_x = 0; nCntIdx_x < MAX_X + 1; nCntIdx_x++, nCountIdx++)
		{
			pIdx[0] = nCountIdx + MAX_X + 1;
			pIdx[1] = nCountIdx;
			pIdx += 2;
			if (nCntIdx_z < MAX_Y && nCntIdx_x == MAX_X)
			{
				pIdx[0] = nCountIdx + 0;
				pIdx[1] = nCountIdx + (MAX_X + 1) + 1;

				pIdx += 2;
			}
		}
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshWall->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitMeshWall(void)
{
	//// テクスチャの開放
	if (g_pTextureMeshWall != NULL)
	{
		g_pTextureMeshWall->Release();
		g_pTextureMeshWall = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffMeshWall != NULL)
	{
		g_pVtxBuffMeshWall->Release();
		g_pVtxBuffMeshWall = NULL;
	}

	//インデックスバッファの開放
	if (g_pIdxBuffMeshWall != NULL)
	{
		g_pIdxBuffMeshWall->Release();
		g_pIdxBuffMeshWall = NULL;
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
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCount = 0; nCount < MAX_WALL; nCount++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Wall[nCount].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Wall[nCount].rot.y, g_Wall[nCount].rot.x, g_Wall[nCount].rot.z);
		D3DXMatrixMultiply(&g_Wall[nCount].mtxWorld, &g_Wall[nCount].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Wall[nCount].pos.x, g_Wall[nCount].pos.y, g_Wall[nCount].pos.z);
		D3DXMatrixMultiply(&g_Wall[nCount].mtxWorld, &g_Wall[nCount].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Wall[nCount].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pIdxBuffMeshWall);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshWall);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
			0, 
			0,
			(MAX_Y + 1)*(MAX_X + 1), 
			0, 
			(MAX_X*MAX_Y * 2 + (4 * (MAX_Y - 1))));

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

