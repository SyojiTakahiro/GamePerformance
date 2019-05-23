#pragma once
#pragma comment(lib,"XInput.lib")
#include <windows.h>
#include <Xinput.h>
class Controller
{
public:
	enum PAD_STATE {
		PAD_A,  //  A�{�^��
		PAD_B,  //  B�{�^��
		PAD_X,  //  X�{�^��
		PAD_Y,  //  Y�{�^��
		PAD_L_SHOULDER, //  LB
		PAD_R_SHOULDER, //  RB
		PAD_BACK,   //  Back
		PAD_START,  //  Start
		PAD_LEFT_THUMB, //  ���X�e�B�b�N
		PAD_RIGHT_THUMB,//  �E�X�e�B�b�N
		PAD_LEFT_U_STICK,   //  ���X�e�B�b�N�̏�
		PAD_LEFT_D_STICK,   //  ���X�e�B�b�N�̉�
		PAD_LEFT_L_STICK,   //  ���X�e�B�b�N�̍�
		PAD_LEFT_R_STICK,   //  ���X�e�B�b�N�̉E
		PAD_RIGHT_U_STICK,  //  �E�X�e�B�b�N�̏�
		PAD_RIGHT_D_STICK,  //  �E�X�e�B�b�N�̉�
		PAD_RIGHT_L_STICK,  //  �E�X�e�B�b�N�̍�
		PAD_RIGHT_R_STICK,  //  �E�X�e�B�b�N�̉E
		PAD_L_TRIGGER,  //  ���g���K�[
		PAD_R_TRIGGER,  //  �E�g���K�[
		PAD_UP,     //  �\���L�[�̏�
		PAD_DOWN,   //  �\���L�[�̉�
		PAD_LEFT,   //  �\���L�[�̍�
		PAD_RIGHT,  //  �\���L�[�̉E
	};

	enum STATE_TYPE {
		STATE_BUTTON,
		STATE_TRIGGER,
		STATE_LEFT_U_STICK,
		STATE_LEFT_D_STICK,
		STATE_LEFT_L_STICK,
		STATE_LEFT_R_STICK,
		STATE_RIGHT_U_STICK,
		STATE_RIGHT_D_STICK,
		STATE_RIGHT_L_STICK,
		STATE_RIGHT_R_STICK,
	};
public:
	Controller(int playerNum);
	bool GetPadState(PAD_STATE p_state);
private:
	int _controllerNum;
};