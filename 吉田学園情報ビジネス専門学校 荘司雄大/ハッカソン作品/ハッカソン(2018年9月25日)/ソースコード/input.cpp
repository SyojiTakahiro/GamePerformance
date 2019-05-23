//=============================================================================
//
// 入力処理 [input.cpp]
// Author : Niwa Hodaka
//
//=============================================================================
#include "input.h"

typedef struct
{
	XINPUT_STATE m_State;     // ゲームパッドの状態
	bool m_bConnected;        // 接続されたかどうか
}CONTROLER_STATE;

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX	     (256)	 // キーの最大数
#define DI_JOY_I_INPUT   (1000)  // スティックの方向入力受付を行う範囲
#define MAX_DEVICE       (4)
#define REPEAT_FRAME     (20)    // リピート情報を起動し始めるフレーム数の合計
#define REPEAT_TRIGGER   (7)     // リピート情報を起動するフレームの間隔


#define MIN_GAMEPAD_LEFT_THUMB_X  (300)     // 左スティックの横方向の入力を受け付ける軸の最小値
#define MIN_GAMEPAD_LEFT_THUMB_Y  (25000)   // 左スティックの縦方向の入力を受け付ける軸の最小値

#define MIN_GAMEPAD_RIGHT_THUMB_X (300)     // 右スティックの横方向の入力を受け付ける軸の最小値
#define MIN_GAMEPAD_RIGHT_THUMB_Y (25000)   // 右スティックの縦方向の入力を受け付ける軸の最小値

#define MIN_GAMEPAD_LEFT_TRIGGER  (130)     // LTボタンの入力を受け付ける値の最小値
#define MIN_GAMEPAD_RIGHT_TRIGGER (130)     // RTボタンの入力を受け付ける値の最小値

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8 g_pInput = NULL;              // DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;  // 入力デバイス(キーボード)へのポインタ
LPDIRECTINPUTDEVICE8 g_pDevJoypad = NULL;    // 入力デバイス(ジョイパッド)へのポインタ
CONTROLER_STATE g_XInput[MAX_DEVICE];        // XInputの状態
XINPUT_VIBRATION g_aVibrarion[MAX_DEVICE];   // 振動機能設定用

BYTE g_aKeyState[NUM_KEY_MAX];               // キーボードの入力情報(プレス情報)
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];        // キーボードの入力情報(トリガー情報)
BYTE g_aKeyStateRelease[NUM_KEY_MAX];        // キーボードの入力情報(リリース情報)
BYTE g_aJoyState[DIJS_BUTTON_MAX];           // ジョイパッドの入力情報(プレス情報)
BYTE g_aJoyStateTrigger[DIJS_BUTTON_MAX];    // ジョイパッドの入力情報(トリガー情報)
BYTE g_aJoyStateRelease[DIJS_BUTTON_MAX];    // ジョイパッドの入力情報(リリース情報)

