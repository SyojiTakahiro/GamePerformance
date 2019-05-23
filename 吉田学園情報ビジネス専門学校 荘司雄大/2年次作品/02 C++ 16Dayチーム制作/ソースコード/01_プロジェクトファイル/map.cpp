//*****************************************************************************
//
//     �}�b�v�̏���[map.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "map.h"
#include "object.h"
#include "dome.h"
#include "cylinder.h"
#include "field.h"
#include "water.h"
#include "modelManager.h"
#include "player.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
// �X�N���v�g�t�@�C���̓ǂݍ��ݗp�̃p�X��
#define MAPSET               "MAPSET"                // �}�b�v�Z�b�g�J�n�̍��}
#define END_MAPSET           "END_MAPSET"            // �}�b�v�Z�b�g�I���̍��}
#define FIELDSET             "FIELDSET"              // �n�ʃZ�b�g�J�n�̍��}
#define END_FIELDSET         "END_FIELDSET"          // �n�ʃZ�b�g�I���̍��}
#define WATERSET             "WATERSET"              // ���Z�b�g�J�n�̍��}
#define END_WATERSET         "END_WATERSET"          // ���Z�b�g�I���̍��}
#define OBJECTSET            "OBJECTSET"             // �z�u���Z�b�g�J�n�̍��}
#define END_OBJECTSET        "END_OBJECTSET"         // �z�u���Z�b�g�I���̍��}
#define OBJECTMOVESET        "OBJECTMOVESET"         // �����z�u���Z�b�g�J�n�̍��}
#define END_OBJECTMOVESET    "END_OBJECTMOVESET"     // �����z�u���Z�b�g�I���̍��}
#define OBJECTBALLOONSET     "OBJECTBALLOONSET"      // �����o���p�̔z�u���Z�b�g�J�n�̍��}
#define END_OBJECTBALLOONSET "END_OBJECTBALLOONSET"  // �����o���p�̔z�u���Z�b�g�I���̍��}
#define OBJECTGOALSET        "OBJECTGOALSET"         // �S�[���p�̔z�u���Z�b�g�J�n�̍��}
#define END_OBJECTGOALSET    "END_OBJECTGOALSET"     // �S�[���p�̔z�u���Z�b�g�I���̍��}
#define OBJECTTARGETSET      "OBJECTTARGETSET"       // �W�b�v���C���p�̔z�u���Z�b�g�J�n�̍��}
#define END_OBJECTTARGETSET  "END_OBJECTTARGETSET"   // �W�b�v���C���p�̔z�u���Z�b�g�I���̍��}
#define MOUNTAINSET          "MOUNTAINSET"           // �R�Z�b�g�J�n�̍��}
#define END_MOUNTAINSET      "END_MOUNTAINSET"       // �R�Z�b�g�I���̍��}
#define SKYSET               "SKYSET"                // ��Z�b�g�J�n�̍��}
#define END_SKYSET           "END_SKYSET"            // ��Z�b�g�I���̍��}

#define FIELD_FILENAME       "FIELD_FILENAME"        // �n�ʂ̃X�N���v�g�t�@�C����
#define WATER_FILENAME       "WATER_FILENAME"        // ���̃X�N���v�g�t�@�C����
#define OBJECT_FILENAME      "OBJECT_FILENAME"       // �z�u���̃X�N���v�g�t�@�C����
#define MOUNTAIN_FILENAME    "MOUNTAIN_FILENAME"     // �R�̃X�N���v�g�t�@�C����
#define SKY_FILENAME         "SKY_FILENAME"          // ��̃X�N���v�g�t�@�C����
#define PLAYER_DEFAULT_POS   "PLAYER_DEFAULT_POS"    // �v���C���[�̍��W�̏����l
#define PLAYER_DEFAULT_ROT   "PLAYER_DEFAULT_ROT"    // �v���C���[�̌����̏����l
#define CAMERA_DEFAULT_ROT   "CAMERA_DEFAULT_ROT"    // �J�����̌����̏����l

#define NUM_MODEL            "NUM_MODEL"             // �ǂݍ��ރ��f����
#define MODEL_FILENAME       "MODEL_FILENAME"        // �ǂݍ��ރ��f���t�@�C����

