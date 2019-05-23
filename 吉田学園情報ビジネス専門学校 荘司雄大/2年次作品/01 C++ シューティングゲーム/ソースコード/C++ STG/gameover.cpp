//=============================================================================
//
// �Q�[���I�[�o�[���� [gameover.cpp]
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
#include "fade.h"
#include "gameover.h"
#include "bgGameover.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//�ÓI�����o�ϐ�
CRenderer *m_pGameOverRenderer = NULL;
LPDIRECT3DTEXTURE9 CGameover::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CGameover::CGameover()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGameover::~CGameover()
{
	m_pTexture = NULL;
}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CGameover::Load(void)
{
	CManager manager;

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE0, &m_pTexture);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CGameover::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CGameover::Init(void)
{
	CBgGameover::Create();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CGameover::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void CGameover::Update(void)
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
		pFade->SetFade(CManager::MODE_TITLE);
		pSound->StopSound(CSound::SOUND_LABEL_BGM004);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
	}

	if (pInputJoypad != NULL)
	{
		if (pInputJoypad->GetTrigger(3) == true ||
			pInputJoypad->GetTrigger(11) == true)
		{
			pFade->SetFade(CManager::MODE_TITLE);
			pSound->StopSound(CSound::SOUND_LABEL_BGM004);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CGameover::Draw(void)
{

}

//=============================================================================
// �N���G�C�g
//=============================================================================
CGameover *CGameover::Create(void)
{
	CGameover *pResult = NULL;
	if (pResult == NULL)
	{
		// ���I�m��
		pResult = new CGameover;

		if (pResult != NULL)
		{
			// ����������
			pResult->Init();
		}
	}
	return pResult;
}