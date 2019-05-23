//=============================================================================
//
// 通行人処理 [passage.cpp]
// Author : SHUN YOKOMICHI
//
//=============================================================================
#include "passage.h"
#include "shadow.h"			// 影の処理
#include "camera.h"			// カメラの処理
#include "game.h"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshPassage[MAX_PASSAGE_TYPE];						//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatPassage[MAX_PASSAGE_TYPE];				//マテリアル情報へのポインタ
LPDIRECT3DTEXTURE9	g_pTexturePassage[MAX_PASSAGE_TYPE];			//テクスチャへのポインタ
DWORD g_nNumMatPassage[MAX_PASSAGE_TYPE] = {};					//マテリアル情報の数
PASSAGE g_aPassage[MAX_PASSAGE];
//D3DXMATERIAL *g_pMat;
VTXPASSAGE g_aPassageVtx[MAX_PASSAGE_TYPE];
//=============================================================================
// 初期化処理
//=============================================================================
void InitPassage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(PASSAGE_NAME000,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatPassage[0],
		NULL,
		&g_nNumMatPassage[0],
		&g_pMeshPassage[0]);


	int nNumVtex;	//頂点数
	DWORD sizeFvF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_PASSAGE];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_PASSAGE];

	for (int nCntPassage = 0; nCntPassage < MAX_PASSAGE; nCntPassage++)
	{
		g_aPassage[nCntPassage].VtxMinEnemy = D3DXVECTOR3(10000, 0, 10000);
		g_aPassage[nCntPassage].VtxMaxEnemy = D3DXVECTOR3(-10000, 0, -10000);
		g_aPassage[nCntPassage].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPassage[nCntPassage].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPassage[nCntPassage].nPattan = 0;
		g_aPassage[nCntPassage].nActiontime = 0;
		g_aPassage[nCntPassage].nType = 0;
		g_aPassage[nCntPassage].nCntTime = 0;
		g_aPassage[nCntPassage].fSpeed = 0.0f;
		g_aPassage[nCntPassage].fDestAngle = 0.0f;
		g_aPassage[nCntPassage].bUse = false;
		g_aPassage[nCntPassage].bChange = false;
	}
	for (int nCntTypeEnemy = 0; nCntTypeEnemy < MAX_PASSAGE; nCntTypeEnemy++)
	{
		g_aPassageVtx[nCntTypeEnemy].VtxMaxEnemy = D3DXVECTOR3(-10000, 0, -10000);
		g_aPassageVtx[nCntTypeEnemy].VtxMinEnemy = D3DXVECTOR3(10000, 0, 10000);
	}


	for (int nCntTypeEnemy = 0; nCntTypeEnemy < MAX_PASSAGE_TYPE; nCntTypeEnemy++)
	{
		//頂点数を取得
		nNumVtex = g_pMeshPassage[nCntTypeEnemy]->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFvF = D3DXGetFVFVertexSize(g_pMeshPassage[nCntTypeEnemy]->GetFVF());
		//頂点　ロック
		g_pMeshPassage[nCntTypeEnemy]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nCntTypeEnemy]);
		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
		{	//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff[nCntTypeEnemy];
			if (g_aPassageVtx[nCntTypeEnemy].VtxMinEnemy.x > vtx.x)
			{
				g_aPassageVtx[nCntTypeEnemy].VtxMinEnemy.x = vtx.x;
			}
			if (g_aPassageVtx[nCntTypeEnemy].VtxMinEnemy.y > vtx.y)
			{
				g_aPassageVtx[nCntTypeEnemy].VtxMinEnemy.y = vtx.y;
			}
			if (g_aPassageVtx[nCntTypeEnemy].VtxMinEnemy.z > vtx.z)
			{
				g_aPassageVtx[nCntTypeEnemy].VtxMinEnemy.z = vtx.z;
			}

			if (g_aPassageVtx[nCntTypeEnemy].VtxMaxEnemy.x < vtx.x)
			{
				g_aPassageVtx[nCntTypeEnemy].VtxMaxEnemy.x = vtx.x;
			}
			if (g_aPassageVtx[nCntTypeEnemy].VtxMaxEnemy.y < vtx.y)
			{
				g_aPassageVtx[nCntTypeEnemy].VtxMaxEnemy.y = vtx.y;
			}
			if (g_aPassageVtx[nCntTypeEnemy].VtxMaxEnemy.z < vtx.z)
			{
				g_aPassageVtx[nCntTypeEnemy].VtxMaxEnemy.z = vtx.z;
			}
			pVtxBuff[nCntTypeEnemy] += sizeFvF;
		}
		//			//頂点バッファをアンロック
		g_pMeshPassage[nCntTypeEnemy]->UnlockVertexBuffer();
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPassage(void)
{
	for (int nCntPassage = 0; nCntPassage < MAX_PASSAGE_TYPE; nCntPassage++)
	{
		// メッシュの開放
		if (g_pMeshPassage[nCntPassage] != NULL)
		{
			g_pMeshPassage[nCntPassage]->Release();
			g_pMeshPassage[nCntPassage] = NULL;
		}
		// マテリアルの開放
		if (g_pBuffMatPassage[nCntPassage] != NULL)
		{
			g_pBuffMatPassage[nCntPassage]->Release();
			g_pBuffMatPassage[nCntPassage] = NULL;
		}
		//テクスチャの破棄
		if (g_pTexturePassage[nCntPassage] != NULL)
		{
			g_pTexturePassage[nCntPassage]->Release();
			g_pTexturePassage[nCntPassage] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePassage(void)
{
	// 変数宣言
	Camera *pCamera;
	pCamera = GetCamera();

	for (int nCntPassage = 0; nCntPassage < MAX_PASSAGE; nCntPassage++)
	{
		if (g_aPassage[nCntPassage].bUse == true)
		{
			g_aPassage[nCntPassage].nCntTime++;
		
			//カメラとの距離を求める
			double length = pow((g_aPassage[nCntPassage].pos.x - pCamera->posV.x)*(g_aPassage[nCntPassage].pos.x - pCamera->posV.x)
				+ (g_aPassage[nCntPassage].pos.z - pCamera->posV.z)*(g_aPassage[nCntPassage].pos.z - pCamera->posV.z), 0.5);

			//float型に変換
			g_aPassage[nCntPassage].fLength = (float)length;

			if (g_aPassage[nCntPassage].nCntTime <= g_aPassage[nCntPassage].nActiontime)
			{
				if (g_aPassage[nCntPassage].nPattan == 0)
				{
					if (g_aPassage[nCntPassage].bChange == true)
					{
						// 右移動
						g_aPassage[nCntPassage].move.x += sinf(D3DX_PI * 0.5f) * g_aPassage[nCntPassage].fSpeed;
						g_aPassage[nCntPassage].move.z += cosf(D3DX_PI * 0.5f) * g_aPassage[nCntPassage].fSpeed;
						g_aPassage[nCntPassage].rotDest.y = D3DX_PI * 0.5f;
					}
					else
					{
						// 左移動
						g_aPassage[nCntPassage].move.x += sinf(-D3DX_PI * 0.5f) * g_aPassage[nCntPassage].fSpeed;
						g_aPassage[nCntPassage].move.z += cosf(-D3DX_PI * 0.5f) * g_aPassage[nCntPassage].fSpeed;
					
						g_aPassage[nCntPassage].rotDest.y = -D3DX_PI * 0.5f;
					}
				}
				else
				{
					if (g_aPassage[nCntPassage].bChange == true)
					{
						// 下移動
						g_aPassage[nCntPassage].move.x += sinf(D3DX_PI) * g_aPassage[nCntPassage].fSpeed;
						g_aPassage[nCntPassage].move.z += cosf(D3DX_PI) * g_aPassage[nCntPassage].fSpeed;
						g_aPassage[nCntPassage].rotDest.y = D3DX_PI;
					}
					else
					{
						// 上移動
						g_aPassage[nCntPassage].move.x += sinf(0) * g_aPassage[nCntPassage].fSpeed;
						g_aPassage[nCntPassage].move.z += cosf(0) * g_aPassage[nCntPassage].fSpeed;
						g_aPassage[nCntPassage].rotDest.y = 0;
					}
				}
			}
			if (g_aPassage[nCntPassage].nCntTime >= g_aPassage[nCntPassage].nActiontime + 1 && g_aPassage[nCntPassage].nCntTime <= g_aPassage[nCntPassage].nActiontime + 120)
			{	// 止まる時間
				g_aPassage[nCntPassage].move.x += 0.0f;
				g_aPassage[nCntPassage].move.z += 0.0f;


				if (g_aPassage[nCntPassage].nCntTime >= g_aPassage[nCntPassage].nActiontime + 120)
				{
					g_aPassage[nCntPassage].bChange = g_aPassage[nCntPassage].bChange ? false : true;			// 行動の切替
					g_aPassage[nCntPassage].nCntTime = 0;					// カウントを初期化

				}

				g_aPassage[nCntPassage].fDestAngle = g_aPassage[nCntPassage].rotDest.y - g_aPassage[nCntPassage].rot.y;

				//角度の設定
				if (g_aPassage[nCntPassage].fDestAngle > D3DX_PI)
				{
					g_aPassage[nCntPassage].fDestAngle -= D3DX_PI* 2.0f;
				}
				if (g_aPassage[nCntPassage].fDestAngle < -D3DX_PI)
				{
					g_aPassage[nCntPassage].fDestAngle += D3DX_PI* 2.0f;
				}

				g_aPassage[nCntPassage].rot.y += g_aPassage[nCntPassage].fDestAngle * 0.05f;

				if (g_aPassage[nCntPassage].rot.y > D3DX_PI)
				{
					g_aPassage[nCntPassage].rot.y -= D3DX_PI* 2.0f;
				}
				if (g_aPassage[nCntPassage].rot.y < -D3DX_PI)
				{
					g_aPassage[nCntPassage].rot.y += D3DX_PI* 2.0f;
				}
			}

			// 減速
			g_aPassage[nCntPassage].move.x += (0.0f - g_aPassage[nCntPassage].move.x) * 0.15f;
			g_aPassage[nCntPassage].move.z += (0.0f - g_aPassage[nCntPassage].move.z) * 0.15f;

			// 位置更新
			g_aPassage[nCntPassage].pos += g_aPassage[nCntPassage].move;

			SetPositionShadow(g_aPassage[nCntPassage].nIdxShadow, D3DXVECTOR3(g_aPassage[nCntPassage].pos.x, 0.1f, g_aPassage[nCntPassage].pos.z), 20.0f);


		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPassage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntPassage = 0; nCntPassage < MAX_PASSAGE; nCntPassage++)
	{
		if (g_aPassage[nCntPassage].bUse == true)	//モデルが使用されている
		{
			if (g_aPassage[nCntPassage].fLength < 2500)
			{//モデルとプレイヤーの距離で描画するか決める
			// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aPassage[nCntPassage].mtxWorld);
				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPassage[nCntPassage].rot.y, g_aPassage[nCntPassage].rot.x, g_aPassage[nCntPassage].rot.z);
				D3DXMatrixMultiply(&g_aPassage[nCntPassage].mtxWorld, &g_aPassage[nCntPassage].mtxWorld, &mtxRot);
				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aPassage[nCntPassage].pos.x, g_aPassage[nCntPassage].pos.y, g_aPassage[nCntPassage].pos.z);
				D3DXMatrixMultiply(&g_aPassage[nCntPassage].mtxWorld, &g_aPassage[nCntPassage].mtxWorld, &mtxTrans);
				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aPassage[nCntPassage].mtxWorld);
				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);


				// マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_pBuffMatPassage[g_aPassage[nCntPassage].nType]->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)g_nNumMatPassage[g_aPassage[nCntPassage].nType]; nCntMat++)
				{	// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					// テクスチャ

					pDevice->SetTexture(0, NULL);

					// モデル(パーツ)の描画
					g_pMeshPassage[g_aPassage[nCntPassage].nType]->DrawSubset(nCntMat);

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
PASSAGE *GetPassage(void)
{
	return &g_aPassage[0];
}
//*****************************************************************************
//通行人との当たり判定処理
//*****************************************************************************
bool CollisionPassage(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	bool bLand = false;

	GAMESTATE GameState;
	GameState = GetGameState();

	for (int nCntPassage = 0; nCntPassage < MAX_PASSAGE; nCntPassage++)
	{
		if (g_aPassage[nCntPassage].bUse == true)
		{
#if 1
			//モデルのY幅の中
			if (pPos->y < g_aPassage[nCntPassage].pos.y + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMaxEnemy.y
				&& pPos->y > g_aPassage[nCntPassage].pos.y + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMinEnemy.y)
			{
				//モデルのZ幅の中
				if (pPos->z + VtxMin.z < g_aPassage[nCntPassage].pos.z + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMaxEnemy.z
					&& pPos->z + VtxMax.z > g_aPassage[nCntPassage].pos.z + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMinEnemy.z)
				{
					//モデルのX幅の中
					if (pPos->x + VtxMin.x < g_aPassage[nCntPassage].pos.x + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMaxEnemy.x
						&& pPos->x + VtxMax.x > g_aPassage[nCntPassage].pos.x + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMinEnemy.x)
					{
#if 1
							//X幅の当たり判定
							if (pPosOld->x + 1 > g_aPassage[nCntPassage].pos.x + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMaxEnemy.x)
							{
								pPos->x = g_aPassage[nCntPassage].pos.x + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMaxEnemy.x + VtxMax.x;
								pPosOld->x = g_aPassage[nCntPassage].pos.x + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMaxEnemy.x + VtxMax.x;
								bLand = true;
							}
							else if (pPosOld->x - 1 < g_aPassage[nCntPassage].pos.x + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMinEnemy.x)
							{
								pPos->x = g_aPassage[nCntPassage].pos.x + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMinEnemy.x + VtxMin.x;
								pPosOld->x = g_aPassage[nCntPassage].pos.x + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMinEnemy.x + VtxMin.x;
								bLand = true;
							}

							//Z幅の当たり判定
							else if (pPosOld->z + 1 > g_aPassage[nCntPassage].pos.z + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMaxEnemy.z)
							{
								pPos->z = g_aPassage[nCntPassage].pos.z + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMaxEnemy.z + VtxMax.z;
								pPosOld->z = g_aPassage[nCntPassage].pos.z + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMaxEnemy.z + VtxMax.z;
								bLand = true;
							}
							else if (pPosOld->z - 1 < g_aPassage[nCntPassage].pos.z + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMinEnemy.z)
							{
								pPos->z = g_aPassage[nCntPassage].pos.z + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMinEnemy.z + VtxMin.z;
								pPosOld->z = g_aPassage[nCntPassage].pos.z + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMinEnemy.z + VtxMin.z;
								bLand = true;
							}
							//Y幅の当たり判定
							else if (pPosOld->y >= g_aPassage[nCntPassage].pos.y + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMaxEnemy.y)
							{
								pPos->y = g_aPassage[nCntPassage].pos.y + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMaxEnemy.y;
								pPosOld->y = g_aPassage[nCntPassage].pos.y + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMaxEnemy.y;
								bLand = true;
							}
							else if (pPosOld->y <= g_aPassage[nCntPassage].pos.y + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMinEnemy.y)
							{
								pPos->y = g_aPassage[nCntPassage].pos.y + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMinEnemy.y;
								pPosOld->y = g_aPassage[nCntPassage].pos.y + g_aPassageVtx[g_aPassage[nCntPassage].nType].VtxMinEnemy.y;
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
void SetPassage(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPattan, int nActiontime, float fSpeed, bool bChange)
{
	int nNumVtex;	//頂点数
	DWORD sizeFvF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_PASSAGE];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_PASSAGE];

	for (int nCntPassage = 0; nCntPassage < MAX_PASSAGE; nCntPassage++)
	{
		if (g_aPassage[nCntPassage].bUse == false)
		{
#if 1

			//for (int nCntTypeEnemy = 0; nCntTypeEnemy < MAX_ENEMY_TYPE; nCntTypeEnemy++)
			//{
				//頂点数を取得
				nNumVtex = g_pMeshPassage[nType]->GetNumVertices();
				//頂点フォーマットのサイズを取得
				sizeFvF = D3DXGetFVFVertexSize(g_pMeshPassage[nType]->GetFVF());
			//	for (int nCntPassage = 0; nCntPassage < MAX_ENEMY; nCntPassage++)
				//{
					//頂点　ロック
					g_pMeshPassage[nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nType]);
					for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
					{	//頂点座標の代入
						D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff[nType];
						if (g_aPassage[nCntPassage].VtxMinEnemy.x > vtx.x)
						{
							g_aPassage[nCntPassage].VtxMinEnemy.x = vtx.x;
						}
						if (g_aPassage[nCntPassage].VtxMinEnemy.y > vtx.y)
						{
							g_aPassage[nCntPassage].VtxMinEnemy.y = vtx.y;
						}
						if (g_aPassage[nCntPassage].VtxMinEnemy.z > vtx.z)
						{
							g_aPassage[nCntPassage].VtxMinEnemy.z = vtx.z;
						}

						if (g_aPassage[nCntPassage].VtxMaxEnemy.x < vtx.x)
						{
							g_aPassage[nCntPassage].VtxMaxEnemy.x = vtx.x;
						}
						if (g_aPassage[nCntPassage].VtxMaxEnemy.y < vtx.y)
						{
							g_aPassage[nCntPassage].VtxMaxEnemy.y = vtx.y;
						}
						if (g_aPassage[nCntPassage].VtxMaxEnemy.z < vtx.z)
						{
							g_aPassage[nCntPassage].VtxMaxEnemy.z = vtx.z;
						}
						pVtxBuff[nCntPassage] += sizeFvF;
					}
					//			//頂点バッファをアンロック
					g_pMeshPassage[nType]->UnlockVertexBuffer();
				//}
			//}
			// 位置・向きの初期設定
#endif
			g_aPassage[nCntPassage].pos = pos;
			g_aPassage[nCntPassage].rot = rot;
			g_aPassage[nCntPassage].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aPassage[nCntPassage].fShadow = 6;
			g_aPassage[nCntPassage].nType = nType;
			g_aPassage[nCntPassage].nActiontime = nActiontime;		// 行動時間
			g_aPassage[nCntPassage].nCntTime = 0;
			g_aPassage[nCntPassage].nPattan = nPattan;			// 行動パターン( 0 : 横移動 / 1 :縦移動)
			g_aPassage[nCntPassage].bChange = bChange;			// 行動の変化の代入(更新で変化)
			g_aPassage[nCntPassage].fSpeed = fSpeed;
			//　影のセット
			g_aPassage[nCntPassage].nIdxShadow = SetShadow(D3DXVECTOR3(g_aPassage[nCntPassage].pos.x, g_aPassage[nCntPassage].pos.y + 0.1f, g_aPassage[nCntPassage].pos.z), D3DXVECTOR3(0, 0, 0), 20.0f);
			g_aPassage[nCntPassage].bUse = true;
			break;
		}
	}
}