#define TYPE                 "TYPE"                  // ��ޏ��
#define POS                  "POS"                   // ���W���
#define POS_TARGET           "POS_TARGET"            // �ړI�̈ʒu
#define ROT                  "ROT"                   // �������
#define MOVE                 "MOVE"                  // �ړ��ʏ��
#define PUSH                 "PUSH"                  // �����邩�ǂ���
#define BREAK                "BREAK"                 // �󂹂邩�ǂ���
#define ROLL_SPEED           "ROLL_SPEED"            // ��]����X�s�[�h�ʏ��
#define CARRY_TIME           "CARRY_TIME"            // �v���C���[���^�Ԃ̂ɂ����鎞��
#define COLLISION            "COLLISION"             // �����蔻������邩�ǂ���
#define WIDTH                "WIDTH"                 // 1�u���b�N���̕����
#define DEPTH                "DEPTH"                 // 1�u���b�N�����s���
#define RADIUS               "RADIUS"                // ���a���
#define HEIGHT               "HEIGHT"                // �������
#define XBLOCK               "XBLOCK"                // ���̕��������
#define YBLOCK               "YBLOCK"                // �c�̕��������
#define ZBLOCK               "ZBLOCK"                // ���s�̕��������
#define VERTEX_FILENAME      "VERTEX_FILENAME"       // ���_���̃o�C�i���t�@�C����
#define NEXT_MAPNAME         "NEXT_MAPNAME"          // ���̃}�b�v�f�[�^�̃t�@�C����

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMap::CMap()
{
	// �e��l�̏�����
	m_pModelManager = NULL;  // ���f���Ǌ��N���X�ւ̃|�C���^
	for (int nCntWater = 0; nCntWater < MAX_WATER; nCntWater++)
	{// �z�u�ł��鐅�̐������J��Ԃ�
		m_pWater[nCntWater] = NULL;         // ���N���X�ւ̃|�C���^
	}
	m_pField = NULL;         // �n�ʃN���X�ւ̃|�C���^
	m_pMoutain = NULL;       // �~���N���X�ւ̃|�C���^
	m_pSky = NULL;           // ��N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMap *CMap::Create(char *aFileName)
{
	CMap *pMap = NULL;  // �}�b�v�N���X�^�̃|�C���^
	if (pMap == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pMap = new CMap;
		if (pMap != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pMap->Init(aFileName);
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
	return pMap;
}

//=============================================================================
//    ����������
//=============================================================================
void CMap::Init(char *aFileName)
{
	FILE *pFile = NULL;      // �t�@�C���|�C���^
	char aString[256];       // �e�L�X�g�i�[�p������
	char aFileString[256];   // �t�@�C�����i�[�p������

	// �ǂݍ��ރ}�b�v�̃X�N���v�g�t�@�C������ۑ�
	strcpy(m_aFileName, aFileName);

	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// �e�L�X�g���Ō�̍s�ɓ��B����܂œǂݍ��݂𑱂���
				if (strcmp(&aString[0], MAPSET) == 0)
				{// �}�b�v�Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_MAPSET) != 0)
					{// �}�b�v�Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], FIELD_FILENAME) == 0)
						{// �n�ʂ̃X�N���v�g�t�@�C����������
							if (fscanf(pFile, "%s %s", &aString[0], &aFileString[0]) == 2)
							{// �n�ʂ̃X�N���v�g�t�@�C������ǂݎ�ꂽ
								FieldLoad(aFileString);
							}
						}
						if (strcmp(&aString[0], WATER_FILENAME) == 0)
						{// ���̃X�N���v�g�t�@�C����������
							if (fscanf(pFile, "%s %s", &aString[0], &aFileString[0]) == 2)
							{// ���̃X�N���v�g�t�@�C������ǂݎ�ꂽ
								WaterLoad(aFileString);
							}
						}
						if (strcmp(&aString[0], OBJECT_FILENAME) == 0)
						{// �z�u���̃X�N���v�g�t�@�C����������
							if (fscanf(pFile, "%s %s", &aString[0], &aFileString[0]) == 2)
							{// �z�u���̃X�N���v�g�t�@�C������ǂݎ�ꂽ
								ObjectLoad(aFileString);
							}
						}
						if (strcmp(&aString[0], MOUNTAIN_FILENAME) == 0)
						{// �R�̃X�N���v�g�t�@�C����������
							if (fscanf(pFile, "%s %s", &aString[0], &aFileString[0]) == 2)
							{// �R�̃X�N���v�g�t�@�C������ǂݎ�ꂽ
								MountainLoad(aFileString);
							}
						}
						if (strcmp(&aString[0], SKY_FILENAME) == 0)
						{// ��̃X�N���v�g�t�@�C����������
							if (fscanf(pFile, "%s %s", &aString[0], &aFileString[0]) == 2)
							{// ��̃X�N���v�g�t�@�C������ǂݎ�ꂽ
								SkyLoad(aFileString);
							}
						}

						if (strcmp(&aString[0], PLAYER_DEFAULT_POS) == 0)
						{// �v���C���[�̏������W������
							if (fscanf(pFile, "%s %f %f %f", &aString[0], &m_DefaultPlayerPos.x, &m_DefaultPlayerPos.y, &m_DefaultPlayerPos.z) == 4)
							{// �v���C���[�̏������W��ǂݎ�ꂽ
							    // �v���C���[�N���X�ւ̃|�C���^���擾����
								CPlayer *pPlayer = NULL;
								CScene *pScene = CScene::GetScene(PLAYER_PRIORITY);
								if (pScene != NULL)
								{// �|�C���^���擾�ł���
									CScene *pSceneNext;  // ���̃|�C���^�ۑ��p
									while (pScene != NULL)
									{// ���̃|�C���^���Ȃ��Ȃ�܂Ń��[�v
										pSceneNext = pScene->GetNext();
										if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
										{// �v���C���[�N���X�̃|�C���^������
											pPlayer = (CPlayer*)pScene;
											break;
										}
										pScene = pSceneNext;
									}
								}
								if (pPlayer != NULL)
								{// �v���C���[���擾�ł���
									pPlayer->SetPos(m_DefaultPlayerPos);
									pPlayer->SetPosOld(m_DefaultPlayerPos);
								}
							}
						}
						if (strcmp(&aString[0], PLAYER_DEFAULT_ROT) == 0)
						{// �v���C���[�̏�������������
							if (fscanf(pFile, "%s %f %f %f", &aString[0], &m_DefaultPlayerRot.x, &m_DefaultPlayerRot.y, &m_DefaultPlayerRot.z) == 4)
							{// �v���C���[�̏������W��ǂݎ�ꂽ
							    // �v���C���[�N���X�ւ̃|�C���^���擾����
								CPlayer *pPlayer = NULL;
								CScene *pScene = CScene::GetScene(PLAYER_PRIORITY);
								if (pScene != NULL)
								{// �|�C���^���擾�ł���
									CScene *pSceneNext;  // ���̃|�C���^�ۑ��p
									while (pScene != NULL)
									{// ���̃|�C���^���Ȃ��Ȃ�܂Ń��[�v
										pSceneNext = pScene->GetNext();
										if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
										{// �v���C���[�N���X�̃|�C���^������
											pPlayer = (CPlayer*)pScene;
											break;
										}
										pScene = pSceneNext;
									}
								}
								if (pPlayer != NULL)
								{// �v���C���[���擾�ł���
									pPlayer->SetRot(m_DefaultPlayerRot);
								}
							}
						}
						if (strcmp(&aString[0], CAMERA_DEFAULT_ROT) == 0)
						{// �J�����̏�������������
							if (fscanf(pFile, "%s %f %f %f", &aString[0], &m_DefaultCameraRot.x, &m_DefaultCameraRot.y, &m_DefaultCameraRot.z) == 4)
							{// �J�����̏�������
								CManager::GetCamera()->SetDefaultCamera(m_DefaultCameraRot);
							}
						}
					}
				}
			}
			fclose(pFile);
		}
	}
}

