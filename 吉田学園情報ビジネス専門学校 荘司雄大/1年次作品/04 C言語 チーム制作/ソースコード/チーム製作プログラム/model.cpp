//=============================================================================
//
// モデル処理 [model.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "model.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "polygon.h"
#include "player.h"
#include "game.h"
#include "explosion.h"
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshModel[MAX_MODEL_TYPE];						//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatModel[MAX_MODEL_TYPE];				//マテリアル情報へのポインタ
LPDIRECT3DTEXTURE9	g_pTextureModel[MAX_MODEL_TYPE];			//テクスチャへのポインタ
DWORD g_nNumMatModel[MAX_MODEL_TYPE] = {};					//マテリアル情報の数
MODEL g_aModel[MAX_MODEL];
D3DXMATERIAL *g_pMat;
VTX g_aModelVtx[MAX_MODEL_TYPE];
//=============================================================================
// 初期化処理
//=============================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_NAME000,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[0],
		NULL,
		&g_nNumMatModel[0],
		&g_pMeshModel[0]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_NAME001,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[1],
		NULL,
		&g_nNumMatModel[1],
		&g_pMeshModel[1]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_NAME002,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[2],
		NULL,
		&g_nNumMatModel[2],
		&g_pMeshModel[2]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_NAME003,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[3],
		NULL,
		&g_nNumMatModel[3],
		&g_pMeshModel[3]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_NAME004,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatModel[4],
		NULL,
		&g_nNumMatModel[4],
		&g_pMeshModel[4]);


	int nNumVtex;	//頂点数
	DWORD sizeFvF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_MODEL];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_MODEL];

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_aModel[nCntModel].VtxMinModel = D3DXVECTOR3(10000, 0, 10000);
		g_aModel[nCntModel].VtxMaxModel = D3DXVECTOR3(-10000, 0, -10000);
		g_aModel[nCntModel].nType = 0;
		g_aModel[nCntModel].bUse = false;
	}
	for (int nCntTypeModel = 0; nCntTypeModel < MAX_MODEL_TYPE; nCntTypeModel++)
	{
		g_aModelVtx[nCntTypeModel].VtxMaxModel = D3DXVECTOR3(-10000, 0, -10000);
		g_aModelVtx[nCntTypeModel].VtxMinModel = D3DXVECTOR3(10000, 0, 10000);
	}

#if 0
	for (int nCntTypeModel = 0; nCntTypeModel < MAX_MODEL_TYPE; nCntTypeModel++)
	{
		//頂点数を取得
		nNumVtex = g_pMeshModel[nCntTypeModel]->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFvF = D3DXGetFVFVertexSize(g_pMeshModel[nCntTypeModel]->GetFVF());
		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{
			//頂点　ロック
			g_pMeshModel[nCntTypeModel]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nCntModel]);
			for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
			{	//頂点座標の代入
				D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff[nCntModel];
				if (g_aModel[nCntModel].VtxMinModel.x > vtx.x)
				{
					g_aModel[nCntModel].VtxMinModel.x = vtx.x;
				}
				if (g_aModel[nCntModel].VtxMinModel.y > vtx.y)
				{
					g_aModel[nCntModel].VtxMinModel.y = vtx.y;
				}
				if (g_aModel[nCntModel].VtxMinModel.z > vtx.z)
				{
					g_aModel[nCntModel].VtxMinModel.z = vtx.z;
				}

				if (g_aModel[nCntModel].VtxMaxModel.x < vtx.x)
				{
					g_aModel[nCntModel].VtxMaxModel.x = vtx.x;
				}
				if (g_aModel[nCntModel].VtxMaxModel.y < vtx.y)
				{
					g_aModel[nCntModel].VtxMaxModel.y = vtx.y;
				}
				if (g_aModel[nCntModel].VtxMaxModel.z < vtx.z)
				{
					g_aModel[nCntModel].VtxMaxModel.z = vtx.z;
				}
				pVtxBuff[nCntModel] += sizeFvF;
			}
			//			//頂点バッファをアンロック
			g_pMeshModel[nCntTypeModel]->UnlockVertexBuffer();
		}
	}
	
