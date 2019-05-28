//*****************************************************************************
//
//     �t�F�[�h�̏���[fade.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "system.h"
#include "manager.h"
#include "2DPolygon.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define FADE_LOGO_CREATE    (false)                               // �t�F�[�h�̃��S�|���S���𐶐����邩�ǂ���

#define FADE_LOGO_TEXNAME   "data/TEXTURE/COMMON/fade_logo.png"  // �t�F�[�h�Ɏg�p���镶���e�N�X�`���̑��΃p�X��
#define FADE_STOP_PERIOD    (60)                                 // �t�F�[�h���ǂꂾ���~�߂邩
#define FADE_PRIORITY       (MAX_PRIORITY_NUM - 1)               // �t�F�[�h�̕`��D�揇��
#define FADE_OUT_ALPHA      (0.02f)                              // �t�F�[�h�A�E�g����Ƃ��̃��l�̌��Z�l
#define FADE_IN_ALPHA       (-0.02f)                             // �t�F�[�h�C������Ƃ��̃��l�̉��Z�l
#define FADE_COLOR_INI      (D3DXCOLOR(0.0f,0.0f,0.0f,1.0f))     // �t�F�[�h�|���S���̐F(�����l)
#define FADE_COLOR_START    (D3DXCOLOR(0.0f,0.0f,0.0f,0.0f))     // �t�F�[�h�|���S���̐F(�X�^�[�g��)

