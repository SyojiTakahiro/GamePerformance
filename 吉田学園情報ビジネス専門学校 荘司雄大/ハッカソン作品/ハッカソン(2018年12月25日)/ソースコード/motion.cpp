//*****************************************************************************
//
//     ���[�V�����̏���[motion.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "motion.h"
#include "model.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define MOTION_SAVE_FILENAME "data/TEXT/key_info.txt"    // ���[�V�����f�[�^�������o���t�@�C����

// �e�L�X�g�t�@�C������ǂݍ��ލۂ̖ڈ��ƂȂ�p�X��
#define MOTIONSET        "MOTIONSET"
#define LOOP             "LOOP"
#define BLEND            "BLEND"
#define NUM_KEY          "NUM_KEY"
#define KEYSET           "KEYSET"
#define FRAME            "FRAME"
#define KEY              "KEY"
#define POS              "POS"
#define ROT              "ROT"
#define END_KEY          "END_KEY"
#define END_KEYSET       "END_KEYSET"
#define END_MOTIONSET    "END_MOTIONSET"

#define MAX_KEYFRAME     (20)     // �ǂݍ��߂�L�[�t���[���̍ő吔
#define MAX_MODEL_NUM    (50)     // �ǂݍ��߂郂�f���̍ő吔

//*****************************************************************************
//     CMotionAttack�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMotionAttack::CMotionAttack()
{
	// �e��l�̃N���A
	m_nNextMotionIdx = 0;      // ���̃��[�V�����ɔh������ԍ�
	m_nStartTiming = 0;        // ���̃��[�V�����ɔh���ł��鎞�Ԃ��n�܂�^�C�~���O
	m_nFinishTiming = 0;       // ���̃��[�V�����ɔh���ł��鎞�Ԃ��I���^�C�~���O
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMotionAttack::~CMotionAttack()
{

}


//*****************************************************************************
//     CMotionCollision�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMotionCollision::CMotionCollision()
{
	// �e��l�̃N���A
	m_nModelIdx = 0;                              // �����蔻�����郂�f���̔ԍ�
	m_OffSetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �����蔻�����郂�f������̃I�t�Z�b�g����
	m_fRange = 0.0f;                              // �����蔻������͈�
	m_nStartTiming = 0;                           // �����蔻������n�߂鎞��
	m_nFinishTiming = 0;                          // �����蔻������I��鎞��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMotionCollision::~CMotionCollision()
{

}

//*****************************************************************************
//     CMotionOrbit�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMotionOrbit::CMotionOrbit()
{
	// �e��l�̃N���A
	m_nModelIdx = 0;                                           // �O�Ղ��o�����f���̔ԍ�
	m_nTexIdx = 0;                                             // �O�Ղɓ\��e�N�X�`���̔ԍ�
	m_OffSetPos1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);              // �O�Ղ��o�����f������̃I�t�Z�b�g����(1�Ԗ�)
	m_OffSetPos2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);              // �O�Ղ��o�����f������̃I�t�Z�b�g����(2�Ԗ�)
	D3DXVECTOR3 m_OffSetAmp1 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �O�Ղ̃I�t�Z�b�g�����l(1�Ԗ�)
	D3DXVECTOR3 m_OffSetAmp2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �O�Ղ̃I�t�Z�b�g�����l(2�Ԗ�)
	m_ColUp = D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);                  // �O�Ղ̒��_�㑤�̐F
	m_ColDown = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);             // �O�Ղ̒��_�����̐F
	m_fAlphaDecayUp = 0.0f;                                    // �O�Ղ̒��_�㑤�̓����x�����l
	m_fAlphaDecayDown = 0.0f;                                  // �O�Ղ̒��_�㑤�̓����x�����l
	m_nXBlock = 0;                                             // �O�Ղ̉��̕�����
	m_nYBlock = 0;                                             // �O�Ղ̏c�̕�����
	m_nStartTiming = 0;                                        // �O�Ղ��o���n�߂�^�C�~���O
	m_nFinishTiming = 0;                                       // �O�Ղ����܂��n�߂�^�C�~���O
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMotionOrbit::~CMotionOrbit()
{

}


