//----------------------------------------------------
//	Diutil.cpp
//----------------------------------------------------
#include "Diutil.h"

#define PAD_0				0
#define PAD_1				1
#define PAD_2				2
#define PAD_3				3
#define PAD_4				4
#define PAD_5				5
#define PAD_6				6
#define PAD_7				7
#define PAD_8				8
#define PAD_9				9
#define PAD_10				10

#define DEADZONE		2500			// 各軸の25%を無効ゾーンとする
#define RANGE_MAX		1000			// 有効範囲の最大値
#define RANGE_MIN		-1000			// 有効範囲の最小値

LPDIRECTINPUT7			pDInput = NULL;
LPDIRECTINPUTDEVICE7	pDIDeviceJ = NULL;
LPDIRECTINPUTDEVICE7	pDIDeviceK = NULL;

static UINT Device = FALSE;

/*-------------------------------------------
	ジョイスティック・デバイスを列挙（コールバック関数）
---------------------------------------------*/
BOOL CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi,LPVOID /*pvRef*/)
{
	HRESULT hr;
	hr = pDInput->CreateDeviceEx(lpddi->guidInstance, IID_IDirectInputDevice7, (void**)&pDIDeviceJ, NULL);
	if (FAILED(hr))
		return DIENUM_CONTINUE;	// 次のデバイスを列挙

	return DIENUM_STOP;	// デバイスの列挙を中止
}

/*-------------------------------------------
	Direct Input 初期化
---------------------------------------------*/
bool InitDInput(HINSTANCE hInstApp,HWND hWnd)
{
	HRESULT hr;

	hr = DirectInput8Create(hInstApp, DIRECTINPUT_VERSION, IID_IDirectInput7, (void**)&pDInput, NULL);
	if (FAILED(hr))		return false;  // DirectInput7の作成に失敗

	HRESULT DirectInputCreateEx(
		HINSTANCE	hinst,
		DWORD	dwVersion,
		REFIID	riidltf,
		LPVOID*	ppvOut,
		LPUNKNOWN	punkOuter
	);

	//ジョイスティックを探す
	pDInput->EnumDevices(DI8DEVTYPE_JOYSTICK,(LPDIENUMDEVICESCALLBACK)GetJoystickCallback,NULL,DIEDFL_ATTACHEDONLY);
	if (pDIDeviceJ != NULL)
	{
		//データフォーマットを設定
		hr = pDIDeviceJ->SetDataFormat(&c_dfDIJoystick);	// ジョイスティック用のデータ・フォーマットを設定
		if (FAILED(hr))		return false; // データフォーマットの設定に失敗

		//モードを設定（フォアグラウンド＆非排他モード）
		hr = pDIDeviceJ->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr))		return false; // モードの設定に失敗

		//軸の値の範囲を設定
		//X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
		//(max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
		//差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドは、関係なし)
		DIPROPRANGE diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize	= sizeof(diprg); 
		diprg.diph.dwHeaderSize	= sizeof(diprg.diph); 
		diprg.diph.dwHow	= DIPH_BYOFFSET; 
		diprg.lMin	= RANGE_MIN;
		diprg.lMax	= RANGE_MAX;
		//X軸の範囲を設定
		diprg.diph.dwObj	= DIJOFS_X; 
		hr = pDIDeviceJ->SetProperty(DIPROP_RANGE, &diprg.diph);
		if(FAILED(hr))		return FALSE;
		//Y軸の範囲を設定
		diprg.diph.dwObj	= DIJOFS_Y;
		hr = pDIDeviceJ->SetProperty(DIPROP_RANGE, &diprg.diph);
		if(FAILED(hr))		return FALSE;

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize	= sizeof(dipdw.diph);
		dipdw.diph.dwHow		= DIPH_BYOFFSET;
		dipdw.dwData			= DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_X;
		hr = pDIDeviceJ->SetProperty( DIPROP_DEADZONE, &dipdw.diph);
		if(FAILED(hr))		return FALSE;
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj		= DIJOFS_Y;
		hr = pDIDeviceJ->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		if(FAILED(hr))		return FALSE;
		
		//ジョイスティック入力制御開始
		pDIDeviceJ->Acquire();
		
		Device = TRUE;
	}
	else
	{

		//キーボード用にデバイスオブジェクトを作成
		hr = pDInput->CreateDeviceEx(GUID_SysKeyboard, IID_IDirectInputDevice7, (void**)&pDIDeviceK, NULL); 
		if (FAILED(hr))
		{
			OutputDebugString("CreateDeviceEx NO\n");
			return false;  // デバイスの作成に失敗
		}

		//データフォーマットを設定 キーボード用のデータ・フォーマットを設定
		hr = pDIDeviceK->SetDataFormat(&c_dfDIKeyboard);	
		if (FAILED(hr))
		{
			OutputDebugString("SetDateFormat NO\n");
			return false; // デバイスの作成に失敗
		}

		//モードを設定（フォアグラウンド＆非排他モード）
		hr = pDIDeviceK->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		if (FAILED(hr))
		{
			OutputDebugString("SetCooperativeLevel NO\n");
			return false; // モードの設定に失敗
		}

		//キーボード入力制御開始
		pDIDeviceK->Acquire();
	}

	return true;
}

