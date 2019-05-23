#include "Controller.h"

Controller::Controller(int playerNum)
{
	_controllerNum = playerNum - 1;
}

bool Controller::GetPadState(PAD_STATE p_state)
{
	STATE_TYPE type;        //  �^�C�v�p
	XINPUT_GAMEPAD  pad;    //  ��r�p
	ZeroMemory(&pad, sizeof(XINPUT_GAMEPAD));   // ������
	switch (p_state) {
		//---- �{�^��
	case PAD_A: //  A�{�^��
		pad.wButtons = XINPUT_GAMEPAD_A;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;
	case PAD_B: //  B�{�^��
		pad.wButtons = XINPUT_GAMEPAD_B;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;
	case PAD_X: //  X�{�^��
		pad.wButtons = XINPUT_GAMEPAD_X;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;
	case PAD_Y: //  Y�{�^��
		pad.wButtons = XINPUT_GAMEPAD_Y;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;
	case PAD_L_SHOULDER: // LB
		pad.wButtons = XINPUT_GAMEPAD_LEFT_SHOULDER;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;
	case PAD_R_SHOULDER: // RB
		pad.wButtons = XINPUT_GAMEPAD_RIGHT_SHOULDER;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;
	case PAD_BACK:  //  Back
		pad.wButtons = XINPUT_GAMEPAD_BACK;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;
	case PAD_START: //  Start
		pad.wButtons = XINPUT_GAMEPAD_START;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;
	case PAD_UP:    //  �\���L�[�̏�
		pad.wButtons = XINPUT_GAMEPAD_DPAD_UP;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;
	case PAD_DOWN:  //  �\���L�[�̉�
		pad.wButtons = XINPUT_GAMEPAD_DPAD_DOWN;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;
	case PAD_LEFT:  //  �\���L�[�̍�
		pad.wButtons = XINPUT_GAMEPAD_DPAD_LEFT;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;
	case PAD_RIGHT: //  �\���L�[�̉E
		pad.wButtons = XINPUT_GAMEPAD_DPAD_RIGHT;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;
	case PAD_LEFT_THUMB:    //  ���X�e�B�b�N
		pad.wButtons = XINPUT_GAMEPAD_LEFT_THUMB;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;
	case PAD_RIGHT_THUMB:   //  �E�X�e�B�b�N
		pad.wButtons = XINPUT_GAMEPAD_RIGHT_THUMB;
		type = STATE_BUTTON;    // �^�C�v���{�^����
		break;

	case PAD_L_TRIGGER: //  ���g���K�[
		pad.bLeftTrigger = 100;
		type = STATE_TRIGGER;
		break;
	case PAD_R_TRIGGER: //  �E�g���K�[
		pad.bRightTrigger = 100;
		type = STATE_TRIGGER;
		break;

		//---- �X�e�B�b�N
	case PAD_LEFT_U_STICK:  //  ���X�e�B�b�N�̏�
		type = STATE_LEFT_U_STICK;
		pad.sThumbLY = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		pad.sThumbLX = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		break;
	case PAD_LEFT_R_STICK:  //  ���X�e�B�b�N�̉E
		type = STATE_LEFT_R_STICK;
		pad.sThumbLY = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		pad.sThumbLX = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		break;
	case PAD_LEFT_D_STICK:  //  ���X�e�B�b�N�̉�
		type = STATE_LEFT_D_STICK;
		pad.sThumbLY = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		pad.sThumbLX = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		break;
	case PAD_LEFT_L_STICK:  //  ���X�e�B�b�N�̍�
		type = STATE_LEFT_L_STICK;
		pad.sThumbLY = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		pad.sThumbLX = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
		break;

		break;

	case PAD_RIGHT_U_STICK: //  �E�X�e�B�b�N�̏�
		type = STATE_RIGHT_U_STICK;
		pad.sThumbRY = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		pad.sThumbRX = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		break;
	case PAD_RIGHT_R_STICK: //  �E�X�e�B�b�N�̉E
		type = STATE_RIGHT_U_STICK;
		pad.sThumbRY = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		pad.sThumbRX = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		break;
	case PAD_RIGHT_D_STICK: //  �E�X�e�B�b�N�̉�
		type = STATE_RIGHT_U_STICK;
		pad.sThumbRY = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		pad.sThumbRX = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		break;
	case PAD_RIGHT_L_STICK: //  �E�X�e�B�b�N�̍�
		type = STATE_RIGHT_U_STICK;
		pad.sThumbRY = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		pad.sThumbRX = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
		break;
	}

	XINPUT_STATE state; // �R���g���[�����擾�p
	ZeroMemory(&state, sizeof(XINPUT_STATE)); // ������
	DWORD dwResult; // �֐����ʔ���p
					// ���擾
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