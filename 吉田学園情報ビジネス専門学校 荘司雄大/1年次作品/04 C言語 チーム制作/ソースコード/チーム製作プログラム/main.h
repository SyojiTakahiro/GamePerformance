//=============================================================================
//
// メイン処理 [main.h]
// Author :  Meguro Mikiya
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"						//描画処理に必要
#define DIRECTINPUT_VERSION (0x0800)	// 警告対処用
#include "dinput.h"						//入力処理に必要
#include "xaudio2.h"					//音楽データに必要
#include "XInput.h"
//*****************************************************************************
// ライブラリファイル
//*****************************************************************************
#pragma comment (lib, "d3d9.lib")		//描画処理に必要
#pragma comment (lib, "d3dx9.lib")		//d3d9の拡張ライブラリ
#pragma comment (lib, "dxguid.lib")		//DirectXコンポーネント(部品)使用に必要
#pragma comment (lib, "winmm.lib")		//システム時間取得に必要
#pragma comment (lib, "dinput8.lib")	//入力処理に必要
#pragma comment (lib, "xinput.lib")	//入力処理に必要
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(1280)	// ウインドウの幅
#define SCREEN_HEIGHT	(720)	// ウインドウの高さ

// 2Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 法線ベクトル / 頂点カラー )
#define FVF_VERTEX_2D			(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)	

// 3Ｄポリゴン頂点フォーマット( 頂点座標[3D] / 法線ベクトル / 頂点カラー )
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)


//*****************************************************************************
// 構造体定義
//*****************************************************************************
//画面遷移用
typedef enum
{
	MODE_TITLE = 0,		//タイトル
	MODE_TUTORIAL,		//チュートリアル
	MODE_GAME,			//ゲーム
	MODE_RESULT,		//リザルト
	MODE_RANKING,		//ランキング
	MODE_GAMEOVER,		//ゲームオーバー
	MODE_MAX
}MODE;
// 2Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//1.0fで固定
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

// 3Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点座標
	D3DXVECTOR3 nor;	//	法線ベクトル
	//float rhw;			// 座標変換用係数(1.0fで固定)
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャ座標
} VERTEX_3D;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif