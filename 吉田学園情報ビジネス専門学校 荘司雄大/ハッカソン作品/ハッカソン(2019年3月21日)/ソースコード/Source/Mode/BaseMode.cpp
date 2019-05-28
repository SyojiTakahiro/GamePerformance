//*****************************************************************************
//
//     ���[�h�x�[�X�̏���[BaseMode.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mode.h"
#include "manager.h"
#include "system.h"
#include "2DPolygon.h"
#include "character.h"
#include "effect.h"
#include "map.h"
#include "textureManager.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBaseMode::CBaseMode()
{
	// �e��l�̃N���A
	m_pPress = NULL;          // PRESSENTER�N���X�ւ̃|�C���^
	m_pTextureManager = NULL; // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_nModeCounter = 0;       // ���[�h�J�E���^�[(��ɉ�ʑJ�ڂɎg�p)
	m_bModeChange = false;    // ��ʑJ�ڂ����Ă��邩�ǂ���
	m_pPlayerManager = NULL;  // �v���C���[�Ǌ��N���X�ւ̃|�C���^
	m_pPlayer = NULL;         // �v���C���[�N���X�ւ̃|�C���^
	m_pEffectManager = NULL;  // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	m_pMap = NULL;            // �}�b�v�Ǌ��N���X�ւ̃|�C���^
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA_NUM; nCntCamera++)
	{// �J�����̍ő吔���J��Ԃ�
		m_pCamera[nCntCamera] = NULL;  // �J�����N���X�ւ̃|�C���^
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBaseMode::~CBaseMode()
{

}

//=============================================================================
//    �I������
//=============================================================================
void CBaseMode::Uninit(void)
{
	// �v���C���[�Ǌ����J������
	ReleasePlayerManager();

	// �G�t�F�N�g�Ǌ��N���X���J������
	ReleaseEffectManager();

	// �}�b�v�Ǌ��N���X���J������
	ReleaseMap();

	// �J�����N���X���J������
	ReleaseCamera();
}

//=============================================================================
//    ��ʑJ�ڂ��邩�ǂ����`�F�b�N���鏈��
//=============================================================================
void CBaseMode::ModeChange(int nAutoNext, int nPressNext, int nChangeTiming)
{
	if (m_bModeChange == false)
	{// ��ʑJ�ڂ����Ă��Ȃ�
		// �J�E���^�[��i�߂�
		m_nModeCounter++;

		if (m_nModeCounter >= nChangeTiming)
		{// �J�E���^�[������̒l�𒴂���
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
				CManager::GetFade()->SetFade(nAutoNext);
				m_bModeChange = true;
			}
		}
		else
		{// �J�E���^�[���܂�����̒l�𒴂��Ă��Ȃ�
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
				if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
				{// ����{�^���������ꂽ
					CManager::GetFade()->SetFade(nPressNext);
					if (m_pPress != NULL)
					{// ���������m�ۂ���Ă���
						m_pPress->FlashStart();
					}
					m_bModeChange = true;
					//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
				}
				else
				{// �L�[�{�[�h�̌���{�^����������Ă��Ȃ�
					for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_7; nCntButton++)
					{// ��������{�^�����̐������J��Ԃ�
						if (CManager::GetJoyStick()->GetJoyStickDevice()->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
						{// �{�^���������ꂽ
							CManager::GetFade()->SetFade(nPressNext);
							if (m_pPress != NULL)
							{// ���������m�ۂ���Ă���
								m_pPress->FlashStart();
							}
							m_bModeChange = true;
							//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
						}
					}
				}
			}
		}
	}
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X��j������
//=============================================================================
void CBaseMode::ReleaseTextureManager(void)
{
	if (m_pTextureManager != NULL)
	{// ���������m�ۂ���Ă���
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}
}

//=============================================================================
//    �v���C���[�Ǌ��N���X��j������
//=============================================================================
void CBaseMode::ReleasePlayerManager(void)
{
	if (m_pPlayerManager != NULL)
	{// ���������m�ۂ���Ă���
		m_pPlayerManager->Uninit();

		// �������̊J��
		delete m_pPlayerManager;
		m_pPlayerManager = NULL;
	}
}

