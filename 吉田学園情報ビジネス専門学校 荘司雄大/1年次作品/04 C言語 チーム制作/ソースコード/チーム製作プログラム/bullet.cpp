//=============================================================================
//
// ポリゴン処理 [Bullet.cpp]
// Author : Meguro Mikiya
//
//=============================================================================
#include "bullet.h"
#include "shadow.h"
#include "model.h"
#include "explosion.h"
#include "effect.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_TEXTURENAME		 "data\\TEXTURE\\bullet000.png"			//テクスチャのファイル名
#define TEX_POS_X_INIT			(1.0f)									//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)									//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)									//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)									//テクスチャ座標U右
#define TEX_TOP					(0.0f)									//テクスチャ座標V上
#define TEX_BOT					(1.0f)									//テクスチャ座標V下
#define MAX_BULLET				(100)									//弾の最大数
#define EFFECT_COLOR_PLAYER		(D3DXCOLOR (0.3f, 0.8f, 0.8f, 1.0f))	//自機の弾のエフェクトカラー
#define EFFECT_RADIUS_PLAYER	(10.0f)									//エフェクトの半径
#define EFFECT_LIFE_PLAYER		(10)									//エフェクトのライフ


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9	g_pTextureBullet = NULL;		//テクスチャへのポインタ
BULLET g_aBullet[MAX_BULLET];

//=============================================================================
// 初期化処理
//=============================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;	// デバイスの取得
	pDevice = GetDevice();	// デバイスの取得

	//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			BULLET_TEXTURENAME,
			&g_pTextureBullet);
	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_3D *pVtx;
	//頂点バッファをロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(-10.0f, 10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-10.0f, -10.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(10.0f, -10.0f, 0.0f);

		//法線の設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
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
		//頂点データのポインタを4つ分進める
		pVtx += 4;	
	}

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].fRadius = 0.0f;
		g_aBullet[nCntBullet].fHeght = 0.0f;
		g_aBullet[nCntBullet].fWidth = 0.0f;
		g_aBullet[nCntBullet].nLife = 200;
		g_aBullet[nCntBullet].bUse = false;
		g_aBullet[nCntBullet].VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}
	// 頂点バッファの開放
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
	VERTEX_3D *pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)	//弾が使用されている
		{
			g_aBullet[nCntBullet].posold = g_aBullet[nCntBullet].pos;
			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
			g_aBullet[nCntBullet].pos.z += g_aBullet[nCntBullet].move.z;
			//エフェクト描画
			SetEffect(g_aBullet[nCntBullet].pos, EFFECT_COLOR_PLAYER, EFFECT_RADIUS_PLAYER, EFFECT_LIFE_PLAYER);

			g_aBullet[nCntBullet].nLife--;

			//弾のライフが尽きたら消える
			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				g_aBullet[nCntBullet].bUse = false;
				DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.7f, 0.1f, 1.0f), 15.0f, 15.0f);
			}
			//ブロックとの当たり判定
			if (CollisionModel(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posold, &g_aBullet[nCntBullet].move, g_aBullet[nCntBullet].VtxMax, g_aBullet[nCntBullet].VtxMin) == true)
			{
				g_aBullet[nCntBullet].bUse = false;
				DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), 15.0f, 15.0f);

			}
			//壁との当たり判定
			if (FIELD_SIZE < g_aBullet[nCntBullet].pos.x)
			{
				g_aBullet[nCntBullet].bUse = false;
				DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.7f, 0.1f, 1.0f), 15.0f, 15.0f);

			}
			if (-FIELD_SIZE > g_aBullet[nCntBullet].pos.x)
			{
				g_aBullet[nCntBullet].bUse = false;
				DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.7f, 0.1f, 1.0f), 15.0f, 15.0f);

			}
			if (FIELD_SIZE < g_aBullet[nCntBullet].pos.z)
			{
				g_aBullet[nCntBullet].bUse = false;
				DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.7f, 0.1f, 1.0f), 15.0f, 15.0f);

			}
			if (-FIELD_SIZE > g_aBullet[nCntBullet].pos.z)
			{
				g_aBullet[nCntBullet].bUse = false;
				DeleteShadow(g_aBullet[nCntBullet].nIdxShadow);
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.7f, 0.1f, 1.0f), 15.0f, 15.0f);

			}

			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow,
				D3DXVECTOR3(g_aBullet[nCntBullet].pos.x, 0, g_aBullet[nCntBullet].pos.z), 5);
		}
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX  mtxView, mtxTrans;				//計算用マトリックス
	
	//アルファテスト(透明色を描画しないように)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//Zバッファの設定
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtrxWorldBullet);
		//ビューマトリックスを取得
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);
		//逆行列を設定
		g_aBullet[nCntBullet].mtrxWorldBullet._11 = mtxView._11;
		g_aBullet[nCntBullet].mtrxWorldBullet._12 = mtxView._21;
		g_aBullet[nCntBullet].mtrxWorldBullet._13 = mtxView._31;
		g_aBullet[nCntBullet].mtrxWorldBullet._21 = mtxView._12;
		g_aBullet[nCntBullet].mtrxWorldBullet._22 = mtxView._22;
		g_aBullet[nCntBullet].mtrxWorldBullet._23 = mtxView._32;
		g_aBullet[nCntBullet].mtrxWorldBullet._31 = mtxView._13;
		g_aBullet[nCntBullet].mtrxWorldBullet._32 = mtxView._23;
		g_aBullet[nCntBullet].mtrxWorldBullet._33 = mtxView._33;
		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);
		D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtrxWorldBullet, &g_aBullet[nCntBullet].mtrxWorldBullet, &mtxTrans);
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtrxWorldBullet);
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));
		//頂点フォーマットを設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureBullet);

		if (g_aBullet[nCntBullet].bUse == true)	//弾が使用されている
		{
			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				0 + (4 * nCntBullet),	//開始する頂点のインデックス
				2); //描画するプリミティブ数
		}
	}
	//レンダーステートの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//Zバッファをデフォルトの設定に戻す
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexBullet(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成


	// 頂点情報の設定

}

//=============================================================================
// 弾の設定処理
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fHeght, float fWidth)
{
	int nCntBullet;
	VERTEX_3D *pVtx;//頂点情報へのポインタ
					//頂点バッファをロックし頂点データのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)	//弾が使用されていない
		{
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].fHeght= fHeght;
			g_aBullet[nCntBullet].fWidth = fWidth;
			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeght, 0);
			pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fWidth, g_aBullet[nCntBullet].fHeght, 0);
			pVtx[2].pos = D3DXVECTOR3(-g_aBullet[nCntBullet].fWidth, -g_aBullet[nCntBullet].fHeght, 0);
			pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].fWidth, -g_aBullet[nCntBullet].fHeght, 0);
			//　影のセット
			g_aBullet[nCntBullet].nIdxShadow = SetShadow(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0),
				5);
			g_aBullet[nCntBullet].nLife = 100;
			g_aBullet[nCntBullet].bUse = true;
			g_aBullet[nCntBullet].bShadowUse = true;
			g_aBullet[nCntBullet].VtxMax = D3DXVECTOR3(fWidth, 0, 10);
			g_aBullet[nCntBullet].VtxMin = D3DXVECTOR3(-fWidth, 0, -10);
			break;
		}
		pVtx += 4;	//頂点データのポインタを4つ分進める	
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}
