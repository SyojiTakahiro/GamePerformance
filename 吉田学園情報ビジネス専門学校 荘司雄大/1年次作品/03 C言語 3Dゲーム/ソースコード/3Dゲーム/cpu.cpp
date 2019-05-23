//=============================================================================
//
// モデル処理 [cpu.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "cpu.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "particle.h"
#include "explosion.h"
#include "building.h"
#include "game.h"
#include "cpulife.h"
#include "fade.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME		"data/MODEL/P2.x"
#define NORMAL_SPEED    (5)
#define DASH_SPEED		(6)
#define MAX_PARTS		(1)
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshcpu = NULL;			//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatcpu = NULL;	//マテリアル情報へのポインタ

CPU g_cpu;

LPDIRECT3DTEXTURE9 g_pTexturecpu = NULL;
DWORD g_nNumMatcpu = 0;				//マテリアル情報の数

										//D3DXVECTOR3 g_vtxMinModel, g_vtxMaxModel;

										//D3DXMATRIX mtxWorld2;

										//D3DXMATRIX g_mtxWorld2cpu;	
int g_nIdxCPUShadow;

#define CPU_LIFE			 (2)								//プレイヤーの体力

int g_BulletRecastCPU = 150;

//=============================================================================
// 初期化処理
//=============================================================================
void InitCPU(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntcpu = 0; nCntcpu < MAX_PLAYER; nCntcpu++)
	{
		g_cpu.state = CPUSTATE_NORMAL;
		g_cpu.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_cpu.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_cpu.rot = D3DXVECTOR3(0, 0, 0);
		g_cpu.nType = 0;
		g_cpu.nCounterState = 60;
		g_cpu.state;
		g_cpu.nLife = CPU_LIFE;
		g_cpu.DirectionMove = 0;
		g_cpu.bJump = false;
		g_cpu.bUse = true;
	}

	g_cpu.pos = D3DXVECTOR3(350.0f, 10.0f, -160.0f);

	//// Xファイルの読み込み
	//D3DXLoadMeshFromX(MODEL_NAME,
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatcpu,
	//	NULL,
	//	&g_nNumMatcpu,
	//	&g_pMeshcpu);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_cpu.aModel[0].pBuffMat,
		NULL,
		&g_cpu.aModel[0].nNumMat,
		&g_cpu.aModel[0].pMesh);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_NAME2,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_cpu.aModel[1].pBuffMat,
		NULL,
		&g_cpu.aModel[1].nNumMat,
		&g_cpu.aModel[1].pMesh);

	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff;	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx;

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_cpu.VtxMin = D3DXVECTOR3(10000, 0, 10000);
		g_cpu.VtxMax = D3DXVECTOR3(-10000, 0, -10000);
		g_cpu.nType = 0;
	}

	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		//頂点数を取得
		nNumVtx = g_cpu.aModel[nCntModel].pMesh->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_cpu.aModel[nCntModel].pMesh->GetFVF());

		//頂点　ロック
		g_cpu.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{	//頂点座標の代入
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (g_cpu.VtxMin.x > vtx.x)
			{
				g_cpu.VtxMin.x = vtx.x;
			}
			if (g_cpu.VtxMin.y > vtx.y)
			{
				g_cpu.VtxMin.y = vtx.y;
			}
			if (g_cpu.VtxMin.z > vtx.z)
			{
				g_cpu.VtxMin.z = vtx.z;
			}

			if (g_cpu.VtxMax.x < vtx.x)
			{
				g_cpu.VtxMax.x = vtx.x;
			}
			if (g_cpu.VtxMax.y < vtx.y)
			{
				g_cpu.VtxMax.y = vtx.y;
			}
			if (g_cpu.VtxMax.z < vtx.z)
			{
				g_cpu.VtxMax.z = vtx.z;
			}
			//初期処理
			pVtxBuff += sizeFVF;
		}
		//			//頂点バッファをアンロック
		g_cpu.aModel[nCntModel].pMesh->UnlockVertexBuffer();
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MODEL_NAME, &g_pTexturecpu);

	//オゥフセット
	g_cpu.aModel[0].nIdxModelParent = -1;
	g_cpu.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_cpu.aModel[1].nIdxModelParent = 0;
	g_cpu.aModel[1].pos = D3DXVECTOR3(0.0f, 15.0f, 8.0f);
	g_cpu.aModel[1].rot = D3DXVECTOR3(-150.0f, 0.0f, 0.0f);

	g_nIdxCPUShadow = SetShadow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_cpu.rot);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCPU(void)
{
	// メッシュの開放
	if (g_pMeshcpu != NULL)
	{
		g_pMeshcpu->Release();
		g_pMeshcpu = NULL;
	}

	// マテリアルの開放
	if (g_pBuffMatcpu != NULL)
	{
		g_pBuffMatcpu->Release();
		g_pBuffMatcpu = NULL;
	}

	//テクスチャの破棄
	if (g_pTexturecpu != NULL)
	{
		g_pTexturecpu->Release();
		g_pTexturecpu = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCPU(void)
{
	CAMERA *pCamera;
	pCamera = GetCamera();

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	GAMESTATE GameState = GetGameState();

	float fData;

	g_cpu.posOld = g_cpu.pos;

	g_cpu.rot.y++;

	//g_cpu.pos.x -= sinf(g_cpu.rot.y + D3DX_PI * 0.75f) * NORMAL_SPEED;
	//g_cpu.pos.z -= cosf(g_cpu.rot.y + D3DX_PI * 0.75f) * NORMAL_SPEED;

	//pCamera->rot.y;

	if (GetKeyboardPress(DIK_RCONTROL) == true)
	{
		g_cpu.pos.y -= cosf(D3DX_PI * 0) * 4;
	}

	/*if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
	g_cpu.rot.y -= cosf(-D3DX_PI) * 6;
	}*/
	/*if (GetKeyboardPress(DIK_RSHIFT) == true)
	{
	g_cpu.rot.y -= cosf(D3DX_PI * 0) * 6;
	}*/
	g_BulletRecastCPU++;
	if (g_BulletRecastCPU >= 150)
	{
		/*if (GetKeyboardTrigger(DIK_L) == true)
		{*/
			fData = atan2f(pPlayer->pos.x - g_cpu.pos.x, pPlayer->pos.z - g_cpu.pos.z);
			SetBullet(D3DXVECTOR3(g_cpu.pos.x, g_cpu.pos.y + 5, g_cpu.pos.z), D3DXVECTOR3(sinf(fData) * 5.0f, tanf(fData) * 5.0f, cosf(fData) * 5.0f), 10.0f, 10.0f, BULLETTYPE_CPU);
			g_BulletRecastCPU = 0;
		//}
	}

	if (g_cpu.pos.x  < 30)
	{
		g_cpu.pos.x = 80;
		CutCPULife(1);
		g_cpu.nLife--;
	}
	if (g_cpu.pos.x > 500)
	{
		g_cpu.pos.x = 450;
		CutCPULife(1);
		g_cpu.nLife--;
	}
	if (g_cpu.pos.z  > 50)
	{
		g_cpu.pos.z = 0;
		CutCPULife(1);
		g_cpu.nLife--;
	}
	if (g_cpu.pos.z < -340)
	{
		g_cpu.pos.z = -260;
		CutCPULife(1);
		g_cpu.nLife--;
	}

	/*if (g_cpu.nLife == 0)
	{
	if (GameState != GAMESTATE_P1WIN)
	{
	SetFade(MODE_P1WIN);
	}
	}*/

	//if (GetKeyboardPress(DIK_RSHIFT) == false)
	//{
	//	SetParticle(g_cpu.pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), 10.0f, 100);
	//	//斜め移動の処理
	//	if (GetKeyboardPress(DIK_LEFT) == true)
	//	{
	//		if (GetKeyboardPress(DIK_UP) == true)
	//		{//左上移動
	//			g_cpu.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.75f) * NORMAL_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.75f) * NORMAL_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.75f);
	//		}
	//		else if (GetKeyboardPress(DIK_DOWN) == true)
	//		{//左下移動
	//			g_cpu.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.25f) * NORMAL_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.25f) * NORMAL_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.25f);
	//		}
	//		else
	//		{//左移動
	//			g_cpu.pos.x += sinf(pCamera->rot.y - D3DX_PI * 0.5f) * NORMAL_SPEED;
	//			g_cpu.pos.z += cosf(pCamera->rot.y - D3DX_PI * 0.5f) * NORMAL_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.5f);
	//		}
	//	}

	//	//斜め移動の処理
	//	else if (GetKeyboardPress(DIK_RIGHT) == true)
	//	{
	//		if (GetKeyboardPress(DIK_UP) == true)
	//		{//右上移動
	//			g_cpu.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.75f) * NORMAL_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.75f) * NORMAL_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y - D3DX_PI * 0.75f);
	//		}
	//		else if (GetKeyboardPress(DIK_DOWN) == true)
	//		{//右下移動
	//			g_cpu.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.25f) * NORMAL_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.25f) * NORMAL_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y - D3DX_PI * 0.25f);
	//		}
	//		else
	//		{//右移動
	//			g_cpu.pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * NORMAL_SPEED;
	//			g_cpu.pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * NORMAL_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y - D3DX_PI * 0.5f);
	//		}
	//	}
	//	else if (GetKeyboardPress(DIK_UP) == true)
	//	{
	//		g_cpu.pos.x += sinf(pCamera->rot.y) * NORMAL_SPEED;
	//		g_cpu.pos.z += cosf(pCamera->rot.y) * NORMAL_SPEED;
	//		//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 1.0f);
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN) == true)
	//	{
	//		g_cpu.pos.x -= sinf(pCamera->rot.y) * NORMAL_SPEED;
	//		g_cpu.pos.z -= cosf(pCamera->rot.y) * NORMAL_SPEED;
	//		//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.0f);
	//	}

	//}
	//if (GetKeyboardPress(DIK_RSHIFT) == true)
	//{
	//	SetParticle(g_cpu.pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 20.0f, 100);
	//	//斜め移動の処理
	//	if (GetKeyboardPress(DIK_LEFT) == true)
	//	{
	//		if (GetKeyboardPress(DIK_UP) == true)
	//		{//左上移動
	//			g_cpu.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.75f) * DASH_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.75f) * DASH_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.75f);
	//		}
	//		else if (GetKeyboardPress(DIK_DOWN) == true)
	//		{//左下移動
	//			g_cpu.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.25f) * DASH_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.25f) * DASH_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.25f);
	//		}
	//		else
	//		{//左移動
	//			g_cpu.pos.x += sinf(pCamera->rot.y - D3DX_PI * 0.5f) * DASH_SPEED;
	//			g_cpu.pos.z += cosf(pCamera->rot.y - D3DX_PI * 0.5f) * DASH_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.5f);
	//		}
	//	}

	//	//斜め移動の処理
	//	else if (GetKeyboardPress(DIK_RIGHT) == true)
	//	{
	//		if (GetKeyboardPress(DIK_UP) == true)
	//		{//右上移動
	//			g_cpu.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.75f) * DASH_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.75f) * DASH_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y - D3DX_PI * 0.75f);
	//		}
	//		else if (GetKeyboardPress(DIK_DOWN) == true)
	//		{//右下移動
	//			g_cpu.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.25f) * DASH_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.25f) * DASH_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y - D3DX_PI * 0.25f);
	//		}
	//		else
	//		{//右移動
	//			g_cpu.pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * DASH_SPEED;
	//			g_cpu.pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * DASH_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y - D3DX_PI * 0.5f);
	//		}
	//	}
	//	else if (GetKeyboardPress(DIK_UP) == true)
	//	{
	//		g_cpu.pos.x += sinf(pCamera->rot.y) * DASH_SPEED;
	//		g_cpu.pos.z += cosf(pCamera->rot.y) * DASH_SPEED;
	//		//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 1.0f);
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN) == true)
	//	{
	//		g_cpu.pos.x -= sinf(pCamera->rot.y) * DASH_SPEED;
	//		g_cpu.pos.z -= cosf(pCamera->rot.y) * DASH_SPEED;
	//		//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.0f);
	//	}
	//}

	/*if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		g_cpu.move.y -= cosf(-D3DX_PI) * 2;
	}*/

	g_cpu.pos.x -= pPlayer->move.x;
	g_cpu.pos.z -= pPlayer->move.z;
	fData = atan2f(pPlayer->pos.x - sinf(pPlayer->rot.y) - g_cpu.pos.x, pPlayer->pos.z -cosf(pPlayer->rot.y) - g_cpu.pos.z);
	g_cpu.pos += D3DXVECTOR3(sinf(fData) * NORMAL_SPEED, 0.0f, cosf(fData) * NORMAL_SPEED);

	g_cpu.move.y -= 0.1f;

	if (g_cpu.pos.y < 1.0f)
	{
		SetExplosion(g_cpu.pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
		g_cpu.pos.y = 1;
		g_cpu.move.y = 0.0f;
	}

	g_cpu.pos.x += g_cpu.move.x;
	g_cpu.pos.z += g_cpu.move.z;
	g_cpu.pos.y += g_cpu.move.y;

	if (CollisionPlayer(&g_cpu.pos, &g_cpu.posOld, &g_cpu.move, g_cpu.VtxMax, g_cpu.VtxMin) == true)
	{

	}

	if (CollisionBuilding(&g_cpu.pos, &g_cpu.posOld, &g_cpu.move, g_cpu.VtxMax, g_cpu.VtxMin) == true)
	{

	}

	g_cpu.move.x += (0.0f - g_cpu.move.x) * 0.15f;
	g_cpu.move.z += (0.0f - g_cpu.move.z) * 0.15f;

	SetPositionShadow(g_nIdxCPUShadow, D3DXVECTOR3(g_cpu.pos.x, 0.1f, g_cpu.pos.z));
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCPU(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;
	D3DXMATRIX mtxParent;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_cpu.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_cpu.rot.y, g_cpu.rot.x, g_cpu.rot.z);

	D3DXMatrixMultiply(&g_cpu.mtxWorld, &g_cpu.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_cpu.pos.x, g_cpu.pos.y, g_cpu.pos.z);

	D3DXMatrixMultiply(&g_cpu.mtxWorld, &g_cpu.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_cpu.mtxWorld);
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (g_cpu.aModel[nCnt].nIdxModelParent == -1)
		{
			mtxParent = g_cpu.mtxWorld;
		}
		else
		{
			mtxParent = g_cpu.aModel[g_cpu.aModel[nCnt].nIdxModelParent].mtxWorld;
		}
		//=============================================================================
		// 親のモデル
		//=============================================================================
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_cpu.aModel[nCnt].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_cpu.aModel[nCnt].rot.y, g_cpu.aModel[nCnt].rot.x, g_cpu.aModel[nCnt].rot.z);

		D3DXMatrixMultiply(&g_cpu.aModel[nCnt].mtxWorld, &g_cpu.aModel[nCnt].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_cpu.aModel[nCnt].pos.x, g_cpu.aModel[nCnt].pos.y, g_cpu.aModel[nCnt].pos.z);

		D3DXMatrixMultiply(&g_cpu.aModel[nCnt].mtxWorld, &g_cpu.aModel[nCnt].mtxWorld, &mtxTrans);

		D3DXMatrixMultiply(&g_cpu.aModel[nCnt].mtxWorld, &g_cpu.aModel[nCnt].mtxWorld, &mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_cpu.aModel[nCnt].mtxWorld);


		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_cpu.aModel[nCnt].pBuffMat->GetBufferPointer();


		for (int nCntMat = 0; nCntMat < (int)g_cpu.aModel[nCnt].nNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの読み込み
			pDevice->SetTexture(0, g_pTexturecpu);

			// モデル(パーツ)の描画
			g_cpu.aModel[nCnt].pMesh->DrawSubset(nCntMat);
		}
		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
	//=============================================================================
	// 子のモデル
	//=============================================================================
}

