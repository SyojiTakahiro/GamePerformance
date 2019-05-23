//*****************************************************************************
//
//     �v���C���[�̏���[player.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "input.h"

#include "model.h"
#include "motion.h"
#include "field.h"
#include "water.h"
#include "object.h"
#include "shadow.h"
#include "particle.h"

#include "game.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
// �e�L�X�g����l��ǂݎ��p�X��
#define NUM_MODEL        "NUM_MODEL"           // �ǂݍ��ރ��f���̐�
#define MODEL_FILENAME   "MODEL_FILENAME"      // ���f���̃t�@�C����
#define PARTSSET         "PARTSSET"            // �p�[�c�Z�b�g�ǂݍ��݊J�n�̍��}
#define END_PARTSSET     "END_PARTSSET"        // �p�[�c�Z�b�g�ǂݍ��ݏI���̍��}
#define CHARACTERSET     "CHARACTERSET"        // �L�����N�^�[�Z�b�g�ǂݍ��݊J�n�̍��}
#define END_CHARACTERSET "END_CHARACTERSET"    // �p�[�c�Z�b�g�ǂݍ��ݏI���̍��}

// �L�����N�^�[�Z�b�g�p�X��
#define MOVE             "MOVE"                // �v���C���[�̉����x
#define MAX_WALKSPEED    "MAX_WALKSPEED"       // �v���C���[�̕����X�s�[�h�̍ő�l
#define JUMP             "JUMP"                // �v���C���[�̃W�����v��
#define GRAVITY          "GRAVITY"             // �v���C���[�̏d��
#define INERTIA          "INERTIA"             // �v���C���[�̊���
#define RIVISION_ANGLE   "RIVISION_ANGLE"      // �v���C���[�̌�����␳����{��
#define NUM_PARTS        "NUM_PARTS"           // �v���C���[���g�p����p�[�c��
#define COL_HEIGHT       "COL_HEIGHT"          // �v���C���[�̓����蔻����Ƃ鍂��

// �p�[�c�Z�b�g�p�X��
#define INDEX            "INDEX"               // ���蓖�Ă�p�[�c�̃��f���ԍ�
#define PARENT           "PARENT"              // �e�p�[�c�̔ԍ�
#define POS              "POS"                 // �e����̃I�t�Z�b�g�ʒu
#define ROT              "ROT"                 // �e����̃I�t�Z�b�g����


// �����l����p�}�N��
#define PLAYER_MAX_WALKSPEED_INI (2.0f)      // �v���C���[�������X�s�[�h�̍ő�l
#define PLAYER_ACCELERATION_INI  (3.0f)      // �v���C���[�̉����x
#define PLAYER_GRAVITY_INI       (-0.7f)     // �v���C���[�̏d��
#define PLAYER_JUMPPOWER_INI     (11.65f)    // �v���C���[�̃W�����v��
#define PLAYER_INERTIA_INI       (0.423f)    // �v���C���[�̊���
#define PLAYER_ANGLE_REVISION    (0.1f)      // �v���C���[�̌�����␳����{��
#define PLAYER_COLHEIGHT_INI     (80.0f)     // �v���C���[�̓����蔻����Ƃ鍂��

// ����p�}�N��
#define PLAYER_COL_RANGE         (15.0f)     // �����蔻������͈�
#define PLAYER_MOVE_SPEED        (0.5f)      // �v���C���[���ړ����Ă���Ɣ��肳��鐔�l�̍ő�l
#define PLAYER_MAX_WALKSPEED     (2.0f)      // �v���C���[�������Ă���Ɣ��肳��鐔�l�̍ő�l
#define PLAYER_PANCH_COLINDEX    (3)         // �p���`�̓����蔻����Ƃ郂�f���̔ԍ�
#define PLAYER_PANCH_COLRANGE    (5.0f)      // �p���`�̓����蔻����Ƃ�͈�
#define PLAYER_KICK_COLINDEX     (7)         // �L�b�N�̓����蔻����Ƃ郂�f���̔ԍ�
#define PLAYER_KICK_COLRANGE     (14.0f)     // �L�b�N�̓����蔻����Ƃ�͈�

