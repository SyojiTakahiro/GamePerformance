//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "enemy.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "polygon.h"
#include "player.h"
#include "game.h"
#include "explosion.h"
#include "sound.h"
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshEnemy[MAX_ENEMY_TYPE];						//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatEnemy[MAX_ENEMY_TYPE];				//マテリアル情報へのポインタ
LPDIRECT3DTEXTURE9	g_pTextureEnemy[MAX_ENEMY_TYPE];			//テクスチャへのポインタ
DWORD g_nNumMatEnemy[MAX_ENEMY_TYPE] = {};					//マテリアル情報の数
ENEMY g_aEnemy[MAX_ENEMY];
VTX g_aEnemyVtx[MAX_ENEMY_TYPE];
//=============================================================================
// 初期化処理
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// Xファイルの読み込み
	D3DXLoadMeshFromX(ENEMY_NAME000,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatEnemy[0],
		NULL,
		&g_nNumMatEnemy[0],
		&g_pMeshEnemy[0]);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(ENEMY_NAME001,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatEnemy[1],
		NULL,
		&g_nNumMatEnemy[1],
		&g_pMeshEnemy[1]);
	
	// Xファイルの読み込み
	D3DXLoadMeshFromX(ENEMY_NAME002,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatEnemy[2],
		NULL,
		&g_nNumMatEnemy[2],
		&g_pMeshEnemy[2]);

	int nNumVtex;	//頂点数
	DWORD sizeFvF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_ENEMY];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_ENEMY];
	
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{//敵の初期化
		g_aEnemy[nCntEnemy].VtxMinEnemy = D3DXVECTOR3(10000, 0, 10000);
		g_aEnemy[nCntEnemy].VtxMaxEnemy = D3DXVECTOR3(-10000, 0, -10000);
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].bUse = false;

	}	
	for (int nCntTypeEnemy = 0; nCntTypeEnemy < MAX_ENEMY_TYPE; nCntTypeEnemy++)
	{//当たり判定の初期化
		g_aEnemyVtx[nCntTypeEnemy].VtxMaxEnemy = D3DXVECTOR3(-10000, 0, -10000);
		g_aEnemyVtx[nCntTypeEnemy].VtxMinEnemy = D3DXVECTOR3(10000, 0, 10000);
	}

	for (int nCntTypeEnemy = 0; nCntTypeEnemy < MAX_ENEMY_TYPE; nCntTypeEnemy++)
	{
		//頂点数を取得
		nNumVtex = g_pMeshEnemy[nCntTypeEnemy]->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFvF = D3DXGetFVFVertexSize(g_pMeshEnemy[nCntTypeEnemy]->GetFVF());
		//頂点　ロック
		g_pMeshEnemy[nCntTypeEnemy]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nCntTypeEnemy]);
		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
		{	//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff[nCntTypeEnemy];
			if (g_aEnemyVtx[nCntTypeEnemy].VtxMinEnemy.x > vtx.x)
			{
				g_aEnemyVtx[nCntTypeEnemy].VtxMinEnemy.x = vtx.x;
			}
			if (g_aEnemyVtx[nCntTypeEnemy].VtxMinEnemy.y > vtx.y)
			{
				g_aEnemyVtx[nCntTypeEnemy].VtxMinEnemy.y = vtx.y;
			}
			if (g_aEnemyVtx[nCntTypeEnemy].VtxMinEnemy.z > vtx.z)
			{
				g_aEnemyVtx[nCntTypeEnemy].VtxMinEnemy.z = vtx.z;
			}

			if (g_aEnemyVtx[nCntTypeEnemy].VtxMaxEnemy.x < vtx.x)
			{
				g_aEnemyVtx[nCntTypeEnemy].VtxMaxEnemy.x = vtx.x;
			}
			if (g_aEnemyVtx[nCntTypeEnemy].VtxMaxEnemy.y < vtx.y)
			{
				g_aEnemyVtx[nCntTypeEnemy].VtxMaxEnemy.y = vtx.y;
			}
			if (g_aEnemyVtx[nCntTypeEnemy].VtxMaxEnemy.z < vtx.z)
			{
				g_aEnemyVtx[nCntTypeEnemy].VtxMaxEnemy.z = vtx.z;
			}
			pVtxBuff[nCntTypeEnemy] += sizeFvF;
		}
		//			//頂点バッファをアンロック
		g_pMeshEnemy[nCntTypeEnemy]->UnlockVertexBuffer();
	}


	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		ENEMY_NAME000,
		&g_pTextureEnemy[0]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		ENEMY_NAME000,
		&g_pTextureEnemy[1]);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		ENEMY_NAME000,
		&g_pTextureEnemy[2]);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY_TYPE; nCntEnemy++)
	{
		// メッシュの開放
		if (g_pMeshEnemy[nCntEnemy] != NULL)
		{
			g_pMeshEnemy[nCntEnemy]->Release();
			g_pMeshEnemy[nCntEnemy] = NULL;
		}
		// マテリアルの開放
		if (g_pBuffMatEnemy[nCntEnemy] != NULL)
		{
			g_pBuffMatEnemy[nCntEnemy]->Release();
			g_pBuffMatEnemy[nCntEnemy] = NULL;
		}
		//テクスチャの破棄
		if (g_pTextureEnemy[nCntEnemy] != NULL)
		{
			g_pTextureEnemy[nCntEnemy]->Release();
			g_pTextureEnemy[nCntEnemy] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	for (int nCnt = 0; nCnt < MAX_ENEMY; nCnt++)
	{
		if (g_aEnemy[nCnt].bUse == true)
		{
			if (g_aEnemy[nCnt].nType == 1)
			{
				//モデルの移動	モデルの移動する角度(敵の向き + 角度) * 移動量
				g_aEnemy[nCnt].move.x -= sinf(g_aEnemy[nCnt].rot.y) * MOVE_ENEMY / 2;
				g_aEnemy[nCnt].move.z -= cosf(g_aEnemy[nCnt].rot.y) * MOVE_ENEMY / 2;
				g_aEnemy[nCnt].fDestAngle = (g_aEnemy[nCnt].rot.y + D3DX_PI * 0.0f);

				//回転の大きさ(小さいほど大きく)
				g_aEnemy[nCnt].rot.y += 0.015f;
			}
			else if (g_aEnemy[nCnt].nType == 0)
			{
				//g_aEnemy[nCnt].fDestAngle = (g_aEnemy[nCnt].rot.y + D3DX_PI * 0.0f);
				//g_aEnemy[nCnt].rot.y += 0.2f;

			}
			else if (g_aEnemy[nCnt].nType == 2)
			{
				//g_aEnemy[nCnt].fDestAngle = (g_aEnemy[nCnt].rot.y + D3DX_PI * 0.0f);
				//g_aEnemy[nCnt].rot.y += 0.02f;
			}

			//向きの慣性
			g_aEnemy[nCnt].fDiffAngle = g_aEnemy[nCnt].fDestAngle - g_aEnemy[nCnt].rot.y;
			//角度の設定
			if (g_aEnemy[nCnt].fDiffAngle > D3DX_PI)
			{
				g_aEnemy[nCnt].fDiffAngle -= D3DX_PI* 2.0f;
			}
			if (g_aEnemy[nCnt].fDiffAngle < -D3DX_PI)
			{
				g_aEnemy[nCnt].fDiffAngle += D3DX_PI* 2.0f;
			}
			g_aEnemy[nCnt].rot.y += g_aEnemy[nCnt].fDiffAngle * 0.1f;

			if (g_aEnemy[nCnt].rot.y > D3DX_PI)
			{
				g_aEnemy[nCnt].rot.y -= D3DX_PI* 2.0f;
			}
			if (g_aEnemy[nCnt].rot.y < -D3DX_PI)
			{
				g_aEnemy[nCnt].rot.y += D3DX_PI* 2.0f;
			}
			if (g_aEnemy[nCnt].rot.x > D3DX_PI)
			{
				g_aEnemy[nCnt].rot.x -= D3DX_PI* 2.0f;
			}
			if (g_aEnemy[nCnt].rot.x < -D3DX_PI)
			{
				g_aEnemy[nCnt].rot.x += D3DX_PI* 2.0f;
			}

			g_aEnemy[nCnt].pos += g_aEnemy[nCnt].move;
			//減速
			g_aEnemy[nCnt].move.x += (0.0f - g_aEnemy[nCnt].move.x) * 0.05f;
			g_aEnemy[nCnt].move.z += (0.0f - g_aEnemy[nCnt].move.z) * 0.05f;
			g_aEnemy[nCnt].move.y += (0.0f - g_aEnemy[nCnt].move.y) * 0.05f;
			//モデルとの当たり判定
			if (CollisionModel(&g_aEnemy[nCnt].pos, &g_aEnemy[nCnt].posold, &g_aEnemy[nCnt].move,
				g_aEnemyVtx[g_aEnemy[nCnt].nType].VtxMaxEnemy, g_aEnemyVtx[g_aEnemy[nCnt].nType].VtxMinEnemy) == true)
			{

			}
			SetPositionShadow(g_aEnemy[nCnt].nIdxShadow, D3DXVECTOR3(g_aEnemy[nCnt].pos.x, 0.1f, g_aEnemy[nCnt].pos.z), 15);
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)	//敵が使用されている
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);
			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatEnemy[g_aEnemy[nCntEnemy].nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatEnemy[g_aEnemy[nCntEnemy].nType]; nCntMat++)
			{	// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				// テクスチャ
				pDevice->SetTexture(0, g_pTextureEnemy[0]);
				// 敵(パーツ)の描画
				g_pMeshEnemy[g_aEnemy[nCntEnemy].nType]->DrawSubset(nCntMat);
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}
//*****************************************************************************
//敵ポインタ処理
//*****************************************************************************
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}
//*****************************************************************************
//敵との当たり判定処理
//*****************************************************************************
bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin)
{
	bool bLand = false;

	GAMESTATE GameState;
	GameState = GetGameState();
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	D3DXVECTOR3 VecA(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 VecB(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 VecC(0.0f, 0.0f, 0.0f);

	/*VecA = *pPosOld - *pPos;
	VecB = ;*/

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
#if 1
			//敵のY幅の中
			if (pPos->y + VtxMin.y < g_aEnemy[nCntEnemy].pos.y + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMaxEnemy.y
				&& pPos->y + VtxMax.y > g_aEnemy[nCntEnemy].pos.y + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMinEnemy.y)
			{
				//敵のZ幅の中
				if (pPos->z + VtxMin.z < g_aEnemy[nCntEnemy].pos.z + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMaxEnemy.z
					&& pPos->z + VtxMax.z > g_aEnemy[nCntEnemy].pos.z + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMinEnemy.z)
				{
					//敵のX幅の中
					if (pPos->x + VtxMin.x < g_aEnemy[nCntEnemy].pos.x + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMaxEnemy.x
						&& pPos->x + VtxMax.x > g_aEnemy[nCntEnemy].pos.x + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMinEnemy.x)
					{
#if 1
						//X幅の当たり判定
						if (pPosOld->x + 1 > g_aEnemy[nCntEnemy].pos.x + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMaxEnemy.x)
						{
							pPos->x = g_aEnemy[nCntEnemy].pos.x + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMaxEnemy.x + VtxMax.x;
							pPosOld->x = g_aEnemy[nCntEnemy].pos.x + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMaxEnemy.x + VtxMax.x;
							if (g_aEnemy[nCntEnemy].nType == 1)
							{//トゲに当たった
								pMove->x = 5.0f;
							
							}

							bLand = true;
						}
						else if (pPosOld->x - 1 < g_aEnemy[nCntEnemy].pos.x + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMinEnemy.x)
						{
							pPos->x = g_aEnemy[nCntEnemy].pos.x + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMinEnemy.x + VtxMin.x;
							pPosOld->x = g_aEnemy[nCntEnemy].pos.x + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMinEnemy.x + VtxMin.x;
							if (g_aEnemy[nCntEnemy].nType == 1)
							{//トゲに当たった
								pMove->x = -5.0f;
							
							}
							bLand = true;
						}
						//Z幅の当たり判定
						else if (pPosOld->z + 1 > g_aEnemy[nCntEnemy].pos.z + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMaxEnemy.z)
						{
							pPos->z = g_aEnemy[nCntEnemy].pos.z + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMaxEnemy.z + VtxMax.z;
							pPosOld->z = g_aEnemy[nCntEnemy].pos.z + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMaxEnemy.z + VtxMax.z;
							if (g_aEnemy[nCntEnemy].nType == 1)
							{//トゲに当たった
								pMove->z = 10.0f;
							
							}
							bLand = true;
						}
						else if (pPosOld->z - 1 < g_aEnemy[nCntEnemy].pos.z + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMinEnemy.z)
						{						
							if (g_aEnemy[nCntEnemy].nType == 1)
							{//トゲに当たった
								pMove->z = - 10.0f;
							
							}
							else
							{
								pPos->z = g_aEnemy[nCntEnemy].pos.z + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMinEnemy.z + VtxMin.z;
								pPosOld->z = g_aEnemy[nCntEnemy].pos.z + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMinEnemy.z + VtxMin.z;
							}
							bLand = true;
						}
						//Y幅の当たり判定
						else if (pPosOld->y >= g_aEnemy[nCntEnemy].pos.y + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMaxEnemy.y)
						{
							pPos->y = g_aEnemy[nCntEnemy].pos.y + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMaxEnemy.y;
							pPosOld->y = g_aEnemy[nCntEnemy].pos.y + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMaxEnemy.y;
							bLand = true;
						}
						else if (pPosOld->y <= g_aEnemy[nCntEnemy].pos.y + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMinEnemy.y)
						{
							pPos->y = g_aEnemy[nCntEnemy].pos.y + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMinEnemy.y;
							pPosOld->y = g_aEnemy[nCntEnemy].pos.y + g_aEnemyVtx[g_aEnemy[nCntEnemy].nType].VtxMinEnemy.y;
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
void SetEnemy(D3DXVECTOR3 pos, int nType)
{

	//int nNumVtex;	//頂点数
	//DWORD sizeFvF[MAX_ENEMY];	//頂点フォーマットのサイズ
	//BYTE *pVtxBuff[MAX_ENEMY];	//頂点バッファへのポインタ
	//D3DXVECTOR3 vtx[MAX_ENEMY];

	////頂点数を取得
	//nNumVtex = g_pMeshEnemy[nType]->GetNumVertices();
	////頂点フォーマットのサイズを取得
	//sizeFvF[nType] = D3DXGetFVFVertexSize(g_pMeshEnemy[nType]->GetFVF());

	//for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	//{
	//	if (g_aEnemy[nCntEnemy].bUse == false)
	//	{
	//		//頂点　ロック
	//		g_pMeshEnemy[nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nType]);
	//		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
	//		{	//頂点座標の代入
	//			vtx[nCntEnemy] = *(D3DXVECTOR3*)pVtxBuff[nType];
	//			if (g_aEnemy[nCntEnemy].VtxMinEnemy.x > vtx[nCntEnemy].x)
	//			{
	//				g_aEnemy[nCntEnemy].VtxMinEnemy.x = vtx[nCntEnemy].x;
	//			}
	//			if (g_aEnemy[nCntEnemy].VtxMinEnemy.y > vtx[nCntEnemy].y)
	//			{
	//				g_aEnemy[nCntEnemy].VtxMinEnemy.y = vtx[nCntEnemy].y;
	//			}
	//			if (g_aEnemy[nCntEnemy].VtxMinEnemy.z > vtx[nCntEnemy].z)
	//			{
	//				g_aEnemy[nCntEnemy].VtxMinEnemy.z = vtx[nCntEnemy].z;
	//			}

	//			if (g_aEnemy[nCntEnemy].VtxMaxEnemy.x < vtx[nCntEnemy].x)
	//			{
	//				g_aEnemy[nCntEnemy].VtxMaxEnemy.x = vtx[nCntEnemy].x;
	//			}
	//			if (g_aEnemy[nCntEnemy].VtxMaxEnemy.y < vtx[nCntEnemy].y)
	//			{
	//				g_aEnemy[nCntEnemy].VtxMaxEnemy.y = vtx[nCntEnemy].y;
	//			}
	//			if (g_aEnemy[nCntEnemy].VtxMaxEnemy.z < vtx[nCntEnemy].z)
	//			{
	//				g_aEnemy[nCntEnemy].VtxMaxEnemy.z = vtx[nCntEnemy].z;
	//			}
	//			pVtxBuff[nCntEnemy] += sizeFvF[nType];
	//		}
	//		//頂点バッファをアンロック
	//		g_pMeshEnemy[nType]->UnlockVertexBuffer();
	//		break;
	//	}
	//}

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			// 位置・向きの初期設定
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0, 0, 0);
			g_aEnemy[nCntEnemy].fShadow = 12;
			g_aEnemy[nCntEnemy].nType = nType;
			//　影のセット
			g_aEnemy[nCntEnemy].nIdxShadow = SetShadow(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, 0.1f, g_aEnemy[nCntEnemy].pos.z), D3DXVECTOR3(0, 0, 0), 30.0f);
			g_aEnemy[nCntEnemy].bUse = true;
			break;
		}
	}

}