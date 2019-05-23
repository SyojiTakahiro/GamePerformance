//=============================================================================
//
// モデル処理 [player.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "player2.h"
#include "cpu.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "particle.h"
#include "p1win.h"
#include "sound.h"
#include "fade.h"
#include "game.h"
#include "building.h"
#include "score.h"
#include "explosion.h"
#include "life.h"
//#include "Diutil.h" 
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_NAME		"data/MODEL/P1.x"
#define NORMAL_SPEED    (5)
#define DASH_SPEED		(6)
#define MAX_PARTS		(1)
//*****************************************************************************
// グローバル変数
//*****************************************************************************
//LPD3DXMESH g_pMeshPlayer = NULL;			//メッシュ情報へのポインタ
//LPD3DXBUFFER g_pBuffMatPlayer = NULL;		//マテリアル情報へのポインタ

PLAYER g_player;
//MODEL g_model;

int RemainingBullets;

LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;
//DWORD g_nNumMatPlayer = 0;					//マテリアル情報の数

//D3DXMATRIX mtxWorld;

//D3DXVECTOR3 g_vtxMinModel, g_vtxMaxModel;

//D3DXMATRIX g_mtxWorldPlayer;	
int g_nIdxShadow;

float g_pSpin = 1.01f;

#define PLAYER_LIFE			 (2)			//プレイヤーの体力

int g_BulletRecast = 150;

//JOYINFOEX   joyinfo;

