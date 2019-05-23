//=============================================================================
//
// 壁処理 [Boal.cpp]
// Author : 
//
//=============================================================================
#include "boal.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_NAME	"data/TEXTURE/bullet000.png"
#define MAX_Boal (128)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexBoal(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBoal = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBoal = NULL;

//D3DXVECTOR3 g_aBoal[nCntBoal].pos;
//D3DXVECTOR3 g_aBoal[nCntBoal].rot;
//D3DXMATRIX g_aBoal[nCntBoal].mtxWorld;

Boal g_aBoal[MAX_Boal];

//=============================================================================
// 初期化処理
//=============================================================================
void InitBoal(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	pDevice = GetDevice();
	int nCntBoal;

	for (nCntBoal = 0; nCntBoal < MAX_Boal; nCntBoal++)
	{
		g_aBoal[nCntBoal].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoal[nCntBoal].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBoal[nCntBoal].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEXTURE_NAME, &g_pTextureBoal);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_Boal,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBoal,
		NULL);

	VERTEX_3D*pVtx;

	//頂点バッファをロック
	g_pVtxBuffBoal->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBoal = 0; nCntBoal < MAX_Boal; nCntBoal++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-10, 10, 0);
		pVtx[1].pos = D3DXVECTOR3(10, 10, 0);
		pVtx[2].pos = D3DXVECTOR3(-10, -10, 0);
		pVtx[3].pos = D3DXVECTOR3(10, -10, 0);

		pVtx[0].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx[0].tex = D3DXVECTOR2(0, 0);
		pVtx[1].tex = D3DXVECTOR2(1, 0);
		pVtx[2].tex = D3DXVECTOR2(0, 1);
		pVtx[3].tex = D3DXVECTOR2(1, 1);

		// 位置・向きの初期設定
		g_aBoal[nCntBoal].pos = D3DXVECTOR3(0, 30, 0);
		g_aBoal[nCntBoal].rot = D3DXVECTOR3(0, 0, 0);

		pVtx += 4;
	}
	//アンロック
	g_pVtxBuffBoal->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBoal(void)
{
	//テクスチャの破棄
	if (g_pTextureBoal != NULL)
	{
		g_pTextureBoal->Release();
		g_pTextureBoal = NULL;
	}
	//頂点の破棄
	if (g_pVtxBuffBoal != NULL)
	{
		g_pVtxBuffBoal->Release();
		g_pVtxBuffBoal = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBoal(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBoal(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	D3DXMATRIX mtxRot, mtxTrans, mtxView;
	int nCntBoal;

	pDevice = GetDevice();	// デバイスの取得

	for (nCntBoal = 0; nCntBoal < MAX_Boal; nCntBoal++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aBoal[nCntBoal].mtxWorld);

		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		g_aBoal[nCntBoal].mtxWorld._11 = mtxView._11;
		g_aBoal[nCntBoal].mtxWorld._12 = mtxView._21;
		g_aBoal[nCntBoal].mtxWorld._13 = mtxView._31;
		g_aBoal[nCntBoal].mtxWorld._21 = mtxView._12;
		g_aBoal[nCntBoal].mtxWorld._22 = mtxView._22;
		g_aBoal[nCntBoal].mtxWorld._23 = mtxView._32;
		g_aBoal[nCntBoal].mtxWorld._31 = mtxView._13;
		g_aBoal[nCntBoal].mtxWorld._32 = mtxView._23;
		g_aBoal[nCntBoal].mtxWorld._33 = mtxView._33;


		//// 向きを反映
		//D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBoal[nCntBoal].rot.y, g_aBoal[nCntBoal].rot.x, g_aBoal[nCntBoal].rot.z);

		//D3DXMatrixMultiply(&g_aBoal[nCntBoal].mtxWorld, &g_aBoal[nCntBoal].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aBoal[nCntBoal].pos.x, g_aBoal[nCntBoal].pos.y, g_aBoal[nCntBoal].pos.z);

		D3DXMatrixMultiply(&g_aBoal[nCntBoal].mtxWorld, &g_aBoal[nCntBoal].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aBoal[nCntBoal].mtxWorld);

		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffBoal, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBoal);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBoal, 2);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexBoal(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定

}
