//=============================================================================
//
// モデル処理 [model.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "model.h"
#include "input.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define  MAX_TYPE           (30)                // モデルの種類の配列領域の最大数
#define  MAX_MODEL          (300)               // モデルの配列領域の最大数
#define  NUM_MODEL          "NUM_MODEL"
#define  MODEL_FILENAME     "MODEL_FILENAME"
#define  MODELSET           "MODELSET"
#define  END_MODELSET       "END_MODELSET"

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH   g_pMeshModel[MAX_TYPE] = {};          // メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatModel[MAX_TYPE] = {};       // マテリアル情報へのポインタ
DWORD        g_nNumMatModel[MAX_TYPE] = {};        // マテリアル情報の数
LPDIRECT3DTEXTURE9* g_pTexturModel[MAX_TYPE] = {}; // テクスチャのポインタ
Model  g_model[MAX_TYPE][MAX_MODEL];               // モデル情報
int   g_NumModelType = 0;                          // モデルの種類の総数
int   g_NumModel[MAX_TYPE] = {};                   // モデルの配置数

//=============================================================================
// 初期化処理
//=============================================================================
void InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	FILE *pFile;         // テキストファイルへのポインタ
	char str0[128];      // いらない文字
	char xFileName[64];  // xファイルの名前
	D3DXVECTOR3 pos;     // モデルの座標
	D3DXVECTOR3 rot;     // モデルの向き
	int nCntType = 0;    // モデルの種類を数える
	for (nCntType = 0; nCntType < MAX_TYPE; nCntType++)
	{
		g_NumModel[nCntType] = 0;
	}
	nCntType = 0;
	g_NumModelType = 0;

	// テキストファイルを開く
	pFile = fopen("data/TEXT/model.txt", "r");

	if (pFile != NULL)
	{// テキストファイルがある
		while (fscanf(pFile, "%s", &str0[0]) != EOF)
		{// テキストファイルの末端まで文字列を改行か空白があるまで読み取る
			if (strcmp(&str0[0], NUM_MODEL) == 0)
			{// モデルの総数を読み取る場所だったら
				// モデルの総数を読み取る
				fscanf(pFile, "%s %d", &str0[0], &g_NumModelType);
			}
			if (strcmp(&str0[0], MODEL_FILENAME) == 0)
			{// xファイルの拡張子が書かれていたら
				if (fscanf(pFile, "%s %s", &str0[0], &xFileName[0]) == 2)
				{// モデルのファイル名を読み取ったら
					// xファイルの読み込み
					D3DXLoadMeshFromX(&xFileName[0],
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&g_pBuffMatModel[nCntType],
						NULL,
						&g_nNumMatModel[nCntType],
						&g_pMeshModel[nCntType]);

					// マテリアル情報からテクスチャを引き出す
					D3DXMATERIAL* pMat = (D3DXMATERIAL*)g_pBuffMatModel[nCntType]->GetBufferPointer();
					D3DMATERIAL9* pMatBuff = new D3DMATERIAL9[g_nNumMatModel[nCntType]];
					g_pTexturModel[nCntType] = new LPDIRECT3DTEXTURE9[g_nNumMatModel[nCntType]];

					for (DWORD nCntMat = 0; nCntMat < g_nNumMatModel[nCntType]; nCntMat++)
					{// 頂点数の数だけ繰り返し
					 // マテリアル情報を読み込む
						pMatBuff[nCntMat] = pMat[nCntMat].MatD3D;

						// 環境光を初期化する
						pMatBuff[nCntMat].Ambient = pMatBuff[nCntMat].Diffuse;

						// テクスチャ情報を初期化
						g_pTexturModel[nCntType][nCntMat] = NULL;

						// テクスチャの情報を読み込む
						if (pMat[nCntMat].pTextureFilename != NULL &&
							lstrlen(pMat[nCntMat].pTextureFilename) > 0)
						{// テクスチャのファイル名がある
							D3DXCreateTextureFromFile(pDevice,
								pMat[nCntMat].pTextureFilename,
								&g_pTexturModel[nCntType][nCntMat]);
						}

					}
				}
				nCntType++;  // 種類が変わるので進めておく
			}
			if (strcmp(&str0[0], MODELSET) == 0)
			{// モデル情報が書かれているところがある
				int nCntSet = 0;  // 情報を設定した回数
				fscanf(pFile, "%s %s %d", &str0[0], &str0[0], &nCntType);
				while (strcmp(&str0[0], END_MODELSET) != 0)
				{// モデルの設定が終わるまで繰り返し
					if (nCntSet == MODELSET_POS)
					{// １回目だったら
						if (fscanf(pFile, "%s %s %f %f %f", &str0[0], &str0[0],&pos.x, &pos.y, &pos.z) == 5)
						{// 座標情報を読み取ったら
							g_model[nCntType][g_NumModel[nCntType]].pos = D3DXVECTOR3(pos.x, pos.y, pos.z);  // 位置を設定
						}
					}
					else if (nCntSet == MODELSET_ROT)
					{// ２回目だったら
						if (fscanf(pFile, "%s %f %f %f", &str0[0], &rot.x, &rot.y, &rot.z) == 4)
						{// 向き情報を読み取ったら
							g_model[nCntType][g_NumModel[nCntType]].rot = D3DXVECTOR3(rot.x, rot.y, rot.z);  // 向きを設定
						}
					}
					fscanf(pFile, "%s", &str0[0]);
					nCntSet++;                     // セットした回数を増やす
				}
				g_NumModel[nCntType]++;  // モデルの配置数を数える
			}
		}
	}

	// ファイルを閉じて他のプログラムからいじれるようにする
	fclose(pFile);

	for (int nCntType = 0; nCntType < g_NumModelType; nCntType++)
	{// モデルの種類の数だけ繰り返し
		for (int nCntModel = 0; nCntModel < g_NumModel[nCntType]; nCntModel++)
		{// モデルの数だけ繰り返し
			if (nCntModel == 0)
			{// 1回目だったら
				int nNumVtx;	 // 頂点数
				DWORD sizeFVF;	 // 頂点フォーマットのサイズ
				BYTE *pVtxBuff;  // 頂点バッファへのポインタ

				// 最小値の設定
				g_model[nCntType][nCntModel].vtxMinObstacle = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
				g_model[nCntType][nCntModel].vtxMaxObstacle = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

				// 頂点数を取得
				nNumVtx = g_pMeshModel[nCntType]->GetNumVertices();

				// 頂点フォーマットのサイズを取得
				sizeFVF = D3DXGetFVFVertexSize(g_pMeshModel[nCntType]->GetFVF());

				// 頂点バッファをロック
				g_pMeshModel[nCntType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

				for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
				{// モデルの頂点数だけ繰り返し
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

					if (vtx.x < g_model[nCntType][nCntModel].vtxMinObstacle.x)
					{// X座標がより小さい
						g_model[nCntType][nCntModel].vtxMinObstacle.x = vtx.x;
					}
					if (vtx.y < g_model[nCntType][nCntModel].vtxMinObstacle.y)
					{// Y座標がより小さい
						g_model[nCntType][nCntModel].vtxMinObstacle.y = vtx.y;
					}
					if (vtx.z < g_model[nCntType][nCntModel].vtxMinObstacle.z)
					{// Z座標がより小さい
						g_model[nCntType][nCntModel].vtxMinObstacle.z = vtx.z;
					}

					if (vtx.x > g_model[nCntType][nCntModel].vtxMaxObstacle.x)
					{// X座標がより大きい
						g_model[nCntType][nCntModel].vtxMaxObstacle.x = vtx.x;
					}
					if (vtx.y > g_model[nCntType][nCntModel].vtxMaxObstacle.y)
					{// Y座標がより大きい
						g_model[nCntType][nCntModel].vtxMaxObstacle.y = vtx.y;
					}
					if (vtx.z > g_model[nCntType][nCntModel].vtxMaxObstacle.z)
					{// Z座標がより大きい
						g_model[nCntType][nCntModel].vtxMaxObstacle.z = vtx.z;
					}

					pVtxBuff += sizeFVF;	// サイズ分ポインタを進める
				}

				// 頂点バッファをアンロック
				g_pMeshModel[nCntType]->UnlockVertexBuffer();
			}
			else
			{// それ以降
				g_model[nCntType][nCntModel].vtxMaxObstacle = g_model[nCntType][nCntModel - 1].vtxMaxObstacle;
				g_model[nCntType][nCntModel].vtxMinObstacle = g_model[nCntType][nCntModel - 1].vtxMinObstacle;
			}
			g_model[nCntType][nCntModel].Vertex[0].pos = D3DXVECTOR3(g_model[nCntType][nCntModel].vtxMinObstacle.x - 8.0f, 0.0f, g_model[nCntType][nCntModel].vtxMaxObstacle.z + 8.0f);
			g_model[nCntType][nCntModel].Vertex[1].pos = D3DXVECTOR3(g_model[nCntType][nCntModel].vtxMaxObstacle.x + 8.0f, 0.0f, g_model[nCntType][nCntModel].vtxMaxObstacle.z + 8.0f);
			g_model[nCntType][nCntModel].Vertex[2].pos = D3DXVECTOR3(g_model[nCntType][nCntModel].vtxMaxObstacle.x + 8.0f, 0.0f, g_model[nCntType][nCntModel].vtxMinObstacle.z - 8.0f);
			g_model[nCntType][nCntModel].Vertex[3].pos = D3DXVECTOR3(g_model[nCntType][nCntModel].vtxMinObstacle.x - 8.0f, 0.0f, g_model[nCntType][nCntModel].vtxMinObstacle.z - 8.0f);
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModel(void)
{
	for (int nCntType = 0; nCntType < MAX_TYPE; nCntType++)
	{// モデルの種類の数だけ繰り返し
			// メッシュの破棄
		if (g_pMeshModel[nCntType] != NULL)
		{
			g_pMeshModel[nCntType]->Release();
			g_pMeshModel[nCntType] = NULL;
		}

		// マテリアルの破棄
		if (g_pBuffMatModel[nCntType] != NULL)
		{
			g_pBuffMatModel[nCntType]->Release();
			g_pBuffMatModel[nCntType] = NULL;
		}

		// テクスチャの破棄
		for (DWORD nCntMat = 0; nCntMat < g_nNumMatModel[nCntType]; nCntMat++)
		{
			if (g_pTexturModel[nCntType][nCntMat] != NULL)
			{
				g_pTexturModel[nCntType][nCntMat]->Release();
				g_pTexturModel[nCntType][nCntMat] = NULL;
			}
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
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;                // 計算用マトリックス
	D3DMATERIAL9 matDef;                        // 現在のマテリアル保存用
	D3DXMATERIAL *pMat;                         // マテリアルデータへのポインタ
	for (int nCntType = 0; nCntType < g_NumModelType; nCntType++)
	{// モデルの種類の数だけ繰り返し
		for (int nCntModel = 0; nCntModel < g_NumModel[nCntType]; nCntModel++)
		{// モデルの数だけ繰り返し
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_model[nCntType][nCntModel].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_model[nCntType][nCntModel].rot.y, g_model[nCntType][nCntModel].rot.x, g_model[nCntType][nCntModel].rot.z);
			D3DXMatrixMultiply(&g_model[nCntType][nCntModel].mtxWorld, &g_model[nCntType][nCntModel].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_model[nCntType][nCntModel].pos.x, g_model[nCntType][nCntModel].pos.y, g_model[nCntType][nCntModel].pos.z);
			D3DXMatrixMultiply(&g_model[nCntType][nCntModel].mtxWorld, &g_model[nCntType][nCntModel].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_model[nCntType][nCntModel].mtxWorld);

			for (int nCntVer = 0; nCntVer < 4; nCntVer++)
			{// 頂点数の数だけ繰り返し
			 // ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_model[nCntType][nCntModel].Vertex[nCntVer].mtxWorld);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_model[nCntType][nCntModel].Vertex[nCntVer].rot.y, g_model[nCntType][nCntModel].Vertex[nCntVer].rot.x, g_model[nCntType][nCntModel].Vertex[nCntVer].rot.z);
				D3DXMatrixMultiply(&g_model[nCntType][nCntModel].Vertex[nCntVer].mtxWorld, &g_model[nCntType][nCntModel].Vertex[nCntVer].mtxWorld, &mtxRot);

				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_model[nCntType][nCntModel].Vertex[nCntVer].pos.x, g_model[nCntType][nCntModel].Vertex[nCntVer].pos.y, g_model[nCntType][nCntModel].Vertex[nCntVer].pos.z);
				D3DXMatrixMultiply(&g_model[nCntType][nCntModel].Vertex[nCntVer].mtxWorld, &g_model[nCntType][nCntModel].Vertex[nCntVer].mtxWorld, &mtxTrans);

				// 親のマトリックスを掛け合わせる
				D3DXMatrixMultiply(&g_model[nCntType][nCntModel].Vertex[nCntVer].mtxWorld, &g_model[nCntType][nCntModel].Vertex[nCntVer].mtxWorld, &g_model[nCntType][nCntModel].mtxWorld);
			}

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatModel[nCntType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatModel[nCntType]; nCntMat++)
			{// 設定されていたマテリアルの数だけ繰り返し
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_pTexturModel[nCntType][nCntMat]);

				// モデル(パーツ)の描画
				g_pMeshModel[nCntType]->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}
//=============================================================================
// モデルの当たり判定
//=============================================================================
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 radius)
{
	bool bLand = false;  // 乗ったかどうか

	// 外積の当たり判定用
	D3DXVECTOR3 vecA, vecC;
	D3DXVECTOR3 aVerPos[4];
	int nCntVec = 0;

	for (int nCntType = 0; nCntType < g_NumModelType; nCntType++)
	{// モデルの種類の数だけ繰り返し
		for (int nCntModel = 0; nCntModel < g_NumModel[nCntType]; nCntModel++)
		{// モデルの数だけ繰り返し
			nCntVec = 0;
			// そのコースの頂点座標を取得
			aVerPos[0] = D3DXVECTOR3(g_model[nCntType][nCntModel].Vertex[0].mtxWorld._41, g_model[nCntType][nCntModel].Vertex[0].mtxWorld._42, g_model[nCntType][nCntModel].Vertex[0].mtxWorld._43);
			aVerPos[1] = D3DXVECTOR3(g_model[nCntType][nCntModel].Vertex[1].mtxWorld._41, g_model[nCntType][nCntModel].Vertex[1].mtxWorld._42, g_model[nCntType][nCntModel].Vertex[1].mtxWorld._43);
			aVerPos[2] = D3DXVECTOR3(g_model[nCntType][nCntModel].Vertex[2].mtxWorld._41, g_model[nCntType][nCntModel].Vertex[2].mtxWorld._42, g_model[nCntType][nCntModel].Vertex[2].mtxWorld._43);
			aVerPos[3] = D3DXVECTOR3(g_model[nCntType][nCntModel].Vertex[3].mtxWorld._41, g_model[nCntType][nCntModel].Vertex[3].mtxWorld._42, g_model[nCntType][nCntModel].Vertex[3].mtxWorld._43);

			for (int nCntVer = 0; nCntVer < 4; nCntVer++)
			{// 頂点数の数だけ繰り返し
				vecC = *pPos - aVerPos[nCntVer];
				vecA = aVerPos[(nCntVer + 1) % 4] - aVerPos[nCntVer];

				if ((vecA.z * vecC.x) - (vecA.x * vecC.z) < 0)
				{// そのベクトルの外側にいる
					nCntVec++;
				}
			}

			if (pPos->y <= g_model[nCntType][nCntModel].pos.y + g_model[nCntType][nCntModel].vtxMaxObstacle.y - 5.0f && pPos->y + radius.y >= g_model[nCntType][nCntModel].pos.y + g_model[nCntType][nCntModel].vtxMaxObstacle.y - 5.0f
				|| pPos->y + radius.y >= g_model[nCntType][nCntModel].pos.y + g_model[nCntType][nCntModel].vtxMinObstacle.y && pPos->y <= g_model[nCntType][nCntModel].pos.y + g_model[nCntType][nCntModel].vtxMinObstacle.y
				|| pPos->y + radius.y <= g_model[nCntType][nCntModel].pos.y + g_model[nCntType][nCntModel].vtxMaxObstacle.y - 5.0f && pPos->y >= g_model[nCntType][nCntModel].pos.y + g_model[nCntType][nCntModel].vtxMinObstacle.y)
			{// 障害物のY座標の中にいる
				if (nCntVec == 0)
				{// 一度もベクトルの外側にいなかった
					pPos->x = pPosOld->x;
					pPos->z = pPosOld->z;
					pPos->x -= pMove->x * 0.001f;
					pPos->z -= pMove->z * 0.001f;
					pPos->x += g_model[nCntType][nCntModel].move.x;
					pPos->z += g_model[nCntType][nCntModel].move.z;
				}
			}
		}
	}

	return bLand;
}