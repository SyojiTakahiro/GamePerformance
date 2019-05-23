//=============================================================================
//
// 入力処理 [input.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// 
//*****************************************************************************
class CInput
{
public:
	CInput();
	~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
protected:
	LPDIRECTINPUTDEVICE8 m_pDevice;
	static LPDIRECTINPUT8 m_pInput;
private:
	
	
};

class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetKeyboardPress(int nKey);
	bool GetKeyboardTrigger(int nKey);
private:
	BYTE m_aKeyState[256];
	BYTE m_aKeyStateTrigger[256];
};


class CInputJoypad : public CInput
{
public:
	CInputJoypad();
	~CInputJoypad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nButton);
	bool GetTrigger(int nButton);

	float GetLeftStickAxiz(void);
	float GetRightStickAxiz(void);
	float GetRadian(void);

	typedef enum
	{
		DIJS_BUTTON_1 = 0,
		DIJS_BUTTON_2,
		DIJS_BUTTON_3,
		DIJS_BUTTON_4,

		DIJS_BUTTON_5,
		DIJS_BUTTON_6,
		DIJS_BUTTON_7,
		DIJS_BUTTON_8,

		DIJS_BUTTON_9,
		DIJS_BUTTON_10,
		DIJS_BUTTON_11,
		DIJS_BUTTON_12,

		DIJS_BUTTON_13,
		DIJS_BUTTON_14,
		DIJS_BUTTON_15,
		DIJS_BUTTON_16,

		DIJS_BUTTON_17,
		DIJS_BUTTON_18,
		DIJS_BUTTON_19,
		//左スティック
		DIJS_BUTTON_20,	//↑
		DIJS_BUTTON_21,	//↓
		DIJS_BUTTON_22,	//→
		DIJS_BUTTON_23,	//←
		//右スティック
		DIJS_BUTTON_24,	//↑
		DIJS_BUTTON_25,	//↓
		DIJS_BUTTON_26,	//→
		DIJS_BUTTON_27,	//←
		//十字ボタン
		DIJS_BUTTON_28,	//↑
		DIJS_BUTTON_29,	//↓
		DIJS_BUTTON_30,	//→
		DIJS_BUTTON_31,	//←

		DIJS_BUTTON_MAX
	}DIJS_BUTTON;

private:
	BYTE m_aState[DIJS_BUTTON_MAX];
	BYTE m_aStateTrigger[DIJS_BUTTON_MAX];

	float m_LeftAxizX;
	float m_LeftAxizY;
	float m_RightAxizX;
	float m_RightAxizY;

	float m_Radian;
};

#endif