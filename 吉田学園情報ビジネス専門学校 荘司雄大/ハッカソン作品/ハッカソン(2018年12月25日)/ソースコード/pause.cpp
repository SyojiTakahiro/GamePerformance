//*****************************************************************************
//
//     �|�[�Y�̏���[pause.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "pause.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "scene2D.h"
#include "fade.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define PAUSEBG_TEXTURE_NAME     "data/TEXTURE/GAME/pause_frame.png"                     // �|�[�Y�̔w�i�̃e�N�X�`���t�@�C�X�p�X��
#define PAUSEBG_POS_INI          (D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f))  // �|�[�Y�̔w�i�̍��W
#define PAUSEBG_WIDTH_INI        (300.0f)                                                // �|�[�Y�̔w�i�̕�
#define PAUSEBG_HEIGHT_INI       (310.0f)                                                // �|�[�Y�̔w�i�̍���

#define PAUSEBLACK_COL_INI      (D3DXCOLOR(0.0f,0.0f,0.0f,0.8f))                         // �|�[�Y�w�i(�����|���S��)�̐F(�����l)

#define PAUSESELECT_POS_INI     (D3DXVECTOR3(SCREEN_WIDTH / 2,185.0f,0.0f))              // �|�[�Y���ڂ̍��W(�����l)
#define PAUSESELECT_COL_INI     (D3DXCOLOR(0.5f,0.5f,0.5f,1.0f))                         // �|�[�Y���ڂ̐F(�����l)
#define PAUSESELECT_WIDTH_INI   (250.0f)                                                 // �I������Ă���|�[�Y�I�����ڂ̕�
#define PAUSESELECT_HEIGHT_INI  (80.0f)                                                  // �I������Ă���|�[�Y�I�����ڃ|�[�Y�I�����ڂ̍���
#define PAUSESELECT_WIDTH_NONE  (180.0f)                                                 // �I������Ă��Ȃ��|�[�Y�I�����ڂ̕�
#define PAUSESELECT_HEIGHT_NONE (50.0f)                                                  // �I������Ă��Ȃ��|�[�Y�I�����ڂ̍���
#define PAUSESELECT_COL_NONE    (D3DXCOLOR(0.1f,0.1f,0.1f,1.0f))                         // �I������Ă��Ȃ��|�[�Y�I�����ڂ̐F(�����l)
#define PAUSESELECT_INTERVAL    (180.0f)                                                 // �|�[�Y�I�����ړ��m�̊Ԋu

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CPause::CPause()
{
	// �e��l�̏�����
	m_pBlackBg = NULL;                     // �|�[�Y�w�i�p�|���S��(�����|���S��)
	m_pPauseBg = NULL;                     // �|�[�Y�̔w�i�p�|���S��
	m_nSelect = SELECT_CONTINUE;           // ���ݑI�����Ă��鍀��
	m_nCounter = 0;                        // �`��Ǘ��J�E���^�[
	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// �|�[�Y��ʂ̑I�����ڂ̐������J��Ԃ�
		m_apLogoPause[nCntPause] = NULL;  // �|�[�Y���S�N���X�^�̃|�C���^
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
//    ��������
//=============================================================================
CPause *CPause::Create(int nPriority)
{
	CPause *pPause = NULL;  // �|�[�Y�N���X�^�̃|�C���^
	if (pPause == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pPause = new CPause;
		if (pPause != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pPause->Init(nPriority);
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return NULL;
	}

	// �C���X�^���X��Ԃ�
	return pPause;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CPause::Init(int nPriority)
{
	// �|�[�Y�w�i�p�����|���S���̍쐬
	if (m_pBlackBg == NULL)
	{
		m_pBlackBg = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), PAUSEBLACK_COL_INI, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, nPriority);
	}

	// �|�[�Y�̔w�i�p�̃|���S����������
	if (m_pPauseBg == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pPauseBg = CScene2D::Create(PAUSEBG_POS_INI, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSEBG_WIDTH_INI, PAUSEBG_HEIGHT_INI, 0.0f, nPriority);
	}

	// �|�[�Y�I�����ڂ̃|���S����������
	D3DXVECTOR3 pos = PAUSESELECT_POS_INI;                    // ���W
	D3DXCOLOR col = PAUSESELECT_COL_INI;                      // �F
	float fWidth = PAUSESELECT_WIDTH_INI;                     // ��
	float fHeight = PAUSESELECT_HEIGHT_INI;                   // ����
	CScene2DFlash::STATE state = CScene2DFlash::STATE_SELECT; // ���
	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// �|�[�Y��ʂ̑I�����ڂ̐������J��Ԃ�
		if (m_apLogoPause[nCntPause] == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_apLogoPause[nCntPause] = CScene2DFlash::Create(pos, col, fWidth, fHeight, 0.0f, state, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f), 0.01f, 4, nPriority);
		}

		if (nCntPause == 0)
		{// 1��ڂ̌J��Ԃ����I�����
			fWidth = PAUSESELECT_WIDTH_NONE;
			fHeight = PAUSESELECT_HEIGHT_NONE;
			col = PAUSESELECT_COL_NONE;
			state = CScene2DFlash::STATE_NONE;
		}
		pos.y += PAUSESELECT_INTERVAL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CPause::Uninit(void)
{
	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// �|�[�Y��ʂ̑I�����ڂ̐������J��Ԃ�
		if (m_apLogoPause[nCntPause] != NULL)
		{// ���������m�ۂ���Ă���
			// �|�[�Y���S�̏I������
			m_apLogoPause[nCntPause]->Uninit();
			m_apLogoPause[nCntPause] = NULL;
		}
	}

	// �w�i�|���S���̊J��
	if (m_pPauseBg != NULL)
	{// ���������m�ۂ���Ă���
		m_pPauseBg->Uninit();
		m_pPauseBg = NULL;
	}

	// �����|���S���̊J��
	if (m_pBlackBg != NULL)
	{// ���������m�ۂ���Ă���
		m_pBlackBg->Uninit();
		m_pBlackBg = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CPause::Update(void)
{
	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// �|�[�Y��ʂ̑I�����ڂ̐������J��Ԃ�
		if (m_apLogoPause[nCntPause] != NULL)
		{// ���������m�ۂ���Ă���
		    // �|�[�Y���S�̍X�V����
			m_apLogoPause[nCntPause]->Update();
		}
	}

	CInputKeyboard *pKeyboard = CManager::GetKeyboard();                   // �L�[�{�[�h�̎擾
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice();   // �W���C�X�e�B�b�N�̎擾

	// �I�����ڂ̏���
	if (pKeyboard->GetTrigger(DIK_W) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_20) == TRUE || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_28) == TRUE
		|| pKeyboard->GetRepeat(DIK_W) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_20) == TRUE || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_28) == TRUE)
	{// ������̓��͂����ꂽ
		// �I������Ă������ڂ̐ݒ�
		m_apLogoPause[m_nSelect]->SetState(CScene2DFlash::STATE_NONE);   // ���
		m_apLogoPause[m_nSelect]->SetCol(PAUSESELECT_COL_NONE);          // �F
		m_apLogoPause[m_nSelect]->SetWidth(PAUSESELECT_WIDTH_NONE);      // ��
		m_apLogoPause[m_nSelect]->SetHeight(PAUSESELECT_HEIGHT_NONE);    // ����

		// ���ɑI������鍀�ڂ̐ݒ�
		m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetState(CScene2DFlash::STATE_SELECT);  // ���
		m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetCol(PAUSESELECT_COL_INI);            // �F
		m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetWidth(PAUSESELECT_WIDTH_INI);        // ��
		m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetHeight(PAUSESELECT_HEIGHT_INI);      // ����

		// �ԍ���ݒ�
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		// ����炷
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	}
	else if (pKeyboard->GetTrigger(DIK_S) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_21) == TRUE || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_29) == TRUE
		|| pKeyboard->GetRepeat(DIK_S) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_21) == TRUE || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_29) == TRUE)
	{// �������̓��͂����ꂽ
	    // �I������Ă������ڂ̐ݒ�
		m_apLogoPause[m_nSelect]->SetState(CScene2DFlash::STATE_NONE);   // ���
		m_apLogoPause[m_nSelect]->SetCol(PAUSESELECT_COL_NONE);          // �F
		m_apLogoPause[m_nSelect]->SetWidth(PAUSESELECT_WIDTH_NONE);      // ��
		m_apLogoPause[m_nSelect]->SetHeight(PAUSESELECT_HEIGHT_NONE);    // ����

		// ���ɑI������鍀�ڂ̐ݒ�
		m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetState(CScene2DFlash::STATE_SELECT);  // ���
		m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetCol(PAUSESELECT_COL_INI);            // �F
		m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetWidth(PAUSESELECT_WIDTH_INI);        // ��
		m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetHeight(PAUSESELECT_HEIGHT_INI);      // ����

		// �ԍ���ݒ�
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;

		// ����炷
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_3) == TRUE)
	{// ����{�^���������ꂽ
		// �����_�����O�N���X�̎擾
		CRenderer *pRenderer = CManager::GetRenderer();

		switch (m_nSelect)
		{// ���ݑI������Ă������ڂɂ���ď����킯
		case CPause::SELECT_CONTINUE:  // �ĊJ
			pRenderer->SetPause(false);
			break;
		case CPause::SELECT_RETRY:     // ��蒼��
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
			break;
		case CPause::SELECT_QUIT:      // �I��
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
			break;
		}

		// �|�[�Y�̊J��
		pRenderer->ReleasePause();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CPause::Draw(void)
{

}

//=============================================================================
//    �I�����ڗp�̃|���S���N���X�ւ̃|�C���^�擾����
//=============================================================================
CScene2DFlash *CPause::GetScene2DFlash(int nIdx)
{
	return m_apLogoPause[nIdx];
}

//=============================================================================
//    �|�[�Y�w�i�p�̃|���S���N���X�ւ̃|�C���^�擾����
//=============================================================================
CScene2D *CPause::GetPauseBg(void)
{
	return m_pPauseBg;
}

//=============================================================================
//    �I�����ڂ̎擾
//=============================================================================
int CPause::GetSelect(void)
{
	return m_nSelect;
}