//=============================================================================
//    �I������
//=============================================================================
void CMap::Uninit(void)
{
	// �n�ʂ̔j��
	if (m_pField != NULL)
	{// ���������m�ۂ���Ă���
		m_pField->Uninit();
		m_pField = NULL;
	}

	// �z�u���̔j��
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetScene(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
			{// �z�u���̃N���X��������
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTMOVE)
			{// �����z�u���̃N���X��������
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTBALLOON)
			{// �����o���p�̔z�u���̃N���X��������
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTGOAL)
			{// �S�[���p�z�u���̃N���X��������
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTTARGET)
			{// �W�b�v���C���p�z�u���̃N���X��������
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}

	//// �z�u���̔j��
	//for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	//{// �z�u�ł���I�u�W�F�N�g�̐������J��Ԃ�
	//	if (m_pObject[nCntObject] != NULL)
	//	{// ���������m�ۂ���Ă���
	//		m_pObject[nCntObject]->Uninit();
	//		m_pObject[nCntObject] = NULL;
	//	}
	//}

	//// �����z�u���̔j��
	//for (int nCntObjectMove = 0; nCntObjectMove < MAX_OBJECT_MOVE; nCntObjectMove++)
	//{// �z�u�ł���I�u�W�F�N�g�̐������J��Ԃ�
	//	if (m_pObjectMove[nCntObjectMove] != NULL)
	//	{// ���������m�ۂ���Ă���
	//		m_pObjectMove[nCntObjectMove]->Uninit();
	//		m_pObjectMove[nCntObjectMove] = NULL;
	//	}
	//}

	//// �����o���p�z�u���̔j��
	//for (int nCntObjectBalloon = 0; nCntObjectBalloon < MAX_OBJECT_BALLOON; nCntObjectBalloon++)
	//{// �z�u�ł���I�u�W�F�N�g�̐������J��Ԃ�
	//	if (m_pObjectBalloon[nCntObjectBalloon] != NULL)
	//	{// ���������m�ۂ���Ă���
	//		m_pObjectBalloon[nCntObjectBalloon]->Uninit();
	//		m_pObjectBalloon[nCntObjectBalloon] = NULL;
	//	}
	//}

	//// �S�[���p�z�u���̔j��
	//for (int nCntObjectGoal = 0; nCntObjectGoal < MAX_OBJECT_GOAL; nCntObjectGoal++)
	//{// �z�u�ł���I�u�W�F�N�g�̐������J��Ԃ�
	//	if (m_pObjectGoal[nCntObjectGoal] != NULL)
	//	{// ���������m�ۂ���Ă���
	//		m_pObjectGoal[nCntObjectGoal]->Uninit();
	//		m_pObjectGoal[nCntObjectGoal] = NULL;
	//	}
	//}

	//// �W�b�v���C���p�z�u���̔j��
	//for (int nCntObjectTarget = 0; nCntObjectTarget < MAX_OBJECT_TARGET; nCntObjectTarget++)
	//{// �z�u�ł���I�u�W�F�N�g�̐������J��Ԃ�
	//	if (m_pObjectTarget[nCntObjectTarget] != NULL)
	//	{// ���������m�ۂ���Ă���
	//		m_pObjectTarget[nCntObjectTarget]->Uninit();
	//		m_pObjectTarget[nCntObjectTarget] = NULL;
	//	}
	//}

	// ���̔j��
	for (int nCntWater = 0; nCntWater < MAX_WATER; nCntWater++)
	{// �z�u�ł���I�u�W�F�N�g�̐������J��Ԃ�
		if (m_pWater[nCntWater] != NULL)
		{// ���������m�ۂ���Ă���
			m_pWater[nCntWater]->Uninit();
			m_pWater[nCntWater] = NULL;
		}
	}

	// �R�̔j��
	if (m_pMoutain != NULL)
	{// ���������m�ۂ���Ă���
		m_pMoutain->Uninit();
		m_pMoutain = NULL;
	}

	// ��̔j��
	if (m_pSky != NULL)
	{// ���������m�ۂ���Ă���
		m_pSky->Uninit();
		m_pSky = NULL;
	}

	// ���f���Ǌ��N���X�̔j��
	if (m_pModelManager != NULL)
	{// ���������m�ۂ���Ă���
		m_pModelManager->Uninit();

		// �������̊J��
		delete m_pModelManager;
		m_pModelManager = NULL;
	}
}

//=============================================================================
//    �n�ʏ��ǂݍ��ݏ���
//=============================================================================
void CMap::FieldLoad(char *aFieldFileName)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	char aString[256];   // �e�L�X�g�i�[�p������
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(aFieldFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �n�ʗp�̊i�[�ϐ�
			D3DXVECTOR3 FieldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �n�ʂ̍��W
			D3DXVECTOR3 FieldRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �n�ʂ̌���
			float FieldWidth = 0.0f;                               // �n�ʂ�1�u���b�N���̕�
			float FieldDepth = 0.0f;                               // �n�ʂ�1�u���b�N���̉��s
			int FieldXblock = 0;                                   // �n�ʂ̉��̃u���b�N��
			int FieldZblock = 0;                                   // �n�ʂ̉��s�̃u���b�N��
			char aFieldVertexFile[256];                            // �n�ʂ̒��_��񂪊i�[���ꂽ�t�@�C����

			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// �e�L�X�g���Ō�̍s�ɓ��B����܂œǂݍ��݂𑱂���
				if (strcmp(&aString[0], FIELDSET) == 0)
				{// �n�ʃZ�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_FIELDSET) != 0)
					{// �n�ʃZ�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], POS) == 0)
						{// �n�ʂ̍��W��񂪂���
							fscanf(pFile, "%s %f %f %f", &aString[0], &FieldPos.x, &FieldPos.y, &FieldPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// �n�ʂ̌�����񂪂���
							fscanf(pFile, "%s %f %f %f", &aString[0], &FieldRot.x, &FieldRot.y, &FieldRot.z);
						}
						if (strcmp(&aString[0], WIDTH) == 0)
						{// �n�ʂ�1�u���b�N���̕���񂪂���
							fscanf(pFile, "%s %f", &aString[0], &FieldWidth);
						}
						if (strcmp(&aString[0], DEPTH) == 0)
						{// �n�ʂ�1�u���b�N���̉��s��񂪂���
							fscanf(pFile, "%s %f", &aString[0], &FieldDepth);
						}
						if (strcmp(&aString[0], XBLOCK) == 0)
						{// �n�ʂ̉��̕�������񂪂���
							fscanf(pFile, "%s %d", &aString[0], &FieldXblock);
						}
						if (strcmp(&aString[0], ZBLOCK) == 0)
						{// �n�ʂ̉��s�̕������̉��s��񂪂���
							fscanf(pFile, "%s %d", &aString[0], &FieldZblock);
						}
						if (strcmp(&aString[0], VERTEX_FILENAME) == 0)
						{// �n�ʂ̒��_���̃o�C�i���t�@�C������񂪂���
							fscanf(pFile, "%s %s", &aString[0], &aFieldVertexFile[0]);
						}
					}
					// �n�ʂ̐���
					if (m_pField == NULL)
					{// ���������m�ۂł����Ԃɂ���
						m_pField = CField::Create(FieldPos, FieldRot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FieldWidth, FieldDepth, FieldXblock, FieldZblock, aFieldVertexFile);
					}
				}
			}
			// �t�@�C�������
			fclose(pFile);
		}
	}
}