//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		g_player.state = PLAYERSTATE_NORMAL;
		g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_player.rot = D3DXVECTOR3(0, 0, 0);
		g_player.nType = 0;
		g_player.nCounterState = 60;
		g_player.state;
		g_player.nLife = PLAYER_LIFE;
		g_player.DirectionMove = 0;
		g_player.BulletKind = BULLETKIND_NOMAL;
		g_player.bJump = false;
		g_player.bUse = true;
	}

	g_player.pos = D3DXVECTOR3(130.0f, 10.0f, -160.0f);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_player.aModel[0].pBuffMat,
		NULL,
		&g_player.aModel[0].nNumMat,
		&g_player.aModel[0].pMesh);

	// Xファイルの読み込み
	D3DXLoadMeshFromX(MODEL_NAME2,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_player.aModel[1].pBuffMat,
		NULL,
		&g_player.aModel[1].nNumMat,
		&g_player.aModel[1].pMesh);

	int nNumVtx;	//頂点数
	DWORD sizeFVF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_PARTS];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_PARTS];

	

	//nNumVtx = g_player.aModel[0].pMesh->GetNumVertices();

	//sizeFVF = D3DXGetFVFVertexSize(g_player.aModel[0].pMesh->GetFVF());

	//g_player.aModel[0].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//for (int nCntVtx=0; nCntVtx < nNumVtx; nCntVtx++)
	//{
	//	D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
	//	//----------MIN-----------
	//	if (vtx.x < g_vtxMinModel.x)
	//	{
	//		g_vtxMinModel.x = 0;
	//	}

	//	if (vtx.y < g_vtxMinModel.y)
	//	{
	//		g_vtxMinModel.y = 0;
	//	}

	//	if (vtx.z < g_vtxMinModel.z)
	//	{
	//		g_vtxMinModel.z = 0;
	//	}
	//	//----------MAX------------
	//	if (vtx.x > g_vtxMaxModel.x)
	//	{
	//		g_vtxMaxModel.x = 10;
	//	}
	//	
	//	if (vtx.y > g_vtxMaxModel.y)
	//	{
	//		g_vtxMaxModel.y = 10;
	//	}

	//	if (vtx.z > g_vtxMaxModel.z)
	//	{
	//		g_vtxMaxModel.z = -10;
	//	}

	//	pVtxBuff[MAX_MODEL] += sizeFVF;
	//}

	//g_player.aModel[0].pMesh->UnlockVertexBuffer();

	
	
	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		g_player.VtxMin = D3DXVECTOR3(10000, 0, 10000);
		g_player.VtxMax = D3DXVECTOR3(-10000, 0, -10000);
		g_player.nType = 0;
	}

	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
		{
		//頂点数を取得
		nNumVtx = g_player.aModel[0].pMesh->GetNumVertices();
		//頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_player.aModel[0].pMesh ->GetFVF());
		//頂点　ロック
		g_player.aModel[0].pMesh ->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nCntModel]);
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{	//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff[nCntModel];

			if (g_player.VtxMin.x > vtx.x)
			{
				g_player.VtxMin.x = vtx.x;
			}
			if (g_player.VtxMin.y > vtx.y)
			{
				g_player.VtxMin.y = vtx.y;
			}
			if (g_player.VtxMin.z > vtx.z)
			{
				g_player.VtxMin.z = vtx.z;
			}

			if (g_player.VtxMax.x < vtx.x)
			{
				g_player.VtxMax.x = vtx.x;
			}
			if (g_player.VtxMax.y < vtx.y)
			{
				g_player.VtxMax.y = vtx.y;
			}
			if (g_player.VtxMax.z < vtx.z)
			{
				g_player.VtxMax.z = vtx.z;
			}
			//初期処理
			pVtxBuff[nCntModel] += sizeFVF;
		}
		//			//頂点バッファをアンロック
		g_player.aModel[0].pMesh ->UnlockVertexBuffer();
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, MODEL_NAME, &g_pTexturePlayer);

	//オゥフセット
	g_player.aModel[0].nIdxModelParent = -1;
	g_player.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_player.aModel[1].nIdxModelParent = 0;
	g_player.aModel[1].pos = D3DXVECTOR3(0.0f, 15.0f, 8.0f);
	g_player.aModel[1].rot = D3DXVECTOR3(-150.0f, 0.0f, 0.0f);

	g_nIdxShadow = SetShadow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_player.rot);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		// メッシュの開放
		if (g_player.aModel[nCnt].pMesh != NULL)
		{
			g_player.aModel[nCnt].pMesh->Release();
			g_player.aModel[nCnt].pMesh = NULL;
		}

		// マテリアルの開放
		if (g_player.aModel[nCnt].pBuffMat != NULL)
		{
			g_player.aModel[nCnt].pBuffMat->Release();
			g_player.aModel[nCnt].pBuffMat = NULL;
		}
	}
	////テクスチャの破棄
	//if (g_pTexturePlayer != NULL)
	//{
	//	g_pTexturePlayer->Release();
	//	g_pTexturePlayer = NULL;
	//}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	CAMERA *pCamera;
	PLAYER2 *pPlayer2;
	CPU *pCPU;

	pCamera = GetCamera();
	pPlayer2 = GetPlayer2();
	pCPU = GetCPU();

	FADE fade;
	fade = GetFade();

	MODE Mode = GetMode();


	GAMESTATE GameState = GetGameState();

	g_player.posOld = g_player.pos;

	g_player.rot.y++;


	float fData;

	//pCamera->rot.y;
	
	/*if (GetKeyboardPress(DIK_LCONTROL) == true)
	{
		g_player.pos.y -= cosf(D3DX_PI * 0) * 4;
	}*/

	/*if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
		g_player.rot.y -= cosf(-D3DX_PI) * 6;
	}*/
	/*if (GetKeyboardPress(DIK_RSHIFT) == true)
	{
		g_player.rot.y -= cosf(D3DX_PI * 0) * 6;
	}*/


	
	g_BulletRecast++;
	if (g_BulletRecast >= 150 && Mode == MODE_GAME)
	{
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			fData = atan2f(pPlayer2->pos.x - g_player.pos.x, pPlayer2->pos.z - g_player.pos.z);
			SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 5, g_player.pos.z), D3DXVECTOR3(sinf(fData) * 5.0f, tanf(fData) * 5.0f, cosf(fData) * 5.0f), 10.0f, 10.0f, BULLETTYPE_PLAYER);
			g_BulletRecast=0;
		}
	}

	if (g_BulletRecast >= 150 && Mode == MODE_SINGLEGAME)
	{
		if (GetKeyboardTrigger(DIK_SPACE) == true)
		{
			fData = atan2f(pCPU->pos.x - g_player.pos.x, pCPU->pos.z - g_player.pos.z);
			SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + 5, g_player.pos.z), D3DXVECTOR3(sinf(fData) * 5.0f, tanf(fData) * 5.0f, cosf(fData) * 5.0f), 10.0f, 10.0f, BULLETTYPE_PLAYER);
			g_BulletRecast = 0;
		}
	}


	if (g_player.pos.x  < 30)
	{
		g_player.pos.x = 80;
		CutLife(1);
		PlaySound(SOUND_LABEL_SE_HIT000);
		g_player.nLife--;
	}
	if (g_player.pos.x > 500)
	{
		g_player.pos.x =  450;
		CutLife(1);
		PlaySound(SOUND_LABEL_SE_HIT000);
		g_player.nLife--;
	}
	if (g_player.pos.z  > 50)
	{
		g_player.pos.z = 0;
		CutLife(1);
		PlaySound(SOUND_LABEL_SE_HIT000);
		g_player.nLife--;
	}
	if (g_player.pos.z < -340)
	{
		g_player.pos.z = -260;
		CutLife(1);
		PlaySound(SOUND_LABEL_SE_HIT000);
		g_player.nLife--;
	}

	/*if (g_player.nLife == 0)
	{
		if (GameState != GAMESTATE_P2WIN)
		{
			SetFade(MODE_P2WIN);
		}
	}*/

	if (GetKeyboardPress(DIK_LSHIFT) == false)
	{
		SetParticle(g_player.pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), 10.0f, 100);
		//斜め移動の処理
		if (GetKeyboardPress(DIK_A) == true)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{//左上移動
				g_player.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.75f) * NORMAL_SPEED;
				g_player.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.75f) * NORMAL_SPEED;
				//g_player.rot.y = (pCamera->rot.y + D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//左下移動
				g_player.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.25f) * NORMAL_SPEED;
				g_player.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.25f) * NORMAL_SPEED;
				//g_player.rot.y = (pCamera->rot.y + D3DX_PI * 0.25f);
			}
			else
			{//左移動
				g_player.pos.x += sinf(pCamera->rot.y - D3DX_PI * 0.5f) * NORMAL_SPEED;
				g_player.pos.z += cosf(pCamera->rot.y - D3DX_PI * 0.5f) * NORMAL_SPEED;
				//g_player.rot.y = (pCamera->rot.y + D3DX_PI * 0.5f);
			}
		}

		//斜め移動の処理
		else if (GetKeyboardPress(DIK_D) == true)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{//右上移動
				g_player.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.75f) * NORMAL_SPEED;
				g_player.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.75f) * NORMAL_SPEED;
				//g_player.rot.y = (pCamera->rot.y - D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{//右下移動
				g_player.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.25f) * NORMAL_SPEED;
				g_player.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.25f) * NORMAL_SPEED;
				//g_player.rot.y = (pCamera->rot.y - D3DX_PI * 0.25f);
			}
			else
			{//右移動
				g_player.pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * NORMAL_SPEED;
				g_player.pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * NORMAL_SPEED;
				//g_player.rot.y = (pCamera->rot.y - D3DX_PI * 0.5f);
			}
		}

		else if (GetKeyboardPress(DIK_W) == true)
		{
			g_player.pos.x += sinf(pCamera->rot.y) * NORMAL_SPEED;
			g_player.pos.z += cosf(pCamera->rot.y) * NORMAL_SPEED;
			//g_player.rot.y = (pCamera->rot.y + D3DX_PI * 1.0f);
		}
		
		else if (GetKeyboardPress(DIK_S) == true)
		{
			g_player.pos.x -= sinf(pCamera->rot.y) * NORMAL_SPEED;
			g_player.pos.z -= cosf(pCamera->rot.y) * NORMAL_SPEED;
			//g_player.rot.y = (pCamera->rot.y + D3DX_PI * 0.0f);
		}

	}
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
		//SetParticle(g_player.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20.0f, 100);
		////斜め移動の処理
		//if (GetKeyboardPress(DIK_A) == true)
		//{
		//	if (GetKeyboardPress(DIK_W) == true)
		//	{//左上移動
		//		g_player.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.75f) * DASH_SPEED;
		//		g_player.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.75f) * DASH_SPEED;
		//		//g_player.rot.y = (pCamera->rot.y + D3DX_PI * 0.75f);
		//	}
		//	else if (GetKeyboardPress(DIK_S) == true)
		//	{//左下移動
		//		g_player.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.25f) * DASH_SPEED;
		//		g_player.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.25f) * DASH_SPEED;
		//		//g_player.rot.y = (pCamera->rot.y + D3DX_PI * 0.25f);
		//	}
		//	else
		//	{//左移動
		//		g_player.pos.x += sinf(pCamera->rot.y - D3DX_PI * 0.5f) * DASH_SPEED;
		//		g_player.pos.z += cosf(pCamera->rot.y - D3DX_PI * 0.5f) * DASH_SPEED;
		//		//g_player.rot.y = (pCamera->rot.y + D3DX_PI * 0.5f);

		//	}
		//}

		////斜め移動の処理
		//else if (GetKeyboardPress(DIK_D) == true)
		//{
		//	if (GetKeyboardPress(DIK_W) == true)
		//	{//右上移動
		//		g_player.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.75f) * DASH_SPEED;
		//		g_player.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.75f) * DASH_SPEED;
		//		//g_player.rot.y = (pCamera->rot.y - D3DX_PI * 0.75f);
		//	}
		//	else if (GetKeyboardPress(DIK_S) == true)
		//	{//右下移動
		//		g_player.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.25f) * DASH_SPEED;
		//		g_player.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.25f) * DASH_SPEED;
		//		//g_player.rot.y = (pCamera->rot.y - D3DX_PI * 0.25f);
		//	}
		//	else
		//	{//右移動
		//		g_player.pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * DASH_SPEED;
		//		g_player.pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * DASH_SPEED;
		//		//g_player.rot.y = (pCamera->rot.y - D3DX_PI * 0.5f);
		//	}
		//}
		//else if (GetKeyboardPress(DIK_W) == true)
		//{
		//	g_player.pos.x += sinf(pCamera->rot.y) * DASH_SPEED;
		//	g_player.pos.z += cosf(pCamera->rot.y) * DASH_SPEED;
		//	//g_player.rot.y = (pCamera->rot.y + D3DX_PI * 1.0f);
		//}
		//else if (GetKeyboardPress(DIK_S) == true)
		//{
		//	g_player.pos.x -= sinf(pCamera->rot.y) * DASH_SPEED;
		//	g_player.pos.z -= cosf(pCamera->rot.y) * DASH_SPEED;
		//	//g_player.rot.y = (pCamera->rot.y + D3DX_PI * 0.0f);
		//}
	}

		/*if (GetKeyboardTrigger(DIK_F) == true)
		{
			g_player.move.y -= cosf(-D3DX_PI) * 2;
			AddScore(100);
		}*/

	g_player.move.y -= 0.1f;

	if (g_player.pos.y < 1.0f)
	{
	SetExplosion(g_player.pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	g_player.pos.y = 1;
	g_player.move.y = 0.0f;
	}

	g_player.pos.x += g_player.move.x;
	g_player.pos.z += g_player.move.z;
	g_player.pos.y += g_player.move.y;

	if (CollisionBuilding(&g_player.pos, &g_player.posOld, &g_player.move, g_player.VtxMax, g_player.VtxMin) == true)
	{
	}

	if (CollisionPlayer2(&g_player.pos, &g_player.posOld, &g_player.move, g_player.VtxMax, g_player.VtxMin) == true)
	{
	}

	if (CollisionCPU(&g_player.pos, &g_player.posOld, &g_player.move, g_player.VtxMax, g_player.VtxMin) == true)
	{
	}

	g_player.move.x += (0.0f - g_player.move.x) * 0.15f;
	g_player.move.z += (0.0f - g_player.move.z) * 0.15f;

	SetPositionShadow(g_nIdxShadow, D3DXVECTOR3(g_player.pos.x, 0.1f, g_player.pos.z));
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;
	D3DXMATRIX mtxParent;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);

	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);
	
	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);

	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (g_player.aModel[nCnt].nIdxModelParent == -1)
		{
			mtxParent = g_player.mtxWorld;
		}
		else
		{
			mtxParent = g_player.aModel[g_player.aModel[nCnt].nIdxModelParent].mtxWorld;
		}
		//=============================================================================
		// 親のモデル
		//=============================================================================
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.aModel[nCnt].mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.aModel[nCnt].rot.y, g_player.aModel[nCnt].rot.x, g_player.aModel[nCnt].rot.z);

		D3DXMatrixMultiply(&g_player.aModel[nCnt].mtxWorld, &g_player.aModel[nCnt].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_player.aModel[nCnt].pos.x, g_player.aModel[nCnt].pos.y, g_player.aModel[nCnt].pos.z);

		D3DXMatrixMultiply(&g_player.aModel[nCnt].mtxWorld, &g_player.aModel[nCnt].mtxWorld, &mtxTrans);

		D3DXMatrixMultiply(&g_player.aModel[nCnt].mtxWorld, &g_player.aModel[nCnt].mtxWorld, &mtxParent);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCnt].mtxWorld);


		// 現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aModel[nCnt].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCnt].nNumMat; nCntMat++)
		{
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの読み込み
			pDevice->SetTexture(0, g_pTexturePlayer);

			// モデル(パーツ)の描画
			g_player.aModel[nCnt].pMesh->DrawSubset(nCntMat);
		}
		// マテリアルをデフォルトに戻す
		pDevice->SetMaterial(&matDef);
	}
	//=============================================================================
	// 子のモデル
	//=============================================================================
}

