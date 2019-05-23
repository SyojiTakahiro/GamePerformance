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
#include "sky.h"
#include "mountain.h"
#include "object.h"
#include "meshField.h"
#include "modelManager.h"
#include "textureManager.h"
#include "functionlib.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MAP_OBJECT_SAVEFILENAME "data/TEXT/MAP/OBJECT/save_object.txt"  // �z�u������ۑ�����e�L�X�g�t�@�C����

// �X�N���v�g�t�@�C���̓ǂݍ��ݗp�̃p�X��
#define MAPSET               "MAPSET"                // �}�b�v�Z�b�g�J�n�̍��}
#define END_MAPSET           "END_MAPSET"            // �}�b�v�Z�b�g�I���̍��}
#define FIELDSET             "FIELDSET"              // �n�ʃZ�b�g�J�n�̍��}
#define END_FIELDSET         "END_FIELDSET"          // �n�ʃZ�b�g�I���̍��}
#define OBJECTSET            "OBJECTSET"             // �z�u���Z�b�g�J�n�̍��}
#define END_OBJECTSET        "END_OBJECTSET"         // �z�u���Z�b�g�I���̍��}
#define OBJECTMOVESET        "OBJECTMOVESET"         // �����z�u���Z�b�g�J�n�̍��}
#define END_OBJECTMOVESET    "END_OBJECTMOVESET"     // �����z�u���Z�b�g�I���̍��}
#define MOUNTAINSET          "MOUNTAINSET"           // �R�Z�b�g�J�n�̍��}
#define END_MOUNTAINSET      "END_MOUNTAINSET"       // �R�Z�b�g�I���̍��}
#define SKYSET               "SKYSET"                // ��Z�b�g�J�n�̍��}
#define END_SKYSET           "END_SKYSET"            // ��Z�b�g�I���̍��}

#define FIELD_FILENAME       "FIELD_FILENAME = "     // �n�ʂ̃X�N���v�g�t�@�C����
#define OBJECT_FILENAME      "OBJECT_FILENAME = "    // �z�u���̃X�N���v�g�t�@�C����
#define MOUNTAIN_FILENAME    "MOUNTAIN_FILENAME = "  // �R�̃X�N���v�g�t�@�C����
#define SKY_FILENAME         "SKY_FILENAME = "       // ��̃X�N���v�g�t�@�C����

#define NUM_TEXTURE          "NUM_TEXTURE = "        // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME     "TEXTURE_FILENAME = "   // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define NUM_MODEL            "NUM_MODEL = "          // �ǂݍ��ރ��f���̐�
#define MODEL_FILENAME       "MODEL_FILENAME = "     // �ǂݍ��ރ��f���t�@�C����

#define TEX_TYPE             "TEX_TYPE = "           // �e�N�X�`���̔ԍ����
#define TYPE                 "TYPE = "               // ��ޏ��
#define POS                  "POS = "                // ���W���
#define POS_TARGET           "POS_TARGET = "         // �ړI�̈ʒu
#define ROT                  "ROT = "                // �������
#define COL                  "COL = "                // �F���
#define MOVE                 "MOVE = "               // �ړ��ʏ��
#define MOVE_MAX             "MOVE_MAX = "           // �ړ��ʂ�؂�ւ��鎞�ԏ��
#define PUSH                 "PUSH = "               // �����邩�ǂ���
#define BREAK                "BREAK = "              // �󂹂邩�ǂ���
#define ROLL_SPEED           "ROLL_SPEED = "         // ��]����X�s�[�h�ʏ��
#define CARRY_TIME           "CARRY_TIME = "         // �v���C���[���^�Ԃ̂ɂ����鎞��
#define COLLISION            "COLLISION = "          // �����蔻������邩�ǂ���
#define WIDTH                "WIDTH = "              // 1�u���b�N���̕����
#define DEPTH                "DEPTH = "              // 1�u���b�N�����s���
#define RADIUS               "RADIUS = "             // ���a���
#define HEIGHT               "HEIGHT = "             // �������
#define XBLOCK               "XBLOCK = "             // ���̕��������
#define YBLOCK               "YBLOCK = "             // �c�̕��������
#define ZBLOCK               "ZBLOCK = "             // ���s�̕��������
#define BINARY_FILENAME      "BINARY_FILENAME = "    // ���_��񂪊i�[���ꂽ�o�C�i���t�@�C����
#define NEXT_MAPNAME         "NEXT_MAPNAME = "       // ���̃}�b�v�f�[�^�̃t�@�C����

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMap::CMap()
{
	// �e��l�̏�����
	m_pModelManager = NULL;    // ���f���Ǌ��N���X�ւ̃|�C���^
	m_pTextureManager = NULL;  // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_pField = NULL;           // �n�ʃN���X�ւ̃|�C���^
	m_pMoutain = NULL;         // �~���N���X�ւ̃|�C���^
	m_pSky = NULL;             // ��N���X�ւ̃|�C���^
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
	// �}�b�v�̊O���t�@�C������ݒ�
	strcpy(m_aFileName, aFileName);

	// �}�b�v�����O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		 // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                         // 1�s���ǂݎ��p
			char *pStrCur = NULL;                       // ���݂̐擪�|�C���^
			char *pStr = NULL;                          // �擪�|�C���^�ۑ��p
			int nNumTex = 0;                            // �e�N�X�`����ǂݍ��ސ�
			int nNumModel = 0;                          // ���f����ǂݍ��ސ�
			int nCntTex = 0;                            // �e�N�X�`����ǂݍ��񂾉�
			int nCntModel = 0;                          // ���f����ǂݍ��񂾉�
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // �e�N�X�`���ǂݎ��p
			LPD3DXMESH pMesh = NULL;                    // ���b�V�����ւ̃|�C���^
			LPD3DXBUFFER pBuffMat = NULL;               // �}�e���A�����ւ̃|�C���^
			DWORD nNumMat = 0;                          // �}�e���A�����̐�
			char aTexFileName[256] = "\0";              // �e�N�X�`���̃t�@�C����

			if (pLine == NULL && pStr == NULL)
			{// ���������m�ۂł����Ԃł���
				pLine = new char[256];
				pStr = new char[256];
				if (pLine != NULL && pStr != NULL)
				{// ���������m�ۂł���
				 // �������������
					strcpy(pLine, "\0");
					strcpy(pStr, "\0");

					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// �e�N�X�`���̌���񂪂�����
								nNumTex = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTex >= 1)
								{// ���f������1�ȏ゠��
									m_pTextureManager = CTextureManager::Create(nNumTex);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// �e�N�X�`���̃t�@�C���p�X����񂪂�����
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
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
							{// �e�N�X�`���̌���񂪂�����
								nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
								if (nNumModel >= 1)
								{// ���f������1�ȏ゠��
									m_pModelManager = CModelManager::Create(nNumModel);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
							{// ���f���̃t�@�C���p�X����񂪂�����
							 // ���f���̃t�@�C���p�X����ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MODEL_FILENAME);

								// ���f���̓ǂݍ���
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
							else if (CFunctionLib::Memcmp(pStrCur, FIELD_FILENAME) == 0)
							{// �n�ʂ̃X�N���v�g�t�@�C������������
							 // �n�ʂ̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, FIELD_FILENAME);
								FieldLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, OBJECT_FILENAME) == 0)
							{// �z�u���̃X�N���v�g�t�@�C������������
							 // �z�u���̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, OBJECT_FILENAME);
								ObjectLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOUNTAIN_FILENAME) == 0)
							{// �R�̃X�N���v�g�t�@�C������������
							 // �R�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MOUNTAIN_FILENAME);
								MountainLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, SKY_FILENAME) == 0)
							{// ��̃X�N���v�g�t�@�C������������
							    // ��̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, SKY_FILENAME);
								SkyLoad(pStr, pStrCur, pLine);
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
//    �I������
//=============================================================================
void CMap::Uninit(void)
{
	// �z�u������ۑ�����
	//SaveObject();

	// �}�b�v�Ǌ��N���X���J������
	if (m_pModelManager != NULL)
	{
		m_pModelManager->Uninit();

		// �������̊J��
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// �e�N�X�`���Ǌ��N���X���J������
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// �n�ʂ̔j��
	if (m_pField != NULL)
	{// ���������m�ۂ���Ă���
		m_pField = NULL;
	}

	// �R�̔j��
	if (m_pMoutain != NULL)
	{// ���������m�ۂ���Ă���
		m_pMoutain = NULL;
	}

	// ��̔j��
	if (m_pSky != NULL)
	{// ���������m�ۂ���Ă���
		m_pSky = NULL;
	}
}

//=============================================================================
//    �n�ʏ��ǂݍ��ݏ���
//=============================================================================
void CMap::FieldLoad(char *pFieldFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(pFieldFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		 // �n�ʗp�̊i�[�ϐ�
			int nFieldTexIdx = 0;                                    // �n�ʂ̃e�N�X�`���ԍ�
			D3DXVECTOR3 FieldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �n�ʂ̍��W
			D3DXVECTOR3 FieldRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �n�ʂ̌���
			D3DXCOLOR FieldCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // �n�ʂ̐F
			float FieldWidth = 0.0f;                                 // �n�ʂ�1�u���b�N���̕�
			float FieldDepth = 0.0f;                                 // �n�ʂ�1�u���b�N���̉��s
			int FieldXblock = 0;                                     // �n�ʂ̉��̃u���b�N��
			int FieldZblock = 0;                                     // �n�ʂ̉��s�̃u���b�N��
			char aFileName[256] = "\0";                              // �n�ʂ̒��_��񂪊i�[���ꂽ�o�C�i���t�@�C����

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
				while (1)
				{// ���[�v�J�n
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, FIELDSET) == 0)
					{// �n�ʏ��ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// �g�p����e�N�X�`���̔ԍ���񂾂���
								nFieldTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// ���W��񂾂���
								FieldPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// ������񂾂���
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								FieldRot.x = D3DXToRadian(Rot.x);
								FieldRot.y = D3DXToRadian(Rot.y);
								FieldRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// �F��񂾂���
								FieldCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
							{// 1�u���b�N���̕���񂾂���
								FieldWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, DEPTH) == 0)
							{// 1�u���b�N���̕���񂾂���
								FieldDepth = CFunctionLib::ReadFloat(pStrCur, DEPTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// ���̕�����������
								FieldXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ZBLOCK) == 0)
							{// ���s�̕�����������
								FieldZblock = CFunctionLib::ReadInt(pStrCur, ZBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, BINARY_FILENAME) == 0)
							{// ���_��񂪊i�[���ꂽ�o�C�i���t�@�C����������
								pStrCur = CFunctionLib::ReadString(pStrCur, aFileName, BINARY_FILENAME);
								strcpy(aFileName, pStrCur);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_FIELDSET) == 0)
							{// �n�ʏ��ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��ݏI���̍��}������
						break;  // ���[�v�I��
					}
				}
				// �n�ʂ𐶐�����
				m_pField = CMeshField::Create(FieldPos, FieldRot, FieldCol, FieldWidth, FieldDepth, FieldXblock, FieldZblock, aFileName);
				if (m_pField != NULL)
				{// �n�ʂ������ł���
					m_pField->BindTexture(m_pTextureManager->GetTexture(nFieldTexIdx));
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
void CMap::ObjectLoad(char *pObjectFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(pObjectFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		 // �z�u���p�̊i�[�ϐ�
			CObject *pObject = NULL;                                        // �z�u���N���X�ւ̃|�C���^
			CObjectMove *pObjectMove = NULL;                                // �����z�u���N���X�ւ̃|�C���^
			int nObjectType = 0;                                            // �z�u���̃��f���ԍ�
			D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �z�u���̍��W
			D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �z�u���̌���
			D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �z�u���̉�]�X�s�[�h
			D3DXVECTOR3 ObjectMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �z�u���̈ړ���
			int nObjectMaxCounter = 0;                                      // �z�u���̈ړ��ʂ�؂�ւ��鎞��

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
				while (1)
				{// ���[�v�J�n
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, OBJECTSET) == 0)
					{// �z�u�����ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
							{// �g�p���郂�f���ԍ���񂾂���
								nObjectType = CFunctionLib::ReadInt(pStrCur, TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// ���W��񂾂���
								ObjectPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// ������񂾂���
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								ObjectRot.x = D3DXToRadian(Rot.x);
								ObjectRot.y = D3DXToRadian(Rot.y);
								ObjectRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
							{// ��]����X�s�[�h��񂾂���
								D3DXVECTOR3 RotSpeed = CFunctionLib::ReadVector3(pStrCur, ROLL_SPEED);
								ObjectRollSpeed.x = D3DXToRadian(RotSpeed.x);
								ObjectRollSpeed.y = D3DXToRadian(RotSpeed.y);
								ObjectRollSpeed.z = D3DXToRadian(RotSpeed.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_OBJECTSET) == 0)
							{// �z�u�����ǂݍ��ݏI���̍��}������
							    // �z�u���𐶐�����
								pObject = CObject::Create(ObjectPos, ObjectRot, ObjectRollSpeed,4);
								if (pObject != NULL)
								{
									pObject->BindModel(m_pModelManager->GetMesh(nObjectType), m_pModelManager->GetBuffMat(nObjectType), m_pModelManager->GetNumMat(nObjectType), m_pModelManager->GetVtxMax(nObjectType), m_pModelManager->GetVtxMin(nObjectType), m_pModelManager->GetTexture(nObjectType));
									pObject->SetModelIdx(nObjectType);
								}
								break;  // ���[�v�I��
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, OBJECTMOVESET) == 0)
					{// �����z�u�����ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
							{// �g�p���郂�f���ԍ���񂾂���
								nObjectType = CFunctionLib::ReadInt(pStrCur, TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// ���W��񂾂���
								ObjectPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// ������񂾂���
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								ObjectRot.x = D3DXToRadian(Rot.x);
								ObjectRot.y = D3DXToRadian(Rot.y);
								ObjectRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
							{// ��]����X�s�[�h��񂾂���
								D3DXVECTOR3 RotSpeed = CFunctionLib::ReadVector3(pStrCur, ROLL_SPEED);
								ObjectRollSpeed.x = D3DXToRadian(RotSpeed.x);
								ObjectRollSpeed.y = D3DXToRadian(RotSpeed.y);
								ObjectRollSpeed.z = D3DXToRadian(RotSpeed.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOVE) == 0)
							{// �ړ��ʏ�񂾂���
								ObjectMove = CFunctionLib::ReadVector3(pStrCur, MOVE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOVE_MAX) == 0)
							{// �ړ��ʂ�؂�ւ��鎞�ԏ�񂾂���
								nObjectMaxCounter = CFunctionLib::ReadInt(pStrCur, MOVE_MAX);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_OBJECTMOVESET) == 0)
							{// �����z�u�����ǂݍ��ݏI���̍��}������
							    // �����z�u���𐶐�����
								pObjectMove = CObjectMove::Create(ObjectPos, ObjectRot, ObjectRollSpeed, ObjectMove, nObjectMaxCounter);
								if (pObjectMove != NULL)
								{
									pObjectMove->BindModel(m_pModelManager->GetMesh(nObjectType), m_pModelManager->GetBuffMat(nObjectType), m_pModelManager->GetNumMat(nObjectType), m_pModelManager->GetVtxMax(nObjectType), m_pModelManager->GetVtxMin(nObjectType), m_pModelManager->GetTexture(nObjectType));
									pObjectMove->SetModelIdx(nObjectType);
								}
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
			// �t�@�C�������
			fclose(pFile);
		}
	}
}

//=============================================================================
//    �R���ǂݍ��ݏ���
//=============================================================================
void CMap::MountainLoad(char *pMoutainFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(pMoutainFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		 // �R�p�̊i�[�ϐ�
			int nMountainTexIdx = 0;                                    // �R�̃e�N�X�`���ԍ�
			D3DXVECTOR3 MountainPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �R�̍��W
			D3DXVECTOR3 MountainRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �R�̌���
			D3DXCOLOR MountainCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // �R�̐F
			float MountainRadius = 0.0f;                                // �R�̔��a
			float MountainHeight = 0.0f;                                // �R�̍���
			int MountainXblock = 0;                                     // �R�̉��̃u���b�N��
			int MountainYblock = 0;                                     // �R�̏c�̃u���b�N��

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
				while (1)
				{// ���[�v�J�n
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, MOUNTAINSET) == 0)
					{// �R���ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// �g�p����e�N�X�`���̔ԍ���񂾂���
								nMountainTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// ���W��񂾂���
								MountainPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// ������񂾂���
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								MountainRot.x = D3DXToRadian(Rot.x);
								MountainRot.y = D3DXToRadian(Rot.y);
								MountainRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// �F��񂾂���
								MountainCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, RADIUS) == 0)
							{// �R�̔��a��񂾂���
								MountainRadius = CFunctionLib::ReadFloat(pStrCur, RADIUS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
							{// �R�̍�����񂾂���
								MountainHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// ���̕�����������
								MountainXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
							{// �c�̕�����������
								MountainYblock = CFunctionLib::ReadInt(pStrCur, YBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_MOUNTAINSET) == 0)
							{// �R���ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��ݏI���̍��}������
						break;  // ���[�v�I��
					}
				}
				// �R�𐶐�����
				m_pMoutain = CMountain::Create(MountainPos, MountainRot, MountainCol, MountainHeight, MountainRadius, MountainXblock, MountainYblock);
				if (m_pMoutain != NULL)
				{// �R�������ł���
					m_pMoutain->BindTexture(m_pTextureManager->GetTexture(nMountainTexIdx));
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
void CMap::SkyLoad(char *pSkyFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(pSkyFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		 // ��p�̊i�[�ϐ�
			int nSkyTexIdx = 0;                                    // ��̃e�N�X�`���ԍ�
			D3DXVECTOR3 SkyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ��̍��W
			D3DXVECTOR3 SkyRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ��̌���
			D3DXCOLOR SkyCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // ��̐F
			float SkyRadius = 0.0f;                                // ��̔��a
			int SkyXblock = 0;                                     // ��̉��̃u���b�N��
			int SkyYblock = 0;                                     // ��̏c�̃u���b�N��
			float fRotSpeed = 0.0f;                                // ��̉�]����X�s�[�h

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
				while (1)
				{// ���[�v�J�n
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, SKYSET) == 0)
					{// ����ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// �g�p����e�N�X�`���̔ԍ���񂾂���
								nSkyTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// ���W��񂾂���
								SkyPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// ������񂾂���
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								SkyRot.x = D3DXToRadian(Rot.x);
								SkyRot.y = D3DXToRadian(Rot.y);
								SkyRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// �F��񂾂���
								SkyCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, RADIUS) == 0)
							{// ��̔��a��񂾂���
								SkyRadius = CFunctionLib::ReadFloat(pStrCur, RADIUS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// ���̕�����������
								SkyXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
							{// �c�̕�����������
								SkyYblock = CFunctionLib::ReadInt(pStrCur, YBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
							{// �c�̕�����������
								fRotSpeed = CFunctionLib::ReadFloat(pStrCur, ROLL_SPEED);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SKYSET) == 0)
							{// ����ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��ݏI���̍��}������
						break;  // ���[�v�I��
					}
				}
				// ��𐶐�����
				m_pSky = CSky::Create(SkyPos, SkyRot, SkyCol, SkyRadius, SkyXblock, SkyYblock, fRotSpeed);
				if (m_pSky != NULL)
				{// �󂪐����ł���
					m_pSky->BindTexture(m_pTextureManager->GetTexture(nSkyTexIdx));
				}
			}
			// �t�@�C�������
			fclose(pFile);
		}
	}
}

//=============================================================================
//    ���f������ۑ����鏈��
//=============================================================================
void CMap::SaveObject(void)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(MAP_OBJECT_SAVEFILENAME, "w");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		 // �R�����g��������������
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#       �z�u�����X�N���v�g�t�@�C��[save_object.txt]\n");
			fprintf(pFile, "#       Auther : Hodaka Niwa\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "#  ���f�����\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// �I�u�W�F�N�g����ۑ�
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			CObject *pObject = NULL;
			CObjectMove *pObjectMove = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// �D�揇�ʂ̐������J��Ԃ�
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// ����������ɂȂ�܂ŌJ��Ԃ�
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
					{// �z�u���N���X�ւ̃|�C���^������
						pObject = (CObject*)pScene;
						if (pObject != NULL)
						{// �I�u�W�F�N�g�ւ̃|�C���^���擾�ł���
						 // ���O�Ɍv�Z�����܂��Ă���
							D3DXVECTOR3 ModelRot = pObject->GetRotStd();
							D3DXVECTOR3 ModelRollSpeed = pObject->GetRollSpeed();
							// ����
							ModelRot.x = D3DXToDegree(ModelRot.x);
							ModelRot.y = D3DXToDegree(ModelRot.y);
							ModelRot.z = D3DXToDegree(ModelRot.z);

							// ��]����X�s�[�h
							ModelRollSpeed.x = D3DXToDegree(ModelRollSpeed.x);
							ModelRollSpeed.y = D3DXToDegree(ModelRollSpeed.y);
							ModelRollSpeed.z = D3DXToDegree(ModelRollSpeed.z);

							// �z�u���̏�����������
							fprintf(pFile, "%s\n", OBJECTSET);
							fprintf(pFile, "	%s%d						# �g�p���郂�f���̔ԍ�\n", TYPE, pObject->GetModelIdx());
							fprintf(pFile, "	%s%.1f %.1f %.1f			# ���f���̍��W\n", POS, pObject->GetPos().x, pObject->GetPos().y, pObject->GetPos().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f				# ���f���̌���\n", ROT, ModelRot.x, ModelRot.y, ModelRot.z);
							fprintf(pFile, "	%s%.1f %.1f %.1f		# ���f���̉�]����X�s�[�h\n", ROLL_SPEED, ModelRollSpeed.x, ModelRollSpeed.y, ModelRollSpeed.z);
							fprintf(pFile, "%s\n", END_OBJECTSET);
							fprintf(pFile, "\n");
						}
					}
					else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTMOVE)
					{// �����z�u���N���X�ւ̃|�C���^������
						pObjectMove = (CObjectMove*)pScene;
						if (pObjectMove != NULL)
						{// �I�u�W�F�N�g�ւ̃|�C���^���擾�ł���
						 // ���O�Ɍv�Z�����܂��Ă���
							D3DXVECTOR3 ModelRot = pObjectMove->GetRotStd();
							D3DXVECTOR3 ModelRollSpeed = pObjectMove->GetRollSpeed();
							// ����
							ModelRot.x = D3DXToDegree(ModelRot.x);
							ModelRot.y = D3DXToDegree(ModelRot.y);
							ModelRot.z = D3DXToDegree(ModelRot.z);

							// ��]����X�s�[�h
							ModelRollSpeed.x = D3DXToDegree(ModelRollSpeed.x);
							ModelRollSpeed.y = D3DXToDegree(ModelRollSpeed.y);
							ModelRollSpeed.z = D3DXToDegree(ModelRollSpeed.z);

							// �z�u���̏�����������
							fprintf(pFile, "%s\n", OBJECTMOVESET);
							fprintf(pFile, "	%s%d						# �g�p���郂�f���̔ԍ�\n", TYPE, pObjectMove->GetModelIdx());
							fprintf(pFile, "	%s%.1f %.1f %.1f			# ���f���̍��W\n", POS, pObjectMove->GetPosStd().x, pObjectMove->GetPosStd().y, pObjectMove->GetPosStd().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f				# ���f���̌���\n", ROT, ModelRot.x, ModelRot.y, ModelRot.z);
							fprintf(pFile, "	%s%.1f %.1f %.1f		# ���f���̉�]����X�s�[�h\n", ROLL_SPEED, ModelRollSpeed.x, ModelRollSpeed.y, ModelRollSpeed.z);
							fprintf(pFile, "	%s%.1f %.1f %.1f				# ���f���̈ړ���\n", MOVE, pObjectMove->GetMove().x, pObjectMove->GetMove().y, pObjectMove->GetMove().z);
							fprintf(pFile, "	%s%d					# �ړ��ʂ�؂�ւ��鎞��\n", MOVE_MAX, pObjectMove->GetMaxMoveCounter());
							fprintf(pFile, "%s\n", END_OBJECTMOVESET);
							fprintf(pFile, "\n");
						}
					}
					pScene = pSceneNext;
				}
			}


			// �R�����g��������������
			fprintf(pFile, "%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n", END_SCRIPT);

			// �t�@�C�������
			fclose(pFile);
		}
	}
}

//=============================================================================
//    �z�u����z�u���鏈��
//=============================================================================
void CMap::SetObject(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 RollSpeed, int nModelIdx)
{
	// �z�u���𐶐�����
	CObject *pObject = CObject::Create(Pos, Rot, RollSpeed);
	if (pObject != NULL)
	{
		pObject->BindModel(m_pModelManager->GetMesh(nModelIdx), m_pModelManager->GetBuffMat(nModelIdx), m_pModelManager->GetNumMat(nModelIdx), m_pModelManager->GetVtxMax(nModelIdx), m_pModelManager->GetVtxMin(nModelIdx), m_pModelManager->GetTexture(nModelIdx));
		pObject->SetModelIdx(nModelIdx);
	}
}

//=============================================================================
//    �����z�u����z�u���鏈��
//=============================================================================
void CMap::SetObjectMove(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 RollSpeed, int nModelIdx, D3DXVECTOR3 Move, int nMaxMoveCounter)
{
	// �����z�u���𐶐�����
	CObjectMove *pObjectMove = CObjectMove::Create(Pos, Rot, RollSpeed, Move, nMaxMoveCounter);
	if (pObjectMove != NULL)
	{
		pObjectMove->BindModel(m_pModelManager->GetMesh(nModelIdx), m_pModelManager->GetBuffMat(nModelIdx), m_pModelManager->GetNumMat(nModelIdx), m_pModelManager->GetVtxMax(nModelIdx), m_pModelManager->GetVtxMin(nModelIdx), m_pModelManager->GetTexture(nModelIdx));
		pObjectMove->SetModelIdx(nModelIdx);
	}
}

//=============================================================================
//    �ǂݍ��񂾃}�b�v�̃t�@�C�������擾���鏈��
//=============================================================================
char *CMap::GetMapName(void)
{
	return m_aFileName;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CModelManager *CMap::GetModelManager(void)
{
	return m_pModelManager;
}