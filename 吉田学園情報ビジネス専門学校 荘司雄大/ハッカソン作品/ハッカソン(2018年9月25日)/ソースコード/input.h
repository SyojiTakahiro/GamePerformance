//=============================================================================
//
// 入力処理 [input.h]
// Author : Niwa Hodaka
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// 構造体の定義
//*****************************************************************************
typedef enum
{
	DIJS_BUTTON_0 = 0,  // Xボタン
	DIJS_BUTTON_1,      // Aボタン
	DIJS_BUTTON_2,      // Bボタン
	DIJS_BUTTON_3,      // Yボタン
	DIJS_BUTTON_4,      // LBボタン
	DIJS_BUTTON_5,      // RBボタン
	DIJS_BUTTON_6,      // LTボタン
	DIJS_BUTTON_7,      // RTボタン
	DIJS_BUTTON_8,      // BACKボタン
	DIJS_BUTTON_9,      // STARTボタン
	DIJS_BUTTON_10,     // 左アナログスティック上方向入力
	DIJS_BUTTON_11,     // 左アナログスティック下方向入力
	DIJS_BUTTON_12,     // 左アナログスティック右方向入力
	DIJS_BUTTON_13,     // 左アナログスティック左方向入力
	DIJS_BUTTON_14,     // 右アナログスティック上方向入力
	DIJS_BUTTON_15,     // 右アナログスティック下方向入力
	DIJS_BUTTON_16,     // 右アナログスティック右方向入力
	DIJS_BUTTON_17,     // 右アナログスティック左方向入力
	DIJS_BUTTON_18,     // 十字キー上方向入力
	DIJS_BUTTON_19,     // 十字キー下方向入力
	DIJS_BUTTON_20,     // 十字キー右方向入力
	DIJS_BUTTON_21,     // 十字キー左方向入力
	// 以下割り当てなし
	DIJS_BUTTON_22,
	DIJS_BUTTON_23,
	DIJS_BUTTON_24,
	DIJS_BUTTON_25,
	DIJS_BUTTON_26,
	DIJS_BUTTON_27,
	DIJS_BUTTON_28,
	DIJS_BUTTON_29,
	DIJS_BUTTON_30,
	DIJS_BUTTON_31,
	DIJS_BUTTON_MAX
}PADBUTTON;

typedef enum
{
	XIJS_BUTTON_0,     // 十字キー上入力
	XIJS_BUTTON_1,     // 十字キー下入力
	XIJS_BUTTON_2,     // 十字キー左入力
	XIJS_BUTTON_3,     // 十字キー右入力
	XIJS_BUTTON_4,     // STARTボタン
	XIJS_BUTTON_5,     // BACKボタン
	XIJS_BUTTON_6,     // 左スティック押し込み入力
	XIJS_BUTTON_7,     // 右スティック押し込み入力
	XIJS_BUTTON_8,     // LBボタン
	XIJS_BUTTON_9,     // RBボタン
	XIJS_BUTTON_10,    // Aボタン
	XIJS_BUTTON_11,    // Bボタン
	XIJS_BUTTON_12,    // Xボタン
	XIJS_BUTTON_13,    // Yボタン
	XIJS_BUTTON_14,    // LTボタン
	XIJS_BUTTON_15,    // RTボタン
	XIJS_BUTTON_16,
	XIJS_BUTTON_17,
	XIJS_BUTTON_18,
	XIJS_BUTTON_19,
	XIJS_BUTTON_20,
	XIJS_BUTTON_21,
	XIJS_BUTTON_22,
	XIJS_BUTTON_23,
	XIJS_BUTTON_24,    // 左スティック上入力
	XIJS_BUTTON_25,    // 左スティック下入力
	XIJS_BUTTON_26,    // 左スティック左入力
	XIJS_BUTTON_27,    // 左スティック右入力
	XIJS_BUTTON_28,    // 右スティック上入力
	XIJS_BUTTON_29,    // 右スティック下入力
	XIJS_BUTTON_30,    // 右スティック左入力
	XIJS_BUTTON_31,    // 右スティック右入力
	XIJS_BUTTON_MAX,
}XIJS_BUTTON;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetJoyPadPress(int nButton);
bool GetJoyPadTrigger(int nButton);
bool GetJoyPadRelease(int nButton);

bool GetXInputPress(int nCntPad, XIJS_BUTTON nButton);
bool GetXInputTrigger(int nCntPad, XIJS_BUTTON nButton);
bool GetXInputRelease(int nCntPad, XIJS_BUTTON nButton);
bool GetXInputRepeat(int nCntPad, XIJS_BUTTON nButton);
void SetXInputLeftMotorSpeed(int nCntPad, WORD wLeftMotorSpeed);
void SetXInputRightMotorSpeed(int nCntPad, WORD wRightMotorSpeed);

BYTE GetXInputLeftTrigger(int nCntPad);
BYTE GetXInputRightTrigger(int nCntPad);
float GetXInputLeftAxiz(int nCntPad);
float GetXInputRightAxiz(int nCntPad);
SHORT GetXInputThumbLX(int nCntPad);
SHORT GetXInputThumbLY(int nCntPad);
SHORT GetXInputThumbRX(int nCntPad);
SHORT GetXInputThumbRY(int nCntPad);

#endif