//*****************************************************************************
//     CPlayer�̏���
//*****************************************************************************
//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH     *CPlayer::m_apMesh = NULL;      // ���b�V�����ւ̃|�C���^
LPD3DXBUFFER   *CPlayer::m_apBuffMat = NULL;   // �}�e���A�����ւ̃|�C���^
DWORD          *CPlayer::m_anNumMat = NULL;    // �}�e���A�����̐�
D3DXVECTOR3    *CPlayer::m_aVtxMin = NULL;     // ���_���W�̍ŏ��l
D3DXVECTOR3    *CPlayer::m_aVtxMax = NULL;     // ���_���W�̍ő�l
int            CPlayer::m_nNumModel = 0;       // �v���C���[�̎g�p����p�[�c��
char           CPlayer::m_aFileName[256] = ""; // �v���C���[�f�[�^�̊O���t�@�C����

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_apModel = NULL;                                  // ���f���N���X�ւ̃|�C���^
	m_nNumPart = 0;                                    // �g�p����p�[�c��
	m_pMotionManager = NULL;                           // ���[�V�����N���X�ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);             // ���݂̍��W
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �O��̍��W
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);            // �ړ���
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);             // ���݂̌���
	m_DiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // ���݂̌����ƖړI�̌����̍���
	m_DestAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �ړI�̌���
	D3DXMatrixIdentity(&m_MtxWorld);                   // ���[���h�}�g���b�N�X
	m_State = STATE_NORMAL;                            // ���
	m_fGravity = 0.0f;                                 // �d��
	m_fMaxWalkSpeed = 0.0f;                            // �����X�s�[�h�̍ő�l
	m_fJumpPower = 0.0f;                               // �W�����v��
	m_bAction = false;                                 // �A�N�V������Ԃł��邩�ǂ���
	m_bJump = false;                                   // �W�����v�������ǂ���
	m_fAccel = 0.0f;                                   // �����x
	m_fInertia = 0.0f;                                 // ����
	m_fRivisionRot = 0.0f;                             // ������␳����{��
	m_bPush = false;                                   // �I�u�W�F�N�g�������Ă��邩�ǂ���
	m_pLandScene = NULL;                               // �v���C���[������Ă���N���X�ւ̃|�C���^
	m_pShadow = NULL;                                  // �e�N���X�ւ̃|�C���^
	m_bLand = false;                                   // �����ɏ���Ă��邩�ǂ���
	m_bCol = false;                                    // �I�u�W�F�N�g�ɓ������Ă��邩�ǂ���
	m_OffsetPosStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ��������̔z�u���Ƃ̃I�t�Z�b�g�ʒu
	m_OffsetRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // ��������̃v���C���[�̃I�t�Z�b�g����
	m_OffsetRotStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���݂̌����ƖړI�̌����̍���
	m_OffsetPosAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���݂̃I�t�Z�b�g�ʒu�ɉ�����l
	m_OffsetPosAddOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �O��̃I�t�Z�b�g�ʒu�ɉ�����l
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
//    �v���C���[�̐�������
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	CPlayer *pPlayer = NULL;   // �v���C���[�N���X�^�̃|�C���^
	if (pPlayer == NULL)
	{// ����������ɂȂ��Ă���
		pPlayer = new CPlayer(nPriority);
		if (pPlayer != NULL)
		{// �C���X�^���X�𐶐��ł���
			pPlayer->SetPos(pos);
			pPlayer->SetRot(rot);
			if (FAILED(pPlayer->Init()))
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

	return pPlayer;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �v���C���[�̃p�[�c�ǂݍ��ݏ���
//=============================================================================
HRESULT CPlayer::Load(char *pFileName)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			FILE *pFile = NULL;             // �v���C���[�f�[�^�ւ̊O���t�@�C���|�C���^
			char aString[256];              // �e�L�X�g�i�[�p�ϐ�(��{�I�ɗv��Ȃ��������i�[����)
			char aXFileName[256];           // ���f����X�t�@�C����
			int  nCntModel = 0;             // �p�[�c��ǂݍ��񂾉�
			strcpy(m_aFileName,pFileName);  // �t�@�C������ۑ�

			if (pFile == NULL)
			{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
				pFile = fopen(m_aFileName, "r");
				if (pFile != NULL)
				{// �t�@�C���|�C���^���m�ۂł���
					while (fscanf(pFile,"%s",&aString[0]) != EOF)
					{// �e�L�X�g���Ō�̍s�ɓ��B����܂œǂݍ��݂𑱂���
						if (strcmp(&aString[0], NUM_MODEL) == 0)
						{// �ǂݍ��ރ��f�����̃p�X������
							fscanf(pFile, "%s %d", &aString[0], &m_nNumModel);
							if (m_nNumModel >= 1)
							{// �ǂݍ��ރ��f�����P�ȏ゠��
								// �K�v�ȕϐ��𓮓I�m�ۂ���
								// ���b�V�����ւ̃|�C���^
								if (m_apMesh == NULL)
								{// ���������m�ۂł����Ԃɂ���
									m_apMesh = new LPD3DXMESH[m_nNumModel];
								}

								// �}�e���A�����ւ̃|�C���^
								if (m_apBuffMat == NULL)
								{// ���������m�ۂł����Ԃɂ���
									m_apBuffMat = new LPD3DXBUFFER[m_nNumModel];
								}

								// �}�e���A���̐����ւ̃|�C���^
								if (m_anNumMat == NULL)
								{// ���������m�ۂł����Ԃɂ���
									m_anNumMat = new DWORD[m_nNumModel];
								}

								// ���b�V���̒��_�ŏ��l���ւ̃|�C���^
								if (m_aVtxMin == NULL)
								{// ���������m�ۂł����Ԃɂ���
									m_aVtxMin = new D3DXVECTOR3[m_nNumModel];
								}

								// ���b�V���̒��_�ő�l���ւ̃|�C���^
								if (m_aVtxMax == NULL)
								{// ���������m�ۂł����Ԃɂ���
									m_aVtxMax = new D3DXVECTOR3[m_nNumModel];
								}
							}
						}
						if (strcmp(&aString[0], MODEL_FILENAME) == 0)
						{// �ǂݍ��ރ��f���̃t�@�C���p�X������
							if (nCntModel <= m_nNumModel)
							{// �m�ۂ������������I�[�o�[���Ă��Ȃ�
								if (fscanf(pFile, "%s %s", &aString[0], &aXFileName[0]) == 2)
								{// ���f���̃t�@�C���p�X�����ǂݎ�ꂽ
									// x�t�@�C���̓ǂݍ���
									D3DXLoadMeshFromX(aXFileName,
										D3DXMESH_SYSTEMMEM,
										pDevice,
										NULL,
										&m_apBuffMat[nCntModel],
										NULL,
										&m_anNumMat[nCntModel],
										&m_apMesh[nCntModel]);

									if (m_apMesh[nCntModel] != NULL)
									{// ���f�����ǂݍ��߂�
										// �ŏ��l�ƍő�l��������
										m_aVtxMin[nCntModel] = D3DXVECTOR3(10000000.0f, 10000000.0f, 10000000.0f);
										m_aVtxMax[nCntModel] = D3DXVECTOR3(-10000000.0f, -10000000.0f, -10000000.0f);

										int nNumVtx;     // ���_��
										DWORD sizeFVF;   // ���_�t�H�[�}�b�g�̃T�C�Y
										BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^

										// ���_�����l��
										nNumVtx = m_apMesh[nCntModel]->GetNumVertices();

										// ���_�t�H�[�}�b�g�̃T�C�Y���擾
										sizeFVF = D3DXGetFVFVertexSize(m_apMesh[nCntModel]->GetFVF());

										// ���_�o�b�t�@�����b�N
										m_apMesh[nCntModel]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

										for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
										{// ���ׂĂ̒��_���r���ă��f���̍ŏ��l,�ő�l�𔲂��o��
											D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // ���_���W�̑��

											if (Vtx.x < m_aVtxMin[nCntModel].x)
											{// ���o����X���W�̍ŏ��l�����܂ł̂���������
												m_aVtxMin[nCntModel].x = Vtx.x;  // �ŏ��l�X�V
											}
											if (Vtx.y < m_aVtxMin[nCntModel].y)
											{// ���o����Y���W�̍ŏ��l�����܂ł̂���������
												m_aVtxMin[nCntModel].y = Vtx.y;  // �ŏ��l�X�V
											}
											if (Vtx.z < m_aVtxMin[nCntModel].z)
											{// ���o����Z���W�̍ŏ��l�����܂ł̂���������
												m_aVtxMin[nCntModel].z = Vtx.z;  // �ŏ��l�X�V
											}
											if (Vtx.x > m_aVtxMax[nCntModel].x)
											{// ���o����X���W�̍ő�l�����܂ł̂����傫��
												m_aVtxMax[nCntModel].x = Vtx.x;  // �ő�l�X�V
											}
											if (Vtx.y > m_aVtxMax[nCntModel].y)
											{// ���o����Y���W�̍ő�l�����܂ł̂����傫��
												m_aVtxMax[nCntModel].y = Vtx.y;  // �ő�l�X�V
											}
											if (Vtx.z > m_aVtxMax[nCntModel].z)
											{// ���o����Z���W�̍ő�l�����܂ł̂����傫��
												m_aVtxMax[nCntModel].z = Vtx.z;  // �ő�l�X�V
											}

											pVtxBuff += sizeFVF;  // �T�C�Y���|�C���^��i�߂�
										}
										// ���_�o�b�t�@���A�����b�N
										m_apMesh[nCntModel]->UnlockVertexBuffer();
									}

									nCntModel++;   // �ǂݍ��񂾃��f���̐���i�߂�
								}
							}
						}
					}
					fclose(pFile);    // �t�@�C�������
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    �v���C���[�̊J������
//=============================================================================
void CPlayer::UnLoad(void)
{
	// ���b�V���̔j��
	if (m_apMesh != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			if (m_apMesh[nCntModel] != NULL)
			{
				m_apMesh[nCntModel]->Release();
				m_apMesh[nCntModel] = NULL;
			}
		}

		// �������̊J��
		delete[] m_apMesh;
		m_apMesh = NULL;
	}

	// �}�e���A���̔j��
	if (m_apBuffMat != NULL)
	{
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			if (m_apBuffMat[nCntModel] != NULL)
			{
				m_apBuffMat[nCntModel]->Release();
				m_apBuffMat[nCntModel] = NULL;
			}
		}

		// �������̊J��
		delete[] m_apBuffMat;
		m_apBuffMat = NULL;
	}

	// �}�e���A���̐����̔j��
	if (m_anNumMat != NULL)
	{
		// �������̊J��
		delete[] m_anNumMat;
		m_anNumMat = NULL;
	}

	// ���_�̍ŏ��l���̔j��
	if (m_aVtxMin != NULL)
	{
		// �������̊J��
		delete[] m_aVtxMin;
		m_aVtxMin = NULL;
	}

	// ���_�̍ő�l���̔j��
	if (m_aVtxMax != NULL)
	{
		// �������̊J��
		delete[] m_aVtxMax;
		m_aVtxMax = NULL;
	}
}

//=============================================================================
//    �v���C���[�̏���������
//=============================================================================
HRESULT CPlayer::Init(void)
{
	// �e��l�̏����ݒ�
	m_fGravity = PLAYER_GRAVITY_INI;             // �d��
	m_fMaxWalkSpeed = PLAYER_MAX_WALKSPEED_INI;  // �����X�s�[�h�̍ő�l
	m_fJumpPower = PLAYER_JUMPPOWER_INI;         // �W�����v��
	m_fAccel = PLAYER_ACCELERATION_INI;          // �����x
	m_fInertia = PLAYER_INERTIA_INI;             // ����
	m_fRivisionRot = PLAYER_ANGLE_REVISION;      // ������␳����{��
	m_fColHeight = PLAYER_COLHEIGHT_INI;         // �����蔻�����鍂��

	// ���f���̐���
	FILE *pFile = NULL;                                          // �v���C���[�f�[�^�ւ̊O���t�@�C���|�C���^
	char aString[256];                                           // �e�L�X�g�i�[�p�ϐ�(��{�I�ɗv��Ȃ��������i�[����)
	int nIdx = 0;                                                // �g�p����p�[�c�ԍ�
	int nIdxParent = -1;                                         // �e�p�[�c�ւ̔ԍ�
	D3DXVECTOR3 ModelOffsetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �e�p�[�c����̃I�t�Z�b�g�ʒu
	D3DXVECTOR3 ModelOffsetRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �e�p�[�c����̃I�t�Z�b�g����

	// �O���t�@�C������v���C���[�f�[�^��ǂݍ���
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// �e�L�X�g���Ō�̍s�ɓ��B����܂œǂݍ��݂𑱂���
				if (strcmp(&aString[0], CHARACTERSET) == 0)
				{// �L�����N�^�[���̃p�X������
					while (strcmp(&aString[0], END_CHARACTERSET) != 0)
					{// �L�����N�^�[����ǂݍ��ݏI����܂ŌJ��Ԃ�
						fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g�t�@�C����ǂݐi�߂�
						if (strcmp(&aString[0], MOVE) == 0)
						{// �ړ��ʂ̏�񂪂���
							fscanf(pFile, "%s %f", &aString[0], &m_fAccel);
						}
						if (strcmp(&aString[0], MAX_WALKSPEED) == 0)
						{// �����X�s�[�h�̍ő�l�̏�񂪂���
							fscanf(pFile, "%s %f", &aString[0], &m_fMaxWalkSpeed);
						}
						if (strcmp(&aString[0], JUMP) == 0)
						{// �W�����v�͂̏�񂪂���
							fscanf(pFile, "%s %f", &aString[0], &m_fJumpPower);
						}
						if (strcmp(&aString[0], GRAVITY) == 0)
						{// �d�͂̏�񂪂���
							fscanf(pFile, "%s %f", &aString[0], &m_fGravity);
						}
						if (strcmp(&aString[0], INERTIA) == 0)
						{// �����̏�񂪂���
							fscanf(pFile, "%s %f", &aString[0], &m_fInertia);
						}
						if (strcmp(&aString[0], RIVISION_ANGLE) == 0)
						{// ������␳����{���̏�񂪂���
							fscanf(pFile, "%s %f", &aString[0], &m_fRivisionRot);
						}
						if (strcmp(&aString[0], COL_HEIGHT) == 0)
						{// �����蔻�����鍂���̏�񂪂���
							fscanf(pFile, "%s %f", &aString[0], &m_fColHeight);
						}
						if (strcmp(&aString[0], NUM_PARTS) == 0)
						{// �v���C���[���g�p����p�[�c���̏�񂪂���
							fscanf(pFile, "%s %d", &aString[0], &m_nNumPart);
							if (m_apModel == NULL)
							{// ���������m�ۂł����Ԃɂ���
								m_apModel = new CModel*[m_nNumPart];
							}
						}
						if (strcmp(&aString[0], PARTSSET) == 0)
						{// �p�[�c���̃p�X������
							while (strcmp(&aString[0], END_PARTSSET) != 0)
							{// ����ǂݍ��ݏI����܂ŌJ��Ԃ�
								fscanf(pFile, "%s", &aString[0]);  // �e�L�X�g�t�@�C����ǂݐi�߂�
								if (strcmp(&aString[0], INDEX) == 0)
								{// �p�[�c�ԍ��̏�񂪂���
									fscanf(pFile, "%s %d", &aString[0], &nIdx);
								}
								if (strcmp(&aString[0], PARENT) == 0)
								{// �e�̃p�[�c�ԍ��̏�񂪂���
									fscanf(pFile, "%s %d", &aString[0], &nIdxParent);
								}
								if (strcmp(&aString[0], POS) == 0)
								{// �e�p�[�c����̃I�t�Z�b�g�ʒu�̏�񂪂���
									fscanf(pFile, "%s %f %f %f", &aString[0], &ModelOffsetPos.x, &ModelOffsetPos.y, &ModelOffsetPos.z);
								}
								if (strcmp(&aString[0], ROT) == 0)
								{// �e�p�[�c����̃I�t�Z�b�g�����̏�񂪂���
									fscanf(pFile, "%s %f %f %f", &aString[0], &ModelOffsetRot.x, &ModelOffsetRot.y, &ModelOffsetRot.z);
								}
							}
							if (m_apModel != NULL)
							{// ���������m�ۂł���
								m_apModel[nIdx] = CModel::Create(ModelOffsetPos, ModelOffsetRot, m_apMesh[nIdx], m_apBuffMat[nIdx], m_anNumMat[nIdx], m_aVtxMin[nIdx], m_aVtxMax[nIdx]);
								if (nIdxParent > -1)
								{// �e�p�[�c������
									if (m_apModel[nIdxParent] != NULL)
									{// �e�p�[�c�ւ̃|�C���^���m�ۂ���Ă���
										m_apModel[nIdx]->SetParent(m_apModel[nIdxParent]);
									}
								}
							}
						}
					}
				}
			}
			fclose(pFile);    // �t�@�C�������
		}
	}

	// ���[�V�����̐���
	if (m_pMotionManager == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pMotionManager = CMotionManager::Create(STATE_MAX, m_nNumPart, m_aFileName);
	}


	// ���f���̃I�t�Z�b�g�����̐ݒ�
	if (m_apModel != NULL && m_pMotionManager != NULL)
	{// ���������m�ۂł���
		for (int nCntModel = 0; nCntModel < m_nNumPart; nCntModel++)
		{// ���f���𐶐����鐔�����J��Ԃ�
			m_apModel[nCntModel]->SetRot(m_pMotionManager->GetMotion()[m_State]->GetKeyframe()[0][nCntModel]->GetDestRot());   // �����𔽉f
		}
	}

	// �e�̐���
	if (m_pShadow == NULL)
	{// ���������m�ۂł����Ԃł���
		m_pShadow = CShadow::Create(D3DXVECTOR3(m_Pos.x, 0.0f, m_Pos.z), m_Rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f), 20.0f, 5);
	}

	return S_OK;
}

//=============================================================================
//    �v���C���[�̏I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// ���f���̔j��
	if (m_apModel != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntModel = 0; nCntModel < m_nNumPart; nCntModel++)
		{// ���f���𐶐����鐔�����J��Ԃ�
			if (m_apModel[nCntModel] != NULL)
			{// ���������m�ۂ���Ă���
				// ���f���̏I������
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
	{// ���������m�ۂ���Ă���
		// ���[�V�����̏I������
		m_pMotionManager->Uninit();

		// �������̊J��
		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}

	// �e�̔j��
	if (m_pShadow != NULL)
	{// ���������m�ۂ���Ă���
		// �e�̏I������
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	// �v���C���[�I�u�W�F�N�g�̍폜
	Release();
}

//=============================================================================
//    �v���C���[�̍X�V����
//=============================================================================
void CPlayer::Update(void)
{
	// �f�o�b�O���\��
	CDebugProc::Print("\n( �v���C���[������@ )\n");
	CDebugProc::Print("[ �ړ� ]       : [ W ] [ S ] [ A ] [ D ] [ ���A�i���O�X�e�B�b�N ] [ �\���L�[ ]\n");
	CDebugProc::Print("[ �W�����v ]   : [ ENTER ] [ A�{�^�� ]\n");
	CDebugProc::Print("[ �A�N�V���� ] : [ SPACE ] [ X�{�^�� ]\n\n");

	// �|�C���^����ɂ��Ă���
	if (m_pLandScene != NULL)
	{// �A�h���X�����݂���
		m_pLandScene = NULL;
	}

	// �O��̍��W�X�V
	m_PosOld = m_Pos;
	m_OffsetPosAddOld = m_OffsetPosAdd;

	if (m_State != STATE_PANCH && m_State != STATE_ZIPLINE)
	{// �p���`��Ԃł͂Ȃ����W�b�v���C���ɂ��܂��Ă����Ԃł͂Ȃ�
		// �A�N�V��������
		Action();

		// �ړ�����
		Movement();
	}

	if (m_State == STATE_PANCH)
	{
		// ���݂̍��W�Ɉړ��ʂ�������
		m_Pos += m_Move;

		// �d�͂�������
		m_Move.y += m_fGravity;

		// �ړ��ʂɊ�����������
		m_Move.x += (0.0f - m_Move.x) * m_fInertia;
		m_Move.z += (0.0f - m_Move.z) * m_fInertia;
	}

	if (m_State != STATE_ZIPLINE)
	{// �W�b�v���C���ɂ��܂��Ă����Ԃł͂Ȃ�
		// �����蔻�菈��
		Collision();
	}

	// ��ԑJ�ڏ���
	Statement();

	// ���[�V�����̍X�V����
	if (m_pMotionManager != NULL)
	{// ���������m�ۂ���Ă���
		m_pMotionManager->Update(m_apModel);
	}

	// �G�t�F�N�g�̐�������
	CreateEffect();

	// ���̍Đ�
	Sound();

	// �v���C���[�̃f�o�b�O���\��
	CDebugProc::Print("�v���C���[�̌��݂̍��W : ( %.1f %.1f %.1f )\n", m_Pos.x, m_Pos.y, m_Pos.z);
	CDebugProc::Print("�v���C���[�̑O��̍��W : ( %.1f %.1f %.1f )\n", m_PosOld.x, m_PosOld.y, m_PosOld.z);
	CDebugProc::Print("�v���C���[�̌���       : ( %.2f %.2f %.2f )\n", m_Rot.x, m_Rot.y, m_Rot.z);
	CDebugProc::Print("�v���C���[�̈ړ���     : ( %.1f  %.1f  %.1f )\n", m_Move.x, m_Move.y, m_Move.z);

	if (m_bAction == true)
	{
		CDebugProc::Print("�A�N�V�������Ă���\n");
	}
	else
	{
		CDebugProc::Print("�A�N�V�������Ă��Ȃ�\n");
	}

	CDebugProc::Print("�v���C���[�̏��       : ");
	switch (m_State)
	{// ��Ԃɂ���ď����킯
	case STATE_NORMAL:
		CDebugProc::Print("NORMAL\n");
		break;
	case STATE_MOVE_WALK:
		CDebugProc::Print("MOVE_WALK\n");
		break;
	case STATE_MOVE_RUN:
		CDebugProc::Print("MOVE_RUN\n");
		break;
	case STATE_JUMP:
		CDebugProc::Print("JUMP\n");
		break;
	case STATE_LANDING:
		CDebugProc::Print("LANDING\n");
		break;
	case STATE_PUSH:
		CDebugProc::Print("PUSH\n");
		break;
	case STATE_PANCH:
		CDebugProc::Print("PANCH\n");
		break;
	case STATE_KICK:
		CDebugProc::Print("KICK\n");
		break;
	case STATE_ZIPLINE:
		CDebugProc::Print("ZIPLINE\n");
		break;
	}
}

//=============================================================================
//    �v���C���[�̕`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
	        // �v���C���[�̃��[���h�}�g���b�N�X�v�Z����
			CalcMtxWorld(pDevice);

			if (m_apModel != NULL)
			{// ���������m�ۂł���
			    // ���f���̕`��
				for (int nCntModel = 0; nCntModel < m_nNumPart; nCntModel++)
				{// ���f���𐶐����鐔�����J��Ԃ�
					// ���f���̕`�揈��
					m_apModel[nCntModel]->Draw();
				}
			}
		}
	}
}

//=============================================================================
//    �v���C���[�̃��[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CPlayer::CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    �v���C���[�̈ړ�����
//=============================================================================
void CPlayer::Movement(void)
{
	// �ړ��ʌv�Z����
	CGame *pGame = CManager::GetGame();
	if (pGame != NULL)
	{// �Q�[���N���X���擾�ł���
		if (pGame->GetState() != CGame::STATE_PLAYERFALL
			&& pGame->GetState() != CGame::STATE_MAPCLEAR
			&& pGame->GetState() != CGame::STATE_MAPCHANGE
			&& pGame->GetState() != CGame::STATE_END)
		{// �v���C���[�����񂾏�Ԃł͂Ȃ����}�b�v�N���A��Ԃł͂Ȃ����}�b�v�؂�ւ���Ԃł͂Ȃ�
			AddMovement();
		}
	}

	// ���݂̍��W�Ɉړ��ʂ�������
	m_Pos += m_Move;

	// �d�͂�������
	m_Move.y += m_fGravity;

	// �ړ��ʂɊ�����������
	m_Move.x += (0.0f - m_Move.x) * m_fInertia;
	m_Move.z += (0.0f - m_Move.z) * m_fInertia;

	// �p�x�̏C��
	m_DiffAngle.y = m_DestAngle.y - m_Rot.y;   // ���݂̌����ƖړI�̌����̍������v�Z

	if (m_DiffAngle.y > D3DX_PI)
	{// ������D3DX_PI�𒴂���
		m_DiffAngle.y -= D3DX_PI * 2.0f;
	}
	if (m_DiffAngle.y < -D3DX_PI)
	{// ������-D3DX_PI�𒴂���
		m_DiffAngle.y += D3DX_PI * 2.0f;
	}

	// ���݂̌����ɖړI�̌����Ƃ̍�����{���ŕ␳����
	m_Rot.y += m_DiffAngle.y * m_fRivisionRot;

	if (m_Rot.y > D3DX_PI)
	{// ���݂̌�����D3DX_PI�𒴂���
		m_Rot.y -= D3DX_PI * 2.0f;
	}
	if (m_Rot.y < -D3DX_PI)
	{// ���݂̌�����-D3DX_PI�𒴂���
		m_Rot.y += D3DX_PI * 2.0f;
	}
}

//=============================================================================
//    �v���C���[�̈ړ��ʌv�Z����
//=============================================================================
void CPlayer::AddMovement(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �W���C�p�b�h���擾
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	// �J�������擾
	CCamera *pCamera = CManager::GetCamera();

	if (pKeyboard != NULL)
	{// �L�[�{�[�h���擾�ł���
		if (pKeyboard->GetPress(DIK_A) == true)
		{// �������̓��͂����ꂽ
			if (pKeyboard->GetPress(DIK_W) == true)
			{// �����ɏ�����̓��͂����ꂽ
			    // �ړ��ʌv�Z����
				m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * (m_fAccel);
				m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * (m_fAccel);

				// �ړI�̊p�x�ύX
				m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
				return;
			}
			else if (pKeyboard->GetPress(DIK_S) == true)
			{// �����ɉ������̓��͂����ꂽ
			    // �ړ��ʌv�Z����
				m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * (m_fAccel);
				m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * (m_fAccel);

				// �ړI�̊p�x�ύX
				m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
				return;
			}
			else
			{// ����������ĂȂ�
			    // �ړ��ʌv�Z����
				m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * (m_fAccel);
				m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * (m_fAccel);

				// �ړI�̊p�x�ύX
				m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
				return;
			}
		}
		else if (pKeyboard->GetPress(DIK_D) == true)
		{// �E�����̓��͂����ꂽ
			if (pKeyboard->GetPress(DIK_W) == true)
			{// �����ɏ�����̓��͂����ꂽ
			    // �ړ��ʌv�Z����
				m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * (m_fAccel);
				m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * (m_fAccel);

				// �ړI�̊p�x�ύX
				m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * -0.75f);
				return;
			}
			else if (pKeyboard->GetPress(DIK_S) == true)
			{// �����ɉ������̓��͂����ꂽ
			    // �ړ��ʌv�Z����
				m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * (m_fAccel);
				m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * (m_fAccel);

				// �ړI�̊p�x�ύX
				m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * -0.25f);
				return;
			}
			else
			{// ����������ĂȂ�
			    // �ړ��ʌv�Z����
				m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * (m_fAccel);
				m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * (m_fAccel);

				// �ړI�̊p�x�ύX
				m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * -0.5f);
				return;
			}
		}
		else if (pKeyboard->GetPress(DIK_W) == true)
		{// ������̓��͂����ꂽ
		    // �ړ��ʌv�Z����
			m_Move.x += sinf(pCamera->GetRot().y) * (m_fAccel);
			m_Move.z += cosf(pCamera->GetRot().y) * (m_fAccel);

			// �ړI�̊p�x�ύX
			m_DestAngle.y = pCamera->GetRot().y + D3DX_PI;
			return;
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// �������̓��͂����ꂽ
	     	// �ړ��ʌv�Z����
			m_Move.x += sinf(pCamera->GetRot().y + D3DX_PI) * (m_fAccel);
			m_Move.z += cosf(pCamera->GetRot().y + D3DX_PI) * (m_fAccel);

			// �ړI�̊p�x�ύX
			m_DestAngle.y = pCamera->GetRot().y;
			return;
		}
	}
	if (pJoyStick != NULL)
	{// �W���C�X�e�B�b�N���擾�ł���
		if (pJoyStick->GetLeftAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisX() <= -DI_STICK_AXIS_MIN
			|| pJoyStick->GetLeftAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisY() <= -DI_STICK_AXIS_MIN)
		{// ���A�i���O���͂�����Ă���
			// �X�e�B�b�N�̓|���ꂽ�����̔䗦���v�Z
			float fLengthX = (float)(sqrt(pJoyStick->GetLeftAxisX() * pJoyStick->GetLeftAxisX() + pJoyStick->GetLeftAxisY() * pJoyStick->GetLeftAxisY()) / DI_JOY_STICK_INPUT);

			// �R�T�C���J�[�u�Ŕ䗦��␳
			float fRivisionX = cosf(sqrtf(2) - fLengthX);
			if (fRivisionX < 1.0)
			{// �{����1�𒴂��Ă��Ȃ�
				fRivisionX += 0.09f;  // ����Ȃ�����␳
				if (fRivisionX >= 1.0f)
				{// �{����1�𒴂��Ă��܂���
					fRivisionX = 1.0f;
				}
			}

		    // �ړ��ʌv�Z����
			m_Move.x += sinf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (m_fAccel * fRivisionX);
			m_Move.z -= cosf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (m_fAccel * fRivisionX);

			// �ړI�̊p�x�ύX
			m_DestAngle.y = pCamera->GetRot().y - pJoyStick->GetLeftAxis();
			return;
		}
		else if (pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_28) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_29) == true
			|| pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_30) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_31) == true)
		{// �\���L�[���͂�����Ă���
		    // �ړ��ʌv�Z����
			m_Move.x += sinf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (m_fAccel);
			m_Move.z += cosf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (m_fAccel);

			// �ړI�̊p�x�ύX
			m_DestAngle.y = pCamera->GetRot().y + (pJoyStick->GetRadian() - D3DX_PI);
			return;
		}
	}
}

