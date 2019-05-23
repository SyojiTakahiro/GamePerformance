//*****************************************************************************
//
//     �t�F�[�h�̏���[fade.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "scene2DSquare.h"

#include "game.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define FADE_TEXTURENAME    "data/TEXTURE/COMMON/fadelogo.png"   // �e�N�X�`���̃t�@�C����(������)

#define FADE_STOP_PERIOD    (60)                             // �t�F�[�h���ǂꂾ���~�߂邩
#define FADE_PRIORITY       (MAX_PRIORITY_NUM - 1)           // �t�F�[�h�̕`��D�揇��
#define FADE_OUT_ALPHA      (0.02f)                          // �t�F�[�h�A�E�g����Ƃ��̃��l�̌��Z�l
#define FADE_IN_ALPHA       (-0.02f)                         // �t�F�[�h�C������Ƃ��̃��l�̉��Z�l
#define FADE_COLOR          (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f)) // �t�F�[�h�|���S���̐F

#define FADE_PERIOD_WIDTH   (300.0f)                         // �������|���S���̕�
#define FADE_PERIOD_HEIGHT  (150.0f)                         // �������|���S���̍���

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CFade::m_pTexture = NULL;  // �e�N�X�`���ւ̃|�C���^

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CFade::CFade()
{
	// �e��l�̃N���A
	m_pScene2DSquare = NULL;           // �t�F�[�h�Ɏg�p����|���S��
	m_State = STATE_IN;                // ��Ԃ̓t�F�[�h�C����Ԃ�
	m_Type = TYPE_MODECHANGE;          // ��ނ͉�ʑJ�ڐ؂�ւ���
	m_NextMode = CManager::MODE_TITLE; // ���[�h�̓^�C�g����Ԃ�
	m_pPreparation = NULL;             // �������̃|���S��
	m_nCounter = 0;                    // ��Ԃ��Ǘ�����J�E���^�[
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CFade::~CFade()
{

}

//=============================================================================
//    ���O�̃e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CFade::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			D3DXCreateTextureFromFile(pDevice, FADE_TEXTURENAME, &m_pTexture);
		}
	}
	return S_OK;
}

//=============================================================================
//    ���O�̃e�N�X�`���������
//=============================================================================
void CFade::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CFade::Init(void)
{
	// �t�F�[�h�Ɏg�p����|���S���𐶐�
	if (m_pScene2DSquare == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pScene2DSquare = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, FADE_PRIORITY);
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	// �������|���S���𐶐�
	if (m_pPreparation == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pPreparation = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH - FADE_PERIOD_WIDTH - 10.0f, SCREEN_HEIGHT - FADE_PERIOD_HEIGHT - 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FADE_PERIOD_WIDTH, FADE_PERIOD_HEIGHT, FADE_PRIORITY);
		if (m_pPreparation != NULL)
		{// ���������m�ۂł���
			// �e�N�X�`���̊��蓖��
			m_pPreparation->BindTexture(m_pTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CFade::Uninit(void)
{
	if (m_pScene2DSquare != NULL)
	{// ���������m�ۂ���Ă���
	    // �I������
	    m_pScene2DSquare->Uninit();
		m_pScene2DSquare = NULL;
	}

	if (m_pPreparation != NULL)
	{// ���������m�ۂ���Ă���
	    // �I������
		m_pPreparation->Uninit();
		m_pPreparation = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CFade::Update(void)
{
	if (m_State != STATE_NONE)
	{// �t�F�[�h���g�����ԂȂ�
		if (m_pScene2DSquare != NULL)
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
//    �t�F�[�h�̐ݒ�(��ʐ؂�ւ���p)
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	if (m_State == STATE_NONE)
	{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
		m_nCounter = 0;           // �J�E���^�[��߂��Ă���
		m_NextMode = modeNext;    // ���̃��[�h��ݒ�
		m_State = STATE_OUT;      // ��Ԃ��t�F�[�h�A�E�g��Ԃ�
		m_Type = TYPE_MODECHANGE; // ��ނ���ʑJ�ڐ؂�ւ���

		// �t�F�[�h�Ɏg�p����|���S���𐶐�
		if (m_pScene2DSquare == NULL)
		{// ���������m�ۂł����Ԃɂ���
			D3DXCOLOR col = FADE_COLOR;
			col.a = 0.0f;
			m_pScene2DSquare = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), col, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, FADE_PRIORITY);
		}

		// �������|���S���𐶐�
		if (m_pPreparation == NULL)
		{// ���������m�ۂł����Ԃɂ���
			D3DXCOLOR colPre = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			m_pPreparation = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH - FADE_PERIOD_WIDTH - 10.0f, SCREEN_HEIGHT - FADE_PERIOD_HEIGHT - 10.0f, 0.0f), colPre, FADE_PERIOD_WIDTH, FADE_PERIOD_HEIGHT,FADE_PRIORITY);
			if (m_pPreparation != NULL)
			{// ���������m�ۂł���
			    // �e�N�X�`���̊��蓖��
				m_pPreparation->BindTexture(m_pTexture);
			}
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
		if (m_pScene2DSquare == NULL)
		{// ���������m�ۂł����Ԃɂ���
			D3DXCOLOR col = FADE_COLOR;
			col.a = 0.0f;
			m_pScene2DSquare = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), col, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, FADE_PRIORITY);
		}

		// �������|���S���𐶐�
		if (m_pPreparation == NULL)
		{// ���������m�ۂł����Ԃɂ���
			D3DXCOLOR colPre = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
			m_pPreparation = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH - FADE_PERIOD_WIDTH - 10.0f, SCREEN_HEIGHT - FADE_PERIOD_HEIGHT - 10.0f, 0.0f), colPre, FADE_PERIOD_WIDTH, FADE_PERIOD_HEIGHT, FADE_PRIORITY);
			if (m_pPreparation != NULL)
			{// ���������m�ۂł���
			 // �e�N�X�`���̊��蓖��
				m_pPreparation->BindTexture(m_pTexture);
			}
		}
	}
}


