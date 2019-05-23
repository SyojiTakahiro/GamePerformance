//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : 荘司　雄大
//
//=============================================================================
#include "bullet.h"
#include "shadow.h"
#include "explosion.h"
//#include "enemy.h"
#include "player.h"
#include "player2.h"
#include "cpu.h"
#include "particle.h"
#include "building.h"
//#include "effect.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BULLET			(256)					//弾の最大数
#define BULLET_WIDTH		(10)					//弾の幅
#define BULLET_HEIGHT		(10)					//弾の高さ
#define BULLET_TEXTURENAME	"data/TEXTURE/bullet000.png"	//テクスチャ

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// 頂点バッファへのポインタ
D3DXCOLOR g_color[5];
float g_iSpin = 1.01f;
Bullet g_aBullet[MAX_BULLET];						//弾の情報

D3DXVECTOR3 aPos[4];
D3DXVECTOR3 VecA;
D3DXVECTOR3 VecB;
D3DXVECTOR3 VecC;

//int g_nIdxShadow;
//=============================================================================
// 初期化処理
//=============================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_3D *pVtx;//頂点情報へのポインタ
	int nCntBullet;

	// デバイスの取得
	pDevice = GetDevice();

	// 弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 100;
		g_aBullet[nCntBullet].VtxMax = D3DXVECTOR3(g_aBullet[nCntBullet].fWidth, 0, 10);
		g_aBullet[nCntBullet].VtxMin = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, 0, -10);
		g_aBullet[nCntBullet].bUse = false;
	}


	/*g_color[0] = D3DXCOLOR(1.0f, 0.0f, 0.0f,1.0f);
	g_color[1] = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	g_color[2] = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	g_color[3] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	g_color[4] = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);*/
	//g_color[5] = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURENAME, &g_pTextureBullet);

	// 頂点情報の作成
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,	//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-1, 1, 0);
		pVtx[1].pos = D3DXVECTOR3(1, 1, 0);
		pVtx[2].pos = D3DXVECTOR3(-1, -1, 0);
		pVtx[3].pos = D3DXVECTOR3(1, -1, 0);

		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0, 0);
		pVtx[1].tex = D3DXVECTOR2(1, 0);
		pVtx[2].tex = D3DXVECTOR2(0, 1);
		pVtx[3].tex = D3DXVECTOR2(1, 1);

		pVtx += 4;//頂点データのポインタを4つ進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
	//30 500 50 -340
	aPos[0] = D3DXVECTOR3(30, 0, 50);
	aPos[1] = D3DXVECTOR3(500, 0, 50);
	aPos[2] = D3DXVECTOR3(500, 0, -340);
	aPos[3] = D3DXVECTOR3(50, 0, -340);

	//g_nIdxShadow = SetShadow(g_posBullet, g_moveBullet);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	// テクスチャの開放
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	int nCntBullet;

	PLAYER *pPlayer;
	//プレイヤーの取得
	pPlayer = GetPlayer();

	PLAYER2 *pPlayer2;
	//プレイヤー2の取得
	pPlayer2 = GetPlayer2();

	CPU *pCPU;
	//CPUの取得
	pCPU = GetCPU();


	VERTEX_3D *pVtx;//頂点情報へのポインタ
	
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)//弾が使用される
		{
			g_aBullet[nCntBullet].nLife--;
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;//弾の位置を移動させる
			g_aBullet[nCntBullet].pos.z += g_aBullet[nCntBullet].move.z;//弾の位置を移動させる

			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				g_aBullet[nCntBullet].bUse = false;
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.2f, 0.0f,1.0f));
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-1, 1, 0);
			pVtx[1].pos = D3DXVECTOR3(1, 1, 0);
			pVtx[2].pos = D3DXVECTOR3(-1, -1, 0);
			pVtx[3].pos = D3DXVECTOR3(1, -1, 0);

			//=============================================================================
			// プレイヤー１の弾
			//=============================================================================

			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{
				if (pPlayer2->bUse == true)
				{
					if (CollisionPlayer2(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, g_aBullet[nCntBullet].VtxMax, g_aBullet[nCntBullet].VtxMin) == true)
					{
						g_aBullet[nCntBullet].bUse = false;
						SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
					}
				}
				if (pCPU->bUse == true)
				{
					if (CollisionCPU(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, g_aBullet[nCntBullet].VtxMax, g_aBullet[nCntBullet].VtxMin) == true)
					{
						g_aBullet[nCntBullet].bUse = false;
						SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
					}
				}
				SetParticle(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 10.0f, 10);
				SetParticle(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5.0f, 10);
			}

			//=============================================================================
			// プレイヤー２の弾
			//=============================================================================
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER2)
			{
				if (pPlayer->bUse == true)
				{
						if (CollisionPlayer(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, g_aBullet[nCntBullet].VtxMax, g_aBullet[nCntBullet].VtxMin) == true)
						{
							g_aBullet[nCntBullet].bUse = false;
							SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
						}
				}
				SetParticle(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 10.0f, 10);
				SetParticle(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5.0f, 10);
			}


			//=============================================================================
			// CPUの弾
			//=============================================================================
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_CPU)
			{
				if (pPlayer->bUse == true)
				{
					if (CollisionPlayer(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, g_aBullet[nCntBullet].VtxMax, g_aBullet[nCntBullet].VtxMin) == true)
					{
						g_aBullet[nCntBullet].bUse = false;
						SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
					}
				}
				SetParticle(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 10.0f, 10);
				SetParticle(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 5.0f, 10);
			}

			float fGaiseki[4];

			VecA = aPos[1] - aPos[0];
			VecC = g_aBullet[nCntBullet].pos - aPos[0];

			fGaiseki[0] = ((VecA.z * VecC.x) - (VecA.x * VecC.z));

			/*if (fGaiseki[0] < 1)
			{
				g_aBullet[nCntBullet].bUse = false;
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
			}

			VecA = aPos[2] - aPos[1];
			VecC = g_aBullet[nCntBullet].pos - aPos[1];

			fGaiseki[1] = ((VecA.z * VecC.x) - (VecA.x * VecC.z));

			if (fGaiseki[1] < 1)
			{
			g_aBullet[nCntBullet].bUse = false;
			SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
			}

			VecA = aPos[3] - aPos[2];
			VecC = g_aBullet[nCntBullet].pos - aPos[2];

			fGaiseki[2] = ((VecA.z * VecC.x) - (VecA.x * VecC.z));

			if (fGaiseki[2] < 1)
			{
				g_aBullet[nCntBullet].bUse = false;
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
			}

			VecA = aPos[4] - aPos[3];
			VecC = g_aBullet[nCntBullet].pos - aPos[3];

			fGaiseki[3] = ((VecA.z * VecC.x) - (VecA.x * VecC.z));

			if (fGaiseki[3] < 1)
			{
				g_aBullet[nCntBullet].bUse = false;
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
			}*/

			/*if (CollisionPlayer(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, g_aBullet[nCntBullet].VtxMax, g_aBullet[nCntBullet].VtxMin) == true)
			{
				g_aBullet[nCntBullet].bUse = false;
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
			}*/

			if (CollisionBuilding(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld, &g_aBullet[nCntBullet].move, g_aBullet[nCntBullet].VtxMax, g_aBullet[nCntBullet].VtxMin) == true)
			{
				g_aBullet[nCntBullet].bUse = false;
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
			}
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();

	/*SetPositionShadow(g_nIdxShadow, D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0, g_aBullet[nCntBullet].pos.z));*/
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans, mtxView;
	int nCntBullet;

	pDevice = GetDevice();	// デバイスの取得

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		g_aBullet[nCntBullet].mtxWorld._11 = mtxView._11;
		g_aBullet[nCntBullet].mtxWorld._12 = mtxView._21;
		g_aBullet[nCntBullet].mtxWorld._13 = mtxView._31;
		g_aBullet[nCntBullet].mtxWorld._21 = mtxView._12;
		g_aBullet[nCntBullet].mtxWorld._22 = mtxView._22;
		g_aBullet[nCntBullet].mtxWorld._23 = mtxView._32;
		g_aBullet[nCntBullet].mtxWorld._31 = mtxView._13;
		g_aBullet[nCntBullet].mtxWorld._32 = mtxView._23;
		g_aBullet[nCntBullet].mtxWorld._33 = mtxView._33;


		//// 向きを反映
		//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBullet[nCntBullet].rot.y, g_aBullet[nCntBullet].rot.x, g_aBullet[nCntBullet].rot.z);

		//D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);

		D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBullet);

		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// ポリゴンの描画
			if (g_aBullet[nCntBullet].bUse == true)
			{
				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
					nCntBullet * 4,//描画を開始する頂点インデックス
					2);
			}
	}
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}


//=============================================================================
// 弾の設定処理
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, BULLETTYPE type)
{
	int nCntBullet;
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].fWidth = fWidth;
			g_aBullet[nCntBullet].fHeight = fHeight;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-1, 1, 0);
			pVtx[1].pos = D3DXVECTOR3(1, 1, 0);
			pVtx[2].pos = D3DXVECTOR3(-1, -1, 0);
			pVtx[3].pos = D3DXVECTOR3(1, -1, 0);

			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_aBullet[nCntBullet].nLife = 100;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].move = move;
			//g_aBullet[nCntBullet].color = g_color[rand() % 5];
			g_aBullet[nCntBullet].bUse = true;//使用している状態にする

			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}