//=============================================================================
//    �G�t�F�N�g�Ǌ��N���X��j������
//=============================================================================
void CBaseMode::ReleaseEffectManager(void)
{
	if (m_pEffectManager != NULL)
	{// ���������m�ۂ���Ă���
		m_pEffectManager->Uninit();

		// �������̊J��
		delete m_pEffectManager;
		m_pEffectManager = NULL;
	}
}

//=============================================================================
//    �}�b�v�Ǌ��N���X��j������
//=============================================================================
void CBaseMode::ReleaseMap(void)
{
	if (m_pMap != NULL)
	{// ���������m�ۂ���Ă���
		m_pMap->Uninit();

		// �������̊J��
		delete m_pMap;
		m_pMap = NULL;
	}
}

//=============================================================================
//    �J�����N���X��j������
//=============================================================================
void CBaseMode::ReleaseCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < MAX_CAMERA_NUM; nCntCamera++)
	{// �J�����̍ő吔���J��Ԃ�
		if (m_pCamera[nCntCamera] != NULL)
		{// ���������m�ۂ���Ă���
			m_pCamera[nCntCamera]->Uninit();

			// �������̊J��
			delete 	m_pCamera[nCntCamera];
			m_pCamera[nCntCamera] = NULL;
		}
	}
}

//=============================================================================
//    ���[�h�J�E���^�[��ݒ肷��
//=============================================================================
void CBaseMode::SetModeCounter(const int nModeCounter)
{
	m_nModeCounter = nModeCounter;
}

//=============================================================================
//    PRESSENTER�N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CBaseMode::SetPress(CScene2DPress *pPress)
{
	m_pPress = pPress;
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CBaseMode::SetTextureManager(CTextureManager *pTextureManager)
{
	m_pTextureManager = pTextureManager;
}

//=============================================================================
//    ��ʑJ�ڂ����Ă��邩�ǂ�����ݒ肷��
//=============================================================================
void CBaseMode::SetModeChange(const bool bModeChange)
{
	m_bModeChange = bModeChange;
}

//=============================================================================
//    �v���C���[�Ǌ��N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CBaseMode::SetPlayerManager(CPlayerManager *pPlayerManager)
{
	m_pPlayerManager = pPlayerManager;
}

//=============================================================================
//    �v���C���[�N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CBaseMode::SetPlayer(CPlayer *pPlayer)
{
	m_pPlayer = pPlayer;
}

//=============================================================================
//    �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CBaseMode::SetEffectManager(CEffectManager *pEffectManager)
{
	m_pEffectManager = pEffectManager;
}

//=============================================================================
//    �}�b�v�Ǌ��N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CBaseMode::SetMap(CMap *pMap)
{
	m_pMap = pMap;
}

//=============================================================================
//    �J�����N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CBaseMode::SetCamera(CCamera *pCamera, int nIdx)
{
	m_pCamera[nIdx] = pCamera;
}

//=============================================================================
//    ���[�h�J�E���^�[���擾����
//=============================================================================
int CBaseMode::GetModeCounter(void)
{
	return m_nModeCounter;
}

//=============================================================================
//    PRESSENTER�N���X�ւ̃|�C���^���擾����
//=============================================================================
CScene2DPress *CBaseMode::GetPress(void)
{
	return m_pPress;
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CTextureManager *CBaseMode::GetTextureManager(void)
{
	return m_pTextureManager;
}

//=============================================================================
//    ��ʑJ�ڂ����Ă��邩�ǂ�����ݒ肷��
//=============================================================================
bool CBaseMode::GetModeChange(void)
{
	return m_bModeChange;
}

//=============================================================================
//    �v���C���[�Ǌ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CPlayerManager *CBaseMode::GetPlayerManager(void)
{
	return m_pPlayerManager;
}

//=============================================================================
//    �v���C���[�N���X�ւ̃|�C���^���擾����
//=============================================================================
CPlayer *CBaseMode::GetPlayer(void)
{
	return m_pPlayer;
}

//=============================================================================
//    �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CEffectManager *CBaseMode::GetEffectManager(void)
{
	return m_pEffectManager;
}

//=============================================================================
//    �}�b�v�Ǌ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CMap *CBaseMode::GetMap(void)
{
	return m_pMap;
}

//=============================================================================
//    �J�����N���X�ւ̃|�C���^���擾����
//=============================================================================
CCamera *CBaseMode::GetCamera(int nIdx)
{
	return m_pCamera[nIdx];
}