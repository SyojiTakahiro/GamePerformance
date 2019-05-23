//=============================================================================
//
// 車処理 [car.cpp]
// Author : SHUN YOKOMICHI
//
//=============================================================================
#include "car.h"
#include "shadow.h"			// 影の処理
#include "camera.h"			// カメラの処理
#include "game.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshCar[MAX_CAR_TYPE];						//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatCar[MAX_CAR_TYPE];				//マテリアル情報へのポインタ
LPDIRECT3DTEXTURE9	g_pTextureCar[MAX_CAR_TYPE];			//テクスチャへのポインタ
DWORD g_nNumMatCar[MAX_CAR_TYPE] = {};					//マテリアル情報の数
CAR g_aCar[MAX_CAR];
//D3DXMATERIAL *g_pMat;
VTXCAR g_aCarVtx[MAX_CAR_TYPE];
//=============================================================================
// 初期化処理
//=============================================================================
void InitCar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	// Xファイルの読み込み
	D3DXLoadMeshFromX(CAR_NAME000, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatCar[0], NULL, &g_nNumMatCar[0], &g_pMeshCar[0]);
	D3DXLoadMeshFromX(CAR_NAME001, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatCar[1], NULL, &g_nNumMatCar[1], &g_pMeshCar[1]);
	D3DXLoadMeshFromX(CAR_NAME002, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatCar[2], NULL, &g_nNumMatCar[2], &g_pMeshCar[2]);
	D3DXLoadMeshFromX(CAR_NAME003, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatCar[3], NULL, &g_nNumMatCar[3], &g_pMeshCar[3]);
	D3DXLoadMeshFromX(CAR_NAME004, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatCar[4], NULL, &g_nNumMatCar[4], &g_pMeshCar[4]);
	D3DXLoadMeshFromX(CAR_NAME005, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatCar[5], NULL, &g_nNumMatCar[5], &g_pMeshCar[5]);
	D3DXLoadMeshFromX(CAR_NAME006, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatCar[6], NULL, &g_nNumMatCar[6], &g_pMeshCar[6]);
	D3DXLoadMeshFromX(CAR_NAME007, D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatCar[7], NULL, &g_nNumMatCar[7], &g_pMeshCar[7]);

	int nNumVtex;	//頂点数
	DWORD sizeFvF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_CAR];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_CAR];

	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{
		g_aCar[nCntCar].VtxMinEnemy = D3DXVECTOR3(10000, 0, 10000);
		g_aCar[nCntCar].VtxMaxEnemy = D3DXVECTOR3(-10000, 0, -10000);
		g_aCar[nCntCar].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCar[nCntCar].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aCar[nCntCar].nPattan = 0;
		g_aCar[nCntCar].nActiontime = 0;
		g_aCar[nCntCar].nType = 0;
		g_aCar[nCntCar].nCntTime = 0;
		g_aCar[nCntCar].fSpeed = 0.0f;
		g_aCar[nCntCar].fDestAngle = 0.0f;
		g_aCar[nCntCar].bUse = false;
		g_aCar[nCntCar].bChange = false;
	}
	for (int nCntTypeEnemy = 0; nCntTypeEnemy < MAX_CAR; nCntTypeEnemy++)
	{
		g_aCarVtx[nCntTypeEnemy].VtxMaxEnemy = D3DXVECTOR3(-10000, 0, -10000);
		g_aCarVtx[nCntTypeEnemy].VtxMinEnemy = D3DXVECTOR3(10000, 0, 10000);
	}


	for (int nCntTypeEnemy = 0; nCntTypeEnemy < MAX_CAR_TYPE; nCntTypeEnemy++)
	{
		//頂点数を取得
		nNumVtex = g_pMeshCar[nCntTypeEnemy]->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFvF = D3DXGetFVFVertexSize(g_pMeshCar[nCntTypeEnemy]->GetFVF());
		//頂点　ロック
		g_pMeshCar[nCntTypeEnemy]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nCntTypeEnemy]);
		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
		{	//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff[nCntTypeEnemy];
			if (g_aCarVtx[nCntTypeEnemy].VtxMinEnemy.x > vtx.x)
			{
				g_aCarVtx[nCntTypeEnemy].VtxMinEnemy.x = vtx.x;
			}
			if (g_aCarVtx[nCntTypeEnemy].VtxMinEnemy.y > vtx.y)
			{
				g_aCarVtx[nCntTypeEnemy].VtxMinEnemy.y = vtx.y;
			}
			if (g_aCarVtx[nCntTypeEnemy].VtxMinEnemy.z > vtx.z)
			{
				g_aCarVtx[nCntTypeEnemy].VtxMinEnemy.z = vtx.z;
			}

			if (g_aCarVtx[nCntTypeEnemy].VtxMaxEnemy.x < vtx.x)
			{
				g_aCarVtx[nCntTypeEnemy].VtxMaxEnemy.x = vtx.x;
			}
			if (g_aCarVtx[nCntTypeEnemy].VtxMaxEnemy.y < vtx.y)
			{
				g_aCarVtx[nCntTypeEnemy].VtxMaxEnemy.y = vtx.y;
			}
			if (g_aCarVtx[nCntTypeEnemy].VtxMaxEnemy.z < vtx.z)
			{
				g_aCarVtx[nCntTypeEnemy].VtxMaxEnemy.z = vtx.z;
			}
			pVtxBuff[nCntTypeEnemy] += sizeFvF;
		}
		//			//頂点バッファをアンロック
		g_pMeshCar[nCntTypeEnemy]->UnlockVertexBuffer();
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCar(void)
{
	for (int nCntCar = 0; nCntCar < MAX_CAR_TYPE; nCntCar++)
	{
		// メッシュの開放
		if (g_pMeshCar[nCntCar] != NULL)
		{
			g_pMeshCar[nCntCar]->Release();
			g_pMeshCar[nCntCar] = NULL;
		}
		// マテリアルの開放
		if (g_pBuffMatCar[nCntCar] != NULL)
		{
			g_pBuffMatCar[nCntCar]->Release();
			g_pBuffMatCar[nCntCar] = NULL;
		}
		//テクスチャの破棄
		if (g_pTextureCar[nCntCar] != NULL)
		{
			g_pTextureCar[nCntCar]->Release();
			g_pTextureCar[nCntCar] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCar(void)
{
	// 変数宣言
	Camera *pCamera;
	pCamera = GetCamera();

	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{
		if (g_aCar[nCntCar].bUse == true)
		{
			g_aCar[nCntCar].nCntTime++;

			//カメラとの距離を求める
			double length = pow((g_aCar[nCntCar].pos.x - pCamera->posV.x)*(g_aCar[nCntCar].pos.x - pCamera->posV.x)
				+ (g_aCar[nCntCar].pos.z - pCamera->posV.z)*(g_aCar[nCntCar].pos.z - pCamera->posV.z), 0.5);

			//float型に変換
			g_aCar[nCntCar].fLength = (float)length;

			if (g_aCar[nCntCar].nCntTime <= g_aCar[nCntCar].nActiontime)
			{
				if (g_aCar[nCntCar].nPattan == 0)
				{
					if (g_aCar[nCntCar].bChange == true)
					{
						// 右移動
						g_aCar[nCntCar].move.x += sinf(D3DX_PI * 0.5f) * g_aCar[nCntCar].fSpeed;
						g_aCar[nCntCar].move.z += cosf(D3DX_PI * 0.5f) * g_aCar[nCntCar].fSpeed;
					}
					else
					{
						// 左移動
						g_aCar[nCntCar].move.x += sinf(-D3DX_PI * 0.5f) * g_aCar[nCntCar].fSpeed;
						g_aCar[nCntCar].move.z += cosf(-D3DX_PI * 0.5f) * g_aCar[nCntCar].fSpeed;
					}
				}
				else
				{
					if (g_aCar[nCntCar].bChange == true)
					{
						// 下移動
						g_aCar[nCntCar].move.x += sinf(D3DX_PI) * g_aCar[nCntCar].fSpeed;
						g_aCar[nCntCar].move.z += cosf(D3DX_PI) * g_aCar[nCntCar].fSpeed;
					}
					else
					{
						// 上移動
						g_aCar[nCntCar].move.x += sinf(0) * g_aCar[nCntCar].fSpeed;
						g_aCar[nCntCar].move.z += cosf(0) * g_aCar[nCntCar].fSpeed;
					}
				}
			}
			if (g_aCar[nCntCar].nCntTime >= g_aCar[nCntCar].nActiontime + 1 && g_aCar[nCntCar].nCntTime <= g_aCar[nCntCar].nActiontime + 120)
			{	// 止まる時間
				g_aCar[nCntCar].move.x += 0.0f;
				g_aCar[nCntCar].move.z += 0.0f;


				if (g_aCar[nCntCar].nCntTime >= g_aCar[nCntCar].nActiontime + 120)
				{
					g_aCar[nCntCar].bChange = g_aCar[nCntCar].bChange ? false : true;			// 行動の切替
					g_aCar[nCntCar].nCntTime = 0;					// カウントを初期化

				}
			}

			//フィールドの大きさの判定
			if (FIELD_MAX_SIZE * 3.1 <= g_aCar[nCntCar].pos.x)
			{
				g_aCar[nCntCar].bUse = false;
				DeleteShadow(g_aCar[nCntCar].nIdxShadow);
			}
			if (-FIELD_MAX_SIZE * 1.1 >= g_aCar[nCntCar].pos.x)
			{
				g_aCar[nCntCar].bUse = false;
				DeleteShadow(g_aCar[nCntCar].nIdxShadow);
			}
			if (FIELD_MAX_SIZE * 3.1 <= g_aCar[nCntCar].pos.z)
			{
				g_aCar[nCntCar].bUse = false;
				DeleteShadow(g_aCar[nCntCar].nIdxShadow);
			}
			if (-FIELD_MAX_SIZE * 1.1 >= g_aCar[nCntCar].pos.z)
			{
				g_aCar[nCntCar].bUse = false;
				DeleteShadow(g_aCar[nCntCar].nIdxShadow);
			}

			// 減速
			g_aCar[nCntCar].move.x += (0.0f - g_aCar[nCntCar].move.x) * 0.15f;
			g_aCar[nCntCar].move.z += (0.0f - g_aCar[nCntCar].move.z) * 0.15f;

			// 位置更新
			g_aCar[nCntCar].pos += g_aCar[nCntCar].move;

			SetPositionShadow(g_aCar[nCntCar].nIdxShadow, D3DXVECTOR3(g_aCar[nCntCar].pos.x, 0.1f, g_aCar[nCntCar].pos.z), 60.0f);


		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{
		if (g_aCar[nCntCar].bUse == true)	//モデルが使用されている
		{
			if (g_aCar[nCntCar].fLength < 2500)
			{//モデルとプレイヤーの距離で描画するか決める

			// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aCar[nCntCar].mtxWorld);
				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCar[nCntCar].rot.y, g_aCar[nCntCar].rot.x, g_aCar[nCntCar].rot.z);
				D3DXMatrixMultiply(&g_aCar[nCntCar].mtxWorld, &g_aCar[nCntCar].mtxWorld, &mtxRot);
				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aCar[nCntCar].pos.x, g_aCar[nCntCar].pos.y, g_aCar[nCntCar].pos.z);
				D3DXMatrixMultiply(&g_aCar[nCntCar].mtxWorld, &g_aCar[nCntCar].mtxWorld, &mtxTrans);
				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aCar[nCntCar].mtxWorld);
				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);


				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_pBuffMatCar[g_aCar[nCntCar].nType]->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_nNumMatCar[g_aCar[nCntCar].nType]; nCntMat++)
				{	// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					// テクスチャ

					pDevice->SetTexture(0, NULL);

					// モデル(パーツ)の描画
					g_pMeshCar[g_aCar[nCntCar].nType]->DrawSubset(nCntMat);

				}
				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
				pDevice->SetTexture(0, NULL);
			}
		}
	}
}

