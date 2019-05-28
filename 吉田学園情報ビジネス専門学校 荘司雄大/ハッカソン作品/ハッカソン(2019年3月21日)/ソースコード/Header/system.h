//*****************************************************************************
//
//     �V�X�e���̏���[system.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
// �f�o�b�O���O�N���X�p
#define MAX_DEBUG_LOG             (10000)   // ������i�[�ʂ̍ő�l

// ���̓N���X�p
#define NUM_KEY_MAX               (256)     // �L�[�{�[�h�̍ő吔
#define MAX_DEVICE                (4)       // �ڑ��ł�����̓f�o�C�X�̐�

#define DI_JOY_STICK_INPUT        (1000)    // �X�e�B�b�N�̕������͎�t���s���͈�
#define DI_STICK_AXIS_MIN         (15.0f)   // �X�e�B�b�N�̓��͂��J�n����X�e�B�b�N�̎��̍ŏ��l

#define MIN_GAMEPAD_LEFT_THUMB_X  (25000)   // ���X�e�B�b�N�̉������̓��͂��󂯕t���鎲�̍ŏ��l
#define MIN_GAMEPAD_LEFT_THUMB_Y  (25000)   // ���X�e�B�b�N�̏c�����̓��͂��󂯕t���鎲�̍ŏ��l
#define MIN_GAMEPAD_RIGHT_THUMB_X (25000)   // �E�X�e�B�b�N�̉������̓��͂��󂯕t���鎲�̍ŏ��l
#define MIN_GAMEPAD_RIGHT_THUMB_Y (25000)   // �E�X�e�B�b�N�̏c�����̓��͂��󂯕t���鎲�̍ŏ��l
#define MIN_GAMEPAD_LEFT_TRIGGER  (130)     // LT�{�^���̓��͂��󂯕t����l�̍ŏ��l
#define MIN_GAMEPAD_RIGHT_TRIGGER (130)     // RT�{�^���̓��͂��󂯕t����l�̍ŏ��l

// ���C�g�N���X�p
#define MAX_LIGHT                 (3)       // ���C�g�I�u�W�F�N�g�̑���

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CScene2D;
class CPlayer;

//*****************************************************************************
//    �J�����N���X�̒�`
//*****************************************************************************
class CCamera
{
public:    // �N�ł��A�N�Z�X�\
	CCamera();
	~CCamera();

	static CCamera *Create(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot, float fLength, D3DVIEWPORT9 ViewPort, CPlayer *pPlayer = NULL);

