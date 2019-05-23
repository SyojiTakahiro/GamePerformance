//=============================================================================
//
// モデル処理 [model.cpp]
// Author :
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
#include "score.h"
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshModel[MAX_MODEL_TYPE1];						//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatModel[MAX_MODEL_TYPE1];				//マテリアル情報へのポインタ
LPDIRECT3DTEXTURE9	g_pTextureModel[MAX_MODEL_TYPE1];			//テクスチャへのポインタ
DWORD g_nNumMatModel[MAX_MODEL_TYPE1] = {};					//マテリアル情報の数
MODEL1 g_aModel[MAX_MODEL1];
D3DXMATERIAL *g_pMat;
VTX1 g_aModelVtx[MAX_MODEL_TYPE1];
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



	int nNumVtex;	//頂点数
	DWORD sizeFvF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_MODEL1];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_MODEL1];
	
	for (int nCntModel = 0; nCntModel < MAX_MODEL1; nCntModel++)
	{//モデルの初期化
		g_aModel[nCntModel].VtxMinModel = D3DXVECTOR3(10000, 0, 10000);
		g_aModel[nCntModel].VtxMaxModel = D3DXVECTOR3(-10000, 0, -10000);
		g_aModel[nCntModel].nType = 0;
		g_aModel[nCntModel].bUse = false;

	}	
	for (int nCntTypeModel = 0; nCntTypeModel < MAX_MODEL_TYPE1; nCntTypeModel++)
	{//当たり判定の初期化
		g_aModelVtx[nCntTypeModel].VtxMaxModel = D3DXVECTOR3(-10000, 0, -10000);
		g_aModelVtx[nCntTypeModel].VtxMinModel = D3DXVECTOR3(10000, 0, 10000);
	}

#if 0
	//頂点数を取得
	nNumVtex = g_pMeshModel[0]->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFvF = D3DXGetFVFVertexSize(g_pMeshModel[0]->GetFVF());

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		//頂点　ロック
		g_pMeshModel[0]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nCntModel]);
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
			//初期処理
#if 0
//			//頂点バッファをロック
//			g_pMeshModel[0]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
//			for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
//			{	//頂点座標の代入
//
//			vtx[nCntModel] = *(D3DXVECTOR3*)pVtxBuff;
//
//			if (g_aModel[nCntModel].VtxMinModel.x > vtx[nCntModel].x)
//			{
//				g_aModel[nCntModel].VtxMinModel.x = vtx[nCntModel].x;
//			}
//			if (g_aModel[nCntModel].VtxMinModel.y > vtx[nCntModel].y)
//			{
//				g_aModel[nCntModel].VtxMinModel.y = vtx[nCntModel].y;
//			}
//			if (g_aModel[nCntModel].VtxMinModel.z > vtx[nCntModel].z)
//			{
//				g_aModel[nCntModel].VtxMinModel.z = vtx[nCntModel].z;
//			}
//
//			if (g_aModel[nCntModel].VtxMaxModel.x < vtx[nCntModel].x)
//			{
//				g_aModel[nCntModel].VtxMaxModel.x = vtx[nCntModel].x;
//			}
//			if (g_aModel[nCntModel].VtxMaxModel.y < vtx[nCntModel].y)
//			{
//				g_aModel[nCntModel].VtxMaxModel.y = vtx[nCntModel].y;
//			}
//			if (g_aModel[nCntModel].VtxMaxModel.z < vtx[nCntModel].z)
//			{
//				g_aModel[nCntModel].VtxMaxModel.z = vtx[nCntModel].z;
//			}
//
//			//サイズ分ポインタを進める
//			pVtxBuff += sizeFvF;
#endif

			pVtxBuff[nCntModel] += sizeFvF;
		}
		//			//頂点バッファをアンロック
		g_pMeshModel[0]->UnlockVertexBuffer();
	}

