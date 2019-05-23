//*****************************************************************************
//
//     �����L���O�̏���[ranking.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "ranking.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "fade.h"
#include "scene2D.h"
#include "textureManager.h"
#include "functionlib.h"
#include "map.h"

#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define RANKING_SYSTEM_FILENAME    "data/TEXT/MODE/ranking.ini"                // �����L���O�̏��������X�N���v�g�t�@�C����
#define RANKING_MODECHANGE_TIMING  (600)                                       // ��ʑJ�ڂ������ōs���^�C�~���O

// �l��ǂݎ��p�X��
#define NUM_TEXTURE                "NUM_TEXTURE = "                            // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME           "TEXTURE_FILENAME = "                       // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define MAP_FILENAME               "MAP_FILENAME = "                           // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����

// �����L���O�X�R�A�p�̃}�N��
#define RANKING_SCORE_POS_INI      (D3DXVECTOR3(1000.0f,180.0f,0.0f))          // �����L���O�X�R�A�̍��W(�����l)
#define RANKING_SCORE_POS_MAX      (1080.0f)                                   // �����L���O�X�R�A�̍��W�̍ő�l
#define RANKING_SCORE_COL_INI      (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))            // �����L���O�X�R�A�̐F(�����l)
#define RANKING_SCORE_WIDTH_INI    (70.0f)                                     // �����L���O�X�R�A�̕�(�����l)
#define RANKING_SCORE_HEIGHT_INI   (54.0f)                                     // �����L���O�X�R�A�̍���(�����l)
#define RANKING_SCORE_MOVE_INI     (D3DXVECTOR3(-15.0f,0.0f,0.0f))             // �����L���O�X�R�A�̈ړ���(�����l)

// �����L���O���ʗp�̃}�N��
#define RANKING_RANK_POS_INI       (D3DXVECTOR3(200.0f,180.0f,0.0f))           // �����L���O���ʂ̍��W(�����l)
#define RANKING_RANK_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // �����L���O���ʂ̐F(�����l)
#define RANKING_RANK_WIDTH_INI     (180.0f)                                    // �����L���O���ʂ̕�(�����l)
#define RANKING_RANK_HEIGHT_INI    (70.0f)                                     // �����L���O���ʂ̍���(�����l)
#define RANKING_RANK_POS_MAX       (260.0f)                                    // �����L���O���ʂ̍��W�̍ő�l

// �����L���O���S�p�̃}�N��
#define RANKING_LOGO_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 2,65.0f,0.0f))  // �����L���O���S�̍��W(�����l)
#define RANKING_LOGO_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))            // �����L���O���S�̐F(�����l)
#define RANKING_LOGO_WIDTH_INI     (400.0f)                                    // �����L���O���S�̕�(�����l)
#define RANKING_LOGO_HEIGHT_INI    (80.0f)                                     // �����L���O���S�̍���(�����l)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
CRanking::STATE CRanking::m_State[MAX_RANKING] = {};                           // �����L���O�X�R�A�̏��
int CRanking::m_nRankingScore[MAX_RANKING] = {100000,80000,60000,40000,20000}; // �����L���O�̃X�R�A

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRanking::CRanking(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐������J��Ԃ�
		m_aDigit[nCntRanking] = 0;                            // �����L���O�X�R�A�̌���
		m_apNumber[nCntRanking] = NULL;                       // �����L���O�X�R�A�\���p�̃|���S��
		m_Move[nCntRanking] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �����L���O�X�R�A�̈ړ���
		m_apRankingRank[nCntRanking] = NULL;                  // �����L���O���ʕ\���p�̃|���S��
	}
	m_pMap = NULL;              // �}�b�v�N���X�ւ̃|�C���^
	m_pTextureManager = NULL;   // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_pRankingLogo = NULL;      // �����L���O���S�p�\���p�̃|���S��
	m_nCounter = 0;             // �J�ڂ��Ǘ�����J�E���^�[
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRanking::~CRanking()
{

}