	HRESULT Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot, float fLength, D3DVIEWPORT9 ViewPort, CPlayer *pPlayer = NULL);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void SetPosV(const D3DXVECTOR3 posV);
	void SetPosVDest(const D3DXVECTOR3 posVDest);
	void SetPosVDiff(const D3DXVECTOR3 posVDiff);
	void SetPosVOld(const D3DXVECTOR3 posVOld);
	void SetPosR(const D3DXVECTOR3 posR);
	void SetPosRDest(const D3DXVECTOR3 posRDest);
	void SetPosRDiff(const D3DXVECTOR3 posRDiff);
	void SetMoveV(const D3DXVECTOR3 MoveV);
	void SetMoveR(const D3DXVECTOR3 MoveR);
	void SetRot(const D3DXVECTOR3 Rot);
	void SetRotDest(const D3DXVECTOR3 RotDest);
	void SetRotDiff(const D3DXVECTOR3 RotDiff);
	void SetRotOld(const D3DXVECTOR3 RotOld);
	void SetMtxProjection(const D3DXMATRIX MtxProjection);
	void SetMtxView(const D3DXMATRIX MtxView);

	D3DXVECTOR3 GetPosV(void);
	D3DXVECTOR3 GetPosVDest(void);
	D3DXVECTOR3 GetPosVDiff(void);
	D3DXVECTOR3 GetPosVOld(void);
	D3DXVECTOR3 GetPosR(void);
	D3DXVECTOR3 GetPosRDest(void);
	D3DXVECTOR3 GetPosRDiff(void);
	D3DXVECTOR3 GetMoveV(void);
	D3DXVECTOR3 GetMoveR(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetRotDest(void);
	D3DXVECTOR3 GetRotDiff(void);
	D3DXVECTOR3 GetRotOld(void);
	D3DXMATRIX GetMtxProjection(void);
	D3DXMATRIX GetMtxView(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Title(void);
	void ViewMap(void);
	void Tutorial(void);
	void Setting(void);
	void Game(void);
	void Result(void);
	void CalcRot(void);
	void MovieCamera(void);

	CPlayer      *m_pPlayer;        // �v���C���[�N���X�ւ̃|�C���^
	D3DXVECTOR3  m_PosV;            // ���݂̎��_
	D3DXVECTOR3  m_PosVDest;        // �ړI�̎��_
	D3DXVECTOR3  m_PosVDiff;        // ���݂̎��_�ƖړI�̎��_�̍���
	D3DXVECTOR3  m_PosVOld;         // �O��̎��_
	D3DXVECTOR3  m_PosR;            // ���݂̒����_
	D3DXVECTOR3  m_PosRDest;        // �ړI�̒����_
	D3DXVECTOR3  m_PosRDiff;        // ���݂̒����_�ƖړI�̒����_�̍���
	D3DXVECTOR3  m_MoveV;           // ���_�̈ړ���
	D3DXVECTOR3  m_MoveR;           // �����_�̈ړ���
	D3DXVECTOR3  m_Rot;             // ���݂̌���
	D3DXVECTOR3  m_RotDest;         // �ړI�̌���
	D3DXVECTOR3  m_RotDiff;         // �ړI�̌����ƌ��݂̌����̍���
	D3DXVECTOR3  m_RotOld;          // �O��̌���
	D3DXVECTOR3  m_VecU;            // ������x�N�g��
	D3DXMATRIX   m_MtxProjection;   // �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX   m_MtxView;         // �r���[�}�g���b�N�X
	D3DVIEWPORT9 m_ViewPort;        // �r���[�|�[�g(�`��̈�)
	float        m_fLength;         // ����
	bool         m_bMoving;         // �J�����𓮂����邩�ǂ���(����p)
};

//*****************************************************************************
//     �f�o�b�O���N���X�̒�`
//*****************************************************************************
class CDebugProc
{
public:    // �N�ł��A�N�Z�X�\
	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);
	void Update(void);
	static void Print(char *fmt, ...);
	static void Draw(void);

	static void SetDisp(bool bDisp);
	static bool GetDisp(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPD3DXFONT	m_pFont;		         // �t�H���g�ւ̃|�C���^
	static char         m_aStr[MAX_DEBUG_LOG];   // ������i�[�p�ϐ�
	static bool         m_bDisp;                 // �`�悷�邩���Ȃ���
};


//*****************************************************************************
//    �t�F�[�h�N���X�̒�`
//*****************************************************************************
class CFade
{
public:     // �N�ł��A�N�Z�X�\
	//------------------------
	//  �t�F�[�h�̎��
	//------------------------
	typedef enum
	{
		TYPE_MODECHANGE = 0,  // ��ʑJ�ڂɎg�p����
		TYPE_SWITCHMAP,       // �}�b�v�؂�ւ��Ɏg�p����
		TYPE_MAX
	}TYPE;

	//------------------------
	//  �t�F�[�h�̏��
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // �������Ă��Ȃ����
		STATE_IN,        // �t�F�[�h�C�����
		STATE_STOP,      // �t�F�[�h�����܂܎~�߂���
		STATE_OUT,       // �t�F�[�h�A�E�g���
		STATE_MAX
	}STATE;

	CFade();
	~CFade();

	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFade(int modeNext = 0);
	void SetFade(TYPE type = TYPE_SWITCHMAP);

	STATE GetState(void);
	CScene2D *GetScene2D(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	void FadeIn(void);
	void FadeStop(void);
	void FadeOut(void);

	static LPDIRECT3DTEXTURE9 m_pTexture;        // �����e�N�X�`���ւ̃|�C���^

	CScene2D           *m_pScene2D;       // �t�F�[�h�Ɏg�p����|���S��
	CScene2D           *m_pLogo2D;        // �����|���S��
	int                m_NextMode;        // ���ɑJ�ڂ��郂�[�h
	STATE              m_State;           // ���
	TYPE               m_Type;            // ���
	int                m_nCounter;        // �t�F�[�h���ǂꂾ���~�߂邩
};

//*****************************************************************************
//     Input�N���X�̒�`
//*****************************************************************************
class CInput
{
public:     // �N�ł��A�N�Z�X�\
	CInput(int nNumDevice = 1);
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	static HWND           m_hWnd;        // �E�B���h�E�n���h��
	static LPDIRECTINPUT8 m_pInput;      // DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8  *m_pDevice;    // ���̓f�o�C�X�ւ̃|�C���^
	int                   m_nDeviceNum;  // ���̓f�o�C�X����������������
};

//*****************************************************************************
//     �L�[�{�[�h�N���X�̒�`
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:     // �N�ł��A�N�Z�X�\
	CInputKeyboard(int nNumDevice = 1);
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);
private:    // ���̃N���X�������A�N�Z�X�\
	BYTE   m_aKeyState[NUM_KEY_MAX];          // �L�[�{�[�h�̓��͏��(�v���X���)
	BYTE   m_aKeyStateTrigger[NUM_KEY_MAX];   // �L�[�{�[�h�̓��͏��(�g���K�[���)
	BYTE   m_aKeyStateRelease[NUM_KEY_MAX];   // �L�[�{�[�h�̓��͏��(�����[�X���)
	BYTE   m_aKeyStateRepeat[NUM_KEY_MAX];    // �L�[�{�[�h�̓��͏��(���s�[�g���)
	DWORD  m_aKeyboardCounter[NUM_KEY_MAX];   // �L�[�{�[�h�����͂���Ă�Ԃ𐔂���J�E���^�[
};

