//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "title.h"
#include "fade.h"
#include "bgTitle.h"
#include "pause.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//�ÓI�����o�ϐ�
CRenderer *m_pTitleRenderer = NULL;
LPDIRECT3DTEXTURE9 CTitle::m_pTexture = NULL;

CPause *CTitle::m_pSelect = NULL;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CTitle::CTitle()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{
	
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	CBgTitle::Create();

	CManager manager;

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = manager.GetSound();

	pSound->PlaySound(CSound::SOUND_LABEL_BGM000);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CTitle::Uninit(void)
{
	CScene::ReleaseAll();
}
//=============================================================================
// �X�V����
//=============================================================================
void CTitle::Update(void)
{
	CManager manager;
	CFade *pFade = CManager::GetFade();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = manager.GetSound();

	//�L�[�{�[�h�̎擾
	CInputKeyboard *pInput;
	pInput = manager.GetInputKeyboard();

	//�W���C�p�b�h�̎擾
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetInputJoypad();

	if (pInput->GetKeyboardTrigger(DIK_RETURN) == true)
	{
		pFade->SetFade(CManager::MODE_TUTORIAL);
		pSound->StopSound(CSound::SOUND_LABEL_BGM000);
		pSound->PlaySound(CSound::SOUND_LABEL_BGM001);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
	}

	if (pInputJoypad != NULL)
	{
		if (pInputJoypad->GetTrigger(3) == true||
			pInputJoypad->GetTrigger(11) == true)
		{
			pFade->SetFade(CManager::MODE_TUTORIAL);
			pSound->StopSound(CSound::SOUND_LABEL_BGM000);
			pSound->PlaySound(CSound::SOUND_LABEL_BGM001);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CTitle::Draw(void)
{
	
}

//=============================================================================
// �N���G�C�g
//=============================================================================
CTitle *CTitle::Create(void)
{
	CTitle *pTitle = NULL;
	if (pTitle == NULL)
	{
		// ���I�m��
		pTitle = new CTitle;

		if (pTitle != NULL)
		{
			// ����������
			pTitle->Init();
		}
	}
	return pTitle;
}