#define FADE_LOGO_POS       (D3DXVECTOR3(960.0f,630.0f,0.0f))    // ���S�|���S���̍��W
#define FADE_LOGO_WIDTH     (300.0f)                             // ���S�|���S���̕�
#define FADE_LOGO_HEIGHT    (60.0f)                              // ���S�|���S���̍���

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CFade::m_pTexture = NULL;    // �����e�N�X�`���ւ̃|�C���^

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CFade::CFade()
{
	// �e��l�̃N���A
	m_pScene2D = NULL;                 // �t�F�[�h�Ɏg�p����|���S��
	m_pLogo2D = NULL;                  // �����|���S��
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
HRESULT CFade::Load(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), FADE_LOGO_TEXNAME, &m_pTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    ����������
//=============================================================================
void CFade::UnLoad(void)
{
	DIRECT_RELEASE(m_pTexture)
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CFade::Init(void)
{
	// �t�F�[�h�Ɏg�p����e�N�X�`����ǂݍ���
	CFade::Load();

	// �t�F�[�h�Ɏg�p����|���S���𐶐�
	if (m_pScene2D == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR_INI, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, FADE_PRIORITY);

		if (FADE_LOGO_CREATE == true)
		{// �t�F�[�h�Ɏg�p���镶���|���S���𐶐�����Ȃ��
			m_pLogo2D = CScene2D::Create(FADE_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FADE_LOGO_WIDTH, FADE_LOGO_HEIGHT, 0.0f, FADE_PRIORITY);
			if (m_pLogo2D != NULL)
			{// �����ł���
				m_pLogo2D->BindTexture(m_pTexture);
			}
		}
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

	if (m_pLogo2D != NULL)
	{// ���������m�ۂ���Ă���
	    // �I������
		m_pLogo2D->Uninit();
		m_pLogo2D = NULL;
	}

	// �t�F�[�h�Ɏg�p����e�N�X�`�����J��
	CFade::UnLoad();
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
void CFade::SetFade(int modeNext)
{
	if (m_State == STATE_NONE)
	{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
		m_nCounter = 0;                         // �J�E���^�[��߂��Ă���
		m_NextMode = (CManager::MODE)modeNext;  // ���̃��[�h��ݒ�
		m_State = STATE_OUT;                    // ��Ԃ��t�F�[�h�A�E�g��Ԃ�
		m_Type = TYPE_MODECHANGE;               // ��ނ���ʑJ�ڂɐݒ�

		// �t�F�[�h�Ɏg�p����|���S���𐶐�
		if (m_pScene2D == NULL)
		{// ���������m�ۂł����Ԃɂ���
			D3DXCOLOR col = FADE_COLOR_START;
			col.a = 0.0f;
			m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR_START, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, FADE_PRIORITY);

			if (FADE_LOGO_CREATE == true)
			{// �t�F�[�h�Ɏg�p���镶���|���S���𐶐�����Ȃ��
				m_pLogo2D = CScene2D::Create(FADE_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), FADE_LOGO_WIDTH, FADE_LOGO_HEIGHT, 0.0f, FADE_PRIORITY);
				if (m_pLogo2D != NULL)
				{// �����ł���
					m_pLogo2D->BindTexture(m_pTexture);
				}
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
		if (m_pScene2D == NULL)
		{// ���������m�ۂł����Ԃɂ���
			D3DXCOLOR col = FADE_COLOR_START;
			col.a = 0.0f;
			m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR_START, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, FADE_PRIORITY);

			if (FADE_LOGO_CREATE == true)
			{// �t�F�[�h�Ɏg�p���镶���|���S���𐶐�����Ȃ��
				m_pLogo2D = CScene2D::Create(FADE_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), FADE_LOGO_WIDTH, FADE_LOGO_HEIGHT, 0.0f, FADE_PRIORITY);
				if (m_pLogo2D != NULL)
				{// �����ł���
					m_pLogo2D->BindTexture(m_pTexture);
				}
			}
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

		if (m_pLogo2D != NULL)
		{// ��������������Ă���
		    // ���݂̐F���擾
			D3DXCOLOR LogoCol = m_pLogo2D->GetCol();

			// �����x�����Z����
			LogoCol.a += FADE_IN_ALPHA;

			// �F�̐ݒ�
			m_pLogo2D->SetCol(LogoCol);
		}

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
			if (m_pLogo2D != NULL)
			{// ���������m�ۂ���Ă���
			    // �I������
				m_pLogo2D->Uninit();
				m_pLogo2D = NULL;
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
		if (m_pLogo2D != NULL)
		{// ���������m�ۂ���Ă���
		    // �I������
			m_pLogo2D->Uninit();
			m_pLogo2D = NULL;
		}

		if (m_Type == TYPE_MODECHANGE)
		{// ��ʑJ�ڏ�ԂȂ��
			CManager::SetMode(m_NextMode);
		}
		//if (m_Type == TYPE_SWITCHMAP)
		//{// �}�b�v�؂�ւ���ԂȂ��
		//	CGame *pGame = CManager::GetGame();
		//	if (pGame != NULL)
		//	{// �Q�[�����擾�ł���
		//		pGame->SetMap();
		//	}
		//}

		// �t�F�[�h�Ɏg�p����|���S�����m�ۂ��Ȃ���
		if (m_pScene2D == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_pScene2D = CScene2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), FADE_COLOR_INI, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f, FADE_PRIORITY);
		}

		if (FADE_LOGO_CREATE == true)
		{// �t�F�[�h�Ɏg�p���镶���|���S���𐶐�����Ȃ��
			m_pLogo2D = CScene2D::Create(FADE_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FADE_LOGO_WIDTH, FADE_LOGO_HEIGHT, 0.0f, FADE_PRIORITY);
			if (m_pLogo2D != NULL)
			{// �����ł���
				m_pLogo2D->BindTexture(m_pTexture);
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

		if (m_pLogo2D != NULL)
		{// ��������������Ă���
		    // ���݂̐F���擾
			D3DXCOLOR LogoCol = m_pLogo2D->GetCol();

			// �����x�����Z����
			LogoCol.a += FADE_OUT_ALPHA;

			// �F�̐ݒ�
			m_pLogo2D->SetCol(LogoCol);
		}
	}
}

//=============================================================================
//    �t�F�[�h�̏�Ԃ��擾
//=============================================================================
CFade::STATE CFade::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �t�F�[�h�Ɏg�p����|���S���N���X���擾����N���X
//=============================================================================
CScene2D *CFade::GetScene2D(void)
{
	return m_pScene2D;
}