//*****************************************************************************
//     �}�E�X�N���X�̒�`
//*****************************************************************************
class CInputMouse : public CInput
{
public:     // �N�ł��A�N�Z�X�\
	//------------------------
	//  �{�^���̗�
	//------------------------
	typedef enum
	{
		DIMS_BUTTON_0 = 0,  // �}�E�X�̍��{�^��
		DIMS_BUTTON_1,      // �}�E�X�̉E�{�^��
		DIMS_BUTTON_2,      // �}�E�X�̃z�C�[���{�^��
		// �ȉ��̓{�^�����̂��Ȃ��}�E�X������
		DIMS_BUTTON_3,
		DIMS_BUTTON_4,
		DIMS_BUTTON_5,
		DIMS_BUTTON_6,
		DIMS_BUTTON_7,
		DIMS_BUTTON_MAX
	}DIMS_BUTTON;

	CInputMouse(int nNumDevice = 1);
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelease(int nButton);
	bool GetRepeat(int nButton);

	LONG GetMouseAxisX(void);
	LONG GetMouseAxisY(void);
	LONG GetMouseAxisZ(void);

	POINT GetPoint(void);
	POINT GetPointOld(void);

private:    // ���̃N���X�������A�N�Z�X�\
	BYTE   m_aMouseState[DIMS_BUTTON_MAX];         // �}�E�X�̓��͏��(�v���X���)
	BYTE   m_aMouseStateTrigger[DIMS_BUTTON_MAX];  // �}�E�X�̓��͏��(�g���K�[���)
	BYTE   m_aMouseStateRelease[DIMS_BUTTON_MAX];  // �}�E�X�̓��͏��(�����[�X���)
	BYTE   m_aMouseStateRepeat[DIMS_BUTTON_MAX];   // �}�E�X�̓��͏��(���s�[�g���)
	DWORD  m_aMouseCounter[DIMS_BUTTON_MAX];       // �}�E�X�����͂���Ă�Ԃ𐔂���J�E���^�[
	LONG   m_MouseXAxis;                           // �}�E�X��X�����i�[����
	LONG   m_MouseYAxis;                           // �}�E�X��Y�����i�[����
	LONG   m_MouseZAxis;                           // �}�E�X��Z�����i�[����
	HWND   m_hWnd;                                 // �E�B���h�E�n���h��(�J�[�\�����W���擾���邽�߂ɕK�v)
	POINT  m_Point;                                // ���݂̃}�E�X�̃J�[�\�����W
	POINT  m_PointOld;                             // �O��̃}�E�X�̃J�[�\�����W
};

