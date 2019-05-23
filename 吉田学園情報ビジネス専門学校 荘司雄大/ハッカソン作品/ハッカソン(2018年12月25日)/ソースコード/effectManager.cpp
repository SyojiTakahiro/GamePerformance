//*****************************************************************************
//
//     �G�t�F�N�g�Ǌ��̏���[effectManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "effectManager.h"
#include "emitter.h"
#include "particle.h"
#include "textureManager.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define EFFECT_SAVE_FILENAME "data/TEXT/save_particle.txt"  // �G�t�F�N�g�f�[�^��ۑ����鏈��

// �l�ǂݍ��݂�����ۂ̖ڈ�ƂȂ镶����
// �e�N�X�`��
#define NUM_TEXTURE       "NUM_TEXTURE = "         // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME  "TEXTURE_FILENAME = "    // �ǂݍ��ރe�N�X�`���̃t�@�C����

// �G�~�b�^�f�[�^
#define NUM_EMITTER       "NUM_EMITTER = "         // �G�~�b�^�f�[�^�̌�
#define EMITTERSET        "EMITTERSET"             // �G�~�b�^�f�[�^�ǂݍ��݊J�n�̍��}
#define END_EMITTERSET    "END_EMITTERSET"         // �G�~�b�^�f�[�^�ǂݍ��ݏI���̍��}
#define TEX_TYPE          "TEX_TYPE = "            // �g�p����e�N�X�`���̔ԍ�
#define TYPE              "TYPE = "                // �g�p����G�t�F�N�g�̔ԍ�
#define RANGE             "RANGE = "               // �G�t�F�N�g�𐶐�����͈�
#define LIFE              "LIFE = "                // �G�~�b�^�̎���
#define LAP               "LAP = "                 // �P��̕��o�ŏo���G�t�F�N�g�̌�
#define APPEAR            "APPEAR = "              // �G�t�F�N�g���o���Ԋu
#define LOOP              "LOOP = "                // ���[�v���邩���Ȃ���

