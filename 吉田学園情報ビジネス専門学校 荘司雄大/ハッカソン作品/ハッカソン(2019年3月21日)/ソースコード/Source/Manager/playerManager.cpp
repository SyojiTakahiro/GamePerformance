//*****************************************************************************
//
//     �v���C���[�Ǌ��̏���[playerManager.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "character.h"
#include "manager.h"
#include "system.h"
#include "functionlib.h"
#include "model.h"
#include "motion.h"
#include "modelManager.h"
#include "textureManager.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define PLAYER_PRIORITY   (3)                     // �v���C���[�̏����D�揇��

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
#define DAMAGE            "DAMAGE = "             // ���������ꍇ�̃_���[�W
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
//     �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CPlayerManager::CPlayerManager() : CCharacterManager()
{
	// �e��l�̃N���A
	m_nControllerIdx = 0;   // �g�p����R���g���[���[�̔ԍ�
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
CPlayerManager *CPlayerManager::Create(char *pFileName, int nControllerIdx)
{
	CPlayerManager *pPlayerManager = NULL;  // �v���C���[�Ǌ��N���X�^�̃|�C���^
	if (pPlayerManager == NULL)
	{// ����������ɂȂ��Ă���
		pPlayerManager = new CPlayerManager;
		if (pPlayerManager != NULL)
		{// �C���X�^���X�𐶐��ł���
			pPlayerManager->SetFileName(pFileName);
			pPlayerManager->SetControllerIdx(nControllerIdx);
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
		pFile = fopen(GetFileName(), "r");
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
			CTextureManager *pTextureManager = NULL;    // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
			CModelManager *pModelManager = NULL;        // ���f���Ǌ��N���X�ւ̃|�C���^
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
									pTextureManager = CTextureManager::Create(nNumTexture);
									SetTextureManager(pTextureManager);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// �e�N�X�`���̃t�@�C���p�X����ǂݍ���
								if (pTextureManager != NULL && nCntTex < nNumTexture)
								{// �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���m�ۂ���Ă���
								    // �e�N�X�`���̃t�@�C���p�X����ǂݎ��
									pStr = CFunctionLib::ReadString(pStrCur, pStr, TEXTURE_FILENAME);

									// �e�N�X�`���̓ǂݍ���
									D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);
									if (pTexture != NULL)
									{// �e�N�X�`����ǂݍ��߂�
										pTextureManager->BindTexture(pTexture, nCntTex);
										pTextureManager->SetFileName(pStr, nCntTex);
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
									pModelManager = CModelManager::Create(nNumModel);
									SetModelManager(pModelManager);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
							{// ���f���̃t�@�C���p�X����ǂݍ���
								if (pModelManager != NULL && nCntModel < nNumModel)
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
									pModelManager->SetMesh(pMesh, nCntModel);
									pModelManager->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);

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
			CreatePlayer(apModel, pModelManager, pTextureManager, pMotionManager, nNumParts, pNumParent, fAccel, fInertia, fJumpPower, fRivisionRot, fGravity, fColHeight, fColRange);

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
void CPlayerManager::Uninit(void)
{
	// ���ʂ̏I������
	CCharacterManager::Uninit();
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
void CPlayerManager::CreatePlayer(CModel **apModel, CModelManager *pModelManager, CTextureManager *pTextureManager, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange)
{
	CModel **apModelCpy = NULL;                     // ���f���N���X�R�s�[�p
	CMotionAttack *pMotionAttack = NULL;            // �U�����[�V�����f�[�^�N���X�R�s�[�p
	CMotionCollision **pMotionCollision = NULL;     // ���[�V�����̓����蔻��f�[�^�N���X�R�s�[�p
	CMotionOrbit **pMotionOrbit = NULL;             // ���[�V�����̋O�Ճf�[�^�N���X�R�s�[�p
	CMotion *pMotionCpy[CPlayer::STATE_MAX] = {};   // ���[�W�����N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;       // ���[�V�����Ǌ��N���X�R�s�[�p
	CPlayer *pPlayer = NULL;                        // �v���C���[�N���X�ւ̃|�C���^

	if (apModel != NULL && pMotionManager != NULL && pModelManager != NULL)
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
						apModelCpy[nCntParts] = CModel::Create(apModel[nCntParts]->GetPos(), apModel[nCntParts]->GetRot(), pModelManager->GetMesh(nCntParts), pModelManager->GetBuffMat(nCntParts), pModelManager->GetNumMat(nCntParts), pModelManager->GetVtxMax(nCntParts), pModelManager->GetVtxMin(nCntParts), pModelManager->GetTexture(nCntParts));
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
											pMotionCollision[nCntCol]->SetDamage(pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetDamage());
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
			pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), apModelCpy, pMotionManagerCpy, pTextureManager, nNumParts, fAccel, fInertia, fJumpPower, fGravity, fRivisionRot, fColHeight, fColRange, m_nControllerIdx, PLAYER_PRIORITY);
		}
	}
}


//=============================================================================
//    �g�p����R���g���[���[�̔ԍ���ݒ肷��
//=============================================================================
void CPlayerManager::SetControllerIdx(const int nControllerIdx)
{
	m_nControllerIdx = nControllerIdx;
}

//=============================================================================
//    �g�p����R���g���[���[�̔ԍ���ݒ肷��
//=============================================================================
int CPlayerManager::GetControllerIdx(void)
{
	return m_nControllerIdx;
}