//*****************************************************************************
//     �W���C�X�e�B�b�N�N���X�̒�`
//*****************************************************************************
class CJoyStick
{
public:     // �N�ł��A�N�Z�X�\
	//------------------------
	//  �{�^���̗�
	//------------------------
	typedef enum
	{
		DIJS_BUTTON_0 = 0,  // X�{�^��
		DIJS_BUTTON_1,      // Y�{�^��
		DIJS_BUTTON_2,      // A�{�^��
		DIJS_BUTTON_3,      // B�{�^��
		DIJS_BUTTON_4,      // LB�{�^��
		DIJS_BUTTON_5,      // RB�{�^��
		DIJS_BUTTON_6,      // LT�{�^��
		DIJS_BUTTON_7,      // RT�{�^��
		DIJS_BUTTON_8,      // ���X�e�B�b�N��������
		DIJS_BUTTON_9,      // �E�X�e�B�b�N��������
		DIJS_BUTTON_10,     // BACK�{�^��
		DIJS_BUTTON_11,     // START�{�^��
		DIJS_BUTTON_12,
		DIJS_BUTTON_13,
		DIJS_BUTTON_14,
		DIJS_BUTTON_15,
		DIJS_BUTTON_16,
		DIJS_BUTTON_17,
		DIJS_BUTTON_18,
		DIJS_BUTTON_19,
		DIJS_BUTTON_20,      // ���A�i���O�X�e�B�b�N���������
		DIJS_BUTTON_21,      // ���A�i���O�X�e�B�b�N����������
		DIJS_BUTTON_22,      // ���A�i���O�X�e�B�b�N�E��������
		DIJS_BUTTON_23,      // ���A�i���O�X�e�B�b�N����������
		DIJS_BUTTON_24,      // �E�A�i���O�X�e�B�b�N���������
		DIJS_BUTTON_25,      // �E�A�i���O�X�e�B�b�N����������
		DIJS_BUTTON_26,      // �E�A�i���O�X�e�B�b�N�E��������
		DIJS_BUTTON_27,      // �E�A�i���O�X�e�B�b�N����������
		DIJS_BUTTON_28,      // �\���L�[���������
		DIJS_BUTTON_29,      // �\���L�[����������
		DIJS_BUTTON_30,      // �\���L�[�E��������
		DIJS_BUTTON_31,      // �\���L�[����������
		DIJS_BUTTON_MAX
	}DIJS_BUTTON;

