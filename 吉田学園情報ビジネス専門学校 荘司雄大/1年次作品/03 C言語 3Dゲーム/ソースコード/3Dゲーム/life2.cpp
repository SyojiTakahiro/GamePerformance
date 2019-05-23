//=============================================================================
//
// ライフ処理 [life.cpp]
// Author : Takahiro
//
//=============================================================================
#include "game.h"
#include "player2.h"
#include "life2.h"
#include "particle.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TIMER2		"data/TEXTURE/life001.png"	// 読み込むテクスチャファイル名
#define TEXTURE_SIZE2        (20)                            // テクスチャのサイズ
#define MAX_LIFE2			 (3)                             // プレイヤーのライフ数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexLife2(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureLife2 = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife2 = NULL;	// 頂点バッファへのポインタ
LIFE2                    g_life2[MAX_LIFE2];		// ライフ
int                     g_nCounterLife2;         // 現在のライフ数
float g_lSpin2 = 1.01f;

//=============================================================================
// 初期化処理
//=============================================================================
void InitLife2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntLife = 0; nCntLife < MAX_LIFE2; nCntLife++)
	{// ライフアイコン数だけ繰り返し
		g_life2[nCntLife].pos = D3DXVECTOR3(1100, 40, 0);   // 座標を初期化
		g_life2[nCntLife].bUse = true;                     // 使用している状態にする
	}
	g_nCounterLife2 = MAX_LIFE2;							  // 現在のライフ数を初期化

														  // テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					  // デバイスへのポインタ
		TEXTURE_TIMER2,									  // ファイルの名前
		&g_pTextureLife2);								  // テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexLife2(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitLife2(void)
{
	// テクスチャの開放
	if (g_pTextureLife2 != NULL)
	{
		g_pTextureLife2->Release();
		g_pTextureLife2 = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffLife2 != NULL)
	{
		g_pVtxBuffLife2->Release();
		g_pVtxBuffLife2 = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateLife2(void)
{

	/*if (g_nCounterLife2 == 0)
	{
		if (GameState != GAMESTATE_P2WIN)
		{
			SetFade(MODE_P2WIN);
		}
	}*/
	//int nCntLife;
	//VERTEX_2D*pVtx;
	//float pos = 0;

	////頂点バッファをロック
	//g_pVtxBuffLife2->Lock(0, 0, (void**)&pVtx, 0);

	//for (nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	//{
	//	if (g_life2[nCntLife].bUse == true)
	//	{
	//		pVtx[nCntLife * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
	//		pVtx[nCntLife * 4+1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//		pVtx[nCntLife * 4+2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//		pVtx[nCntLife * 4+3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	}
		//for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		//{// パーティクル発生
		//	D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);
		//	movePar.x = sinf(D3DX_PI * g_lSpin) * (rand() % 10 * 1.5f);
		//	movePar.y = cosf(D3DX_PI * g_lSpin) * (rand() % 3);
		//	SetParticle(g_life2[nCntLife].pos, movePar, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), rand() % 20, rand() % 15);
		//}
	//}
	////アンロック
	//g_pVtxBuffLife2->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawLife2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffLife2, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureLife2);

	// ポリゴンの描画
	for (int nCntLife = 0; nCntLife < MAX_LIFE2; nCntLife++)
	{// 表示するライフアイコン数だけ繰り返し
		if (g_life2[nCntLife].bUse == true)
		{// ライフが使用されている
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntLife * 4,
				2);
		}
	}
}
//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexLife2(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife2,
		NULL);

	// 頂点バッファを生成
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffLife2->Lock(0, 0, (void**)&pVtx, 0);

	float pos = 0;
	// 頂点情報を設定
	for (int nCntLife = 0; nCntLife < MAX_LIFE2; nCntLife++)
	{// ライフアイコン数だけ繰り返し
	 // 頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_life2[nCntLife].pos.x - TEXTURE_SIZE2 + pos, g_life2[nCntLife].pos.y - TEXTURE_SIZE2, g_life2[nCntLife].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_life2[nCntLife].pos.x + TEXTURE_SIZE2 + pos, g_life2[nCntLife].pos.y - TEXTURE_SIZE2, g_life2[nCntLife].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_life2[nCntLife].pos.x - TEXTURE_SIZE2 + pos, g_life2[nCntLife].pos.y + TEXTURE_SIZE2, g_life2[nCntLife].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_life2[nCntLife].pos.x + TEXTURE_SIZE2 + pos, g_life2[nCntLife].pos.y + TEXTURE_SIZE2, g_life2[nCntLife].pos.z);


		// 頂点テクスチャ
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
		pos -= 40.0f;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffLife2->Unlock();
}
//=============================================================================
// ライフの減少
//=============================================================================
void CutLife2(int cut)
{
	GAMESTATE GameState;
	GameState = GetGameState();

	for (int nCntLife = 0; nCntLife < MAX_LIFE2; nCntLife++)
	{// ライフアイコン数だけ繰り返し
		if (g_life2[nCntLife].bUse == true)
		{// ライフが使用されている
			g_life2[nCntLife].bUse = false;
			g_nCounterLife2--;
			break;
		}

		if (g_nCounterLife2 == 1)
		{
			if (GameState != GAMESTATE_P1WIN)
			{
				SetFade(MODE_P1WIN);
				StopSound(SOUND_LABEL_BGM001);
			}
		}
	}
}