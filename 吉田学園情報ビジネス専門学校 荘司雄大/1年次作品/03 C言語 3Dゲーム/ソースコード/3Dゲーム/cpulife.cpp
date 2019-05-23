//=============================================================================
//
// ライフ処理 [life.cpp]
// Author : Takahiro
//
//=============================================================================
#include "game.h"
#include "player.h"
#include "cpulife.h"
#include "particle.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TIMER		"data/TEXTURE/life000.png"	// 読み込むテクスチャファイル名
#define TEXTURE_SIZE        (20)                            // テクスチャのサイズ
#define MAX_LIFE			(3)                             // プレイヤーのライフ数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexCPULife(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureCPULife = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCPULife = NULL;	// 頂点バッファへのポインタ
CPULIFE                    g_life[MAX_LIFE];		// ライフ
int                     g_nCounterCPULife;         // 現在のライフ数
float g_lCPUSpin = 1.01f;

//=============================================================================
// 初期化処理
//=============================================================================
void InitCPULife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntCPULife = 0; nCntCPULife < MAX_LIFE; nCntCPULife++)
	{// ライフアイコン数だけ繰り返し
		g_life[nCntCPULife].pos = D3DXVECTOR3(1100, 40, 0);   // 座標を初期化
		g_life[nCntCPULife].bUse = true;                     // 使用している状態にする
	}
	g_nCounterCPULife = MAX_LIFE;							  // 現在のライフ数を初期化

														  // テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,					  // デバイスへのポインタ
		TEXTURE_TIMER,									  // ファイルの名前
		&g_pTextureCPULife);								  // テクスチャへのポインタ

														  // 頂点情報の作成
	MakeVertexCPULife(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitCPULife(void)
{
	// テクスチャの開放
	if (g_pTextureCPULife != NULL)
	{
		g_pTextureCPULife->Release();
		g_pTextureCPULife = NULL;
	}

	// 頂点バッファの破棄
	if (g_pVtxBuffCPULife != NULL)
	{
		g_pVtxBuffCPULife->Release();
		g_pVtxBuffCPULife = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateCPULife(void)
{
	GAMESTATE GameState;
	GameState = GetGameState();

	//int nCntCPULife;
	//VERTEX_2D*pVtx;
	//float pos = 0;

	////頂点バッファをロック
	//g_pVtxBuffCPULife->Lock(0, 0, (void**)&pVtx, 0);

	//for (nCntCPULife = 0; nCntCPULife < MAX_LIFE; nCntCPULife++)
	//{
	//	if (g_life[nCntCPULife].bUse == true)
	//	{
	//		pVtx[nCntCPULife * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
	//		pVtx[nCntCPULife * 4+1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//		pVtx[nCntCPULife * 4+2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//		pVtx[nCntCPULife * 4+3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	}
	//for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
	//{// パーティクル発生
	//	D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);
	//	movePar.x = sinf(D3DX_PI * g_lSpin) * (rand() % 10 * 1.5f);
	//	movePar.y = cosf(D3DX_PI * g_lSpin) * (rand() % 3);
	//	SetParticle(g_life[nCntCPULife].pos, movePar, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), rand() % 20, rand() % 15);
	//}
	//}
	////アンロック
	//g_pVtxBuffCPULife->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawCPULife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCPULife, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureCPULife);

	// ポリゴンの描画
	for (int nCntCPULife = 0; nCntCPULife < MAX_LIFE; nCntCPULife++)
	{// 表示するライフアイコン数だけ繰り返し
		if (g_life[nCntCPULife].bUse == true)
		{// ライフが使用されている
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntCPULife * 4,
				2);
		}
	}
}
//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexCPULife(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCPULife,
		NULL);

	// 頂点バッファを生成
	VERTEX_2D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffCPULife->Lock(0, 0, (void**)&pVtx, 0);

	float pos = 0;
	// 頂点情報を設定
	for (int nCntCPULife = 0; nCntCPULife < MAX_LIFE; nCntCPULife++)
	{// ライフアイコン数だけ繰り返し
	 // 頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_life[nCntCPULife].pos.x - TEXTURE_SIZE + pos, g_life[nCntCPULife].pos.y - TEXTURE_SIZE, g_life[nCntCPULife].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_life[nCntCPULife].pos.x + TEXTURE_SIZE + pos, g_life[nCntCPULife].pos.y - TEXTURE_SIZE, g_life[nCntCPULife].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_life[nCntCPULife].pos.x - TEXTURE_SIZE + pos, g_life[nCntCPULife].pos.y + TEXTURE_SIZE, g_life[nCntCPULife].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_life[nCntCPULife].pos.x + TEXTURE_SIZE + pos, g_life[nCntCPULife].pos.y + TEXTURE_SIZE, g_life[nCntCPULife].pos.z);


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
	g_pVtxBuffCPULife->Unlock();
}
//=============================================================================
// ライフの減少
//=============================================================================
void CutCPULife(int cut)
{

	GAMESTATE GameState;
	GameState = GetGameState();

	for (int nCntCPULife = 0; nCntCPULife < MAX_LIFE; nCntCPULife++)
	{// ライフアイコン数だけ繰り返し
		if (g_life[nCntCPULife].bUse == true)
		{// ライフが使用されている
			g_life[nCntCPULife].bUse = false;
			g_nCounterCPULife--;
			break;
		}

		if (g_nCounterCPULife == 1)
		{
			if (GameState != GAMESTATE_P1WIN)
			{
				SetFade(MODE_P1WIN);
				StopSound(SOUND_LABEL_BGM001);
			}
		}
	}
}