//=============================================================================
//
// 入力処理 [input.cpp]
// Author :	Shun Yokomichi
//
//=============================================================================
#include "dinput.h"

//*************************************************************************************************************************
// マクロ定義
//*************************************************************************************************************************
#define	NUM_KEY_MAX			(256)				// キーの最大数
#define JOYKEY_MAX			(12)				// ジョイキーの最大数

//*************************************************************************************************************************
// グローバル変数
//*************************************************************************************************************************
LPDIRECTINPUT8				g_pInput = NULL;					// DirerctInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8		g_pDevKeyboard = NULL;				// 入力デバイス（キーボード）へのポインタ
BYTE						g_aKeyState[NUM_KEY_MAX];			// キーボードの入力情報（プレス情報）
BYTE						g_aKeyStateTrigger[NUM_KEY_MAX];	// キーボードの入力情報（トリガー情報）
BYTE						g_aKeyStateRelease[NUM_KEY_MAX];	// キーボードの入力情報（リリース情報）
LPDIRECTINPUT8				g_pInputJoycon = NULL;				// ダイレクトインプット
LPDIRECTINPUTDEVICE8		g_lpDIJoycon = NULL;				// ダイレクトインプット デバイス
DIDEVCAPS					g_diDevCaps;						// ジョイスティックの能力
DIJOYSTATE2					g_JoyKeyState;						// ジョイキーの入力情報(プレス情報)
DIJOYSTATE2					g_JoyKeyStateTrigger;				// ジョイキーの入力情報(トリガー情報)
DIJOYSTATE2					g_InputStick;						// スティックの入力情報

//=========================================================================================================================
// キーボードの初期化
//=========================================================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=========================================================================================================================
// キーボードの終了処理
//=========================================================================================================================
void UninitKeyboard(void)
{
	// 入力デバイス(キーボード)の開放
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();	// キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// 入力デバイス(ジョイスティック)の開放
	if (g_lpDIJoycon != NULL)
	{
		g_lpDIJoycon->Unacquire();	// キーボードへのアクセス権を放棄
		g_lpDIJoycon->Release();
		g_lpDIJoycon = NULL;
	}

	// DirectInputオブジェクトの開放
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}

}

//=========================================================================================================================
// キーボードの更新処理
//=========================================================================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey];
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];
			g_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();		// キーボードへのアクセス権を獲得
	}
}

//=========================================================================================================================
// キーボードの入力情報(プレス情報)を取得
//=========================================================================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//=========================================================================================================================
// キーボードの入力情報(トリガー情報)を取得
//=========================================================================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=========================================================================================================================
// キーボードの入力情報(リリース情報)を取得
//=========================================================================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

/*=========================================================
ジョイコントローラーを取得(デバイスの作成)
=========================================================*/
BOOL CALLBACK EnumJoystickCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	HRESULT hr;

	hr = g_pInputJoycon->CreateDevice(pdidInstance->guidInstance, &g_lpDIJoycon, NULL);

	if (FAILED(hr))
	{
		return DIENUM_CONTINUE;
	}

	return DIENUM_STOP;
}


