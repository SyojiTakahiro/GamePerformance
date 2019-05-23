//=============================================================================
//
// 爆発処理 [explosion.cpp]
// Author :
//
//=============================================================================
#include "explosion.h"
#include "bullet.h"

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
int nCounterAnim;	
int nPatternAnim;	
bool bUse;
}Explosion;

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// 頂点バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION];
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
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	//g_posExplosion = D3DXVECTOR3(600, 600, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME, &g_pTextureExplosion);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffExplosion,
								NULL);

	VERTEX_2D*pVtx;

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 200, 200, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 200, 200, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 200, 200, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 200, 200, 255);
	
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
	VERTEX_2D*pVtx;

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
	int nCntExplosion;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureExplosion);

	// ポリゴンの描画
	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntExplosion, 2);
		}
	}
}

//=============================================================================
// 弾の設定処理
//=============================================================================
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCntExplosion ;
	VERTEX_2D*pVtx;
	
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == false)
		{
			g_aExplosion[nCntExplosion].pos = pos;
			pVtx[0].pos = D3DXVECTOR3(-(EXPLOSION_WIDTH / 2), -(EXPLOSION_HEIGHT / 2), 0.0f) + g_aExplosion[nCntExplosion].pos;
			pVtx[1].pos = D3DXVECTOR3((EXPLOSION_WIDTH / 2), -(EXPLOSION_HEIGHT / 2), 0.0f) + g_aExplosion[nCntExplosion].pos;
			pVtx[2].pos = D3DXVECTOR3(-(EXPLOSION_WIDTH / 2), (EXPLOSION_HEIGHT / 2), 0.0f) + g_aExplosion[nCntExplosion].pos;
			pVtx[3].pos = D3DXVECTOR3((EXPLOSION_WIDTH / 2), (EXPLOSION_HEIGHT / 2), 0.0f) + g_aExplosion[nCntExplosion].pos;
			g_aExplosion[nCntExplosion].col = col;
			
			pVtx[0].col = D3DCOLOR_RGBA(255, 69, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 69, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 69, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 69, 0, 255);

			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffExplosion->Unlock();
}