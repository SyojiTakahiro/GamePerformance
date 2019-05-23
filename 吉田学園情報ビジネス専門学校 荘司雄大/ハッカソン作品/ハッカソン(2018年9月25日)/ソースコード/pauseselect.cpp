//=============================================================================
//
// ポーズ項目処理 [pauseselect.cpp]
// Author : Niwa Hodaka
//
//=============================================================================
#include "pauseselect.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME1        "data/TEXTURE/pause000.png" // 読み込むテクスチャファイル名(CONTINUE)
#define TEXTURE_NAME2        "data/TEXTURE/pause001.png" // 読み込むテクスチャファイル名(RETRY)
#define TEXTURE_NAME3        "data/TEXTURE/pause002.png" // 読み込むテクスチャファイル名(QUIT)
#define MAX_PAUSE_SELECT     (RESTART_MAX)               // ポーズ選択できる項目の数
#define PAUSESELECT_POS_X    (470)                       // ポーズ項目の左上Ｘ座標
#define PAUSESELECT_POS_Y    (220)                       // ポーズ項目左上Ｙ座標
#define PAUSESELECT_WIDTH    (800)                       // ポーズ項目の幅
#define PAUSESELECT_HEIGHT   (280)                       // ポーズ項目の高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexPauseSelect(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTexturePauseSelect[MAX_PAUSE_SELECT] = {}; // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPauseSelect = NULL;                 // 頂点バッファへのポインタ
PAUSE                   g_aPause[MAX_PAUSE_SELECT];                   // ポーズ項目
RESTART                 g_restart;                                    // 遷移する先
float                   g_col;                                        // 色の変化に使用
int                     g_Select;                                     // 選択に使用

