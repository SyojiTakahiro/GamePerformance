//=============================================================================
//
// モデル処理 [pokuru.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "pokuru.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "polygon.h"
#include "model.h"
#include "game.h"
#include "goal.h"
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pTexturePokuru = NULL;		//テクスチャへのポインタ
POKURU g_aPokuru;
//=============================================================================
// 初期化処理
//=============================================================================
void InitPokuru(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// Xファイルの読み込み
	D3DXLoadMeshFromX(POKURU_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPokuru.aModel[0].pBuffMat,
		NULL,
		&g_aPokuru.aModel[0].nNumMat,
		&g_aPokuru.aModel[0].pMesh);
	// Xファイルの読み込み
	D3DXLoadMeshFromX(POKURU_NAME1,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPokuru.aModel[1].pBuffMat,
		NULL,
		&g_aPokuru.aModel[1].nNumMat,
		&g_aPokuru.aModel[1].pMesh);
	// Xファイルの読み込み
	D3DXLoadMeshFromX(POKURU_NAME2,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPokuru.aModel[2].pBuffMat,
		NULL,
		&g_aPokuru.aModel[2].nNumMat,
		&g_aPokuru.aModel[2].pMesh);
	// Xファイルの読み込み
	D3DXLoadMeshFromX(POKURU_NAME3,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPokuru.aModel[3].pBuffMat,
		NULL,
		&g_aPokuru.aModel[3].nNumMat,
		&g_aPokuru.aModel[3].pMesh);
	// Xファイルの読み込み
	D3DXLoadMeshFromX(POKURU_NAME4,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPokuru.aModel[4].pBuffMat,
		NULL,
		&g_aPokuru.aModel[4].nNumMat,
		&g_aPokuru.aModel[4].pMesh);
	// Xファイルの読み込み
	D3DXLoadMeshFromX(POKURU_NAME5,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPokuru.aModel[5].pBuffMat,
		NULL,
		&g_aPokuru.aModel[5].nNumMat,
		&g_aPokuru.aModel[5].pMesh);
	// Xファイルの読み込み
	D3DXLoadMeshFromX(POKURU_NAME6,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPokuru.aModel[6].pBuffMat,
		NULL,
		&g_aPokuru.aModel[6].nNumMat,
		&g_aPokuru.aModel[6].pMesh);
	// Xファイルの読み込み
	D3DXLoadMeshFromX(POKURU_NAME7,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPokuru.aModel[7].pBuffMat,
		NULL,
		&g_aPokuru.aModel[7].nNumMat,
		&g_aPokuru.aModel[7].pMesh);
	// Xファイルの読み込み
	D3DXLoadMeshFromX(POKURU_NAME8,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPokuru.aModel[8].pBuffMat,
		NULL,
		&g_aPokuru.aModel[8].nNumMat,
		&g_aPokuru.aModel[8].pMesh);
	// Xファイルの読み込み
	D3DXLoadMeshFromX(POKURU_NAME9,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_aPokuru.aModel[9].pBuffMat,
		NULL,
		&g_aPokuru.aModel[9].nNumMat,
		&g_aPokuru.aModel[9].pMesh);

	int nNumVtex;	//頂点数
	DWORD sizeFvF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx;

	for (int nCntPokuru = 0; nCntPokuru < POKURU_PARTS; nCntPokuru++)
	{
		//頂点数を取得
		nNumVtex = g_aPokuru.aModel[nCntPokuru].pMesh->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFvF = D3DXGetFVFVertexSize(g_aPokuru.aModel[nCntPokuru].pMesh->GetFVF());

		//頂点バッファをロック
		g_aPokuru.aModel[nCntPokuru].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
		{	//頂点座標の代入

			vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (g_aPokuru.VtxMin.x > vtx.x)
			{
				g_aPokuru.VtxMin.x = vtx.x;
			}
			if (g_aPokuru.VtxMin.y > vtx.y)
			{
				g_aPokuru.VtxMin.y = vtx.y;
			}
			if (g_aPokuru.VtxMin.z > vtx.z)
			{
				g_aPokuru.VtxMin.z = vtx.z;
			}

			if (g_aPokuru.VtxMax.x < vtx.x)
			{
				g_aPokuru.VtxMax.x = vtx.x;
			}
			if (g_aPokuru.VtxMax.y < vtx.y)
			{
				g_aPokuru.VtxMax.y = vtx.y;
			}
			if (g_aPokuru.VtxMax.z < vtx.z)
			{
				g_aPokuru.VtxMax.z = vtx.z;
			}

			//サイズ分ポインタを進める
			pVtxBuff += sizeFvF;
		}
		//頂点バッファをアンロック
		g_aPokuru.aModel[nCntPokuru].pMesh->UnlockVertexBuffer();
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		POKURU_NAME,
		&g_pTexturePokuru);

	GOAL *pGoal = GetGoal();

	// 位置・向きの初期設定
	g_aPokuru.pos = D3DXVECTOR3(pGoal->pos.x, pGoal->pos.y, pGoal->pos.z);
	g_aPokuru.rot = D3DXVECTOR3(0, D3DX_PI * 0.0f, 0);
	g_aPokuru.move = D3DXVECTOR3(0, 0, 0);
	g_aPokuru.fShadow = 1;
	g_aPokuru.fDestAngle = D3DX_PI * 1.0f;
	g_aPokuru.state = POKURUSTATE_NORMAL;
	g_aPokuru.fDiffAngle = 0;
	g_aPokuru.fHeight = 5;
	g_aPokuru.fWidth = 10;
	//モデルのオフセット
	g_aPokuru.aModel[0].nIdxModelParent = -1;
	g_aPokuru.aModel[0].pos = D3DXVECTOR3(0, 30, 0);

	g_aPokuru.aModel[1].nIdxModelParent = 0;
	g_aPokuru.aModel[1].pos = D3DXVECTOR3(0, 30, 0);

	g_aPokuru.aModel[2].nIdxModelParent = 0;
	g_aPokuru.aModel[2].pos = D3DXVECTOR3(-8.5, 23.0, 0.0);
	g_aPokuru.aModel[2].rot = D3DXVECTOR3(0.0f, 0.0f, 1.4f);

	g_aPokuru.aModel[3].nIdxModelParent = 2;
	g_aPokuru.aModel[3].pos = D3DXVECTOR3(-30.0, -1.0, 0.0);

	g_aPokuru.aModel[4].nIdxModelParent = 0;
	g_aPokuru.aModel[4].pos = D3DXVECTOR3(8.5, 25.0, 0.0);
	g_aPokuru.aModel[4].rot = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	g_aPokuru.aModel[5].nIdxModelParent = 4;
	g_aPokuru.aModel[5].pos = D3DXVECTOR3(30.0, -3.0, 0.0);

	g_aPokuru.aModel[6].nIdxModelParent = 0;
	g_aPokuru.aModel[6].pos = D3DXVECTOR3(-5.0, 0.0, 0.0);

	g_aPokuru.aModel[7].nIdxModelParent = 6;
	g_aPokuru.aModel[7].pos = D3DXVECTOR3(0.0, -25.0, 5.0);

	g_aPokuru.aModel[8].nIdxModelParent = 0;
	g_aPokuru.aModel[8].pos = D3DXVECTOR3(5.0, 0.0, 0.0);

	g_aPokuru.aModel[9].nIdxModelParent = 8;
	g_aPokuru.aModel[9].pos = D3DXVECTOR3(0.0f, -25.0f, 2.7f);

	//　影のセット
	g_aPokuru.nIdxShadow = SetShadow(D3DXVECTOR3(0, 0, 100), D3DXVECTOR3(0, 0, 0), 20.0f);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPokuru(void)
{
	for (int nCntPokuru = 0; nCntPokuru < POKURU_PARTS; nCntPokuru++)
	{
		// メッシュの開放
		if (g_aPokuru.aModel[nCntPokuru].pMesh != NULL)
		{
			g_aPokuru.aModel[nCntPokuru].pMesh->Release();
			g_aPokuru.aModel[nCntPokuru].pMesh = NULL;
		}
		// マテリアルの開放
		if (g_aPokuru.aModel[nCntPokuru].pBuffMat != NULL)
		{
			g_aPokuru.aModel[nCntPokuru].pBuffMat->Release();
			g_aPokuru.aModel[nCntPokuru].pBuffMat = NULL;
		}
	}
	//テクスチャの破棄
	if (g_pTexturePokuru != NULL)
	{
		g_pTexturePokuru->Release();
		g_pTexturePokuru = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePokuru(void)
{
	Camera *pCamera = GetCamera();
	GOAL *pGoal = GetGoal();

	// 位置・向きの初期設定
	g_aPokuru.pos = D3DXVECTOR3(pGoal->pos.x, pGoal->pos.y, pGoal->pos.z + 70);

	//カメラとの距離を求める
	double length = pow((g_aPokuru.pos.x - pCamera->posV.x)*(g_aPokuru.pos.x - pCamera->posV.x)
		+ (g_aPokuru.pos.z - pCamera->posV.z)*(g_aPokuru.pos.z - pCamera->posV.z), 0.5);

	//float型に変換
	g_aPokuru.fLength = (float)length;

	//ゲームステイトの取得
	GAMESTATE GameState;
	GameState = GetGameState();

	if (0 >= g_aPokuru.pos.y)
	{
		g_aPokuru.pos.y = 0 + 0.1f;
		g_aPokuru.move.y = 0;
	}

	//モデルとの当たり判定
	if (CollisionModel(&g_aPokuru.pos, &g_aPokuru.posold, &g_aPokuru.move, g_aPokuru.VtxMax, g_aPokuru.VtxMin) == true)
	{//モデルに着地したとき

	}

	SetPositionShadow(g_aPokuru.nIdxShadow, D3DXVECTOR3(g_aPokuru.pos.x, 0, g_aPokuru.pos.z), 15);
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPokuru(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;
	if (g_aPokuru.fLength < 2200)
	{//モデルとプレイヤーの距離で描画するか決める
	// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPokuru.mtxWorld);
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPokuru.rot.y, g_aPokuru.rot.x, g_aPokuru.rot.z);
		D3DXMatrixMultiply(&g_aPokuru.mtxWorld, &g_aPokuru.mtxWorld, &mtxRot);
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aPokuru.pos.x, g_aPokuru.pos.y, g_aPokuru.pos.z);
		D3DXMatrixMultiply(&g_aPokuru.mtxWorld, &g_aPokuru.mtxWorld, &mtxTrans);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPokuru.mtxWorld);

		for (int nCntPokuru = 0; nCntPokuru < POKURU_PARTS; nCntPokuru++)
		{
			if (g_aPokuru.aModel[nCntPokuru].nIdxModelParent == -1)
			{
				mtxParent = g_aPokuru.mtxWorld;
			}
			else
			{
				mtxParent = g_aPokuru.aModel[g_aPokuru.aModel[nCntPokuru].nIdxModelParent].mtxWorld;
			}

			// 子モデル
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPokuru.aModel[nCntPokuru].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPokuru.aModel[nCntPokuru].rot.y, g_aPokuru.aModel[nCntPokuru].rot.x, g_aPokuru.aModel[nCntPokuru].rot.z);
			D3DXMatrixMultiply(&g_aPokuru.aModel[nCntPokuru].mtxWorld, &g_aPokuru.aModel[nCntPokuru].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aPokuru.aModel[nCntPokuru].pos.x, g_aPokuru.aModel[nCntPokuru].pos.y, g_aPokuru.aModel[nCntPokuru].pos.z);
			D3DXMatrixMultiply(&g_aPokuru.aModel[nCntPokuru].mtxWorld, &g_aPokuru.aModel[nCntPokuru].mtxWorld, &mtxTrans);
			//親の情報を入れる
			D3DXMatrixMultiply(&g_aPokuru.aModel[nCntPokuru].mtxWorld, &g_aPokuru.aModel[nCntPokuru].mtxWorld, &mtxParent);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPokuru.aModel[nCntPokuru].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);
			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPokuru.aModel[nCntPokuru].pBuffMat->GetBufferPointer();
			for (int nCntMat = 0; nCntMat < (int)g_aPokuru.aModel[nCntPokuru].nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				// テクスチャ
				pDevice->SetTexture(0, g_pTexturePokuru);
				// モデル(パーツ)の描画
				g_aPokuru.aModel[nCntPokuru].pMesh->DrawSubset(nCntMat);
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//*****************************************************************************
//プレイヤーポインタ処理
//*****************************************************************************
POKURU *GetPokuru(void)
{
	return &g_aPokuru;
}