PLAYER *GetPlayer(void)
{
	return &g_player;
}

//*****************************************************************************
//モデルとの当たり判定処理
//*****************************************************************************
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pModel*/)
{
	bool bLand = false;

	GAMESTATE GameState = GetGameState();

	PLAYER2 *pPlayer2 = GetPlayer2();

	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		if (g_player.bUse == true)
		{
			//モデルのY幅の中
			if (pPos->y < g_player.pos.y + g_player.VtxMax.y
				&& pPos->y > g_player.pos.y + g_player.VtxMin.y)
			{
				//モデルのZ幅の中
				if (pPos->z + VtxMin.z < g_player.pos.z + g_player.VtxMax.z
					&& pPos->z + VtxMax.z > g_player.pos.z + g_player.VtxMin.z)
				{
					//モデルのX幅の中
					if (pPos->x + VtxMin.x < g_player.pos.x + g_player.VtxMax.x
						&& pPos->x + VtxMax.x > g_player.pos.x + g_player.VtxMin.x)
					{

						//X幅の当たり判定
						if (pPosOld->x > g_player.pos.x + g_player.VtxMax.x)
						{
							g_player.move.x += pMove->x - 15;

							pPos->x = g_player.pos.x + g_player.VtxMax.x + VtxMax.x;
							pPosOld->x = g_player.pos.x + g_player.VtxMax.x + VtxMax.x;
							pMove->x = 0;
							bLand = true;
						}
						else if (pPosOld->x < g_player.pos.x + g_player.VtxMin.x)
						{
							g_player.move.x += pMove->x+15;

							pPos->x = g_player.pos.x + g_player.VtxMin.x + VtxMin.x;
							pPosOld->x = g_player.pos.x + g_player.VtxMin.x + VtxMin.x;
							pMove->x = 0;
							bLand = true;

						}
						//Z幅の当たり判定
						else if (pPosOld->z > g_player.pos.z + g_player.VtxMax.z)
						{
							g_player.move.z += pMove->z-15;

						    pPos->z = g_player.pos.z + g_player.VtxMax.z + VtxMax.z;
							pPosOld->z = g_player.pos.z + g_player.VtxMax.z + VtxMax.z;
							//pMove->z = 0;
							bLand = true;
						}
						else if (pPosOld->z < g_player.pos.z + g_player.VtxMin.z)
						{
							g_player.move.z += pMove->z+15;

							pPos->z = g_player.pos.z + g_player.VtxMin.z + VtxMin.z;
							pPosOld->z = g_player.pos.z + g_player.VtxMin.z + VtxMin.z;
							//pMove->z = 0;

							bLand = true;
						}
						//Y幅の当たり判定
						if (pPosOld->y >= g_player.pos.y + g_player.VtxMax.y)
						{
							pPos->y = g_player.pos.y + g_player.VtxMax.y;
							pPosOld->y = g_player.pos.y + g_player.VtxMax.y;
							pMove->y = 0;
						}
						else if (pPosOld->y <= g_player.pos.y + g_player.VtxMin.y)
						{
							pPos->y = g_player.pos.y + g_player.VtxMin.y;
							pPosOld->y = g_player.pos.y + g_player.VtxMin.y;
						}
						bLand = true;
					}
				}
			}
		}
	}
	return bLand;
}

//=============================================================================
// Hit時の処理
//=============================================================================
void HitPlayer(int nDmage)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	g_player.nLife -= nDmage;
	if (g_player.nLife <= 0)
	{//ライフが０以下になったとき消滅
		//SetExplosion(g_player.pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		g_player.bDisp = true;
		g_player.state = PLAYERSTATE_DEATH;
	}
}