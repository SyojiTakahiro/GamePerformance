//*****************************************************************************
//
//     ���͂̏���[input.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "input.h"
#include "manager.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define DI_JOY_I_INPUT   (1000)  // �X�e�B�b�N�̕������͎�t���s���͈�
#define REPEAT_FRAME     (20)    // ���s�[�g�����N�����n�߂�t���[�����̍��v
#define REPEAT_TRIGGER   (7)     // ���s�[�g�����N������t���[���̊Ԋu

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECTINPUT8    CInput::m_pInput = NULL;         // DirectInput�I�u�W�F�N�g�ւ̃|�C���^
HWND              CInput::m_hWnd = 0;              // �E�B���h�E�n���h��
int               CInputJoyStick::m_nNumAll = 0;   // �R���g���[���[�̃f�o�C�X��

//*****************************************************************************
//    CInput�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CInput::CInput(int nNumDevice)
{
	m_pDevice = NULL;           // ���̓f�o�C�X�ւ̃|�C���^
	m_nDeviceNum = nNumDevice;  // ���̓f�o�C�X�̐�����
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CInput::~CInput()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (m_pInput == NULL)
	{// �܂�����Ă��Ȃ�
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
		m_hWnd = hWnd;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CInput::Uninit(void)
{
	// ���̓f�o�C�X�̊J��
	if (m_pDevice != NULL)
	{
		for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
		{// �f�o�C�X���m�ۂ����������J��Ԃ�
			if (m_pDevice[nCntDevice] != NULL)
			{// �f�o�C�X���m�ۂ���Ă���
				m_pDevice[nCntDevice]->Unacquire();   // �f�o�C�X�ւ̃A�N�Z�X�������
				m_pDevice[nCntDevice]->Release();
				m_pDevice[nCntDevice] = NULL;
			}
		}
		// �������̊J��
		delete m_pDevice;
		m_pDevice = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//*****************************************************************************
//    CInputKeyboard�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CInputKeyboard::CInputKeyboard(int nNumDevice) : CInput(nNumDevice)
{
	for (int nCntKeyboard = 0; nCntKeyboard < NUM_KEY_MAX; nCntKeyboard++)
	{// �L�[�{�[�h�̃L�[�̐������J��Ԃ�
		// �L�[�{�[�h�̏���������
		m_aKeyState[nCntKeyboard] = 0x00;         // �v���X���
		m_aKeyStateTrigger[nCntKeyboard] = 0x00;  // �g���K�[���
		m_aKeyStateRelease[nCntKeyboard] = 0x00;  // �����[�X���
		m_aKeyStateRepeat[nCntKeyboard] = 0x00;   // ���s�[�g���
		m_aKeyboardCounter[nCntKeyboard] = 0;     // ���̓J�E���^�[
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ���̓I�u�W�F�N�g�̍쐬
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�𓮓I�Ɋm�ۂ���
	m_pDevice = new LPDIRECTINPUTDEVICE8[m_nDeviceNum];
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// ��������f�o�C�X�̐������J��Ԃ�
		m_pDevice[nCntDevice] = NULL;
	}

	// ���̓f�o�C�X����������Ă��Ȃ�
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// �f�o�C�X�̐����������J��Ԃ�
		if (m_pDevice[nCntDevice] == NULL)
		{// ���������m�ۂł���
		     // ���̓f�o�C�X(�L�[�{�[�h)�̐���
			if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice[nCntDevice], NULL)))
			{
				return E_FAIL;
			}

			// �f�[�^�t�H�[�}�b�g��ݒ�
			if (FAILED(m_pDevice[nCntDevice]->SetDataFormat(&c_dfDIKeyboard)))
			{
				return E_FAIL;
			}

			// �������[�h��ݒ�
			if (FAILED(m_pDevice[nCntDevice]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			{
				return E_FAIL;
			}

			// �L�[�{�[�h�ւ̃A�N�Z�X�����l��
			m_pDevice[nCntDevice]->Acquire();
		}
	}
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	// Input�N���X�̊J��
	CInput::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX]; // �L�[�{�[�h�̓��͏��

	// ���̓f�o�C�X(�L�[�{�[�h)����f�[�^���擾
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// �f�o�C�X�̐����������J��Ԃ�
		if (m_pDevice[nCntDevice] != NULL)
		{// �����ł���
			if (SUCCEEDED(m_pDevice[nCntDevice]->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
			{// �f�o�C�X�̓��͏�Ԃ��擾�ł���
				for (int nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
				{// �L�[�{�[�h�̃L�[�̐������J��Ԃ�
					m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey];   // �L�[�{�[�h�̓��͏��(�g���K�[���)�ۑ�
					m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey]; // �L�[�{�[�h�̓��͏��(�����[�X���)�ۑ�
					if (aKeyState[nCntKey] == 0x80)
					{// �L�[�{�[�h�����͂���Ă���
						m_aKeyboardCounter[nCntKey]++;    // �J�E���^�[��i�߂�
						if (m_aKeyboardCounter[nCntKey] >= REPEAT_FRAME)
						{// 20�t���[�����L�[��������Ă���
							if (m_aKeyboardCounter[nCntKey] % REPEAT_TRIGGER == 0)
							{// 3�t���[������
								m_aKeyStateRepeat[nCntKey] = 0x80;   // �L�[�{�[�h�̓��͏��(���s�[�g���)���N��
							}
							else
							{// ����ȊO�̃t���[��
								m_aKeyStateRepeat[nCntKey] = 0;      // �L�[�{�[�h�̓��͏��(���s�[�g���)���ꎞ��~
							}
						}
					}
					else
					{// �L�[�{�[�h�����͂���Ă��Ȃ�
						m_aKeyboardCounter[nCntKey] = 0;        // �J�E���^�[��߂�
						m_aKeyStateRepeat[nCntKey] = 0;         // �L�[�{�[�h�̓��͏��(���s�[�g���)���~
					}
					m_aKeyState[nCntKey] = aKeyState[nCntKey];  // �L�[�{�[�h�̓��͏��(�v���X���)�ۑ�
				}
			}
			else
			{// �f�o�C�X�̓��͏�Ԃ��擾�ł��Ȃ�����
				m_pDevice[nCntDevice]->Acquire(); // �L�[�{�[�h�ւ̃A�N�Z�X�����l��
			}
		}
	}
}

//=============================================================================
//    �L�[�{�[�h�̓��͏��(�v���X���)���擾
//=============================================================================
bool CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

//=============================================================================
//    �L�[�{�[�h�̓��͏��(�g���K�[���)���擾
//=============================================================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
//    �L�[�{�[�h�̓��͏��(�����[�X���)���擾
//=============================================================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//=============================================================================
//    �L�[�{�[�h�̓��͏��(���s�[�g���)���擾
//=============================================================================
bool CInputKeyboard::GetRepeat(int nKey)
{
	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
}

//*****************************************************************************
//    CInputMouse�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CInputMouse::CInputMouse(int nNumDevice) : CInput(nNumDevice)
{
	for (int nCntMouse = 0; nCntMouse < DIMS_BUTTON_MAX; nCntMouse++)
	{// �}�E�X�̃{�^���̐������J��Ԃ�
	    // �}�E�X�̏���������
		m_aMouseState[nCntMouse] = 0x00;         // �v���X���
		m_aMouseStateTrigger[nCntMouse] = 0x00;  // �g���K�[���
		m_aMouseStateRelease[nCntMouse] = 0x00;  // �����[�X���
		m_aMouseStateRepeat[nCntMouse] = 0x00;   // ���s�[�g���
		m_aMouseCounter[nCntMouse] = 0;          // ���̓J�E���^�[
	}

	m_MouseXAxis = 0;  // �}�E�X��X��
	m_MouseYAxis = 0;  // �}�E�X��Y��
	m_MouseZAxis = 0;  // �}�E�X��Z��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CInputMouse::~CInputMouse()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ���̓I�u�W�F�N�g�̍쐬
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�𓮓I�Ɋm�ۂ���
	m_pDevice = new LPDIRECTINPUTDEVICE8[m_nDeviceNum];
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// ��������f�o�C�X�̐������J��Ԃ�
		m_pDevice[nCntDevice] = NULL;
	}

	// ���̓f�o�C�X����������Ă��Ȃ�
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// �f�o�C�X�̐����������J��Ԃ�
		if (m_pDevice[nCntDevice] == NULL)
		{// ���������m�ۂł���
		    // ���̓f�o�C�X(�}�E�X)�̐���
			if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice[nCntDevice], NULL)))
			{
				return E_FAIL;
			}

			// �f�[�^�t�H�[�}�b�g��ݒ�
			if (FAILED(m_pDevice[nCntDevice]->SetDataFormat(&c_dfDIMouse2)))
			{
				return E_FAIL;
			}

			// �������[�h��ݒ�
			if (FAILED(m_pDevice[nCntDevice]->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
			{
				return E_FAIL;
			}

			// ���̐ݒ�
			DIPROPDWORD diprop;
			diprop.diph.dwSize = sizeof(diprop);
			diprop.diph.dwHeaderSize = sizeof(diprop.diph);
			diprop.diph.dwObj = 0;
			diprop.diph.dwHow = DIPH_DEVICE;
			diprop.dwData = DIPROPAXISMODE_REL; // ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j
			m_pDevice[nCntDevice]->SetProperty(DIPROP_AXISMODE, &diprop.diph);

			// �E�B���h�E�n���h�����L�����Ă���
			m_hWnd = hWnd;

			// �}�E�X�ւ̃A�N�Z�X�����l��
			m_pDevice[nCntDevice]->Acquire();
		}
	}
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CInputMouse::Uninit(void)
{
	// Input�N���X�̊J��
	CInput::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CInputMouse::Update(void)
{
	DIMOUSESTATE2 aMouseState; // �}�E�X�̓��͏��

	// ���̓f�o�C�X(�}�E�X)����f�[�^���擾
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// �f�o�C�X�̐����������J��Ԃ�
		if (m_pDevice[nCntDevice] != NULL)
		{// �����ł���
			if (SUCCEEDED(m_pDevice[nCntDevice]->GetDeviceState(sizeof(DIMOUSESTATE2), &aMouseState)))
			{// �f�o�C�X�̓��͏�Ԃ��擾�ł���
				// ���̎擾
				m_MouseXAxis = aMouseState.lX;  // �}�E�X��X�����i�[
				m_MouseYAxis = aMouseState.lY;  // �}�E�X��Y�����i�[
				m_MouseZAxis = aMouseState.lZ;  // �}�E�X��Z�����i�[

				// �O��̃J�[�\�����W���L��
				m_PointOld = m_Point;

				// �J�[�\���ʒu�̐�΍��W���擾
				GetCursorPos(&m_Point);

				// �J�[�\����΍��W����A���̃E�B���h�E���̍��W�ɕ␳
				ScreenToClient(m_hWnd, &m_Point);

				// ����ɃT�C�Y�ύX���l�����āA���݂̃T�C�Y�ŕ␳�i�g�T�C�Y������̂Ō����ł͂Ȃ��j
				m_Point.x = LONG((FLOAT)m_Point.x * ((FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_WIDTH));
				m_Point.y = LONG((FLOAT)m_Point.y * ((FLOAT)SCREEN_HEIGHT / (FLOAT)SCREEN_HEIGHT));

				for (int nCntMouse = 0; nCntMouse < DIMS_BUTTON_MAX; nCntMouse++)
				{// �}�E�X�̃{�^���̐������J��Ԃ�
					m_aMouseStateTrigger[nCntMouse] = (m_aMouseState[nCntMouse] ^ aMouseState.rgbButtons[nCntMouse]) & aMouseState.rgbButtons[nCntMouse];  // �}�E�X�̓��͏��(�g���K�[���)�ۑ�
					m_aMouseStateRelease[nCntMouse] = (m_aMouseState[nCntMouse] ^ aMouseState.rgbButtons[nCntMouse]) & m_aMouseState[nCntMouse];           // �}�E�X�̓��͏��(�����[�X���)�ۑ�
					if (aMouseState.rgbButtons[nCntMouse] == 0x80)
					{// �}�E�X�����͂���Ă���
						m_aMouseCounter[nCntMouse]++;    // �J�E���^�[��i�߂�
						if (m_aMouseCounter[nCntMouse] >= REPEAT_FRAME)
						{// 20�t���[�����L�[��������Ă���
							if (m_aMouseCounter[nCntMouse] % REPEAT_TRIGGER == 0)
							{// 3�t���[������
								m_aMouseStateRepeat[nCntMouse] = 0x80;   // �}�E�X�̓��͏��(���s�[�g���)���N��
							}
							else
							{// ����ȊO�̃t���[��
								m_aMouseStateRepeat[nCntMouse] = 0;      // �}�E�X�̓��͏��(���s�[�g���)���ꎞ��~
							}
						}
					}
					else
					{// �L�[�{�[�h�����͂���Ă��Ȃ�
						m_aMouseCounter[nCntMouse] = 0;               // �J�E���^�[��߂�
						m_aMouseStateRepeat[nCntMouse] = 0;           // �}�E�X�̓��͏��(���s�[�g���)���~
					}
					m_aMouseState[nCntMouse] = aMouseState.rgbButtons[nCntMouse];// �}�E�X�̓��͏��(�v���X���)�ۑ�
				}
			}
			else
			{// �f�o�C�X�̓��͏�Ԃ��擾�ł��Ȃ�����
				m_pDevice[nCntDevice]->Acquire(); // �}�E�X�ւ̃A�N�Z�X�����l��
			}
		}
	}
}

//=============================================================================
//    �}�E�X�̓��͏��(�v���X���)���擾
//=============================================================================
bool CInputMouse::GetPress(int nButton)
{
	return (m_aMouseState[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    �}�E�X�̓��͏��(�g���K�[���)���擾
//=============================================================================
bool CInputMouse::GetTrigger(int nButton)
{
	return (m_aMouseStateTrigger[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    �}�E�X�̓��͏��(�����[�X���)���擾
//=============================================================================
bool CInputMouse::GetRelease(int nButton)
{
	return (m_aMouseStateRelease[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    �}�E�X�̓��͏��(���s�[�g���)���擾
//=============================================================================
bool CInputMouse::GetRepeat(int nButton)
{
	return (m_aMouseStateRepeat[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    �}�E�X��X�����擾
//=============================================================================
LONG CInputMouse::GetMouseAxisX(void)
{
	return m_MouseXAxis;
}

//=============================================================================
//    �}�E�X��Y�����擾
//=============================================================================
LONG CInputMouse::GetMouseAxisY(void)
{
	return m_MouseYAxis;
}

//=============================================================================
//    �}�E�X��Z�����擾
//=============================================================================
LONG CInputMouse::GetMouseAxisZ(void)
{
	return m_MouseZAxis;
}

//=============================================================================
//    �}�E�X�̌��݂̃J�[�\�����W���擾
//=============================================================================
POINT CInputMouse::GetPoint(void)
{
	return m_Point;
}

//=============================================================================
//    �}�E�X�̑O��̃J�[�\�����W���擾
//=============================================================================
POINT CInputMouse::GetPointOld(void)
{
	return m_PointOld;
}

//*****************************************************************************
//    CInputJoyStick�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CInputJoyStick::CInputJoyStick(int nNumDevice) : CInput(nNumDevice)
{
	for (int nCntJoyStick = 0; nCntJoyStick < MAX_DEVICE; nCntJoyStick++)
	{// �����ł���f�o�C�X�̐������J��Ԃ�
		m_apJoyStick[nCntJoyStick] = NULL;
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CInputJoyStick::~CInputJoyStick()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CInputJoyStick::Init(HINSTANCE hInstance, HWND hWnd)
{
	// ���̓I�u�W�F�N�g�̍쐬
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X�𓮓I�Ɋm�ۂ���
	m_pDevice = new LPDIRECTINPUTDEVICE8[m_nDeviceNum];
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// ��������f�o�C�X�̐������J��Ԃ�
		m_pDevice[nCntDevice] = NULL;
	}

	// �W���C�X�e�B�b�N��񋓂���
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, CallEnumJoyDeviceProc, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}

	// �W���C�X�e�B�b�N�N���X�̃C���X�^���X����
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// ��������f�o�C�X�̐������J��Ԃ�
		if (m_apJoyStick[nCntDevice] == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_apJoyStick[nCntDevice] = new CJoyStick;
			if (m_apJoyStick[nCntDevice] != NULL)
			{// ���������m�ۂł���
				m_apJoyStick[nCntDevice]->Init();
			}
			else
			{// ���������m�ۂł��Ȃ�����
				return E_FAIL;
			}
		}
		else
		{// ���������m�ۂł����ԂɂȂ�
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CInputJoyStick::Uninit(void)
{
	// �W���C�X�e�B�b�N�N���X�̊J��
	for (int nCntDevice = 0; nCntDevice < MAX_DEVICE; nCntDevice++)
	{// �����ł���f�o�C�X�̐������J��Ԃ�
		if (m_apJoyStick[nCntDevice] != NULL)
		{// ���������m�ۂ���Ă���
			// �I������
			m_apJoyStick[nCntDevice]->Uninit();

		    // �������̊J��
			delete m_apJoyStick[nCntDevice];
			m_apJoyStick[nCntDevice] = NULL;
		}
	}

	// Input�N���X�̊J��
	CInput::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CInputJoyStick::Update(void)
{
	for (int nCntDevice = 0; nCntDevice < m_nDeviceNum; nCntDevice++)
	{// ��������f�o�C�X�̐������J��Ԃ�
		if (m_apJoyStick[nCntDevice] != NULL && m_pDevice[nCntDevice] != NULL)
		{// ���������m�ۂł���
			m_apJoyStick[nCntDevice]->Update(m_pDevice[nCntDevice]);
		}
	}
}

//=============================================================================
//    �W���C�X�e�B�b�N�̏����擾����֐�
//=============================================================================
CJoyStick *CInputJoyStick::GetJoyStickDevice(int nIdxDevice)
{
	return m_apJoyStick[nIdxDevice];
}

//=============================================================================
//    �W���C�X�e�B�b�N�̗񋓏����Ăяo���֐�
//=============================================================================
BOOL FAR PASCAL CInputJoyStick::CallEnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	CInputJoyStick *pJoyStickWk = CManager::GetJoyStick();
	// �f�o�C�X��񋓂���
	return pJoyStickWk->EnumJoyDeviceProc(lpddi, pvRef);
}

//=============================================================================
//    �W���C�X�e�B�b�N�̗񋓏���
//=============================================================================
BOOL FAR PASCAL CInputJoyStick::EnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	// ���̓f�o�C�X(�W���C�X�e�B�b�N)�̐���
	if (FAILED(m_pInput->CreateDevice(lpddi->guidInstance, &m_pDevice[m_nNumAll], NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice[m_nNumAll]->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice[m_nNumAll]->SetCooperativeLevel(m_hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	if (m_pDevice[m_nNumAll] != NULL)
	{// �����ł���
		DIPROPRANGE diprg;

		// ���̒l�͈̔͂�ݒ�
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = -DI_JOY_I_INPUT;
		diprg.lMax = DI_JOY_I_INPUT;

		// ���̐ݒ�
		// ���A�i���O�X�e�B�b�N
		diprg.diph.dwObj = DIJOFS_X;
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Y;
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Z;
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �E�A�i���O�X�e�B�b�N
		diprg.diph.dwObj = DIJOFS_RX;
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RY;
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RZ;
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ?
		//diprg.diph.dwObj = DIJOFS_SLIDER(0);
		//g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �\���L�[(���0�x�Ƃ����v���Ɋp�x * 100)
		diprg.diph.dwObj = DIJOFS_POV(0);
		m_pDevice[m_nNumAll]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �W���C�X�e�B�b�N�ւ̃A�N�Z�X�����l��(���͐���J�n)
		m_pDevice[m_nNumAll]->Acquire();
	}

	//  �����񋓂̏ꍇ�͂�����
	if (m_nNumAll < m_nDeviceNum)
	{// �܂��f�o�C�X��񋓂ł���
		m_nNumAll++;
		return DIENUM_CONTINUE;
	}
	else
	{// �����f�o�C�X��񋓂ł��Ȃ�
		return DIENUM_STOP;
	}
}

//*****************************************************************************
//    CJoyStick�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CJoyStick::CJoyStick()
{
	for (int nCntJoyStick = 0; nCntJoyStick < DIJS_BUTTON_MAX; nCntJoyStick++)
	{// �W���C�X�e�B�b�N�̃{�^���̐������J��Ԃ�
	    // �W���C�X�e�B�b�N�̏���������
		m_aJoyState[nCntJoyStick] = 0x00;          // �v���X���
		m_aJoyStateTrigger[nCntJoyStick] = 0x00;   // �g���K�[���
		m_aJoyStateRelease[nCntJoyStick] = 0x00;   // �����[�X���
		m_aJoyStateRepeat[nCntJoyStick] = 0x00;    // ���s�[�g���
		m_aJoyCounter[nCntJoyStick] = 0;           // ���̓J�E���^�[
	}
	m_Radian = 0.0f;                           // �\���L�[�̃��W�A��
	m_LeftAxis_X = 0.0f;                       // ���X�e�B�b�N��X��
	m_LeftAxis_Y = 0.0f;                       // ���X�e�B�b�N��Y��
	m_RightAxis_X = 0.0f;                      // �E�X�e�B�b�N��X��
	m_RightAxis_Y = 0.0f;                      // �E�X�e�B�b�N��Y��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CJoyStick::~CJoyStick()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CJoyStick::Init(void)
{
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CJoyStick::Uninit(void)
{
	for (int nCntJoyStick = 0; nCntJoyStick < DIJS_BUTTON_MAX; nCntJoyStick++)
	{// �W���C�X�e�B�b�N�̃{�^���̐������J��Ԃ�
	    // �W���C�X�e�B�b�N�̏���������
		m_aJoyState[nCntJoyStick] = 0x00;          // �v���X���
		m_aJoyStateTrigger[nCntJoyStick] = 0x00;   // �g���K�[���
		m_aJoyStateRelease[nCntJoyStick] = 0x00;   // �����[�X���
		m_aJoyStateRepeat[nCntJoyStick] = 0x00;    // ���s�[�g���
		m_aJoyCounter[nCntJoyStick] = 0;           // ���̓J�E���^�[
	}
	m_Radian = 0.0f;                           // �\���L�[�̃��W�A��
	m_LeftAxis_X = 0.0f;                       // ���X�e�B�b�N��X��
	m_LeftAxis_Y = 0.0f;                       // ���X�e�B�b�N��Y��
	m_RightAxis_X = 0.0f;                      // �E�X�e�B�b�N��X��
	m_RightAxis_Y = 0.0f;                      // �E�X�e�B�b�N��Y��
}

//=============================================================================
//    �X�V����
//=============================================================================
void CJoyStick::Update(LPDIRECTINPUTDEVICE8 pDevice)
{
	DIJOYSTATE dijs;    // �W���C�X�e�B�b�N�̓��͏����i�[����
	if (pDevice != NULL)
	{// �����ł���
	    // ���̓f�o�C�X(�W���C�X�e�B�b�N)����f�[�^���擾
		pDevice->Poll();
		if (SUCCEEDED(pDevice->GetDeviceState(sizeof(DIJOYSTATE), &dijs)))
		{// �W���C�X�e�B�b�N�̏�Ԃ��擾�ł���
		    // �X�e�B�b�N�̎����X�V
			m_LeftAxis_X = (float)dijs.lX;
			m_LeftAxis_Y = (float)dijs.lY;
			m_RightAxis_X = (float)dijs.lZ;
			m_RightAxis_Y = (float)dijs.lRz;

			//-----------------------------
			// ���A�i���O�X�e�B�b�N
			//-----------------------------
			if (dijs.lY <= -100)
			{// ���A�i���O�X�e�B�b�N����ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_20] = 0x80;
			}
			if (dijs.lY >= 100)
			{// ���A�i���O�X�e�B�b�N�����ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_21] = 0x80;
			}
			if (dijs.lX <= -100)
			{// ���A�i���O�X�e�B�b�N�����ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_23] = 0x80;
			}
			if (dijs.lX >= 100)
			{// ���A�i���O�X�e�B�b�N���E�ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_22] = 0x80;
			}

			//-----------------------------
			// �E�A�i���O�X�e�B�b�N
			//-----------------------------
			if (dijs.lRz <= -100)
			{// �E�A�i���O�X�e�B�b�N����ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_24] = 0x80;
			}
			if (dijs.lRz >= 100)
			{// �E�A�i���O�X�e�B�b�N�����ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_25] = 0x80;
			}
			if (dijs.lZ <= -100)
			{// �E�A�i���O�X�e�B�b�N�����ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_27] = 0x80;
			}
			if (dijs.lZ >= 100)
			{// �E�A�i���O�X�e�B�b�N���E�ɓ|���ꂽ
				dijs.rgbButtons[DIJS_BUTTON_26] = 0x80;
			}

			//-----------------------------
			// �\���L�[
			//-----------------------------
			if (dijs.rgdwPOV[0] / 100 == 0)
			{// �\���L�[�̏オ�����ꂽ
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 45)
			{// �\���L�[�̉E�オ�����ꂽ
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 90)
			{// �\���L�[�̉E�������ꂽ
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 135)
			{// �\���L�[�̉E���������ꂽ
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 180)
			{// �\���L�[�̉��������ꂽ
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 225)
			{// �\���L�[�̍����������ꂽ
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 270)
			{// �\���L�[�̍��������ꂽ
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}
			if (dijs.rgdwPOV[0] / 100 == 315)
			{// �\���L�[�̍��オ�����ꂽ
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}

			for (int nCntJoyButton = 0; nCntJoyButton < DIJS_BUTTON_MAX; nCntJoyButton++)
			{// �W���C�X�e�B�b�N�̃{�^���̐������J��Ԃ�
				m_aJoyStateTrigger[nCntJoyButton] = (m_aJoyState[nCntJoyButton] ^ dijs.rgbButtons[nCntJoyButton]) & dijs.rgbButtons[nCntJoyButton];      // �W���C�X�e�B�b�N�̓��͏��(�g���K�[���)�ۑ�
				m_aJoyStateRelease[nCntJoyButton] = (m_aJoyState[nCntJoyButton] ^ dijs.rgbButtons[nCntJoyButton]) & m_aJoyState[nCntJoyButton]; // �W���C�X�e�B�b�N�̓��͏��(�����[�X���)�ۑ�
				if (dijs.rgbButtons[nCntJoyButton] == 0x80)
				{// �W���C�X�e�B�b�N�����͂���Ă���
					m_aJoyCounter[nCntJoyButton]++;    // �J�E���^�[��i�߂�
					if (m_aJoyCounter[nCntJoyButton] >= REPEAT_FRAME)
					{// 20�t���[�����L�[��������Ă���
						if (m_aJoyCounter[nCntJoyButton] % REPEAT_TRIGGER == 0)
						{// 3�t���[������
							m_aJoyStateRepeat[nCntJoyButton] = 0x80;   // �W���C�X�e�B�b�N�̓��͏��(���s�[�g���)���N��
						}
						else
						{// ����ȊO�̃t���[��
							m_aJoyStateRepeat[nCntJoyButton] = 0;     // �W���C�X�e�B�b�N�̓��͏��(���s�[�g���)���ꎞ��~
						}
					}
				}
				else
				{// �W���C�X�e�B�b�N�����͂���Ă��Ȃ�
					m_aJoyCounter[nCntJoyButton] = 0;          // �J�E���^�[��߂�
					m_aJoyStateRepeat[nCntJoyButton] = 0;      // �W���C�X�e�B�b�N�̓��͏��(���s�[�g���)���~
				}
				m_aJoyState[nCntJoyButton] = dijs.rgbButtons[nCntJoyButton];   // �W���C�X�e�B�b�N�̓��͏��(�v���X���)�ۑ�
			}
		}
		else
		{// �W���C�p�b�h�ւ̃A�N�Z�X�����Ȃ�
			pDevice->Acquire(); //  �W���C�p�b�h�ւ̃A�N�Z�X�����l��
		}
	}
}

//=============================================================================
//    �W���C�X�e�B�b�N�̓��͏��(�v���X���)���擾
//=============================================================================
bool CJoyStick::GetPress(int nButton)
{
	return (m_aJoyState[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    �W���C�X�e�B�b�N�̓��͏��(�g���K�[���)���擾
//=============================================================================
bool CJoyStick::GetTrigger(int nButton)
{
	return (m_aJoyStateTrigger[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    �W���C�X�e�B�b�N�̓��͏��(�����[�X���)���擾
//=============================================================================
bool CJoyStick::GetRelease(int nButton)
{
	return (m_aJoyStateRelease[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    �W���C�X�e�B�b�N�̓��͏��(���s�[�g���)���擾
//=============================================================================
bool CJoyStick::GetRepeat(int nButton)
{
	return (m_aJoyStateRepeat[nButton] & 0x80) ? true : false;
}

//=============================================================================
//    �W���C�X�e�B�b�N�̏\���L�[�̃��W�A�����擾
//=============================================================================
float CJoyStick::GetRadian(void)
{
	return m_Radian / 100;
}

//=============================================================================
//    �W���C�X�e�B�b�N�̍��X�e�B�b�N��X�����擾
//=============================================================================
float CJoyStick::GetLeftAxisX(void)
{
	return m_LeftAxis_X;
}

//=============================================================================
//    �W���C�X�e�B�b�N�̍��X�e�B�b�N��Y�����擾
//=============================================================================
float CJoyStick::GetLeftAxisY(void)
{
	return m_LeftAxis_Y;
}

//=============================================================================
//    �W���C�X�e�B�b�N�̍��X�e�B�b�N�̎����擾
//=============================================================================
float CJoyStick::GetLeftAxis(void)
{
	float Axis = atan2f(m_LeftAxis_X, m_LeftAxis_Y);  // �~�����ɕϊ�
	return Axis;
}

//=============================================================================
//    �W���C�X�e�B�b�N�̉E�X�e�B�b�N��X�����擾
//=============================================================================
float CJoyStick::GetRightAxisX(void)
{
	return m_RightAxis_X;
}

//=============================================================================
//    �W���C�X�e�B�b�N�̉E�X�e�B�b�N��Y�����擾
//=============================================================================
float CJoyStick::GetRightAxisY(void)
{
	return m_RightAxis_Y;
}

//=============================================================================
//    �W���C�X�e�B�b�N�̉E�X�e�B�b�N�̎����擾
//=============================================================================
float CJoyStick::GetRightAxis(void)
{
	float Axis = atan2f(m_RightAxis_X, m_RightAxis_Y); // �~�����ɕϊ�
	return Axis;
}

//*****************************************************************************
//    XInput�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CXInput::CXInput()
{
	// �e��l�̃N���A
	for (int nCntPad = 0; nCntPad < MAX_DEVICE; nCntPad++)
	{// �����ł���f�o�C�X�̐������J��Ԃ�
		for (int nCntButton = 0; nCntButton < XIJS_BUTTON_MAX; nCntButton++)
		{// �{�^���̐������J��Ԃ�
			m_aGamePadState[nCntPad][nCntButton] = 0x00;          // �v���X���
			m_aGamePadStateTrigger[nCntPad][nCntButton] = 0x00;   // �g���K�[���
			m_aGamePadStateRelease[nCntPad][nCntButton] = 0x00;   // �����[�X���
			m_aGamePadStateRepeat[nCntPad][nCntButton] = 0x00;    // ���s�[�g���
			m_aGamePadCounter[nCntPad][nCntButton] = 0;           // ���̓J�E���^�[
		}
		m_aGamePad[nCntPad].m_bConnected = false;                 // �ڑ��̏��
		m_aGamePad[nCntPad].m_State.Gamepad.wButtons = 0;         // �{�^���̃r�b�g���
		m_aGamePad[nCntPad].m_State.Gamepad.bLeftTrigger = 0;     // LT�{�^���̏��
		m_aGamePad[nCntPad].m_State.Gamepad.bRightTrigger = 0;    // RT�{�^���̏��
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbLX = 0;         // ���X�e�B�b�N��X��
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbLY = 0;         // ���X�e�B�b�N��Y��
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbRX = 0;         // �E�X�e�B�b�N��X��
		m_aGamePad[nCntPad].m_State.Gamepad.sThumbRY = 0;         // �E�X�e�B�b�N��Y��

		m_bLeftTrigger[nCntPad] = 0;                              // LT�{�^���̏��
		m_bRightTrigger[nCntPad] = 0;                             // RT�{�^���̏��
		m_sThumbLX[nCntPad] = 0;                                  // ���X�e�B�b�N��X���̒l
		m_sThumbLY[nCntPad] = 0;                                  // ���X�e�B�b�N��Y���̒l
		m_sThumbRX[nCntPad] = 0;                                  // �E�X�e�B�b�N��X���̒l
		m_sThumbRY[nCntPad] = 0;                                  // �E�X�e�B�b�N��Y���̒l
		m_wButtons[nCntPad] = 0;                                  // �{�^���̏��
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CXInput::~CXInput()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CXInput::Init(void)
{
	// �Q�[���p�b�h�̃N���A
	ZeroMemory(m_aGamePad, sizeof(CONTROLER_STATE) * MAX_DEVICE);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CXInput::Uninit(void)
{
	// �Q�[���p�b�h�̊J��
	XInputEnable(false);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CXInput::Update(void)
{
	DWORD dwResult;                             // �Q�[���p�b�h���擾�ł������ǂ���
	BYTE aGamePadState[XIJS_BUTTON_MAX] = {};   // �Q�[���p�b�h�̃{�^�����͏��
	for (DWORD nCntPad = 0; nCntPad < MAX_DEVICE; nCntPad++)
	{// �ڑ��ł���Q�[���p�b�h(�ő�4�܂�)�̐������J��Ԃ�
		// �Q�[���p�b�h�̏�Ԃ��擾
 		dwResult = XInputGetState(nCntPad, &m_aGamePad[nCntPad].m_State);
		if (dwResult == ERROR_SUCCESS)
		{// ��Ԏ擾�ɐ�������
			// ��Ԃ��i�[����
			m_aGamePad[nCntPad].m_bConnected = true;                                       // �ڑ����ꂽ��Ԃɂ���
			m_wButtons[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.wButtons;            // �{�^���̃r�b�g�����擾
			m_bLeftTrigger[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.bLeftTrigger;    // LT�{�^���̏�Ԃ��擾
			m_bRightTrigger[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.bRightTrigger;  // RT�{�^���̏�Ԃ��擾
			m_sThumbLX[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbLX;            // ���X�e�B�b�N��X�����擾
			m_sThumbLY[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbLY;            // ���X�e�B�b�N��Y�����擾
			m_sThumbRX[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbRX;            // �E�X�e�B�b�N��X�����擾
			m_sThumbRY[nCntPad] = m_aGamePad[nCntPad].m_State.Gamepad.sThumbRY;            // �E�X�e�B�b�N��Y�����擾

			// ���͂̏�Ԃ��擾����
			// �{�^��
			DWORD wButtons = m_wButtons[nCntPad]; // �{�^���̃r�b�g���
			if (wButtons >= XINPUT_GAMEPAD_Y)
			{// Y�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_13] = 0x80;   // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_Y;
			}
			if (wButtons >= XINPUT_GAMEPAD_X)
			{// X�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_12] = 0x80;   // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_X;
			}
			if (wButtons >= XINPUT_GAMEPAD_B)
			{// B�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_11] = 0x80;   // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_B;
			}
			if (wButtons >= XINPUT_GAMEPAD_A)
			{// A�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_10] = 0x80;   // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_A;
			}
			if (wButtons >= XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{// RB�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_9] = 0x80;    // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_RIGHT_SHOULDER;
			}
			if (wButtons >= XINPUT_GAMEPAD_LEFT_SHOULDER)
			{// LB�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_8] = 0x80;    // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_LEFT_SHOULDER;
			}
			if (wButtons >= XINPUT_GAMEPAD_RIGHT_THUMB)
			{// �E�X�e�B�b�N���������܂�Ă���
				aGamePadState[XIJS_BUTTON_7] = 0x80;    // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_RIGHT_THUMB;
			}
			if (wButtons >= XINPUT_GAMEPAD_LEFT_THUMB)
			{// �E�X�e�B�b�N���������܂�Ă���
				aGamePadState[XIJS_BUTTON_6] = 0x80;    // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_LEFT_THUMB;
			}
			if (wButtons >= XINPUT_GAMEPAD_BACK)
			{// BACK�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_5] = 0x80;    // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_BACK;
			}
			if (wButtons >= XINPUT_GAMEPAD_START)
			{// BACK�{�^����������Ă���
				aGamePadState[XIJS_BUTTON_4] = 0x80;    // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_START;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_RIGHT)
			{// �\���L�[�̉E��������Ă���
				aGamePadState[XIJS_BUTTON_3] = 0x80;    // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_DPAD_RIGHT;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_LEFT)
			{// �\���L�[�̍���������Ă���
				aGamePadState[XIJS_BUTTON_2] = 0x80;    // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_DPAD_LEFT;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_DOWN)
			{// �\���L�[�̉���������Ă���
				aGamePadState[XIJS_BUTTON_1] = 0x80;    // ���͂��N��
				wButtons -= XINPUT_GAMEPAD_DPAD_DOWN;
			}
			if (wButtons >= XINPUT_GAMEPAD_DPAD_UP)
			{// �\���L�[�̏オ������Ă���
				aGamePadState[XIJS_BUTTON_0] = 0x80;    // ���͂��N��
			}

			// LT�g���K�[
			if (m_bLeftTrigger[nCntPad] >= MIN_GAMEPAD_LEFT_TRIGGER)
			{// LT�g���K�[��������Ă���
				aGamePadState[XIJS_BUTTON_14] = 0x80;   // ���͂��N������
			}
			// RT�g���K�[
			if (m_bRightTrigger[nCntPad] >= MIN_GAMEPAD_RIGHT_TRIGGER)
			{// LR�g���K�[��������Ă���
				aGamePadState[XIJS_BUTTON_15] = 0x80;   // ���͂��N������
			}

			// ���X�e�B�b�N
			if (m_sThumbLX[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_X)
			{// ���X�e�B�b�N���E�ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_19] = 0x80;   // ���͂��N������
			}
			if (m_sThumbLX[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_X)
			{// ���X�e�B�b�N�����ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_18] = 0x80;   // ���͂��N������
			}
			if (m_sThumbLY[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_Y)
			{// ���X�e�B�b�N����ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_16] = 0x80;   // ���͂��N������
			}
			if (m_sThumbLY[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_Y)
			{// ���X�e�B�b�N�����ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_17] = 0x80;   // ���͂��N������
			}

			// �E�X�e�B�b�N
			if (m_sThumbRX[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_X)
			{// �E�X�e�B�b�N���E�ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_23] = 0x80;   // ���͂��N������
			}
			if (m_sThumbRX[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_X)
			{// �E�X�e�B�b�N�����ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_22] = 0x80;   // ���͂��N������
			}
			if (m_sThumbRY[nCntPad] >= MIN_GAMEPAD_LEFT_THUMB_Y)
			{// �E�X�e�B�b�N����ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_20] = 0x80;   // ���͂��N������
			}
			if (m_sThumbRY[nCntPad] <= -MIN_GAMEPAD_LEFT_THUMB_Y)
			{// �E�X�e�B�b�N�����ɓ|���ꂽ
				aGamePadState[XIJS_BUTTON_21] = 0x80;   // ���͂��N������
			}

			// �{�^���̏�Ԃ��i�[����
			for (int nCntButton = 0; nCntButton < XIJS_BUTTON_MAX; nCntButton++)
			{// �{�^���̐������J��Ԃ�
				m_aGamePadStateTrigger[nCntPad][nCntButton] = (m_aGamePadState[nCntPad][nCntButton] ^ aGamePadState[nCntButton]) & aGamePadState[nCntButton];            // �{�^���̓��͏��(�g���K�[���)�ۑ�
				m_aGamePadStateRelease[nCntPad][nCntButton] = (m_aGamePadState[nCntPad][nCntButton] ^ aGamePadState[nCntButton]) & m_aGamePadState[nCntPad][nCntButton]; // �{�^���̓��͏��(�����[�X���)�ۑ�
				if (aGamePadState[nCntButton] == 0x80)
				{// �{�^�������͂���Ă���
					m_aGamePadCounter[nCntPad][nCntButton]++;    // �J�E���^�[��i�߂�
					if (m_aGamePadCounter[nCntPad][nCntButton] >= REPEAT_FRAME)
					{// 20�t���[�����{�^����������Ă���
						if (m_aGamePadCounter[nCntPad][nCntButton] % REPEAT_TRIGGER == 0)
						{// 3�t���[������
							m_aGamePadStateRepeat[nCntPad][nCntButton] = 0x80;   // �{�^���̓��͏��(���s�[�g���)���N��
						}
						else
						{// ����ȊO�̃t���[��
							m_aGamePadStateRepeat[nCntPad][nCntButton] = 0;      // �{�^���̓��͏��(���s�[�g���)���ꎞ��~
						}
					}
				}
				else
				{// �Q�[���p�b�h�̃{�^�������͂���Ă��Ȃ�
					m_aGamePadCounter[nCntPad][nCntButton] = 0;        // �J�E���^�[��߂�
					m_aGamePadStateRepeat[nCntPad][nCntButton] = 0;    // �{�^���̓��͏��(���s�[�g���)���~
				}
				m_aGamePadState[nCntPad][nCntButton] = aGamePadState[nCntButton];  // �{�^���̓��͏��(�v���X���)�ۑ�
			}
		}
		else
		{// ��Ԏ擾�Ɏ��s����
			m_aGamePad[nCntPad].m_bConnected = false;  // �ڑ�����Ă��Ȃ���Ԃɂ���
		}
	}
}

//=============================================================================
//    �Q�[���p�b�h�̓��͏��(�v���X���)���擾
//=============================================================================
bool CXInput::GetPress(int nCntPad, XIJS_BUTTON Button)
{
	return (m_aGamePadState[nCntPad][Button] & 0x80) ? true : false;
}

//=============================================================================
//    �Q�[���p�b�h�̓��͏��(�g���K�[���)���擾
//=============================================================================
bool CXInput::GetTrigger(int nCntPad, XIJS_BUTTON Button)
{
	return (m_aGamePadStateTrigger[nCntPad][Button] & 0x80) ? true : false;
}

//=============================================================================
//    �Q�[���p�b�h�̓��͏��(�����[�X���)���擾
//=============================================================================
bool CXInput::GetRelease(int nCntPad, XIJS_BUTTON Button)
{
	return (m_aGamePadStateRelease[nCntPad][Button] & 0x80) ? true : false;
}

//=============================================================================
//    �Q�[���p�b�h�̓��͏��(���s�[�g���)���擾
//=============================================================================
bool CXInput::GetRepeat(int nCntPad, XIJS_BUTTON Button)
{
	return (m_aGamePadStateRepeat[nCntPad][Button] & 0x80) ? true : false;
}

//=============================================================================
//    LT�{�^���̏�Ԃ��擾
//=============================================================================
BYTE CXInput::GetLeftTrigger(int nCntPad)
{
	return m_bLeftTrigger[nCntPad];
}

//=============================================================================
//    RT�{�^���̏�Ԃ��擾
//=============================================================================
BYTE CXInput::GetRightTrigger(int nCntPad)
{
	return m_bRightTrigger[nCntPad];
}

//=============================================================================
//    ���X�e�B�b�N�̌������擾
//=============================================================================
float CXInput::GetLeftAxiz(int nCntPad)
{
	float axiz = atan2f(m_sThumbLX[nCntPad], m_sThumbLY[nCntPad]);
	return axiz;
}

//=============================================================================
//    �E�X�e�B�b�N�̌������擾
//=============================================================================
float CXInput::GetRightAxiz(int nCntPad)
{
	float axiz = atan2f(m_sThumbRX[nCntPad], m_sThumbRY[nCntPad]);
	return axiz;
}

//=============================================================================
//    ���X�e�B�b�N��X�����擾
//=============================================================================
SHORT CXInput::GetThumbLX(int nCntPad)
{
	return m_sThumbLX[nCntPad];
}

//=============================================================================
//    ���X�e�B�b�N��Y�����擾
//=============================================================================
SHORT CXInput::GetThumbLY(int nCntPad)
{
	return m_sThumbLY[nCntPad];
}

//=============================================================================
//    �E�X�e�B�b�N��X�����擾
//=============================================================================
SHORT CXInput::GetThumbRX(int nCntPad)
{
	return m_sThumbRX[nCntPad];
}

//=============================================================================
//    �E�X�e�B�b�N��Y�����擾
//=============================================================================
SHORT CXInput::GetThumbRY(int nCntPad)
{
	return m_sThumbRY[nCntPad];
}
