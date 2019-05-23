//=============================================================================
//
// タイム処理 [time.cpp]
// Author : Mikiya Meguro
//
//=============================================================================
#include "time.h"
#include "game.h"
#include "tutorial.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/number001.png"		// 読み込むテクスチャファイル名
#define TIME_POS_X				(20)							//頂点座標Xの移動量
#define TIME_POS_Y				(20)							//頂点座標Yの移動量
#define TEX_LEFT				(0.0f)							//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)							//テクスチャ座標U右
#define TEX_TOP					(0.0f)							//テクスチャ座標V上
#define TEX_BOT					(1.0f)							//テクスチャ座標V下
#define MAX_TIME				(3)								//タイムの桁数
#define TIME_NUM				(180)							//タイム
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureTime = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;		// 頂点バッファへのポインタ
TIME					g_Time;
//=============================================================================
// 初期化処理
//=============================================================================
void InitTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	

	// タイムの初期化
	g_Time.nTime = TIME_NUM;
	g_Time.bTimerStop = true;
	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,				// デバイスへのポインタ
								TEXTURE_SCORE,		// ファイルの名前
								&g_pTextureTime);	// テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexTime(pDevice);
	
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTime(void)
{
	// テクスチャの開放
		if (g_pTextureTime != NULL)
		{
			g_pTextureTime->Release();
			g_pTextureTime = NULL;
		}
	
	// 頂点バッファの開放
		if (g_pVtxBuffTime != NULL)
		{
			g_pVtxBuffTime->Release();
			g_pVtxBuffTime = NULL;
		}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTime(void)
{

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	//ゲームステイトの取得
	GAMESTATE GameState;
	GameState = GetGameState();

	int nCntTime;
	int nPosMove = 0;
	int nAnswer = 0;
	int nTime = 1;
	g_Time.TimePos.x = 680;
	g_Time.TimePos.y = 52.0f;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Time.bTimerStop == true)
	{
		g_Time.nTimeCounter++;
		if (g_Time.nTimeCounter >= 60)
		{
			g_Time.nTimeCounter = 0;
			g_Time.nTime -= 1;
		}
	}
	else if (g_Time.bTimerStop == false)
	{
	/*	g_nTimeCounter++;
		if (g_nTimeCounter >= 70)
		{*/
			//SetGameState(GAMESTATE_END);
		//	g_nTimeCounter = 0;
		//}
	}



	if (g_Time.nTime >= 0)
	{
		for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
		{

			nAnswer = g_Time.nTime % (nTime * 10) / nTime;

			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_Time.TimePos.x - TIME_POS_X, g_Time.TimePos.y - TIME_POS_Y, g_Time.TimePos.z);
			pVtx[1].pos = D3DXVECTOR3(g_Time.TimePos.x + TIME_POS_X, g_Time.TimePos.y - TIME_POS_Y, g_Time.TimePos.z);
			pVtx[2].pos = D3DXVECTOR3(g_Time.TimePos.x - TIME_POS_X, g_Time.TimePos.y + TIME_POS_Y, g_Time.TimePos.z);
			pVtx[3].pos = D3DXVECTOR3(g_Time.TimePos.x + TIME_POS_X, g_Time.TimePos.y + TIME_POS_Y, g_Time.TimePos.z);

			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f + (nAnswer * 0.1f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (nAnswer * 0.1f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (nAnswer * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (nAnswer * 0.1f), 1.0f);

			//頂点
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
			
			if (g_Time.nTime <= 10)
			{
				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}

			pVtx += 4;	//頂点データのポインタを4つ分進める
			nTime *= 10;
			g_Time.TimePos.x -= 40;
		}		
	}
	if (g_Time.nTime == 0)
	{
		if (GameState != GAMESTATE_GAMEOVER)
		{
			SetGameState(GAMESTATE_GAMEOVER);
		}
		//g_nTime = 60;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntScore;

	// 頂点バッファをデータストリームに設定
    pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTime);

	// ポリゴンの描画

	for (nCntScore = 0; nCntScore < MAX_TIME; nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0 + (4 * nCntScore),	//開始する頂点のインデックス
			2); //描画するプリミティブ数
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexTime(LPDIRECT3DDEVICE9 pDevice)
{	
	// 頂点情報の作成
	VERTEX_2D *pVtx;
	int nCntScore;
	int nPosMove = 0;
	g_Time.TimePos.x = SCREEN_WIDTH / 2;
	g_Time.TimePos.y = 52.0f;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	//頂点情報を設定
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore  < MAX_TIME; nCntScore++)
	{
	//頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_Time.TimePos.x - TIME_POS_X, g_Time.TimePos.y - TIME_POS_Y, g_Time.TimePos.z);
			pVtx[1].pos = D3DXVECTOR3(g_Time.TimePos.x + TIME_POS_X, g_Time.TimePos.y - TIME_POS_Y, g_Time.TimePos.z);
			pVtx[2].pos = D3DXVECTOR3(g_Time.TimePos.x - TIME_POS_X, g_Time.TimePos.y + TIME_POS_Y, g_Time.TimePos.z);
			pVtx[3].pos = D3DXVECTOR3(g_Time.TimePos.x + TIME_POS_X, g_Time.TimePos.y + TIME_POS_Y, g_Time.TimePos.z);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		//頂点
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
		g_Time.TimePos.x -= 40;
	}
	 //頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}

//=============================================================================
// タイムの加算
//=============================================================================
void AddTime(int nValue)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;
	int nCntTime;
	int nPosMove = 0;
	int nAnswer = 0;
	int nTime = 1;
	g_Time.TimePos.x = SCREEN_WIDTH / 2;
	g_Time.TimePos.y = 60.0f;

	g_Time.nTime += nValue;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime = 0; nCntTime < MAX_TIME; nCntTime++)
	{
		nAnswer = g_Time.nTime % (nTime * 10) / nTime;

		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_Time.TimePos.x - TIME_POS_X, g_Time.TimePos.y - TIME_POS_Y, g_Time.TimePos.z);
		pVtx[1].pos = D3DXVECTOR3(g_Time.TimePos.x + TIME_POS_X, g_Time.TimePos.y - TIME_POS_Y, g_Time.TimePos.z);
		pVtx[2].pos = D3DXVECTOR3(g_Time.TimePos.x - TIME_POS_X, g_Time.TimePos.y + TIME_POS_Y, g_Time.TimePos.z);
		pVtx[3].pos = D3DXVECTOR3(g_Time.TimePos.x + TIME_POS_X, g_Time.TimePos.y + TIME_POS_Y, g_Time.TimePos.z);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f + (nAnswer * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + (nAnswer * 0.1f), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + (nAnswer * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + (nAnswer * 0.1f), 1.0f);

		//頂点
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 4;	//頂点データのポインタを4つ分進める
		nTime *= 10;
		g_Time.TimePos.x -= 40;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffTime->Unlock();
}
//=============================================================================
// タイムの取得
//=============================================================================
TIME *GetTime(void)
{
	return &g_Time;
}