//=============================================================================
//    �n�ʏ��ǂݍ��ݏ���
//=============================================================================
void CMap::WaterLoad(char *aFieldFileName)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	char aString[256];   // �e�L�X�g�i�[�p������
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(aFieldFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // ���p�̊i�[�ϐ�
			D3DXVECTOR3 WaterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���̍��W
			D3DXVECTOR3 WaterRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���̌���
			float WaterWidth = 0.0f;                               // ����1�u���b�N���̕�
			float WaterDepth = 0.0f;                               // ����1�u���b�N���̉��s
			int WaterXblock = 0;                                   // ���̉��̃u���b�N��
			int WaterZblock = 0;                                   // ���̉��s�̃u���b�N��
			char aWaterVertexFile[256];                            // ���̒��_��񂪊i�[���ꂽ�t�@�C����
			int nCntWater = 0;                                     // ����z�u������

			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// �e�L�X�g���Ō�̍s�ɓ��B����܂œǂݍ��݂𑱂���
				if (strcmp(&aString[0], WATERSET) == 0)
				{// ���Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_WATERSET) != 0)
					{// ���Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], POS) == 0)
						{// ���̍��W��񂪂���
							fscanf(pFile, "%s %f %f %f", &aString[0], &WaterPos.x, &WaterPos.y, &WaterPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// ���̌�����񂪂���
							fscanf(pFile, "%s %f %f %f", &aString[0], &WaterRot.x, &WaterRot.y, &WaterRot.z);
						}
						if (strcmp(&aString[0], WIDTH) == 0)
						{// ����1�u���b�N���̕���񂪂���
							fscanf(pFile, "%s %f", &aString[0], &WaterWidth);
						}
						if (strcmp(&aString[0], DEPTH) == 0)
						{// ����1�u���b�N���̉��s��񂪂���
							fscanf(pFile, "%s %f", &aString[0], &WaterDepth);
						}
						if (strcmp(&aString[0], XBLOCK) == 0)
						{// ���̉��̕�������񂪂���
							fscanf(pFile, "%s %d", &aString[0], &WaterXblock);
						}
						if (strcmp(&aString[0], ZBLOCK) == 0)
						{// ���̉��s�̕������̉��s��񂪂���
							fscanf(pFile, "%s %d", &aString[0], &WaterZblock);
						}
						if (strcmp(&aString[0], VERTEX_FILENAME) == 0)
						{// ���̒��_���̃o�C�i���t�@�C������񂪂���
							fscanf(pFile, "%s %s", &aString[0], &aWaterVertexFile[0]);
						}
					}
					// ���̐���
					if (m_pWater[nCntWater] == NULL)
					{// ���������m�ۂł����Ԃɂ���
						m_pWater[nCntWater] = CWater::Create(WaterPos, WaterRot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), WaterWidth, WaterDepth, WaterXblock, WaterZblock, aWaterVertexFile, nCntWater);
						nCntWater++;
					}
				}
			}
			// �t�@�C�������
			fclose(pFile);
		}
	}
}