	CJoyStick();
	~CJoyStick();
	HRESULT Init(void);
	void Uninit(void);
	void Update(LPDIRECTINPUTDEVICE8 pDevice);

	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelease(int nButton);
	bool GetRepeat(int nButton);
	float GetRadian(void);
	float GetLeftAxisX(void);
	float GetLeftAxisY(void);
	float GetLeftAxis(void);
	float GetRightAxisX(void);
	float GetRightAxisY(void);
	float GetRightAxis(void);

private:    // ���̃N���X�������A�N�Z�X�\
	BYTE   m_aJoyState[DIJS_BUTTON_MAX];          // �W���C�X�e�B�b�N�̓��͏��(�v���X���)
	BYTE   m_aJoyStateTrigger[DIJS_BUTTON_MAX];   // �W���C�X�e�B�b�N�̓��͏��(�g���K�[���)
	BYTE   m_aJoyStateRelease[DIJS_BUTTON_MAX];   // �W���C�X�e�B�b�N�̓��͏��(�����[�X���)
	BYTE   m_aJoyStateRepeat[DIJS_BUTTON_MAX];    // �W���C�X�e�B�b�N�̓��͏��(���s�[�g���)
	DWORD  m_aJoyCounter[DIJS_BUTTON_MAX];        // �W���C�X�e�B�b�N�̓��͂���Ă�Ԃ𐔂���J�E���^�[
	float  m_Radian;                              // �\���L�[�̃��W�A�����i�[����
	float  m_LeftAxis_X;                          // ���A�i���O�X�e�B�b�N��X�����i�[����
	float  m_LeftAxis_Y;                          // ���A�i���O�X�e�B�b�N��Y�����i�[����
	float  m_RightAxis_X;                         // �E�A�i���O�X�e�B�b�N��X�����i�[����
	float  m_RightAxis_Y;                         // �E�A�i���O�X�e�B�b�N��Y�����i�[����
};

//*****************************************************************************
//     �W���C�X�e�B�b�N�I�u�W�F�N�g�N���X�̒�`
//*****************************************************************************
class CInputJoyStick : public CInput
{
public:     // �N�ł��A�N�Z�X�\
	CInputJoyStick(int nNumDevice = 1);
	~CInputJoyStick();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	static BOOL PASCAL CallEnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	BOOL FAR PASCAL EnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

	CJoyStick *GetJoyStickDevice(int nIdxDevice = 0);

private:    // ���̃N���X�������A�N�Z�X�\
	static int m_nNumAll;                        // �R���g���[���[�̃f�o�C�X��
	CJoyStick *m_apJoyStick[MAX_DEVICE];         // �W���C�X�e�B�b�N�N���X�^�̃|�C���^
};

