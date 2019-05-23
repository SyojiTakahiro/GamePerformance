//=============================================================================
//
// ���͏��� [input.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define DI_JOY_I_INPUT (1000)

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
LPDIRECTINPUT8 CInput::m_pInput = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInput::CInput()
{
	m_pDevice = NULL;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInput::~CInput()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̐���
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CInput::Uninit(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̊J��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputKeyboard::CInputKeyboard()
{
	//�L�[���̃N���A

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputKeyboard::~CInputKeyboard()
{

}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[256];
	int nCntKey;
	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < 256; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//=============================================================================
// �L�[�{�[�h�̓��͏��(�v���X���)���擾
//=============================================================================
bool CInputKeyboard::GetKeyboardPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x80) ? true : false;
}

bool CInputKeyboard::GetKeyboardTrigger(int nKey)
{
	return(m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=============================================================================
// �W���C�p�b�h�̏���
//=============================================================================

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CInputJoypad::CInputJoypad()
{
	//�L�[���̃N���A

}
//=============================================================================
// �f�X�g���N�^
//=============================================================================
CInputJoypad::~CInputJoypad()
{

}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	CInput::Init(hInstance, hWnd);

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	if (m_pDevice != NULL)
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
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Y;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_Z;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �E�A�i���O�X�e�B�b�N
		diprg.diph.dwObj = DIJOFS_RX;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RY;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);
		diprg.diph.dwObj = DIJOFS_RZ;
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// ?
		//diprg.diph.dwObj = DIJOFS_SLIDER(0);
		//g_pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �\���L�[(���0�x�Ƃ����v���Ɋp�x * 100)
		diprg.diph.dwObj = DIJOFS_POV(0);
		m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph);

		// �W���C�X�e�B�b�N�ւ̃A�N�Z�X�����l��(���͐���J�n)
		m_pDevice->Acquire();
	}

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void CInputJoypad::Uninit(void)
{
	CInput::Uninit();
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
void CInputJoypad::Update(void)
{
	DIJOYSTATE dijs;
	int nCntButton;
	if (m_pDevice != NULL)
	{
		m_pDevice->Poll();
		// ���̓f�o�C�X����f�[�^���擾
		if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(DIJOYSTATE), &dijs)))
		{
			m_LeftAxizX = (float)dijs.lX;
			m_LeftAxizY = (float)dijs.lY;
			m_RightAxizX = (float)dijs.lZ;
			m_RightAxizY = (float)dijs.lRz;

			//���X�e�B�b�N����
			if (dijs.lY < -300)
			{
				dijs.rgbButtons[DIJS_BUTTON_20] = 0x80;
			}

			if (dijs.lY > 300)
			{
				dijs.rgbButtons[DIJS_BUTTON_21] = 0x80;
			}

			if (dijs.lX > 300)
			{
				dijs.rgbButtons[DIJS_BUTTON_22] = 0x80;
			}

			if (dijs.lX < -300)
			{
				dijs.rgbButtons[DIJS_BUTTON_23] = 0x80;
			}

			//�E�X�e�B�b�N
			if (dijs.lRz < -300)
			{
				dijs.rgbButtons[DIJS_BUTTON_24] = 0x80;
			}

			if (dijs.lRz > 300)
			{
				dijs.rgbButtons[DIJS_BUTTON_25] = 0x80;
			}

			if (dijs.lZ > 300)
			{
				dijs.rgbButtons[DIJS_BUTTON_26] = 0x80;
			}

			if (dijs.lZ < -300)
			{
				dijs.rgbButtons[DIJS_BUTTON_27] = 0x80;
			}

			//�\���{�^��
			if (dijs.rgdwPOV[0] == 0)
			{
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}

			if (dijs.rgdwPOV[0] == 18000)
			{
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}

			if (dijs.rgdwPOV[0] == 9000)
			{
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}

			if (dijs.rgdwPOV[0] == 27000)
			{
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}

			if (dijs.rgdwPOV[0] == 4500)
			{
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}

			if (dijs.rgdwPOV[0] == 13500)
			{
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_30] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}

			if (dijs.rgdwPOV[0] == 22500)
			{
				dijs.rgbButtons[DIJS_BUTTON_29] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}

			if (dijs.rgdwPOV[0] == 31500)
			{
				dijs.rgbButtons[DIJS_BUTTON_28] = 0x80;
				dijs.rgbButtons[DIJS_BUTTON_31] = 0x80;
				m_Radian = D3DXToRadian(dijs.rgdwPOV[0]);
			}

			for (nCntButton = 0; nCntButton < 32; nCntButton++)
			{
				m_aStateTrigger[nCntButton] = (m_aState[nCntButton] ^ dijs.rgbButtons[nCntButton]) & dijs.rgbButtons[nCntButton];
				m_aState[nCntButton] = dijs.rgbButtons[nCntButton];
			}
		}
		else
		{
			m_pDevice->Acquire();
		}
	}
}

//=============================================================================
// �W���C�p�b�h�̓��͏��(�v���X���)���擾
//=============================================================================
bool CInputJoypad::GetPress(int nButton)
{
	return (m_aState[nButton] & 0x80) ? true : false;
}

bool CInputJoypad::GetTrigger(int nButton)
{
	return (m_aStateTrigger[nButton] & 0x80) ? true : false;
}

float CInputJoypad::GetLeftStickAxiz(void)
{
	float Axiz = atan2f(m_LeftAxizX, m_LeftAxizY);
	return Axiz;
}

float CInputJoypad::GetRightStickAxiz(void)
{
	float Axiz = atan2f(m_RightAxizX, m_RightAxizY);
	return Axiz;
}

float CInputJoypad::GetRadian(void)
{
	return m_Radian / 100;
}