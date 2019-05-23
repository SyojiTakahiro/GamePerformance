#include "Controller.h"

Controller::Controller(int playerNum)
{
	_controllerNum = playerNum - 1;
}

bool Controller::GetPadState(PAD_STATE p_state)
{
	STATE_TYPE type;        //  タイプ用
	XINPUT_GAMEPAD  pad;    //  比較用
	ZeroMemory(&pad, sizeof(XINPUT_GAMEPAD));   // 初期化
	switch (p_state) {
		//---- ボタン
	case PAD_A: //  Aボタン
		pad.wButtons = XINPUT_GAMEPAD_A;
		type = STATE_BUTTON;    // タイプをボタンに
		break;
	case PAD_B: //  Bボタン
		pad.wButtons = XINPUT_GAMEPAD_B;
		type = STATE_BUTTON;    // タイプをボタンに
		break;
	case PAD_X: //  Xボタン
		pad.wButtons = XINPUT_GAMEPAD_X;
		type = STATE_BUTTON;    // タイプをボタンに
		break;
	case PAD_Y: //  Yボタン
		pad.wButtons = XINPUT_GAMEPAD_Y;
		type = STATE_BUTTON;    // タイプをボタンに
		break;
	case PAD_L_SHOULDER: // LB
		pad.wButtons = XINPUT_GAMEPAD_LEFT_SHOULDER;
		type = STATE_BUTTON;    // タイプをボタンに
		break;
	case PAD_R_SHOULDER: // RB
		pad.wButtons = XINPUT_GAMEPAD_RIGHT_SHOULDER;
		type = STATE_BUTTON;    // タイプをボタンに
		break;
	case PAD_BACK:  //  Back
		pad.wButtons = XINPUT_GAMEPAD_BACK;
		type = STATE_BUTTON;    // タイプをボタンに
		break;
	case PAD_START: //  Start
		pad.wButtons = XINPUT_GAMEPAD_START;
		type = STATE_BUTTON;    // タイプをボタンに
		break;
	case PAD_UP:    //  十字キーの上
		pad.wButtons = XINPUT_GAMEPAD_DPAD_UP;
		type = STATE_BUTTON;    // タイプをボタンに
		break;
	case PAD_DOWN:  //  十字キーの下
		pad.wButtons = XINPUT_GAMEPAD_DPAD_DOWN;
		type = STATE_BUTTON;    // タイプをボタンに
		break;
	case PAD_LEFT:  //  十字キーの左
		pad.wButtons = XINPUT_GAMEPAD_DPAD_LEFT;
		type = STATE_BUTTON;    // タイプをボタンに
		break;
	case PAD_RIGHT: //  十字キーの右
		pad.wButtons = XINPUT_GAMEPAD_DPAD_RIGHT;
		type = STATE_BUTTON;    // タイプをボタンに
		break;
	case PAD_LEFT_THUMB:    //  左スティック
		pad.wButtons = XINPUT_GAMEPAD_LEFT_THUMB;
		type = STATE_BUTTON;    // タイプをボタンに
		break;
	case PAD_RIGHT_THUMB:   //  右スティック
		pad.wButtons = XINPUT_GAMEPAD_RIGHT_THUMB;
		type = STATE_BUTTON;    // タイプをボタンに
		break;

	case PAD_L_TRIGGER: //  左トリガー
		pad.bLeftTrigger = 100;
		type = STATE_TRIGGER;
		break;
	case PAD_R_TRIGGER: //  右トリガー
		pad.bRightTrigger = 100;
		type = STATE_TRIGGER;
		break;

		//---- スティック
	case PAD_LEFT_U_STICK:  //  左スティックの上
		type = STATE_LEFT_U_STICK;
		pad.sThumbLY = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		pad.sThumbLX = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		break;
	case PAD_LEFT_R_STICK:  //  左スティックの右
		type = STATE_LEFT_R_STICK;
		pad.sThumbLY = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		pad.sThumbLX = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		break;
	case PAD_LEFT_D_STICK:  //  左スティックの下
		type = STATE_LEFT_D_STICK;
		pad.sThumbLY = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		pad.sThumbLX = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		break;
	case PAD_LEFT_L_STICK:  //  左スティックの左
		type = STATE_LEFT_L_STICK;
		pad.sThumbLY = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		pad.sThumbLX = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		break;

		break;

	case PAD_RIGHT_U_STICK: //  右スティックの上
		type = STATE_RIGHT_U_STICK;
		pad.sThumbRY = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		pad.sThumbRX = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		break;
	case PAD_RIGHT_R_STICK: //  右スティックの右
		type = STATE_RIGHT_U_STICK;
		pad.sThumbRY = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		pad.sThumbRX = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		break;
	case PAD_RIGHT_D_STICK: //  右スティックの下
		type = STATE_RIGHT_U_STICK;
		pad.sThumbRY = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		pad.sThumbRX = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		break;
	case PAD_RIGHT_L_STICK: //  右スティックの左
		type = STATE_RIGHT_U_STICK;
		pad.sThumbRY = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		pad.sThumbRX = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		break;
	}

	XINPUT_STATE state; // コントローラ情報取得用
	ZeroMemory(&state, sizeof(XINPUT_STATE)); // 初期化
	DWORD dwResult; // 関数結果判定用
					// 情報取得
	dwResult = XInputGetState(_controllerNum, &state);
	if (dwResult == ERROR_SUCCESS) {
		switch (type) {
		case STATE_BUTTON:
			if (state.Gamepad.wButtons & pad.wButtons) { return true; }
			break;
		case STATE_TRIGGER:
			if (state.Gamepad.bLeftTrigger & pad.bLeftTrigger) { return true; }
			if (state.Gamepad.bRightTrigger & pad.bRightTrigger) { return true; }
			break;
		case STATE_LEFT_U_STICK:
			if (state.Gamepad.sThumbLY >  pad.sThumbLY) { return true; }
			break;
		case STATE_LEFT_D_STICK:
			if (state.Gamepad.sThumbLY < -pad.sThumbLY) { return true; }
			break;
		case STATE_LEFT_R_STICK:
			if (state.Gamepad.sThumbLX >  pad.sThumbLX) { return true; }
		case STATE_LEFT_L_STICK:
			if (state.Gamepad.sThumbLX < -pad.sThumbLX) { return true; }
			break;
		case STATE_RIGHT_U_STICK:
			if (state.Gamepad.sThumbRY >  pad.sThumbRY) { return true; }
			break;
		case STATE_RIGHT_D_STICK:
			if (state.Gamepad.sThumbRY < -pad.sThumbRY) { return true; }
			break;
		case STATE_RIGHT_R_STICK:
			if (state.Gamepad.sThumbRX >  pad.sThumbRX) { return true; }
			break;
		case STATE_RIGHT_L_STICK:
			if (state.Gamepad.sThumbRX < -pad.sThumbRX) { return true; }
			break;
		}
	}
	return false;
}