#endif
	for (int nCntTypeModel = 0; nCntTypeModel < MAX_MODEL_TYPE; nCntTypeModel++)
	{
		//頂点数を取得
		nNumVtex = g_pMeshModel[nCntTypeModel]->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFvF = D3DXGetFVFVertexSize(g_pMeshModel[nCntTypeModel]->GetFVF());
		//頂点　ロック
		g_pMeshModel[nCntTypeModel]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nCntTypeModel]);
		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
		{	//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff[nCntTypeModel];
			if (g_aModelVtx[nCntTypeModel].VtxMinModel.x > vtx.x)
			{
				g_aModelVtx[nCntTypeModel].VtxMinModel.x = vtx.x;
			}
			if (g_aModelVtx[nCntTypeModel].VtxMinModel.y > vtx.y)
			{
				g_aModelVtx[nCntTypeModel].VtxMinModel.y = vtx.y;
			}
			if (g_aModelVtx[nCntTypeModel].VtxMinModel.z > vtx.z)
			{
				g_aModelVtx[nCntTypeModel].VtxMinModel.z = vtx.z;
			}

			if (g_aModelVtx[nCntTypeModel].VtxMaxModel.x < vtx.x)
			{
				g_aModelVtx[nCntTypeModel].VtxMaxModel.x = vtx.x;
			}
			if (g_aModelVtx[nCntTypeModel].VtxMaxModel.y < vtx.y)
			{
				g_aModelVtx[nCntTypeModel].VtxMaxModel.y = vtx.y;
			}
			if (g_aModelVtx[nCntTypeModel].VtxMaxModel.z < vtx.z)
			{
				g_aModelVtx[nCntTypeModel].VtxMaxModel.z = vtx.z;
			}
			pVtxBuff[nCntTypeModel] += sizeFvF;
		}
		//			//頂点バッファをアンロック
		g_pMeshModel[nCntTypeModel]->UnlockVertexBuffer();
	}


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\exclamation000.jpg",
		&g_pTextureModel[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\question000.jpg",
		&g_pTextureModel[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		MODEL_NAME002,
		&g_pTextureModel[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		MODEL_NAME003,
		&g_pTextureModel[3]);

	//for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	//{
	//	for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel[nCntModel]; nCntMat++)
	//	{
	//		// マテリアルデータへのポインタを取得
	//		g_pMat = (D3DXMATERIAL*)g_pBuffMatModel[g_aModel[nCntModel].nType]->GetBufferPointer();
	//		{
	//			D3DXCreateTextureFromFile(pDevice,
	//				g_pMat[nCntMat].pTextureFilename,
	//				&g_pTextureModel[g_aModel[nCntModel].nType]);
	//		}
	//	}
	//}

	//float testpos = 100;
	//for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	//{
	//	// 位置・向きの初期設定
	//	g_aModel[nCntModel].pos = D3DXVECTOR3(testpos, 0, 0);
	//	g_aModel[nCntModel].rot = D3DXVECTOR3(0, 0, 0);
	//	g_aModel[nCntModel].fShadow = 12;
	////	g_aModel[nCntModel].fDestAngle = 0;
	//	//g_aModel
	//	//　影のセット
	//	g_aModel[nCntModel].nIdxShadow = SetShadow(g_aModel[nCntModel].pos, D3DXVECTOR3(0, 0, 0), 20.0f);

	//	testpos -= 100;
	//	g_aModel[nCntModel].bUse = true;
	//}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL_TYPE; nCntModel++)
	{
		// メッシュの開放
		if (g_pMeshModel[nCntModel] != NULL)
		{
			g_pMeshModel[nCntModel]->Release();
			g_pMeshModel[nCntModel] = NULL;
		}
		// マテリアルの開放
		if (g_pBuffMatModel[nCntModel] != NULL)
		{
			g_pBuffMatModel[nCntModel]->Release();
			g_pBuffMatModel[nCntModel] = NULL;
		}
		//テクスチャの破棄
		if (g_pTextureModel[nCntModel] != NULL)
		{
			g_pTextureModel[nCntModel]->Release();
			g_pTextureModel[nCntModel] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModel(void)
{
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)	//モデルが使用されている
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxTrans);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].mtxWorld);
			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);


			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatModel[g_aModel[nCntModel].nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel[g_aModel[nCntModel].nType]; nCntMat++)
			{	// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				// テクスチャ

				pDevice->SetTexture(0, g_pTextureModel[2]);

				// モデル(パーツ)の描画
				g_pMeshModel[g_aModel[nCntModel].nType]->DrawSubset(nCntMat);

			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//*****************************************************************************
//モデルポインタ処理
//*****************************************************************************
MODEL *GetModel(void)
{
	return &g_aModel[0];
}
//*****************************************************************************
//モデルとの当たり判定処理
//*****************************************************************************
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pModel*/)
{
	bool bLand = false;

	GAMESTATE GameState;
	GameState = GetGameState();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
#if 0
			//モデルのY幅の中
			if (pPos->y < g_aModel[nCntModel].pos.y + g_aModel[nCntModel].VtxMaxModel.y
				&& pPos->y > g_aModel[nCntModel].pos.y + g_aModel[nCntModel].VtxMinModel.y)
			{
				//モデルのZ幅の中
				if (pPos->z + VtxMin.z < g_aModel[nCntModel].pos.z + g_aModel[nCntModel].VtxMaxModel.z
					&& pPos->z + VtxMax.z > g_aModel[nCntModel].pos.z + g_aModel[nCntModel].VtxMinModel.z)
				{
					//モデルのX幅の中
					if (pPos->x + VtxMin.x < g_aModel[nCntModel].pos.x + g_aModel[nCntModel].VtxMaxModel.x
						&& pPos->x + VtxMax.x > g_aModel[nCntModel].pos.x + g_aModel[nCntModel].VtxMinModel.x)
					{
#if 1
						if (g_aModel[nCntModel].nType == 1)
						{

							//X幅の当たり判定
							if (pPosOld->x + 1 > g_aModel[nCntModel].pos.x + g_aModel[nCntModel].VtxMaxModel.x)
							{
								pPos->x = g_aModel[nCntModel].pos.x + g_aModel[nCntModel].VtxMaxModel.x + VtxMax.x;
								pPosOld->x = g_aModel[nCntModel].pos.x + g_aModel[nCntModel].VtxMaxModel.x + VtxMax.x;
							}
							else if (pPosOld->x - 1 < g_aModel[nCntModel].pos.x + g_aModel[nCntModel].VtxMinModel.x)
							{
								pPos->x = g_aModel[nCntModel].pos.x + g_aModel[nCntModel].VtxMinModel.x + VtxMin.x;
								pPosOld->x = g_aModel[nCntModel].pos.x + g_aModel[nCntModel].VtxMinModel.x + VtxMin.x;
							}

							//Z幅の当たり判定
							if (pPosOld->z + 1 > g_aModel[nCntModel].pos.z + g_aModel[nCntModel].VtxMaxModel.z)
							{
								pPos->z = g_aModel[nCntModel].pos.z + g_aModel[nCntModel].VtxMaxModel.z + VtxMax.z;
								pPosOld->z = g_aModel[nCntModel].pos.z + g_aModel[nCntModel].VtxMaxModel.z + VtxMax.z;
							}
							else if (pPosOld->z - 1 < g_aModel[nCntModel].pos.z + g_aModel[nCntModel].VtxMinModel.z)
							{
								pPos->z = g_aModel[nCntModel].pos.z + g_aModel[nCntModel].VtxMinModel.z + VtxMin.z;
								pPosOld->z = g_aModel[nCntModel].pos.z + g_aModel[nCntModel].VtxMinModel.z + VtxMin.z;
							}
							//Y幅の当たり判定
							if (pPosOld->y >= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].VtxMaxModel.y)
							{
								pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].VtxMaxModel.y;
								pPosOld->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].VtxMaxModel.y;

							}
							else if (pPosOld->y <= g_aModel[nCntModel].pos.y + g_aModel[nCntModel].VtxMinModel.y)
							{
								pPos->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].VtxMinModel.y;
								pPosOld->y = g_aModel[nCntModel].pos.y + g_aModel[nCntModel].VtxMinModel.y;
							}
#endif
						}
					}
				}
			}
