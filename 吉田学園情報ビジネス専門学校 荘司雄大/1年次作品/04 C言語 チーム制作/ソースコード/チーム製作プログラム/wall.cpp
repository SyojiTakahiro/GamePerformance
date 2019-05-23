//=============================================================================
//
// ポリゴン処理 [Wall.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "wall.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define WALL_TEXTURENAME		 "data\\TEXTURE\\wall002.jpg"	//テクスチャのファイル名
#define TEX_POS_X_INIT			(1.0f)							//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)							//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)							//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)							//テクスチャ座標U右
#define TEX_TOP					(0.0f)							//テクスチャ座標V上
#define TEX_BOT					(1.0f)							//テクスチャ座標V下
#define MAX_WALL				(128)							//壁の最大数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;	//頂点バッファへのポインタ
//D3DXVECTOR3 g_posWall;							//位置
//D3DXVECTOR3 g_rotWall;							//向き
//D3DXMATRIX	g_mtrxWorldWall;					//ワールドマトリックス
LPDIRECT3DTEXTURE9	g_pTextureWall = NULL;		//テクスチャへのポインタ
WALL g_aWall[MAX_WALL];
//=============================================================================
// 初期化処理
//=============================================================================
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスの取得
	pDevice = GetDevice();	// デバイスの取得

	//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			WALL_TEXTURENAME,
			&g_pTextureWall);
	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_WALL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWall,
		NULL);

	VERTEX_3D *pVtx;
	//頂点バッファをロック
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		//頂点座標
		//pVtx[0].pos = D3DXVECTOR3(-100.0f, 50.0f, 0.0f);
		//pVtx[1].pos = D3DXVECTOR3(100.0f, 50.0f, 0.0f);
		//pVtx[2].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);
		//pVtx[3].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);

		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aWall[nCntWall].fWidth, g_aWall[nCntWall].fHeght, 0);
		pVtx[1].pos = D3DXVECTOR3(g_aWall[nCntWall].fWidth, g_aWall[nCntWall].fHeght, 0);
		pVtx[2].pos = D3DXVECTOR3(g_aWall[nCntWall].fWidth, 0, 0);
		pVtx[3].pos = D3DXVECTOR3(g_aWall[nCntWall].fWidth, 0, 0);
		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(TEX_LEFT, TEX_TOP);
		pVtx[1].tex = D3DXVECTOR2(TEX_RIGHT, TEX_TOP);
		pVtx[2].tex = D3DXVECTOR2(TEX_LEFT, TEX_BOT);
		pVtx[3].tex = D3DXVECTOR2(TEX_RIGHT, TEX_BOT);
		//頂点データのポインタを4つ分進める
		pVtx += 4;	
	}

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		g_aWall[nCntWall].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWall[nCntWall].fRadius = 0.0f;
		g_aWall[nCntWall].fHeght = 0.0f;
		g_aWall[nCntWall].fWidth = 0.0f;
		g_aWall[nCntWall].bUse = false;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitWall(void)
{	//テクスチャの破棄
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}
	// 頂点バッファの開放
	if (g_pVtxBuffWall != NULL)
	{
		g_pVtxBuffWall->Release();
		g_pVtxBuffWall = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateWall(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX  mtxRot, mtxTrans;				//計算用マトリックス

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aWall[nCntWall].mtrxWorld);
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.z);
		D3DXMatrixMultiply(&g_aWall[nCntWall].mtrxWorld, &g_aWall[nCntWall].mtrxWorld, &mtxRot);
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aWall[nCntWall].pos.x , g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
		D3DXMatrixMultiply(&g_aWall[nCntWall].mtrxWorld, &g_aWall[nCntWall].mtrxWorld, &mtxTrans);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCntWall].mtrxWorld);
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));
		//頂点フォーマットを設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureWall);

		if (g_aWall[nCntWall].bUse == true)	//壁が使用されている
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				0 + (4 * nCntWall),	//開始する頂点のインデックス
				2); //描画するプリミティブ数
		}
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定

}

//=============================================================================
// 壁の設定処理
//=============================================================================
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fHeght, float fWidth)
{
	int nCntWall;
	VERTEX_3D *pVtx;//頂点情報へのポインタ
					//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		if (g_aWall[nCntWall].bUse == false)	//壁が使用されていない
		{
			g_aWall[nCntWall].pos = pos;
			g_aWall[nCntWall].rot = rot;
			g_aWall[nCntWall].fHeght= fHeght;
			g_aWall[nCntWall].fWidth = fWidth;

			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(-g_aWall[nCntWall].fWidth, g_aWall[nCntWall].fHeght, 0);
			pVtx[1].pos = D3DXVECTOR3(g_aWall[nCntWall].fWidth, g_aWall[nCntWall].fHeght, 0);
			pVtx[2].pos = D3DXVECTOR3(-g_aWall[nCntWall].fWidth, 0, 0);
			pVtx[3].pos = D3DXVECTOR3(g_aWall[nCntWall].fWidth, 0, 0);

			g_aWall[nCntWall].bUse = true;
			break;
		}
		pVtx += 4;	//頂点データのポインタを4つ分進める	
	}
	//頂点バッファをアンロックする
	g_pVtxBuffWall->Unlock();
}
