//*****************************************************************************
//
//     �}�b�v�̏���[map.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "map.h"
#include "manager.h"
#include "system.h"
#include "effect.h"
#include "object.h"
#include "modelManager.h"
#include "textureManager.h"
#include "scene.h"
#include "character.h"
#include "mode.h"
#include "functionlib.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
// �}�b�v�z�u�p
#define MAP_SET_ENEMY              (true)                                        // �G��z�u���邩�ǂ���
#define MAP_SET_OBJECT             (true)                                        // ���f����z�u���邩�ǂ���
#define MAP_SET_EFFECT             (true)                                        // �G�t�F�N�g��z�u���邩�ǂ���

// �Z�[�u�f�[�^�p
#define MAP_OBJECT_SAVEFILENAME    "data/TEXT/MAP/OBJECT/save_object.txt"        // �z�u������ۑ�����e�L�X�g�t�@�C����
#define MAP_ENEMY_SAVEFILENAME     "data/TEXT/MAP/ENEMY/save_set_enemy.txt"      // �G�̔z�u����ۑ�����e�L�X�g�t�@�C����
#define MAP_BILLBOARD_SAVEFILENAME "data/TEXT/MAP/BILLBOARD/save_billboard.txt"  // �z�u�p�r���{�[�h����ۑ�����e�L�X�g�t�@�C����
#define MAP_EFFECT_SAVEFILENAME    "data/TEXT/MAP/EFFECT/save_effect.txt"        // �z�u�G�t�F�N�g����ۑ�����e�L�X�g�t�@�C����
#define MAP_WALL_SAVEFILENAME      "data/TEXT/MAP/WALL/save_wall.txt"            // �Ǐ���ۑ�����e�L�X�g�t�@�C����
#define MAP_ITEM_SAVEFILENAME      "data/TEXT/MAP/ITEM/save_item.txt"            // �A�C�e������ۑ�����e�L�X�g�t�@�C����

// �X�N���v�g�t�@�C���̓ǂݍ��ݗp�̃p�X��
#define MAPSET               "MAPSET"                  // �}�b�v�Z�b�g�J�n�̍��}
#define END_MAPSET           "END_MAPSET"              // �}�b�v�Z�b�g�I���̍��}
#define FIELDSET             "FIELDSET"                // �n�ʃZ�b�g�J�n�̍��}
#define END_FIELDSET         "END_FIELDSET"            // �n�ʃZ�b�g�I���̍��}
#define RIVERSET             "RIVERSET"                // ��Z�b�g�J�n�̍��}
#define END_RIVERSET         "END_RIVERSET"            // ��Z�b�g�I���̍��}
#define OBJECTSET            "OBJECTSET"               // �z�u���Z�b�g�J�n�̍��}
#define END_OBJECTSET        "END_OBJECTSET"           // �z�u���Z�b�g�I���̍��}
#define OBJECTMOVESET        "OBJECTMOVESET"           // �����z�u���Z�b�g�J�n�̍��}
#define END_OBJECTMOVESET    "END_OBJECTMOVESET"       // �����z�u���Z�b�g�I���̍��}
#define OBJECTGOALSET        "OBJECTGOALSET"           // �S�[���p�z�u���Z�b�g�J�n�̍��}
#define END_OBJECTGOALSET    "END_OBJECTGOALSET"       // �S�[���p�z�u���Z�b�g�I���̍��}
#define POLYGONSET           "POLYGONSET"              // �S�[���p�|���S�����J�n�̍��}
#define END_POLYGONSET       "END_POLYGONSET"          // �S�[���p�|���S�����I���̍��}
#define MOUNTAINSET          "MOUNTAINSET"             // �R�Z�b�g�J�n�̍��}
#define END_MOUNTAINSET      "END_MOUNTAINSET"         // �R�Z�b�g�I���̍��}
#define SKYSET               "SKYSET"                  // ��Z�b�g�J�n�̍��}
#define END_SKYSET           "END_SKYSET"              // ��Z�b�g�I���̍��}
#define ENEMYSET             "ENEMYSET"                // �G�̔z�u�f�[�^�ǂݍ��݊J�n�̍��}
#define END_ENEMYSET         "END_ENEMYSET"            // �G�̔z�u�f�[�^�ǂݍ��ݏI���̍��}
#define BILLBOARDSET         "BILLBOARDSET"            // �z�u�p�r���{�[�h�ǂݍ��݊J�n�̍��}
#define END_BILLBOARDSET     "END_BILLBOARDSET"        // �z�u�p�r���{�[�h�ǂݍ��ݏI���̍��}
#define EFFECTSET            "EFFECTSET"               // �z�u�G�t�F�N�g�ǂݍ��݊J�n�̍��}
#define END_EFFECTSET        "END_EFFECTSET"           // �z�u�G�t�F�N�g�ǂݍ��ݏI���̍��}
#define WALLSET              "WALLSET"                 // �ǃZ�b�g�ǂݍ��݊J�n�̍��}
#define END_WALLSET          "END_WALLSET"             // �ǃZ�b�g�ǂݍ��ݏI���̍��}
#define ITEMSET              "ITEMSET"                 // �A�C�e���ǂݍ��݊J�n�̍��}
#define END_ITEMSET          "END_ITEMSET"             // �A�C�e���ǂݍ��ݏI���̍��}
#define AISET                "AISET"                   // �G��AI���ǂݍ��݊J�n�̍��}
#define END_AISET            "END_AISET"               // �G��AI���ǂݍ��ݏI���̍��}

// �t�@�C�����ǂݎ��p
#define FIELD_FILENAME       "FIELD_FILENAME = "       // �n�ʂ̃X�N���v�g�t�@�C����
#define RIVER_FILENAME       "RIVER_FILENAME = "       // ��̃X�N���v�g�t�@�C����
#define OBJECT_FILENAME      "OBJECT_FILENAME = "      // �z�u���̃X�N���v�g�t�@�C����
#define MOUNTAIN_FILENAME    "MOUNTAIN_FILENAME = "    // �R�̃X�N���v�g�t�@�C����
#define SKY_FILENAME         "SKY_FILENAME = "         // ��̃X�N���v�g�t�@�C����
#define ENEMY_FILENAME       "ENEMY_FILENAME = "       // �G�̃X�N���v�g�t�@�C����
#define BILLBOARD_FILENAME   "BILLBOARD_FILENAME = "   // �z�u�p�r���{�[�h�̃X�N���v�g�t�@�C����
#define EFFECT_FILENAME      "EFFECT_FILENAME = "      // �z�u�G�t�F�N�g�̃X�N���v�g�t�@�C����
#define WALL_FILENAME        "WALL_FILENAME = "        // �ǂ̃X�N���v�g�t�@�C����
#define ITEM_FILENAME        "ITEM_FILENAME = "        // �A�C�e���̃X�N���v�g�t�@�C����
#define PLAYER_DEFAULT_SPAWN "PLAYER_DEFAULT_SPAWN = " // �v���C���[�̃f�t�H���g�X�|�[���ʒu
#define CAMERA_DEFAULT_ROT   "CAMERA_DEFAULT_ROT = "   // �J�����̃f�t�H���g����

