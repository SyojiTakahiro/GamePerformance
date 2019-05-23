//*****************************************************************************
//
//     �G�l�~�[�̏���[enemy.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "enemy.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "modelManager.h"
#include "motion.h"
#include "gauge.h"
#include "score.h"
#include "game.h"
#include "meshField.h"
#include "object.h"
#include "functionlib.h"
#include "debuglog.h"
#include "player.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
// �G�̃f�[�^�ݒ�p�}�N��
#define ENEMY_GAUGE_POS (D3DXVECTOR3(0.0f,90.0f,0.0f))    // �G�̃Q�[�W�I�t�Z�b�g�ʒu


// �l�ǂݍ��݂�����ۂ̖ڈ�ƂȂ镶����
// ����
#define POS                "POS = "                // ���W��ǂݎ�鍇�}
#define ROT                "ROT = "                // ������ǂݎ�鍇�}
#define MOVE               "MOVE = "               // �ړ��ʂ�ǂݎ�鍇�}

// ���̑�
#define ENEMY_SET_FILENAME "ENEMY_SET_FILENAME = " // �G�̔z�u�f�[�^�X�N���v�g�t�@�C����
#define NUM_MODEL          "NUM_MODEL = "          // �ǂݍ��ރ��f���̐�
#define MODEL_FILENAME     "MODEL_FILENAME = "     // �ǂݍ��ރ��f���̃t�@�C����

// �L�����N�^�[���
#define CHARACTERSET       "CHARACTERSET"          // �L�����N�^�[���ǂݍ��݊J�n�̍��}
#define END_CHARACTERSET   "END_CHARACTERSET"      // �L�����N�^�[���ǂݍ��ݏI���̍��}
#define JUMP               "JUMP = "               // �W�����v��
#define GRAVITY            "GRAVITY = "            // �d�͗�
#define COL_HEIGHT         "COL_HEIGHT = "         // �����蔻����Ƃ鍂��
#define COL_RANGE          "COL_RANGE = "          // �����蔻����Ƃ�͈�
#define NUM_PARTS          "NUM_PARTS = "          // �L�����N�^�[�̃p�[�c��

// �p�[�c���
#define PARTSSET           "PARTSSET"              // �p�[�c���ǂݍ��݊J�n�̍��}
#define END_PARTSSET       "END_PARTSSET"          // �p�[�c���ǂݍ��ݏI���̍��}
#define INDEX              "INDEX = "              // �ԍ���ǂݎ�鍇�}
#define PARENT             "PARENT = "             // �e�p�[�c�̔ԍ���ǂݎ�鍇�}

// ���[�V�������
#define MOTIONSET          "MOTIONSET"             // ���[�V�������ǂݍ��݊J�n�̍��}
#define END_MOTIONSET      "END_MOTIONSET"         // ���[�V�������ǂݍ��ݏI���̍��}
#define LOOP               "LOOP = "               // ���[�V���������[�v�Đ����邩�ǂ�����ǂݎ�鍇�}
#define BLEND              "BLEND = "              // ���[�V�����u�����h�̔{����ǂݎ�鍇�}
#define NUM_KEY            "NUM_KEY = "            // ���[�V�����̃L�[�t���[���̐���ǂݎ�鍇�}

// �L�[�t���[�����
#define KEYSET             "KEYSET"                // �L�[�t���[�����ǂݍ��݊J�n�̍��}
#define END_KEYSET         "END_KEYSET"            // �L�[�t���[�����ǂݍ��ݏI���̍��}
#define FRAME              "FRAME = "              // �L�[�t���[���̍Đ��t���[������ǂݎ�鍇�}

// �L�[���
#define KEY                "KEY"                   // �L�[���ǂݍ��݊J�n�̍��}
#define END_KEY            "END_KEY"               // �L�[���ǂݍ��ݏI���̍��}

// �G�̔z�u���
#define ENEMYSET           "ENEMYSET"              // �G�̔z�u�f�[�^�ǂݍ��݊J�n�̍��}
#define END_ENEMYSET       "END_ENEMYSET"          // �G�̔z�u�f�[�^�ǂݍ��ݏI���̍��}
#define LIFE               "LIFE = "               // �G�̗̑�
#define SCORE              "SCORE = "              // �G��|�������ɑ�����X�R�A��

#define ENEMY_SPEED		   (1.5f)				   // �G�̑��x

//*****************************************************************************
//     �ÓI�����o�ϐ��錾
//*****************************************************************************