/*-------------------------------------------
	DirectInputの解放処理
--------------------------------------------*/
bool ReleaseDInput(void)
{
	// DirectInputのデバイスを解放
	RELEASE(pDIDeviceJ);
	RELEASE(pDIDeviceK);
	RELEASE(pDInput);

	return true;
}

/*--------------------------------------------
	押されているキーの状態を取得
--------------------------------------------*/
DWORD GetInputDeviceState(void)
{
	HRESULT		hr;
	DWORD		dwInput = 0x00000000l;

	if (!pDInput)
		return dwInput;
		
	if ( Device == TRUE )
	{
		/* ジョイスティックの状態を取得 */
		DIJOYSTATE	dijs;

		// ジョイスティックにポールをかける
		pDIDeviceJ->Poll();

		// デバイス状態を読み取る
		hr = pDIDeviceJ->GetDeviceState(sizeof(DIJOYSTATE),&dijs);
		if (hr == DIERR_INPUTLOST){
			pDIDeviceJ->Acquire();
			return dwInput;
		}

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* y-axis (forward)
		if(dijs.lY < 0)						dwInput |= BUTTON_UP;
		//* y-axis (backward)
		if(dijs.lY > 0)						dwInput |= BUTTON_DOWN;
		//* x-axis (left)
		if(dijs.lX < 0)						dwInput |= BUTTON_LEFT;
		//* x-axis (right)
		if(dijs.lX > 0)						dwInput |= BUTTON_RIGHT;
		//* Ａボタン
		if(dijs.rgbButtons[PAD_0] & 0x80)		dwInput |= BUTTON_A;
		//* Ｂボタン
		if(dijs.rgbButtons[PAD_1] & 0x80)		dwInput |= BUTTON_B;
		//* Ｃボタン
		if(dijs.rgbButtons[PAD_2] & 0x80)		dwInput |= BUTTON_C;
		//* Ｘボタン
		if(dijs.rgbButtons[PAD_3] & 0x80)		dwInput |= BUTTON_X;
		//* Ｙボタン
		if(dijs.rgbButtons[PAD_4] & 0x80)		dwInput |= BUTTON_Y;
		//* Ｚボタン
		if(dijs.rgbButtons[PAD_5] & 0x80)		dwInput |= BUTTON_Z;
	    //* Ｌボタン
	    if(dijs.rgbButtons[PAD_6] & 0x80)		dwInput |= BUTTON_L;
		//* Ｒボタン
		if(dijs.rgbButtons[PAD_7] & 0x80)		dwInput |= BUTTON_R;
		//* ＳＴＡＲＴボタン
		if(dijs.rgbButtons[PAD_8] & 0x80)		dwInput |= BUTTON_START;
		//* Ｍボタン
		if(dijs.rgbButtons[PAD_9] & 0x80)		dwInput |= BUTTON_M;
	}
	else
	{
		/* キーボードの状態を取得 */
		BYTE diKeyState[256];

		hr = pDIDeviceK->GetDeviceState(256,diKeyState);
		if (hr == DIERR_INPUTLOST){
			pDIDeviceK->Acquire();
			return dwInput;
		}

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* y-axis (forward)
		if(diKeyState[DIK_UP] & 0x80)			dwInput |= BUTTON_UP;
		//* y-axis (backward)
		if(diKeyState[DIK_DOWN] & 0x80)			dwInput |= BUTTON_DOWN;
		//* x-axis (left)
		if(diKeyState[DIK_LEFT] & 0x80)			dwInput |= BUTTON_LEFT;
		//* x-axis (right)
		if(diKeyState[DIK_RIGHT] & 0x80)		dwInput |= BUTTON_RIGHT;
		//* Ａボタン
		if(diKeyState[DIK_A] & 0x80)			dwInput |= BUTTON_A;
		//* Ｂボタン
		if(diKeyState[DIK_B] & 0x80)			dwInput |= BUTTON_B;
		//* Ｃボタン
		if(diKeyState[DIK_C] & 0x80)			dwInput |= BUTTON_C;
		//* Ｘボタン
		if(diKeyState[DIK_X] & 0x80)			dwInput |= BUTTON_X;
		//* Ｙボタン
		if(diKeyState[DIK_Y] & 0x80)			dwInput |= BUTTON_Y;
		//* Ｚボタン
		if(diKeyState[DIK_Z] & 0x80)			dwInput |= BUTTON_Z;
		//* Ｌボタン
		if(diKeyState[DIK_L] & 0x80)			dwInput |= BUTTON_L;
		//* Ｒボタン
		if(diKeyState[DIK_R] & 0x80)			dwInput |= BUTTON_R;
		//* ＳＴＡＲＴボタン
		if(DIK_RETURN & 0x80)					dwInput |= BUTTON_START;
		//* Ｍボタン
		if(diKeyState[DIK_M] & 0x80)			dwInput |= BUTTON_M;
	}

	return dwInput;
}