//=============================================================================
//    �z�u�����ǂݍ��ݏ���
//=============================================================================
void CMap::ObjectLoad(char *aObjectFileName)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	char aString[256];   // �e�L�X�g�i�[�p������
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(aObjectFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �z�u���p�̊i�[�ϐ�
			CObject *pObject = NULL;
			CObjectMove *pObjectMove = NULL;
			CObjectBalloon *pObjectBalloon = NULL;
			CObjectGoal *pObjectGoal = NULL;
			CObjectTarget *pObjectTarget = NULL;
			int nNumModel;                   // �ǂݍ��ރ��f���̐�
			char aXFileName[256];            // X�t�@�C�����i�[�p
			LPD3DXMESH pMesh = NULL;         // ���b�V�����i�[�p
			LPD3DXBUFFER pBuffMat = NULL;    // �}�e���A�����i�[�p
			DWORD nNumMat = NULL;            // �}�e���A���̐����i�[�p
			D3DXVECTOR3 VtxMax;              // ���_�̍ő�l���i�[�p
			D3DXVECTOR3 VtxMin;              // ���_�̍ŏ��l���i�[�p
			int nCntModelManager = 0;        // �Ǌ��N���X�֊��蓖�Ă�ۂ̃��f���ԍ�
			int nObjectIndex = 0;            // ���蓖�Ă郂�f���̔ԍ�
			D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �z�u���̍��W
			D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �z�u���̌���
			D3DXVECTOR3 ObjectMove = D3DXVECTOR3(0.0f,0.0f,0.0f);          // �z�u���̈ړ���
			D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �z�u���̉�]����X�s�[�h��
			D3DXVECTOR3 ObjectPosTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �z�u���̃v���C���[���^�Ԉʒu
			bool bCollision = true;                                        // �����蔻�����邩�ǂ���
			bool bPush = false;                                            // �z�u���������邩�ǂ���
			bool bBreak = false;                                           // �z�u�����󂹂邩�ǂ���
			int nMaxCarryTime = 0;                                         // �v���C���[���^�Ԃ̂ɂ����鎞��
			int nCntObject = 0;                                            // �z�u����u�������𐔂���ϐ�
			int nCntObjectMove = 0;                                        // �����z�u����u�������𐔂���ϐ�
			int nCntObjectBalloon = 0;                                     // �����o���p�z�u����u�������𐔂���ϐ�
			int nCntObjectGoal = 0;                                        // �S�[���p�z�u����u�������𐔂���ϐ�
			int nCntObjectTarget = 0;                                      // �W�b�v���C���p�z�u����u�������𐔂���ϐ�
			char aNextMapName[256];                                        // ���̃}�b�v�f�[�^�̃t�@�C����

			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// �e�L�X�g���Ō�̍s�ɓ��B����܂œǂݍ��݂𑱂���
				if (strcmp(&aString[0], NUM_MODEL) == 0)
				{// �ǂݍ��ރ��f������񂪂���
					fscanf(pFile, "%s %d", &aString[0], &nNumModel);
					if (nNumModel >= 1)
					{// �ǂݍ��ރ��f���̐���1�ȏ゠��
						m_pModelManager = CModelManager::Create(nNumModel);
					}
				}

				if (strcmp(&aString[0], MODEL_FILENAME) == 0)
				{// �ǂݍ��ރ��f���̃t�@�C������񂪏�����Ă�����
					fscanf(pFile, "%s %s", &aString[0], &aXFileName[0]);
					// x�t�@�C���̓ǂݍ���
					D3DXLoadMeshFromX(aXFileName,
						D3DXMESH_SYSTEMMEM,
						CManager::GetRenderer()->GetDevice(),
						NULL,
						&pBuffMat,
						NULL,
						&nNumMat,
						&pMesh);

					// ���_�̍ŏ��l, �ő�l����ǂݍ���
					if (pMesh != NULL)
					{// ���b�V����񂪓ǂݍ��߂�
						int nNumVtx;     // ���_��
						DWORD sizeFVF;   // ���_�t�H�[�}�b�g�̃T�C�Y
						BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^

						// ���_�̍ŏ��l, �ő�l���i�[�p�ϐ���������
						VtxMax = D3DXVECTOR3(-100000000.0f, -100000000.0f, -100000000.0f);
						VtxMin = D3DXVECTOR3(100000000.0f, 100000000.0f, 100000000.0f);

						// ���_�����l��
					    nNumVtx = pMesh->GetNumVertices();

						// ���_�t�H�[�}�b�g�̃T�C�Y���擾
						sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

						// ���_�o�b�t�@�����b�N
						pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

						for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
						{// ���ׂĂ̒��_���r���ă��f���̍ŏ��l,�ő�l�𔲂��o��
							D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // ���_���W�̑��

							if (Vtx.x < VtxMin.x)
							{// ���o����X���W�̍ŏ��l�����܂ł̂���������
								VtxMin.x = Vtx.x;  // �ŏ��l�X�V
							}
							if (Vtx.y < VtxMin.y)
							{// ���o����Y���W�̍ŏ��l�����܂ł̂���������
								VtxMin.y = Vtx.y;  // �ŏ��l�X�V
							}
							if (Vtx.z < VtxMin.z)
							{// ���o����Z���W�̍ŏ��l�����܂ł̂���������
								VtxMin.z = Vtx.z;  // �ŏ��l�X�V
							}
							if (Vtx.x > VtxMax.x)
							{// ���o����X���W�̍ő�l�����܂ł̂����傫��
								VtxMax.x = Vtx.x;  // �ő�l�X�V
							}
							if (Vtx.y > VtxMax.y)
							{// ���o����Y���W�̍ő�l�����܂ł̂����傫��
								VtxMax.y = Vtx.y;  // �ő�l�X�V
							}
							if (Vtx.z > VtxMax.z)
							{// ���o����Z���W�̍ő�l�����܂ł̂����傫��
								VtxMax.z = Vtx.z;  // �ő�l�X�V
							}

							pVtxBuff += sizeFVF;   // �T�C�Y���|�C���^��i�߂�
						}
						// ���_�o�b�t�@���A�����b�N
						pMesh->UnlockVertexBuffer();

						// ���f���Ǌ��N���X�ւ̊��蓖��
						m_pModelManager->SetMesh(pMesh, nCntModelManager);        // ���b�V�����
						m_pModelManager->SetBuffMat(pBuffMat, nCntModelManager);  // �}�e���A�����
						m_pModelManager->SetNumMat(nNumMat, nCntModelManager);    // �}�e���A���̐����
						m_pModelManager->SetVtxMax(VtxMax, nCntModelManager);     // ���_�̍ő�l���
						m_pModelManager->SetVtxMin(VtxMin, nCntModelManager);     // ���_�̍ŏ��l���

						nCntModelManager++;    // ���f���ԍ���i�߂Ă���
					}
				}

				// �z�u��
				if (strcmp(&aString[0], OBJECTSET) == 0)
				{// �z�u���Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_OBJECTSET) != 0)
					{// �z�u���Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], TYPE) == 0)
						{// �z�u����I�u�W�F�N�g�̔ԍ���ǂݎ��
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// ���W��ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// ������ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// ��]����X�s�[�h�ʂ�ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// �����蔻�����邩�ǂ�����ǂݎ��
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
						if (strcmp(&aString[0], PUSH) == 0)
						{// �����邩�ǂ�����ǂݎ��
							int nPush;
							fscanf(pFile, "%s %d", &aString[0], &nPush);
							if (nPush == 0)
							{
								bPush = false;
							}
							else
							{
								bPush = true;
							}
						}
						if (strcmp(&aString[0], BREAK) == 0)
						{// �󂹂邩�ǂ�����ǂݎ��
							int nBreak;
							fscanf(pFile, "%s %d", &aString[0], &nBreak);
							if (nBreak == 0)
							{
								bBreak = false;
							}
							else
							{
								bBreak = true;
							}
						}
					}
					pObject = CObject::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, bPush, bBreak);
					if (pObject != NULL)
					{// ���������m�ۂł���
					 // ���f���̊��蓖��
						pObject->BindModel(CManager::GetRenderer()->GetDevice(),
							m_pModelManager->GetMesh(nObjectIndex),
							m_pModelManager->GetBuffMat(nObjectIndex),
							m_pModelManager->GetNumMat(nObjectIndex));

						// ���_�̍ŏ��l�A�ő�l���̐ݒ�
						pObject->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObject->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObject++;  // �z�u����z�u��������i�߂�

									   // �ϐ������������Ă���
						bCollision = true;
						bBreak = false;
						bPush = false;
					}
				}

				// �����z�u��
				if (strcmp(&aString[0], OBJECTMOVESET) == 0)
				{// �����z�u���Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_OBJECTMOVESET) != 0)
					{// �����z�u���Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], TYPE) == 0)
						{// �z�u����I�u�W�F�N�g�̔ԍ���ǂݎ��
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// ���W��ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// ������ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], MOVE) == 0)
						{// �ړ��ʂ�ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectMove.x, &ObjectMove.y, &ObjectMove.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// ��]����X�s�[�h�ʂ�ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// �����蔻�����邩�ǂ�����ǂݎ��
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
					}
					pObjectMove = CObjectMove::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, ObjectMove);
					if (pObjectMove != NULL)
					{// ���������m�ۂł���
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// ���f���̊��蓖��
						pObjectMove->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// ���_�̍ŏ��l�A�ő�l���̐ݒ�
						pObjectMove->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectMove->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectMove++;  // �����z�u����z�u��������i�߂�

										   // �ϐ������������Ă���
						bCollision = true;
					}
				}

				// �����o���p
				if (strcmp(&aString[0], OBJECTBALLOONSET) == 0)
				{// �����o���p�z�u���Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_OBJECTBALLOONSET) != 0)
					{// �����o���p�z�u���Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], TYPE) == 0)
						{// �z�u����I�u�W�F�N�g�̔ԍ���ǂݎ��
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// ���W��ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// ������ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// ��]����X�s�[�h�ʂ�ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// �����蔻�����邩�ǂ�����ǂݎ��
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
						if (strcmp(&aString[0], NEXT_MAPNAME) == 0)
						{// ���̃}�b�v�f�[�^�̃t�@�C������ǂݎ��
							fscanf(pFile, "%s %s", &aString[0], &aNextMapName[0]);
						}
					}

					pObjectBalloon = CObjectBalloon::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, (CObjectBalloon::BALLOON)nCntObjectBalloon, aNextMapName, 4);
					if (pObjectBalloon != NULL)
					{// ���������m�ۂł���
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// ���f���̊��蓖��
						pObjectBalloon->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// ���_�̍ŏ��l�A�ő�l���̐ݒ�
						pObjectBalloon->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectBalloon->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectBalloon++;  // �����o���p�z�u����z�u��������i�߂�

											  // �ϐ������������Ă���
						bCollision = true;
					}
				}

				// �S�[���p
				if (strcmp(&aString[0], OBJECTGOALSET) == 0)
				{// �S�[���p�z�u���Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_OBJECTGOALSET) != 0)
					{// �S�[���p�z�u���Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], TYPE) == 0)
						{// �z�u����I�u�W�F�N�g�̔ԍ���ǂݎ��
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// ���W��ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// ������ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// ��]����X�s�[�h�ʂ�ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// �����蔻�����邩�ǂ�����ǂݎ��
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
					}
					pObjectGoal = CObjectGoal::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, 4);
					if (pObjectGoal != NULL)
					{// ���������m�ۂł���
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// ���f���̊��蓖��
						pObjectGoal->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// ���_�̍ŏ��l�A�ő�l���̐ݒ�
						pObjectGoal->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectGoal->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectGoal++;  // �S�[���p�z�u����z�u��������i�߂�

										   // �ϐ������������Ă���
						bCollision = true;
					}
				}


				// �W�b�v���C���p
				if (strcmp(&aString[0], OBJECTTARGETSET) == 0)
				{// �W�b�v���C���p�z�u���Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_OBJECTTARGETSET) != 0)
					{// �W�b�v���C���p�z�u���Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], TYPE) == 0)
						{// �z�u����I�u�W�F�N�g�̔ԍ���ǂݎ��
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// ���W��ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// ������ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// ��]����X�s�[�h�ʂ�ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], POS_TARGET) == 0)
						{// �v���C���[���^�Ԉʒu��ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPosTarget.x, &ObjectPosTarget.y, &ObjectPosTarget.z);
						}
						if (strcmp(&aString[0], CARRY_TIME) == 0)
						{// �v���C���[���^�Ԃ̂ɂ����鎞�Ԃ�ǂݎ��
							fscanf(pFile, "%s %d", &aString[0], &nMaxCarryTime);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// �����蔻�����邩�ǂ�����ǂݎ��
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
					}
					pObjectTarget = CObjectTarget::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, ObjectPosTarget, nMaxCarryTime);
					if (pObjectTarget != NULL)
					{// ���������m�ۂł���
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// ���f���̊��蓖��
						pObjectTarget->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// ���_�̍ŏ��l�A�ő�l���̐ݒ�
						pObjectTarget->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectTarget->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectTarget++;  // �W�b�v���C���p�z�u����z�u��������i�߂�

											 // �ϐ������������Ă���
						bCollision = true;
					}
				}
			}

			// �I�u�W�F�N�g�̃t�@�C�������R�s�[���Ă���
			strcpy(m_aObjectFileName, aObjectFileName);

			// �t�@�C�������
			fclose(pFile);
		}
	}
}

//=============================================================================
//    �R���ǂݍ��ݏ���
//=============================================================================
void CMap::MountainLoad(char *aMoutainFileName)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	char aString[256];   // �e�L�X�g�i�[�p������
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(aMoutainFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �R�p�̊i�[�ϐ�
			D3DXVECTOR3 MountainPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �R�̍��W
			D3DXVECTOR3 MountainRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �R�̌���
			float MountainRadius;                                     // �R�̔��a
			float MountainHeight;                                     // �R�̍���
			int MountainXblock = 0;                                   // �R�̉��̃u���b�N��
			int MountainYblock = 0;                                   // �R�̏c�̃u���b�N��

			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// �e�L�X�g���Ō�̍s�ɓ��B����܂œǂݍ��݂𑱂���
				if (strcmp(&aString[0], MOUNTAINSET) == 0)
				{// �R�Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_MOUNTAINSET) != 0)
					{// �R�Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], POS) == 0)
						{// �R�̍��W��񂪂���
							fscanf(pFile, "%s %f %f %f", &aString[0], &MountainPos.x, &MountainPos.y, &MountainPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// �R�̌�����񂪂���
							fscanf(pFile, "%s %f %f %f", &aString[0], &MountainRot.x, &MountainRot.y, &MountainRot.z);
						}
						if (strcmp(&aString[0], RADIUS) == 0)
						{// �R�̔��a��񂪂���
							fscanf(pFile, "%s %f", &aString[0], &MountainRadius);
						}
						if (strcmp(&aString[0], HEIGHT) == 0)
						{// �R�̍�����񂪂���
							fscanf(pFile, "%s %f", &aString[0], &MountainHeight);
						}
						if (strcmp(&aString[0], XBLOCK) == 0)
						{// �R�̉��̕�������񂪂���
							fscanf(pFile, "%s %d", &aString[0], &MountainXblock);
						}
						if (strcmp(&aString[0], YBLOCK) == 0)
						{// �R�̏c�̕������̉��s��񂪂���
							fscanf(pFile, "%s %d", &aString[0], &MountainYblock);
						}
					}
					// �R�̐���
					if (m_pMoutain == NULL)
					{// ���������m�ۂł����Ԃɂ���
						m_pMoutain = CCylinder::Create(MountainPos, MountainRot, MountainRadius, MountainHeight, MountainXblock, MountainYblock);
					}
				}
			}
			// �t�@�C�������
			fclose(pFile);
		}
	}
}

