//*****************************************************************************
//
//     �`���[�g���A���̏���[tutorial.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "scene2DSquare.h"

#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define TUTORIAL_TEXTURENAME_0 "data/TEXTURE/TUTORIAL/tutorial.jpg"
#define TUTORIAL_TEXTURENAME_1 "data/TEXTURE/TUTORIAL/TUTORIAL.png"

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTutorial::m_apTexture[2] = {};  // �`���[�g���A���̃e�N�X�`���摜

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CTutorial::CTutorial(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	for (int nCntLogo = 0; nCntLogo < 2; nCntLogo++)
	{
		m_apSceneSquare[nCntLogo] = NULL;
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CTutorial::~CTutorial()
{

}

//=============================================================================
//    ��������
//=============================================================================
CTutorial *CTutorial::Create(void)
{
	CTutorial *pTutorial = NULL;  // �`���[�g���A���N���X�^�̃|�C���^
	if (pTutorial == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pTutorial = new CTutorial;
		if (pTutorial != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pTutorial->Init();
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
	return pTutorial;
}

//=============================================================================
//    �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CTutorial::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			char aTexFileName[2][256] =
			{
				TUTORIAL_TEXTURENAME_0,
				TUTORIAL_TEXTURENAME_1,
			};
			for (int nCntTex = 0; nCntTex < 2; nCntTex++)
			{
				D3DXCreateTextureFromFile(pDevice, aTexFileName[nCntTex], &m_apTexture[nCntTex]);
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`���̉������
//=============================================================================
void CTutorial::UnLoad(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < 2; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CTutorial::Init(void)
{
	// �e�N�X�`���̓ǂݍ���
	Load();

	// �ꖇ�G�̐���
	if (m_apSceneSquare[0] == NULL)
	{
		m_apSceneSquare[0] = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
		if (m_apSceneSquare[0] != NULL)
		{
			m_apSceneSquare[0]->BindTexture(m_apTexture[0]);
		}
	}

	// ���S�̐���
	if (m_apSceneSquare[1] == NULL)
	{
		m_apSceneSquare[1] = CScene2DSquare::Create(D3DXVECTOR3(1060.0f, 660.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 230.0f, 60.0f);
		if (m_apSceneSquare[1] != NULL)
		{
			m_apSceneSquare[1]->BindTexture(m_apTexture[1]);
		}
	}


	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CTutorial::Uninit(void)
{
	// �e�N�X�`���̊J��
	UnLoad();

	// �|���S���̊J��
	for (int nCntLogo = 0; nCntLogo < 2; nCntLogo++)
	{
		if (m_apSceneSquare[nCntLogo] != NULL)
		{
			m_apSceneSquare[nCntLogo]->Uninit();
			m_apSceneSquare[nCntLogo] = NULL;
		}
	}

	// �`���[�g���A���I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CTutorial::Update(void)
{
	if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
	{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// ����{�^���������ꂽ
				CManager::GetFade()->SetFade(CManager::MODE_GAME);
			}
			else
			{// �L�[�{�[�h�̌���{�^����������Ă��Ȃ�
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_12; nCntButton++)
				{// ��������{�^�����̐������J��Ԃ�
					if (CManager::GetJoyStick()->GetJoyStickDevice(0)->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// �{�^���������ꂽ
						CManager::GetFade()->SetFade(CManager::MODE_GAME);
					}
				}
			}
		}
	}

	CDebugProc::Print("�`���[�g���A���Ȃ�\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CTutorial::Draw(void)
{

}