DWORD            m_wButtons[MAX_DEVICE];                                // ボタンの状態
BYTE             m_bLeftTrigger[MAX_DEVICE];                            // LTボタンの状態
BYTE             m_bRightTrigger[MAX_DEVICE];                           // RTボタンの状態
SHORT            m_sThumbLX[MAX_DEVICE];                                // 左スティックのX軸
SHORT            m_sThumbLY[MAX_DEVICE];                                // 左スティックのY軸
SHORT            m_sThumbRX[MAX_DEVICE];                                // 右スティックのX軸
SHORT            m_sThumbRY[MAX_DEVICE];                                // 右スティックのY軸
BYTE             m_aGamePadState[MAX_DEVICE][XIJS_BUTTON_MAX];          // ゲームパッドの入力情報(プレス情報)
BYTE             m_aGamePadStateTrigger[MAX_DEVICE][XIJS_BUTTON_MAX];   // ゲームパッドの入力情報(トリガー情報)
BYTE             m_aGamePadStateRelease[MAX_DEVICE][XIJS_BUTTON_MAX];   // ゲームパッドの入力情報(リリース情報)
BYTE             m_aGamePadStateRepeat[MAX_DEVICE][XIJS_BUTTON_MAX];    // ゲームパッドの入力情報(リピート情報)
DWORD            m_aGamePadCounter[MAX_DEVICE][XIJS_BUTTON_MAX];        // ゲームパッドの入力されてる間を数えるカウンター
int  Pad;                                    // 接続されているジョイパッドの数

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInputオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	//======================================
	// キーボード初期化処理
	//======================================
	// 入力デバイス(キーボード)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	//======================================
	// ジョイパッド初期化処理
	//======================================
	// 入力デバイス(ジョイパッド)の生成
	if (FAILED(g_pInput->CreateDevice(GUID_Joystick, &g_pDevJoypad, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevJoypad->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(g_pDevJoypad->SetCooperativeLevel(hWnd, (DISCL_BACKGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	if (FAILED(g_pDevJoypad->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	if (g_pDevJoypad != NULL)
	{// 生成できた
		DIPROPRANGE diprg;

		// 軸の値の範囲を設定
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -DI_JOY_I_INPUT;
		diprg.lMax = DI_JOY_I_INPUT;

		// 軸の設定
		// 左アナログスティック
		diprg.diph.dwObj = DIJOFS_X;
		g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Y;
		g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Z;
		g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 右アナログスティック
		diprg.diph.dwObj = DIJOFS_RX;
		g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RY;
		g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RZ;
		g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ?
		//diprg.diph.dwObj = DIJOFS_SLIDER(0);
		//g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 十字キー(上を0度とし時計回りに角度 * 100)
		diprg.diph.dwObj = DIJOFS_POV(0);
		g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ジョイパッドへのアクセス権を獲得(入力制御開始)
		g_pDevJoypad->Acquire();
	}

	//======================================
	// XInput初期化処理
	//======================================
	// ゲームパッドのクリア
	ZeroMemory(&g_XInput, sizeof(CONTROLER_STATE) * MAX_DEVICE);

	// 振動機能のクリア
	ZeroMemory(&g_aVibrarion, sizeof(XINPUT_VIBRATION) * MAX_DEVICE);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitInput(void)
{
	// 入力デバイス(キーボード)の開放
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire(); // キーボードへのアクセス権を放棄
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// 入力デバイス(ジョイパッド)の開放
	if (g_pDevJoypad != NULL)
	{
		g_pDevJoypad->Unacquire();   // ジョイパッドへのアクセス権を放棄
		g_pDevJoypad->Release();
		g_pDevJoypad = NULL;
	}

	// DirectInputオブジェクトの開放
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}

	// ゲームパッドの開放
	XInputEnable(false);
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateInput(void)
{
	//======================================
	// キーボード更新処理
	//======================================
	BYTE aKeyState[NUM_KEY_MAX]; // キーボードの入力情報
	int nCntKey;

	// 入力デバイス(キーボード)からデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateTrigger[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];   // キーボードの入力情報(トリガー情報)保存
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & g_aKeyState[nCntKey]; // キーボードの入力情報(リリース情報)保存
			g_aKeyState[nCntKey] = aKeyState[nCntKey];  // キーボードの入力情報(プレス情報)保存
		}
	}
	else
	{
		g_pDevKeyboard->Acquire(); // キーボードへのアクセス権を獲得
	}

	//======================================
	// ジョイパッド更新処理
	//======================================
	if (g_pDevJoypad != NULL)
	{// 生成できた
		DIJOYSTATE dijs;   // ジョイパッドの入力情報
		BYTE aJoyState[DIJS_BUTTON_MAX]; // キーボードの入力情報

		// 入力デバイス(ジョイパッド)からデータを取得
		g_pDevJoypad->Poll();
		if (SUCCEEDED(g_pDevJoypad->GetDeviceState(sizeof(DIJOYSTATE), &dijs)))
		{
			//-----------------------------
			// 左アナログスティック
			//-----------------------------
			if (dijs.lY <= -100)
			{// 左アナログスティックが上に倒された
				dijs.rgbButtons[DIJS_BUTTON_10] = 0x80;
			}
			if (dijs.lY >= 100)
			{// 左アナログスティックが下に倒された
				dijs.rgbButtons[DIJS_BUTTON_11] = 0x80;
			}
			if (dijs.lX <= -100)
			{// 左アナログスティックが左に倒された
				dijs.rgbButtons[DIJS_BUTTON_13] = 0x80;
			}
			if (dijs.lX >= 100)
			{// 左アナログスティックが右に倒された
				dijs.rgbButtons[DIJS_BUTTON_12] = 0x80;
			}

			//-----------------------------
			// 右アナログスティック
			//-----------------------------
			if (dijs.lRz <= -100)
			{// 右アナログスティックが上に倒された
				dijs.rgbButtons[DIJS_BUTTON_14] = 0x80;
			}
			if (dijs.lRz >= 100)
			{// 右アナログスティックが下に倒された
				dijs.rgbButtons[DIJS_BUTTON_15] = 0x80;
			}
			if (dijs.lZ <= -100)
			{// 右アナログスティックが左に倒された
				dijs.rgbButtons[DIJS_BUTTON_17] = 0x80;
			}
			if (dijs.lZ >= 100)
			{// 右アナログスティックが右に倒された
				dijs.rgbButtons[DIJS_BUTTON_16] = 0x80;
			}

			//-----------------------------
			// 十字キー
			//-----------------------------
			if (dijs.rgdwPOV[0] / 100 == 0)
			{// 十字キーの上が押された
				dijs.rgbButtons[DIJS_BUTTON_18] = 0x80;
			}
			if (dijs.rgdwPOV[0] / 100 == 45)
			{// 十字キーの右上が押された
				dijs.rgbButtons[DIJS_BUTTON_18] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_20] = 0x80;
			}
			if (dijs.rgdwPOV[0] / 100 == 90)
			{// 十字キーの下が押された
				dijs.rgbButtons[DIJS_BUTTON_20] = 0x80;
			}
			if (dijs.rgdwPOV[0] / 100 == 135)
			{// 十字キーの右下が押された
				dijs.rgbButtons[DIJS_BUTTON_19] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_20] = 0x80;
			}
			if (dijs.rgdwPOV[0] / 100 == 180)
			{// 十字キーの右が押された
				dijs.rgbButtons[DIJS_BUTTON_19] = 0x80;
			}
			if (dijs.rgdwPOV[0] / 100 == 225)
			{// 十字キーの左下が押された
				dijs.rgbButtons[DIJS_BUTTON_19] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_21] = 0x80;
			}
			if (dijs.rgdwPOV[0] / 100 == 270)
			{// 十字キーの左が押された
				dijs.rgbButtons[DIJS_BUTTON_21] = 0x80;
			}
			if (dijs.rgdwPOV[0] / 100 == 315)
			{// 十字キーの左上が押された
				dijs.rgbButtons[DIJS_BUTTON_18] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_21] = 0x80;
			}

			for (int nCntJoyButton = 0; nCntJoyButton < DIJS_BUTTON_MAX; nCntJoyButton++)
			{// ジョイパッドのボタンの数だけ繰り返し
				g_aJoyStateTrigger[nCntJoyButton] = (g_aJoyState[nCntJoyButton] ^ dijs.rgbButtons[nCntJoyButton]) & dijs.rgbButtons[nCntJoyButton];  // ジョイパッドの入力情報(トリガー情報)保存
				g_aJoyStateRelease[nCntJoyButton] = (g_aJoyState[nCntJoyButton] ^ dijs.rgbButtons[nCntJoyButton]) & g_aJoyState[nCntJoyButton];      // ジョイパッドの入力情報(リリース情報)保存
				g_aJoyState[nCntJoyButton] = dijs.rgbButtons[nCntJoyButton];   // ジョイパッドの入力情報(プレス情報)保存
				aJoyState[nCntJoyButton] = false;
			}
		}
		else
		{
			g_pDevJoypad->Acquire(); // ジョイパッドへのアクセス権を獲得
		}
	}



	DWORD dwResult;                                         // ゲームパッドが取得できたかどうか
	BYTE aGamePadState[MAX_DEVICE][XIJS_BUTTON_MAX] = {};   // ゲームパッドのボタン入力情報
	for (DWORD nCntPad = 0; nCntPad < MAX_DEVICE; nCntPad++)
	{// 接続できるゲームパッド(最大4つまで)の数だけ繰り返し
	 // ゲームパッドの状態を取得
		dwResult = XInputGetState(nCntPad, &g_XInput[nCntPad].m_State);
		if (dwResult == ERROR_SUCCESS)
		{// 状態取得に成功した
		 // 状態を格納する
			g_XInput[nCntPad].m_bConnected = true;                                       // 接続された状態にする
			m_wButtons[nCntPad] = g_XInput[nCntPad].m_State.Gamepad.wButtons;            // ボタンのビット情報を取得
			m_bLeftTrigger[nCntPad] = g_XInput[nCntPad].m_State.Gamepad.bLeftTrigger;    // LTボタンの状態を取得
			m_bRightTrigger[nCntPad] = g_XInput[nCntPad].m_State.Gamepad.bRightTrigger;  // RTボタンの状態を取得
			m_sThumbLX[nCntPad] = g_XInput[nCntPad].m_State.Gamepad.sThumbLX;            // 左スティックのX軸を取得
			m_sThumbLY[nCntPad] = g_XInput[nCntPad].m_State.Gamepad.sThumbLY;            // 左スティックのY軸を取得
			m_sThumbRX[nCntPad] = g_XInput[nCntPad].m_State.Gamepad.sThumbRX;            // 右スティックのX軸を取得
			m_sThumbRY[nCntPad] = g_XInput[nCntPad].m_State.Gamepad.sThumbRY;            // 右スティックのY軸を取得

			// 入力の状態を取得する
			// ボタン
			DWORD wButtons = m_wButtons[nCntPad];   // ボタンのビット情報
			if (wButtons >= XINPUT_GAMEPAD_Y)
			{// Yボタンが押されている
				aGamePadState[nCntPad][XIJS_BUTTON_13] = 0x80;  // 入力を起動
				wButtons -= XINPUT_GAMEPAD_Y;
			}
			if (wButtons >= XINPUT_GAMEPAD_X)
			{// Xボタンが押されている
				aGamePadState[nCntPad][XIJS_BUTTON_12] = 0x80;  // 入力を起動
				wButtons -= XINPUT_GAMEPAD_X;
			}
			if (wButtons >= XINPUT_GAMEPAD_B)
			{// Bボタンが押されている
				aGamePadState[nCntPad][XIJS_BUTTON_11] = 0x80;  // 入力を起動
				wButtons -= XINPUT_GAMEPAD_B;
			}
			if (wButtons >= XINPUT_GAMEPAD_A)
			{// Aボタンが押されている
				aGamePadState[nCntPad][XIJS_BUTTON_10] = 0x80;  // 入力を起動
				wButtons -= XINPUT_GAMEPAD_A;
			}
			if (wButtons >= XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{// RBボタンが押されている
				aGamePadState[nCntPad][XIJS_BUTTON_9] = 0x80;  // 入力を起動
				wButtons -= XINPUT_GAMEPAD_RIGHT_SHOULDER;
			}
			if (wButtons >= XINPUT_GAMEPAD_LEFT_SHOULDER)
			{// LBボタンが押されている
				aGamePadState[nCntPad][XIJS_BUTTON_8] = 0x80;  // 入力を起動
				wButtons -= XINPUT_GAMEPAD_LEFT_SHOULDER;
			}
			if (wButtons >= XINPUT_GAMEPAD_RIGHT_THUMB)
			{// 右スティックが押し込まれている
				aGamePadState[nCntPad][XIJS_BUTTON_7] = 0x80;  // 入力を起動
				wButtons -= XINPUT_GAMEPAD_RIGHT_THUMB;
			}
			if (wButtons >= XINPUT_GAMEPAD_LEFT_THUMB)
			{// 右スティックが押し込まれている
				aGamePadState[nCntPad][XIJS_BUTTON_6] = 0x80;  // 入力を起動
				wButtons -= XINPUT_GAMEPAD_LEFT_THUMB;
			}
			if (wButtons >= XINPUT_GAMEPAD_BACK)
			{// BACKボタンが押されている
				aGamePadState[nCntPad][XIJS_BUTTON_5] = 0x80;  // 入力を起動
				wButtons -= XINPUT_GAMEPAD_BACK;
			}
			if (wButtons >= XINPUT_GAMEPAD_START)
			{// BACKボタンが押されている
				aGamePadState[nCntPad][XIJS_BUTTON_4] = 0x80;  // 入力を起動
				wButtons -= XINPUT_GAMEPAD_START;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_RIGHT)
			{// 十字キーの右が押されている
				aGamePadState[nCntPad][XIJS_BUTTON_3] = 0x80;  // 入力を起動
				wButtons -= XINPUT_GAMEPAD_DPAD_RIGHT;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_LEFT)
			{// 十字キーの左が押されている
				aGamePadState[nCntPad][XIJS_BUTTON_2] = 0x80;  // 入力を起動
				wButtons -= XINPUT_GAMEPAD_DPAD_LEFT;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_DOWN)
			{// 十字キーの下が押されている
				aGamePadState[nCntPad][XIJS_BUTTON_1] = 0x80;  // 入力を起動
				wButtons -= XINPUT_GAMEPAD_DPAD_DOWN;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_UP)
			{// 十字キーの上が押されている
				aGamePadState[nCntPad][XIJS_BUTTON_0] = 0x80;  // 入力を起動
			}

			// LTトリガー
			if (m_bLeftTrigger[nCntPad] >= MIN_GAMEPAD_LEFT_TRIGGER)
			{// LTトリガーが押されている
				aGamePadState[nCntPad][XIJS_BUTTON_14] = 0x80;  // 入力を起動する
			}
			// RTトリガー
			if (m_bRightTrigger[nCntPad] >= MIN_GAMEPAD_RIGHT_TRIGGER)
			{// LRトリガーが押されている
				aGamePadState[nCntPad][XIJS_BUTTON_15] = 0x80;  // 入力を起動する
			}

			// 左スティック
			if (m_sThumbLX[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_X)
			{// 左スティックが右に倒された
				aGamePadState[nCntPad][XIJS_BUTTON_27] = 0x80;   // 入力を起動する
			}
			if (m_sThumbLX[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_X)
			{// 左スティックが左に倒された
				aGamePadState[nCntPad][XIJS_BUTTON_26] = 0x80;   // 入力を起動する
			}
			if (m_sThumbLY[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_Y)
			{// 左スティックが上に倒された
				aGamePadState[nCntPad][XIJS_BUTTON_24] = 0x80;   // 入力を起動する
			}
			if (m_sThumbLY[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_Y)
			{// 左スティックが下に倒された
				aGamePadState[nCntPad][XIJS_BUTTON_25] = 0x80;   // 入力を起動する
			}

			// 右スティック
			if (m_sThumbRX[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_X)
			{// 右スティックが右に倒された
				aGamePadState[nCntPad][XIJS_BUTTON_31] = 0x80;   // 入力を起動する
			}
			if (m_sThumbRX[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_X)
			{// 右スティックが左に倒された
				aGamePadState[nCntPad][XIJS_BUTTON_30] = 0x80;   // 入力を起動する
			}
			if (m_sThumbRY[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_Y)
			{// 右スティックが上に倒された
				aGamePadState[nCntPad][XIJS_BUTTON_28] = 0x80;   // 入力を起動する
			}
			if (m_sThumbRY[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_Y)
			{// 右スティックが下に倒された
				aGamePadState[nCntPad][XIJS_BUTTON_29] = 0x80;   // 入力を起動する
			}

			// ボタンの状態を格納する
			for (int nCntButton = 0; nCntButton < XIJS_BUTTON_MAX; nCntButton++)
			{// ボタンの数だけ繰り返し
				m_aGamePadStateTrigger[nCntPad][nCntButton] = (m_aGamePadState[nCntPad][nCntButton] ^ aGamePadState[nCntPad][nCntButton]) & aGamePadState[nCntPad][nCntButton];   // ボタンの入力情報(トリガー情報)保存
				m_aGamePadStateRelease[nCntPad][nCntButton] = (m_aGamePadState[nCntPad][nCntButton] ^ aGamePadState[nCntPad][nCntButton]) & m_aGamePadState[nCntPad][nCntButton]; // ボタンの入力情報(リリース情報)保存
				if (aGamePadState[nCntPad][nCntButton] == 0x80)
				{// ボタンが入力されている
					m_aGamePadCounter[nCntPad][nCntButton]++;    // カウンターを進める
					if (m_aGamePadCounter[nCntPad][nCntButton] >= REPEAT_FRAME)
					{// 20フレーム分ボタンが押されている
						if (m_aGamePadCounter[nCntPad][nCntButton] % REPEAT_TRIGGER == 0)
						{// 3フレームごと
							m_aGamePadStateRepeat[nCntPad][nCntButton] = 0x80;   // ボタンの入力情報(リピート情報)を起動
						}
						else
						{// それ以外のフレーム
							m_aGamePadStateRepeat[nCntPad][nCntButton] = 0;      // ボタンの入力情報(リピート情報)を一時停止
						}
					}
				}
				else
				{// ゲームパッドのボタンが入力されていない
					m_aGamePadCounter[nCntPad][nCntButton] = 0;        // カウンターを戻す
					m_aGamePadStateRepeat[nCntPad][nCntButton] = 0;    // ボタンの入力情報(リピート情報)を停止
				}
				m_aGamePadState[nCntPad][nCntButton] = aGamePadState[nCntPad][nCntButton];  // ボタンの入力情報(プレス情報)保存
			}
		}
		else
		{// 状態取得に失敗した
			g_XInput[nCntPad].m_bConnected = false;  // 接続されていない状態にする
		}
	}
}

//=============================================================================
// キーボードの入力情報(プレス情報)を取得
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}
//=============================================================================
// キーボードの入力情報(トリガー情報)を取得
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}
//=============================================================================
// キーボードの入力情報(リリース情報)を取得
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}
//=============================================================================
// ジョイパッドの入力情報(プレス情報)を取得
//=============================================================================
bool GetJoyPadPress(int nButton)
{
	return (g_aJoyState[nButton] & 0x80) ? true : false;
}
//=============================================================================
// ジョイパッドの入力情報(トリガー情報)を取得
//=============================================================================
bool GetJoyPadTrigger(int nButton)
{
	return (g_aJoyStateTrigger[nButton] & 0x80) ? true : false;
}
//=============================================================================
// ジョイパッドの入力情報(リリース情報)を取得
//=============================================================================
bool GetJoyPadRelease(int nButton)
{
	return (g_aJoyStateRelease[nButton] & 0x80) ? true : false;
}



//=============================================================================
//    左側の振動のモーターを設定する
//=============================================================================
void SetXInputLeftMotorSpeed(int nCntPad, WORD wLeftMotorSpeed)
{
	g_aVibrarion[nCntPad].wLeftMotorSpeed = wLeftMotorSpeed;
	XInputSetState(nCntPad, &g_aVibrarion[nCntPad]);
}

//=============================================================================
//    右側の振動のモーターを設定する
//=============================================================================
void SetXInputRightMotorSpeed(int nCntPad, WORD wRightMotorSpeed)
{
	g_aVibrarion[nCntPad].wRightMotorSpeed = wRightMotorSpeed;
	XInputSetState(nCntPad, &g_aVibrarion[nCntPad]);
}

//=============================================================================
//    ゲームパッドの入力情報(プレス情報)を取得
//=============================================================================
bool GetXInputPress(int nCntPad, XIJS_BUTTON Button)
{
	return (m_aGamePadState[nCntPad][Button] & 0x80) ? true : false;
}

//=============================================================================
//    ゲームパッドの入力情報(トリガー情報)を取得
//=============================================================================
bool GetXInputTrigger(int nCntPad, XIJS_BUTTON Button)
{
	return (m_aGamePadStateTrigger[nCntPad][Button] & 0x80) ? true : false;
}

//=============================================================================
//    ゲームパッドの入力情報(リリース情報)を取得
//=============================================================================
bool GetXInputRelease(int nCntPad, XIJS_BUTTON Button)
{
	return (m_aGamePadStateRelease[nCntPad][Button] & 0x80) ? true : false;
}

//=============================================================================
//    ゲームパッドの入力情報(リピート情報)を取得
//=============================================================================
bool GetXInputRepeat(int nCntPad, XIJS_BUTTON Button)
{
	return (m_aGamePadStateRepeat[nCntPad][Button] & 0x80) ? true : false;
}

//=============================================================================
//    LTボタンの状態を取得
//=============================================================================
BYTE GetXInputLeftTrigger(int nCntPad)
{
	return m_bLeftTrigger[nCntPad];
}

//=============================================================================
//    RTボタンの状態を取得
//=============================================================================
BYTE GetXInputRightTrigger(int nCntPad)
{
	return m_bRightTrigger[nCntPad];
}

//=============================================================================
//    左スティックの向きを取得
//=============================================================================
float GetXInputLeftAxiz(int nCntPad)
{
	float axiz = atan2f((float)m_sThumbLX[nCntPad], (float)-m_sThumbLY[nCntPad]);
	return axiz;
}

//=============================================================================
//    右スティックの向きを取得
//=============================================================================
float GetXInputRightAxiz(int nCntPad)
{
	float axiz = atan2f((float)m_sThumbRX[nCntPad], (float)-m_sThumbRY[nCntPad]);
	return axiz;
}

//=============================================================================
//    左スティックのX軸を取得
//=============================================================================
SHORT GetXInputThumbLX(int nCntPad)
{
	return m_sThumbLX[nCntPad];
}

//=============================================================================
//    左スティックのY軸を取得
//=============================================================================
SHORT GetXInputThumbLY(int nCntPad)
{
	return m_sThumbLY[nCntPad];
}

//=============================================================================
//    右スティックのX軸を取得
//=============================================================================
SHORT GetXInputThumbRX(int nCntPad)
{
	return m_sThumbRX[nCntPad];
}

//=============================================================================
//    右スティックのY軸を取得
//=============================================================================
SHORT GetXInputThumbRY(int nCntPad)
{
	return m_sThumbRY[nCntPad];
}