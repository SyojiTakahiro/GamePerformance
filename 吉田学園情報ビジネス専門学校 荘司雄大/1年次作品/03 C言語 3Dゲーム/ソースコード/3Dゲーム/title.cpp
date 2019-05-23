//=============================================================================
//
// TITLE [title.cpp]
// Author :Takahiro
//
//=============================================================================
#include "title.h"
#include "titlelogo.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "pressenter.h"
#include "rank.h"
#include "ranking.h"
#include "rankingbg.h"
#include "score.h"
#include "camera.h"
#include "light.h"
#include "meshField.h"
#include "building.h"
#include "modeselect.h"
#include "selecttitle.h"
#include "model.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TITLE_TEXTURENAME		"data/TEXTURE/title.jpg"			//背景画像
#define TITLE_POS_X			(0)					//X座標
#define TITLE_POS_Y			(0)					//Y座標
#define TITLE_WIDTH			(SCREEN_WIDTH)		//画面の幅
#define TITLE_HEIGHT		(SCREEN_HEIGHT)		//画面の高さ

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTitle = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;

//GAMEMODESELECT g_GameState = GAMEMODESELECT_NONE;

int g_nCntRankingTimer = 0;

bool g_bModeSelect;

GAMEMODESELECT	*g_ModeSelect;
//=============================================================================
// メイン関数
//=============================================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TITLE_TEXTURENAME, &g_pTextureTitle);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	VERTEX_2D*pVtx;

	//頂点バッファをロック
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標
	pVtx[0].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, TITLE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(TITLE_POS_X, TITLE_POS_Y + TITLE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(TITLE_POS_X + TITLE_WIDTH, TITLE_POS_Y + TITLE_HEIGHT, 0.0f);

	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx[0].tex = D3DXVECTOR2(0, 0);
	pVtx[1].tex = D3DXVECTOR2(1, 0);
	pVtx[2].tex = D3DXVECTOR2(0, 1);
	pVtx[3].tex = D3DXVECTOR2(1, 1);

	//アンロック
	g_pVtxBuffTitle->Unlock();

	// カメラの初期化処理
	InitCamera();
	// ライトの初期化処理
	InitLight();

	InitBuilding();

	InitModel();

	InitTitleLogo();

	InitMeshField();

	InitPressEnter();

	InitSelectTitle();

	SetModel(D3DXVECTOR3(200, 0, -160), 0);

	SetModel(D3DXVECTOR3(280, 0, -160), 1);

	g_nCntRankingTimer = 0;

	g_bModeSelect = false;

	PlaySound(SOUND_LABEL_BGM000);
}

void UninitTitle(void)
{
	//テクスチャの破棄
	if (g_pTextureTitle != NULL)
	{
		g_pTextureTitle->Release();
		g_pTextureTitle = NULL;
	}
	//頂点の破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

	// カメラの終了処理
	UninitCamera();
	// ライトの終了処理
	UninitLight();

	UninitBuilding();

	UninitModel();

	UninitTitleLogo();

	UninitSelectTitle();

	UninitMeshField();

	UninitPressEnter();
}

void UpdateTitle(void)
{
	FADE fade;
	fade = GetFade();
	g_nCntRankingTimer++;

	//エンターキー
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		PlaySound(SOUND_LABEL_SE_PAUSE);
		g_bModeSelect = g_bModeSelect ? false : true;
	}
	g_ModeSelect = GetModeMode();

	//if (GetKeyboardTrigger(DIK_RETURN) == true && fade == FADE_NONE)
	//{
	//	UpdateSelectTitle();
	//	/*SetFade(MODE_GAME);
	//	StopSound(SOUND_LABEL_BGM000);
	//	PlaySound(SOUND_LABEL_SE_DECIDE);*/
	//}

	/*if (GetKeyboardTrigger(DIK_O) == true && fade == FADE_NONE)
	{
		SetFade(MODE_SINGLEGAME);
		StopSound(SOUND_LABEL_BGM000);
		PlaySound(SOUND_LABEL_SE_DECIDE);
	}

	if (GetKeyboardTrigger(DIK_P) == true && fade == FADE_NONE)
	{
		SetFade(MODE_T);
		StopSound(SOUND_LABEL_BGM000);
		PlaySound(SOUND_LABEL_SE_DECIDE);
	}*/

	/*if (g_ScreenChangeCnt >= 800)
	{
		if (g_ScreenChange == 0)
		{
			g_ScreenChange = 1;
		}
		else if (g_ScreenChange == 1)
		{
			g_ScreenChange = 0;
		}

		UpdateRankingBG();

		g_ScreenChangeCnt = 0;
	}
	UpdatePressEnter();*/

	/*if (g_nCntRankingTimer >= 600)
	{
		SetScore(0);
		SetFade(MODE_RANKING);
		g_nCntRankingTimer = 0;
	}
*/

	// カメラの更新処理
	UpdateCamera();
	// ライトの更新処理
	UpdateLight();

	UpdateBuilding();

	UpdateModel();

	UpdateMeshField();
	UpdateTitleLogo();
	UpdatePressEnter();

	if (g_bModeSelect == false)
	{
		
	}
	else
	{
		UpdateSelectTitle();
		if (*g_ModeSelect == GAMEMODESELECT_SINGLEMODE)
		{
			g_bModeSelect = g_bModeSelect ? false : true;
			*g_ModeSelect = GAMEMODESELECT_NONE;
		}
	}

}

void DrawTitle(void)
{

	// カメラの設定
	SetCamera();

	////デバイスの取得
	//pDevice = GetDevice();

	//pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

	//pDevice->SetTexture(0, g_pTextureTitle);

	//pDevice->SetFVF(FVF_VERTEX_2D);
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	DrawBuilding();

	DrawModel();

	DrawMeshField();

	DrawTitleLogo();

	DrawPressEnter();

	if (g_bModeSelect == true)
	{
		DrawSelectTitle();
		DrawModeSelect();
	}
}