//=============================================================================
//    ����ǂݍ��ݏ���
//=============================================================================
void CMap::SkyLoad(char *aSkyFileName)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	char aString[256];   // �e�L�X�g�i�[�p������
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(aSkyFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // ��p�̊i�[�ϐ�
			D3DXVECTOR3 SkyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ��̍��W
			D3DXVECTOR3 SkyRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ��̌���
			float SkyRadius;                                     // ��̔��a
			int SkyXblock = 0;                                   // ��̉��̃u���b�N��
			int SkyYblock = 0;                                   // ��̏c�̃u���b�N��

			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// �e�L�X�g���Ō�̍s�ɓ��B����܂œǂݍ��݂𑱂���
				if (strcmp(&aString[0], SKYSET) == 0)
				{// ��Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_SKYSET) != 0)
					{// ��Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], POS) == 0)
						{// ��̍��W��񂪂���
							fscanf(pFile, "%s %f %f %f", &aString[0], &SkyPos.x, &SkyPos.y, &SkyPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// ��̌�����񂪂���
							fscanf(pFile, "%s %f %f %f", &aString[0], &SkyRot.x, &SkyRot.y, &SkyRot.z);
						}
						if (strcmp(&aString[0], RADIUS) == 0)
						{// ��̔��a��񂪂���
							fscanf(pFile, "%s %f", &aString[0], &SkyRadius);
						}
						if (strcmp(&aString[0], XBLOCK) == 0)
						{// ��̉��̕�������񂪂���
							fscanf(pFile, "%s %d", &aString[0], &SkyXblock);
						}
						if (strcmp(&aString[0], YBLOCK) == 0)
						{// ��̏c�̕������̉��s��񂪂���
							fscanf(pFile, "%s %d", &aString[0], &SkyYblock);
						}
					}
					// ��̐���
					if (m_pSky == NULL)
					{// ���������m�ۂł����Ԃɂ���
						m_pSky = CDome::Create(SkyPos, SkyRot, SkyRadius, SkyXblock, SkyYblock);
					}
				}
			}
			// �t�@�C�������
			fclose(pFile);
		}
	}
}