//*****************************************************************************
//     CEnemyManager�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEnemyManager::CEnemyManager(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̐ݒ�
	m_nCntPop = 0;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEnemyManager::~CEnemyManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CEnemyManager *CEnemyManager::Create(char *pFileName, int nPriority)
{
	CEnemyManager *pEnemyManager = NULL;         // �G�l�~�[�Ǌ��N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pEnemyManager == NULL)
		{// ����������ɂȂ��Ă���
			pEnemyManager = new CEnemyManager(nPriority);
			if (pEnemyManager != NULL)
			{// �C���X�^���X�𐶐��ł���
				pEnemyManager->SetFileName(pFileName);
				if (FAILED(pEnemyManager->Init()))
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

	return pEnemyManager;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CEnemyManager::Init(void)
{
	// �G�l�~�[�̃p�[�c���𔲂��o��
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
			char aSetFileName[256] = "\0";              // �G�̔z�u�f�[�^�X�N���v�g�t�@�C����
			int nNumModel = 0;                          // �ǂݍ��ރ��f����
			int nNumParts = 0;                          // �ǂݍ��ރp�[�c��
			int nCntModel = 0;                          // ���f����ǂݍ��񂾉񐔂𐔂���
			int nCntParts = 0;                          // �p�[�c����ǂݍ��񂾉񐔂𐔂���
			int nCntMotion = 0;                         // ���[�V������ǂݍ��񂾉񐔂𐔂���
			int *pNumParent = NULL;                     // �e���f���̔ԍ�
			CMotionManager *pMotionManager = NULL;      // ���[�V�����Ǘ��N���X�ւ̃|�C���^
			CMotion *pMotion[CEnemy::STATE_MAX] = {};   // ���[�V�����N���X�ւ̃|�C���^
			CModel **apModel = NULL;                    // ���f���N���X�ւ̃|�C���^
			LPD3DXMESH pMesh = NULL;                    // ���b�V�����ւ̃|�C���^
			LPD3DXBUFFER pBuffMat = NULL;               // �}�e���A�����ւ̃|�C���^
			DWORD nNumMat = 0;                          // �}�e���A�����̐�
			float fGravity = 0.0f;                      // �G�̏d�͏��
			float fColHeight = 0.0f;                    // �����蔻�����鍂��
			float fColRange = 0.0f;                     // �����蔻������͈�

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
							if (CFunctionLib::Memcmp(pStrCur, ENEMY_SET_FILENAME) == 0)
							{// �G�̔z�u�f�[�^�X�N���v�g�t�@�C���̃t�@�C���p�X����ǂݍ���
								pStr = CFunctionLib::ReadString(pStrCur, pStr, ENEMY_SET_FILENAME);
								strcpy(aSetFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
							{// ���f������ǂݍ��ނȂ�
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
								apModel = ReadCharacterSet(pLine, pStrCur, pFile, &nNumParts, pNumParent, &fGravity, &fColHeight, &fColRange);
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
				pMotionManager = CMotionManager::Create(CEnemy::STATE_MAX, nNumParts);
				if (pMotionManager != NULL)
				{// ���[�V�����Ǘ��N���X���쐬�ł���
					for (int nCntMotion = 0; nCntMotion < CEnemy::STATE_MAX; nCntMotion++)
					{// ��Ԃ̐������J�肩����
						pMotionManager->SetMotion(pMotion[nCntMotion], nCntMotion);
					}
				}
			}

			// �G��z�u����
			SetEnemy(apModel, pMotionManager, nNumParts, pNumParent, fGravity, fColHeight, fColHeight, aSetFileName);

			// �ǂݎ��p�̃|�C���^���J�����Ă���
			if (apModel != NULL)
			{// ���������m�ۂ���Ă���
				for (int nCntModel = 0; nCntModel < nNumParts; nCntModel++)
				{// �g�p�ł��郂�f�������J��Ԃ�
					if (apModel[nCntModel] != NULL)
					{// �|�C���^���m�ۂ���Ă���
						apModel[nCntModel]->Uninit();

						// �������̊J��
						delete apModel[nCntModel];
						apModel[nCntModel] = NULL;
					}
				}
				// �������̊J��
				delete[] apModel;
				apModel = NULL;
			}

			// ���[�V�����̔j��
			if (pMotionManager != NULL)
			{// �|�C���^���m�ۂ���Ă���
				pMotionManager->Uninit();

				// �������̊J��
				delete pMotionManager;
				pMotionManager = NULL;
			}

			// �e���f���̔ԍ�
			if (pNumParent != NULL)
			{// ���������m�ۂ���Ă���
				delete[] pNumParent;
				pNumParent = NULL;
			}

			fclose(pFile);
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
void CEnemyManager::Uninit(void)
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

	// ���g�̃|�C���^��j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CEnemyManager::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CEnemyManager::Draw(void)
{

}

//=============================================================================
//    �G��z�u���鏈��
//=============================================================================
void CEnemyManager::SetEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, char *pSetFileName)
{
	// �G�̔z�u���𔲂��o��
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(pSetFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                                    // 1�s���ǂݎ��p
			char *pStrCur = NULL;                                  // ���݂̐擪�|�C���^
			char *pStr = NULL;                                     // �擪�|�C���^�ۑ��p
			int nEnemyLife = 0;                                    // �G�̗̑�
			int nEnemyAddScore = 0;                                // �G��|�������ɑ�����X�R�A��
			D3DXVECTOR3 EnemyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �G�̔z�u���W
			D3DXVECTOR3 EnemyRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �G�̌���
			int nCntEnemy = 0;

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
							if (CFunctionLib::Memcmp(pStrCur, ENEMYSET) == 0)
							{// �G�̔z�u���ǂݍ��݊J�n�̍��}������
								while (1)
								{// ���[�v�J�n
									pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
									if (CFunctionLib::Memcmp(pStrCur, LIFE) == 0)
									{// �̗͂�����
										nEnemyLife = CFunctionLib::ReadInt(pStrCur, LIFE);
									}
									else if (CFunctionLib::Memcmp(pStrCur, SCORE) == 0)
									{// �G�̍��W��񂪂���
										nEnemyAddScore = CFunctionLib::ReadInt(pStrCur, SCORE);
									}
									else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
									{// �G�̍��W��񂪂���
										EnemyPos = CFunctionLib::ReadVector3(pStrCur, POS);
									}
									else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
									{// �G�̌�����񂪂���
										EnemyRot = CFunctionLib::ReadVector3(pStrCur, ROT);
									}
									else if (CFunctionLib::Memcmp(pStrCur, END_ENEMYSET) == 0)
									{// �G�̔z�u���ǂݍ��ݏI���̍��}������
										// �G�l�~�[�N���X���쐬����
										CreateEnemy(apModel, pMotionManager, nNumParts, pNumParent, fGravity, fColHeight, fColRange, EnemyPos, EnemyRot, nEnemyLife, nEnemyAddScore);
										nCntEnemy++;
										break;  // ���[�v�I��
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
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
				}
			}
			fclose(pFile);
		}
	}
}

//=============================================================================
//    �G�l�~�[�N���X���쐻���鏈��
//=============================================================================
void CEnemyManager::CreateEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nLife, const int nAddScore)
{
	CModel **apModelCpy = NULL;                    // ���f���N���X�R�s�[�p
	CMotion *pMotionCpy[CEnemy::STATE_MAX] = {};   // ���[�W�����N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;      // ���[�V�����Ǌ��N���X�R�s�[�p
	CEnemy *pEnemy = NULL;                         // �G�l�~�[�N���X�ւ̃|�C���^

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
			pMotionManagerCpy = CMotionManager::Create(CEnemy::STATE_MAX, nNumParts);
			if (pMotionManagerCpy != NULL)
			{// ���[�V�����Ǘ��N���X���쐬�ł���
				for (int nCntMotion = 0; nCntMotion < CEnemy::STATE_MAX; nCntMotion++)
				{// �G�̏�Ԃ̐������J��Ԃ�
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

							// ���[�V�����Ǌ��N���X�Ƀ|�C���^��n��
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// �G�l�~�[�N���X���쐬����
		if (pEnemy == NULL)
		{// ���������m�ۂł����Ԃł���
			pEnemy = CEnemy::Create(pos, rot, apModelCpy, pMotionManagerCpy, nNumParts, fGravity, fColHeight, fColRange, nLife, nAddScore);
		}
	}
}

//=============================================================================
//    �G�l�~�[����ǂݎ�鏈��
//=============================================================================
CModel **CEnemyManager::ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pGravity, float *pColHeight, float *pColRange)
{
	int nCntParts = 0;
	int nNumParts = 0;
	CModel **apModel = NULL;
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, GRAVITY) == 0)
		{// �d�͗ʂ�����
			*pGravity = CFunctionLib::ReadFloat(pStrCur, GRAVITY);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_HEIGHT) == 0)
		{//	�����蔻�����鍂��������
			*pColHeight = CFunctionLib::ReadFloat(pStrCur, COL_HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
		{// �����蔻������͈͂�����
			*pColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_PARTS) == 0)
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
CModel *CEnemyManager::ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel)
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
CMotion *CEnemyManager::ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion)
{
	int nCntKeySet = 0;              // �L�[�t���[���t���[������ǂݍ��񂾉�
	bool bLoop = false;              // ���[�v���邩���Ȃ���
	float fBlendMag = 0.0f;          // �u�����h�{��
	int nNumKey = 0;                 // �L�[�t���[���̑���
	int *pPlaybackKey = NULL;        // �Đ��t���[����(��������̂œ��I�m��)
	D3DXVECTOR3 **pAddPos = NULL;    // ���f���̍��W
	D3DXVECTOR3 **pDestRot = NULL;   // ���f���̌���
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
//    �L�[�t���[������ǂݎ�鏈��
//=============================================================================
void CEnemyManager::ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
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
void CEnemyManager::ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
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
//    �X�N���v�g�t�@�C������ݒ肷�鏈��
//=============================================================================
void CEnemyManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CEnemyManager::SetModelManager(CModelManager *pModelManager)
{
	m_pModelManager = pModelManager;
}

//=============================================================================
//    �X�N���v�g�t�@�C�������擾���鏈��
//=============================================================================
char *CEnemyManager::GetFileName(void)
{
	return m_aFileName;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CModelManager *CEnemyManager::GetModelManager(void)
{
	return m_pModelManager;
}

//*****************************************************************************
//     CEnemy�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	// �e��l�̐ݒ�
	m_State = STATE_NORMAL;                  // ���
	m_nLife = 0;                             // �̗�
	m_nAddScore = 0;                         // �|�������ɑ�����X�R�A��
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �ړ���
	m_pGaugeLife = NULL;                     // �̗̓Q�[�W
	m_bMoveRot = false;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
//    ��������
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore, int nPriority)
{
	CEnemy *pEnemy = NULL;                 // �G�l�~�[�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pEnemy == NULL)
		{// ����������ɂȂ��Ă���
			pEnemy = new CEnemy(nPriority);
			if (pEnemy != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pEnemy->Init(pos, rot, apModel, pMotionManager, nNumParts, fGravity, fColHeight, fColRange, nLife, nAddScore)))
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

	return pEnemy;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CEnemy::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore)
{
	// �e��l�̐ݒ�
	SetPos(pos);                       // ���݂̍��W
	SetRot(rot);                       // ���݂̌���
	SetNumParts(nNumParts);            // �p�[�c��
	SetModel(apModel);                 // ���f���N���X�ւ̃|�C���^
	SetMotionManager(pMotionManager);  // ���[�V�����Ǌ��N���X�ւ̃|�C���^
	SetGravity(fGravity);              // �d��
	SetColHeight(fColHeight);          // �����蔻�����鍂��
	SetColRange(fColRange);            // �����蔻������͈�
	SetLife(nLife);                    // �̗�
	SetAddScore(nAddScore);            // �|�������ɑ�����X�R�A��

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	// �̗̓Q�[�W�̊J��
	if (m_pGaugeLife != NULL)
	{
		m_pGaugeLife->Uninit();
		m_pGaugeLife = NULL;
	}

	// ���ʂ̏I������
	CCharacter::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = GetPosOld();
	D3DXVECTOR3 rot = GetRot();

	posOld = GetPos();
	SetPosOld(posOld);

	if (m_State != STATE_DEATH)
	{
		CScene *pScene = CScene::GetTop(3);

		while (pScene != NULL)
		{
			CScene *pNextScene = pScene->GetNext();

			if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
			{
				CPlayer *pPlayer = (CPlayer*)pScene;
				D3DXVECTOR3 posPlayer = pPlayer->GetPos();

				if (CollisionPlayer(posPlayer, 30) && pPlayer->GetState() != CPlayer::STATE_DEATH)
				{// �v���C���[�Ƃ̓����蔻��
					pPlayer->SetState(CPlayer::STATE_DEATH);
					pPlayer->GetMotionManager()->SwitchMotion(pPlayer->GetModel(), pPlayer->GetState());
					break;
				}
			}
			pScene = pNextScene;
		}

		if (GetLand())
		{// �n��
			STATE state = GetState();
			SetState(STATE_MOVE);

			if (state != STATE_MOVE)
			{// �ړ����[�V����
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}
			if (m_bMoveRot)
			{// �E����
				rot.y = D3DX_PI * -0.5f;
				SetRot(rot);
				m_Move.x = ENEMY_SPEED;

				if (CManager::GetGame()->GetNumStageClear() == 0)
				{// �X�e�[�W�P
					if (pos.x >= 500.0f)
					{
						pos.x = 500.0f;
						m_bMoveRot = false;
					}
				}
				else if (CManager::GetGame()->GetNumStageClear() == 1)
				{// �X�e�[�W2
					if (pos.x >= 1000.0f)
					{
						pos.x = 1000.0f;
						m_bMoveRot = false;
					}
				}
			}
			else
			{// ������
				rot.y = D3DX_PI * 0.5f;
				SetRot(rot);

				m_Move.x = -ENEMY_SPEED;

				if (CManager::GetGame()->GetNumStageClear() == 0)
				{// �X�e�[�W�P
					if (pos.x <= -500.0f)
					{
						pos.x = -500.0f;
						m_bMoveRot = true;
					}
				}
				else if (CManager::GetGame()->GetNumStageClear() == 1)
				{// �X�e�[�W2
					if (pos.x <= -1000.0f)
					{
						m_bMoveRot = true;
					}
				}
			}
		}
		else
		{
			STATE state = GetState();

			SetState(STATE_FALL);

			if (state != STATE_FALL)
			{// �������[�V����
				GetMotionManager()->SwitchMotion(GetModel(), GetState());
			}

			m_Move.x = 0.0f;
		}

		m_Move.y += GetGravity();
		pos += m_Move;

		SetPos(pos);

		// �����蔻�菈��
		Collision();

		// ��ԑJ�ڏ���
		Statement();
	}
	else
	{
		Death();
	}

	if (GetMotionManager() != NULL)
	{// ���[�V�����Ǘ��N���X����������Ă���
		GetMotionManager()->Update(GetModel());
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	// ���ʂ̕`�揈��
	CCharacter::Draw();
}

//=============================================================================
//    �G�����񂾂Ƃ��̏���
//=============================================================================
void CEnemy::Death(void)
{
	if (GetMotionManager()->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
	{// ���񂾂Ƃ��̃��[�V�������I�����
		Uninit();
	}
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
void CEnemy::Collision(void)
{
	bool bLand = false;   // ����Ă��邩�ǂ���

	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();           // ���݂̍��W

	// �z�u���ƒn�ʂւ̃|�C���^�擾����
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	CObject *pObject = NULL;             // �z�u���N���X�ւ̃|�C���^
	CMeshField *pField = NULL;           // �n�ʃN���X�ւ̃|�C���^
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
						}

						if (GetLandScene() == NULL && GetLand() == true)
						{// �����ɏ���Ă���
							pos.y = pObject->GetPos().y + pObject->GetVtxMax().y;
						}
						SetLandScene((CScene*)pObject);
						bLand = true;
					}
				}
			}
			// ���̃I�u�W�F�N�g�ւ̃|�C���^���擾
			pScene = pSceneNext;
		}
	}

	// �e��l�̐ݒ�
	SetPos(pos);         // ���݂̍��W
	SetLand(bLand);      // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ���
}