#endif

#if 1
			//モデルのY幅の中
			if (pPos->y < g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.y
				&& pPos->y > g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.y)
			{
				//モデルのZ幅の中
				if (pPos->z + VtxMin.z < g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.z
					&& pPos->z + VtxMax.z > g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.z)
				{
					//モデルのX幅の中
					if (pPos->x + VtxMin.x < g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.x
						&& pPos->x + VtxMax.x > g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.x)
					{
#if 1
							//X幅の当たり判定
							if (pPosOld->x + 1 > g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.x)
							{
								pPos->x = g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.x + VtxMax.x;
								pPosOld->x = g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.x + VtxMax.x;
							}
							else if (pPosOld->x - 1 < g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.x)
							{
								pPos->x = g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.x + VtxMin.x;
								pPosOld->x = g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.x + VtxMin.x;
							}

							//Z幅の当たり判定
							else if (pPosOld->z + 1 > g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.z)
							{
								pPos->z = g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.z + VtxMax.z;
								pPosOld->z = g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.z + VtxMax.z;
							}
							else if (pPosOld->z - 1 < g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.z)
							{
								pPos->z = g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.z + VtxMin.z;
								pPosOld->z = g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.z + VtxMin.z;
							}
							//Y幅の当たり判定
							else if (pPosOld->y >= g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.y)
							{
								pPos->y = g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.y;
								pPosOld->y = g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.y;

							}
							else if (pPosOld->y <= g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.y)
							{
								pPos->y = g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.y;
								pPosOld->y = g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.y;
							}
#endif
					}
				}
			}
