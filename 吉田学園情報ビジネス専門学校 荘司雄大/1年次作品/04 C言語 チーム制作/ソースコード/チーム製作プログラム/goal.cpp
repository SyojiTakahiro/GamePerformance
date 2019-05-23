//=============================================================================
//
// ゴール処理 [goal.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "goal.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "polygon.h"
#include "player.h"
#include "game.h"
#include "explosion.h"
#include "sound.h"
#include "particle.h"
#include "fade.h"
//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH g_pMeshGoal[MAX_GOAL];					//メッシュ情報へのポインタ
LPD3DXBUFFER g_pBuffMatGoal[MAX_GOAL];			//マテリアル情報へのポインタ
LPDIRECT3DTEXTURE9	g_pTextureGoal[MAX_GOAL];		//テクスチャへのポインタ
DWORD g_nNumMatGoal[MAX_GOAL] = {};							//マテリアル情報の数
GOAL g_aGoal[MAX_GOAL];
D3DXMATERIAL *g_pMatGoal;
//=============================================================================
// 初期化処理
//=============================================================================
void InitGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(GOAL_NAME000,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatGoal[0],
		NULL,
		&g_nNumMatGoal[0],
		&g_pMeshGoal[0]);



	int nNumVtex;	//頂点数
	DWORD sizeFvF;	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_GOAL];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_GOAL];

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		g_aGoal[nCntGoal].VtxMinGoal = D3DXVECTOR3(10000, 0, 10000);
		g_aGoal[nCntGoal].VtxMaxGoal = D3DXVECTOR3(-10000, 0, -10000);
		g_aGoal[nCntGoal].nType = 0;
		g_aGoal[nCntGoal].bUse = false;
		g_aGoal[nCntGoal].nCntGoal = 0;
		g_aGoal[nCntGoal].nGoalPattern = 0;
		g_aGoal[nCntGoal].nCounterState = 60;
		g_aGoal[nCntGoal].state = GOALSTATE_NORMAL;;
	}

	//頂点数を取得
	nNumVtex = g_pMeshGoal[0]->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFvF = D3DXGetFVFVertexSize(g_pMeshGoal[0]->GetFVF());

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		//頂点　ロック
		g_pMeshGoal[0]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nCntGoal]);
		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
		{	//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff[nCntGoal];
			if (g_aGoal[nCntGoal].VtxMinGoal.x > vtx.x)
			{
				g_aGoal[nCntGoal].VtxMinGoal.x = vtx.x;
			}
			if (g_aGoal[nCntGoal].VtxMinGoal.y > vtx.y)
			{
				g_aGoal[nCntGoal].VtxMinGoal.y = vtx.y;
			}
			if (g_aGoal[nCntGoal].VtxMinGoal.z > vtx.z)
			{
				g_aGoal[nCntGoal].VtxMinGoal.z = vtx.z;
			}

			if (g_aGoal[nCntGoal].VtxMaxGoal.x < vtx.x)
			{
				g_aGoal[nCntGoal].VtxMaxGoal.x = vtx.x;
			}
			if (g_aGoal[nCntGoal].VtxMaxGoal.y < vtx.y)
			{
				g_aGoal[nCntGoal].VtxMaxGoal.y = vtx.y;
			}
			if (g_aGoal[nCntGoal].VtxMaxGoal.z < vtx.z)
			{
				g_aGoal[nCntGoal].VtxMaxGoal.z = vtx.z;
			}
			//初期処理

			pVtxBuff[nCntGoal] += sizeFvF;
		}
		//			//頂点バッファをアンロック
		g_pMeshGoal[0]->UnlockVertexBuffer();
	}
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		GOAL_NAME000,
		&g_pTextureGoal[0]);

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		for (int nCntMat = 0; nCntMat < (int)g_nNumMatGoal[nCntGoal]; nCntMat++)
		{
			// マテリアルデータへのポインタを取得
			g_pMatGoal = (D3DXMATERIAL*)g_pBuffMatGoal[g_aGoal[nCntGoal].nType]->GetBufferPointer();
			{
				D3DXCreateTextureFromFile(pDevice,
					g_pMatGoal[nCntMat].pTextureFilename,
					&g_pTextureGoal[g_aGoal[nCntGoal].nType]);
			}
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitGoal(void)
{
	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		// メッシュの開放
		if (g_pMeshGoal[nCntGoal] != NULL)
		{
			g_pMeshGoal[nCntGoal]->Release();
			g_pMeshGoal[nCntGoal] = NULL;
		}
		// マテリアルの開放
		if (g_pBuffMatGoal[nCntGoal] != NULL)
		{
			g_pBuffMatGoal[nCntGoal]->Release();
			g_pBuffMatGoal[nCntGoal] = NULL;
		}
		//テクスチャの破棄
		if (g_pTextureGoal[nCntGoal] != NULL)
		{
			g_pTextureGoal[nCntGoal]->Release();
			g_pTextureGoal[nCntGoal] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateGoal(void)
{
	Camera *pCamera;
	pCamera = GetCamera();
	//カメラとの距離を求める
	double length = pow((g_aGoal[0].pos.x - pCamera->posV.x)*(g_aGoal[0].pos.x - pCamera->posV.x)
		+ (g_aGoal[0].pos.z - pCamera->posV.z)*(g_aGoal[0].pos.z - pCamera->posV.z), 0.5);

	//float型に変換
	g_aGoal[0].fLength = (float)length;

	if(g_aGoal[0].nGoalPattern == 0)
	{
		g_aGoal[0].pos = D3DXVECTOR3(8230, 0, -500);
	}
	else if (g_aGoal[0].nGoalPattern == 1)
	{
		g_aGoal[0].pos = D3DXVECTOR3(2400, 0, 7200);
	}
	else if (g_aGoal[0].nGoalPattern == 2)
	{
		g_aGoal[0].pos = D3DXVECTOR3(6300, 0, 5560);
	}
	else if (g_aGoal[0].nGoalPattern == 3)
	{
		g_aGoal[0].pos = D3DXVECTOR3(550, 0, -480);
	}
	
	if (g_aGoal[0].state == GOALSTATE_CHECKPOINT)
	{
		g_aGoal[0].nCounterState++;
		if(g_aGoal[0].nCounterState > 60)
		{
			g_aGoal[0].nCounterState = 0;
			g_aGoal[0].state = GOALSTATE_NORMAL;
		}
	}

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawGoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{		// ワールドマトリックスの初期化
		if (g_aGoal[0].fLength < 2200)
		{//ゴールとプレイヤーの距離で描画するか決める
			D3DXMatrixIdentity(&g_aGoal[nCntGoal].mtxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aGoal[nCntGoal].rot.y, g_aGoal[nCntGoal].rot.x, g_aGoal[nCntGoal].rot.z);
			D3DXMatrixMultiply(&g_aGoal[nCntGoal].mtxWorld, &g_aGoal[nCntGoal].mtxWorld, &mtxRot);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aGoal[nCntGoal].pos.x, g_aGoal[nCntGoal].pos.y, g_aGoal[nCntGoal].pos.z);
			D3DXMatrixMultiply(&g_aGoal[nCntGoal].mtxWorld, &g_aGoal[nCntGoal].mtxWorld, &mtxTrans);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aGoal[nCntGoal].mtxWorld);
			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);


			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_pBuffMatGoal[g_aGoal[nCntGoal].nType]->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_nNumMatGoal[nCntGoal]; nCntMat++)
			{	// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				// テクスチャ
				pDevice->SetTexture(0, g_pTextureGoal[g_aGoal[nCntGoal].nType]);
				if (g_aGoal[nCntGoal].bUse == true)	//ゴールが使用されている
				{	// ゴール(パーツ)の描画
					g_pMeshGoal[g_aGoal[nCntGoal].nType]->DrawSubset(nCntMat);
				}
			}
			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//*****************************************************************************
//ゴールポインタ処理
//*****************************************************************************
GOAL *GetGoal(void)
{
	return &g_aGoal[0];
}
//*****************************************************************************
//ゴールとの当たり判定処理
//*****************************************************************************
bool CollisionGoal(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, GOAL **pGoal*/)
{
	bool bLand = false;

	GAMESTATE GameState;
	GameState = GetGameState();
	FADE fade = GetFade();

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		if (g_aGoal[nCntGoal].bUse == true)
		{
#if 1
			//ゴールのY幅の中
			if (pPos->y < g_aGoal[nCntGoal].pos.y + g_aGoal[nCntGoal].VtxMaxGoal.y
				&& pPos->y > g_aGoal[nCntGoal].pos.y + g_aGoal[nCntGoal].VtxMinGoal.y)
			{
				//ゴールのZ幅の中
				if (pPos->z + VtxMin.z < g_aGoal[nCntGoal].pos.z + g_aGoal[nCntGoal].VtxMaxGoal.z
					&& pPos->z + VtxMax.z > g_aGoal[nCntGoal].pos.z + g_aGoal[nCntGoal].VtxMinGoal.z)
				{
					//ゴールのX幅の中
					if (pPos->x + VtxMin.x < g_aGoal[nCntGoal].pos.x + g_aGoal[nCntGoal].VtxMaxGoal.x
						&& pPos->x + VtxMax.x > g_aGoal[nCntGoal].pos.x + g_aGoal[nCntGoal].VtxMinGoal.x)
					{

						//X幅の当たり判定
						if (pPosOld->x + 1 > g_aGoal[nCntGoal].pos.x + g_aGoal[nCntGoal].VtxMaxGoal.x)
						{
							//if (g_aGoal[0].nGoalPattern == 0)
							//{
							//	g_aGoal[0].nGoalPattern = 1;
							//}
							//if (g_aGoal[0].nGoalPattern == 1)
							//{
							//	g_aGoal[0].nGoalPattern = 2;
							//}
							if (GameState != GAMESTATE_GAMECLEAR && g_aGoal[0].state == GOALSTATE_NORMAL || fade == FADE_NONE && g_aGoal[0].state == GOALSTATE_NORMAL)
							{
								if(GameState != GAMESTATE_GAMECLEAR && g_aGoal[0].nGoalPattern == 3)
								{
									SetGameState(GAMESTATE_GAMECLEAR);
									PlaySound(SOUND_LABEL_SE_CHIME1);
								}
								g_aGoal[0].state = GOALSTATE_CHECKPOINT;
								PlaySound(SOUND_LABEL_SE_CHIME1);
								SetParticle(D3DXVECTOR3(g_aGoal[nCntGoal].pos.x, g_aGoal[nCntGoal].pos.y + 50, g_aGoal[nCntGoal].pos.z)
									, D3DXVECTOR3(0.8f, 0.2f, 0.8f), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(0, 0, 0, 0), 20, 30, PARTICLE_TYPE_GOAL);
							}
							bLand = true;
						}
						else if (pPosOld->x - 1 < g_aGoal[nCntGoal].pos.x + g_aGoal[nCntGoal].VtxMinGoal.x)
						{
							if (GameState != GAMESTATE_GAMECLEAR && g_aGoal[0].state == GOALSTATE_NORMAL || fade == FADE_NONE && g_aGoal[0].state == GOALSTATE_NORMAL)
							{
								if (GameState != GAMESTATE_GAMECLEAR && g_aGoal[0].nGoalPattern == 3)
								{
									SetGameState(GAMESTATE_GAMECLEAR);
									PlaySound(SOUND_LABEL_SE_CHIME1);
								}
								g_aGoal[0].state = GOALSTATE_CHECKPOINT;
								PlaySound(SOUND_LABEL_SE_CHIME1);
								SetParticle(D3DXVECTOR3(g_aGoal[nCntGoal].pos.x, g_aGoal[nCntGoal].pos.y + 50, g_aGoal[nCntGoal].pos.z)
									, D3DXVECTOR3(0.8f, 0.2f, 0.8f), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(0, 0, 0, 0), 20, 30, PARTICLE_TYPE_GOAL);
							}
							bLand = true;
						}
						//Z幅の当たり判定
						if (pPosOld->z + 1 > g_aGoal[nCntGoal].pos.z + g_aGoal[nCntGoal].VtxMaxGoal.z)
						{
							if (GameState != GAMESTATE_GAMECLEAR && g_aGoal[0].state == GOALSTATE_NORMAL || fade == FADE_NONE && g_aGoal[0].state == GOALSTATE_NORMAL)
							{
								if (GameState != GAMESTATE_GAMECLEAR && g_aGoal[0].nGoalPattern == 3)
								{
									SetGameState(GAMESTATE_GAMECLEAR);
									PlaySound(SOUND_LABEL_SE_CHIME1);
								}
								g_aGoal[0].state = GOALSTATE_CHECKPOINT;
								PlaySound(SOUND_LABEL_SE_CHIME1);
								SetParticle(D3DXVECTOR3(g_aGoal[nCntGoal].pos.x, g_aGoal[nCntGoal].pos.y + 50, g_aGoal[nCntGoal].pos.z)
									, D3DXVECTOR3(0.8f, 0.2f, 0.8f), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(0, 0, 0, 0), 20, 30, PARTICLE_TYPE_GOAL);

							}
							bLand = true;
						}
						else if (pPosOld->z - 1 < g_aGoal[nCntGoal].pos.z + g_aGoal[nCntGoal].VtxMinGoal.z)
						{
							if (GameState != GAMESTATE_GAMECLEAR && g_aGoal[0].state == GOALSTATE_NORMAL || fade == FADE_NONE && g_aGoal[0].state == GOALSTATE_NORMAL)
							{
								if (GameState != GAMESTATE_GAMECLEAR && g_aGoal[0].nGoalPattern == 3)
								{
									SetGameState(GAMESTATE_GAMECLEAR);
									PlaySound(SOUND_LABEL_SE_CHIME1);
								}
								g_aGoal[0].state = GOALSTATE_CHECKPOINT;
								PlaySound(SOUND_LABEL_SE_CHIME1);
								SetParticle(D3DXVECTOR3(g_aGoal[nCntGoal].pos.x, g_aGoal[nCntGoal].pos.y + 50, g_aGoal[nCntGoal].pos.z)
									, D3DXVECTOR3(0.8f, 0.2f, 0.8f), D3DXVECTOR3(0, 0, 0), D3DXCOLOR(0, 0, 0, 0), 20, 30, PARTICLE_TYPE_GOAL);
							}
							bLand = true;
						}
						//Y幅の当たり判定
						if (pPosOld->y >= g_aGoal[nCntGoal].pos.y + g_aGoal[nCntGoal].VtxMaxGoal.y)
						{
						}
						else if (pPosOld->y <= g_aGoal[nCntGoal].pos.y + g_aGoal[nCntGoal].VtxMinGoal.y)
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
void SetGoal(D3DXVECTOR3 pos, int nType)
{

	int nNumVtex;	//頂点数
	DWORD sizeFvF[MAX_GOAL];	//頂点フォーマットのサイズ
	BYTE *pVtxBuff[MAX_GOAL];	//頂点バッファへのポインタ
	D3DXVECTOR3 vtx[MAX_GOAL];

	//頂点数を取得
	nNumVtex = g_pMeshGoal[nType]->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFvF[nType] = D3DXGetFVFVertexSize(g_pMeshGoal[nType]->GetFVF());

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		//if (g_aGoal[nCntGoal].bUse == false)
		//{
			//頂点　ロック
			g_pMeshGoal[nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nType]);
			for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
			{	//頂点座標の代入
				vtx[nCntGoal] = *(D3DXVECTOR3*)pVtxBuff[nType];
				if (g_aGoal[nCntGoal].VtxMinGoal.x > vtx[nCntGoal].x)
				{
					g_aGoal[nCntGoal].VtxMinGoal.x = vtx[nCntGoal].x;
				}
				if (g_aGoal[nCntGoal].VtxMinGoal.y > vtx[nCntGoal].y)
				{
					g_aGoal[nCntGoal].VtxMinGoal.y = vtx[nCntGoal].y;
				}
				if (g_aGoal[nCntGoal].VtxMinGoal.z > vtx[nCntGoal].z)
				{
					g_aGoal[nCntGoal].VtxMinGoal.z = vtx[nCntGoal].z;
				}

				if (g_aGoal[nCntGoal].VtxMaxGoal.x < vtx[nCntGoal].x)
				{
					g_aGoal[nCntGoal].VtxMaxGoal.x = vtx[nCntGoal].x;
				}
				if (g_aGoal[nCntGoal].VtxMaxGoal.y < vtx[nCntGoal].y)
				{
					g_aGoal[nCntGoal].VtxMaxGoal.y = vtx[nCntGoal].y;
				}
				if (g_aGoal[nCntGoal].VtxMaxGoal.z < vtx[nCntGoal].z)
				{
					g_aGoal[nCntGoal].VtxMaxGoal.z = vtx[nCntGoal].z;
				}
				pVtxBuff[nCntGoal] += sizeFvF[nType];
			}
			//頂点バッファをアンロック
			g_pMeshGoal[nType]->UnlockVertexBuffer();
		//	break;
		//}
	}

	for (int nCntGoal = 0; nCntGoal < MAX_GOAL; nCntGoal++)
	{
		if (g_aGoal[nCntGoal].bUse == false)
		{
			// 位置・向きの初期設定
			g_aGoal[nCntGoal].pos = pos;
			g_aGoal[nCntGoal].rot = D3DXVECTOR3(0, 0, 0);
			g_aGoal[nCntGoal].fShadow = 12;
			g_aGoal[nCntGoal].nType = nType;
			//　影のセット
			//g_aGoal[nCntGoal].nIdxShadow = SetShadow(g_aGoal[nCntGoal].pos, D3DXVECTOR3(0, 0, 0), 20.0f);
			g_aGoal[nCntGoal].bUse = true;
			break;
		}
	}

}
