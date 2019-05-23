//=============================================================================
//
// メイン処理 [main.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"					//入力処理に必要
#include "xaudio2.h"

class CRenderer;					//前方宣言
class CScene;						//前方宣言

//*****************************************************************************
// ライブラリファイルのリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")		//描画処理に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")	//システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")	//入力処理に必要

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define SCREEN_WIDTH	(1280)		// ウインドウの幅
#define SCREEN_HEIGHT	(720)		// ウインドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//ポリゴンの最大数
#define MAX_SCENE			(300)

#define MAX_PRI				(8)

//爆発の最大数
#define	MAX_EXPLOSION		(256)

//テクスチャ
#define PLAYER_TEXTURE		"data/TEXTURE/Player001.png"
#define EXPLOSION_TEXTURE	"data/TEXTURE/explosion001.png"
#define EFFECT_TEXTURE		"data/TEXTURE/effect000.jpg"
#define NUMBER_TEXTURE		"data/TEXTURE/number000.png"

//弾
#define BULLET_TEXTURE		"data/TEXTURE/BULLET.png"
#define SLASH_TEXTURE		"data/TEXTURE/BULLET.png"
#define SPBULLET_TEXTURE	"data/TEXTURE/SPBULLET.png"
#define EXBULLET_TEXTURE	"data/TEXTURE/s_kama4.png"

//背景テクスチャ
#define MAX_BG				(3)
#define MAX_GAME_BG			(4)

#define TITLE_TEXTURE0		"data/TEXTURE/title000.jpg"
#define TITLE_TEXTURE1		"data/TEXTURE/title001.png"
#define TITLE_TEXTURE2		"data/TEXTURE/titlelogo.png"

#define BG_TEXTURE0			"data/TEXTURE/BG1000.jpg"
#define BG_TEXTURE1			"data/TEXTURE/bg101.png"
#define BG_TEXTURE2			"data/TEXTURE/bg102.png"
#define BG_TEXTURE3			"data/TEXTURE/bg_bar.png"

#define CLEAR_TEXTURE0		"data/TEXTURE/BG_CLEAR.jpg"
#define CLEAR_TEXTURE1		"data/TEXTURE/cloud-2.png"
#define CLEAR_TEXTURE2		"data/TEXTURE/gameclear.png"

#define GAMEOVER_TEXTURE0	"data/TEXTURE/BG_GAMEOVER.jpg"
#define GAMEOVER_TEXTURE1	"data/TEXTURE/cloud-2.png"
#define GAMEOVER_TEXTURE2	"data/TEXTURE/gameover.png"

//ライフゲージ
#define MAX_GAUGE			(3)
#define GAUGE_TEXTURE0		"data/TEXTURE/GAUGE_BASE.png"
#define GAUGE_TEXTURE1		"data/TEXTURE/LIFE_GAUGE.png"
#define GAUGE_TEXTURE2		"data/TEXTURE/GAUGE_COVER.png"

#define GAUGE2_TEXTURE0		"data/TEXTURE/GAUGE2_BASE.png"
#define GAUGE2_TEXTURE1		"data/TEXTURE/LIFE_GAUGE2.png"
#define GAUGE2_TEXTURE2		"data/TEXTURE/GAUGE2_COVER.png"
//数
#define MAX_NUMBER			(9)

//ポーズメニューの数
#define MAX_PAUSEMENU		(3)

#define PAUSE_TEXTURE		"data\\TEXTURE\\pause100.png"	//テクスチャのファイル名

#define	TEXTURE_CONTINUE	"data/TEXTURE/pause000.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_RETRY		"data/TEXTURE/pause001.png"	// 読み込むテクスチャファイル名
#define	TEXTURE_QUIT		"data/TEXTURE/pause002.png"	// 読み込むテクスチャファイル名
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
int GetFPS(void);
CScene **GetScene(void);

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

#endif