#endif


		}
	}
	return bLand;
}


//=============================================================================
// 設定処理
//=============================================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	int nNumVtex;	//頂点数
	DWORD sizeFvF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_MODEL];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_MODEL];

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)
		{
#if 1

			//for (int nCntTypeModel = 0; nCntTypeModel < MAX_MODEL_TYPE; nCntTypeModel++)
			//{
				//頂点数を取得
				nNumVtex = g_pMeshModel[nType]->GetNumVertices();
				//頂点フォーマットのサイズを取得
				sizeFvF = D3DXGetFVFVertexSize(g_pMeshModel[nType]->GetFVF());
			//	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
				//{
					//頂点　ロック
					g_pMeshModel[nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nType]);
					for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
					{	//頂点座標の代入
						D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff[nType];
						if (g_aModel[nCntModel].VtxMinModel.x > vtx.x)
						{
							g_aModel[nCntModel].VtxMinModel.x = vtx.x;
						}
						if (g_aModel[nCntModel].VtxMinModel.y > vtx.y)
						{
							g_aModel[nCntModel].VtxMinModel.y = vtx.y;
						}
						if (g_aModel[nCntModel].VtxMinModel.z > vtx.z)
						{
							g_aModel[nCntModel].VtxMinModel.z = vtx.z;
						}

						if (g_aModel[nCntModel].VtxMaxModel.x < vtx.x)
						{
							g_aModel[nCntModel].VtxMaxModel.x = vtx.x;
						}
						if (g_aModel[nCntModel].VtxMaxModel.y < vtx.y)
						{
							g_aModel[nCntModel].VtxMaxModel.y = vtx.y;
						}
						if (g_aModel[nCntModel].VtxMaxModel.z < vtx.z)
						{
							g_aModel[nCntModel].VtxMaxModel.z = vtx.z;
						}
						pVtxBuff[nCntModel] += sizeFvF;
					}
					//			//頂点バッファをアンロック
					g_pMeshModel[nType]->UnlockVertexBuffer();
				//}
			//}
			// 位置・向きの初期設定
#endif
			g_aModel[nCntModel].pos = pos;
			g_aModel[nCntModel].rot = D3DXVECTOR3(0, 0, 0);
			g_aModel[nCntModel].fShadow = 12;
			g_aModel[nCntModel].nType = nType;
			//　影のセット
			g_aModel[nCntModel].nIdxShadow = SetShadow(g_aModel[nCntModel].pos, D3DXVECTOR3(0, 0, 0), 20.0f);
			g_aModel[nCntModel].bUse = true;
			break;
		}
	}
}
