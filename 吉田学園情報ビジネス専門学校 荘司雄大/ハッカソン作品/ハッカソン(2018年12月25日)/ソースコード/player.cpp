//*****************************************************************************
//
//     �v���C���[�̏���[player.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "modelManager.h"
#include "motion.h"
#include "camera.h"
#include "input.h"
#include "effectManager.h"
#include "object.h"
#include "enemy.h"
#include "orbitEffect.h"
#include "meshField.h"
#include "textureManager.h"
#include "functionlib.h"
#include "debuglog.h"
#include "Billpresent.h"

#include "game.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
// �l�ǂݍ��݂�����ۂ̖ڈ�ƂȂ镶����
// ����
#define POS               "POS = "                // ���W��ǂݎ�鍇�}
#define ROT               "ROT = "                // ������ǂݎ�鍇�}
#define MOVE              "MOVE = "               // �ړ��ʂ�ǂݎ�鍇�}

// ���̑�
#define NUM_TEXTURE       "NUM_TEXTURE = "        // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME  "TEXTURE_FILENAME = "   // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define NUM_MODEL         "NUM_MODEL = "          // �ǂݍ��ރ��f���̐�
#define MODEL_FILENAME    "MODEL_FILENAME = "     // �ǂݍ��ރ��f���̃t�@�C����

// �L�����N�^�[���
#define CHARACTERSET      "CHARACTERSET"          // �L�����N�^�[���ǂݍ��݊J�n�̍��}
#define END_CHARACTERSET  "END_CHARACTERSET"      // �L�����N�^�[���ǂݍ��ݏI���̍��}
#define JUMP              "JUMP = "               // �W�����v��
#define GRAVITY           "GRAVITY = "            // �d�͗�
#define INERTIA           "INERTIA = "            // ������
#define RIVISION_ANGLE    "RIVISION_ANGLE = "     // ������␳����{��
#define COL_HEIGHT        "COL_HEIGHT = "         // �����蔻����Ƃ鍂��
#define COL_RANGE         "COL_RANGE = "          // �����蔻����Ƃ�͈�
#define NUM_PARTS         "NUM_PARTS = "          // �L�����N�^�[�̃p�[�c��

// �p�[�c���
#define PARTSSET          "PARTSSET"              // �p�[�c���ǂݍ��݊J�n�̍��}
#define END_PARTSSET      "END_PARTSSET"          // �p�[�c���ǂݍ��ݏI���̍��}
#define INDEX             "INDEX = "              // �ԍ���ǂݎ�鍇�}
#define PARENT            "PARENT = "             // �e�p�[�c�̔ԍ���ǂݎ�鍇�}

// ���[�V�������
#define MOTIONSET         "MOTIONSET"             // ���[�V�������ǂݍ��݊J�n�̍��}
#define END_MOTIONSET     "END_MOTIONSET"         // ���[�V�������ǂݍ��ݏI���̍��}
#define COLLISIONSET      "COLLISIONSET"          // ���[�V�����̓����蔻����ǂݍ��݊J�n�̍��}
#define END_COLLISIONSET  "END_COLLISIONSET"      // ���[�V�����̓����蔻����ǂݍ��ݏI���̍��}
#define ORBITSET          "ORBITSET"              // ���[�V�����̋O�Տ��ǂݍ��݊J�n�̍��}
#define END_ORBITSET      "END_ORBITSET"          // ���[�V�����̋O�Տ��ǂݍ��ݏI���̍��}
#define RANGE             "RANGE = "              // �͈͂�ǂݎ�鍇�}
#define XBLOCK            "XBLOCK = "             // �O�Ղ̉��̕�������ǂݎ�鍇�}
#define ZBLOCK            "ZBLOCK = "             // �O�Ղ̉��s�̕�������ǂݎ�鍇�}
#define OFFSET            "OFFSET = "             // �I�t�Z�b�g��ǂݎ�鍇�}
#define LOOP              "LOOP = "               // ���[�V���������[�v�Đ����邩�ǂ�����ǂݎ�鍇�}
#define BLEND             "BLEND = "              // ���[�V�����u�����h�̔{����ǂݎ�鍇�}
#define NUM_KEY           "NUM_KEY = "            // ���[�V�����̃L�[�t���[���̐���ǂݎ�鍇�}

// �U�����[�V�������
#define ATTACKSET         "ATTACKSET"             // �U�����[�V�������ǂݍ��݊J�n�̍��}
#define END_ATTACKSET     "END_ATTACKSET"         // �U�����[�V�������ǂݍ��ݏI���̍��}
#define NEXT_MOTION       "NEXT_MOTION = "        // ���̃��[�V�����̔ԍ�

// �����蔻����
#define NUM_COL           "NUM_COL = "            // ���[�V�����̓����蔻����̐���ǂݎ�鍇�}
#define COLLISION         "COLLISION"             // �����蔻����ǂݍ��݊J�n�̍��}
#define END_COLLISION     "END_COLLISION"         // �����蔻����ǂݍ��ݏI���̍��}

// �O�Տ��
#define NUM_ORBIT         "NUM_ORBIT = "          // ���[�V�����̋O�Տ��̐���ǂݎ�鍇�}
#define ORBIT		      "ORBIT"                 // �O�Տ��ǂݍ��݊J�n�̍��}
#define END_ORBIT	      "END_ORBIT"             // �O�Տ��ǂݍ��ݏI���̍��}
#define TEX_INDEX         "TEX_INDEX = "          // �O�Ղɒ���t����e�N�X�`���̔ԍ���ǂݎ�鍇�}
#define OFFSET1           "OFFSET1 = "            // �P�ڂ̃I�t�Z�b�g��ǂݎ�鍇�}
#define OFFSET2           "OFFSET2 = "            // �Q�ڂ̃I�t�Z�b�g��ǂݎ�鍇�}
#define OFFSETAMP1        "OFFSETAMP1 = "         // �O�Ղ̃I�t�Z�b�g���������l��ǂݎ�鍇�}(�P��)
#define OFFSETAMP2        "OFFSETAMP2 = "         // �O�Ղ̃I�t�Z�b�g���������l��ǂݎ�鍇�}(�Q��)
#define COLUP             "COLUP = "              // �O�Ղ̒��_�㑤�̐F
#define COLDOWN           "COLDOWN = "            // �O�Ղ̒��_�����̐F
#define ALPHA_DECAY_UP    "ALPHA_DECAY_UP = "     // �O�Ղ̒��_�㑤�̓����x�����l
#define ALPHA_DECAY_DOWN  "ALPHA_DECAY_DOWN = "   // �O�Ղ̒��_�����̓����x�����l
#define START             "START = "              // �O�Ղ��o���n�߂�^�C�~���O��ǂݎ�鍇�}
#define FINISH            "FINISH = "             // �O�Ղ����܂��^�C�~���O��ǂݎ�鍇�}

// �L�[�t���[�����
#define KEYSET            "KEYSET"                // �L�[�t���[�����ǂݍ��݊J�n�̍��}
#define END_KEYSET        "END_KEYSET"            // �L�[�t���[�����ǂݍ��ݏI���̍��}
#define FRAME             "FRAME = "              // �L�[�t���[���̍Đ��t���[������ǂݎ�鍇�}

// �L�[���
#define KEY               "KEY"                   // �L�[���ǂݍ��݊J�n�̍��}
#define END_KEY           "END_KEY"               // �L�[���ǂݍ��ݏI���̍��}