#endif

	for (int nCntTypeModel = 0; nCntTypeModel < MAX_MODEL_TYPE1; nCntTypeModel++)
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
		MODEL_NAME000,
		&g_pTextureModel[0]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		MODEL_NAME000,
		&g_pTextureModel[1]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		MODEL_NAME000,
		&g_pTextureModel[2]);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL_TYPE1; nCntModel++)
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
	for (int nCntModel = 0; nCntModel < MAX_MODEL1; nCntModel++)
	{
		g_aModel[nCntModel].rot.y++;

	}
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

	for (int nCntModel = 0; nCntModel < MAX_MODEL1; nCntModel++)
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
				pDevice->SetTexture(0, g_pTextureModel[0]);
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
MODEL1 *GetModel(void)
{
	return &g_aModel[0];
}
//*****************************************************************************
//モデルとの当たり判定処理
//*****************************************************************************
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin, COLLISIONMODELTYPE Type)
{
	bool bLand = false;

	GAMESTATE GameState;
	GameState = GetGameState();
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	D3DXVECTOR3 VecA(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 VecB(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 VecC(0.0f, 0.0f, 0.0f);

	for (int nCntModel = 0; nCntModel < MAX_MODEL1; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{
#if 1
			//モデルのY幅の中
			if (pPos->y + VtxMin.y< g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.y
				&& pPos->y + VtxMax.y> g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.y)
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
						if (pPosOld->x - 20 > g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.x)
						{
							pPos->x = g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.x + VtxMax.x;
							pPosOld->x = g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.x + VtxMax.x;

							//bLand = true;
						}
						else if (pPosOld->x + 20 < g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.x)
						{
							pPos->x = g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.x + VtxMin.x;
							pPosOld->x = g_aModel[nCntModel].pos.x + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.x + VtxMin.x;

							//bLand = true;
						}
						//Z幅の当たり判定
						else if (pPosOld->z > g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.z)
						{
							pPos->z = g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.z + VtxMax.z;
							pPosOld->z = g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.z + VtxMax.z;

							//bLand = true;
						}
						else if (pPosOld->z < g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.z)
						{
							pPos->z = g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.z + VtxMin.z;
							pPosOld->z = g_aModel[nCntModel].pos.z + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.z + VtxMin.z;

							//bLand = true;
						}
						//Y幅の当たり判定
						else if (pPosOld->y + 20 >= g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.y)
						{
							pPos->y = g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.y;
							pPosOld->y = g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMaxModel.y;
							bLand = true;
						}
						else if (pPosOld->y <= g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.y)
						{
							pPos->y = g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.y;
							pPosOld->y = g_aModel[nCntModel].pos.y + g_aModelVtx[g_aModel[nCntModel].nType].VtxMinModel.y;
							bLand = true;
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
// 弾の設定処理
//=============================================================================
void SetModel(D3DXVECTOR3 pos, int nType)
{

	int nNumVtex;	//頂点数
	DWORD sizeFvF[MAX_MODEL1];	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_MODEL1];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_MODEL1];

	//頂点数を取得
	nNumVtex = g_pMeshModel[nType]->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFvF[nType] = D3DXGetFVFVertexSize(g_pMeshModel[nType]->GetFVF());

	for (int nCntModel = 0; nCntModel < MAX_MODEL1; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)
		{
			//頂点　ロック
			g_pMeshModel[nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nType]);
			for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
			{	//頂点座標の代入
				vtx[nCntModel] = *(D3DXVECTOR3*)pVtxBuff[nType];
				if (g_aModel[nCntModel].VtxMinModel.x > vtx[nCntModel].x)
				{
					g_aModel[nCntModel].VtxMinModel.x = vtx[nCntModel].x;
				}
				if (g_aModel[nCntModel].VtxMinModel.y > vtx[nCntModel].y)
				{
					g_aModel[nCntModel].VtxMinModel.y = vtx[nCntModel].y;
				}
				if (g_aModel[nCntModel].VtxMinModel.z > vtx[nCntModel].z)
				{
					g_aModel[nCntModel].VtxMinModel.z = vtx[nCntModel].z;
				}

				if (g_aModel[nCntModel].VtxMaxModel.x < vtx[nCntModel].x)
				{
					g_aModel[nCntModel].VtxMaxModel.x = vtx[nCntModel].x;
				}
				if (g_aModel[nCntModel].VtxMaxModel.y < vtx[nCntModel].y)
				{
					g_aModel[nCntModel].VtxMaxModel.y = vtx[nCntModel].y;
				}
				if (g_aModel[nCntModel].VtxMaxModel.z < vtx[nCntModel].z)
				{
					g_aModel[nCntModel].VtxMaxModel.z = vtx[nCntModel].z;
				}
				pVtxBuff[nCntModel] += sizeFvF[nType];
			}
			//頂点バッファをアンロック
			g_pMeshModel[nType]->UnlockVertexBuffer();
			break;
		}
	}

	for (int nCntModel = 0; nCntModel < MAX_MODEL1; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)
		{
			// 位置・向きの初期設定
			g_aModel[nCntModel].pos = pos;
			g_aModel[nCntModel].rot = D3DXVECTOR3(0, 0, 0);
			g_aModel[nCntModel].fShadow = 12;
			g_aModel[nCntModel].nType = nType;
			//　影のセット					D3DXVECTOR3(g_aPlayer.pos.x, 0.1f, g_aPlayer.pos.z)
			//g_aModel[nCntModel].nIdxShadow = SetShadow(D3DXVECTOR3(g_aModel[nCntModel].pos.x, 0.1f, g_aModel[nCntModel].pos.z), D3DXVECTOR3(0, 0, 0), 20.0f);
			g_aModel[nCntModel].bUse = true;
			break;
		}
	}

}