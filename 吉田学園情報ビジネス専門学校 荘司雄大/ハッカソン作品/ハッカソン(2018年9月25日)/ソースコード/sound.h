//=============================================================================
//
// サウンド処理 [sound.h]
// Author : Ozaki
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*************************************
// サウンドファイル
//*************************************
typedef enum
{
	SOUND_LABEL_BGMTITLE = 0,		// BGMTITLE
	SOUND_LABEL_BGMTUTORIAL,			// BGMTUTORIAL
	SOUND_LABEL_BGMGAME,			// BGMGAME
	SOUND_LABEL_BGMRESULT,			// BGMRESULT
	SOUND_LABEL_SE_DAMAGE,			// ダメージ音
	SOUND_LABEL_SE_WEAKPUNCH,		// 弱パンチ
	SOUND_LABEL_SE_STORONGPUNCH,	// 強パンチ
	SOUND_LABEL_SE_DOWN,			// 倒れる音
	SOUND_LABEL_SE_LANDING,			// 着地
	SOUND_LABEL_SE_JUMP,			// 着地
	SOUND_LABEL_SE_MISS,			// 攻撃避け
	//SOUND_LABEL_SE_1P_ATTACK,		// 1P攻撃
	//SOUND_LABEL_SE_1P_DAMAGE,		// 1Pダメージ
	//SOUND_LABEL_SE_1P_DEATH,		// 1P死亡
	//SOUND_LABEL_SE_1P_ULTIMATE,		// 1P必殺技
	//SOUND_LABEL_SE_2P_ATTACK,		// 2P攻撃
	//SOUND_LABEL_SE_2P_DAMAGE,		// 2Pダメージ
	//SOUND_LABEL_SE_2P_DEATH,		// 2P死亡
	//SOUND_LABEL_SE_2P_ULTIMATE,		// 2P必殺技
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
