//=============================================================================
//
// タイトル画面処理 [title.cpp]
// Author : Niwa Hodaka
//
//=============================================================================
#include "title.h"
#include "pressenter.h"
#include "fade.h"
#include "input.h"
#include "titlelogo.h"
#include "sound.h"

#include "camera.h"
#include "light.h"
#include "sky.h"
#include "shadow.h"
#include "field.h"
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// グローバル変数
//*****************************************************************************
int            g_nCounterTitle;          // ランキング画面に遷移するタイミング

//=============================================================================
// 初期化処理
//=============================================================================
void InitTitle(void)
{
	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// 空の初期化処理
	InitSky();

	// 地面の初期化処理
	InitField();

	// モデルの初期化処理
	InitModel();

	// 影の初期化処理
	InitShadow();

	InitTitleLogo();

	InitPressEnter();
	SetPressEnter(D3DXVECTOR3(SCREEN_WIDTH / 2, 570.0f, 0.0f), 500, 100);

	g_nCounterTitle = 0;   // カウンターは0に
}
//=============================================================================
// 終了処理
//=============================================================================
void UninitTitle(void)
{
	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// 空の終了処理
	UninitSky();

	// 地面の終了処理
	UninitField();

	// モデルの終了処理
	UninitModel();

	// 影の初期化処理
	UninitShadow();

	UninitTitleLogo();

	UninitPressEnter();
}
//=============================================================================
// 更新処理
//=============================================================================
void UpdateTitle(void)
{
	// カメラの更新処理
	UpdateCamera();

	// ライトの終了処理
	UpdateLight();

	// 空の終了処理
	UpdateSky();

	// 地面の終了処理
	UpdateField();

	// モデルの終了処理
	UpdateModel();

	UpdateTitleLogo();

	//
	UpdatePressEnter();

	FADE fade;
	fade = GetFade();
	//bool *disp;
	//disp = GetDispPressEnter();

	g_nCounterTitle++;

	if (GetFade() == FADE_NONE)
	{// フェード状態ではない
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{// 画面遷移
			SetFade(MODE_TUTORIAL);
		}

		for (int nCntPad = 0; nCntPad < 2; nCntPad++)
		{
			for (int nCntButton = XIJS_BUTTON_4; nCntButton < XIJS_BUTTON_16; nCntButton++)
			{// 反応するボタン数の数だけ繰り返し
				if (GetXInputTrigger(nCntPad, (XIJS_BUTTON)nCntButton) == TRUE)
				{
					SetFade(MODE_TUTORIAL);
				}
			}
		}
		if (g_nCounterTitle % 1200 == 0)
		{// 一定時間がたつ
			SetFade(MODE_TITLE);
		}
	}
}
//=============================================================================
// 描画処理
//=============================================================================
void DrawTitle(void)
{
	// カメラの描画処理
	SetCamera();

	// 空の終了処理
	DrawSky();

	// 地面の終了処理
	DrawField();

	// モデルの終了処理
	DrawModel();

	DrawTitleLogo();

	DrawPressEnter();
}