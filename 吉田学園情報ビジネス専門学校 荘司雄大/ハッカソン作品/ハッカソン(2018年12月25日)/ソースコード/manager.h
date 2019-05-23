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
#include "sound.h"

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
class CTitle;
class CTutorial;
class CGame;
class CResult;
class CRanking;

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
		MODE_TITLE = 0,  // �^�C�g��
		MODE_TUTORIAL,   // �`���[�g���A��
		MODE_GAME,       // �Q�[��
		MODE_RESULT,     // ���U���g
		MODE_RANKING,    // �����L���O
		MODE_MAX
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance,HWND hWnd,bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetMode(MODE mode);
	static MODE GetMode(void);

	static CRenderer *GetRenderer(void);
	static CLight *GetLight(void);
	static CCamera *GetCamera(void);
	static CInputKeyboard *GetKeyboard(void);
	static CInputMouse *GetMouse(void);
	static CInputJoyStick *GetJoyStick(void);
	static CXInput *GetXInput(void);
	static CSound *GetSound(void);

	static CFade *GetFade(void);
	static CTitle *GetTitle(void);
	static CTutorial *GetTutorial(void);
	static CGame *GetGame(void);
	static CResult *GetResult(void);
	static CRanking *GetRanking(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static void PlayBgm(CSound::SOUND_LABEL soundLabel);
	static void StopBgm(CSound::SOUND_LABEL soundLabel);

	static CRenderer         *m_pRenderer;  // �����_�����O�N���X�^�̃|�C���^
	static CLight            *m_pLight;     // ���C�g�N���X�^�̃|�C���^
	static CCamera           *m_pCamera;    // �J�����N���X�^�̃|�C���^
	static CInputKeyboard    *m_pKeyboard;  // �L�[�{�[�h�N���X�^�̃|�C���^
	static CInputMouse       *m_pMouse;     // �}�E�X�N���X�^�̃|�C���^
	static CInputJoyStick    *m_pJoyStick;  // �W���C�X�e�B�b�N�N���X�^�̃|�C���^
	static CXInput           *m_pXInput;    // XInput�N���X�^�̃|�C���^
	static CSound            *m_pSound;     // �T�E���h�N���X�^�̃|�C���^
	static CDebugProc        *m_pDebugProc; // �f�o�b�O���N���X�^�̃|�C���^

	static CFade             *m_pFade;      // �t�F�[�h�N���X�^�̃|�C���^
	static CTitle            *m_pTitle;     // �^�C�g���N���X�^�̃|�C���^
	static CTutorial         *m_pTutorial;  // �`���[�g���A���N���X�^�̃|�C���^
	static CGame             *m_pGame;      // �Q�[���N���X�^�̃|�C���^
	static CResult           *m_pResult;    // ���U���g�N���X�^�̃|�C���^
	static CRanking          *m_pRanking;   // �����L���O�N���X�^�̃|�C���^
	static MODE              m_Mode;        // ���݂̃��[�h
};

#if _DEBUG
//*****************************************************************************
//    �v���g�^�C�v�錾
//*****************************************************************************
bool GetUpdate(void);

#endif

#endif