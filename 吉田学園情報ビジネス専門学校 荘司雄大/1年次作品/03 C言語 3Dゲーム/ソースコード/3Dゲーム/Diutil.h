//------------------------------------------------------------------
//	Diutil.h
//------------------------------------------------------------------
#ifndef _DIUTIL_H_
#define _DIUTIL_H_

#include "main.h"

// ジョイパッドの各ボタン押下情報を、３２ビット内の次の位置に割り当てる
// そのビットがオンならば、そのボタンが押されたことになる

#define BUTTON_NULL		0x00000000l		//方向キー押されていない(NULL)
#define BUTTON_UP		0x00000001l		//方向キー上(dijs.IY<0)
#define BUTTON_DOWN		0x00000002l		//方向キー下(dijs.IY>0)
#define BUTTON_LEFT		0x00000004l		//方向キー左(dijs.IX<0)
#define BUTTON_RIGHT	0x00000008l		//方向キー右(dijs.IX>0)
#define BUTTON_A		0x00000010l		//Ａボタン(dijs.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l		//Ｂボタン(dijs.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l		//Ｃボタン(dijs.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l		//Ｘボタン(dijs.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100l		//Ｙボタン(dijs.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200l		//Ｚボタン(dijs.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l		//Ｌボタン(dijs.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l		//Ｒボタン(dijs.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l		//ＳＴＡＲＴボタン(dijs.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000l		//Ｍボタン(dijs.rgbButtons[9]&0x80)

//-----------------------------------------------------------------------------
// プロトタイプ宣言
//-----------------------------------------------------------------------------
extern BOOL CALLBACK GetJoystickCallback(LPDIDEVICEINSTANCE lpddi,LPVOID /*pvRef*/);
extern bool InitDInput(HINSTANCE,HWND);
extern bool ReleaseDInput(void);
extern DWORD GetInputDeviceState(void);
extern BOOL inputAcquireDevices(void);

#include <windows.h>
#include <stdio.h>

#define RELEASE(x)	if(x){x->Release();x=NULL;} //開放処理を簡単にする

#endif