//*****************************************************************************
//     XInput�N���X�̒�`
//*****************************************************************************
class CXInput
{
public:     // �N�ł��A�N�Z�X�\
	//------------------------
	//  �{�^���̗�
	//------------------------
	// XINPUT_GAMEPAD_DPAD_UP          // �\���L�[�����
	// XINPUT_GAMEPAD_DPAD_DOWN        // �\���L�[������
	// XINPUT_GAMEPAD_DPAD_LEFT        // �\���L�[������
	// XINPUT_GAMEPAD_DPAD_RIGHT       // �\���L�[�E����
	// XINPUT_GAMEPAD_START            // START�{�^��
	// XINPUT_GAMEPAD_BACK             // BACK�{�^��
	// XINPUT_GAMEPAD_LEFT_THUMB       // ���X�e�B�b�N�{�^��
	// XINPUT_GAMEPAD_RIGHT_THUMB      // �E�X�e�B�b�N�{�^��
	// XINPUT_GAMEPAD_LEFT_SHOULDER    // LB�{�^��
	// XINPUT_GAMEPAD_RIGHT_SHOULDER   // RB�{�^��
	// XINPUT_GAMEPAD_A                // A�{�^��
	// XINPUT_GAMEPAD_B                // B�{�^��
	// XINPUT_GAMEPAD_X                // X�{�^��
	// XINPUT_GAMEPAD_Y                // Y�{�^��
	typedef enum
	{
		XIJS_BUTTON_0,     // �\���L�[�����
		XIJS_BUTTON_1,     // �\���L�[������
		XIJS_BUTTON_2,     // �\���L�[������
		XIJS_BUTTON_3,     // �\���L�[�E����
		XIJS_BUTTON_4,     // START�{�^��
		XIJS_BUTTON_5,     // BACK�{�^��
		XIJS_BUTTON_6,     // ���X�e�B�b�N�������ݓ���
		XIJS_BUTTON_7,     // �E�X�e�B�b�N�������ݓ���
		XIJS_BUTTON_8,     // LB�{�^��
		XIJS_BUTTON_9,     // RB�{�^��
		XIJS_BUTTON_10,    // A�{�^��
		XIJS_BUTTON_11,    // B�{�^��
		XIJS_BUTTON_12,    // X�{�^��
		XIJS_BUTTON_13,    // Y�{�^��
		XIJS_BUTTON_14,    // LT�{�^��
		XIJS_BUTTON_15,    // RT�{�^��
		XIJS_BUTTON_16,    // ���X�e�B�b�N�����
		XIJS_BUTTON_17,    // ���X�e�B�b�N������
		XIJS_BUTTON_18,    // ���X�e�B�b�N������
		XIJS_BUTTON_19,    // ���X�e�B�b�N�E����
		XIJS_BUTTON_20,    // �E�X�e�B�b�N�����
		XIJS_BUTTON_21,    // �E�X�e�B�b�N������
		XIJS_BUTTON_22,    // �E�X�e�B�b�N������
		XIJS_BUTTON_23,    // �E�X�e�B�b�N�E����

		// �ȉ����蓖�ĂȂ�
		XIJS_BUTTON_24,
		XIJS_BUTTON_25,
		XIJS_BUTTON_26,
		XIJS_BUTTON_27,
		XIJS_BUTTON_28,
		XIJS_BUTTON_29,
		XIJS_BUTTON_30,
		XIJS_BUTTON_31,
		XIJS_BUTTON_MAX,
	}XIJS_BUTTON;

	CXInput();
	~CXInput();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	BYTE GetLeftTrigger(int nCntPad);
	BYTE GetRightTrigger(int nCntPad);
	float GetLeftAxiz(int nCntPad);
	float GetRightAxiz(int nCntPad);
	SHORT GetThumbLX(int nCntPad);
	SHORT GetThumbLY(int nCntPad);
	SHORT GetThumbRX(int nCntPad);
	SHORT GetThumbRY(int nCntPad);

	bool GetPress(int nCntPad, XIJS_BUTTON Button);
	bool GetTrigger(int nCntPad, XIJS_BUTTON Button);
	bool GetRelease(int nCntPad, XIJS_BUTTON Button);
	bool GetRepeat(int nCntPad, XIJS_BUTTON Button);

private:    // ���̃N���X�������A�N�Z�X�\
	typedef struct
	{
		XINPUT_STATE m_State;     // �Q�[���p�b�h�̏��
		bool m_bConnected;        // �ڑ����ꂽ���ǂ���
	}CONTROLER_STATE;

	CONTROLER_STATE m_aGamePad[MAX_DEVICE];                                // �Q�[���p�b�h�̏��
	DWORD           m_wButtons[MAX_DEVICE];                                // �{�^���̏��
	BYTE            m_bLeftTrigger[MAX_DEVICE];                            // LT�{�^���̏��
	BYTE            m_bRightTrigger[MAX_DEVICE];                           // RT�{�^���̏��
	SHORT           m_sThumbLX[MAX_DEVICE];                                // ���X�e�B�b�N��X��
	SHORT           m_sThumbLY[MAX_DEVICE];                                // ���X�e�B�b�N��Y��
	SHORT           m_sThumbRX[MAX_DEVICE];                                // �E�X�e�B�b�N��X��
	SHORT           m_sThumbRY[MAX_DEVICE];                                // �E�X�e�B�b�N��Y��
	BYTE            m_aGamePadState[MAX_DEVICE][XIJS_BUTTON_MAX];          // �Q�[���p�b�h�̓��͏��(�v���X���)
	BYTE            m_aGamePadStateTrigger[MAX_DEVICE][XIJS_BUTTON_MAX];   // �Q�[���p�b�h�̓��͏��(�g���K�[���)
	BYTE            m_aGamePadStateRelease[MAX_DEVICE][XIJS_BUTTON_MAX];   // �Q�[���p�b�h�̓��͏��(�����[�X���)
	BYTE            m_aGamePadStateRepeat[MAX_DEVICE][XIJS_BUTTON_MAX];    // �Q�[���p�b�h�̓��͏��(���s�[�g���)
	DWORD           m_aGamePadCounter[MAX_DEVICE][XIJS_BUTTON_MAX];        // �Q�[���p�b�h�̓��͂���Ă�Ԃ𐔂���J�E���^�[
};

