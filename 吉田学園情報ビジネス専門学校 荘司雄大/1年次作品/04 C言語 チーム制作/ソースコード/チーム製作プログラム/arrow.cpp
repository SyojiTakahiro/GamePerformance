//=============================================================================
//
// 矢印処理 [arrow.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "arrow.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "polygon.h"
#include "model.h"
#include "player.h"
#include "game.h"
#include "explosion.h"
#include "goal.h"
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshArrow[MAX_ARROW];					//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatArrow[MAX_ARROW];			//マテリアル情報へのポインタ
LPDIRECT3DTEXTURE9	g_pTextureArrow[MAX_ARROW];		//テクスチャへのポインタ
DWORD g_nNumMatArrow = 0;							//マテリアル情報の数
ARROW g_aArrow[MAX_ARROW];

int ModelHandle1;
int ModelHandle2;
D3DXVECTOR3 Position1;
D3DXVECTOR3 Position2;
D3DXVECTOR3 SubVector;
float Angle;
//=============================================================================
// 初期化処理
//=============================================================================
void InitArrow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// Xファイルの読み込み
	D3DXLoadMeshFromX(ARROW_NAME000,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatArrow[0],
		NULL,
		&g_nNumMatArrow,
		&g_pMeshArrow[0]);

	int nNumVtex;	//頂点数
	DWORD sizeFvF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_ARROW];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_ARROW];

	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		g_aArrow[nCntArrow].VtxMinArrow = D3DXVECTOR3(10000, 0, 10000);
		g_aArrow[nCntArrow].VtxMaxArrow = D3DXVECTOR3(-10000, 0, -10000);
		g_aArrow[nCntArrow].nType = 0;
		g_aArrow[nCntArrow].bUse = false;

	}

	//頂点数を取得
	nNumVtex = g_pMeshArrow[0]->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFvF = D3DXGetFVFVertexSize(g_pMeshArrow[0]->GetFVF());

	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		//頂点　ロック
		g_pMeshArrow[0]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nCntArrow]);
		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
		{	//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff[nCntArrow];
			//MINVTX
			if (g_aArrow[nCntArrow].VtxMinArrow.x > vtx.x)
			{
				g_aArrow[nCntArrow].VtxMinArrow.x = vtx.x;
			}
			if (g_aArrow[nCntArrow].VtxMinArrow.y > vtx.y)
			{
				g_aArrow[nCntArrow].VtxMinArrow.y = vtx.y;
			}
			if (g_aArrow[nCntArrow].VtxMinArrow.z > vtx.z)
			{
				g_aArrow[nCntArrow].VtxMinArrow.z = vtx.z;
			}
			//MAXVTX
			if (g_aArrow[nCntArrow].VtxMaxArrow.x < vtx.x)
			{
				g_aArrow[nCntArrow].VtxMaxArrow.x = vtx.x;
			}
			if (g_aArrow[nCntArrow].VtxMaxArrow.y < vtx.y)
			{
				g_aArrow[nCntArrow].VtxMaxArrow.y = vtx.y;
			}
			if (g_aArrow[nCntArrow].VtxMaxArrow.z < vtx.z)
			{
				g_aArrow[nCntArrow].VtxMaxArrow.z = vtx.z;
			}
			//初期処理


			pVtxBuff[nCntArrow] += sizeFvF;
		}
		//			//頂点バッファをアンロック
		g_pMeshArrow[0]->UnlockVertexBuffer();
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		ARROW_NAME000,
		&g_pTextureArrow[0]);

	PLAYER *pPlayer = GetPlayer();

	// 位置・向きの初期設定
	g_aArrow[0].pos = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 45, pPlayer->pos.z);
	g_aArrow[0].rot = D3DXVECTOR3(0, pPlayer->rot.y, 0);
	g_aArrow[0].fShadow = 12;
	g_aArrow[0].bUse = true;

}
//=============================================================================
// 終了処理
//=============================================================================
void UninitArrow(void)
{
	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		// メッシュの開放
		if (g_pMeshArrow[nCntArrow] != NULL)
		{
			g_pMeshArrow[nCntArrow]->Release();
			g_pMeshArrow[nCntArrow] = NULL;
		}
		// マテリアルの開放
		if (g_pBuffMatArrow[nCntArrow] != NULL)
		{
			g_pBuffMatArrow[nCntArrow]->Release();
			g_pBuffMatArrow[nCntArrow] = NULL;
		}
		//テクスチャの破棄
		if (g_pTextureArrow[nCntArrow] != NULL)
		{
			g_pTextureArrow[nCntArrow]->Release();
			g_pTextureArrow[nCntArrow] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateArrow(void)
{
	PLAYER *pPlayer = GetPlayer();
	MODEL *pModel = GetModel();
	GOAL *pGoal = GetGoal();

	D3DXVECTOR3 VecA;
	D3DXVECTOR3 VecB;
	D3DXVECTOR3 VecC;

	g_aArrow[0].pos = D3DXVECTOR3(pPlayer->pos.x, pPlayer->pos.y + 75, pPlayer->pos.z);

	Position2 = D3DXVECTOR3(50.0f, 0.0f, 0.0f);
	
	VecA = pGoal->pos - g_aArrow[0].pos;
	VecC = g_aArrow[0].pos - pPlayer->pos;
	g_aArrow[0].DiffAngle.y = (VecA.z * VecC.x) - (VecA.x * VecC.z);
			
	//ゴールーとの角度を測る
	g_aArrow[0].DestAngle.y = atan2f((pGoal->pos.x - sinf(pGoal->rot.y) - g_aArrow[0].pos.x),
		(pGoal->pos.z - cosf(pGoal->rot.y) - g_aArrow[0].pos.z));
	//プレイヤーとの距離の差分
	g_aArrow[0].DiffAngle.y = g_aArrow[0].DestAngle.y - g_aArrow[0].rot.y;

	//角度の設定
	if (g_aArrow[0].DiffAngle.y > D3DX_PI)
	{
		g_aArrow[0].DiffAngle.y -= D3DX_PI* 2.0f;
	}
	if (g_aArrow[0].DiffAngle.y < -D3DX_PI)
	{
		g_aArrow[0].DiffAngle.y += D3DX_PI* 2.0f;
	}
	//向きの設定
	g_aArrow[0].rot.y += g_aArrow[0].DiffAngle.y * 1.0f;
	if (g_aArrow[0].rot.y > D3DX_PI)
	{
		g_aArrow[0].rot.y -= D3DX_PI * 2.0f;
	}
	if (g_aArrow[0].rot.y < -D3DX_PI)
	{
		g_aArrow[0].rot.y += D3DX_PI * 2.0f;
	}


}

//=============================================================================
// 描画処理
//=============================================================================
void DrawArrow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aArrow[nCntArrow].mtxWorld);
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aArrow[nCntArrow].rot.y, g_aArrow[nCntArrow].rot.x, g_aArrow[nCntArrow].rot.z);
		D3DXMatrixMultiply(&g_aArrow[nCntArrow].mtxWorld, &g_aArrow[nCntArrow].mtxWorld, &mtxRot);
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aArrow[nCntArrow].pos.x, g_aArrow[nCntArrow].pos.y, g_aArrow[nCntArrow].pos.z);
		D3DXMatrixMultiply(&g_aArrow[nCntArrow].mtxWorld, &g_aArrow[nCntArrow].mtxWorld, &mtxTrans);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aArrow[nCntArrow].mtxWorld);
		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);
		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatArrow[g_aArrow[nCntArrow].nType]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_nNumMatArrow; nCntMat++)
		{	// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			// テクスチャ
			pDevice->SetTexture(0, g_pTextureArrow[g_aArrow[nCntArrow].nType]);
			if (g_aArrow[nCntArrow].bUse == true)	//矢印が使用されている
			{
				// 矢印(パーツ)の描画
				g_pMeshArrow[g_aArrow[nCntArrow].nType]->DrawSubset(nCntMat);
			}
		}
		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
}