/*=========================================================
ジョイスティックを取得(デバイスの作成)
===========================================================*/
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext)
{
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.diph.dwObj = pdidoi->dwType;
	diprg.lMin = 0 - 1000;										// 上または左ボタンが押されたときの値
	diprg.lMax = 0 + 1000;										// 下または右ボタンが押されたときの値

	if (FAILED(g_lpDIJoycon->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return DIENUM_STOP;
	}
	return DIENUM_CONTINUE;
}


//=========================================================
// ダイレクトインプットの初期化処理
//===========================================================
HRESULT InitJoycon(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr;

	// DirectInputの作成
	hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&g_pInputJoycon, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, "DirectInput8オブジェクトの作成に失敗", "Direct Input Error", MB_OK);
		return hr;
	}
	// デバイスを列挙して作成
	hr = g_pInputJoycon->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoystickCallback,
		NULL, DIEDFL_ATTACHEDONLY);
	if (g_lpDIJoycon == NULL)
	{// コントローラーがささっていない時
		if (FAILED(hr) || g_lpDIJoycon == NULL)
		{
			return hr;
		}
	}
	// データ形式を設定
	if (g_lpDIJoycon != NULL)
	{// コントローラーがささっている時
		hr = g_lpDIJoycon->SetDataFormat(&c_dfDIJoystick2);
	}
	if (g_lpDIJoycon == NULL)
	{// コントローラーがささっていない時
		if (FAILED(hr))
		{
			MessageBox(NULL, "c_dfDIJoystick2形式の設定に失敗", "Direct Input Error", MB_OK);
			return hr;
		}
	}
	// 協調モードを設定（フォアグラウンド＆非排他モード）
	if (g_lpDIJoycon != NULL)
	{// コントローラーがささっている時
		hr = g_lpDIJoycon->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	}
	if (g_lpDIJoycon == NULL)
	{// コントローラーがささっていない時
		if (FAILED(hr))
		{
			MessageBox(NULL, "フォアグラウンド＆非排他モードの設定に失敗", "Direct Input Error", MB_OK);
			return hr;
		}
	}
	// コールバック関数を使って各軸のモードを設定
	if (g_lpDIJoycon != NULL)
	{// コントローラーがささっている時
		hr = g_lpDIJoycon->EnumObjects(EnumAxesCallback, NULL, DIDFT_AXIS);
	}
	if (g_lpDIJoycon == NULL)
	{// コントローラーがささっていない時
		if (FAILED(hr))
		{
			MessageBox(NULL, "軸モードの設定に失敗", "Direct Input Error", MB_OK);
			return hr;
		}
	}
	// コントローラーへのアクセス権を獲得(入力制御開始)
	g_lpDIJoycon->Acquire();
	return S_OK;
}


/*=============================================================================
ジョイコントローラーの更新処理
=============================================================================*/
void UpdateJoycon(void)
{
	DIJOYSTATE2  aJoyKeyState;			//コントローラーの入力待ち
	int nCntKey;
	if (g_lpDIJoycon != NULL)
	{// コントローラーがささっている時
	 // 入力されたものを引き継ぐ
		g_lpDIJoycon->Poll();

		// 入力デバイスからデータを取得
		if (SUCCEEDED(g_lpDIJoycon->GetDeviceState(sizeof(DIJOYSTATE2), &aJoyKeyState)))
		{
			for (nCntKey = 0; nCntKey < JOYKEY_MAX; nCntKey++)
			{
				// キーボード(トリガー)の入力情報保存
				g_JoyKeyStateTrigger.rgbButtons[nCntKey] =
					(g_JoyKeyState.rgbButtons[nCntKey] ^ aJoyKeyState.rgbButtons[nCntKey]) & aJoyKeyState.rgbButtons[nCntKey];
				// キーボード(プレス)の入力情報保存
				g_JoyKeyState.rgbButtons[nCntKey] = aJoyKeyState.rgbButtons[nCntKey];
			}
		}
		else
		{
			g_lpDIJoycon->Acquire(); //キーボードのアクセス権を獲得
		}
	}
}


/*=============================================================================
ジョイコントローラーの入力情報(プレス情報)を取得
=============================================================================*/
bool GetJoyControllerPress(int nKey)
{
	return(g_JoyKeyState.rgbButtons[nKey] & 0x80) ? true : false;
}


/*=============================================================================
ジョイコントローラーの入力情報(トリガー情報)を取得
=============================================================================*/
bool GetJoyControllerTrigger(int nKey)
{
	return(g_JoyKeyStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}


/*=============================================================================
アナログスティックの取得
=============================================================================*/
DIJOYSTATE2 GetStick(void)
{
	if (g_lpDIJoycon != NULL)
	{// コントローラーがささっている時
		g_lpDIJoycon->GetDeviceState(sizeof(DIJOYSTATE2), &g_InputStick);
	}
	return g_InputStick;
}

