//*****************************************************************************
//
//     �}�l�[�W���[�̏���[manager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �O���錾(���̂��Ȃ��錾)
//*****************************************************************************
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoyStick;
class CXInput;
class CSound;
class CLight;
class CCamera;
class CDebugProc;
class CFade;
class CBaseMode;
class CTitle;
class CViewMap;
class CTutorial;
class CSetting;
class CGame;
class CResult;

//*****************************************************************************
//    �}�l�[�W���N���X�̒�`
//*****************************************************************************
class CManager
{
public:    // �N�ł��A�N�Z�X�\
	//------------------------
	//  ���[�h�̎��
	//------------------------
	typedef enum
	{
		MODE_TITLE = 0,  // �^�C�g�����
		MODE_VIEW_MAP,   // �}�b�v�Љ���
		MODE_TUTORIAL,   // �`���[�g���A�����
		MODE_SETTING,    // �Z�b�e�B���O���
		MODE_GAME,       // �Q�[�����
		MODE_RESULT,     // ���U���g���
		MODE_MAX
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance,HWND hWnd,bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetMode(int mode);
	static MODE GetMode(void);

	static CRenderer *GetRenderer(void);
	static CLight *GetLight(void);
	static CInputKeyboard *GetKeyboard(void);
	static CInputMouse *GetMouse(void);
	static CInputJoyStick *GetJoyStick(void);
	static CXInput *GetXInput(void);
	static CSound *GetSound(void);

	static CFade *GetFade(void);
	static CBaseMode *GetBaseMode(void);
	static CTitle *GetTitle(void);
	static CViewMap *GetViewMap(void);
	static CTutorial *GetTutorial(void);
	static CSetting *GetSetting(void);
	static CGame *GetGame(void);
	static CResult *GetResult(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static CRenderer         *m_pRenderer;  // �����_�����O�N���X�^�̃|�C���^
	static CLight            *m_pLight;     // ���C�g�N���X�^�̃|�C���^
	static CInputKeyboard    *m_pKeyboard;  // �L�[�{�[�h�N���X�^�̃|�C���^
	static CInputMouse       *m_pMouse;     // �}�E�X�N���X�^�̃|�C���^
	static CInputJoyStick    *m_pJoyStick;  // �W���C�X�e�B�b�N�N���X�^�̃|�C���^
	static CXInput           *m_pXInput;    // XInput�N���X�^�̃|�C���^
	static CSound            *m_pSound;     // �T�E���h�N���X�^�̃|�C���^
	static CDebugProc        *m_pDebugProc; // �f�o�b�O���N���X�^�̃|�C���^
	static CFade             *m_pFade;      // �t�F�[�h�N���X�^�̃|�C���^
	static CBaseMode         *m_pModeBase;  // ���[�h�x�[�X�N���X�^�̃|�C���^
	static MODE              m_Mode;        // ���݂̃��[�h
};

#if _DEBUG
//*****************************************************************************
//    �v���g�^�C�v�錾
//*****************************************************************************
bool GetUpdate(void);

#endif

#endif