//*****************************************************************************
//矢印ポインタ処理
//*****************************************************************************
ARROW *GetArrow(void)
{
	return &g_aArrow[0];
}
//=============================================================================
// 設定処理
//=============================================================================
void SetArrow(D3DXVECTOR3 pos, int nType)
{

	int nNumVtex;	//頂点数
	DWORD sizeFvF[MAX_ARROW];	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_ARROW];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_ARROW];

	//頂点数を取得
	nNumVtex = g_pMeshArrow[nType]->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFvF[nType] = D3DXGetFVFVertexSize(g_pMeshArrow[nType]->GetFVF());

	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		//if (g_aArrow[nCntArrow].bUse == false)
		//{
			//頂点　ロック
			g_pMeshArrow[nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nType]);
			for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
			{	//頂点座標の代入
				vtx[nCntArrow] = *(D3DXVECTOR3*)pVtxBuff[nType];
				if (g_aArrow[nCntArrow].VtxMinArrow.x > vtx[nCntArrow].x)
				{
					g_aArrow[nCntArrow].VtxMinArrow.x = vtx[nCntArrow].x;
				}
				if (g_aArrow[nCntArrow].VtxMinArrow.y > vtx[nCntArrow].y)
				{
					g_aArrow[nCntArrow].VtxMinArrow.y = vtx[nCntArrow].y;
				}
				if (g_aArrow[nCntArrow].VtxMinArrow.z > vtx[nCntArrow].z)
				{
					g_aArrow[nCntArrow].VtxMinArrow.z = vtx[nCntArrow].z;
				}

				if (g_aArrow[nCntArrow].VtxMaxArrow.x < vtx[nCntArrow].x)
				{
					g_aArrow[nCntArrow].VtxMaxArrow.x = vtx[nCntArrow].x;
				}
				if (g_aArrow[nCntArrow].VtxMaxArrow.y < vtx[nCntArrow].y)
				{
					g_aArrow[nCntArrow].VtxMaxArrow.y = vtx[nCntArrow].y;
				}
				if (g_aArrow[nCntArrow].VtxMaxArrow.z < vtx[nCntArrow].z)
				{
					g_aArrow[nCntArrow].VtxMaxArrow.z = vtx[nCntArrow].z;
				}
				pVtxBuff[nCntArrow] += sizeFvF[nType];
			}
			//頂点バッファをアンロック
			g_pMeshArrow[nType]->UnlockVertexBuffer();
		//	break;
		//}
	}

	for (int nCntArrow = 0; nCntArrow < MAX_ARROW; nCntArrow++)
	{
		if (g_aArrow[nCntArrow].bUse == false)
		{
			// 位置・向きの初期設定
			g_aArrow[nCntArrow].pos = pos;
			g_aArrow[nCntArrow].rot = D3DXVECTOR3(0, 0, 0);
			g_aArrow[nCntArrow].fShadow = 12;
			g_aArrow[nCntArrow].nType = nType;
			//　影のセット
			g_aArrow[nCntArrow].nIdxShadow = SetShadow(g_aArrow[nCntArrow].pos, D3DXVECTOR3(0, 0, 0), 20.0f);
			g_aArrow[nCntArrow].bUse = true;
			break;
		}
	}

}