//*****************************************************************************
//    ���C�g�N���X�̒�`
//*****************************************************************************
class CLight
{
public:    // �N�ł��A�N�Z�X�\
	CLight();
	~CLight();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DLIGHT9 m_Light[MAX_LIGHT];   // ���C�g�̏��

#ifdef _DEBUG  // �f�o�b�O�ɕK�v�ȕϐ�
	int       m_nSelectLight;       // ���ݑI�����Ă��郉�C�g�̔ԍ�
#endif
};

//*****************************************************************************
//    �����_�����O�N���X�̒�`
//*****************************************************************************
class CRenderer
{
public:     // �N�ł��A�N�Z�X�\
	typedef enum
	{
		TARGET_DEFAULT = 0,   // �f�t�H���g�̐ݒ�
		TARGET_TEXTURE,       // �e�N�X�`��
		TARGET_MAX
	}TARGET;

	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	void DefaultRender(void);
	void TextureRender(void);

	LPDIRECT3D9				m_pD3D;			        // Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice;	        // Device�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DSURFACE9      m_pZBufferSurface;      // Z�o�b�t�@�ւ̃|�C���^(�e�N�X�`���Ƀ����_�����O����ۂ�Z�o�b�t�@��؂�ւ��邽��)
	LPDIRECT3DTEXTURE9      m_pTexture;             // �e�N�X�`���ւ̃|�C���^(���̃e�N�X�`���ɃI�u�W�F�N�g��`�悷��)
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;             // ���_�o�b�t�@�ւ̃|�C���^(�I�u�W�F�N�g��`�悵���e�N�X�`�����g�p����)
	TARGET                  m_Target;               // �I�u�W�F�N�g��`�悷�郌���_�����O�^�[�Q�b�g
	float                   m_fRot;                 // �|���S���̌���
	float                   m_fAngle;               // �|���S�����o���p�x
	float                   m_fWidth;               // �|���S���̕�
	float                   m_fHeight;              // �|���S���̍���
	float                   m_fLength;              // �|���S���̒���
};

//*****************************************************************************
//     �T�E���h�N���X�̒�`
//*****************************************************************************
class CSound
{
public:    // �N�ł��A�N�Z�X�\
	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(int nIdx, bool bPause = false);
	void StopSound(int nIdx, bool bPause = false);
	void StopSound(void);
	void SetVoluem(int nIdx, float fValue);

private:   // ���̃N���X�������A�N�Z�X�\
	HRESULT CreatePointer(void);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;						// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice *m_pMasteringVoice;	// �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice **m_apSourceVoice;    	// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE **m_apDataAudio;					    // �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD *m_apSizeAudio;					    // �I�[�f�B�I�f�[�^�T�C�Y�ւ̃|�C���^
	int m_nNumSound;                            // �T�E���h�f�[�^�̐�

	//------------------------
	//  �e���f�ނ̃p�����[�^
	//------------------------
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	} SOUNDPARAM;
	SOUNDPARAM *m_apSoundParam;   // ���f�ނ̃p�����[�^�ւ̃|�C���^
};

#endif