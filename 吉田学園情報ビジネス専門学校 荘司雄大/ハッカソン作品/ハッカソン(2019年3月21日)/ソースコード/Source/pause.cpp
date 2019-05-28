//*****************************************************************************
//
//     �|�[�Y�̏���[pause.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "pause.h"
#include "system.h"
#include "manager.h"
#include "2DPolygon.h"
#include "mode.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define PAUSEBLACK_COL_INI       (D3DXCOLOR(0.0f,0.0f,0.0f,0.7f))                 // �|�[�Y�w�i(�����|���S��)�̐F(�����l)

#define PAUSEBG_TEXTURE_NAME     "data/TEXTURE/GAME/pause_frame.png"              // �|�[�Y�̔w�i�̃e�N�X�`���t�@�C���p�X��
#define PAUSEBG_POS_INI          (D3DXVECTOR3(SCREEN_WIDTH / 2,180.0f,0.0f))      // �|�[�Y�̔w�i�̍��W
#define PAUSEBG_WIDTH_INI        (150.0f)                                         // �|�[�Y�̔w�i�̕�
#define PAUSEBG_HEIGHT_INI       (150.0f)                                         // �|�[�Y�̔w�i�̍���

#define PAUSESELECT_POS_INI      (D3DXVECTOR3(SCREEN_WIDTH / 2,80.0f,0.0f))       // �|�[�Y���ڂ̍��W(�����l)
#define PAUSESELECT_COL_INI      (D3DXCOLOR(0.5f,0.5f,0.5f,1.0f))                 // �|�[�Y���ڂ̐F(�����l)
#define PAUSESELECT_WIDTH_INI    (130.0f)                                         // �I������Ă���|�[�Y�I�����ڂ̕�
#define PAUSESELECT_HEIGHT_INI   (50.0f)                                          // �I������Ă���|�[�Y�I�����ڃ|�[�Y�I�����ڂ̍���
#define PAUSESELECT_WIDTH_NONE   (90.0f)                                          // �I������Ă��Ȃ��|�[�Y�I�����ڂ̕�
#define PAUSESELECT_HEIGHT_NONE  (30.0f)                                          // �I������Ă��Ȃ��|�[�Y�I�����ڂ̍���
#define PAUSESELECT_COL_NONE     (D3DXCOLOR(0.1f,0.1f,0.1f,1.0f))                 // �I������Ă��Ȃ��|�[�Y�I�����ڂ̐F(�����l)
#define PAUSESELECT_INTERVAL     (100.0f)                                         // �|�[�Y�I�����ړ��m�̊Ԋu

// ������@�p�|���S��1��
#define PAUSE_OPERATION_POS_0    (D3DXVECTOR3(330.0f,540.0f,0.0f))                // ������@�p�̃|���S���̍��W(1��)
#define PAUSE_OPERATION_COL_0    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                 // ������@�p�̃|���S���̐F(1��)
#define PAUSE_OPERATION_WIDTH_0  (330.0f)                                         // ������@�p�̃|���S���̕�(1��)
#define PAUSE_OPERATION_HEIGHT_0 (180.0f)                                         // ������@�p�̃|���S���̍���(1��)

// ������@�p�|���S��2��
#define PAUSE_OPERATION_POS_1    (D3DXVECTOR3(950.0f,540.0f,0.0f))                // ������@�p�̃|���S���̍��W(2��)
#define PAUSE_OPERATION_COL_1    (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))                 // ������@�p�̃|���S���̐F(2��)
#define PAUSE_OPERATION_WIDTH_1  (330.0f)                                         // ������@�p�̃|���S���̕�(2��)
#define PAUSE_OPERATION_HEIGHT_1 (180.0f)                                         // ������@�p�̃|���S���̍���(2��)

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
	for (int nCntOpe = 0; nCntOpe < 2; nCntOpe++)
	{// ������@�p�̃|���S���̐������J��Ԃ�
		m_pOperationInfo[nCntOpe] = NULL;
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

	// ������@�p�̃|���S���̊J��
	for (int nCntOpe = 0; nCntOpe < 2; nCntOpe++)
	{// ������@�p�̃|���S���̐������J��Ԃ�
		if (m_pOperationInfo[nCntOpe] != NULL)
		{// ���������m�ۂ���Ă���
			m_pOperationInfo[nCntOpe]->Uninit();
			m_pOperationInfo[nCntOpe] = NULL;
		}
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
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(CManager::GetGame()->GetPauseController());   // �W���C�X�e�B�b�N�̎擾

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
		//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
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
		//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_3) == TRUE)
	{// ����{�^���������ꂽ
		CGame *pGame = CManager::GetGame();

		switch (m_nSelect)
		{// ���ݑI������Ă������ڂɂ���ď����킯
		case CPause::SELECT_CONTINUE:  // �ĊJ
			pGame->SetPause(false);
			break;
		case CPause::SELECT_RETRY:     // ��蒼��
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
			//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
			break;
		case CPause::SELECT_QUIT:      // �I��
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			pGame->SetNumPlayer(0);
			//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
			break;
		}

		// �|�[�Y�̊J��
		pGame->ReleasePause();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CPause::Draw(void)
{
	// �����|���S���̕`��
	if (m_pBlackBg != NULL)
	{// ���������m�ۂ���Ă���
		m_pBlackBg->Draw();
	}

	// �w�i�|���S���̕`��
	if (m_pPauseBg != NULL)
	{// ���������m�ۂ���Ă���
		m_pPauseBg->Draw();
	}

	// ������@�p�̃|���S���̊J��
	for (int nCntOpe = 0; nCntOpe < 2; nCntOpe++)
	{// ������@�p�̃|���S���̐������J��Ԃ�
		if (m_pOperationInfo[nCntOpe] != NULL)
		{// ���������m�ۂ���Ă���
			m_pOperationInfo[nCntOpe]->Draw();
		}
	}


	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// �|�[�Y��ʂ̑I�����ڂ̐������J��Ԃ�
		if (m_apLogoPause[nCntPause] != NULL)
		{// ���������m�ۂ���Ă���
		    // �|�[�Y���S�̕`��
			m_apLogoPause[nCntPause]->Draw();
		}
	}
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
//    ����@���@�\���p�|���S���N���X�ւ̃|�C���^�擾����
//=============================================================================
CScene2D *CPause::GetOpeInfo(int nIdx)
{
	return m_pOperationInfo[nIdx];
}

//=============================================================================
//    �I�����ڂ̎擾
//=============================================================================
int CPause::GetSelect(void)
{
	return m_nSelect;
}