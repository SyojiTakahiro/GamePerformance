//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : Mikiya Meguro
//
//=============================================================================
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "rank.h"
#include "time.h"
#include "title.h"
#include "rankinglogo.h"
#include "rankingbg.h"
#include "polygon.h"
#include "inputx.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_SCORE		"data/TEXTURE/Number000.png"	// 読み込むテクスチャファイル名
#define POLYGON_POS_X			(30)						//頂点座標Xの移動量
#define POLYGON_POS_Y			(30)						//頂点座標Yの移動量
#define TEX_POS_X_INIT			(1.0f)						//テクスチャ座標Uの初期位置
#define TEX_POS_Y_INIT			(1.0f)						//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)						//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)						//テクスチャ座標U右
#define TEX_TOP					(0.0f)						//テクスチャ座標V上
#define TEX_BOT					(1.0f)						//テクスチャ座標V下
#define MAX_RANKING				(5)							//ランキングの数
#define MAX_NUM					(3)
#define NUM_WHIDTH				(50)
#define NUM_HIGHT				(88)
#define POS_WHIDTH				(70)
#define NUMPOS_WHIDTH			(55)
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureRanking = NULL;		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;		// 頂点バッファへのポインタ
int						g_nRanking;						// スコア
int						g_aScore[MAX_RANKING];
Ranking					g_aRanking[MAX_RANKING];	
D3DXVECTOR3				g_RankingPos;
int						g_nCounterTimer;
int						g_nRankNum;
int						g_nCntRankTimer;
int						g_RankChange;
int						g_nUpdateScore;
JoyState g_JoyStateRanking;
//=============================================================================
// 初期化処理
//=============================================================================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_JoyStateRanking.nJoypadState = INPUT_JOYSTATE_NOTPUSH;

	g_RankingPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nRanking = 0;
	g_nCounterTimer = 0;
	g_nCntRankTimer = 0;
	g_RankChange = 0;

	int nScoreChange, nScoreChange2;

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		g_aRanking[nCntRanking].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRanking[nCntRanking].col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);
	}

	TIME *pScore;
	//スコアの取得
	pScore = GetTime();
	g_nRanking = pScore->nTime;
	g_nUpdateScore = g_nRanking;

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{//ランキングを更新
		if (g_aScore[nCnt] <= g_nRanking)
		{
			if (g_aScore[nCnt + 1] >= g_nRanking)
			{//ランキングを入れ替え
				//値をとっておく
				nScoreChange = g_aScore[nCnt];
				//ゲーム後のスコアを代入
				g_aScore[nCnt] = g_nRanking;
				g_nRankNum = nCnt;
				//下のスコアをとっておく
				nScoreChange2 = g_aScore[nCnt - 1];
				//下のスコアに塗り替えたスコアを代入
				g_aScore[nCnt - 1] = nScoreChange;
				//下のスコアを渡しておく
				nScoreChange = nScoreChange2;
				for (nCnt = nCnt - 1; nCnt >= 0; nCnt--)
				{	//渡されたスコアを代入
					nScoreChange2 = nScoreChange;
					//さらに下のスコアをとっておく
					nScoreChange = g_aScore[nCnt - 1];
					//下のスコアをさらに下のスコアに代入
					g_aScore[nCnt -1] = nScoreChange2;
				}
				break;
			}
			if (nCnt == MAX_RANKING - 1)
			{
				if (g_aScore[nCnt] <= g_nRanking)
				{
					//値をとっておく
					nScoreChange = g_aScore[nCnt];
					//ゲーム後のスコアを代入
					g_aScore[nCnt] = g_nRanking;
					g_nRankNum = nCnt;
					//下のスコアをとっておく
					nScoreChange2 = g_aScore[nCnt - 1];
					//下のスコアに塗り替えたスコアを代入
					g_aScore[nCnt - 1] = nScoreChange;
					//下のスコアを渡しておく
					nScoreChange = nScoreChange2;
					for (nCnt = nCnt - 1; nCnt >= 0; nCnt--)
					{	//渡されたスコアを代入
						nScoreChange2 = nScoreChange;
						//さらに下のスコアをとっておく
						nScoreChange = g_aScore[nCnt - 1];
						//下のスコアをさらに下のスコアに代入
						g_aScore[nCnt - 1] = nScoreChange2;
					}
					break;
				}
			}
		}	
	}

	g_nCounterTimer = 0;
	g_nCntRankTimer = 0;
	g_RankChange = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
		TEXTURE_SCORE,							// ファイルの名前
		&g_pTextureRanking);					// テクスチャへのポインタ

	// 頂点情報の作成
	MakeVertexRanking(pDevice);
	//2D初期化
	InitRank();
	InitRankingLogo();
	InitRankingBG();
	//
	//3D空間の初期化
	//
	// カメラの初期化処理
	//InitCamera();
	// ライトの初期化処理
	//InitLight();
	//メッシュフィールドの初期化
	//InitMeshField();
	InitPolygon();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitRanking(void)
{
	// テクスチャの開放
	if (g_pTextureRanking != NULL)
	{
		g_pTextureRanking->Release();
		g_pTextureRanking = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}

	//順位の終了処理
	UninitRank();
	//ロゴの終了処理
	UninitRankingLogo();
	UninitRankingBG();
	//カメラの終了処理
	//UninitCamera();
	//ライトの終了処理
	//UninitLight();
	//メッシュフィールドの終了処理
	//UninitMeshField();
	UninitPolygon();

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateRanking(void)
{
	FADE fade;
	fade = GetFade();

	JoyState pJoyState = GetJoystate();
	XINPUT_STATE state;
	XInputGetState(0, &state);

	TIME *pScore;
	//スコアの取得
	pScore = GetTime();

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	//エンターキー
	if (GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE || (state.Gamepad.wButtons & XINPUT_GAMEPAD_B  && pJoyState.bConnectionJoypad == true) && fade == FADE_NONE)
	{
		pScore = 0;
		SetFade(MODE_TITLE);
	}
	g_nCounterTimer++;
	if (g_nCounterTimer >= 600)
	{
		pScore = 0;
		SetFade(MODE_TITLE);
	}
	//色変え
	int nUpdateScore = NULL;
	int nAnswer = 0;
	int nKeisan = 1;

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{//ランキングを更新
		if (g_aScore[nCnt] == g_nUpdateScore)
		{//更新したスコアは何番目か
			nUpdateScore = nCnt;
			break;
		}
		else if (nCnt == 4)
		{//更新したスコアは何番目か
			nUpdateScore = 4;
		}
	}
	g_aRanking[nUpdateScore].pos.x = SCREEN_WIDTH / 2 + NUM_WHIDTH;
	g_aRanking[nUpdateScore].pos.y = 550.0f - (NUM_HIGHT * nUpdateScore);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * MAX_NUM * nUpdateScore;	//頂点データのポインタを進める
	for (int nCntScore = 0; nCntScore < MAX_NUM; nCntScore++)
	{
		nAnswer = g_aScore[nUpdateScore] % (nKeisan * 10) / nKeisan;
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aRanking[nUpdateScore].pos.x - POLYGON_POS_X, g_aRanking[nUpdateScore].pos.y - POLYGON_POS_Y, g_aRanking[nUpdateScore].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aRanking[nUpdateScore].pos.x + POLYGON_POS_X, g_aRanking[nUpdateScore].pos.y - POLYGON_POS_Y, g_aRanking[nUpdateScore].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aRanking[nUpdateScore].pos.x - POLYGON_POS_X, g_aRanking[nUpdateScore].pos.y + POLYGON_POS_Y, g_aRanking[nUpdateScore].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aRanking[nUpdateScore].pos.x + POLYGON_POS_X, g_aRanking[nUpdateScore].pos.y + POLYGON_POS_Y, g_aRanking[nUpdateScore].pos.z);
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

		if (nUpdateScore == 4)
		{
			if (g_RankChange == 0)
			{
				{	//頂点カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				}
			}
			else if (g_RankChange == 1)
			{
				{	//頂点カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
			}
		}
		else
		{
			if (g_RankChange == 0)
			{
				{	//頂点カラー
					pVtx[0].col = D3DXCOLOR(0.2f, 0.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(0.2f, 0.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(0.2f, 0.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(0.2f, 0.0f, 1.0f, 1.0f);
				}
			}
			else if (g_RankChange == 1)
			{
				{	//頂点カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
			}
		}
		////頂点カラー
		//pVtx[0].col = D3DXCOLOR(0.2f, 0.0f, 1.0f, 1.0f);
		//pVtx[1].col = D3DXCOLOR(0.2f, 0.0f, 1.0f, 1.0f);
		//pVtx[2].col = D3DXCOLOR(0.2f, 0.0f, 1.0f, 1.0f);
		//pVtx[3].col = D3DXCOLOR(0.2f, 0.0f, 1.0f, 1.0f);

		//if (nUpdateScore == 4)
		//{
		//	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		//	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		//	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		//	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		//}



		pVtx += 4;	//頂点データのポインタを4つ分進める
		nKeisan *= 10;
		g_RankingPos.x -= POS_WHIDTH;
		g_aRanking[nUpdateScore].pos.x -= NUMPOS_WHIDTH;
	}


	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	//点滅タイマー
	g_nCntRankTimer++;

	if (g_nCntRankTimer >= 10)
	{
		if (g_RankChange == 0)
		{
			g_RankChange = 1;
		}
		else if (g_RankChange == 1)
		{
			g_RankChange = 0;
		}
		g_nCntRankTimer = 0;
	}


	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();

	//順位
	UpdateRank();
	//ロゴ
	UpdateRankingLogo();
	UpdateRankingBG();
	// カメラの更新処理
	//UpdateCamera();
	// ライトの更新処理
	//UpdateLight();
	//メッシュフィールドの更新処理
	//UpdateMeshField();

	UpdatePolygon();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawRanking(void)
{
	// カメラの設定
	//SetCamera(0);
	DrawRankingBG();

	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntScore;

	//アルファテスト(透明色を描画しないように)
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRanking);
	// ポリゴンの描画
	for (nCntScore = 0; nCntScore < MAX_NUM * MAX_RANKING; nCntScore++)
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0 + (4 * nCntScore),	//開始する頂点のインデックス
			2); //描画するプリミティブ数
	}
	//順位描画処理
	DrawRank();
	//ロゴ描画処理
	//DrawRankingLogo();

	//レンダーステートの設定を元に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	//メッシュフィールドの描画処理
	//DrawMeshField();
	DrawPolygon();

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexRanking(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;
	int nCntScore;
	int nPosMove = 0;
	int nAnswer = 0;
	int nKeisan = 1;
	int nCntRanking;


	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM * MAX_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	//頂点情報を設定
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{//配置
		g_aRanking[nCntRanking].pos.x = SCREEN_WIDTH / 2 + NUM_WHIDTH;
		g_aRanking[nCntRanking].pos.y = 558.0f - (NUM_HIGHT * nCntRanking);

		for (nCntScore = 0; nCntScore < MAX_NUM; nCntScore++)
		{
			nAnswer = g_aScore[nCntRanking] % (nKeisan * 10) / nKeisan;

			//頂点座標
			pVtx[0].pos = D3DXVECTOR3(g_aRanking[nCntRanking].pos.x - POLYGON_POS_X, g_aRanking[nCntRanking].pos.y - POLYGON_POS_Y, g_aRanking[nCntRanking].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aRanking[nCntRanking].pos.x + POLYGON_POS_X, g_aRanking[nCntRanking].pos.y - POLYGON_POS_Y, g_aRanking[nCntRanking].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aRanking[nCntRanking].pos.x - POLYGON_POS_X, g_aRanking[nCntRanking].pos.y + POLYGON_POS_Y, g_aRanking[nCntRanking].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aRanking[nCntRanking].pos.x + POLYGON_POS_X, g_aRanking[nCntRanking].pos.y + POLYGON_POS_Y, g_aRanking[nCntRanking].pos.z);

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

			pVtx += 4;	//頂点データのポインタを4つ分進める
			nKeisan *= 10;
			g_RankingPos.x -= POS_WHIDTH;
			g_aRanking[nCntRanking].pos.x -= NUMPOS_WHIDTH;
		}
		nKeisan = 1;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffRanking->Unlock();
}
//=============================================================================
// ランキングの取得
//=============================================================================
void SetRanking(int nRanking)
{
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{
		g_aScore[nCntRanking] = nRanking * (1 + nCntRanking);
	}
}