//*****************************************************************************
//モデルポインタ処理
//*****************************************************************************
CAR *GetCar(void)
{
	return &g_aCar[0];
}
//*****************************************************************************
//通行人との当たり判定処理
//*****************************************************************************
bool CollisionCar(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	bool bLand = false;

	GAMESTATE GameState;
	GameState = GetGameState();

	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{
		if (g_aCar[nCntCar].bUse == true)
		{
#if 1
			//モデルのY幅の中
			if (pPos->y < g_aCar[nCntCar].pos.y + g_aCarVtx[g_aCar[nCntCar].nType].VtxMaxEnemy.y
				&& pPos->y > g_aCar[nCntCar].pos.y + g_aCarVtx[g_aCar[nCntCar].nType].VtxMinEnemy.y)
			{
				//モデルのZ幅の中
				if (pPos->z + VtxMin.z < g_aCar[nCntCar].pos.z + g_aCarVtx[g_aCar[nCntCar].nType].VtxMaxEnemy.z
					&& pPos->z + VtxMax.z > g_aCar[nCntCar].pos.z + g_aCarVtx[g_aCar[nCntCar].nType].VtxMinEnemy.z)
				{
					//モデルのX幅の中
					if (pPos->x + VtxMin.x < g_aCar[nCntCar].pos.x + g_aCarVtx[g_aCar[nCntCar].nType].VtxMaxEnemy.x
						&& pPos->x + VtxMax.x > g_aCar[nCntCar].pos.x + g_aCarVtx[g_aCar[nCntCar].nType].VtxMinEnemy.x)
					{
#if 1
							//X幅の当たり判定
							if (pPosOld->x + 1 > g_aCar[nCntCar].pos.x + g_aCarVtx[g_aCar[nCntCar].nType].VtxMaxEnemy.x)
							{
								pPos->x = g_aCar[nCntCar].pos.x + g_aCarVtx[g_aCar[nCntCar].nType].VtxMaxEnemy.x + VtxMax.x;
								pPosOld->x = g_aCar[nCntCar].pos.x + g_aCarVtx[g_aCar[nCntCar].nType].VtxMaxEnemy.x + VtxMax.x;
								bLand = true;
							}
							else if (pPosOld->x - 1 < g_aCar[nCntCar].pos.x + g_aCarVtx[g_aCar[nCntCar].nType].VtxMinEnemy.x)
							{
								pPos->x = g_aCar[nCntCar].pos.x + g_aCarVtx[g_aCar[nCntCar].nType].VtxMinEnemy.x + VtxMin.x;
								pPosOld->x = g_aCar[nCntCar].pos.x + g_aCarVtx[g_aCar[nCntCar].nType].VtxMinEnemy.x + VtxMin.x;
								bLand = true;
							}

							//Z幅の当たり判定
							else if (pPosOld->z + 1 > g_aCar[nCntCar].pos.z + g_aCarVtx[g_aCar[nCntCar].nType].VtxMaxEnemy.z)
							{
								pPos->z = g_aCar[nCntCar].pos.z + g_aCarVtx[g_aCar[nCntCar].nType].VtxMaxEnemy.z + VtxMax.z;
								pPosOld->z = g_aCar[nCntCar].pos.z + g_aCarVtx[g_aCar[nCntCar].nType].VtxMaxEnemy.z + VtxMax.z;
								bLand = true;
							}
							else if (pPosOld->z - 1 < g_aCar[nCntCar].pos.z + g_aCarVtx[g_aCar[nCntCar].nType].VtxMinEnemy.z)
							{
								pPos->z = g_aCar[nCntCar].pos.z + g_aCarVtx[g_aCar[nCntCar].nType].VtxMinEnemy.z + VtxMin.z;
								pPosOld->z = g_aCar[nCntCar].pos.z + g_aCarVtx[g_aCar[nCntCar].nType].VtxMinEnemy.z + VtxMin.z;
								bLand = true;
							}
							//Y幅の当たり判定
							else if (pPosOld->y >= g_aCar[nCntCar].pos.y + g_aCarVtx[g_aCar[nCntCar].nType].VtxMaxEnemy.y)
							{
								pPos->y = g_aCar[nCntCar].pos.y + g_aCarVtx[g_aCar[nCntCar].nType].VtxMaxEnemy.y;
								pPosOld->y = g_aCar[nCntCar].pos.y + g_aCarVtx[g_aCar[nCntCar].nType].VtxMaxEnemy.y;
								bLand = true;
							}
							else if (pPosOld->y <= g_aCar[nCntCar].pos.y + g_aCarVtx[g_aCar[nCntCar].nType].VtxMinEnemy.y)
							{
								pPos->y = g_aCar[nCntCar].pos.y + g_aCarVtx[g_aCar[nCntCar].nType].VtxMinEnemy.y;
								pPosOld->y = g_aCar[nCntCar].pos.y + g_aCarVtx[g_aCar[nCntCar].nType].VtxMinEnemy.y;
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
// 設定処理
//=============================================================================
void SetCar(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPattan, int nActiontime, float fSpeed, bool bChange)
{
	int nNumVtex;	//頂点数
	DWORD sizeFvF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_CAR];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_CAR];

	for (int nCntCar = 0; nCntCar < MAX_CAR; nCntCar++)
	{
		if (g_aCar[nCntCar].bUse == false)
		{
#if 1

			//for (int nCntTypeEnemy = 0; nCntTypeEnemy < MAX_ENEMY_TYPE; nCntTypeEnemy++)
			//{
				//頂点数を取得
				nNumVtex = g_pMeshCar[nType]->GetNumVertices();
				//頂点フォーマットのサイズを取得
				sizeFvF = D3DXGetFVFVertexSize(g_pMeshCar[nType]->GetFVF());
			//	for (int nCntCar = 0; nCntCar < MAX_ENEMY; nCntCar++)
				//{
					//頂点　ロック
					g_pMeshCar[nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nType]);
					for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
					{	//頂点座標の代入
						D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff[nType];
						if (g_aCar[nCntCar].VtxMinEnemy.x > vtx.x)
						{
							g_aCar[nCntCar].VtxMinEnemy.x = vtx.x;
						}
						if (g_aCar[nCntCar].VtxMinEnemy.y > vtx.y)
						{
							g_aCar[nCntCar].VtxMinEnemy.y = vtx.y;
						}
						if (g_aCar[nCntCar].VtxMinEnemy.z > vtx.z)
						{
							g_aCar[nCntCar].VtxMinEnemy.z = vtx.z;
						}

						if (g_aCar[nCntCar].VtxMaxEnemy.x < vtx.x)
						{
							g_aCar[nCntCar].VtxMaxEnemy.x = vtx.x;
						}
						if (g_aCar[nCntCar].VtxMaxEnemy.y < vtx.y)
						{
							g_aCar[nCntCar].VtxMaxEnemy.y = vtx.y;
						}
						if (g_aCar[nCntCar].VtxMaxEnemy.z < vtx.z)
						{
							g_aCar[nCntCar].VtxMaxEnemy.z = vtx.z;
						}
						pVtxBuff[nCntCar] += sizeFvF;
					}
					//			//頂点バッファをアンロック
					g_pMeshCar[nType]->UnlockVertexBuffer();
				//}
			//}
			// 位置・向きの初期設定
#endif
			g_aCar[nCntCar].pos = pos;
			g_aCar[nCntCar].rot = rot;
			g_aCar[nCntCar].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aCar[nCntCar].fShadow = 6;
			g_aCar[nCntCar].nType = nType;
			g_aCar[nCntCar].nActiontime = nActiontime;		// 行動時間
			g_aCar[nCntCar].nCntTime = 0;
			g_aCar[nCntCar].nPattan = nPattan;			// 行動パターン( 0 : 横移動 / 1 :縦移動)
			g_aCar[nCntCar].bChange = bChange;			// 行動の変化の代入(更新で変化)
			g_aCar[nCntCar].fSpeed = fSpeed;
			//　影のセット
			g_aCar[nCntCar].nIdxShadow = SetShadow(D3DXVECTOR3(g_aCar[nCntCar].pos.x, g_aCar[nCntCar].pos.y + 0.1f, g_aCar[nCntCar].pos.z), D3DXVECTOR3(0, 0, 0), 20.0f);
			g_aCar[nCntCar].bUse = true;
			break;
		}
	}
}