//=============================================================================
//    �v���C���[�̓����蔻�菈������
//=============================================================================
bool CPlayer::Collision(void)
{
	bool bLand = false;    // ����Ă��邩�ǂ���
	bool bPush = false;    // ���łɉ����Ă��邩�ǂ���
	bool bCol = false;     // �I�u�W�F�N�g�ɓ������Ă��邩�ǂ���

	// �t�B�[���h�Ɣz�u�����擾����
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	CField *pField = NULL;               // �n�ʃN���X�ւ̃|�C���^
	CWater *pWater = NULL;               // ���N���X�ւ̃|�C���^
	CObject *pObject = NULL;             // �z�u���N���X�ւ̃|�C���^
	CObjectMove *pObjectMove = NULL;     // �����z�u���N���X�ւ̃|�C���^
	CObjectGoal *pObjectGoal = NULL;     // �S�[���z�u���N���X�ւ̃|�C���^
	CObjectTarget *pObjectTarget = NULL; // �W�b�v���C���z�u���N���X�ւ̃|�C���^

	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetScene(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_FIELD)
			{// �t�B�[���h��������
				pField = (CField*)pScene;
				if (pField != NULL)
				{// �t�B�[���h���擾�ł���
					D3DXVECTOR3 PolygonNormal;
					float fFieldHeight = pField->GetPolyHeight(m_Pos, &PolygonNormal);  // �|���S���̍������擾
					if (m_Pos.y <= fFieldHeight)
					{// �|���S���̍������Ⴍ�Ȃ���
						m_Pos.y = fFieldHeight;
						m_Move.y = 0.0f;
						if (m_bJump == true)
						{// �W�����v���Ă�����
							m_bJump = false;

							// ���n�����Ƃ��̏���
							Landing();
						}
					}
					if (m_pShadow != NULL)
					{// �e����������Ă���
						if (m_pShadow->GetPos() == m_pShadow->GetPosOld())
						{// �e�̈ʒu�X�V������Ă���
							m_pShadow->SetPos(D3DXVECTOR3(m_Pos.x, fFieldHeight + 1.0f, m_Pos.z));
							// ���_���̐ݒ�
							VERTEX_3D *pVtx;
							LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pShadow->GetVtxBuff();
							// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
							pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

							for (int nCntVer = 0; nCntVer < 4; nCntVer++)
							{// �e�̒��_�����J��Ԃ�
								float DotX = (PolygonNormal.x * pVtx[nCntVer].pos.x);       // X�����̓��ς��v�Z
								float DotZ = (PolygonNormal.z * pVtx[nCntVer].pos.z);       // Z�����̓��ς��v�Z
								float Dot = -DotX - DotZ;                                   // X������Z�����̓��ϓ��m������
								pVtx[nCntVer].pos.y = (Dot / PolygonNormal.y);              // ���������̂�Y�����̖@���Ŋ���
							}

							// ���_�o�b�t�@���A�����b�N����
							pVtxBuff->Unlock();

							// ���_�o�b�t�@�̐ݒ�
							m_pShadow->SetVtxBuff(pVtxBuff);
						}
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_WATER)
			{// ����������
				pWater = (CWater*)pScene;
				if (pWater != NULL)
				{// �����擾�ł���
					D3DXVECTOR3 PolygonNormal;
					float fWaterHeight = pWater->GetPolyHeight(m_Pos, &PolygonNormal);  // �|���S���̍������擾
					if (m_Pos.y + m_fColHeight <= fWaterHeight)
					{// �|���S���̍������Ⴍ�Ȃ���
						CGame *pGame = CManager::GetGame();
						if (pGame != NULL)
						{// �Q�[���N���X���擾�ł���
							if (pGame->GetState() != CGame::STATE_PLAYERFALL)
							{// �v���C���[����������Ԃł͂Ȃ�
								pWater->SetState(CWater::STATE_RIPPLE);
								pWater->SetRipplesHeight(80.0f);
								pWater->SetRipplePos(D3DXVECTOR3(m_Pos.x, m_Pos.y + m_fColHeight, m_Pos.z));
								pGame->SetState(CGame::STATE_PLAYERFALL);
								m_pShadow->SetPos(D3DXVECTOR3(m_Pos.x, -400.0f + 1.0f, m_Pos.z));
								CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_WATER);
							}
						}
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// �z�u����������
				pObject = (CObject*)pScene;

				// �Ԃ��󂵔���
				if (pObject != NULL)
				{// �|�C���^���m�ۂł��Ă���
					if (pObject->GetBreak() == true)
					{// �󂹂�I�u�W�F�N�g��������
						if (AttackCollision(pObject->GetPos(), pObject->GetVtxMin(), pObject->GetVtxMax()) == true)
						{// �U�����q�b�g����
							// �z�u����j������
							pObject->Uninit();
							pObject = NULL;

							D3DXVECTOR3 EffectOffSetPos = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
							D3DXMATRIX EffectMtxWorld;

							D3DXMATRIX mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

							// ���[���h�}�g���b�N�X�̏�����
							D3DXMatrixIdentity(&EffectMtxWorld);

							// ��]�𔽉f
							D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
							D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &mtxRot);

							// �ʒu�𔽉f
							D3DXMatrixTranslation(&mtxTrans, EffectOffSetPos.x, EffectOffSetPos.y, EffectOffSetPos.z);
							D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &mtxTrans);

							// �e�̏��𔽉f
							D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &m_apModel[PLAYER_PANCH_COLINDEX]->GetMtxWorld());

							CParEmitter::Create(D3DXVECTOR3(EffectMtxWorld._41, EffectMtxWorld._42, EffectMtxWorld._43), 1, 6);


							CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_BREAK);
						}
					}
				}

				if (pObject != NULL)
				{// �z�u�����擾�ł���
					if (pObject->GetCollision() == true)
					{// �z�u���Ɠ����蔻������Ȃ��
						// �z�u���Ƃ̔���J�n
						if (pObject->Collision(&m_Pos, &m_PosOld, &m_Move, m_pShadow, m_fColHeight, PLAYER_COL_RANGE) == true)
						{// �z�u���̏�ɏ���Ă���
							if (m_pLandScene == NULL && m_bLand == true && m_bJump == false)
							{// �����ɏ���Ă���
								m_Pos.y = pObject->GetPos().y + pObject->GetVtxMax().y;

								// �e�̈ʒu�����炷
								if (m_pShadow != NULL)
								{// �e����������Ă���
									m_pShadow->SetPos(D3DXVECTOR3(m_Pos.x, pObject->GetPos().y + pObject->GetVtxMax().y + 1.0f, m_Pos.z));
								}
							}
							m_pLandScene = pObject;
							if (m_bLand == false)
							{// �O�����Ă��Ȃ�����
								if (m_bJump == true)
								{// �W�����v���Ă�����
									m_bJump = false;

									// ���n�����Ƃ��̏���
									Landing();
								}
								D3DXVECTOR3 ObjRot = pObject->GetRot();                                                           // ������ۑ�
								m_OffsetPosStd = D3DXVECTOR3(m_Pos.x - pObject->GetPos().x, 0.0f, m_Pos.z - pObject->GetPos().z); // �I�t�Z�b�g���W��ۑ�
								m_OffsetRot = m_Rot - ObjRot;                                                                     // �v���C���[�̃I�t�Z�b�g������ۑ�
								if (m_OffsetRot.y > D3DX_PI)
								{// �~�����𒴂���
									m_OffsetRot.y -= D3DX_PI * 2.0f;
								}
								if (m_OffsetRot.y < -D3DX_PI)
								{// �~�����𒴂���
									m_OffsetRot.y += D3DX_PI * 2.0f;
								}
								m_OffsetRotStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �I�u�W�F�N�g�̃X�^�[�g������������
								m_OffsetPosAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // �I�t�Z�b�g�ɉ�����l��������
								m_bLand = true;                                      // �I�u�W�F�N�g�ɏ���Ă��锻��ɂ���
							}
							if (m_pLandScene != NULL)
							{// �z�u���N���X���擾�ł���
								// �������C������
								if (m_bMove == false)
								{// �ړ����Ă��Ȃ�������
									D3DXVECTOR3 ObjRot = pObject->GetRot();
									m_Rot = m_OffsetRot + ObjRot;    // ������␳
									if (m_Rot.y > D3DX_PI)
									{// �~�����𒴂���
										m_Rot.y -= D3DX_PI * 2.0f;
									}
									if (m_Rot.y < -D3DX_PI)
									{// �~�����𒴂���
										m_Rot.y += D3DX_PI * 2.0f;
									}
									m_DestAngle.y = m_Rot.y;  // �ړI�̌������␳
								}

								// �I�t�Z�b�g�̌����ɔz�u���̉�]�X�s�[�h�����Z����
								m_OffsetRotStart.y += pObject->GetRollSpeed().y;  // �I�u�W�F�N�g����]����X�s�[�h�ɍ��킹�ĕ␳
								if (m_OffsetRotStart.y > D3DX_PI)
								{// �~�����𒴂���
									m_OffsetRotStart.y -= D3DX_PI * 2.0f;
								}
								else if (m_OffsetRotStart.y < -D3DX_PI)
								{// �~�����𒴂���
									m_OffsetRotStart.y += D3DX_PI * 2.0f;
								}
								// �I�t�Z�b�g�ʒu��Y����]�ɉ��킹���ړ��ʂ��v�Z����
								if (m_State != STATE_PANCH && m_State != STATE_LANDING)
								{
									m_OffsetPosAdd.x += cosf(0.0f) * (cosf(m_OffsetRotStart.y) * m_Move.x + -sinf(m_OffsetRotStart.y) * m_Move.z) + sinf(0.0f) * (sinf(m_OffsetRotStart.y) * m_Move.x + cosf(m_OffsetRotStart.y) * m_Move.z);
									m_OffsetPosAdd.z += -sinf(0.0f) * (cosf(m_OffsetRotStart.y) * m_Move.x + -sinf(m_OffsetRotStart.y) * m_Move.z) + cosf(0.0f) * (sinf(m_OffsetRotStart.y) * m_Move.x + cosf(m_OffsetRotStart.y) * m_Move.z);
								}

								// �ʒu��Y����]�ɉ��킹�Ĉړ�������
								float fMtxX = cosf(m_OffsetRotStart.y) * (m_OffsetPosStd.x + (m_OffsetPosAdd.x * 1.5f)) + sinf(m_OffsetRotStart.y) * (m_OffsetPosStd.z + (m_OffsetPosAdd.z * 1.5f));
								float fMtxZ = -sinf(m_OffsetRotStart.y) * (m_OffsetPosStd.x + (m_OffsetPosAdd.x * 1.5f)) + cosf(m_OffsetRotStart.y) * (m_OffsetPosStd.z + (m_OffsetPosAdd.z * 1.5f));
								m_Pos.x = fMtxX + pObject->GetPos().x;
								m_Pos.z = fMtxZ + pObject->GetPos().z;

								// �f�o�b�O���\��
								CDebugProc::Print("�I�t�Z�b�g�̊�ʒu   : ( %.1f  %.1f  %.1f )\n", m_OffsetPosStd.x, m_OffsetPosStd.y, m_OffsetPosStd.z);
								CDebugProc::Print("�I�t�Z�b�g�ɉ�����ʒu : ( %.1f  %.1f  %.1f )\n", m_OffsetPosAdd.x, m_OffsetPosAdd.y, m_OffsetPosAdd.z);
								CDebugProc::Print("�I�t�Z�b�g�̌���       : ( %.1f  %.1f  %.1f )\n", m_OffsetRot.x, m_OffsetRot.y, m_OffsetRot.z);
								CDebugProc::Print("����Ă���̉�]�l     : ( %.1f  %.1f  %.1f )\n", m_OffsetRotStart.x, m_OffsetRotStart.y, m_OffsetRotStart.z);
								bLand = true;
							}
							CDebugProc::Print("�z�u���ɏ���Ă��܂�\n\n");
						}
						else
						{// �I�u�W�F�N�g�ɏ���Ă��Ȃ�
							if (pObject->Collision(&m_Pos, &m_PosOld, &m_Move, m_fColHeight, PLAYER_COL_RANGE) == true)
							{// �I�u�W�F�N�g�ɓ������Ă���
								bCol = true;
								D3DXVECTOR3 ObjPos;
								if (m_pLandScene != NULL)
								{// ����Ă���I�u�W�F�N�g������
									if (m_pLandScene->GetObjType() == OBJTYPE_OBJECT)
									{// �z�u���̃N���X��������
										CObject *pLandObject = (CObject*)m_pLandScene;
										ObjPos = pLandObject->GetPos();
									}
									if (m_pLandScene->GetObjType() == OBJTYPE_OBJECTMOVE)
									{// �����z�u���̃N���X��������
										CObjectMove *pLandObject = (CObjectMove*)m_pLandScene;
										ObjPos = pLandObject->GetPos();
									}
									m_OffsetPosStd = D3DXVECTOR3(m_Pos.x - ObjPos.x, 0.0f, m_Pos.z - ObjPos.z); // �I�t�Z�b�g���W��ۑ�
									m_OffsetPosAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
									m_OffsetPosAddOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								}
								CDebugProc::Print("�z�u���ɓ������Ă��܂�\n");
								if (m_bJump != true)
								{// �W�����v���Ă��Ȃ�
									if (pObject->GetPush() == true)
									{// ������I�u�W�F�N�g�ł���
										if (m_State != STATE_PUSH)
										{// �������[�V�����ɂȂ��Ă��Ȃ�
											// �����������Ă����Ԃ�
											m_State = STATE_PUSH;

											// ���[�V�����؂�ւ�����
											m_pMotionManager->SwitchMotion(m_apModel, m_State);
											bPush = true;
										}
									}
								}
							}
						}
					}
				}
			}
			else if (pScene->GetObjType() == OBJTYPE_OBJECTMOVE)
			{// �����z�u����������
				pObjectMove = (CObjectMove*)pScene;
				if (pObjectMove != NULL)
				{// �����z�u�����擾�ł���
					if (pObjectMove->GetCollision() == true)
					{// �����z�u���Ɠ����蔻������Ȃ��
						// �����z�u���̔���J�n
						if (pObjectMove->Collision(&m_Pos, &m_PosOld, &m_Move, m_pShadow, m_fColHeight, PLAYER_COL_RANGE) == true)
						{// �����z�u���̒��ɂ���
							if (m_pLandScene == NULL && m_bLand == true && m_bJump == false)
							{// �����ɏ���Ă���
								m_Pos.y = pObjectMove->GetPos().y + pObjectMove->GetVtxMax().y;
								// �e�̈ʒu�����炷
								if (m_pShadow != NULL)
								{// �e����������Ă���
									m_pShadow->SetPos(D3DXVECTOR3(m_Pos.x, m_Pos.y + 1.0f, m_Pos.z));
								}
							}
							m_pLandScene = pObjectMove;
							if (m_bLand == false)
							{// �O�����Ă��Ȃ�����
								if (m_bJump == true)
								{// �W�����v���Ă�����
									m_bJump = false;

									// ���n�����Ƃ��̏���
									Landing();
								}
								D3DXVECTOR3 ObjRot = pObjectMove->GetRot();                                                                // ������ۑ�
								m_OffsetPosStd = D3DXVECTOR3(m_Pos.x - pObjectMove->GetPos().x, 0.0f, m_Pos.z - pObjectMove->GetPos().z);  // �I�t�Z�b�g���W��ۑ�
								m_OffsetRot = m_Rot - ObjRot;                                                                              // �v���C���[�̃I�t�Z�b�g������ۑ�
								if (m_OffsetRot.y > D3DX_PI)
								{// �~�����𒴂���
									m_OffsetRot.y -= D3DX_PI * 2.0f;
								}
								if (m_OffsetRot.y < -D3DX_PI)
								{// �~�����𒴂���
									m_OffsetRot.y += D3DX_PI * 2.0f;
								}
								m_OffsetRotStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �I�u�W�F�N�g�̃X�^�[�g������������
								m_OffsetPosAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // �I�t�Z�b�g�ɉ�����l��������
								m_bLand = true;                                      // �I�u�W�F�N�g�ɏ���Ă��锻��ɂ���
								m_Pos.y = pObjectMove->GetPos().y + pObjectMove->GetVtxMax().y;
							}

							if (m_pLandScene != NULL)
							{// �����z�u���N���X���擾�ł���
								// �������C������
								if (m_bMove == false)
								{// �ړ����Ă��Ȃ�������
									D3DXVECTOR3 ObjRot = pObjectMove->GetRot();
									m_Rot = m_OffsetRot + ObjRot;    // ������␳
									if (m_Rot.y > D3DX_PI)
									{// �~�����𒴂���
										m_Rot.y -= D3DX_PI * 2.0f;
									}
									if (m_Rot.y < -D3DX_PI)
									{// �~�����𒴂���
										m_Rot.y += D3DX_PI * 2.0f;
									}
									m_DestAngle.y = m_Rot.y;  // �ړI�̌������␳
								}

								// �I�t�Z�b�g�̌����ɔz�u���̉�]�X�s�[�h�����Z����
								m_OffsetRotStart.y += pObjectMove->GetRollSpeed().y;  // �I�u�W�F�N�g����]����X�s�[�h�ɍ��킹�ĕ␳
								if (m_OffsetRotStart.y > D3DX_PI)
								{// �~�����𒴂���
									m_OffsetRotStart.y -= D3DX_PI * 2.0f;
								}
								else if (m_OffsetRotStart.y < -D3DX_PI)
								{// �~�����𒴂���
									m_OffsetRotStart.y += D3DX_PI * 2.0f;
								}
								// �I�t�Z�b�g�ʒu��Y����]�ɉ��킹���ړ��ʂ��v�Z����
								if (m_State != STATE_PANCH && m_State != STATE_LANDING)
								{
									m_OffsetPosAdd.x += cosf(0.0f) * (cosf(m_OffsetRotStart.y) * m_Move.x + -sinf(m_OffsetRotStart.y) * m_Move.z) + sinf(0.0f) * (sinf(m_OffsetRotStart.y) * m_Move.x + cosf(m_OffsetRotStart.y) * m_Move.z);
									m_OffsetPosAdd.z += -sinf(0.0f) * (cosf(m_OffsetRotStart.y) * m_Move.x + -sinf(m_OffsetRotStart.y) * m_Move.z) + cosf(0.0f) * (sinf(m_OffsetRotStart.y) * m_Move.x + cosf(m_OffsetRotStart.y) * m_Move.z);
								}

								// �ʒu��Y����]�ɉ��킹�Ĉړ�������
								float fMtxX = cosf(m_OffsetRotStart.y) * (m_OffsetPosStd.x + (m_OffsetPosAdd.x * 1.5f)) + sinf(m_OffsetRotStart.y) * (m_OffsetPosStd.z + (m_OffsetPosAdd.z * 1.5f));
								float fMtxZ = -sinf(m_OffsetRotStart.y) * (m_OffsetPosStd.x + (m_OffsetPosAdd.x * 1.5f)) + cosf(m_OffsetRotStart.y) * (m_OffsetPosStd.z + (m_OffsetPosAdd.z * 1.5f));
								m_Pos.x = fMtxX + pObjectMove->GetPos().x;
								m_Pos.z = fMtxZ + pObjectMove->GetPos().z;

								// �f�o�b�O���\��
								CDebugProc::Print("�I�t�Z�b�g�̊�ʒu   : ( %.1f  %.1f  %.1f )\n", m_OffsetPosStd.x, m_OffsetPosStd.y, m_OffsetPosStd.z);
								CDebugProc::Print("�I�t�Z�b�g�ɉ�����ʒu : ( %.1f  %.1f  %.1f )\n", m_OffsetPosAdd.x, m_OffsetPosAdd.y, m_OffsetPosAdd.z);
								CDebugProc::Print("�I�t�Z�b�g�̌���       : ( %.1f  %.1f  %.1f )\n", m_OffsetRot.x, m_OffsetRot.y, m_OffsetRot.z);
								CDebugProc::Print("����Ă���̉�]�l     : ( %.1f  %.1f  %.1f )\n", m_OffsetRotStart.x, m_OffsetRotStart.y, m_OffsetRotStart.z);
								bLand = true;
							}
							CDebugProc::Print("�����z�u���ɏ���Ă��܂�\n\n");
						}
					}
				}
			}

			// �S�[���p�z�u��
			else if (pScene->GetObjType() == OBJTYPE_OBJECTGOAL)
			{// �S�[���p�z�u����������
				pObjectGoal = (CObjectGoal*)pScene;
				if (pObjectGoal != NULL)
				{// �S�[���p�̔z�u�����擾�ł���
					if (pObjectGoal->GetCollision() == true)
					{// �S�[���p�̔z�u���Ɠ����蔻������Ȃ��
						if (pObjectGoal->Collision(&m_Pos, &m_PosOld, &m_Move, m_fColHeight, PLAYER_COL_RANGE) == true)
						{// �S�[���ɐG�ꂽ
							D3DXVECTOR3 ObjPos;
							if (m_pLandScene != NULL)
							{// ����Ă���I�u�W�F�N�g������
								if (m_pLandScene->GetObjType() == OBJTYPE_OBJECT)
								{// �z�u���̃N���X��������
									CObject *pLandObject = (CObject*)m_pLandScene;
									ObjPos = pLandObject->GetPos();
								}
								if (m_pLandScene->GetObjType() == OBJTYPE_OBJECTMOVE)
								{// �����z�u���̃N���X��������
									CObjectMove *pLandObject = (CObjectMove*)m_pLandScene;
									ObjPos = pLandObject->GetPos();
								}
								m_OffsetPosStd = D3DXVECTOR3(m_Pos.x - ObjPos.x, 0.0f, m_Pos.z - ObjPos.z); // �I�t�Z�b�g���W��ۑ�
								m_OffsetPosAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
								m_OffsetPosAddOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
							}

							CGame *pGame = CManager::GetGame();
							if (pGame != NULL)
							{// �Q�[���N���X���擾�ł���
								if (pGame->GetMapClear() == false)
								{// �}�b�v���N���A��Ԃł���
									if (pGame->GetState() != CGame::STATE_MAPCLEAR)
									{// �}�b�v�N���A��ԂɂȂ��Ă��Ȃ�
										pGame->SetMapClear(true);
										pGame->SetState(CGame::STATE_MAPCLEAR);
									}
								}
							}
						}
					}
				}
			}


			// �W�b�v���C���p�z�u��
			else if (pScene->GetObjType() == OBJTYPE_OBJECTTARGET)
			{// �S�[���p�z�u����������
				pObjectTarget = (CObjectTarget*)pScene;
				if (pObjectTarget != NULL)
				{// �|�C���^���擾�ł��Ă���
					if (pObjectTarget->GetCollision() == true)
					{// �����蔻������Ȃ��
						if (m_State != STATE_ZIPLINE)
						{// �W�b�v���C���ɂ��܂��Ă��郂�[�V�����ɂȂ��Ă��Ȃ�
							if (pObjectTarget->Collision(&m_Pos, &m_PosOld, &m_Move, m_pShadow, m_fColHeight, PLAYER_COL_RANGE, this) == true)
							{// ���܂��Ă���
								// �W�b�v���C���ɂ��܂��Ă����Ԃ�
								m_State = STATE_ZIPLINE;

								// ���[�V�����؂�ւ�����
								m_pMotionManager->SwitchMotion(m_apModel, m_State);
								bPush = true;
							}
						}
					}
				}
			}
			pScene = pSceneNext;  // ���̃|�C���^��ݒ�
		}
	}

	if (bLand == false)
	{// ��������Ă��Ȃ�����
		m_bLand = false;  // ����Ă��Ȃ�����ɂ���
	}
	m_bPush = bPush;
	m_bCol = bCol;

	return bLand;   // ��������ǂ�����Ԃ�
}