//=============================================================================
//    ��������
//=============================================================================
CRanking *CRanking::Create(void)
{
	CRanking *pRanking = NULL;  // �����L���O�N���X�^�̃|�C���^
	if (pRanking == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pRanking = new CRanking;
		if (pRanking != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pRanking ->Init();
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
	return pRanking;
}

//=============================================================================
//    �����L���O�X�R�A�̏�����
//=============================================================================
void CRanking::RankingScoreInit(void)
{

}

//=============================================================================
//    �����L���O�X�R�A�̐ݒ�
//=============================================================================
void CRanking::SetRankingScore(int nScore)
{
	int nScoreChange = nScore; // ����ւ���X�R�A
	int nScoreOld = 0;         // �����L���O�X�R�A�̈ꎞ�I�Ȋi�[�ꏊ
	bool bSet = false;         // �����L���O���X�V���ꂽ���ǂ���

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�̌������J��Ԃ�
		if (m_nRankingScore[nCntRanking] < nScoreChange)
		{// �o�^����Ă���X�R�A������ւ���X�R�A���傫��
			nScoreOld = m_nRankingScore[nCntRanking];    // ���̃X�R�A���i�[
			m_nRankingScore[nCntRanking] = nScoreChange; // �X�R�A�X�V
			nScoreChange = nScoreOld;                    // ���̃X�R�A�����ւ���Ώۂ�
			if (bSet == false)
			{// �X�V����Ă��Ȃ�������
				m_State[nCntRanking] = STATE_UPDATE;     // �X�V���ꂽ��Ԃ�
				bSet = true;                             // �X�V������Ԃ�
			}
		}
	}
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRanking::Init(void)
{
	// �����L���O�̏����������O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(RANKING_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                         // 1�s���ǂݎ��p
			char *pStrCur = NULL;                       // ���݂̐擪�|�C���^
			char *pStr = NULL;                          // �擪�|�C���^�ۑ��p
			int nNumTex = 0;                            // �e�N�X�`����ǂݍ��ސ�
			int nCntTex = 0;                            // �e�N�X�`����ǂݍ��񂾉�
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // �e�N�X�`���ǂݎ��p
			char aTexFileName[256] = "\0";              // �e�N�X�`���̃t�@�C����
			char aMapFileName[256] = "\0";              // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����

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
							else if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// �}�b�v�f�[�^�̃X�N���v�g�t�@�C����������
							    // �}�b�v�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(aMapFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}

					// �}�b�v�̐���
					m_pMap = CMap::Create(aMapFileName);

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
				else
				{// ���������m�ۂł��Ȃ�����
					return E_FAIL;
				}
			}
			else
			{// ���������m�ۂł����ԂłȂ�
				return E_FAIL;
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

	// �����L���O�X�R�A�̐���
	D3DXVECTOR3 NumberPos = RANKING_SCORE_POS_INI;     // ���W
	D3DXCOLOR NumberCol = RANKING_SCORE_COL_INI;       // �F
	float fNumberWidth = RANKING_SCORE_WIDTH_INI;      // ��
	float fNumberHeight = RANKING_SCORE_HEIGHT_INI;    // ����
	int nNumber = 0;                                   // �\�����鐔���̌v�Z�p
	CNumber::STATE NumberState = CNumber::STATE_NONE;  // ���

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐������J��Ԃ�
		// ��Ԃ̐ݒ�
		if (m_State[nCntRanking] == STATE_UPDATE)
		{// �X�V���ꂽ��Ԃ�������
			NumberState = CNumber::STATE_FLASH;
		}

		// �����̌v�Z
		m_aDigit[nCntRanking] = (int)log10f((float)m_nRankingScore[nCntRanking]) + 1;  // ���������߂�
		if (m_aDigit[nCntRanking] <= 0) { m_aDigit[nCntRanking] = 1; }	               // 0�ȉ��̂Ƃ�1�ɂ���
		m_apNumber[nCntRanking] = new CNumber*[m_aDigit[nCntRanking]];		           // �������������m��
		if (m_apNumber[nCntRanking] != NULL)
		{// ���������m�ۂ���Ă���
			for (int nCntScore = 0; nCntScore < m_aDigit[nCntRanking]; nCntScore++)
			{// �����L���O�X�R�A�̌������J��Ԃ�
				// �\�����鐔���̌v�Z
				nNumber = m_nRankingScore[nCntRanking] % ((int)powf(10.0f, (float)nCntScore) * 10) / (int)powf(10.0f, (float)nCntScore);

				// �����|���S���̃C���X�^���X��������
				m_apNumber[nCntRanking][nCntScore] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, NumberState, nNumber,3,6);
				if (m_apNumber[nCntRanking][nCntScore] != NULL)
				{// ���������m�ۂł���
					m_apNumber[nCntRanking][nCntScore]->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_0));
				}

				// ���ɐi�߂�
				NumberPos.x -= RANKING_SCORE_WIDTH_INI + (RANKING_SCORE_WIDTH_INI * 0.5f);
			}
		}
		// ���ɐi�߂�
		NumberPos = RANKING_SCORE_POS_INI;
		//NumberPos.x += 500.0f * (nCntRanking + 1);
		NumberPos.y += (RANKING_SCORE_HEIGHT_INI + (RANKING_SCORE_HEIGHT_INI * 1.2f)) * (nCntRanking + 1);

		// ��Ԃ�߂��Ă���
		NumberState = CNumber::STATE_NONE;
	}

	// �����L���O���ʃ|���S���̃C���X�^���X����
	D3DXVECTOR3 RankPos = RANKING_RANK_POS_INI;   // ���W
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�̌������J��Ԃ�
		if (m_apRankingRank[nCntRanking] == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_apRankingRank[nCntRanking] = CScene2D::Create(RankPos, RANKING_RANK_COL_INI, RANKING_RANK_WIDTH_INI, RANKING_RANK_HEIGHT_INI,0.0f,6);
			if (m_apRankingRank[nCntRanking] != NULL)
			{// ���������m�ۂł���
				m_apRankingRank[nCntRanking]->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_1));

				// �e�N�X�`�����W�����炷
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apRankingRank[nCntRanking]->GetVtxBuff();
				if(pVtxBuff != NULL)
				{// ���_�o�b�t�@���擾�ł���
				    // ���_���
					VERTEX_2D *pVtx;

					// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// �e�N�X�`�����W
					pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + (0.2f * nCntRanking));
					pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + (0.2f * nCntRanking));
					pVtx[2].tex = D3DXVECTOR2(0.0f, 0.2f + (0.2f * nCntRanking));
					pVtx[3].tex = D3DXVECTOR2(1.0f, 0.2f + (0.2f * nCntRanking));

					// ���_�o�b�t�@���A�����b�N����
					pVtxBuff->Unlock();
				}
			}
			RankPos = RANKING_RANK_POS_INI;
			//RankPos.x += 500.0f * (nCntRanking + 1);
			RankPos.y += (RANKING_RANK_HEIGHT_INI + (RANKING_RANK_HEIGHT_INI * 0.75f)) * (nCntRanking + 1);
		}
	}

	// �����L���O���S�̐���
	if (m_pRankingLogo == NULL)
	{
		m_pRankingLogo = CScene2D::Create(RANKING_LOGO_POS_INI, RANKING_LOGO_COL_INI, RANKING_LOGO_WIDTH_INI, RANKING_LOGO_HEIGHT_INI, 0.0f, 6);
		if (m_pRankingLogo != NULL)
		{// ���������m�ۂł���
			m_pRankingLogo->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_2));
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRanking::Uninit(void)
{
	// �}�b�v�N���X���J������
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();

		delete m_pMap;
		m_pMap = NULL;
	}

	// �e�N�X�`���Ǌ��N���X���J������
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// �����L���O�X�R�A�̔j��
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐������J��Ԃ�
		if (m_apNumber[nCntRanking] != NULL)
		{// ���������m�ۂ���Ă���
			for (int nCntDigit = 0; nCntDigit < m_aDigit[nCntRanking]; nCntDigit++)
			{// �����L���O�X�R�A�̌������J��Ԃ�
				if (m_apNumber[nCntRanking][nCntDigit] != NULL)
				{// ���������m�ۂ���Ă���
					m_apNumber[nCntRanking][nCntDigit]->Uninit();
					m_apNumber[nCntRanking][nCntDigit] = NULL;
				}
			}
			delete m_apNumber[nCntRanking];
			m_apNumber[nCntRanking] = NULL;
		}

		// ��Ԃ�߂��Ă���
		m_State[nCntRanking] = STATE_NONE;
	}

	// �����L���O���S�̔j��
	if (m_pRankingLogo != NULL)
	{
		m_pRankingLogo->Uninit();
		m_pRankingLogo = NULL;
	}

	// �����L���O���ʂ̔j��
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�̌������J��Ԃ�
		if (m_apRankingRank[nCntRanking] != NULL)
		{// ���������m�ۂ���Ă���
			m_apRankingRank[nCntRanking]->Uninit();
			m_apRankingRank[nCntRanking] = NULL;
		}
	}

	// �����L���O�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRanking::Update(void)
{
	m_nCounter++;  // �J�E���^�[��i�߂�

	if (m_nCounter >= RANKING_MODECHANGE_TIMING)
	{// �J�E���^�[������̒l�𒴂���
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
		}
	}
	else
	{// �J�E���^�[���܂�����̒l�𒴂��Ă��Ȃ�
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// ����{�^���������ꂽ
				CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			}
			else
			{// �L�[�{�[�h�̌���{�^����������Ă��Ȃ�
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_7; nCntButton++)
				{// ��������{�^�����̐������J��Ԃ�
					if (CManager::GetJoyStick()->GetJoyStickDevice()->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// �{�^���������ꂽ
						CManager::GetFade()->SetFade(CManager::MODE_TITLE);
					}
				}
			}
		}
	}

	CDebugProc::Print("�����L���O���\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRanking::Draw(void)
{

}