// �p�[�e�B�N���f�[�^
#define NUM_PARTICLE      "NUM_PARTICLE = "        // �p�[�e�B�N���f�[�^�̌�
#define PARTICLESET       "PARTICLESET"            // �p�[�e�B�N���f�[�^�ǂݍ��݊J�n�̍��}
#define END_PARTICLESET   "END_PARTICLESET"        // �p�[�e�B�N���f�[�^�ǂݍ��ݏI���̍��}
#define RANGE             "RANGE = "               // ��������͈�
#define RANGE_MAG         "RANGE_MAG = "           // ��������͈͂̔{��
#define ROT               "ROT = "                 // �������̌���[ ��:�ő�l �E:�ŏ��l ]
#define MAX_MOVE          "MAX_MOVE = "            // �ړ��ʂ̍ő�l
#define MIN_MOVE          "MIN_MOVE = "            // �ړ��ʂ̍ŏ��l
#define CHANGE_MOVE       "CHANGE_MOVE = "         // �ړ��ʂ̕ω���
#define INIT_COL          "INIT_COL = "            // �������̐F
#define CHANGE_COL        "CHANGE_COL = "          // ���t���[���̐F�̕ω���
#define MAX_SIZE          "MAX_SIZE = "            // �傫���̍ő�l
#define MIN_SIZE          "MIN_SIZE = "            // �傫���̍ŏ��l
#define SPREAD            "SPREAD = "              // �傫���̕ω���
#define MAX_LIFE          "MAX_LIFE = "            // �����̍ő�l
#define MIN_LIFE          "MIN_LIFE = "            // �����̍ŏ��l
#define GRAVITY           "GRAVITY = "             // ���t���[��������d��
#define BOUNCING          "BOUNCING = "            // �o�E���h��
#define ROT_PATTERN       "ROT_PATTERN = "         // ��]�̎��[ 0:���v��� 1:�����v��� 2:�����_�� ]
#define ROT_SPEED         "ROT_SPEED = "           // ��]����X�s�[�h
#define COLLISION         "COLLISION = "           // �����蔻�肷�邩���Ȃ���[ 0:�Ȃ� 1:���� ]
#define DRAW_ADDTIVE      "DRAW_ADDTIVE = "        // ���Z�������邩���Ȃ���[ 0:�Ȃ� 1:���� ]
#define LIGHTING          "LIGHTING = "            // ���C�e�B���O���邩���Ȃ���[ 0:�Ȃ� 1:���� ]

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEffectManager::CEffectManager(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMMITER; nCntEmitter++)
	{// �G�~�b�^�f�[�^�̍ő吔�����J��Ԃ�
		m_apEmitterData[nCntEmitter] = NULL;   // �G�~�b�^�f�[�^�N���X�ւ̃|�C���^
	}
	for (int nCntParData = 0; nCntParData < MAX_PARTICLEDATA; nCntParData++)
	{// �p�[�e�B�N���f�[�^�̍ő吔�����J��Ԃ�
		m_apParData[nCntParData] = NULL;       // �p�[�e�B�N���f�[�^�N���X�ւ̃|�C���^
	}
	m_pEmitter = NULL;      // ���ݐ�������Ă���G�~�b�^�ւ̃|�C���^
	m_nNumEmitterData = 0;  // �G�~�b�^�f�[�^�̌�
	m_nNumParData = 0;      // �p�[�e�B�N���f�[�^�̌�
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEffectManager::~CEffectManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CEffectManager *CEffectManager::Create(char *pFileName)
{
	CEffectManager *pEffectManager = NULL;   // �G�t�F�N�g�Ǌ��N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();     // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pEffectManager == NULL)
		{// ����������ɂȂ��Ă���
			pEffectManager = new CEffectManager;
			if (pEffectManager != NULL)
			{// �C���X�^���X�𐶐��ł���
				pEffectManager->SetFileName(pFileName);
				if (FAILED(pEffectManager->Init()))
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

	return pEffectManager;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CEffectManager::Init(void)
{
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// �|�C���^���m�ۂł���
			char *pLine = NULL;                    // 1�s���ǂݎ��p
			char *pStrCur = NULL;                  // ���݂̐擪�|�C���^
			char *pStr = NULL;                     // �擪�|�C���^�ۑ��p
			int nNumTexture = 0;                   // �ǂݍ��ރe�N�X�`���̐�
			int nCntTexture = 0;                   // �e�N�X�`����ǂݍ��񂾉�
			int nCntEmitter = 0;                   // �G�~�b�^�f�[�^��ǂݍ��񂾉�
			int nCntParticle = 0;                  // �p�[�e�B�N���f�[�^��ǂݍ��񂾉�
			LPDIRECT3DTEXTURE9 pTexture = NULL;    // �e�N�X�`���ǂݎ��p
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
							{// �e�N�X�`���̌���񂪂�����
								nNumTexture = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTexture >= 1)
								{// ���f������1�ȏ゠��
									m_pTextureManager = CTextureManager::Create(nNumTexture);
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
									m_pTextureManager->BindTexture(pTexture, nCntTexture);
									m_pTextureManager->SetFileName(pStr, nCntTexture);
									pTexture = NULL;
									nCntTexture++;
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_EMITTER) == 0)
							{// �G�~�b�^�f�[�^�̌���񂪂���
								m_nNumEmitterData = CFunctionLib::ReadInt(pStrCur, NUM_EMITTER);
							}
							else if (CFunctionLib::Memcmp(pStrCur, EMITTERSET) == 0)
							{// �G�~�b�^�f�[�^�ǂݍ��݊J�n�̍��}������
								m_apEmitterData[nCntEmitter] = ReadEmitterData(pLine, pStrCur, pFile, nCntEmitter);
								nCntEmitter++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_PARTICLE) == 0)
							{// �p�[�e�B�N���f�[�^�̌���񂪂���
								m_nNumParData = CFunctionLib::ReadInt(pStrCur, NUM_PARTICLE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, PARTICLESET) == 0)
							{// �p�[�e�B�N���f�[�^�ǂݍ��݊J�n�̍��}������
								m_apParData[nCntParticle] = ReadParticleData(pLine, pStrCur, pFile, nCntParticle);
								nCntParticle++;
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
			fclose(pFile);   // �t�@�C������Ă���
		}
		else
		{// �|�C���^���m�ۂł��Ȃ�����
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
void CEffectManager::Uninit(void)
{
	// �G�t�F�N�g�f�[�^��ۑ�����
	//Save();

	// �G�~�b�^�̊J��
	if (m_pEmitter != NULL)
	{
		m_pEmitter = NULL;
	}

	// �G�~�b�^�f�[�^�N���X�̊J��
	for (int nCntEmitter = 0; nCntEmitter < MAX_EMMITER; nCntEmitter++)
	{// �G�~�b�^�f�[�^�̍ő吔�����J��Ԃ�
		if (m_apEmitterData[nCntEmitter] != NULL)
		{// ���������m�ۂ���Ă���
			delete m_apEmitterData[nCntEmitter];
			m_apEmitterData[nCntEmitter] = NULL;
		}
	}

	// �p�[�e�B�N���f�[�^�N���X�̊J��
	for (int nCntParData = 0; nCntParData < MAX_PARTICLEDATA; nCntParData++)
	{// �p�[�e�B�N���f�[�^�̍ő吔�����J��Ԃ�
		if (m_apParData[nCntParData] != NULL)
		{// ���������m�ۂ���Ă���
			delete m_apParData[nCntParData];
			m_apParData[nCntParData] = NULL;
		}
	}

	// �e�N�X�`���Ǌ��N���X�̊J��
	if (m_pTextureManager != NULL)
	{// ���������m�ۂ���Ă���
		m_pTextureManager->Uninit();

		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// ���g�̃|�C���^���폜
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CEffectManager::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CEffectManager::Draw(void)
{

}

//=============================================================================
//    �G�t�F�N�g��ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority)
{
	// �G�~�b�^�f�[�^�N���X����f�[�^���擾
	if (m_apEmitterData[nType] != NULL)
	{// �f�[�^�N���X���쐬����Ă���
		int nTexIdx = m_apEmitterData[nType]->GetTexIdx();
		int nParType = m_apEmitterData[nType]->GetType();
		int nLife = m_apEmitterData[nType]->GetLife();
		float fRangeMax = m_apEmitterData[nType]->GetRangeMax();
		float fRangeMin = m_apEmitterData[nType]->GetRangeMin();
		int nLap = m_apEmitterData[nType]->GetLap();
		int nAppear = m_apEmitterData[nType]->GetAppear();
		bool bLoop = m_apEmitterData[nType]->GetLoop();

		// �G�~�b�^�N���X���쐬����
		m_pEmitter = CEmitter::Create(pos, rot, nTexIdx, nParType, fRangeMax, fRangeMin, nLife, nLap, nAppear, bLoop, m_apParData[nParType], m_pTextureManager->GetTexture(nTexIdx), nPriority);
	}
}

//=============================================================================
//    �G�~�b�^�f�[�^��ǂݍ��ޏ���
//=============================================================================
CEmitterData *CEffectManager::ReadEmitterData(char *pLine, char *pStrCur, FILE *pFile, int nCntEmitter)
{
	CEmitterData *pEmitterData = NULL;  // �G�~�b�^�f�[�^�N���X�ւ̃|�C���^
	char aStr[256];
	if (pEmitterData == NULL)
	{// ���������m�ۂł����Ԃł���
		pEmitterData = new CEmitterData;
		if (pEmitterData != NULL)
		{// ���������m�ۂł���
			while (1)
			{// ���[�v�J�n
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
				if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
				{// �e�N�X�`���ԍ���񂪂���
					pEmitterData->SetTexIdx(CFunctionLib::ReadInt(pStrCur, TEX_TYPE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
				{// ��ޏ�񂪂���
					pEmitterData->SetType(CFunctionLib::ReadInt(pStrCur, TYPE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, RANGE) == 0)
				{// �G�t�F�N�g�𐶐�����͈͏�񂪂���
					pEmitterData->SetRangeMax(CFunctionLib::ReadFloat(pStrCur, RANGE));
					pStrCur = CFunctionLib::HeadPutout(pStrCur, RANGE);
					int nWord = CFunctionLib::PopString(pStrCur, aStr);
					pStrCur += nWord;
					pEmitterData->SetRangeMin(CFunctionLib::ReadFloat(pStrCur, ""));
				}
				else if (CFunctionLib::Memcmp(pStrCur, LIFE) == 0)
				{// ������񂪂���
					pEmitterData->SetLife(CFunctionLib::ReadInt(pStrCur, LIFE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, LAP) == 0)
				{// 1��̕��o�Ő����������񂪂���
					pEmitterData->SetLap(CFunctionLib::ReadInt(pStrCur, LAP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, APPEAR) == 0)
				{// ���o����Ԋu��񂪂���
					pEmitterData->SetAppear(CFunctionLib::ReadInt(pStrCur, APPEAR));
				}
				else if (CFunctionLib::Memcmp(pStrCur, LOOP) == 0)
				{// ���[�v���邩���Ȃ�����񂪂���
					pEmitterData->SetLoop(CFunctionLib::ReadBool(pStrCur, LOOP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_EMITTERSET) == 0)
				{// �G�~�b�^�f�[�^�ǂݍ��ݏI���̍��}������
					break;  // ���[�v�I��
				}
			}
		}
	}

	return pEmitterData;
}

//=============================================================================
//    �p�[�e�B�N���f�[�^��ǂݍ��ޏ���
//=============================================================================
CParData *CEffectManager::ReadParticleData(char *pLine, char *pStrCur, FILE *pFile, int nCntParData)
{
	CParData *pParData = NULL;  // �p�[�e�B�N���f�[�^�N���X�ւ̃|�C���^
	char aStr[256];

	if (pParData == NULL)
	{// ���������m�ۂł����Ԃł���
		pParData = new CParData;
		if (pParData != NULL)
		{// ���������m�ۂł���
			while (1)
			{// ���[�v�J�n
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
				if (CFunctionLib::Memcmp(pStrCur, RANGE) == 0)
				{// ��������͈͏�񂪂���
					pParData->SetRange(CFunctionLib::ReadVector3(pStrCur, RANGE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, RANGE_MAG) == 0)
				{// ��������͈͂̔{����񂪂���
					pParData->SetRangeMag(CFunctionLib::ReadFloat(pStrCur, RANGE_MAG));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
				{// �������̌�����񂪂���
					pParData->SetMaxRot(CFunctionLib::ReadFloat(pStrCur, ROT));
					pStrCur = CFunctionLib::HeadPutout(pStrCur, ROT);
					int nWord = CFunctionLib::PopString(pStrCur, aStr);
					pStrCur += nWord;
					pParData->SetMinRot(CFunctionLib::ReadFloat(pStrCur, ""));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MAX_MOVE) == 0)
				{// �ړ��ʂ̍ő�l��񂪂���
					pParData->SetMaxMove(CFunctionLib::ReadVector3(pStrCur, MAX_MOVE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MIN_MOVE) == 0)
				{// �ړ��ʂ̍ŏ��l��񂪂���
					pParData->SetMinMove(CFunctionLib::ReadVector3(pStrCur, MIN_MOVE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, CHANGE_MOVE) == 0)
				{// �ړ��ʂ̕ω��ʏ�񂪂���
					pParData->SetChangeMove(CFunctionLib::ReadVector3(pStrCur, CHANGE_MOVE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, INIT_COL) == 0)
				{// �������̐F��񂪂���
					pParData->SetInitCol(CFunctionLib::ReadVector4(pStrCur, INIT_COL));
				}
				else if (CFunctionLib::Memcmp(pStrCur, CHANGE_COL) == 0)
				{// �t���[�����̐F�̕ω��ʏ�񂪂���
					pParData->SetChangeCol(CFunctionLib::ReadVector4(pStrCur, CHANGE_COL));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MAX_SIZE) == 0)
				{// �傫���̍ő�l��񂪂���
					pParData->SetMaxWidth(CFunctionLib::ReadFloat(pStrCur, MAX_SIZE));
					pStrCur = CFunctionLib::HeadPutout(pStrCur, MAX_SIZE);
					int nWord = CFunctionLib::PopString(pStrCur, aStr);
					pStrCur += nWord;
					pParData->SetMaxHeight(CFunctionLib::ReadFloat(pStrCur, ""));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MIN_SIZE) == 0)
				{// �傫���̍ŏ��l��񂪂���
					pParData->SetMinWidth(CFunctionLib::ReadFloat(pStrCur, MIN_SIZE));
					pStrCur = CFunctionLib::HeadPutout(pStrCur, MIN_SIZE);
					int nWord = CFunctionLib::PopString(pStrCur, aStr);
					pStrCur += nWord;
					pParData->SetMinHeight(CFunctionLib::ReadFloat(pStrCur, ""));
				}
				else if (CFunctionLib::Memcmp(pStrCur, SPREAD) == 0)
				{// �傫���̕ω��ʏ�񂪂���
					pParData->SetSpread(CFunctionLib::ReadFloat(pStrCur, SPREAD));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MAX_LIFE) == 0)
				{// �����̍ő�l��񂪂���
					pParData->SetMaxLife(CFunctionLib::ReadInt(pStrCur, MAX_LIFE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, MIN_LIFE) == 0)
				{// �����̍ŏ��l��񂪂���
					pParData->SetMinLife(CFunctionLib::ReadInt(pStrCur, MIN_LIFE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, GRAVITY) == 0)
				{// ���t���[��������d�͏�񂪂���
					pParData->SetGravity(CFunctionLib::ReadFloat(pStrCur, GRAVITY));
				}
				else if (CFunctionLib::Memcmp(pStrCur, BOUNCING) == 0)
				{// �o�E���h�ʏ�񂪂���
					pParData->SetBouncing(CFunctionLib::ReadFloat(pStrCur, BOUNCING));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ROT_PATTERN) == 0)
				{// ��]�̎�ޏ�񂪂���
					pParData->SetRotPattern(CFunctionLib::ReadInt(pStrCur, ROT_PATTERN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ROT_SPEED) == 0)
				{// ��]�̃X�s�[�h��񂪂���
					pParData->SetRotSpeed(CFunctionLib::ReadFloat(pStrCur, ROT_SPEED));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
				{// �����蔻������邩���Ȃ�����񂪂���
					pParData->SetCollision(CFunctionLib::ReadBool(pStrCur, COLLISION));
				}
				else if (CFunctionLib::Memcmp(pStrCur, DRAW_ADDTIVE) == 0)
				{// ���Z�������邩���Ȃ�����񂪂���
					pParData->SetDrawAddtive(CFunctionLib::ReadBool(pStrCur, DRAW_ADDTIVE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, LIGHTING) == 0)
				{// ���C�e�B���O���邩���Ȃ�����񂪂���
					pParData->SetLighting(CFunctionLib::ReadBool(pStrCur, LIGHTING));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_PARTICLESET) == 0)
				{// �p�[�e�B�N���f�[�^�ǂݍ��ݏI���̍��}������
					break;  // ���[�v�I��
				}
			}
		}
	}

	return pParData;
}

//=============================================================================
//    �G�t�F�N�g�f�[�^��ۑ����鏈��
//=============================================================================
void CEffectManager::Save(void)
{
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(EFFECT_SAVE_FILENAME, "w");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
			// �t�@�C���̖`���������쐬
			fprintf(pFile, "#==============================================================================\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "# �G�t�F�N�g�f�[�^ [%s]\n", EFFECT_SAVE_FILENAME);
			fprintf(pFile, "# Author : HODAKA NIWA\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#==============================================================================\n");
			fprintf(pFile, "%s			#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", SCRIPT);

			// �e�N�X�`���f�[�^�̕ۑ�
			SaveTextureData(pFile);

			// �G�~�b�^�f�[�^�̕ۑ�
			SaveEmitterData(pFile);

			// �p�[�e�B�N���f�[�^�̕ۑ�
			SaveParticleData(pFile);

			fprintf(pFile, "%s			#���̍s�͐�΂ɏ����Ȃ����ƁI\n\n", END_SCRIPT);

			fclose(pFile);   // �t�@�C������Ă���
		}
	}
}

//=============================================================================
//    �e�N�X�`���f�[�^��ۑ����鏈��
//=============================================================================
void CEffectManager::SaveTextureData(FILE *pFile)
{
	// �R�����g�������쐬
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  �g�p����e�N�X�`����\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// �e�N�X�`������ۑ�
	fprintf(pFile, "%s%d\n\n", NUM_TEXTURE, m_pTextureManager->GetNumTexture());

	// �R�����g�������쐬
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  �g�p����e�N�X�`���̃t�@�C���p�X��\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// �e�N�X�`���ւ̃t�@�C���p�X����ۑ�
	for (int nCntTex = 0; nCntTex < m_pTextureManager->GetNumTexture(); nCntTex++)
	{// �ǂݍ��񂾃e�N�X�`���̐������J��Ԃ�
		fprintf(pFile, "%s%s\n", TEXTURE_FILENAME, m_pTextureManager->GetFileName(nCntTex));
	}
	fprintf(pFile, "\n");
}

//=============================================================================
//    �G�~�b�^�f�[�^��ۑ����鏈��
//=============================================================================
void CEffectManager::SaveEmitterData(FILE *pFile)
{
	// �R�����g�������쐬
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  �G�~�b�^��\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// �G�~�b�^����ۑ�
	fprintf(pFile, "%s%d\n\n", NUM_EMITTER, m_nNumEmitterData);

	// �R�����g�������쐬
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  �G�~�b�^�̃f�[�^\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// �G�~�b�^�̃f�[�^���쐬�����������ۑ�
	for (int nCntEmitter = 0; nCntEmitter < m_nNumEmitterData; nCntEmitter++)
	{// �쐬�����G�~�b�^�̐������J��Ԃ�
		if (m_apEmitterData[nCntEmitter] != NULL)
		{// �G�~�b�^�f�[�^���쐬����Ă���
			fprintf(pFile, "%s\n", EMITTERSET);
			fprintf(pFile, "	%s%d		#�e�N�X�`���̔ԍ�\n", TEX_TYPE, m_apEmitterData[nCntEmitter]->GetTexIdx());
			fprintf(pFile, "	%s%d			#�g�p����G�t�F�N�g�̔ԍ�\n", TYPE, m_apEmitterData[nCntEmitter]->GetType());
			fprintf(pFile, "	%s%d %d		#�G�t�F�N�g����o����͈�[ �� :�ő�l �E:�ŏ��l ]\n", RANGE, (int)m_apEmitterData[nCntEmitter]->GetRangeMax(), (int)m_apEmitterData[nCntEmitter]->GetRangeMin());
			fprintf(pFile, "	%s%d			#�G�~�b�^�̎���\n", LIFE, m_apEmitterData[nCntEmitter]->GetLife());
			fprintf(pFile, "	%s%d				#�P��̕��o�ŏo���G�t�F�N�g�̌�\n", LAP, m_apEmitterData[nCntEmitter]->GetLap());
			fprintf(pFile, "	%s%d			#�G�t�F�N�g���o���Ԋu\n", APPEAR, m_apEmitterData[nCntEmitter]->GetAppear());
			fprintf(pFile, "	%s%d			#���[�v���邩���Ȃ���[ 0:�Ȃ� 1:���� ]\n", LOOP, (int)m_apEmitterData[nCntEmitter]->GetLoop());
			fprintf(pFile, "%s\n\n", END_EMITTERSET);
		}
	}
}

//=============================================================================
//    �p�[�e�B�N���f�[�^��ۑ����鏈��
//=============================================================================
void CEffectManager::SaveParticleData(FILE *pFile)
{
	// �R�����g�������쐬
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  �p�[�e�B�N���f�[�^�̐�\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// �p�[�e�B�N���f�[�^����ۑ�
	fprintf(pFile, "%s%d\n\n", NUM_PARTICLE, m_nNumParData);

	// �R�����g�������쐬
	fprintf(pFile, "#------------------------------------------------------------------------------\n");
	fprintf(pFile, "#  �p�[�e�B�N���̃f�[�^\n");
	fprintf(pFile, "#------------------------------------------------------------------------------\n");

	// �p�[�e�B�N���̃f�[�^���쐬�����������ۑ�
	for (int nCntParticle = 0; nCntParticle < m_nNumParData; nCntParticle++)
	{// �쐬�����p�[�e�B�N���̃f�[�^�̐������J��Ԃ�
		fprintf(pFile, "%s\n", PARTICLESET);
		fprintf(pFile, "	%s%.4f %.4f %.4f			#��������͈�\n", RANGE, m_apParData[nCntParticle]->GetRange().x, m_apParData[nCntParticle]->GetRange().y, m_apParData[nCntParticle]->GetRange().z);
		fprintf(pFile, "	%s%.4f						#��������͈͂̔{��\n", RANGE_MAG, m_apParData[nCntParticle]->GetRangeMag());
		fprintf(pFile, "	%s%.4f %.4f							#�������̌���[ ��:�ő�l �E:�ŏ��l ]\n", ROT, m_apParData[nCntParticle]->GetMaxRot(), m_apParData[nCntParticle]->GetMinRot());
		fprintf(pFile, "	%s%.4f %.4f %.4f			#�ړ��ʂ̍ő�l\n", MAX_MOVE, m_apParData[nCntParticle]->GetMaxMove().x, m_apParData[nCntParticle]->GetMaxMove().y, m_apParData[nCntParticle]->GetMaxMove().z);
		fprintf(pFile, "	%s%.4f %.4f %.4f				#�ړ��ʂ̍ŏ��l\n", MIN_MOVE, m_apParData[nCntParticle]->GetMinMove().x, m_apParData[nCntParticle]->GetMinMove().y, m_apParData[nCntParticle]->GetMinMove().z);
		fprintf(pFile, "	%s%.4f %.4f %.4f			#�ړ��ʂ̕ω���\n", CHANGE_MOVE, m_apParData[nCntParticle]->GetChangeMove().x, m_apParData[nCntParticle]->GetChangeMove().y, m_apParData[nCntParticle]->GetChangeMove().z);
		fprintf(pFile, "	%s%.4f %.4f %.4f %.4f			#�������̐F\n", INIT_COL, m_apParData[nCntParticle]->GetInitCol().r, m_apParData[nCntParticle]->GetInitCol().g, m_apParData[nCntParticle]->GetInitCol().b, m_apParData[nCntParticle]->GetInitCol().a);
		fprintf(pFile, "	%s%.4f %.4f %.4f %.4f		#�t���[�����̐F�̕ω���\n", CHANGE_COL, m_apParData[nCntParticle]->GetChangeCol().r, m_apParData[nCntParticle]->GetChangeCol().g, m_apParData[nCntParticle]->GetChangeCol().b, m_apParData[nCntParticle]->GetChangeCol().a);
		fprintf(pFile, "	%s%.4f %.4f				#�傫���̍ő�l\n", MAX_SIZE, m_apParData[nCntParticle]->GetMaxWidth(), m_apParData[nCntParticle]->GetMaxHeight());
		fprintf(pFile, "	%s%.4f %.4f				#�傫���̍ŏ��l\n", MIN_SIZE, m_apParData[nCntParticle]->GetMinWidth(), m_apParData[nCntParticle]->GetMinHeight());
		fprintf(pFile, "	%s%f						#�傫���̕ω���\n", SPREAD, m_apParData[nCntParticle]->GetSpread());
		fprintf(pFile, "	%s%d						#�����̍ő�l\n", MAX_LIFE, m_apParData[nCntParticle]->GetMaxLife());
		fprintf(pFile, "	%s%d						#�����̍ŏ��l\n", MIN_LIFE, m_apParData[nCntParticle]->GetMinLife());
		fprintf(pFile, "	%s%.4f						#���t���[��������d��\n", GRAVITY, m_apParData[nCntParticle]->GetGravity());
		fprintf(pFile, "	%s%.4f						#�o�E���h��\n", BOUNCING, m_apParData[nCntParticle]->GetBouncing());
		fprintf(pFile, "	%s%d						#��]�̃p�^�[��[ 0:���v��� 1:�����v��� 2:�����_�� ]\n", ROT_PATTERN, m_apParData[nCntParticle]->GetRotPattern());
		fprintf(pFile, "	%s%.4f						#��]�̃X�s�[�h\n", ROT_SPEED, m_apParData[nCntParticle]->GetRotSpeed());
		fprintf(pFile, "	%s%d						#�����蔻������邩���Ȃ���[ 0:�Ȃ� 1:���� ]\n", COLLISION, (int)m_apParData[nCntParticle]->GetCollision());
		fprintf(pFile, "	%s%d					#���Z�������邩���Ȃ���[ 0:�Ȃ� 1:���� ]\n", DRAW_ADDTIVE, (int)m_apParData[nCntParticle]->GetDrawAddtive());
		fprintf(pFile, "	%s%d						#���C�e�B���O���邩���Ȃ���[ 0:�Ȃ� 1:���� ]\n", LIGHTING, (int)m_apParData[nCntParticle]->GetLighting());
		fprintf(pFile, "%s\n\n", END_PARTICLESET);
	}
}

//=============================================================================
//    �X�N���v�g�t�@�C������ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetFileName(char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    �G�~�b�^�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetEmitter(CEmitter *pEmitter)
{
	m_pEmitter = pEmitter;
}

//=============================================================================
//    �p�[�e�B�N���f�[�^��ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetParData(CParData *pParData, int nIdx)
{
	m_apParData[nIdx] = pParData;
}

//=============================================================================
//    �G�~�b�^�f�[�^��ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetEmitterData(CEmitterData *pEmitterData, int nIdx)
{
	m_apEmitterData[nIdx] = pEmitterData;
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�ւ̃|�C���^��ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetTexManager(CTextureManager *pTexManager)
{
	m_pTextureManager = pTexManager;
}

//=============================================================================
//    �G�~�b�^�f�[�^�̐���ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetNumEmitterData(int nNumEmitterData)
{
	m_nNumEmitterData = nNumEmitterData;
}

//=============================================================================
//    �p�[�e�B�N���f�[�^�̐���ݒ肷�鏈��
//=============================================================================
void CEffectManager::SetNumParData(int nNumParData)
{
	m_nNumParData = nNumParData;
}

//=============================================================================
//    �e�N�X�`�����擾���鏈��
//=============================================================================
LPDIRECT3DTEXTURE9 CEffectManager::BindTexture(int nIdx)
{
	return m_pTextureManager->GetTexture(nIdx);
}

//=============================================================================
//    �G�~�b�^�N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CEmitter *CEffectManager::GetEmitter(void)
{
	return m_pEmitter;
}

//=============================================================================
//    �p�[�e�B�N���f�[�^���擾���鏈��
//=============================================================================
CParData *CEffectManager::GetParData(int nIdx)
{
	return m_apParData[nIdx];
}

//=============================================================================
//    �G�~�b�^�f�[�^���擾���鏈��
//=============================================================================
CEmitterData *CEffectManager::GetEmitterData(int nIdx)
{
	return m_apEmitterData[nIdx];
}

//=============================================================================
//    �e�N�X�`���Ǌ��N���X�ւ̃|�C���^���擾���鏈��
//=============================================================================
CTextureManager *CEffectManager::GetTexManager(void)
{
	return m_pTextureManager;
}

//=============================================================================
//    �G�~�b�^�f�[�^�̐����擾���鏈��
//=============================================================================
int CEffectManager::GetNumEmitterData(void)
{
	return m_nNumEmitterData;
}

//=============================================================================
//    �p�[�e�B�N���f�[�^�̐����擾���鏈��
//=============================================================================
int CEffectManager::GetNumParData(void)
{
	return m_nNumParData;
}