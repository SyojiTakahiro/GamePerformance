//=============================================================================
//
// 影 処理 [Shadow.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "shadow.h"
#include "camera.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define Shadow_TEXTURENAME		 "data\\TEXTURE\\shadow000.jpg"	//テクスチャのファイル名
#define TEX_POS_X_INIT			(1.0f)							//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)							//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)							//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)							//テクスチャ座標U右
#define TEX_TOP					(0.0f)							//テクスチャ座標V上
#define TEX_BOT					(1.0f)							//テクスチャ座標V下
#define MAX_SHADOW				(600)							//影の最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;		//頂点バッファへのポインタ
SHADOW		g_aShadow[MAX_SHADOW];						//影
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;	//テクスチャへのポインタ
//=============================================================================
// 初期化処理
//=============================================================================
void InitShadow(void)
{
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCntShadow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aShadow[nCntShadow].fRadius = 0.0f;
		g_aShadow[nCntShadow].bUse = false;
	}

	LPDIRECT3DDEVICE9 pDevice;	// デバイスの取得
	pDevice = GetDevice();	// デバイスの取得

	//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			Shadow_TEXTURENAME,
			&g_pTextureShadow);
	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_SHADOW,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffShadow,
		NULL);

	
	VERTEX_3D *pVtx;
	//頂点バッファをロック
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fRadius, 0, g_aShadow[nCntShadow].fRadius);
		pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fRadius, 0, g_aShadow[nCntShadow].fRadius);
		pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fRadius, 0, -g_aShadow[nCntShadow].fRadius);
		pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fRadius, 0, -g_aShadow[nCntShadow].fRadius);

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(TEX_LEFT, TEX_TOP);
		pVtx[1].tex = D3DXVECTOR2(TEX_RIGHT, TEX_TOP);
		pVtx[2].tex = D3DXVECTOR2(TEX_LEFT, TEX_BOT);
		pVtx[3].tex = D3DXVECTOR2(TEX_RIGHT, TEX_BOT);
		
		pVtx += 4;	//頂点データのポインタを4つ分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitShadow(void)
{	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateShadow(void)
{
	Camera *pCamera;
	pCamera = GetCamera();

	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		//カメラとの距離を求める
		double length = pow((g_aShadow[nCntShadow].pos.x - pCamera->posV.x)*(g_aShadow[nCntShadow].pos.x - pCamera->posV.x)
			+ (g_aShadow[nCntShadow].pos.z - pCamera->posV.z)*(g_aShadow[nCntShadow].pos.z - pCamera->posV.z), 0.5);

		//float型に変換
		g_aShadow[nCntShadow].fLength = (float)length;
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX  mtxRot[MAX_SHADOW],  mtxTrans[MAX_SHADOW];				//計算用マトリックス

	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	for (int nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].fLength < 2200)
		{//影とプレイヤーの距離で描画するか決める
		// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtrxWorld);
			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot[nCntShadow], g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtrxWorld, &g_aShadow[nCntShadow].mtrxWorld, &mtxRot[nCntShadow]);
			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans[nCntShadow], g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtrxWorld, &g_aShadow[nCntShadow].mtrxWorld, &mtxTrans[nCntShadow]);
			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtrxWorld);
			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));
			//頂点フォーマットを設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);
			if (g_aShadow[nCntShadow].bUse == true)	//影が使用されている
			{
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
					0 + (4 * nCntShadow),	//開始する頂点のインデックス
					2); //描画するプリミティブ数
			}
		}
	}
	//レンダーステイトを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// 影の設定処理
//=============================================================================
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius)
{
	int nCntShadow;
	VERTEX_3D *pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == false)	//影が使用されていない
		{
			g_aShadow[nCntShadow].pos = pos;
			g_aShadow[nCntShadow].rot = rot;
			g_aShadow[nCntShadow].fRadius = fRadius;
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fRadius, 0, g_aShadow[nCntShadow].fRadius);
			pVtx[1].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fRadius, 0, g_aShadow[nCntShadow].fRadius);
			pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nCntShadow].fRadius, 0, -g_aShadow[nCntShadow].fRadius);
			pVtx[3].pos = D3DXVECTOR3(g_aShadow[nCntShadow].fRadius, 0, -g_aShadow[nCntShadow].fRadius);
			
			g_aShadow[nCntShadow].bUse = true;
			break;
		}
		pVtx += 4;	//頂点データのポインタを4つ分進める	
	}
	return nCntShadow;
	//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// 影の位置設定
//=============================================================================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, float fRadius)
{
	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].fRadius = fRadius;

	VERTEX_3D *pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);
	//頂点データのポインタをインデックス分進める
	pVtx += 4 * nIdxShadow;

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(-g_aShadow[nIdxShadow].fRadius, 0, g_aShadow[nIdxShadow].fRadius);
	pVtx[1].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].fRadius, 0, g_aShadow[nIdxShadow].fRadius);
	pVtx[2].pos = D3DXVECTOR3(-g_aShadow[nIdxShadow].fRadius, 0, -g_aShadow[nIdxShadow].fRadius);
	pVtx[3].pos = D3DXVECTOR3(g_aShadow[nIdxShadow].fRadius, 0, -g_aShadow[nIdxShadow].fRadius);

//頂点バッファをアンロックする
	g_pVtxBuffShadow->Unlock();
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexShadow(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void DeleteShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}