//=============================================================================
//    �v���C���[�̃A�N�V��������
//=============================================================================
void CPlayer::Action(void)
{
	CGame *pGame = CManager::GetGame();
	if (pGame != NULL)
	{// �Q�[���N���X���擾�ł���
		if (pGame->GetState() != CGame::STATE_PLAYERFALL
			&& pGame->GetState() != CGame::STATE_MAPCLEAR
			&& pGame->GetState() != CGame::STATE_MAPCHANGE
			&& pGame->GetState() != CGame::STATE_END)
		{// �v���C���[�����񂾏�Ԃł͂Ȃ����}�b�v�N���A��Ԃł͂Ȃ����}�b�v�؂�ւ���Ԃł͂Ȃ�
	        // �L�[�{�[�h���擾
			CInputKeyboard *pKeyboard = CManager::GetKeyboard();

			// �W���C�p�b�h���擾
			CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

			// XInput���擾
			CXInput *pXInput = CManager::GetXInput();

			if (pKeyboard->GetTrigger(DIK_RETURN) == true
				|| pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
			{// �W�����v�{�^�������͂��ꂽ
				if (m_bJump == false && m_Move.y >= 0.0f)
				{// �W�����v���Ă��Ȃ�
					m_Move.y += m_fJumpPower;
					m_bJump = true;

					// ��Ԃ��W�����v��Ԃ�
					m_State = STATE_JUMP;

					// ���[�V�����؂�ւ�����
					m_pMotionManager->SwitchMotion(m_apModel, m_State);

					// �����Đ�����
					CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_JUMP);
				}
			}

			if (pKeyboard->GetTrigger(DIK_SPACE) == true
				|| pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_0) == true)
			{// �U���{�^�������͂��ꂽ
				if (m_bAction == false)
				{// �A�N�V������Ԃł͂Ȃ�
					m_bAction = true;
					if (m_bJump == false)
					{// �W�����v���Ă��Ȃ�
						if (m_State != STATE_PANCH)
						{// �p���`��Ԃł͂Ȃ�
							// �ړ��ʂ��Ȃ���
							m_Move.x = 0.0f;
							m_Move.z = 0.0f;

							// ��Ԃ��p���`��Ԃ�
							m_State = STATE_PANCH;

							// ���[�V�����؂�ւ�����
							m_pMotionManager->SwitchMotion(m_apModel, m_State);
						}
					}
					else
					{// �W�����v���Ă���
						if (m_State != STATE_KICK)
						{// �L�b�N��Ԃł͂Ȃ�
							// ��Ԃ��L�b�N��Ԃ�
							m_State = STATE_KICK;

							// ���[�V�����؂�ւ�����
							m_pMotionManager->SwitchMotion(m_apModel, m_State);
						}
					}
				}
			}
		}
	}
}

