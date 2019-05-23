//=============================================================================
//
// ホーム処理 [home.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "home.h"
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
LPD3DXMESH g_pMeshHome[MAX_HOME];					//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatHome[MAX_HOME];			//マテリアル情報へのポインタ
LPDIRECT3DTEXTURE9	g_pTextureHome[MAX_HOME];		//テクスチャへのポインタ
DWORD g_nNumMatHome[MAX_HOME] = {};							//マテリアル情報の数
HOME g_aHome[MAX_HOME];
D3DXMATERIAL *g_pMatHome;
//=============================================================================
// 初期化処理
//=============================================================================
void InitHome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(HOME_NAME000,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatHome[0],
		NULL,
		&g_nNumMatHome[0],
		&g_pMeshHome[0]);



	int nNumVtex;	//頂点数
	DWORD sizeFvF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_HOME];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_HOME];

	for (int nCntHome = 0; nCntHome < MAX_HOME; nCntHome++)
	{
		g_aHome[nCntHome].VtxMinHome = D3DXVECTOR3(10000, 0, 10000);
		g_aHome[nCntHome].VtxMaxHome = D3DXVECTOR3(-10000, 0, -10000);
		g_aHome[nCntHome].nType = 0;
		g_aHome[nCntHome].bUse = false;
	}

	//頂点数を取得
	nNumVtex = g_pMeshHome[0]->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFvF = D3DXGetFVFVertexSize(g_pMeshHome[0]->GetFVF());

	for (int nCntHome = 0; nCntHome < MAX_HOME; nCntHome++)
	{
		//頂点　ロック
		g_pMeshHome[0]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nCntHome]);
		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
		{	//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff[nCntHome];
			if (g_aHome[nCntHome].VtxMinHome.x > vtx.x)
			{
				g_aHome[nCntHome].VtxMinHome.x = vtx.x;
			}
			if (g_aHome[nCntHome].VtxMinHome.y > vtx.y)
			{
				g_aHome[nCntHome].VtxMinHome.y = vtx.y;
			}
			if (g_aHome[nCntHome].VtxMinHome.z > vtx.z)
			{
				g_aHome[nCntHome].VtxMinHome.z = vtx.z;
			}

			if (g_aHome[nCntHome].VtxMaxHome.x < vtx.x)
			{
				g_aHome[nCntHome].VtxMaxHome.x = vtx.x;
			}
			if (g_aHome[nCntHome].VtxMaxHome.y < vtx.y)
			{
				g_aHome[nCntHome].VtxMaxHome.y = vtx.y;
			}
			if (g_aHome[nCntHome].VtxMaxHome.z < vtx.z)
			{
				g_aHome[nCntHome].VtxMaxHome.z = vtx.z;
			}
			//初期処理

			pVtxBuff[nCntHome] += sizeFvF;
		}
		//			//頂点バッファをアンロック
		g_pMeshHome[0]->UnlockVertexBuffer();
	}
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		HOME_NAME000,
		&g_pTextureHome[0]);

	for (int nCntHome = 0; nCntHome < MAX_HOME; nCntHome++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_nNumMatHome[nCntHome]; nCntMat++)
		{
			// マテリアルデータへのポインタを取得
			g_pMatHome = (D3DXMATERIAL*)g_pBuffMatHome[g_aHome[nCntHome].nType]->GetBufferPointer();
			{
				D3DXCreateTextureFromFile(pDevice,
					g_pMatHome[nCntMat].pTextureFilename,
					&g_pTextureHome[g_aHome[nCntHome].nType]);
			}
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitHome(void)
{
	for (int nCntHome = 0; nCntHome < MAX_HOME; nCntHome++)
	{
		// メッシュの開放
		if (g_pMeshHome[nCntHome] != NULL)
		{
			g_pMeshHome[nCntHome]->Release();
			g_pMeshHome[nCntHome] = NULL;
		}
		// マテリアルの開放
		if (g_pBuffMatHome[nCntHome] != NULL)
		{
			g_pBuffMatHome[nCntHome]->Release();
			g_pBuffMatHome[nCntHome] = NULL;
		}
		//テクスチャの破棄
		if (g_pTextureHome[nCntHome] != NULL)
		{
			g_pTextureHome[nCntHome]->Release();
			g_pTextureHome[nCntHome] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateHome(void)
{
	
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawHome(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntHome = 0; nCntHome < MAX_HOME; nCntHome++)
	{		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aHome[nCntHome].mtxWorld);
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aHome[nCntHome].rot.y, g_aHome[nCntHome].rot.x, g_aHome[nCntHome].rot.z);
		D3DXMatrixMultiply(&g_aHome[nCntHome].mtxWorld, &g_aHome[nCntHome].mtxWorld, &mtxRot);
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aHome[nCntHome].pos.x, g_aHome[nCntHome].pos.y, g_aHome[nCntHome].pos.z);
		D3DXMatrixMultiply(&g_aHome[nCntHome].mtxWorld, &g_aHome[nCntHome].mtxWorld, &mtxTrans);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aHome[nCntHome].mtxWorld);
		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);
		
		
		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatHome[g_aHome[nCntHome].nType]->GetBufferPointer();
 
		for (int nCntMat = 0; nCntMat < (int)g_nNumMatHome[nCntHome]; nCntMat++)
		{	// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			// テクスチャ
			pDevice->SetTexture(0, g_pTextureHome[g_aHome[nCntHome].nType]);		  
		 	if (g_aHome[nCntHome].bUse == true)	//ホームが使用されている
			{	// ホーム(パーツ)の描画
				g_pMeshHome[g_aHome[nCntHome].nType]->DrawSubset(nCntMat);
			}
		}
		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
}