//=============================================================================
//    �t�F�[�h�C����Ԃ̍X�V����
//=============================================================================
void CFade::FadeIn(void)
{
	// ���݂̐F���擾
	D3DXCOLOR col = m_pScene2DSquare->GetCol();
	D3DXCOLOR colPre = m_pPreparation->GetCol();

	// �����x�����Z����
	col.a += FADE_IN_ALPHA;
	colPre.a += FADE_IN_ALPHA;
	if (m_pScene2DSquare != NULL)
	{// ���������m�ۂ���Ă���
		m_pScene2DSquare->SetVtxBuffCol(col);
	}
	if (m_pPreparation != NULL)
	{// ���������m�ۂ���Ă���
		m_pPreparation->SetVtxBuffCol(colPre);
	}

	if (col.a <= 0.0f)
	{// �����ɂȂ�؂���
		m_State = STATE_NONE;  // ��Ԃ�߂�
		// �g�p�����|���S�����J������
		if (m_pScene2DSquare != NULL)
		{// ���������m�ۂ���Ă���
		    // �I������
			m_pScene2DSquare->Uninit();
			m_pScene2DSquare = NULL;
		}

		if (m_pPreparation != NULL)
		{// ���������m�ۂ���Ă���
		    // �I������
			m_pPreparation->Uninit();
			m_pPreparation = NULL;
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
		if (m_pScene2DSquare != NULL)
		{// ���������m�ۂ���Ă���
		    // �I������
			m_pScene2DSquare->Uninit();
			m_pScene2DSquare = NULL;
		}
		if (m_pPreparation != NULL)
		{// ���������m�ۂ���Ă���
		    // �I������
			m_pPreparation->Uninit();
			m_pPreparation = NULL;
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
		else if (m_Type == TYPE_RESTART)
		{// �Q�[�����X�^�[�g��ԂȂ��
			CGame *pGame = CManager::GetGame();
			if (pGame != NULL)
			{// �Q�[�����擾�ł���
				pGame->PlayerRespawnEvent();
				pGame->SetState(CGame::STATE_CHALLENGE);
			}
		}

		// �t�F�[�h�Ɏg�p����|���S�����m�ۂ��Ȃ���
		if (m_pScene2DSquare == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_pScene2DSquare = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, FADE_PRIORITY);
		}

		// �������|���S���𐶐�
		if (m_pPreparation == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_pPreparation = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH - FADE_PERIOD_WIDTH - 10.0f, SCREEN_HEIGHT - FADE_PERIOD_HEIGHT - 10.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FADE_PERIOD_WIDTH, FADE_PERIOD_HEIGHT, FADE_PRIORITY);
			if (m_pPreparation != NULL)
			{// ���������m�ۂł���
			    // �e�N�X�`���̊��蓖��
				m_pPreparation->BindTexture(m_pTexture);
			}
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
	// ���݂̐F���擾
	D3DXCOLOR col = m_pScene2DSquare->GetCol();
	D3DXCOLOR colPre = m_pPreparation->GetCol();

	// �����x�����Z����
	col.a += FADE_OUT_ALPHA;
	colPre.a += FADE_OUT_ALPHA;
	if (m_pScene2DSquare != NULL)
	{// ���������m�ۂ���Ă���
		m_pScene2DSquare->SetVtxBuffCol(col);
	}
	if (m_pPreparation != NULL)
	{// ���������m�ۂ���Ă���
		m_pPreparation->SetVtxBuffCol(colPre);
	}

	if (col.a >= 1.0f)
	{// �����ɂȂ�؂���
		m_State = STATE_STOP;      // ��Ԃ��t�F�[�h���~�߂��Ԃ�
		col.a = 1.0f;              // �F��߂�
	}
}

//=============================================================================
//    �t�F�[�h�̏�Ԃ��擾
//=============================================================================
CFade::STATE CFade::GetState(void)
{
	return m_State;
}