CPU *GetCPU(void)
{
	return &g_cpu;
}

//*****************************************************************************
//モデルとの当たり判定処理
//*****************************************************************************
bool CollisionCPU(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pModel*/)
{
	bool bLand = false;

	//GAMESTATE GameState = GetGameState();



	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		if (g_cpu.bUse == true)
		{
			//モデルのY幅の中
			if (pPos->y < g_cpu.pos.y + g_cpu.VtxMax.y
				&& pPos->y > g_cpu.pos.y + g_cpu.VtxMin.y)
			{
				//モデルのZ幅の中
				if (pPos->z + VtxMin.z < g_cpu.pos.z + g_cpu.VtxMax.z
					&& pPos->z + VtxMax.z > g_cpu.pos.z + g_cpu.VtxMin.z)
				{
					//モデルのX幅の中
					if (pPos->x + VtxMin.x < g_cpu.pos.x + g_cpu.VtxMax.x
						&& pPos->x + VtxMax.x > g_cpu.pos.x + g_cpu.VtxMin.x)
					{

						//X幅の当たり判定
						if (pPosOld->x > g_cpu.pos.x + g_cpu.VtxMax.x)
						{
							g_cpu.move.x += pMove->x - 15;

							//g_cpu.move.x += bMove->x;

							pPos->x = g_cpu.pos.x + g_cpu.VtxMax.x + VtxMax.x;
							pPosOld->x = g_cpu.pos.x + g_cpu.VtxMax.x + VtxMax.x;
							//pMove->x = 0;
							bLand = true;
						}
						else if (pPosOld->x < g_cpu.pos.x + g_cpu.VtxMin.x)
						{
							if (g_cpu.nType == 1)
							{

							}

							g_cpu.move.x += pMove->x + 15;
							pPos->x = g_cpu.pos.x + g_cpu.VtxMin.x + VtxMin.x;
							pPosOld->x = g_cpu.pos.x + g_cpu.VtxMin.x + VtxMin.x;
							//pMove->x = 0;
							bLand = true;

						}
						//Z幅の当たり判定
						if (pPosOld->z > g_cpu.pos.z + g_cpu.VtxMax.z)
						{
							if (g_cpu.nType == 1)
							{

							}
							bLand = true;

							g_cpu.move.z += pMove->z - 15;

							pPos->z = g_cpu.pos.z + g_cpu.VtxMax.z + VtxMax.z;
							pPosOld->z = g_cpu.pos.z + g_cpu.VtxMax.z + VtxMax.z;
							//pMove->z = 0;
						}
						else if (pPosOld->z < g_cpu.pos.z + g_cpu.VtxMin.z)
						{
							if (g_cpu.nType == 1)
							{

							}
							bLand = true;

							g_cpu.move.z += pMove->z + 15;

							pPos->z = g_cpu.pos.z + g_cpu.VtxMin.z + VtxMin.z;
							pPosOld->z = g_cpu.pos.z + g_cpu.VtxMin.z + VtxMin.z;
							//pMove->z = 0;
						}
						//Y幅の当たり判定
						if (pPosOld->y >= g_cpu.pos.y + g_cpu.VtxMax.y)
						{
							pPos->y = g_cpu.pos.y + g_cpu.VtxMax.y;
							pPosOld->y = g_cpu.pos.y + g_cpu.VtxMax.y;
							pMove->y = 0;

						}
						else if (pPosOld->y <= g_cpu.pos.y + g_cpu.VtxMin.y)
						{
							pPos->y = g_cpu.pos.y + g_cpu.VtxMin.y;
							pPosOld->y = g_cpu.pos.y + g_cpu.VtxMin.y;
						}
						bLand = true;
					}
				}
			}
		}
	}
	return bLand;
}