//=============================================================================
//    �v���C���[�̏�ԑJ�ڏ���
//=============================================================================
void CPlayer::Statement(void)
{
	if (m_bJump != true && m_State != STATE_LANDING && m_State != STATE_PUSH && m_bAction != true && m_bPush != true)
	{// �W�����v���Ă��Ȃ������n��Ԃł͂Ȃ����A�N�V������Ԃł͂Ȃ�
		if (m_Move.x <= 0.1f && m_Move.x >= -0.1f
			&& m_Move.z <= 0.1f && m_Move.z >= -0.1f)
		{// �ړ����Ă��Ȃ�
			m_bMove = false;
			if (m_State != STATE_NORMAL)
			{// �ʏ��Ԃł͂Ȃ�
				// ��Ԃ�ʏ�̏�Ԃ�
				m_State = STATE_NORMAL;

				if (m_bLand == true)
				{// �I�t�Z�b�g�ʒu�����[���h�}�g���b�N�X���g���Ċ���o��
					if (m_pLandScene != NULL)
					{// �v���C���[������Ă���N���X�ւ̃|�C���^���擾�ł��Ă���
						if (m_pLandScene->GetObjType() == OBJTYPE_OBJECT)
						{// �z�u���N���X��������
							CObject *pObject = (CObject*)m_pLandScene;
							D3DXVECTOR3 ObjRot = pObject->GetRot();
							m_OffsetRot = m_Rot - ObjRot;
							if (m_OffsetRot.y > D3DX_PI)
							{// �~�����𒴂���
								m_OffsetRot.y -= D3DX_PI * 2.0f;
							}
							if (m_OffsetRot.y < -D3DX_PI)
							{// �~�����𒴂���
								m_OffsetRot.y += D3DX_PI * 2.0f;
							}
						}
						else if (m_pLandScene->GetObjType() == OBJTYPE_OBJECTMOVE)
						{// �����z�u���N���X��������
							CObjectMove *pObjectMove = (CObjectMove*)m_pLandScene;
							D3DXVECTOR3 ObjRot = pObjectMove->GetRot();
							m_OffsetRot = m_Rot - ObjRot;
							if (m_OffsetRot.y > D3DX_PI)
							{// �~�����𒴂���
								m_OffsetRot.y -= D3DX_PI * 2.0f;
							}
							if (m_OffsetRot.y < -D3DX_PI)
							{// �~�����𒴂���
								m_OffsetRot.y += D3DX_PI * 2.0f;
							}
						}
					}
				}

				// ���[�V�����؂�ւ�����
				m_pMotionManager->SwitchMotion(m_apModel, m_State);
			}
		}
		else
		{// �ړ����Ă���
		    // �ړ��ʂ̒������v�Z
			float fMoveLength = sqrtf((m_Move.x * m_Move.x) + (m_Move.z * m_Move.z));

			if (fMoveLength >= PLAYER_MAX_WALKSPEED)
			{// �ړ��ʂ�����̒l���傫��
				if (m_State != STATE_MOVE_RUN)
				{// �����Ԃł͂Ȃ�
					m_State = STATE_MOVE_RUN;

					// ���[�V�����؂�ւ�����
					m_pMotionManager->SwitchMotion(m_apModel, m_State);
				}
			}
			else
			{// �X�e�B�b�N�̎�������̒l��菬����
				if (m_State != STATE_MOVE_WALK)
				{// ������Ԃł͂Ȃ�
					m_State = STATE_MOVE_WALK;

					// ���[�V�����؂�ւ�����
					m_pMotionManager->SwitchMotion(m_apModel, m_State);
				}
			}

			if (m_bMove == false)
			{// �ړ����Ă����Ԃł͂Ȃ�
				// �ړ����Ă����Ԃ�
				m_bMove = true;
			}
		}
	}
	else
	{// �W�����v���Ă��邩�A���n��Ԃł���
		if (m_bLand == true)
		{// �I�t�Z�b�g�ʒu�����[���h�}�g���b�N�X���g���Ċ���o��
			if (m_pLandScene != NULL)
			{// �v���C���[������Ă���N���X�ւ̃|�C���^���擾�ł��Ă���
				if (m_pLandScene->GetObjType() == OBJTYPE_OBJECT)
				{// �z�u���N���X��������
					CObject *pObject = (CObject*)m_pLandScene;
					D3DXVECTOR3 ObjRot = pObject->GetRot();
					m_OffsetRot = m_Rot - ObjRot;
					if (m_OffsetRot.y > D3DX_PI)
					{// �~�����𒴂���
						m_OffsetRot.y -= D3DX_PI * 2.0f;
					}
					if (m_OffsetRot.y < -D3DX_PI)
					{// �~�����𒴂���
						m_OffsetRot.y += D3DX_PI * 2.0f;
					}
				}
				else if (m_pLandScene->GetObjType() == OBJTYPE_OBJECTMOVE)
				{// �����z�u���N���X��������
					CObjectMove *pObjectMove = (CObjectMove*)m_pLandScene;
					D3DXVECTOR3 ObjRot = pObjectMove->GetRot();
					m_OffsetRot = m_Rot - ObjRot;
					if (m_OffsetRot.y > D3DX_PI)
					{// �~�����𒴂���
						m_OffsetRot.y -= D3DX_PI * 2.0f;
					}
					if (m_OffsetRot.y < -D3DX_PI)
					{// �~�����𒴂���
						m_OffsetRot.y += D3DX_PI * 2.0f;
					}
				}
			}
		}

		if (m_State == STATE_LANDING && m_pMotionManager->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP
			|| m_State == STATE_LANDING && sqrtf((m_Move.x * m_Move.x) + (m_Move.z * m_Move.z)) >= PLAYER_MAX_WALKSPEED)
		{// ���n�̃��[�V�����Đ����I��������ړ����Ă���
			// ��Ԃ�ʏ�̏�Ԃ�
			m_State = STATE_NORMAL;

			// ���[�V�����؂�ւ�����
			m_pMotionManager->SwitchMotion(m_apModel, m_State);
		}

		if (m_bAction == true && m_State != STATE_PANCH)
		{// �A�N�V�����̃��[�V�����Đ����I�����
			// �A�N�V���������Ă��Ȃ���Ԃɂ���
			m_bAction = false;
		}

		if (m_State == STATE_PANCH && m_pMotionManager->GetMotion()[m_State]->GetState() == CMotion::STATE_STOP)
		{// �A�N�V�����̃��[�V�����Đ����I�����
		    // �A�N�V���������Ă��Ȃ���Ԃɂ���
			m_bAction = false;

			// ��Ԃ�ʏ�̏�Ԃ�
			m_State = STATE_NORMAL;

			// ���[�V�����؂�ւ�����
			m_pMotionManager->SwitchMotion(m_apModel, m_State);
		}

		if (m_State == STATE_PUSH)
		{// �����Ă����Ԃł͂Ȃ�
			if (m_bPush == false)
			{// �����Ă����Ԃł���
				if (m_bCol == false)
				{// �I�u�W�F�N�g�ɓ������Ă��Ȃ�
					if (m_State != STATE_NORMAL)
					{// �ʏ��Ԃł͂Ȃ�
						// ��Ԃ�ʏ�̏�Ԃ�
						m_State = STATE_NORMAL;

						// ���[�V�����؂�ւ�����
						m_pMotionManager->SwitchMotion(m_apModel, m_State);
					}
				}
			}
		}
	}
}

