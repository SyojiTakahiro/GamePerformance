//=============================================================================
//
// モードセレクトの処理 [modeselect.cpp]
// Author : Niwa Hodaka
//
//=============================================================================
#include "modeselect.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXTURE_NAME1        "data/TEXTURE/gamestart.png" // 読み込むテクスチャファイル名(GAMESTART)
#define TEXTURE_NAME2        "data/TEXTURE/tutorial.png"  // 読み込むテクスチャファイル名(TUTORIAL)
#define TEXTURE_NAME3        "data/TEXTURE/ranking.png"   // 読み込むテクスチャファイル名(RANKING)
#define TEXTURE_NAME4        "data/TEXTURE/title.png"     // 読み込むテクスチャファイル名(TITLE)
#define MODESELECT_POS_X    (470)                         // モード項目の左上Ｘ座標
#define MODESELECT_POS_Y    (170)                         // モード項目左上Ｙ座標
#define MODESELECT_WIDTH    (800)                         // モード項目の幅
#define MODESELECT_HEIGHT   (230)                         // モード項目の高さ

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexModeSelect(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureModeSelect[MODESTART_MAX] = {};  // テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffModeSelect = NULL;               // 頂点バッファへのポインタ
MODESELECT              g_aModeSelect[MODESTART_MAX];              // モード項目
float                   g_Modecol;                                 // 色の変化に使用
int                     g_ModeSelect;                              // 選択に使用

//=============================================================================
// 初期化処理
//=============================================================================
void InitModeSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_aModeSelect[0].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);  // 色を初期化
	g_aModeSelect[0].state = MODESTATE_SELECT;                 // スタートはMODESTART_GAMEから

	// 値の初期化
	for (int nCntModeSelect = 1; nCntModeSelect < MODESTART_MAX; nCntModeSelect++)
	{// 項目の数だけ繰り返し
		g_aModeSelect[nCntModeSelect].col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);  // 色を初期化
		g_aModeSelect[nCntModeSelect].state = MODESTATE_NONE;                   // 選択されていない状態にする
	}
	g_Modecol = 0.01f;    // 色の変化を初期化
	g_ModeSelect = 0;     // 選択されている番号はMODESTART_GAMEに

	// 頂点情報の設定
	MakeVertexModeSelect(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModeSelect(void)
{
	// テクスチャの開放
	for (int nCntModeSelect = 0; nCntModeSelect < MODESTART_MAX; nCntModeSelect++)
	{// 読み込んだテクスチャの数だけ繰り返し
		if (g_pTextureModeSelect[nCntModeSelect] != NULL)
		{
			g_pTextureModeSelect[nCntModeSelect]->Release();
			g_pTextureModeSelect[nCntModeSelect] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffModeSelect != NULL)
	{
		g_pVtxBuffModeSelect->Release();
		g_pVtxBuffModeSelect = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateModeSelect(void)
{
	// 選択項目移動処理
	if (GetJoyPadTrigger(DIJS_BUTTON_18) == TRUE || GetKeyboardTrigger(DIK_W) == true)
	{// 上方向の入力がされた
		g_aModeSelect[g_ModeSelect].state = MODESTATE_NONE;
		g_aModeSelect[(g_ModeSelect + (MODESTART_MAX - 1)) % MODESTART_MAX].state = MODESTATE_SELECT;
		g_ModeSelect = (g_ModeSelect + (MODESTART_MAX - 1)) % MODESTART_MAX;
	}
	if (GetJoyPadTrigger(DIJS_BUTTON_19) == TRUE || GetKeyboardTrigger(DIK_S) == true)
	{// 下方向の入力がされた
		g_aModeSelect[g_ModeSelect].state = MODESTATE_NONE;
		g_aModeSelect[(g_ModeSelect + 1) % MODESTART_MAX].state = MODESTATE_SELECT;
		g_ModeSelect = (g_ModeSelect + 1) % MODESTART_MAX;
	}

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffModeSelect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntModeSelect = 0; nCntModeSelect < MODESTART_MAX; nCntModeSelect++)
	{// 項目の数だけ繰り返し
		if (g_aModeSelect[nCntModeSelect].state == MODESTATE_SELECT)
		{// 選択されていたら
			g_aModeSelect[nCntModeSelect].col.r += g_Modecol;   // 点滅させる
			g_aModeSelect[nCntModeSelect].col.g += g_Modecol;   // 点滅させる
			g_aModeSelect[nCntModeSelect].col.b += g_Modecol;   // 点滅させる

			if (g_aModeSelect[nCntModeSelect].col.r < 0.2f || g_aModeSelect[nCntModeSelect].col.r >= 1.0f)
			{// 色が既定の値に達した
				g_Modecol *= -1;  // 色の変化を反転
			}
		}
		else if (g_aModeSelect[nCntModeSelect].state == MODESTATE_NONE)
		{// 選択されていなかったら
			g_aModeSelect[nCntModeSelect].col.r = 0.2f;   // 色を固定
			g_aModeSelect[nCntModeSelect].col.g = 0.2f;   // 色を固定
			g_aModeSelect[nCntModeSelect].col.b = 0.2f;   // 色を固定
		}
		// 頂点カラーの更新
		pVtx[0].col = g_aModeSelect[nCntModeSelect].col;
		pVtx[1].col = g_aModeSelect[nCntModeSelect].col;
		pVtx[2].col = g_aModeSelect[nCntModeSelect].col;
		pVtx[3].col = g_aModeSelect[nCntModeSelect].col;

		pVtx += 4;  // ポインタを進める
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffModeSelect->Unlock();

	// 画面遷移
	if (GetJoyPadTrigger(DIJS_BUTTON_2) == TRUE || GetKeyboardTrigger(DIK_RETURN) == true)
	{// 決定ボタンが押された
		FADE fade;
		fade = GetFade();

		if (fade == FADE_NONE)
		{// フェード状態でない
			switch (g_ModeSelect)
			{
			case MODESTART_GAME:
				SetFade(MODE_GAME);      // ゲーム画面へ移行
				break;
			case MODESTART_TUTORIAL:
				SetFade(MODE_TUTORIAL);  // チュートリアル画面へ移行
				break;
			case MODESTART_RANKING:
				SetFade(MODE_RANKING);   // ランキング画面へ移行
				break;
			case MODESTART_TITLE:
				SetFade(MODE_TITLE);     // タイトル画面へ移行
				break;
			}
		}
	}
}
//=============================================================================
// タイトル画面
//=============================================================================
void DrawModeSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffModeSelect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntModeSelect = 0; nCntModeSelect < MODESTART_MAX; nCntModeSelect++)
	{// 項目の数だけ繰り返し
	    // テクスチャの設定
		pDevice->SetTexture(0, g_pTextureModeSelect[nCntModeSelect]);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntModeSelect * 4, NUM_POLYGON);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexModeSelect(LPDIRECT3DDEVICE9 pDevice)
{
	// テクスチャの読み込み(GAMESTART)
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME1,
		&g_pTextureModeSelect[0]);

	// テクスチャの読み込み(TUTORIAL)
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME2,
		&g_pTextureModeSelect[1]);

	// テクスチャの読み込み(RANKING)
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME3,
		&g_pTextureModeSelect[2]);

	// テクスチャの読み込み(TITLE
	D3DXCreateTextureFromFile(pDevice,
		TEXTURE_NAME4,
		&g_pTextureModeSelect[3]);

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MODESTART_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffModeSelect,
		NULL);

	VERTEX_2D *pVtx;   // 頂点情報へのポインタ

    // 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffModeSelect->Lock(0, 0, (void**)&pVtx, 0);

	float Pos = 0.0f;  // 座標をずらすのに使用

	for (int nCntModeSelect = 0; nCntModeSelect < MODESTART_MAX; nCntModeSelect++)
	{// 項目の数だけ繰り返し
	    // 頂点座標
		pVtx[0].pos = D3DXVECTOR3(MODESELECT_POS_X, MODESELECT_POS_Y + Pos, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(MODESELECT_WIDTH, MODESELECT_POS_Y + Pos, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(MODESELECT_POS_X, MODESELECT_HEIGHT + Pos, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(MODESELECT_WIDTH, MODESELECT_HEIGHT + Pos, 0.0f);

		// 頂点テクスチャ
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = g_aModeSelect[nCntModeSelect].col;
		pVtx[1].col = g_aModeSelect[nCntModeSelect].col;
		pVtx[2].col = g_aModeSelect[nCntModeSelect].col;
		pVtx[3].col = g_aModeSelect[nCntModeSelect].col;

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;   // ポインタを進める
		Pos += 110;  // 座標をずらす
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffModeSelect->Unlock();
}