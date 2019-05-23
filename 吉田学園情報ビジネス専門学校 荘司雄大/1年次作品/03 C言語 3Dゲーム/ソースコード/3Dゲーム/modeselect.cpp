//=============================================================================
//
// ポーズメニュー処理 [pauseselect.cpp]
// Author : 
//
//=============================================================================
#include "input.h"
#include "fade.h"
#include "modeselect.h" 
#include "player.h"
#include "t.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_CONTINUE		"data/TEXTURE/1P.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RETRY			"data/TEXTURE/2P.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_QUIT			"data/TEXTURE/tutorial.png"	// 読み込むテクスチャファイル名
#define MODESELECT_POS_X			(120)						//頂点座標Xの移動量
#define MODESELECT_POS_Y			(30)						//頂点座標Yの移動量
//#define TEX_PS_X_INIT			(1.0f)						//テクスチャ座標Uの初期位置
//#define TEX_PS_Y_INIT			(1.0f)						//テクスチャ座標Vの初期位置
#define TEX_LEFT				(0.0f)						//テクスチャ座標U左
#define TEX_RIGHT				(1.0f)						//テクスチャ座標U右
#define TEX_TOP					(0.0f)						//テクスチャ座標V上
#define TEX_BOT					(1.0f)						//テクスチャ座標V下
#define MAX_PAUSEMENU			(3)							//メニューの数

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexModeSelect(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureModeSelect[MAX_PAUSEMENU] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffModeSelect = NULL;					// 頂点バッファへのポインタ
MODESELECT				g_aModeMenu[MAX_PAUSEMENU];					// ポーズメニュー
D3DXVECTOR3				g_ModeMenuPos;
GAMEMODESELECT			g_SelectModeT = GAMEMODESELECT_NONE;
int						g_nGameSelect;
//=============================================================================
// 初期化処理
//=============================================================================
void InitModeSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float fMenuPosY = 0;
	g_nGameSelect = 0;
	g_ModeMenuPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SelectModeT = GAMEMODESELECT_NONE;

	g_aModeMenu[0].pos = D3DXVECTOR3(650.0f, 470.0f + (fMenuPosY), 0.0f);
	g_aModeMenu[0].col = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f);
	g_aModeMenu[0].selectgame = SELECTGAMETYPE_SELECT;
	fMenuPosY += 150;
	for (int nCnt = 1; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		g_aModeMenu[nCnt].pos = D3DXVECTOR3(650.0f, 400.0f + (fMenuPosY), 0.0f);
		g_aModeMenu[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aModeMenu[nCnt].selectgame = SELECTGAMETYPE_NONE;
		fMenuPosY += 80;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
		TEXTURE_CONTINUE,						// ファイルの名前
		&g_pTextureModeSelect[0]);				// テクスチャへのポインタ


	D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
		TEXTURE_RETRY,							// ファイルの名前
		&g_pTextureModeSelect[1]);				// テクスチャへのポインタ

	D3DXCreateTextureFromFile(pDevice,			// デバイスへのポインタ
		TEXTURE_QUIT,							// ファイルの名前
		&g_pTextureModeSelect[2]);				// テクスチャへのポインタ

												// 頂点情報の作成
	MakeVertexModeSelect(pDevice);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitModeSelect(void)
{
	for (int nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{// テクスチャの開放
		if (g_pTextureModeSelect[nCnt] != NULL)
		{
			g_pTextureModeSelect[nCnt]->Release();
			g_pTextureModeSelect[nCnt] = NULL;
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
	// 頂点情報の作成
	VERTEX_2D *pVtx;
	FADE fade;
	fade = GetFade();
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffModeSelect->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_DOWN) == true && fade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_SELECT);
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_NONE;
		g_nGameSelect = (g_nGameSelect + 1) % 3;
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_SELECT;
	}
	if (GetKeyboardTrigger(DIK_UP) == true && fade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_SELECT);
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_NONE;
		g_nGameSelect = (g_nGameSelect + 2) % 3;
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_SELECT;
	}
	if (GetKeyboardTrigger(DIK_S) == true && fade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_SELECT);
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_NONE;
		g_nGameSelect = (g_nGameSelect + 1) % 3;
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_SELECT;
	}
	if (GetKeyboardTrigger(DIK_W) == true && fade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_SELECT);
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_NONE;
		g_nGameSelect = (g_nGameSelect + 2) % 3;
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_SELECT;
	}


	//エンターキー
	if (GetKeyboardTrigger(DIK_SPACE) == true && fade == FADE_NONE )
	{
		if (g_nGameSelect == 0)
		{
			PlaySound(SOUND_LABEL_SE_CANCEL);
			StopSound(SOUND_LABEL_BGM000);
			g_SelectModeT = GAMEMODESELECT_SINGLEMODE;
		}
		else if (g_nGameSelect == 1)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			StopSound(SOUND_LABEL_BGM000);
			g_SelectModeT = GAMEMODESELECT_2PMODE;
		}
		else if (g_nGameSelect == 2)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			StopSound(SOUND_LABEL_BGM000);
			g_SelectModeT = GAMEMODESELECT_TUTORIAL;
		}

		switch (g_SelectModeT)
		{
		case GAMEMODESELECT_SINGLEMODE:
		{
			SetFade(MODE_SINGLEGAME);
			break;
		}
		case GAMEMODESELECT_2PMODE:
		{
			SetFade(MODE_GAME);

			break;
		}
		case GAMEMODESELECT_TUTORIAL:
		{
			SetFade(MODE_T);
			StopSound(SOUND_LABEL_BGM001);
			break;
		}
		}
	}

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		g_nGameSelect = 0;
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_SELECT;
		g_aModeMenu[1].selectgame = SELECTGAMETYPE_NONE;
		g_aModeMenu[2].selectgame = SELECTGAMETYPE_NONE;
	}

	for (int nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		if (g_aModeMenu[nCnt].selectgame == SELECTGAMETYPE_SELECT)
		{//選択中の色	
			g_aModeMenu[nCnt].col = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f);
		}
		else
		{//未選択の色
			g_aModeMenu[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		//頂点カラー
		pVtx[0].col = g_aModeMenu[nCnt].col;
		pVtx[1].col = g_aModeMenu[nCnt].col;
		pVtx[2].col = g_aModeMenu[nCnt].col;
		pVtx[3].col = g_aModeMenu[nCnt].col;
		pVtx += 4;
	}
	// 頂点バッファをアンロックする
	g_pVtxBuffModeSelect->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawModeSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCnt;

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffModeSelect, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureModeSelect[nCnt]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0 + (4 * nCnt),	//開始する頂点のインデックス
			2); //描画するプリミティブ数
	}

}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexModeSelect(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点情報の作成
	VERTEX_2D *pVtx;
	int nCnt;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PAUSEMENU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffModeSelect,
		NULL);

	//頂点情報を設定
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffModeSelect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x - MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y - MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x + MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y - MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x - MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y + MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x + MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y + MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(TEX_LEFT, TEX_TOP);
		pVtx[1].tex = D3DXVECTOR2(TEX_RIGHT, TEX_TOP);
		pVtx[2].tex = D3DXVECTOR2(TEX_LEFT, TEX_BOT);
		pVtx[3].tex = D3DXVECTOR2(TEX_RIGHT, TEX_BOT);
		//頂点
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラー
		pVtx[0].col = g_aModeMenu[g_nGameSelect].col;
		pVtx[1].col = g_aModeMenu[g_nGameSelect].col;
		pVtx[2].col = g_aModeMenu[g_nGameSelect].col;
		pVtx[3].col = g_aModeMenu[g_nGameSelect].col;

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffModeSelect->Unlock();
}

//=============================================================================
// ポーズの設定
//=============================================================================
void SetModeSelect(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCnt;
	float fMenuPosY = 0;
	VERTEX_2D *pVtx;//頂点情報へのポインタ
					//頂点情報を設定
					//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffModeSelect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		//頂点座標
		pVtx[0].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x - MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y - MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x + MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y - MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x - MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y + MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x + MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y + MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		g_aModeMenu[nCnt].pos = pos;
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(TEX_LEFT, TEX_TOP);
		pVtx[1].tex = D3DXVECTOR2(TEX_RIGHT, TEX_TOP);
		pVtx[2].tex = D3DXVECTOR2(TEX_LEFT, TEX_BOT);
		pVtx[3].tex = D3DXVECTOR2(TEX_RIGHT, TEX_BOT);
		//頂点
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		g_aModeMenu[nCnt].col = col;

		fMenuPosY += 100;

		pVtx += 4;	//頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffModeSelect->Unlock();
}

//=============================================================================
// セレクトモードの取得
//=============================================================================
GAMEMODESELECT *GetModeMode(void)
{
	return &g_SelectModeT;
}