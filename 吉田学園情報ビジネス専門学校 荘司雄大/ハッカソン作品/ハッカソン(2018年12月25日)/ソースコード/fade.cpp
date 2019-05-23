//*****************************************************************************
//
//     �t�F�[�h�̏���[fade.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "scene2D.h"
#include "game.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define FADE_STOP_PERIOD    (60)                             // �t�F�[�h���ǂꂾ���~�߂邩
#define FADE_PRIORITY       (MAX_PRIORITY_NUM - 1)           // �t�F�[�h�̕`��D�揇��
#define FADE_OUT_ALPHA      (0.02f)                          // �t�F�[�h�A�E�g����Ƃ��̃��l�̌��Z�l
#define FADE_IN_ALPHA       (-0.02f)                         // �t�F�[�h�C������Ƃ��̃��l�̉��Z�l
#define FADE_COLOR_INI      (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f)) // �t�F�[�h�|���S���̐F(�����l)
#define FADE_COLOR_START    (D3DXCOLOR(0.0f,0.0f,0.0f,0.0f)) // �t�F�[�h�|���S���̐F(�X�^�[�g��)

#define FADE_PERIOD_WIDTH   (300.0f)                         // �������|���S���̕�
#define FADE_PERIOD_HEIGHT  (100.0f)                         // �������|���S���̍���

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CFade::CFade()
{
	// �e��l�̃N���A
	m_pScene2D = NULL;                 // �t�F�[�h�Ɏg�p����|���S��
	m_State = STATE_IN;                // ��Ԃ̓t�F�[�h�C����Ԃ�
	m_Type = TYPE_MODECHANGE;          // ��ނ͉�ʑJ�ڐ؂�ւ���
	m_NextMode = CManager::MODE_TITLE; // ���[�h�̓^�C�g����Ԃ�
	m_nCounter = 0;                    // ��Ԃ��Ǘ�����J�E���^�[
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CFade::Init(void)
{
	// �t�F�[�h�Ɏg�p����|���S���𐶐�
	if (m_pScene2D == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR_INI, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, FADE_PRIORITY);
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CFade::Uninit(void)
{
	if (m_pScene2D != NULL)
	{// ���������m�ۂ���Ă���
	    // �I������
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CFade::Update(void)
{
	if (m_State != STATE_NONE)
	{// �t�F�[�h���g�����ԂȂ�
		if (m_pScene2D != NULL)
		{// �t�F�[�h�Ɏg�p����|���S�����m�ۂ���Ă���
			if (m_State == STATE_IN)
			{// �t�F�[�h�C����ԂȂ��
				FadeIn();
			}
			else if (m_State == STATE_STOP)
			{// �t�F�[�h���~�߂��Ԃ�������
				FadeStop();
			}
			else if(m_State == STATE_OUT)
			{// �t�F�[�h�A�E�g��ԂȂ��
				FadeOut();
			}
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CFade::Draw(void)
{

}

//=============================================================================
//    �t�F�[�h�̐ݒ�
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	if (m_State == STATE_NONE)
	{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
		m_nCounter = 0;         // �J�E���^�[��߂��Ă���
		m_NextMode = modeNext;  // ���̃��[�h��ݒ�
		m_State = STATE_OUT;    // ��Ԃ��t�F�[�h�A�E�g��Ԃ�
		m_Type = TYPE_MODECHANGE;

		// �t�F�[�h�Ɏg�p����|���S���𐶐�
		if (m_pScene2D == NULL)
		{// ���������m�ۂł����Ԃɂ���
			D3DXCOLOR col = FADE_COLOR_START;
			col.a = 0.0f;
			m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR_START, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, FADE_PRIORITY);
		}
	}
}

//=============================================================================
//    �t�F�[�h�̐ݒ�(��ʐ؂�ւ��ȊO�̏ꍇ)
//=============================================================================
void CFade::SetFade(TYPE type)
{
	if (m_State == STATE_NONE)
	{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
		m_nCounter = 0;           // �J�E���^�[��߂��Ă���
		m_State = STATE_OUT;      // ��Ԃ��t�F�[�h�A�E�g��Ԃ�
		m_Type = type;            // ��ނ�ݒ�

		// �t�F�[�h�Ɏg�p����|���S���𐶐�
		if (m_pScene2D == NULL)
		{// ���������m�ۂł����Ԃɂ���
			D3DXCOLOR col = FADE_COLOR_START;
			col.a = 0.0f;
			m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR_START, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, FADE_PRIORITY);
		}
	}
}

//=============================================================================
//    �t�F�[�h�C����Ԃ̍X�V����
//=============================================================================
void CFade::FadeIn(void)
{
	if (m_pScene2D != NULL)
	{// ���������m�ۂ���Ă���
	     // ���݂̐F���擾
		D3DXCOLOR col = m_pScene2D->GetCol();

		// �����x�����Z����
		col.a += FADE_IN_ALPHA;

		// �F�̐ݒ�
		m_pScene2D->SetCol(col);

		if (col.a <= 0.0f)
		{// �����ɂȂ�؂���
			m_State = STATE_NONE;  // ��Ԃ�߂�
			// �g�p�����|���S�����J������
			if (m_pScene2D != NULL)
			{// ���������m�ۂ���Ă���
				// �I������
				m_pScene2D->Uninit();
				m_pScene2D = NULL;
			}
		}
	}
}

//=============================================================================
//    �t�F�[�h���~�߂��Ԃ̍X�V����
//=============================================================================
void CFade::FadeStop(void)
{
	m_nCounter++;
	if (m_nCounter >= FADE_STOP_PERIOD)
	{// �t�F�[�h�I����
	    // �g�p�����|���S�����J������
		if (m_pScene2D != NULL)
		{// ���������m�ۂ���Ă���
		    // �I������
			m_pScene2D->Uninit();
			m_pScene2D = NULL;
		}

		if (m_Type == TYPE_MODECHANGE)
		{// ��ʑJ�ڂ�����t�F�[�h��������
		    // ���̃��[�h�ɑJ��
			CManager::SetMode(m_NextMode);
		}
		else if (m_Type == TYPE_SWITCHMAP)
		{// �}�b�v�؂�ւ���ԂȂ��
			CGame *pGame = CManager::GetGame();
			if (pGame != NULL)
			{// �Q�[�����擾�ł���
				pGame->SetMap();
			}
		}

		// �t�F�[�h�Ɏg�p����|���S�����m�ۂ��Ȃ���
		if (m_pScene2D == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR_INI, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, FADE_PRIORITY);
		}

		// �t�F�[�h�C����Ԃ�
		m_State = STATE_IN;
	}
}

//=============================================================================
//    �t�F�[�h�A�E�g��Ԃ̍X�V����
//=============================================================================
void CFade::FadeOut(void)
{
	if (m_pScene2D != NULL)
	{// ���������m�ۂ���Ă���
	    // ���݂̐F���擾
		D3DXCOLOR col = m_pScene2D->GetCol();

		// �����x�����Z����
		col.a += FADE_OUT_ALPHA;

		if (col.a >= 1.0f)
		{// �����ɂȂ�؂���
			m_State = STATE_STOP;      // ��Ԃ��t�F�[�h���~�߂��Ԃ�
			col.a = 1.0f;              // �F��߂�
		}

		// �F�̐ݒ�
		m_pScene2D->SetCol(col);
	}
}

//=============================================================================
//    �t�F�[�h�̏�Ԃ��擾
//=============================================================================
CFade::STATE CFade::GetState(void)
{
	return m_State;
}