//=============================================================================
//    �v���C���[�����n�����Ƃ��̏���
//=============================================================================
void CPlayer::Landing(void)
{
	// �ړ��ʂ̒������v�Z
	float fMoveLength = sqrtf((m_Move.x * m_Move.x) + (m_Move.z * m_Move.z));

	if (fMoveLength < PLAYER_MAX_WALKSPEED)
	{// �ړ����Ă��Ȃ�
	    // �ړ��ʂ��Ȃ���
		m_Move.x = 0.0f;
		m_Move.z = 0.0f;

	    // ���n��Ԃ�
		m_State = STATE_LANDING;

		// ���[�V�����؂�ւ�����
		m_pMotionManager->SwitchMotion(m_apModel, m_State);
	}
	else
	{// �ړ����Ă���
		if (fMoveLength >= PLAYER_MAX_WALKSPEED)
		{// �ړ��ʂ�����̒l���傫��
			if (m_State != STATE_MOVE_RUN)
			{// �����Ԃł͂Ȃ�
				m_State = STATE_MOVE_RUN;

				// ���[�V�����؂�ւ�����
				m_pMotionManager->SwitchMotion(m_apModel, m_State);
			}
		}
		else
		{// �X�e�B�b�N�̎�������̒l��菬����
			if (m_State != STATE_MOVE_WALK)
			{// ������Ԃł͂Ȃ�
				m_State = STATE_MOVE_WALK;

				// ���[�V�����؂�ւ�����
				m_pMotionManager->SwitchMotion(m_apModel, m_State);
			}
		}
	}

	m_bJump = false;  // �W�����v���Ă��Ȃ���Ԃ�
}

//=============================================================================
//    �v���C���[�̍U�����菈��
//=============================================================================
bool CPlayer::AttackCollision(D3DXVECTOR3 pos, D3DXVECTOR3 VtxMin, D3DXVECTOR3 VtxMax)
{
	bool bCol = false;   // �����������ǂ���

	if (m_State == STATE_PANCH)
	{// �p���`�̃��[�V�������Ȃ��
		// �����蔻������ꏊ��ݒ�
		D3DXVECTOR3 ColPosition = D3DXVECTOR3(m_apModel[PLAYER_PANCH_COLINDEX]->GetMtxWorld()._41, m_apModel[PLAYER_PANCH_COLINDEX]->GetMtxWorld()._42, m_apModel[PLAYER_PANCH_COLINDEX]->GetMtxWorld()._43);
		float fColRange = PLAYER_PANCH_COLRANGE;

		if (ColPosition.x + fColRange >= pos.x + VtxMin.x && ColPosition.x - fColRange <= pos.x + VtxMax.x)
		{// X���̔�����ɓ����Ă���
			if (ColPosition.y + fColRange >= pos.y && ColPosition.y - fColRange <= pos.y + VtxMax.y)
			{// Y���̔�����ɓ����Ă���
				if (ColPosition.z + fColRange >= pos.z + VtxMin.z && ColPosition.z - fColRange <= pos.z + VtxMax.z)
				{// Z���̔�����ɓ����Ă���
					bCol = true;
					CDebugProc::Print("\n�p���`���q�b�g���܂���\n\n");
				}
			}
		}
	}
	else if (m_State == STATE_KICK)
	{// �L�b�N�̃��[�V�������Ȃ��
	    // �����蔻������ꏊ��ݒ�
		D3DXVECTOR3 ColPosition = D3DXVECTOR3(m_apModel[PLAYER_KICK_COLINDEX]->GetMtxWorld()._41, m_apModel[PLAYER_KICK_COLINDEX]->GetMtxWorld()._42, m_apModel[PLAYER_KICK_COLINDEX]->GetMtxWorld()._43);
		float fColRange = PLAYER_KICK_COLRANGE;

		if (ColPosition.x + fColRange >= pos.x + VtxMin.x && ColPosition.x - fColRange <= pos.x + VtxMax.x)
		{// X���̔�����ɓ����Ă���
			if (ColPosition.y + fColRange >= pos.y && ColPosition.y - fColRange <= pos.y + VtxMax.y)
			{// Y���̔�����ɓ����Ă���
				if (ColPosition.z + fColRange >= pos.z + VtxMin.z && ColPosition.z - fColRange <= pos.z + VtxMax.z)
				{// Z���̔�����ɓ����Ă���
					bCol = true;
					CDebugProc::Print("\n�L�b�N���q�b�g���܂���\n\n");
				}
			}
		}
	}

	return bCol;   // �����������ǂ�����Ԃ�
}

//=============================================================================
//    �v���C���[�̃G�t�F�N�g��������
//=============================================================================
void CPlayer::CreateEffect(void)
{
	if (m_State == STATE_PANCH)
	{// �p���`�����Ă�����
	    // �G�t�F�N�g�̐���
		if (m_pMotionManager != NULL)
		{// ���[�V�����Ǘ��N���X����������Ă���
			if (m_pMotionManager->GetKey() == 3 && m_pMotionManager->GetCounter() == 6)
			{
				D3DXVECTOR3 EffectOffSetPos = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
				D3DXMATRIX EffectMtxWorld;

				D3DXMATRIX mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&EffectMtxWorld);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
				D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &mtxRot);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, EffectOffSetPos.x, EffectOffSetPos.y, EffectOffSetPos.z);
				D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &mtxTrans);

				// �e�̏��𔽉f
				D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &m_apModel[PLAYER_PANCH_COLINDEX]->GetMtxWorld());

				CParEmitter::Create(D3DXVECTOR3(EffectMtxWorld._41, EffectMtxWorld._42, EffectMtxWorld._43), 1, 6);
			}
		}
	}

	if (m_State == STATE_KICK)
	{// �L�b�N�����Ă�����
	    // �G�t�F�N�g�̐���
		if (m_pMotionManager != NULL)
		{// ���[�V�����Ǘ��N���X����������Ă���
			if (m_pMotionManager->GetKey() == 1 && m_pMotionManager->GetCounter() == 0)
			{
				D3DXVECTOR3 EffectOffSetPos = D3DXVECTOR3(0.0f, -3.0f, 0.0f);
				D3DXMATRIX EffectMtxWorld;

				D3DXMATRIX mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

				// ���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&EffectMtxWorld);

				// ��]�𔽉f
				D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
				D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &mtxRot);

				// �ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, EffectOffSetPos.x, EffectOffSetPos.y, EffectOffSetPos.z);
				D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &mtxTrans);

				// �e�̏��𔽉f
				D3DXMatrixMultiply(&EffectMtxWorld, &EffectMtxWorld, &m_apModel[PLAYER_KICK_COLINDEX]->GetMtxWorld());

				CParEmitter::Create(D3DXVECTOR3(EffectMtxWorld._41, EffectMtxWorld._42, EffectMtxWorld._43), 2, 6);
			}
		}
	}
}