//*****************************************************************************
//ホームポインタ処理
//*****************************************************************************
HOME *GetHome(void)
{
	return &g_aHome[0];
}
//*****************************************************************************
//ホームとの当たり判定処理
//*****************************************************************************
bool CollisionHome(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, HOME **pHome*/)
{
	bool bLand = false;

	GAMESTATE GameState;
	GameState = GetGameState();

	for (int nCntHome = 0; nCntHome < MAX_HOME; nCntHome++)
	{
		if (g_aHome[nCntHome].bUse == true)
		{
#if 1
			//ホームのY幅の中
			if (pPos->y < g_aHome[nCntHome].pos.y + g_aHome[nCntHome].VtxMaxHome.y
				&& pPos->y > g_aHome[nCntHome].pos.y + g_aHome[nCntHome].VtxMinHome.y)
			{
				//ホームのZ幅の中
				if (pPos->z + VtxMin.z < g_aHome[nCntHome].pos.z + g_aHome[nCntHome].VtxMaxHome.z
					&& pPos->z + VtxMax.z > g_aHome[nCntHome].pos.z + g_aHome[nCntHome].VtxMinHome.z)
				{
					//ホームのX幅の中
					if (pPos->x + VtxMin.x < g_aHome[nCntHome].pos.x + g_aHome[nCntHome].VtxMaxHome.x
						&& pPos->x + VtxMax.x > g_aHome[nCntHome].pos.x + g_aHome[nCntHome].VtxMinHome.x)
					{

						//X幅の当たり判定
						if (pPosOld->x + 1 > g_aHome[nCntHome].pos.x + g_aHome[nCntHome].VtxMaxHome.x)
						{
							bLand = true;
						}
						else if (pPosOld->x - 1 < g_aHome[nCntHome].pos.x + g_aHome[nCntHome].VtxMinHome.x)
						{
							bLand = true;
						}
						//Z幅の当たり判定
						if (pPosOld->z + 1 > g_aHome[nCntHome].pos.z + g_aHome[nCntHome].VtxMaxHome.z)
						{
							bLand = true;
						}
						else if (pPosOld->z - 1 < g_aHome[nCntHome].pos.z + g_aHome[nCntHome].VtxMinHome.z)
						{
							bLand = true;
						}
						//Y幅の当たり判定
						if (pPosOld->y >= g_aHome[nCntHome].pos.y + g_aHome[nCntHome].VtxMaxHome.y)
						{
						}
						else if (pPosOld->y <= g_aHome[nCntHome].pos.y + g_aHome[nCntHome].VtxMinHome.y)
						{
						}
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
void SetHome(D3DXVECTOR3 pos, int nType)
{

	int nNumVtex;	//頂点数
	DWORD sizeFvF[MAX_HOME];	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_HOME];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_HOME];

	//頂点数を取得
	nNumVtex = g_pMeshHome[nType]->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFvF[nType] = D3DXGetFVFVertexSize(g_pMeshHome[nType]->GetFVF());

	for (int nCntHome = 0; nCntHome < MAX_HOME; nCntHome++)
	{
		//if (g_aHome[nCntHome].bUse == false)
		//{
			//頂点　ロック
			g_pMeshHome[nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nType]);
			for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
			{	//頂点座標の代入
				vtx[nCntHome] = *(D3DXVECTOR3*)pVtxBuff[nType];
				if (g_aHome[nCntHome].VtxMinHome.x > vtx[nCntHome].x)
				{
					g_aHome[nCntHome].VtxMinHome.x = vtx[nCntHome].x;
				}
				if (g_aHome[nCntHome].VtxMinHome.y > vtx[nCntHome].y)
				{
					g_aHome[nCntHome].VtxMinHome.y = vtx[nCntHome].y;
				}
				if (g_aHome[nCntHome].VtxMinHome.z > vtx[nCntHome].z)
				{
					g_aHome[nCntHome].VtxMinHome.z = vtx[nCntHome].z;
				}

				if (g_aHome[nCntHome].VtxMaxHome.x < vtx[nCntHome].x)
				{
					g_aHome[nCntHome].VtxMaxHome.x = vtx[nCntHome].x;
				}
				if (g_aHome[nCntHome].VtxMaxHome.y < vtx[nCntHome].y)
				{
					g_aHome[nCntHome].VtxMaxHome.y = vtx[nCntHome].y;
				}
				if (g_aHome[nCntHome].VtxMaxHome.z < vtx[nCntHome].z)
				{
					g_aHome[nCntHome].VtxMaxHome.z = vtx[nCntHome].z;
				}
				pVtxBuff[nCntHome] += sizeFvF[nType];
			}
			//頂点バッファをアンロック
			g_pMeshHome[nType]->UnlockVertexBuffer();
		//	break;
		//}
	}

	for (int nCntHome = 0; nCntHome < MAX_HOME; nCntHome++)
	{
		if (g_aHome[nCntHome].bUse == false)
		{
			// 位置・向きの初期設定
			g_aHome[nCntHome].pos = pos;
			g_aHome[nCntHome].rot = D3DXVECTOR3(0, 0, 0);
			g_aHome[nCntHome].fShadow = 12;
			g_aHome[nCntHome].nType = nType;
			//　影のセット
			//g_aHome[nCntHome].nIdxShadow = SetShadow(g_aHome[nCntHome].pos, D3DXVECTOR3(0, 0, 0), 20.0f);
			g_aHome[nCntHome].bUse = true;
			break;
		}
	}

}