//=============================================================================
//    �z�u�����f�t�H���g�̈ʒu�ɖ߂�����
//=============================================================================
void CMap::SetObjectDefault(void)
{
	// �z�u���̔j��
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetScene(nCntPriority);
		while (pScene != NULL)
		{// �|�C���^����ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
			{// �z�u���̃N���X��������
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTMOVE)
			{// �����z�u���̃N���X��������
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTBALLOON)
			{// �����o���p�̔z�u���̃N���X��������
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTGOAL)
			{// �S�[���p�z�u���̃N���X��������
				pScene->Uninit();
				pScene = NULL;
			}
			else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTTARGET)
			{// �W�b�v���C���p�z�u���̃N���X��������
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}

	FILE *pFile = NULL;  // �t�@�C���|�C���^
	char aString[256];   // �e�L�X�g�i�[�p������
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(m_aObjectFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �z�u���p�̊i�[�ϐ�
			CObject *pObject = NULL;
			CObjectMove *pObjectMove = NULL;
			CObjectBalloon *pObjectBalloon = NULL;
			CObjectGoal *pObjectGoal = NULL;
			CObjectTarget *pObjectTarget = NULL;
			LPD3DXMESH pMesh = NULL;         // ���b�V�����i�[�p
			LPD3DXBUFFER pBuffMat = NULL;    // �}�e���A�����i�[�p
			DWORD nNumMat = NULL;            // �}�e���A���̐����i�[�p
			D3DXVECTOR3 VtxMax;              // ���_�̍ő�l���i�[�p
			D3DXVECTOR3 VtxMin;              // ���_�̍ŏ��l���i�[�p
			int nCntModelManager = 0;        // �Ǌ��N���X�֊��蓖�Ă�ۂ̃��f���ԍ�
			int nObjectIndex = 0;            // ���蓖�Ă郂�f���̔ԍ�
			D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �z�u���̍��W
			D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �z�u���̌���
			D3DXVECTOR3 ObjectMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // �z�u���̈ړ���
			D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �z�u���̉�]����X�s�[�h��
			D3DXVECTOR3 ObjectPosTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �z�u���̃v���C���[���^�Ԉʒu
			bool bCollision = true;                                        // �����蔻�����邩�ǂ���
			bool bPush = false;                                            // �z�u���������邩�ǂ���
			bool bBreak = false;                                           // �z�u�����󂹂邩�ǂ���
			int nMaxCarryTime = 0;                                         // �v���C���[���^�Ԃ̂ɂ����鎞��
			int nCntObject = 0;                                            // �z�u����u�������𐔂���ϐ�
			int nCntObjectMove = 0;                                        // �����z�u����u�������𐔂���ϐ�
			int nCntObjectBalloon = 0;                                     // �����o���p�z�u����u�������𐔂���ϐ�
			int nCntObjectGoal = 0;                                        // �S�[���p�z�u����u�������𐔂���ϐ�
			int nCntObjectTarget = 0;                                      // �W�b�v���C���p�z�u����u�������𐔂���ϐ�
			char aNextMapName[256];                                        // ���̃}�b�v�f�[�^�̃t�@�C����

			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// �e�L�X�g���Ō�̍s�ɓ��B����܂œǂݍ��݂𑱂���
				// �z�u��
				if (strcmp(&aString[0], OBJECTSET) == 0)
				{// �z�u���Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_OBJECTSET) != 0)
					{// �z�u���Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], TYPE) == 0)
						{// �z�u����I�u�W�F�N�g�̔ԍ���ǂݎ��
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// ���W��ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// ������ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// ��]����X�s�[�h�ʂ�ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// �����蔻�����邩�ǂ�����ǂݎ��
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
						if (strcmp(&aString[0], PUSH) == 0)
						{// �����邩�ǂ�����ǂݎ��
							int nPush;
							fscanf(pFile, "%s %d", &aString[0], &nPush);
							if (nPush == 0)
							{
								bPush = false;
							}
							else
							{
								bPush = true;
							}
						}
						if (strcmp(&aString[0], BREAK) == 0)
						{// �󂹂邩�ǂ�����ǂݎ��
							int nBreak;
							fscanf(pFile, "%s %d", &aString[0], &nBreak);
							if (nBreak == 0)
							{
								bBreak = false;
							}
							else
							{
								bBreak = true;
							}
						}
					}
					pObject = CObject::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, bPush, bBreak);
					if (pObject != NULL)
					{// ���������m�ۂł���
						// ���f���̊��蓖��
						pObject->BindModel(CManager::GetRenderer()->GetDevice(),
							m_pModelManager->GetMesh(nObjectIndex),
							m_pModelManager->GetBuffMat(nObjectIndex),
							m_pModelManager->GetNumMat(nObjectIndex));

						// ���_�̍ŏ��l�A�ő�l���̐ݒ�
						pObject->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObject->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObject++;  // �z�u����z�u��������i�߂�

						// �ϐ������������Ă���
						bCollision = true;
						bBreak = false;
						bPush = false;
					}
				}

				// �����z�u��
				if (strcmp(&aString[0], OBJECTMOVESET) == 0)
				{// �����z�u���Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_OBJECTMOVESET) != 0)
					{// �����z�u���Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], TYPE) == 0)
						{// �z�u����I�u�W�F�N�g�̔ԍ���ǂݎ��
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// ���W��ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// ������ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], MOVE) == 0)
						{// �ړ��ʂ�ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectMove.x, &ObjectMove.y, &ObjectMove.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// ��]����X�s�[�h�ʂ�ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// �����蔻�����邩�ǂ�����ǂݎ��
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
					}

					pObjectMove = CObjectMove::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, ObjectMove);
					if (pObjectMove != NULL)
					{// ���������m�ۂł���
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// ���f���̊��蓖��
						pObjectMove->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// ���_�̍ŏ��l�A�ő�l���̐ݒ�
						pObjectMove->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectMove->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectMove++;  // �����z�u����z�u��������i�߂�

						// �ϐ������������Ă���
						bCollision = true;
					}
				}

				// �����o���p
				if (strcmp(&aString[0], OBJECTBALLOONSET) == 0)
				{// �����o���p�z�u���Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_OBJECTBALLOONSET) != 0)
					{// �����o���p�z�u���Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], TYPE) == 0)
						{// �z�u����I�u�W�F�N�g�̔ԍ���ǂݎ��
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// ���W��ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// ������ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// ��]����X�s�[�h�ʂ�ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// �����蔻�����邩�ǂ�����ǂݎ��
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
						if (strcmp(&aString[0], NEXT_MAPNAME) == 0)
						{// ���̃}�b�v�f�[�^�̃t�@�C������ǂݎ��
							fscanf(pFile, "%s %s", &aString[0], &aNextMapName[0]);
						}
					}

					pObjectBalloon = CObjectBalloon::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, (CObjectBalloon::BALLOON)nCntObjectBalloon, aNextMapName, 4);
					if (pObjectBalloon != NULL)
					{// ���������m�ۂł���
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// ���f���̊��蓖��
						pObjectBalloon->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// ���_�̍ŏ��l�A�ő�l���̐ݒ�
						pObjectBalloon->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectBalloon->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectBalloon++;  // �����o���p�z�u����z�u��������i�߂�

						// �ϐ������������Ă���
						bCollision = true;
					}
				}

				// �S�[���p
				if (strcmp(&aString[0], OBJECTGOALSET) == 0)
				{// �S�[���p�z�u���Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_OBJECTGOALSET) != 0)
					{// �S�[���p�z�u���Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], TYPE) == 0)
						{// �z�u����I�u�W�F�N�g�̔ԍ���ǂݎ��
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// ���W��ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// ������ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// ��]����X�s�[�h�ʂ�ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// �����蔻�����邩�ǂ�����ǂݎ��
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
					}
					pObjectGoal = CObjectGoal::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, 4);
					if (pObjectGoal != NULL)
					{// ���������m�ۂł���
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// ���f���̊��蓖��
						pObjectGoal->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// ���_�̍ŏ��l�A�ő�l���̐ݒ�
						pObjectGoal->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectGoal->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectGoal++;  // �S�[���p�z�u����z�u��������i�߂�

						// �ϐ������������Ă���
						bCollision = true;
					}
				}


				// �W�b�v���C���p
				if (strcmp(&aString[0], OBJECTTARGETSET) == 0)
				{// �W�b�v���C���p�z�u���Z�b�g�ǂݍ��݊J�n�̍��}������
					while (strcmp(&aString[0], END_OBJECTTARGETSET) != 0)
					{// �W�b�v���C���p�z�u���Z�b�g��ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g��ǂݐi�߂�
						if (strcmp(&aString[0], TYPE) == 0)
						{// �z�u����I�u�W�F�N�g�̔ԍ���ǂݎ��
							fscanf(pFile, "%s %d", &aString[0], &nObjectIndex);
						}
						if (strcmp(&aString[0], POS) == 0)
						{// ���W��ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPos.x, &ObjectPos.y, &ObjectPos.z);
						}
						if (strcmp(&aString[0], ROT) == 0)
						{// ������ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRot.x, &ObjectRot.y, &ObjectRot.z);
						}
						if (strcmp(&aString[0], ROLL_SPEED) == 0)
						{// ��]����X�s�[�h�ʂ�ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectRollSpeed.x, &ObjectRollSpeed.y, &ObjectRollSpeed.z);
						}
						if (strcmp(&aString[0], POS_TARGET) == 0)
						{// �v���C���[���^�Ԉʒu��ǂݎ��
							fscanf(pFile, "%s %f %f %f", &aString[0], &ObjectPosTarget.x, &ObjectPosTarget.y, &ObjectPosTarget.z);
						}
						if (strcmp(&aString[0], CARRY_TIME) == 0)
						{// �v���C���[���^�Ԃ̂ɂ����鎞�Ԃ�ǂݎ��
							fscanf(pFile, "%s %d", &aString[0], &nMaxCarryTime);
						}
						if (strcmp(&aString[0], COLLISION) == 0)
						{// �����蔻�����邩�ǂ�����ǂݎ��
							int nCollision;
							fscanf(pFile, "%s %d", &aString[0], &nCollision);
							if (nCollision == 0)
							{
								bCollision = false;
							}
							else
							{
								bCollision = true;
							}
						}
					}
					pObjectTarget = CObjectTarget::Create(ObjectPos, ObjectRot, ObjectRollSpeed, bCollision, ObjectPosTarget, nMaxCarryTime);
					if (pObjectTarget != NULL)
					{// ���������m�ۂł���
						pMesh = m_pModelManager->GetMesh(nObjectIndex);
						pBuffMat = m_pModelManager->GetBuffMat(nObjectIndex);
						nNumMat = m_pModelManager->GetNumMat(nObjectIndex);

						// ���f���̊��蓖��
						pObjectTarget->BindModel(CManager::GetRenderer()->GetDevice(), pMesh, pBuffMat, nNumMat);

						// ���_�̍ŏ��l�A�ő�l���̐ݒ�
						pObjectTarget->SetVtxMax(m_pModelManager->GetVtxMax(nObjectIndex));
						pObjectTarget->SetVtxMin(m_pModelManager->GetVtxMin(nObjectIndex));

						nCntObjectTarget++;  // �W�b�v���C���p�z�u����z�u��������i�߂�

						// �ϐ������������Ă���
						bCollision = true;
					}
				}
			}
			// �t�@�C�������
			fclose(pFile);
		}
	}
}

//=============================================================================
//    �v���C���[�̏������W���擾���鏈��
//=============================================================================
D3DXVECTOR3 CMap::GetDefaultPlayerPos(void)
{
	return m_DefaultPlayerPos;
}

//=============================================================================
//    �v���C���[�̏����������擾���鏈��
//=============================================================================
D3DXVECTOR3 CMap::GetDefaultPlayerRot(void)
{
	return m_DefaultPlayerRot;
}

//=============================================================================
//    �J�����̏����������擾���鏈��
//=============================================================================
D3DXVECTOR3 CMap::GetDefaultCameraRot(void)
{
	return m_DefaultCameraRot;
}

//=============================================================================
//    �ǂݍ��񂾃}�b�v�̃t�@�C������Ԃ�����
//=============================================================================
char *CMap::GetMapName(void)
{
	return m_aFileName;
}