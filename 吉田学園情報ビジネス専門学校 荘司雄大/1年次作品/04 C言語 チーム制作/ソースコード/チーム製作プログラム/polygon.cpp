//=============================================================================
//
// ポリゴン処理 [polygon.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "polygon.h"


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPolygon = NULL;	//頂点バッファへのポインタ
//D3DXVECTOR3 g_aPolygon.pos;							//位置
//D3DXVECTOR3 g_aPolygon.rot;							//向き
//D3DXMATRIX	g_aPolygon.mtrxWorld;						//ワールドマトリックス
POLYGON		g_aPolygon[MAX_POLYGON];								//ポリゴン構造体
LPDIRECT3DTEXTURE9			g_pTexturePolygon = NULL;	//テクスチャへのポインタ
//=============================================================================
// 初期化処理
//=============================================================================
void InitPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスの取得
	pDevice = GetDevice();	// デバイスの取得

	//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			POLYGON_TEXTURENAME,
			&g_pTexturePolygon);
	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_POLYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPolygon,
		NULL);


	VERTEX_3D *pVtx;
	//頂点バッファをロック
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_POLYGON; nCnt++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-FIELD_SIZE, 0.0f, FIELD_SIZE);
		pVtx[1].pos = D3DXVECTOR3(FIELD_SIZE, 0.0f, FIELD_SIZE);
		pVtx[2].pos = D3DXVECTOR3(-FIELD_SIZE, 0.0f, -FIELD_SIZE);
		pVtx[3].pos = D3DXVECTOR3(FIELD_SIZE, 0.0f, -FIELD_SIZE);
		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(TEX_LEFT, TEX_TOP);
		pVtx[1].tex = D3DXVECTOR2(TEX_RIGHT, TEX_TOP);
		pVtx[2].tex = D3DXVECTOR2(TEX_LEFT, TEX_BOT);
		pVtx[3].tex = D3DXVECTOR2(TEX_RIGHT, TEX_BOT);
		pVtx += 4;
		g_aPolygon[nCnt].bUse = false;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPolygon(void)
{	//テクスチャの破棄
	if (g_pTexturePolygon != NULL)
	{
		g_pTexturePolygon->Release();
		g_pTexturePolygon = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffPolygon != NULL)
	{
		g_pVtxBuffPolygon->Release();
		g_pVtxBuffPolygon = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePolygon(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPolygon(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCnt = 0; nCnt < MAX_POLYGON; nCnt++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPolygon[nCnt].mtrxWorld);
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPolygon[nCnt].rot.y, g_aPolygon[nCnt].rot.x, g_aPolygon[nCnt].rot.z);
		D3DXMatrixMultiply(&g_aPolygon[nCnt].mtrxWorld, &g_aPolygon[nCnt].mtrxWorld, &mtxRot);
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aPolygon[nCnt].pos.x, g_aPolygon[nCnt].pos.y, g_aPolygon[nCnt].pos.z);
		D3DXMatrixMultiply(&g_aPolygon[nCnt].mtrxWorld, &g_aPolygon[nCnt].mtrxWorld, &mtxTrans);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPolygon[nCnt].mtrxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffPolygon, 0, sizeof(VERTEX_3D));
		//頂点フォーマットを設定
		pDevice->SetFVF(FVF_VERTEX_3D);
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePolygon);
		//ポリゴンが使用されている
		if (g_aPolygon[nCnt].bUse == true)	
		{	// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				0,	//開始する頂点のインデックス
				2); //描画するプリミティブ数
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexPolygon(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定

}

//=============================================================================
// ポリゴンの設置
//=============================================================================
void SetPolygon(D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;//頂点情報へのポインタ
					//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffPolygon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_POLYGON; nCntPolygon++)
	{
		if (g_aPolygon[nCntPolygon].bUse == false)	//ポリゴンが使用されていない
		{
			g_aPolygon[nCntPolygon].pos = pos;
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(-FIELD_SIZE, 0.0f, FIELD_SIZE);
			pVtx[1].pos = D3DXVECTOR3(FIELD_SIZE, 0.0f, FIELD_SIZE);
			pVtx[2].pos = D3DXVECTOR3(-FIELD_SIZE, 0.0f, -FIELD_SIZE);
			pVtx[3].pos = D3DXVECTOR3(FIELD_SIZE, 0.0f, -FIELD_SIZE);

			g_aPolygon[nCntPolygon].bUse = true;
			break;
		}
		pVtx += 4;	//頂点データのポインタを4つ分進める	
	}
	//頂点バッファをアンロックする
	g_pVtxBuffPolygon->Unlock();
}