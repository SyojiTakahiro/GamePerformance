//=============================================================================
//
// 入力処理 [input.h]
// Author :	Shun Yokomichi
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
HRESULT InitJoycon(HINSTANCE hInstance, HWND hWnd);											// ジョイコン初期化処理
void UpdateJoycon(void);																	// ジョイコン更新処理
BOOL CALLBACK EnumJoystickCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);	// コールバック(デバイス作成)に必要な宣言
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext);		// 軸を決める
bool GetJoyControllerPress(int nKey);														// マウスプレス
bool GetJoyControllerTrigger(int nKey);														// マウストリガー
DIJOYSTATE2 GetStick(void);

#endif