//*****************************************************************************
//     CPlayerManager�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CPlayerManager::CPlayerManager(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pTextureManager = NULL;     // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	strcpy(m_aFileName, "\0");    // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
	m_pModelManager = NULL;       // ���f���Ǌ��N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CPlayerManager::~CPlayerManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CPlayerManager *CPlayerManager::Create(char *pFileName, int nPriority)
{
	CPlayerManager *pPlayerManager = NULL;  // �v���C���[�Ǌ��N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();    // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pPlayerManager == NULL)
		{// ����������ɂȂ��Ă���
			pPlayerManager = new CPlayerManager(nPriority);
			if (pPlayerManager != NULL)
			{// �C���X�^���X�𐶐��ł���
				pPlayerManager->SetFileName(pFileName);
				if (FAILED(pPlayerManager->Init()))
				{// �������Ɏ��s����
					return NULL;
				}
			}
			else
			{// �C���X�^���X�𐶐��ł��Ȃ�����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ����ȏ�V�[�����쐬�ł��Ȃ�
		return NULL;
	}

	return pPlayerManager;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CPlayerManager::Init(void)
{
	// �v���C���[�̃p�[�c���𔲂��o��
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                         // 1�s���ǂݎ��p
			char *pStrCur = NULL;                       // ���݂̐擪�|�C���^
			char *pStr = NULL;                          // �擪�|�C���^�ۑ��p
			int nNumTexture = 0;                        // �ǂݍ��ރe�N�X�`���̐�
			int nNumModel = 0;                          // �ǂݍ��ރ��f����
			int nNumParts = 0;                          // �ǂݍ��ރp�[�c��
			int nCntTex = 0;                            // �e�N�X�`����ǂݍ��񂾉񐔂𐔂���
			int nCntModel = 0;                          // ���f����ǂݍ��񂾉񐔂𐔂���
			int nCntParts = 0;                          // �p�[�c����ǂݍ��񂾉񐔂𐔂���
			int nCntMotion = 0;                         // ���[�V������ǂݍ��񂾉񐔂𐔂���
			int *pNumParent = NULL;                     // �e���f���̔ԍ�
			CMotionManager *pMotionManager = NULL;      // ���[�V�����Ǘ��N���X�ւ̃|�C���^
			CMotion *pMotion[CPlayer::STATE_MAX] = {};  // ���[�V�����N���X�ւ̃|�C���^
			CModel **apModel = NULL;                    // ���f���N���X�ւ̃|�C���^
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // �e�N�X�`�����ւ̃|�C���^
			LPD3DXMESH pMesh = NULL;                    // ���b�V�����ւ̃|�C���^
			LPD3DXBUFFER pBuffMat = NULL;               // �}�e���A�����ւ̃|�C���^
			DWORD nNumMat = 0;                          // �}�e���A�����̐�
			float fAccel = 0.0f;                        // �v���C���[�̉����x���
			float fInertia = 0.0f;                      // �v���C���[�̊������
			float fGravity = 0.0f;                      // �v���C���[�̏d�͏��
			float fJumpPower = 0.0f;                    // �v���C���[�̃W�����v�ʏ��
			float fRivisionRot = 0.0f;                  // �v���C���[�̌�����␳����{�����
			float fColHeight = 0.0f;                    // �v���C���[�̓����蔻�����鍂�����
			float fColRange = 0.0f;                     // �v���C���[�̓����蔻������͈͏��

			if (pLine == NULL && pStr == NULL)
			{// ���������m�ۂł����Ԃł���
				pLine = new char[256];
				pStr = new char[256];
				strcpy(pLine, "\0");
				strcpy(pStr, "\0");
				if (pLine != NULL && pStr != NULL)
				{// ���������m�ۂ���Ă���
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// �ǂݍ��ރe�N�X�`����������
								strcpy(pStr, pStrCur);
								nNumTexture = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTexture >= 1)
								{// �ǂݍ��ރe�N�X�`������1�ȏ゠��
									m_pTextureManager = CTextureManager::Create(nNumTexture);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// �e�N�X�`���̃t�@�C���p�X����ǂݍ���
								if (m_pTextureManager != NULL && nCntTex < nNumTexture)
								{// �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���m�ۂ���Ă���
								    // �e�N�X�`���̃t�@�C���p�X����ǂݎ��
									pStr = CFunctionLib::ReadString(pStrCur, pStr, TEXTURE_FILENAME);

									// �e�N�X�`���̓ǂݍ���
									D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);
									if (pTexture != NULL)
									{// �e�N�X�`����ǂݍ��߂�
										m_pTextureManager->BindTexture(pTexture, nCntTex);
										m_pTextureManager->SetFileName(pStr, nCntTex);
										pTexture = NULL;
										nCntTex++;
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
							{// �ǂݍ��ރ��f����������
								strcpy(pStr, pStrCur);
								nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
								if (nNumModel >= 1)
								{// ���f������1�ȏ゠��
									pNumParent = new int[nNumModel];
									m_pModelManager = CModelManager::Create(nNumModel);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
							{// ���f���̃t�@�C���p�X����ǂݍ���
								if (m_pModelManager != NULL && nCntModel < nNumModel)
								{// ���f���Ǌ��N���X�ւ̃|�C���^���m�ۂ���Ă���
									// ���f���̃t�@�C���p�X����ǂݎ��
									pStr = CFunctionLib::ReadString(pStrCur, pStr, MODEL_FILENAME);

									// x�t�@�C���̓ǂݍ���
									D3DXLoadMeshFromX(pStr,
										D3DXMESH_SYSTEMMEM,
										CManager::GetRenderer()->GetDevice(),
										NULL,
										&pBuffMat,
										NULL,
										&nNumMat,
										&pMesh);

									// ���f���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
									m_pModelManager->SetMesh(pMesh, nCntModel);
									m_pModelManager->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);

									// �g�p�����ϐ������������Ă���
									pMesh = NULL;
									pBuffMat = NULL;
									nNumMat = 0;

									nCntModel++;  // ���f����ǂݍ��񂾉񐔂𑝂₷
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, CHARACTERSET) == 0)
							{// �L�����N�^�[���ǂݍ��݊J�n�̍��}��������
								apModel = ReadCharacterSet(pLine, pStrCur, pFile, &nNumParts, pNumParent, &fAccel, &fInertia, &fJumpPower, &fRivisionRot, &fGravity, &fColHeight, &fColRange);
								nCntParts++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOTIONSET) == 0)
							{// ���[�V�������ǂݍ��݊J�n�̍��}��������
								pMotion[nCntMotion] = ReadMotionSet(pLine, pStrCur, pFile, &nNumParts, pMotion[nCntMotion], nCntMotion);
								nCntMotion++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}
				}
			}

			// �������̊J��
			if (pStr != NULL)
			{// ���������m�ۂ���Ă���
				delete[] pStr;
				pStr = NULL;
			}
			if (pLine != NULL)
			{// ���������m�ۂ���Ă���
				delete[] pLine;
				pLine = NULL;
			}

			// �K�v�ȃN���X���쐬���Ă���
			// ���[�V�����Ǌ��N���X
			if (pMotionManager == NULL)
			{// �|�C���^���m�ۂ���Ă���
				pMotionManager = CMotionManager::Create(CPlayer::STATE_MAX, nNumParts);
				if (pMotionManager != NULL)
				{// ���[�V�����Ǘ��N���X���쐬�ł���
					for (int nCntMotion = 0; nCntMotion < CPlayer::STATE_MAX; nCntMotion++)
					{// ��Ԃ̐������J�肩����
						pMotionManager->SetMotion(pMotion[nCntMotion], nCntMotion);
					}
				}
			}

			// �v���C���[�N���X���쐬����
			CreatePlayer(apModel, pMotionManager, nNumParts, pNumParent, fAccel, fInertia, fJumpPower, fRivisionRot, fGravity, fColHeight, fColRange);
			fclose(pFile);


			// �K�v�ȕϐ����L�����Ă���
			m_pMotionManager = pMotionManager;
			m_apModel = apModel;
			m_nNumParts = nNumParts;
			m_pNumParent = pNumParent;
			m_fAccel = fAccel;
			m_fInertia = fInertia;
			m_fJumpPower = fJumpPower;
			m_fRivisionRot = fRivisionRot;
			m_fGravity = fGravity;
			m_fColHeight = fColHeight;
			m_fColRange = fColRange;
		}
		else
		{// �t�@�C���|�C���^���m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// �t�@�C���|�C���^���m�ۂł����ԂłȂ�
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CPlayerManager::Uninit(void)
{
	// ���f���Ǌ��N���X�̔j��
	if (m_pModelManager != NULL)
	{// ���������m�ۂ���Ă���
		// �I������
		m_pModelManager->Uninit();

		// �������̊J��
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// �ǂݎ��p�̃|�C���^���J�����Ă���
	if (m_apModel != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// �g�p�ł��郂�f�������J��Ԃ�
			if (m_apModel[nCntModel] != NULL)
			{// �|�C���^���m�ۂ���Ă���
				m_apModel[nCntModel]->Uninit();

				// �������̊J��
				delete m_apModel[nCntModel];
				m_apModel[nCntModel] = NULL;
			}
		}
		// �������̊J��
		delete[] m_apModel;
		m_apModel = NULL;
	}

	// ���[�V�����̔j��
	if (m_pMotionManager != NULL)
	{// �|�C���^���m�ۂ���Ă���
		m_pMotionManager->Uninit();

		// �������̊J��
		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}

	// �e���f���̔ԍ�
	if (m_pNumParent != NULL)
	{// ���������m�ۂ���Ă���
		delete[] m_pNumParent;
		m_pNumParent = NULL;
	}

	// �e�N�X�`���Ǌ��N���X�̔j��
	if (m_pTextureManager != NULL)
	{// �|�C���^���m�ۂ���Ă���
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// ���g�̃|�C���^��j������
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CPlayerManager::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CPlayerManager::Draw(void)
{

}

//=============================================================================
//    �v���C���[�N���X���쐻���鏈��
//=============================================================================
void CPlayerManager::CreatePlayer(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange)
{
	CModel **apModelCpy = NULL;                     // ���f���N���X�R�s�[�p
	CMotionAttack *pMotionAttack = NULL;            // �U�����[�V�����f�[�^�N���X�R�s�[�p
	CMotionCollision **pMotionCollision = NULL;     // ���[�V�����̓����蔻��f�[�^�N���X�R�s�[�p
	CMotionOrbit **pMotionOrbit = NULL;             // ���[�V�����̋O�Ճf�[�^�N���X�R�s�[�p
	CMotion *pMotionCpy[CPlayer::STATE_MAX] = {};   // ���[�W�����N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;       // ���[�V�����Ǌ��N���X�R�s�[�p
	CPlayer *pPlayer = NULL;                        // �v���C���[�N���X�ւ̃|�C���^

	if (apModel != NULL && pMotionManager != NULL && m_pModelManager != NULL)
	{// ���f���ւ̃|�C���^�ƃ��[�V�����N���X�ւ̃|�C���^�ƃ��f���Ǌ��N���X�ւ̃|�C���^���쐬����Ă���
	    // ���f�������R�s�[����
		if (apModelCpy == NULL)
		{// ���������m�ۂ���Ă���
			apModelCpy = new CModel*[nNumParts];
			if (apModelCpy != NULL)
			{// ���������m�ۂł���
				for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
				{// �p�[�c�����J��Ԃ�
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// ���������m�ۂł����Ԃł���
						apModelCpy[nCntParts] = CModel::Create(apModel[nCntParts]->GetPos(), apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(nCntParts), m_pModelManager->GetBuffMat(nCntParts), m_pModelManager->GetNumMat(nCntParts), m_pModelManager->GetVtxMax(nCntParts), m_pModelManager->GetVtxMin(nCntParts), m_pModelManager->GetTexture(nCntParts));
						if (apModelCpy[nCntParts] != NULL)
						{// ���������m�ۂł���
							if (pNumParent[nCntParts] != -1)
							{// �e���f�������݂���
								apModelCpy[nCntParts]->SetParent(apModelCpy[pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// ���[�V���������R�s�[����
		if (pMotionManagerCpy == NULL)
		{// ���������m�ۂł����Ԃł���
			pMotionManagerCpy = CMotionManager::Create(CPlayer::STATE_MAX, nNumParts);
			if (pMotionManagerCpy != NULL)
			{// ���[�V�����Ǘ��N���X���쐬�ł���
				for (int nCntMotion = 0; nCntMotion < CPlayer::STATE_MAX; nCntMotion++)
				{// �v���C���[�̏�Ԃ̐������J��Ԃ�
					if (pMotionCpy[nCntMotion] == NULL)
					{// ���������m�ۂł����Ԃł���
						pMotionCpy[nCntMotion] = CMotion::Create(nNumParts, pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), pMotionManager->GetMotion()[nCntMotion]->GetLoop(), pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// ���������m�ۂł���
							for (int nCntKey = 0; nCntKey < pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// �L�[�t���[���̑������J��Ԃ�
								for (int nCntParts = 0; nCntParts < nNumParts; nCntParts++)
								{// �L�[�t���[���̑������J��Ԃ�
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// �U�����[�V�����f�[�^�N���X�̃R�s�[
							if (pMotionManager->GetMotion()[nCntMotion]->GetAttack() != NULL)
							{// �U�����[�V�����f�[�^�N���X����������Ă���
								pMotionAttack = new CMotionAttack;
								if (pMotionAttack != NULL)
								{// ���������m�ۂł���
									// �e��l�̐ݒ�
									pMotionAttack->SetNextMotionIdx(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetNextMotionIdx());
									pMotionAttack->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetStartTiming());
									pMotionAttack->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetFinishTiming());
									pMotionCpy[nCntMotion]->SetAttack(pMotionAttack);
								}
							}

							// �����蔻��f�[�^�N���X�̃R�s�[
							if (pMotionManager->GetMotion()[nCntMotion]->GetNumColData() >= 1)
							{// �����蔻��f�[�^���P�ȏ゠��
								pMotionCollision = new CMotionCollision*[pMotionManager->GetMotion()[nCntMotion]->GetNumColData()];
								if (pMotionCollision != NULL)
								{// ���������m�ۂł���
									for (int nCntCol = 0; nCntCol < pMotionManager->GetMotion()[nCntMotion]->GetNumColData(); nCntCol++)
									{// �����蔻��f�[�^�̐������J��Ԃ�
										pMotionCollision[nCntCol] = new CMotionCollision;
										if (pMotionCollision[nCntCol] != NULL)
										{// ���������m�ۂł���
											pMotionCollision[nCntCol]->SetModelIdx(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetModelIdx());
											pMotionCollision[nCntCol]->SetOffsetPos(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetOffsetPos());
											pMotionCollision[nCntCol]->SetRange(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetRange());
											pMotionCollision[nCntCol]->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetStartTiming());
											pMotionCollision[nCntCol]->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetFinishTiming());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetCollision(pMotionCollision);
								pMotionCpy[nCntMotion]->SetNumColData(pMotionManager->GetMotion()[nCntMotion]->GetNumColData());
							}

							// �O�Ճf�[�^�N���X�̃R�s�[
							if (pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData() >= 1)
							{// �����蔻��f�[�^���P�ȏ゠��
								pMotionOrbit = new CMotionOrbit*[pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData()];
								if (pMotionOrbit != NULL)
								{// ���������m�ۂł���
									for (int nCntOrbit = 0; nCntOrbit < pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData(); nCntOrbit++)
									{// �����蔻��f�[�^�̐������J��Ԃ�
										pMotionOrbit[nCntOrbit] = new CMotionOrbit;
										if (pMotionOrbit[nCntOrbit] != NULL)
										{// ���������m�ۂł���
											pMotionOrbit[nCntOrbit]->SetModelIdx(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetModelIdx());
											pMotionOrbit[nCntOrbit]->SetTexIdx(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetTexIdx());
											pMotionOrbit[nCntOrbit]->SetOffsetPos1(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos1());
											pMotionOrbit[nCntOrbit]->SetOffsetPos2(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos2());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp1(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp1());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp2(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp2());
											pMotionOrbit[nCntOrbit]->SetColUp(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColUp());
											pMotionOrbit[nCntOrbit]->SetColDown(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColDown());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayUp());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayDown());
											pMotionOrbit[nCntOrbit]->SetXBlock(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetXBlock());
											pMotionOrbit[nCntOrbit]->SetYBlock(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetYBlock());
											pMotionOrbit[nCntOrbit]->SetStartTiming(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetStartTiming());
											pMotionOrbit[nCntOrbit]->SetFinishTiming(pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetFinishTiming());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetOrbit(pMotionOrbit);
								pMotionCpy[nCntMotion]->SetNumOrbitData(pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());
							}

							// ���[�V�����Ǌ��N���X�Ƀ|�C���^��n��
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// �v���C���[�N���X���쐬����
		if (pPlayer == NULL)
		{// ���������m�ۂł����Ԃł���
			pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), apModelCpy, pMotionManagerCpy, m_pTextureManager, nNumParts, fAccel, fInertia, fJumpPower, fGravity, fRivisionRot, fColHeight, fColRange);
		}
	}
}

//=============================================================================
//    �v���C���[�����X�|�[�������鏈��
//=============================================================================
void CPlayerManager::SetRespawn(void)
{
	CModel **apModelCpy = NULL;                     // ���f���N���X�R�s�[�p
	CMotionAttack *pMotionAttack = NULL;            // �U�����[�V�����f�[�^�N���X�R�s�[�p
	CMotionCollision **pMotionCollision = NULL;     // ���[�V�����̓����蔻��f�[�^�N���X�R�s�[�p
	CMotionOrbit **pMotionOrbit = NULL;             // ���[�V�����̋O�Ճf�[�^�N���X�R�s�[�p
	CMotion *pMotionCpy[CPlayer::STATE_MAX] = {};   // ���[�W�����N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;       // ���[�V�����Ǌ��N���X�R�s�[�p
	CPlayer *pPlayer = NULL;                        // �v���C���[�N���X�ւ̃|�C���^

	if (m_apModel != NULL && m_pMotionManager != NULL && m_pModelManager != NULL)
	{// ���f���ւ̃|�C���^�ƃ��[�V�����N���X�ւ̃|�C���^�ƃ��f���Ǌ��N���X�ւ̃|�C���^���쐬����Ă���
	    // ���f�������R�s�[����
		if (apModelCpy == NULL)
		{// ���������m�ۂ���Ă���
			apModelCpy = new CModel*[m_nNumParts];
			if (apModelCpy != NULL)
			{// ���������m�ۂł���
				for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
				{// �p�[�c�����J��Ԃ�
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// ���������m�ۂł����Ԃł���
						apModelCpy[nCntParts] = CModel::Create(m_apModel[nCntParts]->GetPos(), m_apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(nCntParts), m_pModelManager->GetBuffMat(nCntParts), m_pModelManager->GetNumMat(nCntParts), m_pModelManager->GetVtxMax(nCntParts), m_pModelManager->GetVtxMin(nCntParts), m_pModelManager->GetTexture(nCntParts));
						if (apModelCpy[nCntParts] != NULL)
						{// ���������m�ۂł���
							if (m_pNumParent[nCntParts] != -1)
							{// �e���f�������݂���
								apModelCpy[nCntParts]->SetParent(apModelCpy[m_pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// ���[�V���������R�s�[����
		if (pMotionManagerCpy == NULL)
		{// ���������m�ۂł����Ԃł���
			pMotionManagerCpy = CMotionManager::Create(CPlayer::STATE_MAX, m_nNumParts);
			if (pMotionManagerCpy != NULL)
			{// ���[�V�����Ǘ��N���X���쐬�ł���
				for (int nCntMotion = 0; nCntMotion < CPlayer::STATE_MAX; nCntMotion++)
				{// �v���C���[�̏�Ԃ̐������J��Ԃ�
					if (pMotionCpy[nCntMotion] == NULL)
					{// ���������m�ۂł����Ԃł���
						pMotionCpy[nCntMotion] = CMotion::Create(m_nNumParts, m_pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), m_pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), m_pMotionManager->GetMotion()[nCntMotion]->GetLoop(), m_pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// ���������m�ۂł���
							for (int nCntKey = 0; nCntKey < m_pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// �L�[�t���[���̑������J��Ԃ�
								for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
								{// �L�[�t���[���̑������J��Ԃ�
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(m_pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(m_pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// �U�����[�V�����f�[�^�N���X�̃R�s�[
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetAttack() != NULL)
							{// �U�����[�V�����f�[�^�N���X����������Ă���
								pMotionAttack = new CMotionAttack;
								if (pMotionAttack != NULL)
								{// ���������m�ۂł���
								 // �e��l�̐ݒ�
									pMotionAttack->SetNextMotionIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetNextMotionIdx());
									pMotionAttack->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetStartTiming());
									pMotionAttack->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetFinishTiming());
									pMotionCpy[nCntMotion]->SetAttack(pMotionAttack);
								}
							}

							// �����蔻��f�[�^�N���X�̃R�s�[
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData() >= 1)
							{// �����蔻��f�[�^���P�ȏ゠��
								pMotionCollision = new CMotionCollision*[m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData()];
								if (pMotionCollision != NULL)
								{// ���������m�ۂł���
									for (int nCntCol = 0; nCntCol < m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData(); nCntCol++)
									{// �����蔻��f�[�^�̐������J��Ԃ�
										pMotionCollision[nCntCol] = new CMotionCollision;
										if (pMotionCollision[nCntCol] != NULL)
										{// ���������m�ۂł���
											pMotionCollision[nCntCol]->SetModelIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetModelIdx());
											pMotionCollision[nCntCol]->SetOffsetPos(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetOffsetPos());
											pMotionCollision[nCntCol]->SetRange(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetRange());
											pMotionCollision[nCntCol]->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetStartTiming());
											pMotionCollision[nCntCol]->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetFinishTiming());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetCollision(pMotionCollision);
								pMotionCpy[nCntMotion]->SetNumColData(m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData());
							}

							// �O�Ճf�[�^�N���X�̃R�s�[
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData() >= 1)
							{// �����蔻��f�[�^���P�ȏ゠��
								pMotionOrbit = new CMotionOrbit*[m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData()];
								if (pMotionOrbit != NULL)
								{// ���������m�ۂł���
									for (int nCntOrbit = 0; nCntOrbit < m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData(); nCntOrbit++)
									{// �����蔻��f�[�^�̐������J��Ԃ�
										pMotionOrbit[nCntOrbit] = new CMotionOrbit;
										if (pMotionOrbit[nCntOrbit] != NULL)
										{// ���������m�ۂł���
											pMotionOrbit[nCntOrbit]->SetModelIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetModelIdx());
											pMotionOrbit[nCntOrbit]->SetTexIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetTexIdx());
											pMotionOrbit[nCntOrbit]->SetOffsetPos1(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos1());
											pMotionOrbit[nCntOrbit]->SetOffsetPos2(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos2());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp1(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp1());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp2(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp2());
											pMotionOrbit[nCntOrbit]->SetColUp(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColUp());
											pMotionOrbit[nCntOrbit]->SetColDown(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColDown());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayUp());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayDown());
											pMotionOrbit[nCntOrbit]->SetXBlock(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetXBlock());
											pMotionOrbit[nCntOrbit]->SetYBlock(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetYBlock());
											pMotionOrbit[nCntOrbit]->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetStartTiming());
											pMotionOrbit[nCntOrbit]->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetFinishTiming());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetOrbit(pMotionOrbit);
								pMotionCpy[nCntMotion]->SetNumOrbitData(m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());
							}

							// ���[�V�����Ǌ��N���X�Ƀ|�C���^��n��
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// �v���C���[�N���X���쐬����
		if (pPlayer == NULL)
		{// ���������m�ۂł����Ԃł���
			pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 100.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), apModelCpy, pMotionManagerCpy, m_pTextureManager, m_nNumParts, m_fAccel, m_fInertia, m_fJumpPower, m_fGravity, m_fRivisionRot, m_fColHeight, m_fColRange);
		}
	}
}

//=============================================================================
//    �v���C���[����ǂݎ�鏈��
//=============================================================================
CModel **CPlayerManager::ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pAccel, float *pInertia, float *pJumpPower, float *pRivisionRot, float *pGravity, float *pColHeight, float *pColRange)
{
	int nCntParts = 0;
	int nNumParts = 0;
	CModel **apModel = NULL;
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, MOVE) == 0)
		{// �ړ��ʂ�����
			*pAccel = CFunctionLib::ReadFloat(pStrCur, MOVE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, JUMP) == 0)
		{// �W�����v�ʂ�����
			*pJumpPower = CFunctionLib::ReadFloat(pStrCur, JUMP);
		}
		else if (CFunctionLib::Memcmp(pStrCur, GRAVITY) == 0)
		{// �d�͗ʂ�����
			*pGravity = CFunctionLib::ReadFloat(pStrCur, GRAVITY);
		}
		else if (CFunctionLib::Memcmp(pStrCur, INERTIA) == 0)
		{// �����ʂ�����
			*pInertia = CFunctionLib::ReadFloat(pStrCur, INERTIA);
		}
		else if (CFunctionLib::Memcmp(pStrCur, RIVISION_ANGLE) == 0)
		{// ������␳����{���ʂ�����
			*pRivisionRot = CFunctionLib::ReadFloat(pStrCur, RIVISION_ANGLE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_HEIGHT) == 0)
		{//	�����蔻�����鍂��������
			*pColHeight = CFunctionLib::ReadFloat(pStrCur, COL_HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// �����蔻������͈͂�����
			*pColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		if (CFunctionLib::Memcmp(pStrCur, NUM_PARTS) == 0)
		{// �p�[�c����񂪂���
			*pNumParts = (CFunctionLib::ReadInt(pStrCur, NUM_PARTS));
			if (apModel == NULL)
			{// ���������m�ۂł����Ԃł���
				apModel = new CModel*[*pNumParts];
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARTSSET) == 0)
		{// �p�[�c���ǂݍ��݊J�n�̍��}������
			if (nCntParts < *pNumParts)
			{// �܂��p�[�c����ǂݍ��߂�
				apModel[nCntParts] = ReadPartsSet(pLine, pStrCur, pFile, pNumParts, pNumParent, apModel);
				nCntParts++;
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_CHARACTERSET) == 0)
		{// �L�����N�^�[���ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return apModel;
}

//=============================================================================
//    �p�[�c����ǂݎ�鏈��
//=============================================================================
CModel *CPlayerManager::ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel)
{
	int nIdx = 0;                                      // �p�[�c�ԍ����
	int nParent = 0;                                   // �e���f���̃p�[�c�ԍ�
	CModel *pModel = NULL;                             // ���f���N���X�ւ̃|�C���^
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // ���f���̍��W���
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // ���f���̌������
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
		{// �p�[�c�ԍ���񂪂���
			nIdx = CFunctionLib::ReadInt(pStrCur, INDEX);
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARENT) == 0)
		{// �e���f���̃p�[�c�ԍ���񂪂���
			pNumParent[nIdx] = CFunctionLib::ReadInt(pStrCur, PARENT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���f���̍��W��񂪂���
			Pos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ���f���̌�����񂪂���
			Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_PARTSSET) == 0)
		{// �p�[�c���ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	if (pModel == NULL)
	{// ���������m�ۂł����Ԃł���
		pModel = CModel::Create(Pos, Rot, m_pModelManager->GetMesh(nIdx), m_pModelManager->GetBuffMat(nIdx), m_pModelManager->GetNumMat(nIdx), m_pModelManager->GetVtxMax(nIdx), m_pModelManager->GetVtxMin(nIdx), m_pModelManager->GetTexture(nIdx));
		if (pModel != NULL)
		{// ���f���|�C���^���m�ۂ��ꂽ
			if (nParent != -1)
			{// �e���f�������݂���
				pModel->SetParent(apModel[nParent]);
			}
		}
	}

	return pModel;
}

//=============================================================================
//    ���[�V��������ǂݎ�鏈��
//=============================================================================
CMotion *CPlayerManager::ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion)
{
	int nCntKeySet = 0;                           // �L�[�t���[���t���[������ǂݍ��񂾉�
	CMotionAttack *pMotionAttack = NULL;          // �U�����[�V�����f�[�^�N���X�ւ̃|�C���^
	CMotionCollision **pMotionCollision = NULL;   // ���[�V�����̓����蔻��f�[�^�N���X�ւ̃|�C���^
	int nNumColData = 0;                          // �����蔻��f�[�^�̐�
	CMotionOrbit **pMotionOrbit = NULL;           // ���[�V�����̋O�Ճf�[�^�N���X�ւ̃|�C���^
	int nNumOrbitData = 0;                        // �O�Ճf�[�^�̐�
	bool bLoop = false;                           // ���[�v���邩���Ȃ���
	float fBlendMag = 0.0f;                       // �u�����h�{��
	int nNumKey = 0;                              // �L�[�t���[���̑���
	int *pPlaybackKey = NULL;                     // �Đ��t���[����(��������̂œ��I�m��)
	D3DXVECTOR3 **pAddPos = NULL;                 // ���f���̍��W
	D3DXVECTOR3 **pDestRot = NULL;                // ���f���̌���
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, LOOP) == 0)
		{// ���[�v���邩���Ȃ����̏�񂪂���
			bLoop = CFunctionLib::ReadBool(pStrCur, LOOP);
		}
		else if (CFunctionLib::Memcmp(pStrCur, BLEND) == 0)
		{// �u�����h�{���̏�񂪂���
			fBlendMag = CFunctionLib::ReadFloat(pStrCur, BLEND);
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_KEY) == 0)
		{// �L�[�t���[���̑����̏�񂪂���
			nNumKey = (CFunctionLib::ReadInt(pStrCur, NUM_KEY));
			if (nNumKey >= 1)
			{// 1�ȏ�L�[�t���[��������
			    // �K�v�ȕϐ��𓮓I�Ɋm�ۂ���
			    // �Đ��t���[����
				pPlaybackKey = new int[nNumKey];

				// ���W
				if (pAddPos == NULL)
				{// ���������m�ۂł����Ԃł���
					pAddPos = new D3DXVECTOR3*[nNumKey];
					if (pAddPos != NULL)
					{// ���������m�ۂł���
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// �L�[�t���[���̑������J��Ԃ�
							pAddPos[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}

				if (pDestRot == NULL)
				{// ���������m�ۂł����Ԃł���
				 // ����
					pDestRot = new D3DXVECTOR3*[nNumKey];
					if (pDestRot != NULL)
					{// ���������m�ۂł���
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// �L�[�t���[���̑������J��Ԃ�
							pDestRot[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, ATTACKSET) == 0)
		{// �����蔻����ǂݍ��݊J�n�̍��}������
			pMotionAttack = ReadMotionAttackSet(pLine, pStrCur, pFile);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISIONSET) == 0)
		{// �����蔻����ǂݍ��݊J�n�̍��}������
			pMotionCollision = ReadMotionCollisionSet(pLine, pStrCur, pFile, &nNumColData);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ORBITSET) == 0)
		{// �O�Տ��ǂݍ��݊J�n�̍��}������
			pMotionOrbit = ReadMotionOrbitSet(pLine, pStrCur, pFile, &nNumOrbitData);
		}
		else if (CFunctionLib::Memcmp(pStrCur, KEYSET) == 0)
		{// �L�[�t���[�����ǂݍ��݊J�n�̍��}������
			ReadKeySet(pLine, pStrCur, pFile, nCntKeySet, pPlaybackKey, pAddPos[nCntKeySet], pDestRot[nCntKeySet]);
			nCntKeySet++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_MOTIONSET) == 0)
		{// ���[�V�������ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	if (pMotion == NULL)
	{// ���������m�ۂł����Ԃł���
		pMotion = CMotion::Create(*pNumParts, nNumKey, fBlendMag, bLoop, pPlaybackKey);
		if (pMotion != NULL)
		{// ���������m�ۂł���
			// �U�����[�V�����f�[�^�N���X��ݒ肷��
			pMotion->SetAttack(pMotionAttack);

		    // �����蔻��f�[�^�N���X��ݒ肷��
			pMotion->SetCollision(pMotionCollision);
			pMotion->SetNumColData(nNumColData);

			// �O�Ճf�[�^�N���X��ݒ肷��
			pMotion->SetOrbit(pMotionOrbit);
			pMotion->SetNumOrbitData(nNumOrbitData);

			for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
			{// �L�[�t���[���̑������J��Ԃ�
				for (int nCntParts = 0; nCntParts < *pNumParts; nCntParts++)
				{// �L�[�t���[���̑������J��Ԃ�
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pAddPos[nCntKey][nCntParts]);
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pDestRot[nCntKey][nCntParts]);
				}
			}
		}
	}

	// ���������m�ۂ������J������
	// �Đ��t���[����
	if (pPlaybackKey != NULL)
	{// ���������m�ۂ���Ă���
		delete[] pPlaybackKey;
		pPlaybackKey = NULL;
	}

	// ���W
	if (pAddPos != NULL)
	{// ���������m�ۂł���
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// �L�[�t���[���̑������J��Ԃ�
			delete[] pAddPos[nCntKey];
			pAddPos[nCntKey] = NULL;
		}
		delete[] pAddPos;
		pAddPos = NULL;
	}

	// ����
	if (pDestRot != NULL)
	{// ���������m�ۂł���
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// �L�[�t���[���̑������J��Ԃ�
			delete[] pDestRot[nCntKey];
			pDestRot[nCntKey] = NULL;
		}
		delete[] pDestRot;
		pDestRot = NULL;
	}

	return pMotion;
}

//=============================================================================
//    ���[�V�����̍U�����[�V��������ǂݎ�鏈��
//=============================================================================
CMotionAttack *CPlayerManager::ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile)
{
	CMotionAttack *pMotionAttack = NULL;   // �U�����[�V�����f�[�^�N���X�ւ̃|�C���^
	pMotionAttack = new CMotionAttack;     // ���������m�ۂ���
	if (pMotionAttack != NULL)
	{// ���������m�ۂł���
		while (1)
		{// ���[�v�J�n
			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
			if (CFunctionLib::Memcmp(pStrCur, NEXT_MOTION) == 0)
			{// ���̃��[�V�����̔ԍ���񂾂���
				pMotionAttack->SetNextMotionIdx(CFunctionLib::ReadInt(pStrCur, NEXT_MOTION));
			}
			else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
			{// �O�Ղ��o���n�߂�^�C�~���O��񂾂���
				pMotionAttack->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
			}
			else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
			{// �O�Ղ����܂��^�C�~���O��񂾂���
				pMotionAttack->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
			}
			else if (CFunctionLib::Memcmp(pStrCur, END_ATTACKSET) == 0)
			{// �U�����[�V�����f�[�^�ǂݍ��ݏI���̍��}������
				break;  // ���[�v�I��
			}
		}
	}

	return pMotionAttack;
}

//=============================================================================
//    ���[�V�����̓����蔻�����ǂݎ�鏈��
//=============================================================================
CMotionCollision **CPlayerManager::ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData)
{
	int nCntCollision = 0;                       // �����蔻��f�[�^��ǂݍ��񂾉�
	CMotionCollision **pMotionCollision = NULL;  // ���[�V�����̓����蔻��f�[�^�N���X�ւ̃|�C���^

	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, NUM_COL) == 0)
		{// �����蔻��f�[�^�̐�������
			*pNumColData = CFunctionLib::ReadInt(pStrCur, NUM_COL);
			if (*pNumColData >= 1)
			{// �����蔻��f�[�^���P�ȏ゠��
				pMotionCollision = new CMotionCollision*[*pNumColData];
				if (pMotionCollision != NULL)
				{// ���������m�ۂł���
					for (int nCntCol = 0; nCntCol < *pNumColData; nCntCol++)
					{// �����蔻��f�[�^�̐������J��Ԃ�
						pMotionCollision[nCntCol] = new CMotionCollision;
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
		{// �����蔻��f�[�^�ǂݍ��݊J�n�̍��}������
			while (1)
			{// ���[�v�J�n
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
				if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
				{// �����蔻�����郂�f���̔ԍ���񂾂���
					pMotionCollision[nCntCollision]->SetModelIdx(CFunctionLib::ReadInt(pStrCur, INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET) == 0)
				{// �����蔻�����郂�f������̃I�t�Z�b�g������񂾂���
					pMotionCollision[nCntCollision]->SetOffsetPos(CFunctionLib::ReadVector3(pStrCur, OFFSET));
				}
				else if (CFunctionLib::Memcmp(pStrCur, RANGE) == 0)
				{// �����蔻������͈͏�񂾂���
					pMotionCollision[nCntCollision]->SetRange(CFunctionLib::ReadFloat(pStrCur, RANGE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
				{// �O�Ղ��o���n�߂�^�C�~���O��񂾂���
					pMotionCollision[nCntCollision]->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
				}
				else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
				{// �O�Ղ����܂��^�C�~���O��񂾂���
					pMotionCollision[nCntCollision]->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_COLLISION) == 0)
				{// �����蔻��f�[�^�ǂݍ��ݏI���̍��}������
					nCntCollision++;
					break;  // ���[�v�I��
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_COLLISIONSET) == 0)
		{// �����蔻����ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return pMotionCollision;
}

//=============================================================================
//    ���[�V�����̋O�Տ���ǂݎ�鏈��
//=============================================================================
CMotionOrbit **CPlayerManager::ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData)
{
	int nCntOrbit = 0;                   // �O�Ճf�[�^��ǂݍ��񂾉�
	CMotionOrbit **pMotionOrbit = NULL;  // ���[�V�����̋O�Ճf�[�^�N���X�ւ̃|�C���^

	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, NUM_ORBIT) == 0)
		{// �O�Ճf�[�^�̐�������
			*pNumOrbitData = CFunctionLib::ReadInt(pStrCur, NUM_ORBIT);
			if (*pNumOrbitData >= 1)
			{// �����蔻��f�[�^���P�ȏ゠��
				pMotionOrbit = new CMotionOrbit*[*pNumOrbitData];
				if (pMotionOrbit != NULL)
				{// ���������m�ۂł���
					for (int nCntOr = 0; nCntOr < *pNumOrbitData; nCntOr++)
					{// �����蔻��f�[�^�̐������J��Ԃ�
						pMotionOrbit[nCntOr] = new CMotionOrbit;
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, ORBIT) == 0)
		{// �O�Ճf�[�^�ǂݍ��݊J�n�̍��}������
			while (1)
			{// ���[�v�J�n
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
				if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
				{// �O�Ղ��o�����f���̔ԍ���񂾂���
					pMotionOrbit[nCntOrbit]->SetModelIdx(CFunctionLib::ReadInt(pStrCur, INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, TEX_INDEX) == 0)
				{// �O�Ղɒ���t����e�N�X�`���̔ԍ���񂾂���
					pMotionOrbit[nCntOrbit]->SetTexIdx(CFunctionLib::ReadInt(pStrCur, TEX_INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET1) == 0)
				{// �O�Ղ��o�����f������̃I�t�Z�b�g�������(�P��)������
					pMotionOrbit[nCntOrbit]->SetOffsetPos1(CFunctionLib::ReadVector3(pStrCur, OFFSET1));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET2) == 0)
				{// �O�Ղ��o�����f������̃I�t�Z�b�g�������(�Q��)������
					pMotionOrbit[nCntOrbit]->SetOffsetPos2(CFunctionLib::ReadVector3(pStrCur, OFFSET2));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSETAMP1) == 0)
				{// �O�Ղ̃I�t�Z�b�g���������l(1��)������
					pMotionOrbit[nCntOrbit]->SetOffsetAmp1(CFunctionLib::ReadVector3(pStrCur, OFFSETAMP1));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSETAMP2) == 0)
				{// �O�Ղ̃I�t�Z�b�g���������l(2��)������
					pMotionOrbit[nCntOrbit]->SetOffsetAmp2(CFunctionLib::ReadVector3(pStrCur, OFFSETAMP2));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLUP) == 0)
				{// �O�Ղ̒��_�㑤�̐F������
					pMotionOrbit[nCntOrbit]->SetColUp(CFunctionLib::ReadVector4(pStrCur, COLUP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLDOWN) == 0)
				{// �O�Ղ̒��_�����̐F������
					pMotionOrbit[nCntOrbit]->SetColDown(CFunctionLib::ReadVector4(pStrCur, COLDOWN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ALPHA_DECAY_UP) == 0)
				{// �O�Ղ̒��_�㑤�̓����x�����l������
					pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(CFunctionLib::ReadFloat(pStrCur, ALPHA_DECAY_UP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ALPHA_DECAY_DOWN) == 0)
				{// �O�Ղ̒��_�����̓����x�����l������
					pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(CFunctionLib::ReadFloat(pStrCur, ALPHA_DECAY_DOWN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
				{// �O�Ղ̉��̕�������񂾂���
					pMotionOrbit[nCntOrbit]->SetXBlock(CFunctionLib::ReadInt(pStrCur, XBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ZBLOCK) == 0)
				{// �O�Ղ̏c�̕�������񂾂���
					pMotionOrbit[nCntOrbit]->SetYBlock(CFunctionLib::ReadInt(pStrCur, ZBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
				{// �O�Ղ��o���n�߂�^�C�~���O��񂾂���
					pMotionOrbit[nCntOrbit]->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
				}
				else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
				{// �O�Ղ����܂��^�C�~���O��񂾂���
					pMotionOrbit[nCntOrbit]->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_ORBIT) == 0)
				{// �O�Ճf�[�^�ǂݍ��ݏI���̍��}������
					nCntOrbit++;
					break;  // ���[�v�I��
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_ORBITSET) == 0)
		{// �O�Տ��ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return pMotionOrbit;
}

//=============================================================================
//    �L�[�t���[������ǂݎ�鏈��
//=============================================================================
void CPlayerManager::ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	int nCntKey = 0;
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, FRAME) == 0)
		{// �Đ��t���[����������
			pPlaybackKey[nCntKeySet] = CFunctionLib::ReadInt(pStrCur, FRAME);
		}
		if (CFunctionLib::Memcmp(pStrCur, KEY) == 0)
		{// �L�[���ǂݍ��݊J�n�̍��}������
			ReadKey(pLine, pStrCur, pFile, nCntKey, AddPos, DestRot);
			nCntKey++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEYSET) == 0)
		{// �L�[�t���[�����ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �L�[����ǂݎ�鏈��
//=============================================================================
void CPlayerManager::ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W������
			AddPos[nCntKey] = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ����������
			DestRot[nCntKey] = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEY) == 0)
		{// �L�[���ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �v���C���[�f�[�^�̃X�N���v�g�t�@�C������ݒ肷�鏈��
//=============================================================================
void CPlayerManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CPlayerManager::SetModelManager(CModelManager *pModelManager)
{
	m_pModelManager = pModelManager;
}

//=============================================================================
//    �v���C���[�f�[�^�̃X�N���v�g�t�@�C�������擾���鏈��
//=============================================================================
char *CPlayerManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CModelManager *CPlayerManager::GetModelManager(void)
{
	return m_pModelManager;
}


//*****************************************************************************
//     CPlayer�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	// �e��l�̐ݒ�
	m_State = STATE_NORMAL;                  // ���
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �ړ���
	m_fAccel = 0.0f;                         // �����x
	m_fInertia = 0.0f;                       // ����
	m_fJumpPower = 0.0f;                     // �W�����v��
	m_fRivisionRot = 0.0f;                   // ������␳����{��
	m_bJump = false;                         // �W�����v���Ă��邩�ǂ���
	m_bAction = false;                       // �A�N�V�������Ă��邩�ǂ���
	m_bOrbit = false;                        // �O�Ղ��o���Ă��邩�ǂ���
	m_pEffectManager = NULL;                 // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	m_pOrbitEffect = NULL;                   // �O�ՃG�t�F�N�g�N���X�ւ̃|�C���^
	m_bBooking = false;                      // �A�N�V�����̗\�񂪓����Ă��邩�ǂ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
//    ��������
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange,int nPriority)
{
	CPlayer *pPlayer = NULL;               // �v���C���[�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pPlayer == NULL)
		{// ����������ɂȂ��Ă���
			pPlayer = new CPlayer(nPriority);
			if (pPlayer != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pPlayer->Init(pos, rot, apModel, pMotionManager, pTextureManager, nNumParts, fAccel, fInertia, fJumpPower, fGravity, fRivisionRot, fColHeight, fColRange)))
				{// �������Ɏ��s����
					return NULL;
				}
			}
			else
			{// �C���X�^���X�𐶐��ł��Ȃ�����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ����ȏ�V�[�����쐬�ł��Ȃ�
		return NULL;
	}

	return pPlayer;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager,int nNumParts ,float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange)
{
	// �e��l�̐ݒ�
	SetPos(pos);                        // ���݂̍��W
	SetRot(rot);                        // ���݂̌���
	SetNumParts(nNumParts);             // �p�[�c��
	SetModel(apModel);                  // ���f���N���X�ւ̃|�C���^
	SetMotionManager(pMotionManager);   // ���[�V�����Ǌ��N���X�ւ̃|�C���^
	SetTextureManager(pTextureManager); // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	SetAccel(fAccel);                   // �����x
	SetInertia(fInertia);               // ����
	SetGravity(fGravity);               // �d��
	SetJumpPower(fJumpPower);           // �W�����v��
	SetRivisionRot(fRivisionRot);       // ������␳����{��
	SetColHeight(fColHeight);           // �����蔻�����鍂��
	SetColRange(fColRange);             // �����蔻������͈�

	// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^����肵�Ă���
	if (m_pEffectManager == NULL)
	{// �|�C���^���m�ۂł����Ԃł���
		CScene *pScene = NULL;
		CScene *pSceneNext = NULL;
		for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
		{// �D�揇�ʂ̐������J��Ԃ�
			pScene = GetTop(nCntPriority);
			while (pScene != NULL)
			{// ����������ɂȂ�܂ŌJ��Ԃ�
				pSceneNext = pScene->GetNext();
				if (pScene->GetObjType() == OBJTYPE_EFFECTMANAGER)
				{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^������
					m_pEffectManager = (CEffectManager*)pScene;
				}
				pScene = pSceneNext;
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^����ɂ��Ă���
	m_pEffectManager = NULL;

	// �O�ՃG�t�F�N�g�N���X�ւ̃|�C���^����ɂ��Ă���
	m_pOrbitEffect = NULL;

	// ���ʂ̏I������
	CCharacter::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	if (CManager::GetGame()->GetState() == CGame::STATE_NORMAL)
	{
		// ����Ă���I�u�W�F�N�g�ւ̃|�C���^����ɂ���
		SetLandScene(NULL);

		// �O��̍��W��ۑ�
		SetPosOld(GetPos());
		if (m_State != STATE_DEATH)
		{// ���S��ԂłȂ�
			if (m_State != STATE_LANDING)
			{// �A�N�V������ԂłȂ������n��ԂłȂ�
				if (m_bAction != true)
				{// �A�N�V������ԂłȂ�
					// �ړ��ʌv�Z����
					InputMove();
				}
			}

			// �ړ�����
			Movement();

			// �����蔻�菈��
			Collision();

			if (m_State != STATE_LANDING)
			{// ���n��ԂłȂ�
				// �A�N�V��������
				Action();
			}

			// ��ԑJ�ڏ���
			Statement();

			// �G�t�F�N�g���쐬���鏈��
			CreateEffect();

			if (GetMotionManager() != NULL)
			{// ���[�V�����Ǘ��N���X����������Ă���
				GetMotionManager()->Update(GetModel());
			}

			// �f�o�b�O�p
			if (CManager::GetKeyboard()->GetPress(DIK_3) == true)
			{// �������̓��͂����ꂽ
				m_State = STATE_DEATH;
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
		else
		{
			// �G�t�F�N�g���쐬���鏈��
			CreateEffect();

			if (GetMotionManager() != NULL)
			{// ���[�V�����Ǘ��N���X����������Ă���
				GetMotionManager()->Update(GetModel());
			}

			// ���񂾂Ƃ��̏���
			Death();
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// ���ʂ̕`�揈��
	CCharacter::Draw();
}

//=============================================================================
//    ���͂ɂ��ړ��ʌv�Z����
//=============================================================================
void CPlayer::InputMove(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 DestRot = GetDestRot();   // �ړI�̌���

	// �L�[�{�[�h���擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �R���g���[���[���擾
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	// �J�������擾
	CCamera *pCamera = CManager::GetCamera();

	if (pKeyboard->GetPress(DIK_A) == true)
	{// �������̓��͂����ꂽ
		m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;

		// �ړI�̊p�x�ύX
		DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{// �E�����̓��͂����ꂽ
		// �ړ�����
		m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;

		// �ړI�̊p�x�ύX
		DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.5f);
	}

	else
	{
		if (pJoyStick != NULL)
		{// �W���C�X�e�B�b�N���擾�ł���
			if (pJoyStick->GetLeftAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisX() <= -DI_STICK_AXIS_MIN)
			{// ���A�i���O���͂�����Ă���
			    // �X�e�B�b�N�̓|���ꂽ�����̔䗦���v�Z
				float fLengthX = (float)(sqrt(pJoyStick->GetLeftAxisX() * pJoyStick->GetLeftAxisX() + pJoyStick->GetLeftAxisY() * pJoyStick->GetLeftAxisY()) / DI_JOY_STICK_INPUT);

				// �R�T�C���J�[�u�Ŕ䗦��␳
				float fRivision = cosf(sqrtf(2) - fLengthX);
				if (fRivision < 1.0)
				{// �{����1�𒴂��Ă��Ȃ�
					fRivision += 0.09f;  // ����Ȃ�����␳
					if (fRivision >= 1.0f)
					{// �{����1�𒴂��Ă��܂���
						fRivision = 1.0f;
					}
				}

				// �ړ��ʌv�Z����
				m_Move.x += sinf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (m_fAccel * fRivision);

				// �ړI�̊p�x�ύX
				DestRot.y = pCamera->GetRot().y - pJoyStick->GetLeftAxis();
			}
			else if (pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_30) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_31) == true)
			{// �\���L�[���͂�����Ă���
			    // �ړ��ʌv�Z����
				m_Move.x += sinf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (m_fAccel);

				// �ړI�̊p�x�ύX
				DestRot.y = pCamera->GetRot().y + (pJoyStick->GetRadian() - D3DX_PI);
			}
		}
	}

	// �e��l�̐ݒ�
	SetDestRot(DestRot);    // �ړI�̌���
}

//=============================================================================
//    �v�Z���ꂽ�ړ��ʂ����W���ɉ����鏈��
//=============================================================================
void CPlayer::Movement(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();           // ���݂̍��W
	D3DXVECTOR3 rot = GetRot();           // ���݂̌���
	D3DXVECTOR3 DestRot = GetDestRot();   // �ړI�̌���
	D3DXVECTOR3 DiffRot = GetDiffRot();   // �ړI�̌����ƌ��݂̌����̍���
	float fGravity = GetGravity();        // �d��

	// ���݂̍��W�Ɉړ��ʂ�������
	pos += m_Move;

	// �d�͂�������
	m_Move.y += fGravity;

	// �ړ��ʂɊ�����������
	m_Move.x += (0.0f - m_Move.x) * m_fInertia;
	m_Move.z += (0.0f - m_Move.z) * m_fInertia;

	// �p�x�̏C��
	DiffRot.y = DestRot.y - rot.y;   // ���݂̌����ƖړI�̌����̍������v�Z

	if (DiffRot.y > D3DX_PI)
	{// ������D3DX_PI�𒴂���
		DiffRot.y -= D3DX_PI * 2.0f;
	}
	if (DiffRot.y < -D3DX_PI)
	{// ������-D3DX_PI�𒴂���
		DiffRot.y += D3DX_PI * 2.0f;
	}

	// ���݂̌����ɖړI�̌����Ƃ̍�����{���ŕ␳����
	rot.y += DiffRot.y * m_fRivisionRot;

	if (rot.y > D3DX_PI)
	{// ���݂̌�����D3DX_PI�𒴂���
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// ���݂̌�����-D3DX_PI�𒴂���
		rot.y += D3DX_PI * 2.0f;
	}

	// �e��l�̐ݒ�
	SetPos(pos);          // ���݂̍��W
	SetRot(rot);          // ���݂̌���
	SetDiffRot(DiffRot);  // �ړI�̌����ƌ��݂̌����̍���
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
void CPlayer::Collision(void)
{
	bool bLand = false;   // ����Ă��邩�ǂ���

	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();           // ���݂̍��W

	// �z�u���ƒn�ʂւ̃|�C���^�擾����
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	CObject *pObject = NULL;             // �z�u���N���X�ւ̃|�C���^
	CObjectMove *pObjectMove = NULL;     // �����z�u���N���X�ւ̃|�C���^
	CObjectGoal *pObjectGoal = NULL;     // �S�[���p�̔z�u���N���X�ւ̃|�C���^
	CMeshField *pField = NULL;           // �n�ʃN���X�ւ̃|�C���^
	CBillPresent *pBillPresent = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_MESHFIELD)
			{// �n�ʃN���X�ւ̃|�C���^��������
				pField = (CMeshField*)pScene;
				if (pField != NULL)
				{// �n�ʂ��擾�ł���
					float fFieldHeight = pField->GetPolyHeight(pos);
					if (pos.y < fFieldHeight)
					{// ���W���n�ʂ�艺�ɂȂ���
						m_Move.y = 0.0f;
						pos.y = fFieldHeight;
						if (m_bJump == true)
						{
							Landing();
						}
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// �z�u����������
				pObject = (CObject*)pScene;
				if (pObject != NULL)
				{// �z�u�����擾�ł���
					if (pObject->Collision(&pos, &GetPosOld(), &m_Move, 70.0f, 10.0f, this) == true)
					{// �z�u���̏�ɏ���Ă���
						if (GetLand() == false)
						{// ���n���Ă��Ȃ�������
							SetLand(true);
							Landing();
						}

						if (GetLandScene() == NULL && GetLand() == true && m_bJump == false)
						{// �����ɏ���Ă���
							pos.y = pObject->GetPos().y + pObject->GetVtxMax().y;
						}
						SetLandScene((CScene*)pObject);
						bLand = true;
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_OBJECTMOVE)
			{// �����z�u����������
				pObjectMove = (CObjectMove*)pScene;
				if (pObjectMove != NULL)
				{// �����z�u�����擾�ł���
					if (pObjectMove->Collision(&pos, &GetPosOld(), &m_Move, 70.0f, 10.0f, this) == true)
					{// �����z�u���̏�ɏ���Ă���
						SetLandScene((CScene*)pObjectMove);
						bLand = true;
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_OBJECTGOAL)
			{// �S�[���p�̔z�u����������
				pObjectGoal = (CObjectGoal*)pScene;
				if (pObjectGoal != NULL)
				{// �z�u�����擾�ł���
					if (pObjectGoal->Collision(&pos, &GetPosOld(), &m_Move, 70.0f, 10.0f, this) == true)
					{// �z�u���̏�ɏ���Ă���
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_BILLPRESENT)
			{
				pBillPresent = (CBillPresent*)pScene;
				pBillPresent->Collision(pos);
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}

	// ��l�̕�
	if (CManager::GetMode() == CManager::MODE_GAME)
	{// �Q�[����ʂ�������
		if (CManager::GetGame()->GetNumStageClear() == 0)
		{// �X�e�[�W�P
			if (pos.x >= 500.0f)
			{
				pos.x = 500.0f;
			}
			if (pos.x <= -500.0f)
			{
				pos.x = -500.0f;
			}
		}
		else if (CManager::GetGame()->GetNumStageClear() == 1 || CManager::GetGame()->GetNumStageClear() == 2)
		{// �X�e�[�W2
			if (pos.x >= 1000.0f)
			{
				pos.x = 1000.0f;
			}
			if (pos.x <= -1000.0f)
			{
				pos.x = -1000.0f;
			}
		}
	}

	// �e��l�̐ݒ�
	SetPos(pos);         // ���݂̍��W
	SetLand(bLand);      // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ���
}

//=============================================================================
//    �A�N�V��������
//=============================================================================
void CPlayer::Action(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �R���g���[���[���擾
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	if (pKeyboard->GetTrigger(DIK_SPACE) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_3) == true)
	{// �A�N�V�����{�^���������ꂽ
		BlockBreak();
	}
}

//=============================================================================
//    ��ԑJ�ڏ���
//=============================================================================
void CPlayer::Statement(void)
{
	if (m_State != STATE_LANDING && m_bAction == false)
	{// �W�����v���Ă��Ȃ������n��ԂłȂ����A�N�V���������Ă��Ȃ�
		if (m_Move.x <= 0.1f && m_Move.x >= -0.1f
			&& m_Move.z <= 0.1f && m_Move.z >= -0.1f)
		{// �ړ����Ă��Ȃ�
			if (m_State != STATE_NORMAL)
			{// �ʏ��Ԃł͂Ȃ�
				// ��Ԃ�ʏ�̏�Ԃ�
				m_State = STATE_NORMAL;

				// ���[�V�����؂�ւ�����
				GetMotionManager()->SwitchMotion(GetModel(), m_State);

				if (GetLand() == true)
				{// �����ɏ���Ă���
					CalcOffsetRot();
				}
			}
		}
		else
		{// �ړ����Ă���
			if (m_State != STATE_MOVE)
			{// ������Ԃł͂Ȃ�
				m_State = STATE_MOVE;

				// ���[�V�����؂�ւ�����
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
	}
	else
	{// �W�����v���Ă��邩���n��Ԃ̎�
		if (m_State == STATE_LANDING)
		{// ���n��Ԃ̎�
			if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
			{// ���[�V�����Đ����I������
				m_State = STATE_NORMAL;

				// ���[�V�����؂�ւ�����
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
		if (m_bAction == true)
		{// �A�N�V���������Ă���
			if (GetMotionManager()->GetMotion()[m_State]->GetCollision() != NULL)
			{// �����蔻��f�[�^�N���X����������Ă���
				for (int nCntCol = 0; nCntCol < GetMotionManager()->GetMotion()[m_State]->GetNumColData(); nCntCol++)
				{// �����蔻��f�[�^�̐������J��Ԃ�
					// �����蔻��ɕK�v�ȃf�[�^��錾
					int nModelIdx = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetModelIdx();
					float fRange = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetRange();
					D3DXVECTOR3 Offset = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetOffsetPos();
					int nStartTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetStartTiming();
					int nFinishTiming = GetMotionManager()->GetMotion()[m_State]->GetCollision()[nCntCol]->GetFinishTiming();
					if (GetMotionManager()->GetAllCounter() >= nStartTiming && GetMotionManager()->GetAllCounter() <= nFinishTiming)
					{// �����蔻��𔭐�������^�C�~���O�Ȃ��
						// �U�����̓����蔻��𔭐�������
						AttackCollision(nModelIdx, fRange, Offset);
					}
				}
			}

			if (GetMotionManager()->GetMotion()[m_State]->GetOrbit() != NULL)
			{// �O�Ճf�[�^�N���X����������Ă���
				if (GetMotionManager()->GetAllCounter() >= GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetStartTiming() && m_bOrbit == false)
				{// �O�Ղ��o���^�C�~���O�ɂȂ���
					if (m_pOrbitEffect == NULL)
					{// �I�[�r�b�g�G�t�F�N�g�N���X�ւ̃|�C���^����ɂȂ��Ă���
						// �O�ՂɕK�v�ȏ���錾
						int nModelIdx = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetModelIdx();
						int nTexIdx = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetTexIdx();
						D3DXVECTOR3 OffSetPos1 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetPos1();
						D3DXVECTOR3 OffSetPos2 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetPos2();
						D3DXVECTOR3 OffSetAmp1 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetAmp1();
						D3DXVECTOR3 OffSetAmp2 = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetOffsetAmp2();
						D3DXCOLOR ColUp = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetColUp();
						D3DXCOLOR ColDown = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetColDown();
						float fAlphaDecayUp = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetAlphaDecayUp();
						float fAlphaDecayDown = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetAlphaDecayDown();
						int nXBlock = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetXBlock();
						int nYBlock = GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetYBlock();
						D3DXMATRIX *pMtxParent = GetModel(nModelIdx)->GetMtxWorldPointer();
						m_pOrbitEffect = COrbitEffect::Create(OffSetPos1, OffSetPos2, OffSetAmp1, OffSetAmp2, ColUp, ColDown, fAlphaDecayUp, fAlphaDecayDown, nXBlock, nYBlock, pMtxParent);
						if (m_pOrbitEffect != NULL)
						{// �O�Ղ𐶐��ł���
							m_pOrbitEffect->BindTexture(GetTextureManager()->GetTexture(nTexIdx));
						}
						m_bOrbit = true;
					}
				}
				if (GetMotionManager()->GetAllCounter() >= GetMotionManager()->GetMotion()[m_State]->GetOrbit()[0]->GetFinishTiming())
				{// �O�Ղ����܂��^�C�~���O�ɂȂ���
					if (m_pOrbitEffect != NULL)
					{
						m_pOrbitEffect->Uninit();
						m_pOrbitEffect = NULL;
					}
				}
			}

			if (GetMotionManager()->GetAllCounter() == 20)
			{
				BlockDelete();
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BREAK);
			}

			if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
			{// ���[�V�����Đ����I������
				m_bAction = false;
				m_bOrbit = false;
				m_State = STATE_NORMAL;
				m_bBooking = false;

				// �O�Ղ��J�����Ă���
				if (m_pOrbitEffect != NULL)
				{
					m_pOrbitEffect->Uninit();
					m_pOrbitEffect = NULL;
				}

				// ���[�V�����؂�ւ�����
				GetMotionManager()->SwitchMotion(GetModel(), m_State);
			}
		}
	}
}


//=============================================================================
//    ���n�����Ƃ��̏���
//=============================================================================
void CPlayer::Landing(void)
{
	// �W�����v���Ă��Ȃ���Ԃ�
	m_bJump = false;

	if (m_bAction == false)
	{// �A�N�V���������Ă��Ȃ�
		if (m_Move.x <= 0.1f && m_Move.x >= -0.1f
			&& m_Move.z <= 0.1f && m_Move.z >= -0.1f)
		{// �ړ����Ă��Ȃ�
			// ���n�̃��[�V�����ɐ؂�ւ���
			m_State = STATE_LANDING;
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
		else
		{// �ړ����Ă���
			m_State = STATE_MOVE;

			// ���[�V�����؂�ւ�����
			GetMotionManager()->SwitchMotion(GetModel(), m_State);
		}
	}

	if (GetLand() == true)
	{// �I�t�Z�b�g�ʒu�����[���h�}�g���b�N�X���g���Ċ���o��
		CalcOffsetRot();
	}
}

//=============================================================================
//    �U�����̓����蔻�菈��
//=============================================================================
bool CPlayer::AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset)
{
	bool bHit = false;                 // �U�����q�b�g�������ǂ���
	float fEnemyRange = 0.0f;          // �G�̓����蔻������͈�
	D3DXVECTOR3 OffSet = Offset;       // �����蔻����Ƃ�ꏊ(�e���f������̃I�t�Z�b�g)
	float fHitRange = 0.0f;            // �����蔻�����钷��
	float fLength = 0.0f;              // �����蔻������ꏊ�ƓG�Ƃ̋���

	// �����蔻��������W���m�肳����
	D3DXMATRIX mtxWorld = GetModel(nModelIdx)->GetMtxWorld();
	D3DXVec3TransformCoord(&OffSet, &OffSet,&mtxWorld);

	// ����J�n
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	CEnemy *pEnemy = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// �G�N���X�ւ̃|�C���^������
				pEnemy = (CEnemy*)pScene;
				if (pEnemy != NULL)
				{// �G�N���X�̃|�C���^���擾�ł���
					// �G�̓����蔻������͈͂��擾
					fEnemyRange = pEnemy->GetColRange();

				    // �����蔻�����钷�����v�Z
					fHitRange = sqrtf((fEnemyRange + fRange) * (fEnemyRange + fRange));

					// �����蔻��������W�ƓG�̍��W�Ƃ̋����𑪂�
					D3DXVECTOR3 EnemyPos = pEnemy->GetPos();
					fLength = sqrtf((OffSet.x - EnemyPos.x) * (OffSet.x - EnemyPos.x) + (OffSet.y - EnemyPos.y) * (OffSet.y - EnemyPos.y) + (OffSet.z - EnemyPos.z) * (OffSet.z - EnemyPos.z));

					if (fLength <= fHitRange)
					{// �����蔻��������W�ƓG�̍��W�Ƃ̋����������蔻�����钷�����Z��
					}
				}
			}
			pScene = pSceneNext;
		}
	}

	return bHit;  // �q�b�g�������ǂ�����Ԃ�
}

//=============================================================================
//    �G�t�F�N�g����鏈��
//=============================================================================
void CPlayer::CreateEffect(void)
{
	if (m_pEffectManager != NULL)
	{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^������o���Ă���
		if (m_State == STATE_MOVE)
		{// ��Ԃ�������Ԃł���
			if (GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 0
				|| GetMotionManager()->GetCounter() == 0 && GetMotionManager()->GetKey() == 2)
			{// �G�t�F�N�g�����^�C�~���O�ɂȂ���
				m_pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_DASH);
			}
		}
	}
}

//=============================================================================
//    ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g�������v�Z���鏈��
//=============================================================================
void CPlayer::CalcOffsetRot(void)
{
	if (GetLandScene() != NULL)
	{// �v���C���[������Ă���N���X�ւ̃|�C���^���擾�ł��Ă���
		if (GetLandScene()->GetObjType() == OBJTYPE_OBJECT)
		{// �z�u���N���X��������
			CObject *pObject = (CObject*)GetLandScene();
			D3DXVECTOR3 ObjRot = pObject->GetRot();
			D3DXVECTOR3 OffSetRot;
			OffSetRot = GetRot() - ObjRot;
			if (OffSetRot.y > D3DX_PI)
			{// �~�����𒴂���
				OffSetRot.y -= D3DX_PI * 2.0f;
			}
			if (OffSetRot.y < -D3DX_PI)
			{// �~�����𒴂���
				OffSetRot.y += D3DX_PI * 2.0f;
			}
			SetLandOffsetRot(OffSetRot);
		}
	}
}

//=============================================================================
//    ���񂾂Ƃ��̏���
//=============================================================================
void CPlayer::Death(void)
{
	if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
	{// ���񂾂Ƃ��̃��[�V�������I�����
		Uninit();
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// �Q�[������������
			CGame *pGame = CManager::GetGame();
			if (pGame != NULL)
			{// �Q�[�����擾�ł���
				if (pGame->GetNumStock() - 1 >= 0)
				{// �c�@�����܂�����
					pGame->SetState(CGame::STATE_PLAYER_DEATH);
				}
				else
				{// �c�@���������Ȃ�
					pGame->SetState(CGame::STATE_END);
				}
			}
		}
	}
}

//=============================================================================
//    �u���b�N����������
//=============================================================================
void CPlayer::BlockBreak(void)
{
	if (m_bAction == false)
	{// �A�N�V���������Ă��Ȃ�
		if (m_State == STATE_MOVE)
		{// �����Ă����Ԃ�������
			if (GetLand() == true)
			{// �����ɏ���Ă���
				CalcOffsetRot();
			}
		}

		m_bAction = true;
		m_State = STATE_HOLEOPEN;
		GetMotionManager()->SwitchMotion(GetModel(), m_State);
		if (m_pOrbitEffect != NULL)
		{
			m_pOrbitEffect->Uninit();
			m_pOrbitEffect = NULL;
		}
	}
}

//=============================================================================
//    �u���b�N����������
//=============================================================================
void CPlayer::BlockDelete(void)
{
	// �u���b�N����������
	CScene *pLandObject = GetLandScene();
	CObject *pObject = NULL;
	if (pLandObject != NULL)
	{
		if (pLandObject->GetObjType() == OBJTYPE_OBJECT)
		{// �z�u���N���X�^�̃|�C���^��������
			if (GetRot().y < 0.0f)
			{
				CScene *pNextObject = pLandObject->GetNext();
				if (pNextObject != NULL)
				{
					if (pNextObject->GetObjType() == OBJTYPE_OBJECT)
					{
						pObject = (CObject*)pNextObject;
						if (pObject != NULL)
						{
							CObject *pLand = (CObject*)pLandObject;
							if (pLand != NULL)
							{
								if (pObject->GetPos().y >= pLand->GetPos().y && pObject->GetPos().y <= pLand->GetPos().y)
								{
									pObject->SetDisp(false);
									m_pEffectManager->SetEffect(pObject->GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_BREAK);
								}
							}
						}
					}
				}
			}
			else if (GetRot().y > 0.0f)
			{
				CScene *pPrevObject = pLandObject->GetPrev();
				if (pPrevObject != NULL)
				{
					pObject = (CObject*)pPrevObject;
					if (pObject != NULL)
					{
						CObject *pLand = (CObject*)pLandObject;
						if (pLand != NULL)
						{
							if (pObject->GetPos().y >= pLand->GetPos().y && pObject->GetPos().y <= pLand->GetPos().y)
							{
								pObject->SetDisp(false);
								m_pEffectManager->SetEffect(pObject->GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_BREAK);
							}
						}
					}
				}
			}
		}
	}
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
void CPlayer::SetState(const STATE State)
{
	m_State = State;
	if (m_State == STATE_DEATH)
	{
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PLAYER_DEATH);
	}
}

//=============================================================================
//    �ړ��ʂ�ݒ肷�鏈��
//=============================================================================
void CPlayer::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    �����x��ݒ肷�鏈��
//=============================================================================
void CPlayer::SetAccel(const float fAccel)
{
	m_fAccel = fAccel;
}

//=============================================================================
//    ������ݒ肷�鏈��
//=============================================================================
void CPlayer::SetInertia(const float fInertia)
{
	m_fInertia = fInertia;
}

//=============================================================================
//    �W�����v�ʂ�ݒ肷�鏈��
//=============================================================================
void CPlayer::SetJumpPower(const float fJumpPower)
{
	m_fJumpPower = fJumpPower;
}

//=============================================================================
//    ������␳����{����ݒ肷�鏈��
//=============================================================================
void CPlayer::SetRivisionRot(const float fRivisionRot)
{
	m_fRivisionRot = fRivisionRot;
}

//=============================================================================
//    �W�����v���Ă��邩�ǂ�����ݒ肷�鏈��
//=============================================================================
void CPlayer::SetJump(const bool bJump)
{
	m_bJump = bJump;
}

//=============================================================================
//    �A�N�V�������Ă��邩�ǂ�����ݒ肷�鏈��
//=============================================================================
void CPlayer::SetAction(const bool bAction)
{
	m_bAction = bAction;
}

//=============================================================================
//    �ړ��ʂ��擾���鏈��
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
CPlayer::STATE CPlayer::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �����x���擾���鏈��
//=============================================================================
float CPlayer::GetAccel(void)
{
	return m_fAccel;
}

//=============================================================================
//    �������擾���鏈��
//=============================================================================
float CPlayer::GetInertia(void)
{
	return m_fInertia;
}

//=============================================================================
//    �W�����v�ʂ��擾���鏈��
//=============================================================================
float CPlayer::GetJumpPower(void)
{
	return m_fJumpPower;
}

//=============================================================================
//    ������␳����{�����擾���鏈��
//=============================================================================
float CPlayer::GetRivisionRot(void)
{
	return m_fRivisionRot;
}

//=============================================================================
//    �W�����v���Ă��邩�ǂ������擾���鏈��
//=============================================================================
bool CPlayer::GetJump(void)
{
	return m_bJump;
}

//=============================================================================
//    �A�N�V�������Ă��邩�ǂ������擾���鏈��
//=============================================================================
bool CPlayer::GetAction(void)
{
	return m_bAction;
}