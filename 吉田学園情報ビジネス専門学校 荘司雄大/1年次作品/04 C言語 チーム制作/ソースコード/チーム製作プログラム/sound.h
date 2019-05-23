//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Ozaki Daisuke
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*************************************
// サウンドファイル
//*************************************
typedef enum //ラベルと数と並びを揃える
{
	SOUND_LABEL_BGM_TITLE = 0,	// BGMタイトル
	SOUND_LABEL_BGM_TUTORIAL,	// BGMチュートリアル
	SOUND_LABEL_BGM_GAME,		// BGMゲーム
	SOUND_LABEL_BGM_CLEAR,		// BGMクリア
	SOUND_LABEL_BGM_GAMEOVER,	// BGMゲームオーバー
	SOUND_LABEL_BGM_RANKING,	// BGMランキング
	SOUND_LABEL_BGM_PIZZA,		// ピザ状態
	SOUND_LABEL_BGM_SINZOU,		// 心臓音
	SOUND_LABEL_BGM_SINDENZU,	// 心電図音
	SOUND_LABEL_SE_GOAL,		// ゴール
	SOUND_LABEL_SE_DECIDE,		// 決定音
	SOUND_LABEL_SE_PAUSE,		// ポーズ音
	SOUND_LABEL_SE_SELECT,		// セレクト音
	SOUND_LABEL_SE_CANCEL,		// キャンセル音１
	SOUND_LABEL_SE_SAKEBI1,		// 叫び声１
	SOUND_LABEL_SE_SAKEBI2,		// 叫び声２
	SOUND_LABEL_SE_SAKEBI3,		// 叫び声３
	SOUND_LABEL_SE_ENZIN,		// エンジン音
	SOUND_LABEL_SE_BIKU,		// バイク音
	SOUND_LABEL_SE_TABERU,		// 食べる音
	SOUND_LABEL_SE_NIMIKOMI,	// 飲み込み音
	SOUND_LABEL_SE_DORO,		// 泥音
	SOUND_LABEL_SE_KABE,		// 壁接触音
	SOUND_LABEL_SE_WIND,		// 風音
	SOUND_LABEL_SE_YUKIARASHI,	// 雪嵐音
	SOUND_LABEL_SE_CHIME1,		// チャイム１
	SOUND_LABEL_SE_CHIME2,		// チャイム２
	SOUND_LABEL_SE_CHIME3,		// チャイム３
	SOUND_LABEL_SE_CHIME4,		// チャイム４
	SOUND_LABEL_SE_KLAXON1,		// クラクション１
	SOUND_LABEL_SE_KLAXON2,		// クラクション２
	SOUND_LABEL_SE_GEPPU,		// げっぷ
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