//=============================================================================
// 初期化処理
//=============================================================================
void InitPauseSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_aPause[0].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);  // 色を初期化
	g_aPause[0].select = PAUSESELECT_SELECT;              // スタートはCONTINUEから
	// 値の初期化
	for (int nCntPauseSelect = 1; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{// 項目の数だけ繰り返し
		g_aPause[nCntPauseSelect].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);  // 色を初期化
		g_aPause[nCntPauseSelect].select = PAUSESELECT_NONE;                // 選択されていない状態にする
	}
	g_col = 0.01f;                // 色の変化を初期化
	g_restart = RESTART_CONTINUE; // ゲーム再開状態にしておく
	g_Select = 0;                 // 選択されている番号はCONTINUEに

	// 頂点情報の設定
	MakeVertexPauseSelect(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPauseSelect(void)
{
	// テクスチャの開放
	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{// 読み込んだテクスチャの数だけ繰り返し
		if (g_pTexturePauseSelect[nCntPauseSelect] != NULL)
		{
			g_pTexturePauseSelect[nCntPauseSelect]->Release();
			g_pTexturePauseSelect[nCntPauseSelect] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffPauseSelect != NULL)
	{
		g_pVtxBuffPauseSelect->Release();
		g_pVtxBuffPauseSelect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePauseSelect(void)
{
	// 選択項目移動処理
	for (int nCntPad = 0; nCntPad < 2;nCntPad++)
	{
		if (GetKeyboardTrigger(DIK_S) == true ||
			GetXInputTrigger(nCntPad, XIJS_BUTTON_0) == TRUE || GetXInputTrigger(nCntPad, XIJS_BUTTON_24) == true
			|| GetXInputRepeat(nCntPad, XIJS_BUTTON_0) == TRUE || GetXInputRepeat(nCntPad, XIJS_BUTTON_24) == true)
		{// 下方向の入力がされた
			g_aPause[g_Select].select = PAUSESELECT_NONE;
			g_aPause[(g_Select + (MAX_PAUSE_SELECT - 1)) % MAX_PAUSE_SELECT].select = PAUSESELECT_SELECT;
			g_Select = (g_Select + (MAX_PAUSE_SELECT - 1)) % MAX_PAUSE_SELECT;
		}
		if (GetKeyboardTrigger(DIK_W) == true ||
			GetXInputTrigger(nCntPad, XIJS_BUTTON_1) || GetXInputTrigger(nCntPad, XIJS_BUTTON_25) == true
			|| GetXInputRepeat(nCntPad, XIJS_BUTTON_1) || GetXInputRepeat(nCntPad, XIJS_BUTTON_25) == true)
		{// 上方向の入力がされた
			g_aPause[g_Select].select = PAUSESELECT_NONE;
			g_aPause[(g_Select + 1) % MAX_PAUSE_SELECT].select = PAUSESELECT_SELECT;
			g_Select = (g_Select + 1) % MAX_PAUSE_SELECT;
		}
	}

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffPauseSelect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{// 項目の数だけ繰り返し
		if (g_aPause[nCntPauseSelect].select == PAUSESELECT_SELECT)
		{// 選択されていたら
			g_aPause[nCntPauseSelect].col.r += g_col;
			g_aPause[nCntPauseSelect].col.g += g_col;
			g_aPause[nCntPauseSelect].col.b += g_col;

			if (g_aPause[nCntPauseSelect].col.r < 0.2f || g_aPause[nCntPauseSelect].col.r >= 1.0f)
			{// 色が既定の値に達した
				g_col *= -1;
			}
		}
		else if (g_aPause[nCntPauseSelect].select == PAUSESELECT_NONE)
		{// 選択されていなかったら
			g_aPause[nCntPauseSelect].col.r = 0.2f;
			g_aPause[nCntPauseSelect].col.g = 0.2f;
			g_aPause[nCntPauseSelect].col.b = 0.2f;
		}
		// 頂点カラーの更新
		pVtx[0].col = g_aPause[nCntPauseSelect].col;
		pVtx[1].col = g_aPause[nCntPauseSelect].col;
		pVtx[2].col = g_aPause[nCntPauseSelect].col;
		pVtx[3].col = g_aPause[nCntPauseSelect].col;

		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffPauseSelect->Unlock();

	// 画面遷移
	if (GetXInputTrigger(0,XIJS_BUTTON_11) == TRUE || GetKeyboardTrigger(DIK_RETURN) == true)
	{// 決定ボタンが押された
		FADE fade;
		fade = GetFade();

		if (fade == FADE_NONE)
		{// フェード状態でない
			switch (g_Select)
			{
			case RESTART_CONTINUE:
				g_restart = RESTART_CONTINUE; // ゲーム再開状態に
				break;
			case RESTART_RETRY:
				g_restart = RESTART_RETRY;    // ゲームやり直し状態に
				break;
			case RESTART_QUIT:
				g_restart = RESTART_QUIT;     // タイトル遷移状態に
				break;
			}
		}
	}
	if (GetJoyPadTrigger(DIJS_BUTTON_9) == TRUE || GetKeyboardTrigger(DIK_P) == true)
	{// ポーズが解除された
		g_aPause[0].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);  // 色を初期化
		g_aPause[0].select = PAUSESELECT_SELECT;              // スタートはCONTINUEから
		for (int nCntPauseSelect = 1; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
		{// 項目の数だけ繰り返し
			g_aPause[nCntPauseSelect].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);  // 色を初期化
			g_aPause[nCntPauseSelect].select = PAUSESELECT_NONE;                // 選択されていない状態にする
		}
		g_Select = 0;      // 選択されている番号はCONTINUEに
		VERTEX_2D *pVtx;   // 頂点情報へのポインタ

		// 頂点バッファをロックし,頂点データへのポインタを取得
		g_pVtxBuffPauseSelect->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
		{
			pVtx[0].col = g_aPause[nCntPauseSelect].col;
			pVtx[1].col = g_aPause[nCntPauseSelect].col;
			pVtx[2].col = g_aPause[nCntPauseSelect].col;
			pVtx[3].col = g_aPause[nCntPauseSelect].col;
			pVtx += 4;
		}
		// 頂点バッファをアンロックする
		g_pVtxBuffPauseSelect->Unlock();
	}
}
//=============================================================================
// タイトル画面
//=============================================================================
void DrawPauseSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPauseSelect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{// 項目の数だけ繰り返し
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePauseSelect[nCntPauseSelect]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPauseSelect * 4, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexPauseSelect(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み(CONTINUE)
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME1,
		&g_pTexturePauseSelect[0]);

	// テクスチャの読み込み(RETRY)
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME2,
		&g_pTexturePauseSelect[1]);

	// テクスチャの読み込み(QUIT)
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME3,
		&g_pTexturePauseSelect[2]);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PAUSE_SELECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPauseSelect,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffPauseSelect->Lock(0, 0, (void**)&pVtx, 0);

	float Pos = 0.0f;

	for (int nCntPauseSelect = 0; nCntPauseSelect < MAX_PAUSE_SELECT; nCntPauseSelect++)
	{// 項目の数だけ繰り返し
		// 頂点座標
		pVtx[0].pos = D3DXVECTOR3(PAUSESELECT_POS_X, PAUSESELECT_POS_Y  + Pos, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(PAUSESELECT_WIDTH, PAUSESELECT_POS_Y  + Pos, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(PAUSESELECT_POS_X, PAUSESELECT_HEIGHT + Pos, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(PAUSESELECT_WIDTH, PAUSESELECT_HEIGHT + Pos, 0.0f);

		// 頂点テクスチャ
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = g_aPause[nCntPauseSelect].col;
		pVtx[1].col = g_aPause[nCntPauseSelect].col;
		pVtx[2].col = g_aPause[nCntPauseSelect].col;
		pVtx[3].col = g_aPause[nCntPauseSelect].col;

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
		Pos += 110;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffPauseSelect->Unlock();
}
//=============================================================================
// 項目の取得
//=============================================================================
RESTART GetRestart(void)
{
	return g_restart;
}