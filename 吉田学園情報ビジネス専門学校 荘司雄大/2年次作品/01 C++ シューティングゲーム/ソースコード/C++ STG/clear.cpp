//=============================================================================
//
// �N���A���� [Clear.cpp]
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
#include "clear.h"
#include "fade.h"
#include "bgClear.h"
#include "number.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//�ÓI�����o�ϐ�
CRenderer *m_pClearRenderer = NULL;
LPDIRECT3DTEXTURE9 CClear::m_pTexture = NULL;
int CClear::m_nScore = 0;     

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CClear::CClear()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CClear::~CClear()
{
	m_pTexture = NULL;
}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CClear::Load(void)
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
void CClear::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �n�C�X�R�A�̐ݒ�
//=============================================================================
void CClear::SetClearScore(int nScore)
{
	m_nScore = nScore;
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CClear::Init(void)
{
	CBgClear::Create();

	int nNumber;

	D3DXVECTOR3 pos;
	pos = D3DXVECTOR3(SCREEN_WIDTH /2 - 240 / 2,500,0);

	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		m_apNumber[nCntScore] = CNumber::Create(pos);

		// �����̌v�Z
		nNumber = m_nScore % (int)powf(10.0f, (float)(MAX_NUMBER - nCntScore)) * 10 / (int)powf(10.0f, (float)(MAX_NUMBER - nCntScore));

		m_apNumber[nCntScore]->SetNumber(nNumber);

		pos.x += 30;
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CClear::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();

			delete m_apNumber[nCntScore];
			m_apNumber[nCntScore] = NULL;
		}
	}

	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void CClear::Update(void)
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
		pSound->StopSound(CSound::SOUND_LABEL_BGM003);
		pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
	}

	if (pInputJoypad != NULL)
	{
		if (pInputJoypad->GetTrigger(3) == true ||
			pInputJoypad->GetTrigger(11) == true)
		{
			pFade->SetFade(CManager::MODE_TITLE);
			pSound->StopSound(CSound::SOUND_LABEL_BGM003);
			pSound->PlaySound(CSound::SOUND_LABEL_SE_DECIDE);
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CClear::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		m_apNumber[nCntScore]->Draw();
	}
}

//=============================================================================
// �N���G�C�g
//=============================================================================
CClear *CClear::Create(void)
{
	CClear *pResult = NULL;
	if (pResult == NULL)
	{
		// ���I�m��
		pResult = new CClear;

		if (pResult != NULL)
		{
			// ����������
			pResult->Init();
		}
	}
	return pResult;
}