//=============================================================================
//    �v���C���[�̉��̏���
//=============================================================================
void CPlayer::Sound(void)
{
	if (m_State == STATE_MOVE_WALK)
	{// �������[�V������������
		if (m_pMotionManager != NULL)
		{// ���[�V�����Ǘ��N���X����������Ă���
			if (m_pMotionManager->GetKey() == 0 && m_pMotionManager->GetCounter() == 20)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WALK000);
			}
			else if (m_pMotionManager->GetKey() == 1 && m_pMotionManager->GetCounter() == 20)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WALK001);
			}
		}
	}
	else if (m_State == STATE_MOVE_RUN)
	{// ���郂�[�V������������
		if (m_pMotionManager != NULL)
		{// ���[�V�����Ǘ��N���X����������Ă���
			if (m_pMotionManager->GetKey() == 0 && m_pMotionManager->GetCounter() == 3)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WALK000);
			}
			else if (m_pMotionManager->GetKey() == 1 && m_pMotionManager->GetCounter() == 3)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_WALK001);
			}
		}
	}
	else if (m_State == STATE_PANCH)
	{// �p���`���[�V������������
		if (m_pMotionManager != NULL)
		{// ���[�V�����Ǘ��N���X����������Ă���
			if (m_pMotionManager->GetKey() == 3 && m_pMotionManager->GetCounter() == 0)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PANCH);
			}
		}
	}
	else if (m_State == STATE_KICK)
	{// �L�b�N���[�V������������
		if (m_pMotionManager != NULL)
		{// ���[�V�����Ǘ��N���X����������Ă���
			if (m_pMotionManager->GetKey() == 1 && m_pMotionManager->GetCounter() == 3)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_KICK);
			}
		}
	}
	else if (m_State == STATE_LANDING)
	{// ���n���[�V������������
		if (m_pMotionManager != NULL)
		{// ���[�V�����Ǘ��N���X����������Ă���
			if (m_pMotionManager->GetKey() == 0 && m_pMotionManager->GetCounter() == 0)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_LANDING);
			}
		}
	}
	else if (m_State == STATE_PUSH)
	{// �������[�V������������
		if (m_pMotionManager != NULL)
		{// ���[�V�����Ǘ��N���X����������Ă���
			if (m_pMotionManager->GetKey() == 0 && m_pMotionManager->GetCounter() == 0)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PUSH);
			}
		}
	}
}

//=============================================================================
//    �v���C���[�̈ʒu�擾����
//=============================================================================
D3DXVECTOR3 CPlayer::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �v���C���[�̌����擾����
//=============================================================================
D3DXVECTOR3 CPlayer::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    �v���C���[�̈ړ��ʎ擾����
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    �v���C���[�̓����蔻�����鍂���擾����
//=============================================================================
float CPlayer::GetColHeight(void)
{
	return m_fColHeight;
}

//=============================================================================
//    �v���C���[�̃W�����v���Ă��邩�ǂ����擾����
//=============================================================================
bool CPlayer::GetJump(void)
{
	return m_bJump;
}

//=============================================================================
//    �v���C���[�̏�Ԑݒ菈��
//=============================================================================
void CPlayer::SetState(const STATE state)
{
	m_State = state;
	m_pMotionManager->SwitchMotion(m_apModel, m_State);
}

//=============================================================================
//    �v���C���[�̌��݂̍��W�ݒ菈��
//=============================================================================
void CPlayer::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �v���C���[�̑O��̍��W�ݒ菈��
//=============================================================================
void CPlayer::SetPosOld(const D3DXVECTOR3 posOld)
{
	m_PosOld = posOld;
}

//=============================================================================
//    �v���C���[�̌����ݒ菈��
//=============================================================================
void CPlayer::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
	m_DestAngle = rot;
}

//=============================================================================
//    �v���C���[�̈ړ��ʐݒ菈��
//=============================================================================
void CPlayer::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    �v���C���[�̃W�����v���Ă��邩�ǂ����ݒ菈��
//=============================================================================
void CPlayer::SetJump(const bool bJump)
{
	m_bJump = bJump;
}

//=============================================================================
//    �v���C���[�̃f�t�H���g�ݒ菈��
//=============================================================================
void CPlayer::SetDefault(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// ���W��߂�
	m_Pos = pos;
	m_PosOld = pos;

	// ������߂�
	m_Rot = rot;
	m_DestAngle = rot;

	// ���f���̃I�t�Z�b�g�����̐ݒ�
	if (m_apModel != NULL && m_pMotionManager != NULL)
	{// ���������m�ۂł���
		for (int nCntModel = 0; nCntModel < m_nNumPart; nCntModel++)
		{// ���f���𐶐����鐔�����J��Ԃ�
			m_apModel[nCntModel]->SetRot(m_pMotionManager->GetMotion()[m_State]->GetKeyframe()[0][nCntModel]->GetDestRot());   // �����𔽉f
		}
	}

	// ���[�V������߂�
	if (m_pMotionManager != NULL)
	{// ���[�W�����N���X�̃|�C���^���m�ۂł��Ă���
		// ��Ԃ�߂�
		m_State = STATE_NORMAL;

		// ���[�V�����؂�ւ�����
		m_pMotionManager->SwitchMotion(m_apModel, m_State);
	}
}


// ���f���P�̂̃v���C���[�����Ȃ̂ō��񂱂�͎g��Ȃ��ł�
#if 0
//*****************************************************************************
//     CSceneXPlayer�̏���
//*****************************************************************************
//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define PLAYER_MODEL_FILENAME   "data/MODEL/01_head.x"   // �v���C���[���f���̃t�@�C����

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH     CSceneXPlayer::m_apMesh = NULL;      // ���b�V�����ւ̃|�C���^
LPD3DXBUFFER   CSceneXPlayer::m_apBuffMat = NULL;   // �}�e���A�����ւ̃|�C���^
DWORD          CSceneXPlayer::m_anNumMat = NULL;    // �}�e���A�����̐�

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSceneXPlayer::CSceneXPlayer(int nPriority, OBJTYPE objType) : CSceneX(nPriority, objType)
{
	// �e��l�̃N���A
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �ړ���
	m_DiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���݂̌����ƖړI�̌����̍���
	m_DestAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �ړI�̌���
	m_fGravity = 0.0f;                            // �d��
	m_fJumpPower = 0.0f;                          // �W�����v��
	m_bJump = false;                              // �W�����v�������ǂ���
	m_fAccel = 0.0f;                              // �����x
	m_fInertia = 0.0f;                            // ����
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSceneXPlayer::~CSceneXPlayer()
{

}

//=============================================================================
//    �v���C���[(���f���P��)�̐�������
//=============================================================================
CSceneXPlayer *CSceneXPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	CSceneXPlayer *pSceneXPlayer = NULL;   // �v���C���[(���f���P��)�N���X�^�̃|�C���^
	if (pSceneXPlayer == NULL)
	{// ����������ɂȂ��Ă���
		pSceneXPlayer = new CSceneXPlayer(nPriority);
		if (pSceneXPlayer != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pSceneXPlayer->Init(pos, rot)))
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

	return pSceneXPlayer;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �v���C���[(���f���P��)�̓ǂݍ��ݏ���
//=============================================================================
HRESULT CSceneXPlayer::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // x�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(PLAYER_MODEL_FILENAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_apBuffMat,
				NULL,
				&m_anNumMat,
				&m_apMesh);
		}
	}

	return S_OK;
}

//=============================================================================
//    �v���C���[(���f���P��)�̊J������
//=============================================================================
void CSceneXPlayer::UnLoad(void)
{
	// ���b�V���̔j��
	if (m_apMesh != NULL)
	{
		m_apMesh->Release();
		m_apMesh = NULL;
	}

	// �}�e���A���̔j��
	if (m_apBuffMat != NULL)
	{
		m_apBuffMat->Release();
		m_apBuffMat = NULL;
	}
}

//=============================================================================
//    �v���C���[(���f���P��)�̏���������
//=============================================================================
HRESULT CSceneXPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �e��l�̐ݒ�
	SetPos(pos);                         // ���݂̍��W
	SetRot(rot);                         // ���݂̌���
	m_fGravity = PLAYER_GRAVITY_INI;     // �d��
	m_fJumpPower = PLAYER_JUMPPOWER_INI; // �W�����v��
	m_fAccel = PLAYER_ACCELERATION_INI;  // �����x
	m_fInertia = PLAYER_INERTIA_INI;     // ����

	// ���ʂ̏���������
	CSceneX::Init();

	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���f���̊��蓖��
			BindModel(pDevice, m_apMesh, m_apBuffMat, m_anNumMat);
		}
	}

	// �e�̐���
	if (m_pShadow == NULL)
	{// ���������m�ۂł����Ԃł���
		m_pShadow = CShadow::Create(D3DXVECTOR3(pos.x, 0.0f, pos.z), rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f), 70.0f, 4);
	}

	return S_OK;
}

//=============================================================================
//    �v���C���[(���f���P��)�̏I������
//=============================================================================
void CSceneXPlayer::Uninit(void)
{
	// �e�̏I������
	if (m_pShadow != NULL)
	{// ���������m�ۂ���Ă���
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	// ���ʂ̏I������
	CSceneX::Uninit();
}

//=============================================================================
//    �v���C���[(���f���P��)�̍X�V����
//=============================================================================
void CSceneXPlayer::Update(void)
{
	// �A�N�V��������
	Action();

	// �ړ�����
	Movement();

	// �����蔻�菈��
	Collision();
}

//=============================================================================
//    �v���C���[(���f���P��)�̕`�揈��
//=============================================================================
void CSceneXPlayer::Draw(void)
{
	// ���ʂ̕`�揈��
	CSceneX::Draw();
}

//=============================================================================
//    �v���C���[(���f���P��)�̃��[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CSceneXPlayer::CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXVECTOR3 pos = GetPos();           // ���݂̈ʒu
	D3DXVECTOR3 rot = GetRot();           // ���݂̌���
	D3DXMATRIX MtxWorld = GetMtxWorld();  // ���[���h�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;          // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&MtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&MtxWorld, &MtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &MtxWorld);
	SetMtxWorld(MtxWorld);
}

