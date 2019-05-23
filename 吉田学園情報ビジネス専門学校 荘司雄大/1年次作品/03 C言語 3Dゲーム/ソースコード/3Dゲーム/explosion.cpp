//=============================================================================
//
// 爆発処理 [explosion.cpp]
// Author :
//
//=============================================================================
#include "explosion.h"
#include "Explosion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_EXPLOSION		(256)		// 弾の最大数
#define EXPLOSION_WIDTH		(50)		//爆発の幅
#define EXPLOSION_HEIGHT	(50)		//爆発の高さ
#define POLYGON_TEXTURENAME		"data/TEXTURE/explosion000.png"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
D3DXVECTOR3 pos;
D3DXCOLOR col;
D3DXMATRIX mtxWorld;
int nCounterAnim;	
int nPatternAnim;	
bool bUse;
}EXPLOSION;

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// 頂点バッファへのポインタ
EXPLOSION g_aExplosion[MAX_EXPLOSION];
D3DXVECTOR3 g_posExplosion;
//=============================================================================
// 初期化処理
//=============================================================================
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntExplosion;

	// デバイスの取得
	pDevice = GetDevice();

	// 弾の情報の初期化
	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//g_posExplosion = D3DXVECTOR3(600, 600, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME, &g_pTextureExplosion);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EXPLOSION,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_3D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffExplosion,
								NULL);

	VERTEX_3D*pVtx;

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
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
	//アンロック
	g_pVtxBuffExplosion->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitExplosion(void)
{
	// テクスチャの開放
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateExplosion(void)
{
	int nCntExplosion;
	VERTEX_3D*pVtx;

	/*if (g_aExplosion[nCntExplosion].nCounterAnim %10)== 0)
	{
		g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % 10
	}*/

	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].nCounterAnim++;
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
		/*	g_aExplosion[nCntExplosion].pos.y -= 10.0f;
			g_posExplosion.y -= 10.0f;*/
			//pVtx += (nCntExplosion * 4);   //該当の位置まで進める

				g_aExplosion[nCntExplosion].nPatternAnim++;
				if (g_aExplosion[nCntExplosion].nPatternAnim > 7)
				{
					g_aExplosion[nCntExplosion].bUse = false;
				}
			//頂点バッファをロック
			//g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
			pVtx[nCntExplosion*4].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0.0f);
			pVtx[nCntExplosion*4+1].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f + 0.125f, 0.0f);
			pVtx[nCntExplosion*4+2].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f, 0.0f + 1.0f);
			pVtx[nCntExplosion*4+3].tex = D3DXVECTOR2(g_aExplosion[nCntExplosion].nPatternAnim * 0.125f + 0.125f, 0.0f + 1.0f);
			//アンロック
			//g_pVtxBuffExplosion->Unlock();
			
		}
	}
	//if ((g_aExplosion[nCntExplosion].nCounterAnim % 3) == 0)
	//{
	//	g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim + 1) % 8;
	//}

	g_pVtxBuffExplosion  ->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans, mtxView;
	int nCntExplosion;

	pDevice = GetDevice();	// デバイスの取得

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);

	// αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);


	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aExplosion[nCntExplosion].mtxWorld);

		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		g_aExplosion[nCntExplosion].mtxWorld._11 = mtxView._11;
		g_aExplosion[nCntExplosion].mtxWorld._12 = mtxView._21;
		g_aExplosion[nCntExplosion].mtxWorld._13 = mtxView._31;
		g_aExplosion[nCntExplosion].mtxWorld._21 = mtxView._12;
		g_aExplosion[nCntExplosion].mtxWorld._22 = mtxView._22;
		g_aExplosion[nCntExplosion].mtxWorld._23 = mtxView._32;
		g_aExplosion[nCntExplosion].mtxWorld._31 = mtxView._13;
		g_aExplosion[nCntExplosion].mtxWorld._32 = mtxView._23;
		g_aExplosion[nCntExplosion].mtxWorld._33 = mtxView._33;


		//// 向きを反映
		//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aExplosion[nCntExplosion].rot.y, g_aExplosion[nCntExplosion].rot.x, g_aExplosion[nCntExplosion].rot.z);

		//D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCntExplosion].pos.x, g_aExplosion[nCntExplosion].pos.y, g_aExplosion[nCntExplosion].pos.z);

		D3DXMatrixMultiply(&g_aExplosion[nCntExplosion].mtxWorld, &g_aExplosion[nCntExplosion].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCntExplosion].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureExplosion);

		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		// ポリゴンの描画
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntExplosion * 4,//描画を開始する頂点インデックス
				2);
		}
	}
	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
}

//=============================================================================
// 弾の設定処理
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion ;
	VERTEX_3D*pVtx;
	
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{
			g_aExplosion[nCntExplosion].pos = pos;
			pVtx[0].pos = D3DXVECTOR3(-(EXPLOSION_WIDTH / 2), (EXPLOSION_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((EXPLOSION_WIDTH / 2), (EXPLOSION_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-(EXPLOSION_WIDTH / 2), -(EXPLOSION_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((EXPLOSION_WIDTH / 2), -(EXPLOSION_HEIGHT / 2), 0.0f);
			g_aExplosion[nCntExplosion].col = col;
			
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}