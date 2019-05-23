#pragma once
#pragma comment(lib,"XInput.lib")
#include <windows.h>
#include <Xinput.h>
class Controller
{
public:
	enum PAD_STATE {
		PAD_A,  //  Aボタン
		PAD_B,  //  Bボタン
		PAD_X,  //  Xボタン
		PAD_Y,  //  Yボタン
		PAD_L_SHOULDER, //  LB
		PAD_R_SHOULDER, //  RB
		PAD_BACK,   //  Back
		PAD_START,  //  Start
		PAD_LEFT_THUMB, //  左スティック
		PAD_RIGHT_THUMB,//  右スティック
		PAD_LEFT_U_STICK,   //  左スティックの上
		PAD_LEFT_D_STICK,   //  左スティックの下
		PAD_LEFT_L_STICK,   //  左スティックの左
		PAD_LEFT_R_STICK,   //  左スティックの右
		PAD_RIGHT_U_STICK,  //  右スティックの上
		PAD_RIGHT_D_STICK,  //  右スティックの下
		PAD_RIGHT_L_STICK,  //  右スティックの左
		PAD_RIGHT_R_STICK,  //  右スティックの右
		PAD_L_TRIGGER,  //  左トリガー
		PAD_R_TRIGGER,  //  右トリガー
		PAD_UP,     //  十字キーの上
		PAD_DOWN,   //  十字キーの下
		PAD_LEFT,   //  十字キーの左
		PAD_RIGHT,  //  十字キーの右
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