// ���̑�
#define NUM_TEXTURE          "NUM_TEXTURE = "          // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME     "TEXTURE_FILENAME = "     // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define NUM_MODEL            "NUM_MODEL = "            // �ǂݍ��ރ��f���̐�
#define MODEL_FILENAME       "MODEL_FILENAME = "       // �ǂݍ��ރ��f���t�@�C����
#define ENEMYDATA_FILENAME   "ENEMYDATA_FILENAME = "   // �ǂݍ��ޓG�̃f�[�^�t�@�C����
#define OBJECT_DATA          "OBJECT_DATA = "          // �ǂݍ��߂�z�u���̃f�[�^��
#define OBJECTDATA_FILENAME  "OBJECTDATA_FILENAME = "  // �ǂݍ��ޔz�u���̃f�[�^�t�@�C����

// �f�[�^�ǂݎ��p
#define TEX_TYPE             "TEX_TYPE = "             // �e�N�X�`���̔ԍ����
#define TYPE                 "TYPE = "                 // ��ޏ��
#define POS                  "POS = "                  // ���W���
#define POS_TARGET           "POS_TARGET = "           // �ړI�̈ʒu
#define OFFSET               "OFFSET = "               // �I�t�Z�b�g�ʒu���
#define ROT                  "ROT = "                  // �������
#define SCALE                "SCALE = "                // �傫�����
#define COL                  "COL = "                  // �F���
#define MOVE                 "MOVE = "                 // �ړ��ʏ��
#define MOVE_MAX             "MOVE_MAX = "             // �ړ��ʂ�؂�ւ��鎞�ԏ��
#define PUSH                 "PUSH = "                 // �����邩�ǂ���
#define BREAK                "BREAK = "                // �󂹂邩�ǂ���
#define ROLL_SPEED           "ROLL_SPEED = "           // ��]����X�s�[�h�ʏ��
#define CARRY_TIME           "CARRY_TIME = "           // �v���C���[���^�Ԃ̂ɂ����鎞��
#define COL_RANGE            "COL_RANGE = "            // �����蔻�������͈�
#define COLLISION            "COLLISION = "            // �����蔻������邩�ǂ���(�܂��͎��)
#define WIDTH                "WIDTH = "                // 1�u���b�N���̕����
#define DEPTH                "DEPTH = "                // 1�u���b�N�����s���
#define RADIUS               "RADIUS = "               // ���a���
#define HEIGHT               "HEIGHT = "               // �������
#define XBLOCK               "XBLOCK = "               // ���̕��������
#define YBLOCK               "YBLOCK = "               // �c�̕��������
#define ZBLOCK               "ZBLOCK = "               // ���s�̕��������
#define TEXSPLIT_U           "TEXSPLIT_U = "           // �e�N�X�`��U���W�̕��������
#define TEXSPLIT_V           "TEXSPLIT_V = "           // �e�N�X�`��V���W�̕��������
#define ADDTEX_U             "ADDTEX_U = "             // �e�N�X�`��U���W���ǂꂭ�炢��������
#define ADDTEX_V             "ADDTEX_V = "             // �e�N�X�`��V���W���ǂꂭ�炢��������
#define CULLING              "CULLING = "              // �J�����O���邩�ǂ���
#define BINARY_FILENAME      "BINARY_FILENAME = "      // ���_��񂪊i�[���ꂽ�o�C�i���t�@�C����
#define NEXT_MAPNAME         "NEXT_MAPNAME = "         // ���̃}�b�v�f�[�^�̃t�@�C����
#define LIFE                 "LIFE = "                 // �̗�
#define SCORE                "SCORE = "                // �|�������ɑ�����X�R�A��
#define LIGHTING             "LIGHTING = "             // ���C�e�B���O���邩���Ȃ���
#define DRAW_ADDTIVE         "DRAW_ADDTIVE = "         // ���Z�����ŕ`�悷�邩���Ȃ���
#define CHEACKDISTANCE       "CHEACKDISTANCE = "       // �x���ł���͈�
#define ATTACKDISTANCE       "ATTACKDISTANCE = "       // �U�����d�|����͈�
#define ATTACKRANDOM         "ATTACKRANDOM = "         // �U�����d�|����^�C�~���O

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMap::CMap()
{
	// �e��l�̏�����
	m_pModelManager = NULL;    // ���f���Ǌ��N���X�ւ̃|�C���^
	m_pTextureManager = NULL;  // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_pEffectManager = NULL;   // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	//m_pEnemyAssembly = NULL;   // �G�Ǌ��܂Ƃ߃N���X�ւ̃|�C���^
	m_nCntLoadEnemy = 0;       // �G�f�[�^��ǂݍ��񂾉�

	for (int nCntObj = 0; nCntObj < 50; nCntObj++)
	{
		m_pObjectManager[nCntObj] = NULL;
	}
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
CMap *CMap::Create(char *aFileName, CEffectManager *pEffectManager)
{
	CMap *pMap = NULL;  // �}�b�v�N���X�^�̃|�C���^
	if (pMap == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pMap = new CMap;
		if (pMap != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pMap->Init(aFileName, pEffectManager);
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
void CMap::Init(char *aFileName, CEffectManager *pEffectManager)
{
	// �}�b�v�̊O���t�@�C������ݒ�
	strcpy(m_aFileName, aFileName);

	// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^��ݒ�
	m_pEffectManager = pEffectManager;

	// �}�b�v�����O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                                             // 1�s���ǂݎ��p
			char *pStrCur = NULL;                                           // ���݂̐擪�|�C���^
			char *pStr = NULL;                                              // �擪�|�C���^�ۑ��p
			int nNumTex = 0;                                                // �e�N�X�`����ǂݍ��ސ�
			int nNumModel = 0;                                              // ���f����ǂݍ��ސ�
			int nCntTex = 0;                                                // �e�N�X�`����ǂݍ��񂾉�
			int nCntModel = 0;                                              // ���f����ǂݍ��񂾉�
			LPDIRECT3DTEXTURE9 pTexture = NULL;                             // �e�N�X�`���ǂݎ��p
			LPD3DXMESH pMesh = NULL;                                        // ���b�V�����ւ̃|�C���^
			LPD3DXBUFFER pBuffMat = NULL;                                   // �}�e���A�����ւ̃|�C���^
			DWORD nNumMat = 0;                                              // �}�e���A�����̐�
			char aTexFileName[256] = "\0";                                  // �e�N�X�`���̃t�@�C����
			D3DXVECTOR3 DefaultPlayerSpawn = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �v���C���[�̃f�t�H���g�X�|�[���ʒu
			D3DXVECTOR3 DefaultCameraRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �J�����̃f�t�H���g����

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
								if (MAP_SET_OBJECT)
								{// ���f����z�u����Ȃ�
								    // �z�u���̃X�N���v�g�t�@�C������ǂݎ��
									pStr = CFunctionLib::ReadString(pStrCur, pStr, OBJECT_FILENAME);
									ObjectLoad(pStr, pStrCur, pLine);
								}
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
							else if (CFunctionLib::Memcmp(pStrCur, ENEMY_FILENAME) == 0)
							{// �G�̃X�N���v�g�t�@�C������������
							}
							else if (CFunctionLib::Memcmp(pStrCur, BILLBOARD_FILENAME) == 0)
							{// �z�u�p�r���{�[�h�̃X�N���v�g�t�@�C������������
							    // �z�u�p�r���{�[�h�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, BILLBOARD_FILENAME);
								BillboardLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, EFFECT_FILENAME) == 0)
							{// �z�u�G�t�F�N�g�̃X�N���v�g�t�@�C������������
								if (MAP_SET_EFFECT)
								{// �G�t�F�N�g��z�u����Ȃ�
								 // �z�u�G�t�F�N�g�̃X�N���v�g�t�@�C������ǂݎ��
									pStr = CFunctionLib::ReadString(pStrCur, pStr, EFFECT_FILENAME);
									EffectLoad(pStr, pStrCur, pLine);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, WALL_FILENAME) == 0)
							{// �ǂ̃X�N���v�g�t�@�C������������
							    // �ǂ̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, WALL_FILENAME);
								WallLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, PLAYER_DEFAULT_SPAWN) == 0)
							{// �v���C���[�̃f�t�H���g�X�|�[���ʒu��������
								DefaultPlayerSpawn = CFunctionLib::ReadVector3(pStrCur, PLAYER_DEFAULT_SPAWN);
								CScene *pScene = NULL;
								CScene *pSceneNext = NULL;
								CPlayer *pPlayer = NULL;
								for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
								{// �D�揇�ʂ̐������J��Ԃ�
									pScene = CScene::GetTop(nCntPriority);
									while (pScene != NULL)
									{// ����������ɂȂ�܂ŌJ��Ԃ�
										pSceneNext = pScene->GetNext();
										if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
										{// �v���C���[�N���X�ւ̃|�C���^������
											pPlayer = (CPlayer*)pScene;
											if (pPlayer != NULL)
											{// �v���C���[�N���X�̃|�C���^���擾�ł���
												pPlayer->SetPos(DefaultPlayerSpawn);
												pPlayer->SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
												pPlayer->SetDestRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
												pPlayer->SetDiffRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
												break;
											}
										}
										pScene = pSceneNext;
									}
								}
							}
							//else if (CFunctionLib::Memcmp(pStrCur, CAMERA_DEFAULT_ROT) == 0)
							//{// �J�����̃f�t�H���g������������
							//	DefaultCameraRot = CFunctionLib::ReadVector3(pStrCur, CAMERA_DEFAULT_ROT);

							//	// ���������W�A���p�ɖ߂�
							//	DefaultCameraRot.x = D3DXToRadian(DefaultCameraRot.x);
							//	DefaultCameraRot.y = D3DXToRadian(DefaultCameraRot.y);
							//	DefaultCameraRot.z = D3DXToRadian(DefaultCameraRot.z);
							//	// X��
							//	if (DefaultCameraRot.x > D3DX_PI)
							//	{
							//		DefaultCameraRot.x -= D3DX_PI * 2.0f;
							//	}
							//	// Y��
							//	if (DefaultCameraRot.y > D3DX_PI)
							//	{
							//		DefaultCameraRot.y -= D3DX_PI * 2.0f;
							//	}
							//	// Z��
							//	if (DefaultCameraRot.z > D3DX_PI)
							//	{
							//		DefaultCameraRot.z -= D3DX_PI * 2.0f;
							//	}

							//	CManager::GetBaseMode()->SetRot(DefaultCameraRot);
							//	CManager::GetCamera()->SetRotDest(DefaultCameraRot);
							//	CManager::GetCamera()->SetRotDiff(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
							//}
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
	// �z�u����ۑ�����
	//Save();

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

	// �G�̊Ǌ��܂Ƃ߃N���X�̃��������J������
	//if (m_pEnemyAssembly != NULL)
	//{
	//	// �������̊J��
	//	delete m_pEnemyAssembly;
	//	m_pEnemyAssembly = NULL;
	//}

	// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^����ɂ���
	if (m_pEffectManager != NULL)
	{
		m_pEffectManager = NULL;
	}

	for (int nCntObjData = 0; nCntObjData < 50; nCntObjData++)
	{
		if (m_pObjectManager[nCntObjData] != NULL)
		{
			m_pObjectManager[nCntObjData]->Uninit();

			// �������̊J��
			delete m_pObjectManager[nCntObjData];
			m_pObjectManager[nCntObjData] = NULL;
		}
	}

	// �}�b�v�̗v�f�ł���N���X���폜����
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT || pScene->GetObjType() == CScene::OBJTYPE_BILLOBJ || pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER
				|| pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER || pScene->GetObjType() == CScene::OBJTYPE_ENEMY || pScene->GetObjType() == CScene::OBJTYPE_MESHFIELD
				|| pScene->GetObjType() == CScene::OBJTYPE_MOUNTAIN || pScene->GetObjType() == CScene::OBJTYPE_SKY || pScene->GetObjType() == CScene::OBJTYPE_PARTICLE
				|| pScene->GetObjType() == CScene::OBJTYPE_RINGEFFECT || pScene->GetObjType() == CScene::OBJTYPE_MESHWALL)
			{// �}�b�v�̗v�f���`������N���X�ւ̃|�C���^������
				pScene->Uninit();
				pScene = NULL;
			}
			pScene = pSceneNext;
		}
	}
	CScene::DeathCheck();
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
			CMeshField *pMeshField = NULL;                           // �n�ʃN���X�ւ̃|�C���^
			int nFieldTexIdx = 0;                                    // �n�ʂ̃e�N�X�`���ԍ�
			D3DXVECTOR3 FieldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �n�ʂ̍��W
			D3DXVECTOR3 FieldRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �n�ʂ̌���
			D3DXCOLOR FieldCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // �n�ʂ̐F
			float fFieldWidth = 0.0f;                                // �n�ʂ�1�u���b�N���̕�
			float fFieldDepth = 0.0f;                                // �n�ʂ�1�u���b�N���̉��s
			int nFieldXblock = 0;                                    // �n�ʂ̉��̃u���b�N��
			int nFieldZblock = 0;                                    // �n�ʂ̉��s�̃u���b�N��
			int nFieldTexSplitU = 0;                                 // �n�ʂ̃e�N�X�`��U���W�̕�����
			int nFieldTexSplitV = 0;                                 // �n�ʂ̃e�N�X�`��V���W�̕�����
			bool bFieldCulling = true;                               // �J�����O���邩�ǂ���
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
								fFieldWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, DEPTH) == 0)
							{// 1�u���b�N���̉��s��񂾂���
								fFieldDepth = CFunctionLib::ReadFloat(pStrCur, DEPTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// ���̕�����������
								nFieldXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ZBLOCK) == 0)
							{// ���s�̕�����������
								nFieldZblock = CFunctionLib::ReadInt(pStrCur, ZBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_U) == 0)
							{// �e�N�X�`��U���W�̕�����������
								nFieldTexSplitU = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_U);
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_V) == 0)
							{// �e�N�X�`��V���W�̕�����������
								nFieldTexSplitV = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_V);
							}
							else if (CFunctionLib::Memcmp(pStrCur, CULLING) == 0)
							{// �J�����O���邩�ǂ���������
								bFieldCulling = CFunctionLib::ReadBool(pStrCur, CULLING);
							}
							else if (CFunctionLib::Memcmp(pStrCur, BINARY_FILENAME) == 0)
							{// ���_��񂪊i�[���ꂽ�o�C�i���t�@�C����������
								pStrCur = CFunctionLib::ReadString(pStrCur, aFileName, BINARY_FILENAME);
								strcpy(aFileName, pStrCur);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_FIELDSET) == 0)
							{// �n�ʏ��ǂݍ��ݏI���̍��}������
							    // �n�ʂ𐶐�����
								pMeshField = CMeshField::Create(FieldPos, FieldRot, FieldCol, fFieldWidth, fFieldDepth, nFieldXblock, nFieldZblock, nFieldTexSplitU, nFieldTexSplitV, aFileName, bFieldCulling, FIELD_PRIORITY);
								if (pMeshField != NULL)
								{// �n�ʂ������ł���
									pMeshField->BindTexture(m_pTextureManager->GetTexture(nFieldTexIdx));
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
			char pStr[256] = "\0";                                          // �ǂݍ��ݗp�̕�����
			int nNumObjData = 0;                                            // �z�u���̃f�[�^��ǂݍ��߂鐔
			int nCntObjData = 0;                                            // �z�u���̃f�[�^��ǂݍ��񂾐�
			CObject *pObject = NULL;                                        // �z�u���N���X�ւ̃|�C���^
			int nObjectType = 0;                                            // �z�u���̃��f���ԍ�
			D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �z�u���̍��W
			D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �z�u���̌���
			D3DXVECTOR3 ObjectScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // �z�u���̑傫���̔{��
			D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �z�u���̉�]�X�s�[�h
			bool bObjectBreak = false;                                      // �z�u�����󂹂邩�ǂ���

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
				while (1)
				{// ���[�v�J�n
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, OBJECT_DATA) == 0)
					{// �z�u���f�[�^��������
						nNumObjData = CFunctionLib::ReadInt(pStrCur, OBJECT_DATA);
					}
					else if (CFunctionLib::Memcmp(pStrCur, OBJECTDATA_FILENAME) == 0)
					{// �z�u���f�[�^�̃t�@�C����������
						pLine = CFunctionLib::ReadString(pStrCur, pLine, OBJECTDATA_FILENAME);
						m_pObjectManager[nCntObjData] = CObjectManager::Create(pLine, m_pModelManager);
						nCntObjData++;
					}
					else if (CFunctionLib::Memcmp(pStrCur, OBJECTGOALSET) == 0)
					{// �S�[���p�z�u�����ǂݍ��݊J�n�̍��}������
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
							else if (CFunctionLib::Memcmp(pStrCur, SCALE) == 0)
							{// �X�P�[����񂾂���
								ObjectScale = CFunctionLib::ReadVector3(pStrCur, SCALE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
							{// ��]����X�s�[�h��񂾂���
								D3DXVECTOR3 RotSpeed = CFunctionLib::ReadVector3(pStrCur, ROLL_SPEED);
								ObjectRollSpeed.x = D3DXToRadian(RotSpeed.x);
								ObjectRollSpeed.y = D3DXToRadian(RotSpeed.y);
								ObjectRollSpeed.z = D3DXToRadian(RotSpeed.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, BREAK) == 0)
							{// ��]����X�s�[�h��񂾂���
								bObjectBreak = CFunctionLib::ReadBool(pStrCur, BREAK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_OBJECTGOALSET) == 0)
							{// �S�[���p�z�u�����ǂݍ��ݏI���̍��}������
							    // �S�[���p�z�u���𐶐�����
								if (m_pObjectManager[nObjectType] != NULL)
								{
									m_pObjectManager[nObjectType]->CreateStarObject(ObjectPos, ObjectRot, ObjectScale, ObjectRollSpeed, nObjectType, false, OBJECT_PRIORITY);
								}
								break;  // ���[�v�I��
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, OBJECTSET) == 0)
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
							else if (CFunctionLib::Memcmp(pStrCur, SCALE) == 0)
							{// �X�P�[����񂾂���
								ObjectScale = CFunctionLib::ReadVector3(pStrCur, SCALE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
							{// ��]����X�s�[�h��񂾂���
								D3DXVECTOR3 RotSpeed = CFunctionLib::ReadVector3(pStrCur, ROLL_SPEED);
								ObjectRollSpeed.x = D3DXToRadian(RotSpeed.x);
								ObjectRollSpeed.y = D3DXToRadian(RotSpeed.y);
								ObjectRollSpeed.z = D3DXToRadian(RotSpeed.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, BREAK) == 0)
							{// ��]����X�s�[�h��񂾂���
								bObjectBreak = CFunctionLib::ReadBool(pStrCur, BREAK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_OBJECTSET) == 0)
							{// �z�u�����ǂݍ��ݏI���̍��}������
							    // �z�u���𐶐�����
								if (m_pObjectManager[nObjectType] != NULL)
								{
									m_pObjectManager[nObjectType]->CreateObject(ObjectPos, ObjectRot, ObjectScale, ObjectRollSpeed, nObjectType, bObjectBreak, OBJECT_PRIORITY);
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
			CMountain *pMountain = NULL;                                // �R�N���X�ւ̃|�C���^
			int nMountainTexIdx = 0;                                    // �R�̃e�N�X�`���ԍ�
			D3DXVECTOR3 MountainPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �R�̍��W
			D3DXVECTOR3 MountainRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �R�̌���
			D3DXCOLOR MountainCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // �R�̐F
			float fMountainRadius = 0.0f;                               // �R�̔��a
			float fMountainHeight = 0.0f;                               // �R�̍���
			int nMountainXblock = 0;                                    // �R�̉��̃u���b�N��
			int nMountainYblock = 0;                                    // �R�̏c�̃u���b�N��

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
								fMountainRadius = CFunctionLib::ReadFloat(pStrCur, RADIUS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
							{// �R�̍�����񂾂���
								fMountainHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// ���̕�����������
								nMountainXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
							{// �c�̕�����������
								nMountainYblock = CFunctionLib::ReadInt(pStrCur, YBLOCK);
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
				pMountain = CMountain::Create(MountainPos, MountainRot, MountainCol, fMountainHeight, fMountainRadius, nMountainXblock, nMountainYblock, MOUNTAIN_PRIORITY);
				if (pMountain != NULL)
				{// �R�������ł���
					pMountain->BindTexture(m_pTextureManager->GetTexture(nMountainTexIdx));
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
			CSky *pSky = NULL;                                     // ��N���X�ւ̃|�C���^
			int nSkyTexIdx = 0;                                    // ��̃e�N�X�`���ԍ�
			D3DXVECTOR3 SkyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ��̍��W
			D3DXVECTOR3 SkyRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ��̌���
			D3DXCOLOR SkyCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // ��̐F
			float fSkyRadius = 0.0f;                               // ��̔��a
			int nSkyXblock = 0;                                    // ��̉��̃u���b�N��
			int nSkyYblock = 0;                                    // ��̏c�̃u���b�N��
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
								fSkyRadius = CFunctionLib::ReadFloat(pStrCur, RADIUS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// ���̕�����������
								nSkyXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
							{// �c�̕�����������
								nSkyYblock = CFunctionLib::ReadInt(pStrCur, YBLOCK);
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
				pSky = CSky::Create(SkyPos, SkyRot, SkyCol, fSkyRadius, nSkyXblock, nSkyYblock, fRotSpeed, SKY_PRIORITY);
				if (pSky != NULL)
				{// �󂪐����ł���
					pSky->BindTexture(m_pTextureManager->GetTexture(nSkyTexIdx));
				}
			}
			// �t�@�C�������
			fclose(pFile);
		}
	}
}

//=============================================================================
//    �G���ǂݍ��ݏ���
//=============================================================================
//void CMap::EnemyLoad(char *pEnemyFileName, char *pStrCur, char *pLine)
//{
//	FILE *pFile = NULL;  // �t�@�C���|�C���^
//	if (pFile == NULL)
//	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
//		pFile = fopen(pEnemyFileName, "r");
//		if (pFile != NULL)
//		{// �t�@�C���|�C���^���m�ۂł���
//			int nCntEnemyLoad = 0;                                // �G�l�~�[�f�[�^��ǂݍ��񂾉�
//			char aEnemyDataFileName[256] = "\0";                  // �G�l�~�[�f�[�^�̃X�N���v�g�t�@�C����
//			CEnemyManager *apEnemyManager[CEnemy::TYPE_MAX] = {}; // �G�Ǌ��N���X�ւ̃|�C���^
//
//			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
//			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
//			{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
//				while (1)
//				{// ���[�v�J�n
//					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
//					if (CFunctionLib::Memcmp(pStrCur, ENEMYDATA_FILENAME) == 0)
//					{// �G�̃f�[�^���ǂݍ��݊J�n�̍��}������
//						pStrCur = CFunctionLib::ReadString(pStrCur, aEnemyDataFileName, ENEMYDATA_FILENAME);
//						strcpy(aEnemyDataFileName, pStrCur);
//						if (apEnemyManager[nCntEnemyLoad] == NULL)
//						{// �|�C���^���m�ۂł����Ԃł���
//							apEnemyManager[nCntEnemyLoad] = CEnemyManager::Create(aEnemyDataFileName);
//							nCntEnemyLoad++;
//						}
//					}
//					else if (CFunctionLib::Memcmp(pStrCur, ENEMYSET) == 0)
//					{// �G�̔z�u���ǂݍ��݊J�n�̍��}������
//						int nEnemyType = 0;
//						D3DXVECTOR3 EnemyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//						D3DXVECTOR3 EnemyRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//						float fEnemyAI_CheackDistance = 0.0f;
//						float fEnemyAI_AttackDistance = 0.0f;
//						int nEnemyAI_AttackTiming = 1;
//						int nEnemyLife = 0;
//						int nEnemyAddScore = 0;
//						while (1)
//						{// ���[�v�J�n
//							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
//							if (CFunctionLib::Memcmp(pStrCur, AISET) == 0)
//							{// AI�̏��ǂݍ��݊J�n�̍��}������
//								while (1)
//								{// ���[�v�J�n
//									pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
//									if (CFunctionLib::Memcmp(pStrCur, CHEACKDISTANCE) == 0)
//									{// �x���ł���͈͏�񂪂���
//										fEnemyAI_CheackDistance = CFunctionLib::ReadFloat(pStrCur, CHEACKDISTANCE);
//									}
//									else if (CFunctionLib::Memcmp(pStrCur, ATTACKDISTANCE) == 0)
//									{// �U�����d�|����͈͏�񂪂���
//										fEnemyAI_AttackDistance = CFunctionLib::ReadFloat(pStrCur, ATTACKDISTANCE);
//									}
//									else if (CFunctionLib::Memcmp(pStrCur, ATTACKRANDOM) == 0)
//									{// �U�����d�|����^�C�~���O��񂪂���
//										nEnemyAI_AttackTiming = CFunctionLib::ReadInt(pStrCur, ATTACKRANDOM);
//									}
//									else if (CFunctionLib::Memcmp(pStrCur, END_AISET) == 0)
//									{// AI�̏��ǂݍ��ݏI���̍��}������
//										break;
//									}
//								}
//							}
//							else if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
//							{// �G�̎�ނ�����
//								nEnemyType = CFunctionLib::ReadInt(pStrCur, TYPE);
//							}
//							else if (CFunctionLib::Memcmp(pStrCur, LIFE) == 0)
//							{// �̗͂�����
//								nEnemyLife = CFunctionLib::ReadInt(pStrCur, LIFE);
//							}
//							else if (CFunctionLib::Memcmp(pStrCur, SCORE) == 0)
//							{// �G�̍��W��񂪂���
//								nEnemyAddScore = CFunctionLib::ReadInt(pStrCur, SCORE);
//							}
//							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
//							{// �G�̍��W��񂪂���
//								EnemyPos = CFunctionLib::ReadVector3(pStrCur, POS);
//							}
//							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
//							{// �G�̌�����񂪂���
//								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
//								EnemyRot.x = D3DXToRadian(Rot.x);
//								EnemyRot.y = D3DXToRadian(Rot.y);
//								EnemyRot.z = D3DXToRadian(Rot.z);
//							}
//							else if (CFunctionLib::Memcmp(pStrCur, END_ENEMYSET) == 0)
//							{// �G�̔z�u���ǂݍ��ݏI���̍��}������
//								if (apEnemyManager[nEnemyType] != NULL)
//								{// �G�̊Ǌ��N���X�ւ̃|�C���^���擾�ł��Ă���
//									apEnemyManager[nEnemyType]->SetEnemy(EnemyPos, EnemyRot, nEnemyType, fEnemyAI_CheackDistance, fEnemyAI_AttackDistance, nEnemyAI_AttackTiming, nEnemyLife, nEnemyAddScore);
//								}
//								break;  // ���[�v�I��
//							}
//						}
//					}
//					else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
//					{// �X�N���v�g�ǂݍ��ݏI���̍��}������
//					 // �G�Ǌ��܂Ƃ߃N���X�փ|�C���^��ݒ肷��
//						if (m_pEnemyAssembly == NULL)
//						{// ���������m�ۂł����Ԃł���
//							m_pEnemyAssembly = new CEnemyAssembly;
//							if (m_pEnemyAssembly != NULL)
//							{// ���������m�ۂł���
//								for (int nCntEnemy = 0; nCntEnemy < nCntEnemyLoad; nCntEnemy++)
//								{// �G�̃f�[�^��ǂݍ��񂾉񐔕��J��Ԃ�
//									m_pEnemyAssembly->SetEnemyManager(apEnemyManager[nCntEnemy], nCntEnemy);
//								}
//								// �ǂݍ��񂾉񐔂�ۑ�����
//								m_nCntLoadEnemy = nCntEnemyLoad;
//							}
//						}
//
//						break;  // ���[�v�I��
//					}
//				}
//			}
//			// �t�@�C�������
//			fclose(pFile);
//		}
//	}
//}

//=============================================================================
//    �z�u�p�r���{�[�h���ǂݍ��ݏ���
//=============================================================================
void CMap::BillboardLoad(char *pBillboardFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(pBillboardFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
				while (1)
				{// ���[�v�J�n
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, BILLBOARDSET) == 0)
					{// �z�u�p�r���{�[�h���ǂݍ��݊J�n�̍��}������
						int nBillboardType = 0;
						int nBillboardTexIdx = 0;
						D3DXVECTOR3 BillboardPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
						D3DXCOLOR BillboardCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
						float fBillboardColRange = 0.0f;
						float fBillboardRot = 0.0f;
						float fBillboardWidth = 0.0f;
						float fBillboardHeight = 0.0f;
						bool bBillboardLighting = false;
						bool bBillboardDrawAddtive = false;
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
							{// ��ޔԍ���񂪂���
								nBillboardType = CFunctionLib::ReadInt(pStrCur, TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// �g�p����e�N�X�`���ԍ���񂪂���
								nBillboardTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL_RANGE) == 0)
							{// �����蔻������͈͏�񂪂���
								fBillboardColRange = CFunctionLib::ReadFloat(pStrCur, COL_RANGE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// ���W��񂪂���
								BillboardPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// �F��񂪂���
								BillboardCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// ������񂪂���
								float fRot = CFunctionLib::ReadFloat(pStrCur, ROT);
								fBillboardRot = D3DXToRadian(fRot);
							}
							else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
							{// ����񂪂���
								fBillboardWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
							{// ������񂪂���
								fBillboardHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
							}
							else if (CFunctionLib::Memcmp(pStrCur, LIGHTING) == 0)
							{// ���C�e�B���O���邩���Ȃ�����񂪂���
								bBillboardLighting = CFunctionLib::ReadBool(pStrCur, LIGHTING);
							}
							else if (CFunctionLib::Memcmp(pStrCur, DRAW_ADDTIVE) == 0)
							{// ���Z�����ŕ`�悷�邩���Ȃ�����񂪂���
								bBillboardDrawAddtive = CFunctionLib::ReadBool(pStrCur, DRAW_ADDTIVE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_BILLBOARDSET) == 0)
							{// �z�u�p�r���{�[�h���ǂݍ��ݏI���̍��}������
								CBillboardObject *pBillboardObj = CBillboardObject::Create(BillboardPos, BillboardCol, fBillboardWidth, fBillboardHeight, fBillboardRot, bBillboardLighting, bBillboardDrawAddtive, nBillboardTexIdx, nBillboardType, BILLBOARD_PRIORITY);
								if (pBillboardObj != NULL)
								{// �����ł���
									pBillboardObj->BindTexture(m_pTextureManager->GetTexture(nBillboardTexIdx));
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
//    �z�u�G�t�F�N�g���ǂݍ��ݏ���
//=============================================================================
void CMap::EffectLoad(char *pEffectFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(pEffectFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
			if (m_pEffectManager != NULL)
			{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^���擾�ł���
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
				if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
				{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
					while (1)
					{// ���[�v�J�n
						pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
						if (CFunctionLib::Memcmp(pStrCur, EFFECTSET) == 0)
						{// �z�u�G�t�F�N�g���ǂݍ��݊J�n�̍��}������
							int nEffectType = 0;
							D3DXVECTOR3 EffectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							D3DXVECTOR3 EffectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							while (1)
							{// ���[�v�J�n
								pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
								if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
								{// ��ޏ�񂪂���
									nEffectType = CFunctionLib::ReadInt(pStrCur, TYPE);
								}
								else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
								{// ���W��񂪂���
									EffectPos = CFunctionLib::ReadVector3(pStrCur, POS);
								}
								else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
								{// ������񂪂���
									D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
									EffectRot.x = D3DXToRadian(Rot.x);
									EffectRot.y = D3DXToRadian(Rot.y);
									EffectRot.z = D3DXToRadian(Rot.z);
								}
								else if (CFunctionLib::Memcmp(pStrCur, END_EFFECTSET) == 0)
								{// �z�u�G�t�F�N�g���ǂݍ��ݏI���̍��}������
									m_pEffectManager->SetEffect(EffectPos, EffectRot, nEffectType);
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
			}
			// �t�@�C�������
			fclose(pFile);
		}
	}
}

//=============================================================================
//    �Ǐ��ǂݍ��ݏ���
//=============================================================================
void CMap::WallLoad(char *pWallFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(pWallFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		 // �Ǘp�̊i�[�ϐ�
			CMeshWall *pMeshWall = NULL;                            // �ǃN���X�̃|�C���^
			int nWallTexIdx = 0;                                    // �ǂ̃e�N�X�`���ԍ�
			D3DXVECTOR3 WallPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �ǂ̍��W
			D3DXVECTOR3 WallRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �ǂ̌���
			D3DXCOLOR WallCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // �ǂ̐F
			float fWallWidth = 0.0f;                                // �ǂ�1�u���b�N���̕�
			float fWallHeight = 0.0f;                               // �ǂ�1�u���b�N���̉��s
			int nWallXblock = 0;                                    // �ǂ̉��̃u���b�N��
			int nWallYblock = 0;                                    // �ǂ̏c�̃u���b�N��
			int nWallTexSplitU = 0;                                 // �ǂ̃e�N�X�`��U���W�̕�����
			int nWallTexSplitV = 0;                                 // �ǂ̃e�N�X�`��V���W�̕�����

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
				while (1)
				{// ���[�v�J�n
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, WALLSET) == 0)
					{// �Ǐ��ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// �g�p����e�N�X�`���̔ԍ���񂾂���
								nWallTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// ���W��񂾂���
								WallPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// ������񂾂���
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								WallRot.x = D3DXToRadian(Rot.x);
								WallRot.y = D3DXToRadian(Rot.y);
								WallRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// �F��񂾂���
								WallCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
							{// 1�u���b�N���̕���񂾂���
								fWallWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
							{// 1�u���b�N���̍�����񂾂���
								fWallHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// ���̕�����������
								nWallXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
							{// �c�̕�����������
								nWallYblock = CFunctionLib::ReadInt(pStrCur, YBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_U) == 0)
							{// �e�N�X�`��U���W�̕�����������
								nWallTexSplitU = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_U);
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXSPLIT_V) == 0)
							{// �e�N�X�`��V���W�̕�����������
								nWallTexSplitV = CFunctionLib::ReadInt(pStrCur, TEXSPLIT_V);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_WALLSET) == 0)
							{// �Ǐ��ǂݍ��ݏI���̍��}������
							 // �ǂ𐶐�����
								pMeshWall = CMeshWall::Create(WallPos, WallRot, WallCol, fWallWidth, fWallHeight, nWallXblock, nWallYblock, nWallTexSplitU, nWallTexSplitV, nWallTexIdx, WALL_PRIORITY);
								if (pMeshWall != NULL)
								{// �ǂ������ł���
									pMeshWall->BindTexture(m_pTextureManager->GetTexture(nWallTexIdx));
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
//    �e��z�u����ۑ����鏈��
//=============================================================================
void CMap::Save(void)
{
	// ���f���̔z�u���
	SaveObject();

	// �G�̔z�u���
	//SaveEnemy();

	// �r���{�[�h�̔z�u���
	SaveBillboard();

	// �G�t�F�N�g�̔z�u���
	SaveEffect();

	// �ǂ̔z�u���
	SaveWall();
}

//=============================================================================
//    ���f���̔z�u����ۑ����鏈��
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
							fprintf(pFile, "	%s%d						# �g�p���郂�f���̔ԍ�\n", TYPE, pObject->GetType());
							fprintf(pFile, "	%s%.1f %.1f %.1f			# ���f���̍��W\n", POS, pObject->GetPos().x, pObject->GetPos().y, pObject->GetPos().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f				# ���f���̌���\n", ROT, ModelRot.x, ModelRot.y, ModelRot.z);
							fprintf(pFile, "	%s%.1f %.1f %.1f				# ���f���̑傫��\n", SCALE, pObject->GetScale().x, pObject->GetScale().y, pObject->GetScale().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f		# ���f���̉�]����X�s�[�h\n", ROLL_SPEED, ModelRollSpeed.x, ModelRollSpeed.y, ModelRollSpeed.z);
							fprintf(pFile, "%s\n", END_OBJECTSET);
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
//    �G�̔z�u����ۑ����鏈��
//=============================================================================
//void CMap::SaveEnemy(void)
//{
//	FILE *pFile = NULL;  // �t�@�C���|�C���^
//	if (pFile == NULL)
//	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
//		pFile = fopen(MAP_ENEMY_SAVEFILENAME, "w");
//		if (pFile != NULL)
//		{// �t�@�C���|�C���^���m�ۂł���
//		 // �R�����g��������������
//			fprintf(pFile, "#******************************************************************************\n");
//			fprintf(pFile, "#\n");
//			fprintf(pFile, "#       �G�̔z�u���X�N���v�g�t�@�C��[save_set_enemy.txt]\n");
//			fprintf(pFile, "#       Auther : Hodaka Niwa\n");
//			fprintf(pFile, "#\n");
//			fprintf(pFile, "#******************************************************************************\n");
//			fprintf(pFile, "%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);
//			fprintf(pFile, "#------------------------------------------------------------------------------\n");
//			fprintf(pFile, "#  �g�p����G�̃f�[�^\n");
//			fprintf(pFile, "#------------------------------------------------------------------------------\n");
//
//			// �G�̃f�[�^�t�@�C������ۑ�
//			for (int nCntEnemy = 0; nCntEnemy < CEnemy::TYPE_MAX; nCntEnemy++)
//			{// �G�̃f�[�^��ǂݍ��߂�񐔕��J��Ԃ�
//				if (m_pEnemyAssembly != NULL)
//				{
//					CEnemyManager *pEnemyManager = m_pEnemyAssembly->GetEnemyManager(nCntEnemy);
//					if (pEnemyManager != NULL)
//					{
//						fprintf(pFile, "%s%s		# �G%d�̖�\n", ENEMYDATA_FILENAME, pEnemyManager->GetFileName(), nCntEnemy + 1);
//					}
//				}
//			}
//			fprintf(pFile, "\n");
//
//			// �R�����g��������������
//			fprintf(pFile, "#------------------------------------------------------------------------------\n");
//			fprintf(pFile, "#  �G�̔z�u���\n");
//			fprintf(pFile, "#------------------------------------------------------------------------------\n");
//
//			// �G�̔z�u����ۑ�
//			CScene *pScene = NULL;
//			CScene *pSceneNext = NULL;
//			CEnemy *pEnemy = NULL;
//			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
//			{// �D�揇�ʂ̐������J��Ԃ�
//				pScene = CScene::GetTop(nCntPriority);
//				while (pScene != NULL)
//				{// ����������ɂȂ�܂ŌJ��Ԃ�
//					pSceneNext = pScene->GetNext();
//					if (pScene->GetObjType() == CScene::OBJTYPE_ENEMY)
//					{// �G�N���X�ւ̃|�C���^������
//						pEnemy = (CEnemy*)pScene;
//						if (pEnemy != NULL)
//						{// �G�ւ̃|�C���^���擾�ł���
//						    // ���O�Ɍv�Z�����܂��Ă���
//							D3DXVECTOR3 EnemyRot = pEnemy->GetRotStd();
//							// ����
//							EnemyRot.x = D3DXToDegree(EnemyRot.x);
//							EnemyRot.y = D3DXToDegree(EnemyRot.y);
//							EnemyRot.z = D3DXToDegree(EnemyRot.z);
//
//							// �G�̔z�u������������
//							fprintf(pFile, "%s\n", ENEMYSET);
//							fprintf(pFile, "	%s						# AI�̏��\n", AISET);
//							fprintf(pFile, "		%s%f	# �x���ł���͈�", CHEACKDISTANCE, pEnemy->GetAI_CheackDistance());
//							fprintf(pFile, "		%s%f	# �U�����d�|����͈�", ATTACKDISTANCE, pEnemy->GetAI_AttackDistance());
//							fprintf(pFile, "		%s%d		# �U�����d�|����^�C�~���O", ATTACKRANDOM, pEnemy->GetAI_AttackTiming());
//							fprintf(pFile, "	%s\n\n", END_AISET);
//							fprintf(pFile, "	%s%d					# ���\n", TYPE, pEnemy->GetType());
//							fprintf(pFile, "	%s%d					# �̗�\n", LIFE, pEnemy->GetLife());
//							fprintf(pFile, "	%s%d				# �|�������ɑ�����X�R�A��\n", SCORE, pEnemy->GetAddScore());
//							fprintf(pFile, "	%s%.1f %.1f %.1f		# ���W\n", POS, pEnemy->GetPosStd().x, pEnemy->GetPosStd().y, pEnemy->GetPosStd().z);
//							fprintf(pFile, "	%s%.1f %.1f %.1f			# ����\n", ROT, EnemyRot.x, EnemyRot.y, EnemyRot.z);
//							fprintf(pFile, "%s\n", END_ENEMYSET);
//							fprintf(pFile, "\n");
//						}
//					}
//					pScene = pSceneNext;
//				}
//			}
//
//			// �R�����g��������������
//			fprintf(pFile, "%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n", END_SCRIPT);
//
//			// �t�@�C�������
//			fclose(pFile);
//		}
//	}
//}

//=============================================================================
//    �z�u�p�r���{�[�h����ۑ����鏈��
//=============================================================================
void CMap::SaveBillboard(void)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(MAP_BILLBOARD_SAVEFILENAME, "w");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		 // �R�����g��������������
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#       �z�u�p�r���{�[�h���X�N���v�g�t�@�C��[save_billboard.txt]\n");
			fprintf(pFile, "#       Auther : Hodaka Niwa\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "#  �r���{�[�h���\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// �r���{�[�h����ۑ�
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			CBillboardObject *pBillboardObject = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// �D�揇�ʂ̐������J��Ԃ�
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// ����������ɂȂ�܂ŌJ��Ԃ�
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_BILLOBJ)
					{// �z�u�p�r���{�[�h�N���X�ւ̃|�C���^������
						pBillboardObject = (CBillboardObject*)pScene;
						if (pBillboardObject != NULL)
						{// �z�u�p�r���{�[�h�ւ̃|�C���^���擾�ł���
						    // ���O�Ɍv�Z�����܂��Ă���
							float fBillboardRot = pBillboardObject->GetRot();
							// ����
							fBillboardRot = D3DXToDegree(fBillboardRot);

							// �z�u���̏�����������
							fprintf(pFile, "%s\n", BILLBOARDSET);
							fprintf(pFile, "	%s%d				# ��ޔԍ�\n", TYPE, pBillboardObject->GetType());
							fprintf(pFile, "	%s%d				# �g�p����e�N�X�`���̔ԍ�\n", TEX_TYPE, pBillboardObject->GetTexIdx());
							fprintf(pFile, "	%s%.1f %.1f %.1f		# ���W\n", POS, pBillboardObject->GetPos().x, pBillboardObject->GetPos().y, pBillboardObject->GetPos().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f %.1f		# �F\n", COL, pBillboardObject->GetCol().r, pBillboardObject->GetCol().g, pBillboardObject->GetCol().b, pBillboardObject->GetCol().a);
							fprintf(pFile, "	%s%.1f					# ����\n", ROT, fBillboardRot);
							fprintf(pFile, "	%s%.1f				# ��\n", WIDTH, pBillboardObject->GetWidth());
							fprintf(pFile, "	%s%.1f				# ����\n", HEIGHT, pBillboardObject->GetHeight());
							fprintf(pFile, "	%s%d				# ���C�e�B���O���邩���Ȃ���\n", LIGHTING, pBillboardObject->GetLighting());
							fprintf(pFile, "	%s%d			# ���Z�����ŕ`�悷�邩���Ȃ���\n", DRAW_ADDTIVE, pBillboardObject->GetDrawAddtive());
							fprintf(pFile, "%s\n", END_BILLBOARDSET);
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
//    �z�u�G�t�F�N�g����ۑ����鏈��
//=============================================================================
void CMap::SaveEffect(void)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(MAP_EFFECT_SAVEFILENAME, "w");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		 // �R�����g��������������
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#       �z�u�G�t�F�N�g���X�N���v�g�t�@�C��[save_effect.txt]\n");
			fprintf(pFile, "#       Auther : Hodaka Niwa\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "#  �z�u�G�t�F�N�g���\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// �G�~�b�^����ۑ�
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			CEmitter *pEmitter = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// �D�揇�ʂ̐������J��Ԃ�
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// ����������ɂȂ�܂ŌJ��Ԃ�
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_PAREMITTER || pScene->GetObjType() == CScene::OBJTYPE_RINGEMITTER)
					{// �G�~�b�^�N���X�ւ̃|�C���^������
						pEmitter = (CEmitter*)pScene;
						if (pEmitter != NULL)
						{// �G�~�b�^�ւ̃|�C���^���擾�ł���
						 // ���O�Ɍv�Z�����܂��Ă���
							D3DXVECTOR3 EffectRot = pEmitter->GetRot();
							// ����
							EffectRot.x = D3DXToDegree(EffectRot.x);
							EffectRot.y = D3DXToDegree(EffectRot.y);
							EffectRot.z = D3DXToDegree(EffectRot.z);

							// �z�u�G�t�F�N�g�̏�����������
							fprintf(pFile, "%s\n", EFFECTSET);
							fprintf(pFile, "	%s%d				# �g�p����G�t�F�N�g�̎�ޔԍ�\n", TYPE, pEmitter->GetType());
							fprintf(pFile, "	%s%.1f %.1f %.1f		# ���W\n", POS, pEmitter->GetPos().x, pEmitter->GetPos().y, pEmitter->GetPos().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f		# ����\n", ROT, EffectRot.x, EffectRot.y, EffectRot.z);
							fprintf(pFile, "%s\n", END_EFFECTSET);
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
//    �Ǐ���ۑ����鏈��
//=============================================================================
void CMap::SaveWall(void)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(MAP_WALL_SAVEFILENAME, "w");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		 // �R�����g��������������
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#       �Ǐ��X�N���v�g�t�@�C��[save_wall.txt]\n");
			fprintf(pFile, "#       Auther : Hodaka Niwa\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "%s				#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "#  �Ǐ��\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// �Ǐ���ۑ�
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			CMeshWall *pMeshWall = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// �D�揇�ʂ̐������J��Ԃ�
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// ����������ɂȂ�܂ŌJ��Ԃ�
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_MESHWALL)
					{// �ǃN���X�ւ̃|�C���^������
						pMeshWall = (CMeshWall*)pScene;
						if (pMeshWall != NULL)
						{// �z�u�p�r���{�[�h�ւ̃|�C���^���擾�ł���
						 // ���O�Ɍv�Z�����܂��Ă���
							D3DXVECTOR3 WallRot = pMeshWall->GetRot();
							// ����
							WallRot.x = D3DXToDegree(WallRot.x);
							WallRot.y = D3DXToDegree(WallRot.y);
							WallRot.z = D3DXToDegree(WallRot.z);

							// �ǂ̏�����������
							fprintf(pFile, "%s\n", WALLSET);
							fprintf(pFile, "	%s%d				# �g�p����e�N�X�`���̔ԍ�\n", TEX_TYPE, pMeshWall->GetTexIdx());
							fprintf(pFile, "	%s%.1f %.1f %.1f		# ���W\n", POS, pMeshWall->GetPos().x, pMeshWall->GetPos().y, pMeshWall->GetPos().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f			# ����\n", ROT, WallRot.x, WallRot.y, WallRot.z);
							fprintf(pFile, "	%s%.1f %.1f %.1f %.1f		# �F\n", COL, pMeshWall->GetCol().r, pMeshWall->GetCol().g, pMeshWall->GetCol().b, pMeshWall->GetCol().a);
							fprintf(pFile, "	%s%.1f				# 1�u���b�N���̕�\n", WIDTH, pMeshWall->GetWidth());
							fprintf(pFile, "	%s%.1f				# 1�u���b�N���̍���\n", HEIGHT, pMeshWall->GetHeight());
							fprintf(pFile, "	%s%d					# ���̕�����\n", XBLOCK, pMeshWall->GetXBlock());
							fprintf(pFile, "	%s%d					# �c�̕�����\n", YBLOCK, pMeshWall->GetYBlock());
							fprintf(pFile, "	%s%d				# �e�N�X�`��U���W�̕�����\n", TEXSPLIT_U, pMeshWall->GetTexSplitU());
							fprintf(pFile, "	%s%d				# �e�N�X�`��V���W�̕�����\n", TEXSPLIT_V, pMeshWall->GetTexSplitV());
							fprintf(pFile, "%s\n", END_WALLSET);
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
//    �r���{�[�h��z�u���鏈��
//=============================================================================
void CMap::SetBillboard(D3DXVECTOR3 Pos, D3DXCOLOR Col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, int nType)
{
	CBillboardObject *pBillObj = CBillboardObject::Create(Pos, Col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, nType);
	if (pBillObj != NULL)
	{
		pBillObj->BindTexture(m_pTextureManager->GetTexture(nTexIdx));
	}
}

//=============================================================================
//    �G�t�F�N�g��z�u���鏈��
//=============================================================================
void CMap::SetEffect(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, int nType)
{
	if (m_pEffectManager != NULL)
	{
		m_pEffectManager->SetEffect(Pos, Rot, nType);
	}
}

//=============================================================================
//    �G��z�u���鏈��
//=============================================================================
void CMap::SetEnemy(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, int nType, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nScore)
{
	//if (m_pEnemyAssembly != NULL)
	//{
	//	CEnemyManager *pEnemyManager = m_pEnemyAssembly->GetEnemyManager(nType);
	//	if (pEnemyManager != NULL)
	//	{
	//		pEnemyManager->SetEnemy(Pos, Rot, nType, fAI_CheackDistance, fAI_AttackDistance, nAI_AttackTiming, nLife, nScore);
	//	}
	//}
}

//=============================================================================
//    �ǂ�z�u���鏈��
//=============================================================================
void CMap::SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx)
{
	// �ǂ𐶐�����
	CMeshWall *pMeshWall = CMeshWall::Create(pos, rot, col, fWidth, fHeight, nXBlock, nYBlock, nTexSplitU, nTexSplitV, nTexIdx);
	if (pMeshWall != NULL)
	{// �ǂ������ł���
		pMeshWall->BindTexture(m_pTextureManager->GetTexture(nTexIdx));
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
//    �G�f�[�^��ǂݍ��񂾉񐔂��擾���鏈��
//=============================================================================
int CMap::GetCntEnemyLoad(void)
{
	return m_nCntLoadEnemy;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CModelManager *CMap::GetModelManager(void)
{
	return m_pModelManager;
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CTextureManager *CMap::GetTextureManager(void)
{
	return m_pTextureManager;
}

//=============================================================================
//    �z�u���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CObjectManager *CMap::GetObjectManager(int nIdx)
{
	return m_pObjectManager[nIdx];
}