//*****************************************************************************
//     CMotionManager�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMotionManager::CMotionManager()
{
	// �e��l�̃N���A
	m_pMotion = NULL;        // ���[�V�����N���X�ւ̃|�C���^
	m_nNumber = 0;           // ���݂̃��[�V�����ԍ�
	m_nNumberOld = 0;        // �O��̃��[�V�����ԍ�
	m_nNumAll = 0;           // ���[�V�����̑���
	m_nKey = 0;              // ���݂̃L�[�t���[��
	m_nNumModel = 0;         // ���f����
	m_nBlendCounter = 0;     // �u�����h���Ǘ�����J�E���^�[
	m_nCounter = 0;          // ���[�V�����Ǘ��J�E���^�[
	m_nAllCounter = 0;       // ���݂̑��Đ��t���[����
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMotionManager::~CMotionManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMotionManager *CMotionManager::Create(int nNumMotion, int nNumModel)
{
	CMotionManager *pMotionManager = NULL;   // ���[�V�����Ǘ��N���X�^�̃|�C���^
	if (pMotionManager == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pMotionManager = new CMotionManager; // �C���X�^���X�𐶐�
		if (pMotionManager != NULL)
		{// ���������m�ۂł���
			if (FAILED(pMotionManager->Init(nNumMotion, nNumModel)))
			{// �������Ɏ��s����
				return NULL;
			}
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

	return pMotionManager;  // �|�C���^��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMotionManager::Init(int nNumMotion, int nNumModel)
{
	// �e��l�̑��
	m_nNumAll = nNumMotion;    // ���[�V�����̑���
	m_nNumModel = nNumModel;   // ���f���̑���
	m_nNumber = 0;             // ���݂̃��[�V�����ԍ�
	m_nNumberOld = m_nNumber;  // �O��̃��[�V�����ԍ�

	if (m_pMotion == NULL)
	{
		m_pMotion = new CMotion*[nNumMotion];
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CMotionManager::Uninit(void)
{
	if (m_pMotion != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntMotion = 0; nCntMotion < m_nNumAll; nCntMotion++)
		{// ���[�V�����̑������J��Ԃ�
			if (m_pMotion[nCntMotion] != NULL)
			{// ���������m�ۂ���Ă���
				// ���[�V�����̏I������
				m_pMotion[nCntMotion]->Uninit(m_nNumModel);

				// �������̊J��
				delete m_pMotion[nCntMotion];
				m_pMotion[nCntMotion] = NULL;
			}
		}
		// �������̊J��
		delete[] m_pMotion;
		m_pMotion = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CMotionManager::Update(CModel**pModel)
{
	if (m_pMotion[m_nNumber] != NULL)
	{// ���������m�ۂ���Ă���
		// ���[�V�����̍X�V����
		m_pMotion[m_nNumber]->Update(pModel, m_nNumModel, &m_nKey, &m_nCounter, &m_nBlendCounter, &m_nAllCounter);
	}
}

//=============================================================================
//    ���[�V�����̐؂�ւ�����
//=============================================================================
void CMotionManager::SwitchMotion(CModel **pModel, int nNumber)
{
	m_nCounter = 0;      // �J�E���^�[��߂�
	m_nBlendCounter = 0; // �u�����h�J�E���^�[��߂�
	m_nAllCounter = 0;   // ���Đ��t���[������߂�

	// �O��̃��[�V�����ԍ����L������Ԃ�߂�
	m_pMotion[m_nNumberOld]->SetState(CMotion::STATE_NORMAL);
	m_nNumberOld = m_nNumber;

	// ���[�V�����̔ԍ������킹��
	m_nNumber = nNumber;  // �ԍ������킹��
	m_nKey = 0;           // �L�[�t���[����߂�
	m_pMotion[m_nNumber]->SetState(CMotion::STATE_SWITCH);  // �؂�ւ���Ԃ�

	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{// ���f���̐������J��Ԃ�
		D3DXVECTOR3 DiffRot;  // �����̌���
		D3DXVECTOR3 DestPos;  // �ړI�̈ʒu

		// ���[�V�����̃I�t�Z�b�g�ړI�̈ʒu���v�Z
		DestPos.x = m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->GetAddPos().x - pModel[nCntModel]->GetAddPos().x;
		DestPos.y = m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->GetAddPos().y - pModel[nCntModel]->GetAddPos().y;
		DestPos.z = m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->GetAddPos().z - pModel[nCntModel]->GetAddPos().z;

		// X���̌����̍��������߂�
		DiffRot.x = m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->GetDestRot().x - pModel[nCntModel]->GetRot().x;
		if (DiffRot.x > D3DX_PI)
		{// ���݂̌�����D3DX_PI�𒴂���
			DiffRot.x -= D3DX_PI * 2.0f;
		}
		if (DiffRot.x < -D3DX_PI)
		{// ���݂̌�����-D3DX_PI�𒴂���
			DiffRot.x += D3DX_PI * 2.0f;
		}

		// Y���̌����̍��������߂�
		DiffRot.y = m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->GetDestRot().y - pModel[nCntModel]->GetRot().y;
		if (DiffRot.y > D3DX_PI)
		{// ���݂̌�����D3DX_PI�𒴂���
			DiffRot.y -= D3DX_PI * 2.0f;
		}
		if (DiffRot.y < -D3DX_PI)
		{// ���݂̌�����-D3DX_PI�𒴂���
			DiffRot.y += D3DX_PI * 2.0f;
		}

		// Z���̌����̍��������߂�
		DiffRot.z = m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->GetDestRot().z - pModel[nCntModel]->GetRot().z;
		if (DiffRot.z > D3DX_PI)
		{// ���݂̌�����D3DX_PI�𒴂���
			DiffRot.z -= D3DX_PI * 2.0f;
		}
		if (DiffRot.z < -D3DX_PI)
		{// ���݂̌�����-D3DX_PI�𒴂���
			DiffRot.z += D3DX_PI * 2.0f;
		}

		// �����̌�����ݒ肷��
		m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->SetDiffRot(DiffRot);
		// �ړI�̈ʒu��ݒ肷��
		m_pMotion[m_nNumber]->GetKeyframe()[m_nKey][nCntModel]->SetDestPos(DestPos);
	}
}

//=============================================================================
//    ���[�V�����N���X�ւ̃|�C���^��ݒ�
//=============================================================================
void CMotionManager::SetMotion(CMotion *pMotion, int nIdx)
{
	m_pMotion[nIdx] = pMotion;
}

//=============================================================================
//    ���݂̃L�[�t���[����ݒ�
//=============================================================================
void CMotionManager::SetKey(int nKey)
{
	m_nKey = nKey;
}

//=============================================================================
//    ���݂̑��Đ��t���[������ݒ�
//=============================================================================
void CMotionManager::SetAllCounter(int nAllCounter)
{
	m_nAllCounter = nAllCounter;
}

//=============================================================================
//    ���݂̃��[�V�����ԍ���ݒ�
//=============================================================================
void CMotionManager::SetNumber(int nNumber)
{
	m_nNumber = nNumber;
}

//=============================================================================
//    ���݂̃��[�V�����ԍ����擾
//=============================================================================
int CMotionManager::GetNumber(void)
{
	return m_nNumber;
}

//=============================================================================
//    ���f���̑������擾
//=============================================================================
int CMotionManager::GetNumModel(void)
{
	return m_nNumModel;
}

//=============================================================================
//    ���݂̃L�[�t���[�����擾
//=============================================================================
int CMotionManager::GetKey(void)
{
	return m_nKey;
}

//=============================================================================
//    ���݂̍Đ��t���[�����擾
//=============================================================================
int CMotionManager::GetCounter(void)
{
	return m_nCounter;
}

//=============================================================================
//    ���݂̑��Đ��t���[�����擾
//=============================================================================
int CMotionManager::GetAllCounter(void)
{
	return m_nAllCounter;
}

//=============================================================================
//    ���[�V�����N���X�ւ̃|�C���^�擾����
//=============================================================================
CMotion **CMotionManager::GetMotion(void)
{
	return m_pMotion;
}

//=============================================================================
//    ���[�V�����̃L�[�t���[�����ۑ�����
//=============================================================================
void CMotionManager::Save(void)
{
	FILE *pFile = NULL;   // �t�@�C���|�C���^

	if (pFile == NULL)
	{// �t�@�C���m�ۂł����Ԃł���
		pFile = fopen(MOTION_SAVE_FILENAME, "w");   // �t�@�C���I�[�v��
		if (pFile != NULL)
		{// �t�@�C�����J����
			fprintf(pFile, "#========================================\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "# ���[�V�������\n");
			fprintf(pFile, "#  [ ���[�V�����ԍ� : %d ]\n", m_nNumber);
			fprintf(pFile, "#\n");
			fprintf(pFile, "#========================================\n");
			fprintf(pFile, "\n");

			// ���[�V�����̏��ۑ��J�n
			fprintf(pFile, "%s\n", MOTIONSET);
			fprintf(pFile, "	%s = %d		# ���[�v���邩�ǂ���[0:���[�v���Ȃ� / 1:���[�v����]\n", LOOP, m_pMotion[m_nNumber]->GetLoop());
			fprintf(pFile, "	%s = %.2f	# �u�����h�̔{��\n", BLEND, m_pMotion[m_nNumber]->GetBlendMag());
			fprintf(pFile, "	%s = %d		# �L�[�t���[���̑���\n", NUM_KEY, m_pMotion[m_nNumber]->GetNumKey());

			for (int nCntKey = 0; nCntKey < m_pMotion[m_nNumber]->GetNumKey(); nCntKey++)
			{// �L�[�t���[���̑������J��Ԃ�
			    // �L�[�t���[���̏���ۑ�����
				fprintf(pFile, "\n	%s			# --- << KEY : %d / %d >> ---\n", KEYSET, nCntKey, m_pMotion[m_nNumber]->GetNumKey());
				fprintf(pFile, "		%s = %d\n", FRAME, m_pMotion[m_nNumber]->GetPlayBackKey(nCntKey));
				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{// ���f���̐������J��Ԃ�
					// ���f�����Ƃ̃I�t�Z�b�g����ۑ�
					fprintf(pFile, "		%s # ----- [ %d ] -----\n", KEY, nCntModel);
					fprintf(pFile, "			%s = %.2f %.2f %.2f\n", POS, m_pMotion[m_nNumber]->GetKeyframe()[nCntKey][nCntModel]->GetAddPos().x, m_pMotion[m_nNumber]->GetKeyframe()[nCntKey][nCntModel]->GetAddPos().y, m_pMotion[m_nNumber]->GetKeyframe()[nCntKey][nCntModel]->GetAddPos().z);
					fprintf(pFile, "			%s = %.2f %.2f %.2f\n", ROT, m_pMotion[m_nNumber]->GetKeyframe()[nCntKey][nCntModel]->GetDestRot().x, m_pMotion[m_nNumber]->GetKeyframe()[nCntKey][nCntModel]->GetDestRot().y, m_pMotion[m_nNumber]->GetKeyframe()[nCntKey][nCntModel]->GetDestRot().z);
					fprintf(pFile, "		%s\n\n", END_KEY);
				}
				fprintf(pFile, "	%s\n", END_KEYSET);
			}

			fprintf(pFile, "%s\n", END_MOTIONSET);
			fclose(pFile);   // �t�@�C�������
		}
	}
}


//*****************************************************************************
//     CMotion�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMotion::CMotion()
{
	// �e��l�̃N���A
	m_pMotionAttack = NULL;    // �U�����[�V�����f�[�^�N���X�ւ̃|�C���^
	m_pCollision = NULL;       // �����蔻��f�[�^�N���X�ւ̃|�C���^
	m_nNumColData = 0;         // �����蔻��f�[�^�N���X�̐�
	m_pOrbit = 0;              // �O�Ճf�[�^�N���X�ւ̃|�C���^
	m_nNumOrbitData = 0;       // �O�Ճf�[�^�̐�
	m_pKeyframe = NULL;        // �L�[�t���[���̏��
	m_pPlaybackKey = NULL;     // �Đ��t���[�����ւ̃|�C���^
	m_nAllPlaybackKey = 0;     // ���Đ��t���[����
	m_State = STATE_NORMAL;    // ���
	m_nNumKey = 0;             // �L�[�t���[���̐�
	m_fBlendMag = 0.0f;        // ���[�V�����u�����h�{��
	m_bLoop = false;           // ���[�v���邩���Ȃ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMotion::~CMotion()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMotion *CMotion::Create(int nNumModel, int nNumKey, float fBlendMag, bool bLoop, int *pPlaybackKey)
{
	CMotion *pMotion = NULL;   // ���[�V�����N���X�^�̃|�C���^
	if (pMotion == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pMotion = new CMotion; // �C���X�^���X�𐶐�
		if (pMotion != NULL)
		{// ���������m�ۂł���
			if (FAILED(pMotion->Init(nNumModel, nNumKey, fBlendMag, bLoop, pPlaybackKey)))
			{// �������Ɏ��s����
				return NULL;
			}
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

	return pMotion;  // �|�C���^��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMotion::Init(int nNumModel, int nNumKey, float fBlendMag, bool bLoop, int *pPlaybackKey)
{
	// �L�[�t���[���̏������f���̐��ƃL�[�t���[���������I�Ɋm��
	if (m_pKeyframe == NULL)
	{// �L�[�t���[���ւ̃|�C���^����ɂȂ��Ă���
		m_pKeyframe = new CKeyframe**[nNumKey];  // �L�[�t���[�����ւ̃|�C���^�̃|�C���^���L�[�t���[�������m��
		if (m_pKeyframe != NULL)
		{// ���������m�ۂł���
			for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
			{// �L�[�t���[���̐������J��Ԃ�
				m_pKeyframe[nCntKey] = new CKeyframe*[nNumModel];  // �L�[�t���[�����ւ̃|�C���^�����f���̐������m��
				if (m_pKeyframe[nCntKey] != NULL)
				{// ���������m�ۂł���
					for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
					{// �L�[�t���[���̐������J��Ԃ�
						m_pKeyframe[nCntKey][nCntModel] = new CKeyframe; // �L�[�t���[���̏��̃C���X�^���X�𐶐�
						if (m_pKeyframe[nCntKey][nCntModel] != NULL)
						{// ���������m�ۂł���
							// �L�[�t���[���̏�������������
							m_pKeyframe[nCntKey][nCntModel]->Init();
						}
					}
				}
			}
		}
	}

	if (m_pPlaybackKey == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pPlaybackKey = new int[nNumKey];  // �L�[�t���[���̐������Đ��t���[�����̏����C���X�^���X����
		if (m_pPlaybackKey != NULL)
		{// ���������m�ۂł���
			for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
			{// �L�[�t���[���̐������J��Ԃ�
				m_pPlaybackKey[nCntKey] = pPlaybackKey[nCntKey];
				m_nAllPlaybackKey += pPlaybackKey[nCntKey];
			}
		}
	}

	m_nNumKey = nNumKey;      // �L�[�t���[���̐�
	m_fBlendMag = fBlendMag;  // �u�����h�̔{��
	m_bLoop = bLoop;          // ���[�v���邩���Ȃ���

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CMotion::Uninit(int nNumModel)
{
	// �L�[�t���[���N���X�̊J��
	if (m_pKeyframe != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntKey = 0; nCntKey < m_nNumKey; nCntKey++)
		{// �L�[�t���[���̐������J��Ԃ�
			if (m_pKeyframe[nCntKey] != NULL)
			{// ���������m�ۂ���Ă���
				for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
				{// ���f���̐������J��Ԃ�
					if (m_pKeyframe[nCntKey][nCntModel] != NULL)
					{// ����������ɂȂ��Ă��Ȃ�
						// �I������
						m_pKeyframe[nCntKey][nCntModel]->Uninit();

						// �������̊J��
						delete m_pKeyframe[nCntKey][nCntModel];
						m_pKeyframe[nCntKey][nCntModel] = NULL;
					}
				}
				// �������̊J��
				delete[] m_pKeyframe[nCntKey];
				m_pKeyframe[nCntKey] = NULL;
			}
			if (m_pPlaybackKey!= NULL)
			{// ���������m�ۂ���Ă���
				delete[] m_pPlaybackKey;
				m_pPlaybackKey = NULL;
			}
		}
		// �������̊J��
		delete[] m_pKeyframe;
		m_pKeyframe = NULL;
	}

	// �U�����[�V�����f�[�^�̊J��
	if (m_pMotionAttack != NULL)
	{
		delete m_pMotionAttack;
		m_pMotionAttack = NULL;
	}

	// �����蔻��f�[�^�̊J��
	if (m_pCollision != NULL)
	{
		for (int nCntCol = 0; nCntCol < m_nNumColData; nCntCol++)
		{// �����蔻��f�[�^�̐������J��Ԃ�
			if (m_pCollision[nCntCol] != NULL)
			{
				delete m_pCollision[nCntCol];
				m_pCollision[nCntCol] = NULL;
			}
		}
		delete[] m_pCollision;
		m_pCollision = NULL;
	}

	// �O�Ճf�[�^�̊J��
	if (m_pOrbit != NULL)
	{
		for (int nCntOrbit = 0; nCntOrbit < m_nNumOrbitData; nCntOrbit++)
		{// �����蔻��f�[�^�̐������J��Ԃ�
			if (m_pOrbit[nCntOrbit] != NULL)
			{
				delete m_pOrbit[nCntOrbit];
				m_pOrbit[nCntOrbit] = NULL;
			}
		}
		delete[] m_pOrbit;
		m_pOrbit = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CMotion::Update(CModel **pModel, int nNumModel, int *pKey, int *pCounter, int *pBlendCounter, int *pAllCounter)
{
	D3DXVECTOR3 ModelRot;     // ���f���̌��݂̃I�t�Z�b�g�������
	D3DXVECTOR3 ModelAddPos;  // ���f���̃I�t�Z�b�g�ɉ�������W���

	// ���[�V�����J�E���^�[��i�߂�
	if (m_State == STATE_NORMAL)
	{// ���[�V�������Đ������Ԃ�������
		*pCounter = *pCounter + 1;  // ���[�V�����J�E���^�[��i�߂�
		*pAllCounter += 1;
		if (*pCounter >= m_pPlaybackKey[*pKey])
		{// ���[�V�����J�E���^�[���Đ��t���[������������
			*pCounter = 0;        // ���[�V�����J�E���^�[��߂�
			*pKey = *pKey + 1;    // �L�[�t���[����i�߂�
			if (*pKey >= m_nNumKey)
			{// ���݂̃L�[�t���[�����L�[�t���[���̑�����������
				if (m_bLoop == true)
				{// ���[�v����
					*pKey = 0;  // ���݂̃L�[�t���[������߂�
				}
				else
				{// ���[�v���Ȃ�
					m_State = STATE_STOP;  // ��~��Ԃ�
				}
			}

			// �L�[�t���[���̐؂�ւ�����
			if (m_State == STATE_NORMAL)
			{// �ʏ��ԂȂ��
				SwitchKey(pModel, nNumModel, *pKey);
			}
		}

		if (m_State == STATE_NORMAL)
		{// �ʏ��ԂȂ��
			for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
			{// ���f���̐������J��Ԃ�
				if (pModel[nCntModel] != NULL)
				{// ���f���̃|�C���^����ł͂Ȃ�
					ModelRot = pModel[nCntModel]->GetRot();        // ���f���̌��݂̌������擾
					ModelAddPos = pModel[nCntModel]->GetAddPos();  // ���f���̃I�t�Z�b�g�ɉ�����ʒu���擾

					// ���f���̃I�t�Z�b�g�ɉ�����l�𓮂���
					ModelAddPos.x += m_pKeyframe[*pKey][nCntModel]->GetDestPos().x / m_pPlaybackKey[*pKey];
					ModelAddPos.y += m_pKeyframe[*pKey][nCntModel]->GetDestPos().y / m_pPlaybackKey[*pKey];
					ModelAddPos.z += m_pKeyframe[*pKey][nCntModel]->GetDestPos().z / m_pPlaybackKey[*pKey];

					// X���̍�����������
					ModelRot.x += m_pKeyframe[*pKey][nCntModel]->GetDiffRot().x / m_pPlaybackKey[*pKey];
					if (ModelRot.x > D3DX_PI)
					{// ���݂̌�����D3DX_PI�𒴂���
						ModelRot.x -= D3DX_PI * 2.0f;
					}
					if (ModelRot.x < -D3DX_PI)
					{// ���݂̌�����-D3DX_PI�𒴂���
						ModelRot.x += D3DX_PI * 2.0f;
					}

					// Y���̍�����������
					ModelRot.y += m_pKeyframe[*pKey][nCntModel]->GetDiffRot().y / m_pPlaybackKey[*pKey];
					if (ModelRot.y > D3DX_PI)
					{// ���݂̌�����D3DX_PI�𒴂���
						ModelRot.y -= D3DX_PI * 2.0f;
					}
					if (ModelRot.y < -D3DX_PI)
					{// ���݂̌�����-D3DX_PI�𒴂���
						ModelRot.y += D3DX_PI * 2.0f;
					}

					// Z���̍�����������
					ModelRot.z += m_pKeyframe[*pKey][nCntModel]->GetDiffRot().z / m_pPlaybackKey[*pKey];
					if (ModelRot.z > D3DX_PI)
					{// ���݂̌�����D3DX_PI�𒴂���
						ModelRot.z -= D3DX_PI * 2.0f;
					}
					if (ModelRot.z < -D3DX_PI)
					{// ���݂̌�����-D3DX_PI�𒴂���
						ModelRot.z += D3DX_PI * 2.0f;
					}

					// ���f���ɃI�t�Z�b�g����ݒ�
					pModel[nCntModel]->SetRot(ModelRot);        // ����
					pModel[nCntModel]->SetAddPos(ModelAddPos);  // �I�t�Z�b�g�ɉ�����ʒu
				}
			}
		}
	}
	else if(m_State == STATE_SWITCH)
	{// �؂�ւ���ԂȂ��
		*pBlendCounter = *pBlendCounter + 1;  // ���[�V�����J�E���^�[��i�߂�
		*pAllCounter += 1;
		for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
		{// ���f���̐������J��Ԃ�
			if (pModel[nCntModel] != NULL)
			{// ���f���̃|�C���^����ł͂Ȃ�
				ModelRot = pModel[nCntModel]->GetRot();        // ���f���̌��݂̌������擾
				ModelAddPos = pModel[nCntModel]->GetAddPos();  // ���f���̌��݂̈ʒu���擾

				// ���f���̃I�t�Z�b�g�ʒu�𓮂���
				ModelAddPos.x += m_pKeyframe[*pKey][nCntModel]->GetDestPos().x / (m_pPlaybackKey[*pKey] * m_fBlendMag);
				ModelAddPos.y += m_pKeyframe[*pKey][nCntModel]->GetDestPos().y / (m_pPlaybackKey[*pKey] * m_fBlendMag);
				ModelAddPos.z += m_pKeyframe[*pKey][nCntModel]->GetDestPos().z / (m_pPlaybackKey[*pKey] * m_fBlendMag);

				// X���̍�����������
				ModelRot.x += m_pKeyframe[*pKey][nCntModel]->GetDiffRot().x / (m_pPlaybackKey[*pKey] * m_fBlendMag);
				if (ModelRot.x > D3DX_PI)
				{// ���݂̌�����D3DX_PI�𒴂���
					ModelRot.x -= D3DX_PI * 2.0f;
				}
				if (ModelRot.x < -D3DX_PI)
				{// ���݂̌�����-D3DX_PI�𒴂���
					ModelRot.x += D3DX_PI * 2.0f;
				}

				// Y���̍�����������
				ModelRot.y += m_pKeyframe[*pKey][nCntModel]->GetDiffRot().y / (m_pPlaybackKey[*pKey] * m_fBlendMag);
				if (ModelRot.y > D3DX_PI)
				{// ���݂̌�����D3DX_PI�𒴂���
					ModelRot.y -= D3DX_PI * 2.0f;
				}
				if (ModelRot.y < -D3DX_PI)
				{// ���݂̌�����-D3DX_PI�𒴂���
					ModelRot.y += D3DX_PI * 2.0f;
				}

				// Z���̍�����������
				ModelRot.z += m_pKeyframe[*pKey][nCntModel]->GetDiffRot().z / (m_pPlaybackKey[*pKey] * m_fBlendMag);
				if (ModelRot.z > D3DX_PI)
				{// ���݂̌�����D3DX_PI�𒴂���
					ModelRot.z -= D3DX_PI * 2.0f;
				}
				if (ModelRot.z < -D3DX_PI)
				{// ���݂̌�����-D3DX_PI�𒴂���
					ModelRot.z += D3DX_PI * 2.0f;
				}

				// ���f���Ɍ�����ݒ�
				pModel[nCntModel]->SetRot(ModelRot);        // ����
				pModel[nCntModel]->SetAddPos(ModelAddPos);  // �I�t�Z�b�g�ɉ�����ʒu
			}
		}

		if (*pBlendCounter >= (m_pPlaybackKey[*pKey] * m_fBlendMag))
		{// �u�����h�J�E���^�[���Đ��t���[������������
			*pBlendCounter = 0;     // �J�E���^�[��߂�
			*pKey = *pKey + 1;      // �L�[�t���[����i�߂�
			if (*pKey >= m_nNumKey)
			{// ���݂̃L�[�t���[�����L�[�t���[���̑�����������
				if (m_bLoop == true)
				{// ���[�v����
					*pKey = 0;  // ���݂̃L�[�t���[������߂�
				}
				else
				{// ���[�v���Ȃ�
					m_State = STATE_STOP;  // ��~��Ԃ�
				}
			}
			m_State = STATE_NORMAL; // �ʏ��Ԃɖ߂�

			// �L�[�t���[���̐؂�ւ�
			SwitchKey(pModel, nNumModel, *pKey);
		}
	}
}

//=============================================================================
//    ���[�V�����̍U�����[�V�����f�[�^�N���X�ւ̃|�C���^�擾����
//=============================================================================
CMotionAttack *CMotion::GetAttack(void)
{
	return m_pMotionAttack;
}

//=============================================================================
//    ���[�V�����̓����蔻��f�[�^�N���X�ւ̃|�C���^�擾����
//=============================================================================
CMotionCollision **CMotion::GetCollision(void)
{
	return m_pCollision;
}

//=============================================================================
//    �����蔻��f�[�^�̐����擾���鏈��
//=============================================================================
int CMotion::GetNumColData(void)
{
	return m_nNumColData;
}

//=============================================================================
//    ���[�V�����̋O�Ճf�[�^�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CMotionOrbit **CMotion::GetOrbit(void)
{
	return m_pOrbit;
}

//=============================================================================
//    �O�Ճf�[�^�̐���ݒ肷�鏈��
//=============================================================================
int CMotion::GetNumOrbitData(void)
{
	return m_nNumOrbitData;
}

//=============================================================================
//    ��Ԃ��擾
//=============================================================================
CMotion::STATE CMotion::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �L�[�t���[���̑������擾
//=============================================================================
int CMotion::GetNumKey(void)
{
	return m_nNumKey;
}

//=============================================================================
//    �L�[�t���[���̏����擾����
//=============================================================================
CKeyframe ***CMotion::GetKeyframe(void)
{
	return m_pKeyframe;
}

//=============================================================================
//    ���[�v���邩���Ȃ������擾����
//=============================================================================
bool CMotion::GetLoop(void)
{
	return m_bLoop;
}

//=============================================================================
//    �L�[�t���[���̍Đ��t���[�������擾����
//=============================================================================
int CMotion::GetPlayBackKey(int nIdx)
{
	return m_pPlaybackKey[nIdx];
}

//=============================================================================
//    �L�[�t���[���̍Đ��t���[�������擾����(�|�C���^)
//=============================================================================
int *CMotion::GetPlayBackKey(void)
{
	return m_pPlaybackKey;
}

//=============================================================================
//    ���[�V�����̑��Đ��t���[�������擾����
//=============================================================================
int CMotion::GetAllPlayBackKey(void)
{
	return m_nAllPlaybackKey;
}

//=============================================================================
//    ���[�V�����̃u�����h�{�����擾����
//=============================================================================
float CMotion::GetBlendMag(void)
{
	return m_fBlendMag;
}

//=============================================================================
//    �U�����[�V�����f�[�^�N���X�ւ̃|�C���^�ݒ菈��
//=============================================================================
void CMotion::SetAttack(CMotionAttack *pAttack)
{
	m_pMotionAttack = pAttack;
}

//=============================================================================
//    �����蔻��f�[�^�N���X�ւ̃|�C���^�ݒ菈��
//=============================================================================
void CMotion::SetCollision(CMotionCollision **pCollision)
{
	m_pCollision = pCollision;
}

//=============================================================================
//    �����蔻��f�[�^�̐��ݒ菈��
//=============================================================================
void CMotion::SetNumColData(const int nNumColData)
{
	m_nNumColData = nNumColData;
}

//=============================================================================
//    ���[�V�����̋O�Ճf�[�^�N���X�ւ̃|�C���^�ݒ菈��
//=============================================================================
void CMotion::SetOrbit(CMotionOrbit **pOrbit)
{
	m_pOrbit = pOrbit;
}

//=============================================================================
//    �O�Ճf�[�^�̐��ݒ菈��
//=============================================================================
void CMotion::SetNumOrbitData(const int nNumOrbitData)
{
	m_nNumOrbitData = nNumOrbitData;
}

//=============================================================================
//    �L�[�t���[������ݒ肷��
//=============================================================================
void CMotion::SetKeyFrame(const int nIdxModel, const int nIdxKey, CKeyframe *pKeyFrame)
{
	m_pKeyframe[nIdxKey][nIdxModel] = pKeyFrame;
}

//=============================================================================
//    ��Ԃ�ݒ�
//=============================================================================
void CMotion::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
//    �L�[�t���[������ݒ�
//=============================================================================
void CMotion::SetNumKey(const int nNumKey)
{
	m_nNumKey = nNumKey;
}

//=============================================================================
//    ���[�v���邩���Ȃ�����ݒ�
//=============================================================================
void CMotion::SetLoop(const bool bLoop)
{
	m_bLoop = bLoop;
}

//=============================================================================
//    �Đ��t���[������ݒ�
//=============================================================================
void CMotion::SetPlayBackKey(const int nPlayBackKey, int nIdx)
{
	m_pPlaybackKey[nIdx] = nPlayBackKey;
}

//=============================================================================
//    �Đ��t���[������ݒ�(�|�C���^)
//=============================================================================
void CMotion::SetPlayBackKey(int *pPlayBackKey)
{
	m_pPlaybackKey = pPlayBackKey;
}

//=============================================================================
//    ���Đ��t���[������ݒ�
//=============================================================================
void CMotion::SetAllPlayBackKey(int nAllPlayBackKey)
{
	m_nAllPlaybackKey = nAllPlayBackKey;
}

//=============================================================================
//    �u�����h�{����ݒ�
//=============================================================================
void CMotion::SetBlendMag(const float fBlendMag)
{
	m_fBlendMag = fBlendMag;
}

//=============================================================================
//    �L�[�t���[���؂�ւ�����
//=============================================================================
void CMotion::SwitchKey(CModel **pModel, int nNumModel, int nKey)
{
	for (int nCntModel = 0; nCntModel < nNumModel; nCntModel++)
	{// ���f���̐������J��Ԃ�
		D3DXVECTOR3 DiffRot;  // �����̌���
		D3DXVECTOR3 DestPos;  // �ړI�̈ʒu

		// ���[�V�����̃I�t�Z�b�g�ړI�̈ʒu���v�Z
		DestPos.x = m_pKeyframe[nKey][nCntModel]->GetAddPos().x - pModel[nCntModel]->GetAddPos().x;
		DestPos.y = m_pKeyframe[nKey][nCntModel]->GetAddPos().y - pModel[nCntModel]->GetAddPos().y;
		DestPos.z = m_pKeyframe[nKey][nCntModel]->GetAddPos().z - pModel[nCntModel]->GetAddPos().z;

		// X���̌����̍��������߂�
		DiffRot.x = m_pKeyframe[nKey][nCntModel]->GetDestRot().x - pModel[nCntModel]->GetRot().x;
		if (DiffRot.x > D3DX_PI)
		{// ���݂̌�����D3DX_PI�𒴂���
			DiffRot.x -= D3DX_PI * 2.0f;
		}
		if (DiffRot.x < -D3DX_PI)
		{// ���݂̌�����-D3DX_PI�𒴂���
			DiffRot.x += D3DX_PI * 2.0f;
		}

		// Y���̌����̍��������߂�
		DiffRot.y = m_pKeyframe[nKey][nCntModel]->GetDestRot().y - pModel[nCntModel]->GetRot().y;
		if (DiffRot.y > D3DX_PI)
		{// ���݂̌�����D3DX_PI�𒴂���
			DiffRot.y -= D3DX_PI * 2.0f;
		}
		if (DiffRot.y < -D3DX_PI)
		{// ���݂̌�����-D3DX_PI�𒴂���
			DiffRot.y += D3DX_PI * 2.0f;
		}

		// Z���̌����̍��������߂�
		DiffRot.z = m_pKeyframe[nKey][nCntModel]->GetDestRot().z - pModel[nCntModel]->GetRot().z;
		if (DiffRot.z > D3DX_PI)
		{// ���݂̌�����D3DX_PI�𒴂���
			DiffRot.z -= D3DX_PI * 2.0f;
		}
		if (DiffRot.z < -D3DX_PI)
		{// ���݂̌�����-D3DX_PI�𒴂���
			DiffRot.z += D3DX_PI * 2.0f;
		}

		// �����̌�����ݒ肷��
		m_pKeyframe[nKey][nCntModel]->SetDiffRot(DiffRot);
		// �ړI�̈ʒu��ݒ肷��
		m_pKeyframe[nKey][nCntModel]->SetDestPos(DestPos);
	}
}

//*****************************************************************************
//     CKeyframe�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CKeyframe::CKeyframe()
{
	// �e��l�̃N���A
	m_AddPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���݂̃I�t�Z�b�g�ʒu�ɉ�����l
	m_DestPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �ړI�̃I�t�Z�b�g�ʒu
	m_DestRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �ړI�̃I�t�Z�b�g����
	m_DiffRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �����̌���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CKeyframe::~CKeyframe()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CKeyframe::Init(void)
{
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CKeyframe::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CKeyframe::Update(void)
{

}

//=============================================================================
//    ���݂̃I�t�Z�b�g�ʒu�ɉ�����l��ݒ肷��
//=============================================================================
void CKeyframe::SetAddPos(D3DXVECTOR3 pos)
{
	m_AddPos = pos;
}

//=============================================================================
//    �ړI�̃I�t�Z�b�g�ʒu��ݒ肷��
//=============================================================================
void CKeyframe::SetDestPos(D3DXVECTOR3 pos)
{
	m_DestPos = pos;
}

//=============================================================================
//    �ړI�̃I�t�Z�b�g������ݒ肷��
//=============================================================================
void CKeyframe::SetDestRot(D3DXVECTOR3 rot)
{
	m_DestRot = rot;
}

//=============================================================================
//    �����̌�����ݒ肷��
//=============================================================================
void CKeyframe::SetDiffRot(D3DXVECTOR3 rotDiff)
{
	m_DiffRot = rotDiff;
}

//=============================================================================
//    ���݂̃I�t�Z�b�g�ʒu�ɉ�����l���擾����
//=============================================================================
D3DXVECTOR3 CKeyframe::GetAddPos(void)
{
	return m_AddPos;
}

//=============================================================================
//    �ړI�̃I�t�Z�b�g�ʒu���擾����
//=============================================================================
D3DXVECTOR3 CKeyframe::GetDestPos(void)
{
	return m_DestPos;
}

//=============================================================================
//    �ړI�̃I�t�Z�b�g�������擾����
//=============================================================================
D3DXVECTOR3 CKeyframe::GetDestRot(void)
{
	return m_DestRot;
}

//=============================================================================
//    �����̌������擾����
//=============================================================================
D3DXVECTOR3 CKeyframe::GetDiffRot(void)
{
	return m_DiffRot;
}