//=============================================================================
//    �v���C���[(���f���P��)�̈ړ�����
//=============================================================================
void CSceneXPlayer::Movement(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();        // ���݂̍��W
	D3DXVECTOR3 posOld = GetPosOld();  // �O��̍��W
	D3DXVECTOR3 rot = GetRot();        // ���݂̌���

	// �O��̍��W�X�V
	posOld = pos;

	// �L�[�{�[�h���擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �J�������擾
	CCamera *pCamera = CManager::GetCamera();

	if (pKeyboard->GetPress(DIK_A) == true)
	{// �������̓��͂����ꂽ
		if (pKeyboard->GetPress(DIK_W) == true)
		{// �����ɏ�����̓��͂����ꂽ
		    // �ړ�����
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;

			// �ړI�̊p�x�ύX
			m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// �����ɉ������̓��͂����ꂽ
		    // �ړ�����
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;

			// �ړI�̊p�x�ύX
			m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
		}
		else
		{// ����������ĂȂ�
		    // �ړ�����
			m_Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;
			m_Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;

			// �ړI�̊p�x�ύX
			m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
		}
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{// �E�����̓��͂����ꂽ
		if (pKeyboard->GetPress(DIK_W) == true)
		{// �����ɏ�����̓��͂����ꂽ
		    // �ړ�����
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * m_fAccel;

			// �ړI�̊p�x�ύX
			m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * -0.75f);
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// �����ɉ������̓��͂����ꂽ
		    // �ړ�����
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * m_fAccel;

			// �ړI�̊p�x�ύX
			m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * -0.25f);
		}
		else
		{// ����������ĂȂ�
		    // �ړ�����
			m_Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;
			m_Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * m_fAccel;

			// �ړI�̊p�x�ύX
			m_DestAngle.y = pCamera->GetRot().y + (D3DX_PI * -0.5f);
		}
	}
	else if (pKeyboard->GetPress(DIK_W) == true)
	{// ������̓��͂����ꂽ
	    // �ړ�����
		m_Move.x += sinf(pCamera->GetRot().y) * m_fAccel;
		m_Move.z += cosf(pCamera->GetRot().y) * m_fAccel;

		// �ړI�̊p�x�ύX
		m_DestAngle.y = pCamera->GetRot().y + D3DX_PI;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{// �������̓��͂����ꂽ
	    // �ړ�����
		m_Move.x += sinf(pCamera->GetRot().y + D3DX_PI) * m_fAccel;
		m_Move.z += cosf(pCamera->GetRot().y + D3DX_PI) * m_fAccel;

		// �ړI�̊p�x�ύX
		m_DestAngle.y = pCamera->GetRot().y;
	}

	// ���݂̍��W�Ɉړ��ʂ�������
	pos += m_Move;

	// �d�͂�������
	m_Move.y += m_fGravity;

	// �ړ��ʂɊ�����������
	m_Move.x += (0.0f - m_Move.x) * m_fInertia;
	m_Move.z += (0.0f - m_Move.z) * m_fInertia;

	// �p�x�̏C��
	m_DiffAngle.y = m_DestAngle.y - rot.y;   // ���݂̌����ƖړI�̌����̍������v�Z

	if (m_DiffAngle.y > D3DX_PI)
	{// ������D3DX_PI�𒴂���
		m_DiffAngle.y -= D3DX_PI * 2.0f;
	}
	if (m_DiffAngle.y < -D3DX_PI)
	{// ������-D3DX_PI�𒴂���
		m_DiffAngle.y += D3DX_PI * 2.0f;
	}

	// ���݂̌����ɖړI�̌����Ƃ̍�����{���ŕ␳����
	rot.y += m_DiffAngle.y * PLAYER_ANGLE_REVISION;

	if (rot.y > D3DX_PI)
	{// ���݂̌�����D3DX_PI�𒴂���
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// ���݂̌�����-D3DX_PI�𒴂���
		rot.y += D3DX_PI * 2.0f;
	}

	// �e��l�̐ݒ�
	SetPos(pos);        // ���݂̍��W
	SetPosOld(posOld);  // �O��̍��W
	SetRot(rot);        // ���݂̌���
}

//=============================================================================
//    �v���C���[(���f���P��)�̓����蔻�菈������
//=============================================================================
bool CSceneXPlayer::Collision(void)
{
	bool bCol = false;                 // �����������ǂ���
	D3DXVECTOR3 pos = GetPos();        // �v���C���[�̌��݂̍��W
	D3DXVECTOR3 posOld = GetPosOld();  // �v���C���[�̑O��̍��W

	// �t�B�[���h�Ɣz�u�����擾����
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	CField *pField = NULL;               // �n�ʃN���X�ւ̃|�C���^
	CObject *pObject = NULL;             // �z�u���N���X�ւ̃|�C���^
	CObjectMove *pObjectMove = NULL;     // �����z�u���N���X�ւ̃|�C���^
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetScene(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_FIELD)
			{// �t�B�[���h��������
				pField = (CField*)pScene;
			}
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// �z�u����������
				pObject = (CObject*)pScene;
			}
			if (pScene->GetObjType() == OBJTYPE_OBJECTMOVE)
			{// �z�u����������
				pObjectMove = (CObjectMove*)pScene;
			}
			pScene = pSceneNext;
		}
	}

	if (pObject != NULL)
	{// �z�u�����擾�ł���
	    // �����蔻��ɕK�v�Ȓl��z�u������擾
		D3DXVECTOR3 ObjectPos = pObject->GetPos();          // ���݂̍��W
		D3DXVECTOR3 ObjectPosOld = pObject->GetPosOld();    // �O��̍��W
		D3DXVECTOR3 ObjectVtxMin = pObject->GetVtxMin();    // ���_�̍��W�ŏ��l
		D3DXVECTOR3 ObjectVtxMax = pObject->GetVtxMax();    // ���_�̍��W�ő�l

		// �z�u���Ƃ̔���J�n
		if (pos.x > ObjectPos.x + ObjectVtxMin.x && pos.x < ObjectPos.x + ObjectVtxMax.x
			&& pos.z > ObjectPos.z + ObjectVtxMin.z && pos.z < ObjectPos.z + ObjectVtxMax.z)
		{// �z�u���̒��ɂ���
			CDebugProc::Print("�z�u���̒��ɂ��܂�\n");
			if (posOld.y >= ObjectPos.y + ObjectVtxMax.y && pos.y <= ObjectPos.y + ObjectVtxMax.y)
			{// �z�u���ɏォ�������
				pos.y = ObjectPos.y + ObjectVtxMax.y;
				m_Move.y = 0.0f;
				if (m_bJump == true)
				{// �W�����v���Ă�����
					m_bJump = false;
				}
				CDebugProc::Print("�z�u���ɏ���Ă��܂�\n");
			}
			if (posOld.y + 15.0f <= ObjectPos.y + ObjectVtxMin.y && pos.y + 15.0f > ObjectPos.y + ObjectVtxMin.y)
			{// �z�u���ɉ����������
				pos.y = ObjectPos.y + ObjectVtxMin.y - 15.0f;
				m_Move.y = 0.0f;
			}

			// �e�̈ʒu�����炷
			if (m_pShadow != NULL)
			{// �e����������Ă���
				if (pos.y >= ObjectPos.y + ObjectVtxMax.y)
				{// �z�u���̏�ɂ���
					m_pShadow->SetPos(D3DXVECTOR3(pos.x, ObjectPos.y + ObjectVtxMax.y + 1.0f, pos.z));
				}
			}
		}
	}

	if (pObjectMove != NULL)
	{// �����z�u�����擾�ł���
	    // �����蔻��ɕK�v�Ȓl�𓮂��z�u������擾
		D3DXVECTOR3 ObjectPos = pObjectMove->GetPos();                  // ���݂̍��W
		D3DXVECTOR3 ObjectPosOld = pObjectMove->GetPosOld();            // �O��̍��W
		D3DXVECTOR3 ObjectMove = pObjectMove->GetMove();                // �ړ���
		D3DXVECTOR3 ObjectVtxMin = pObjectMove->GetVtxMin();            // ���_�̍��W�ŏ��l
		D3DXVECTOR3 ObjectVtxMax = pObjectMove->GetVtxMax();            // ���_�̍��W�ő�l
		D3DXVECTOR3 ObjectPosNext = pObjectMove->GetPos() + ObjectMove; // ����̍��W

		// �����z�u���̔���J�n
		if (pos.x > ObjectPos.x + ObjectVtxMin.x && pos.x < ObjectPos.x + ObjectVtxMax.x
			&& pos.z > ObjectPos.z + ObjectVtxMin.z && pos.z < ObjectPos.z + ObjectVtxMax.z)
		{// �z�u���̒��ɂ���
			CDebugProc::Print("�z�u���̒��ɂ��܂�\n");
			if (posOld.y >= ObjectPos.y + ObjectVtxMax.y && pos.y <= ObjectPos.y + ObjectVtxMax.y
				|| ObjectMove.y >= 0.0f && posOld.y >= ObjectPos.y + ObjectVtxMax.y && pos.y <= ObjectPos.y + ObjectVtxMax.y
				|| posOld.y >= ObjectPosOld.y + ObjectVtxMax.y && pos.y <= ObjectPosOld.y + ObjectVtxMax.y
				|| ObjectMove.y >= 0.0f && posOld.y >= ObjectPosOld.y + ObjectVtxMax.y && pos.y <= ObjectPosOld.y + ObjectVtxMax.y)
			{// �z�u���ɏォ�������
				pos.y = ObjectPos.y + ObjectVtxMax.y + ObjectMove.y;
				pos.x += ObjectMove.x;
				pos.z += ObjectMove.z;
				m_Move.y = 0.0f;
				if (m_bJump == true)
				{// �W�����v���Ă�����
					m_bJump = false;
				}
				CDebugProc::Print("�z�u���ɏ���Ă��܂�\n");
			}
			if (posOld.y + 15.0f <= ObjectPos.y + ObjectVtxMin.y && pos.y + 15.0f > ObjectPos.y + ObjectVtxMin.y
				|| posOld.y + 15.0f <= ObjectPosOld.y + ObjectVtxMin.y && pos.y + 15.0f > ObjectPosOld.y + ObjectVtxMin.y)
			{// �z�u���ɉ����������
				pos.y = ObjectPos.y + ObjectVtxMin.y - 15.0f;
				m_Move.y = 0.0f;
			}

			// �e�̈ʒu�����炷
			if (m_pShadow != NULL)
			{// �e����������Ă���
				if (pos.y >= ObjectPos.y + ObjectVtxMax.y || posOld.y >= ObjectPos.y + ObjectVtxMax.y)
				{// �z�u���̏�ɂ���
					m_pShadow->SetPos(D3DXVECTOR3(pos.x, ObjectPos.y + ObjectVtxMax.y + 1.0f + ObjectMove.y, pos.z));
				}
			}
		}
	}


	if (pField != NULL)
	{// �t�B�[���h���擾�ł���
		D3DXVECTOR3 PolygonNormal;
		float fFieldHeight = pField->GetPolyHeight(pos, &PolygonNormal);  // �|���S���̍������擾
		if (pos.y < fFieldHeight)
		{// �|���S���̍������Ⴍ�Ȃ���
			pos.y = fFieldHeight;
			m_Move.y = 0.0f;
			if (m_bJump == true)
			{// �W�����v���Ă�����
				m_bJump = false;
			}
		}
		if (m_pShadow != NULL)
		{// �e����������Ă���
			m_pShadow->SetPos(D3DXVECTOR3(pos.x, fFieldHeight + 1.0f, pos.z));
			// ���_���̐ݒ�
			VERTEX_3D *pVtx;
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pShadow->GetVtxBuff();
			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntVer = 0; nCntVer < 4; nCntVer++)
			{// �e�̒��_�����J��Ԃ�
				float DotX = (PolygonNormal.x * pVtx[nCntVer].pos.x);       // X�����̓��ς��v�Z
				float DotZ = (PolygonNormal.z * pVtx[nCntVer].pos.z);       // Z�����̓��ς��v�Z
				float Dot = -DotX - DotZ;                                   // X������Z�����̓��ϓ��m������
				pVtx[nCntVer].pos.y = (Dot / PolygonNormal.y);              // ���������̂�Y�����̖@���Ŋ���
			}

			// ���_�o�b�t�@���A�����b�N����
			pVtxBuff->Unlock();

			// ���_�o�b�t�@�̐ݒ�
			m_pShadow->SetVtxBuff(pVtxBuff);
		}
	}

	// �e��l�̐ݒ�
	SetPos(pos);       // ���݂̍��W

	CDebugProc::Print("\n�v���C���[�̌��݂̍��W : ( %.1f %.1f %.1f )\n", pos.x, pos.y, pos.z);
	CDebugProc::Print("�v���C���[�̑O��̍��W : ( %.1f %.1f %.1f )\n\n", posOld.x, posOld.y, posOld.z);

	return bCol;   // �����������ǂ�����Ԃ�
}

//=============================================================================
//    �v���C���[(���f���P��)�̃A�N�V��������
//=============================================================================
void CSceneXPlayer::Action(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();  // ���W
	D3DXVECTOR3 rot = GetRot();  // ����

	// �L�[�{�[�h���擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetTrigger(DIK_RETURN) == true)
	{// ENTER�L�[�������ꂽ
		if (m_bJump == false)
		{// �W�����v���Ă��Ȃ�
			m_Move.y += m_fJumpPower;
			m_bJump = true;
		}
	}
}

//=============================================================================
//    �v���C���[(���f���P��)�̈ړ��ʎ擾����
//=============================================================================
D3DXVECTOR3 CSceneXPlayer::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    �v���C���[(���f���P��)�̃W�����v���Ă��邩�ǂ����擾����
//=============================================================================
bool CSceneXPlayer::GetJump(void)
{
	return m_bJump;
}

//=============================================================================
//    �v���C���[(���f���P��)�̈ړ��ʐݒ菈��
//=============================================================================
void CSceneXPlayer::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    �v���C���[(���f���P��)�̃W�����v���Ă��邩�ǂ����ݒ菈��
//=============================================================================
void CSceneXPlayer::SetJump(const bool bJump)
{
	m_bJump = bJump;
}

#endif