//=========================================================================================================================
// �����蔻��
//=========================================================================================================================
bool CEnemy::CollisionPlayer(D3DXVECTOR3 pos, float fRange)
{
	bool bHit = false;

	// �����XY�͈̔�
	float fLength = powf(fRange, 2);

	float fLengthX = pos.x - GetMtxWorld()._41;						// X�̍�
	float fLengthY = pos.y - GetMtxWorld()._42;						// Y�̍�
	float fLengthTotal = powf(fLengthX, 2) + powf(fLengthY, 2);		// XY�̍��̓��

	if (fLength >= fLengthTotal)
	{// ����Ǝ����̔���
		bHit = true;
	}

	return bHit;
}

//=============================================================================
//    ��ԑJ�ڏ���
//=============================================================================
void CEnemy::Statement(void)
{

}

//=============================================================================
//    ��Ԃ�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    �̗͂�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    �|�������ɑ�����X�R�A�ʂ�ݒ肷�鏈��
//=============================================================================
void CEnemy::SetAddScore(const int nAddScore)
{
	m_nAddScore = nAddScore;
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
CEnemy::STATE CEnemy::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �̗͂��擾���鏈��
//=============================================================================
int CEnemy::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    �|�������ɑ�����X�R�A�ʂ��擾���鏈��
//=============================================================================
int CEnemy::GetAddScore(void)
{
	return m_nAddScore;
}