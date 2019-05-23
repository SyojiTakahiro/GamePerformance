//*****************************************************************************
//
//     ���U���g�̏���[result.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "result.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "renderer.h"
#include "logo.h"
#include "number.h"
#include "ranking.h"
#include "map.h"

#include "debuglog.h"
#include "cylinder.h"
#include "dome.h"
#include "shadow.h"
#include "field.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define RESULT_LOGO_TEXTURENAME0	"data/TEXTURE/RESULT/Result.png"			    // ���S�̃e�N�X�`����
#define RESULT_LOGO_TEXTURENAME1	"data/TEXTURE/RESULT/ClearStage.png"	        // ���S�̃e�N�X�`����
#define RESULT_LOGO_TEXTURENAME2	"data/TEXTURE/RESULT/ClearBonus.png"	        // ���S�̃e�N�X�`����
#define RESULT_LOGO_TEXTURENAME3	"data/TEXTURE/RESULT/TotalScore.png"	        // ���S�̃e�N�X�`����
#define RESULT_LOGO_TEXTURENAME4	"data/TEXTURE/RESULT/Multiply.png"	             // ���S�̃e�N�X�`����

#define MAP_FILENAME				"MAP_FILENAME"									// �}�b�v�t�@�C����
#define INITFILENAME				"data/TEXT/MODE/game.ini"						// �Q�[���̃V�X�e���t�@�C����

#define RESULT_MODE_CHANGE_TIMING	(800)	// �����L���O��ʂɑJ�ڂ���܂ł̎���
#define RESULT_LOGO_POS				(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 50.0f, 0.0f))				// ���U���g���S�̈ʒu
#define RESULT_TOTAL_LOGO_POS		(D3DXVECTOR3(400.0f, 530.0f, 0.0f))	// �e���ڃ��S�̈ʒu
#define RESULT_ELSE_LOGO_POS		(D3DXVECTOR3(400.0f, 40.0f + (nCntLogo * 150.0f), 0.0f))	// �e���ڃ��S�̈ʒu
#define RESULT_LOGO_PRESS_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2, 650.0f, 0.0f))				// �v���X���S�̈ʒu

#define RESULT_LOGO_SIZE			(25)	// ���U���g���S�̃T�C�Y
#define RESULT_ELSE_LOGO_SIZE		(100)	// �e���ڃ��S�̃T�C�Y
#define RESULT_LOGO_PRESS_SIZE		(50)	// �v���X���S�̃T�C�Y

#define RESULT_NUMBER_WIDTH			(40.0f)	// �����̕�
#define RESULT_NUMBER_HEIGHT		(35.0f)	// �����̍���

#define RESULT_NUMBER_COLOR			(D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f))	// �����̐F

#define RESULT_STAGE_CLEAR_BONUS    (10000)

#define RESULT_SCORE_SECOND			(5)		// ���v�X�R�A���\�������܂ł̕b��

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
int CResult::m_nNumClearStage = 0;	// �N���A�����X�e�[�W��
int CResult::m_nStageBonus = 0;		// �N���A�����X�e�[�W�̃{�[�i�X
LPDIRECT3DTEXTURE9 CResult::m_apTexture[CResult::LOGO_MAX] = {};	// �e�N�X�`���̃|�C���^
LPDIRECT3DTEXTURE9 CResult::m_apTexture2 = {};	// �e�N�X�`���̃|�C���^

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CResult::CResult(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	for (int nCntLogo = 0; nCntLogo < LOGO_MAX; nCntLogo++)
	{// ���S�̃N���A
		m_apLogo[nCntLogo] = NULL;
	}

	for (int nCntNumber = 0; nCntNumber < NUMBER_MAX; nCntNumber++)
	{// �����̃|�C���^�ϐ����N���A
		m_apNumber[nCntNumber] = NULL;
	}

	m_pMulti = NULL;
	m_pMap = NULL;			// �}�b�v
	m_nScoreTotal = 0;		// �X�R�A�̍��v
	m_pLogoPress = NULL;	// �v���X���S�̃|�C���^�ϐ�
	m_nCounter = 0;			// �J�ڂ��Ǘ�����J�E���^�[
	m_bChange = false;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CResult::~CResult()
{

}

//=============================================================================
//    ��������
//=============================================================================
CResult *CResult::Create(void)
{
	CResult *pResult = NULL;  // �Q�[���N���X�^�̃|�C���^
	if (pResult == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pResult = new CResult;
		if (pResult != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pResult->Init();
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
	return pResult;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CResult::Init(void)
{
	// �e�N�X�`���̓ǂݍ���
	CLogoPress::Load();   // PRESSENTER
	CNumber::Load();      // ����
	CCylinder::Load();    // �R
	CDome::Load();        // ��
	CField::Load();       // �t�B�[���h
	CShadow::Load();      // �e

	FILE *pFile = NULL;
	char aString[256];
	char aMapFileName[256];
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(INITFILENAME, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// �e�L�X�g���Ō�̍s�ɓ��B����܂œǂݍ��݂𑱂���
				if (strcmp(&aString[0], MAP_FILENAME) == 0)
				{// �}�b�v�̃X�N���v�g�t�@�C����������
					fscanf(pFile, "%s %s", &aString[0], &aMapFileName[0]);
				}
			}
		}
	}

	// �}�b�v�̐���
	if (m_pMap == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pMap = CMap::Create(aMapFileName);
	}

	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// �e�N�X�`���̐���
			D3DXCreateTextureFromFile(pDevice, RESULT_LOGO_TEXTURENAME0, &m_apTexture[LOGO_RESULT]);
			D3DXCreateTextureFromFile(pDevice, RESULT_LOGO_TEXTURENAME1, &m_apTexture[LOGO_CLEAR_STAGE]);
			D3DXCreateTextureFromFile(pDevice, RESULT_LOGO_TEXTURENAME2, &m_apTexture[LOGO_STAGE_BONUS]);
			D3DXCreateTextureFromFile(pDevice, RESULT_LOGO_TEXTURENAME3, &m_apTexture[LOGO_TOTAL]);
			D3DXCreateTextureFromFile(pDevice, RESULT_LOGO_TEXTURENAME4, &m_apTexture2);

			for (int nCntLogo = 0; nCntLogo < LOGO_MAX; nCntLogo++)
			{
				// ���S�̐���
				if (nCntLogo == LOGO_RESULT)
				{// ���U���g�̃��S
					m_apLogo[nCntLogo] = CLogo::Create(RESULT_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), RESULT_LOGO_SIZE, 6);
				}
				else
				{// �e���ڂ̃��S
					m_apLogo[nCntLogo] = CLogo::Create(RESULT_ELSE_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), RESULT_ELSE_LOGO_SIZE, 6);
				}

				// ���_�o�b�t�@�̎擾
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apLogo[nCntLogo]->GetVtxBuff();

				// ���_���
				VERTEX_2D *pVtx;

				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// ���_���W�̐ݒ�
				if (nCntLogo == LOGO_RESULT)
				{// ���U���g�̃��S
					pVtx[0].pos = D3DXVECTOR3(-RESULT_LOGO_SIZE * 8.0f, -RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_LOGO_POS;
					pVtx[1].pos = D3DXVECTOR3(RESULT_LOGO_SIZE * 8.0f, -RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_LOGO_POS;
					pVtx[2].pos = D3DXVECTOR3(-RESULT_LOGO_SIZE * 8.0f, RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_LOGO_POS;
					pVtx[3].pos = D3DXVECTOR3(RESULT_LOGO_SIZE * 8.0f, RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_LOGO_POS;
				}
				else if (nCntLogo == LOGO_TOTAL)
				{
					pVtx[0].pos = D3DXVECTOR3(-RESULT_LOGO_SIZE * 8.0f, -RESULT_LOGO_SIZE * 4.0f, 0.0f) + RESULT_TOTAL_LOGO_POS;
					pVtx[1].pos = D3DXVECTOR3(RESULT_LOGO_SIZE * 8.0f, -RESULT_LOGO_SIZE * 4.0f, 0.0f) + RESULT_TOTAL_LOGO_POS;
					pVtx[2].pos = D3DXVECTOR3(-RESULT_LOGO_SIZE * 8.0f, RESULT_LOGO_SIZE * 4.0f, 0.0f) + RESULT_TOTAL_LOGO_POS;
					pVtx[3].pos = D3DXVECTOR3(RESULT_LOGO_SIZE * 8.0f, RESULT_LOGO_SIZE * 4.0f, 0.0f) + RESULT_TOTAL_LOGO_POS;
				}
				else
				{// �e���ڂ̃��S
					pVtx[0].pos = D3DXVECTOR3(-RESULT_LOGO_SIZE * 5.6f, -RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_ELSE_LOGO_POS;
					pVtx[1].pos = D3DXVECTOR3(RESULT_LOGO_SIZE * 5.6f, -RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_ELSE_LOGO_POS;
					pVtx[2].pos = D3DXVECTOR3(-RESULT_LOGO_SIZE * 5.6f, RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_ELSE_LOGO_POS;
					pVtx[3].pos = D3DXVECTOR3(RESULT_LOGO_SIZE * 5.6f, RESULT_LOGO_SIZE * 2.0f, 0.0f) + RESULT_ELSE_LOGO_POS;
				}

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();

				// ���_�o�b�t�@�̐ݒ�
				m_apLogo[nCntLogo]->SetVtxBuff(pVtxBuff);

				// �e�N�X�`����ݒ�
				m_apLogo[nCntLogo]->BindTexture(m_apTexture[nCntLogo]);
			}
		}
	}

	// �����𐶐�����
	if (m_apNumber[NUMBER_CLEAR_STAGE] == NULL)
	{// �����̐���
		int nNumClearStage;
		nNumClearStage = (int)log10f((float)m_nNumClearStage) + 1;			// ���������߂�
		if (nNumClearStage <= 0) { nNumClearStage = 1; }					// ������0�̂Ƃ��P�ɂ���
		m_apNumber[NUMBER_CLEAR_STAGE] = new CNumber*[nNumClearStage];		// �i���o�[�̐���

		for (int nCntLogo = 0; nCntLogo < nNumClearStage; nCntLogo++)
		{// ���������[�v
			int nNumber = m_nNumClearStage % (int)powf(10.0f, (nCntLogo + 1) * 1.0f) / (int)powf(10.0f, nCntLogo * 1.0f);
			m_apNumber[NUMBER_CLEAR_STAGE][nCntLogo] = CNumber::Create(D3DXVECTOR3(RESULT_ELSE_LOGO_POS.x + 300.0f + ((nNumClearStage - 1) * (RESULT_NUMBER_WIDTH * 1.5f)) - (nCntLogo * RESULT_NUMBER_WIDTH * 1.5f), 180.0f, 0.0f),
				RESULT_NUMBER_COLOR, RESULT_NUMBER_WIDTH, RESULT_NUMBER_HEIGHT, nNumber, 6);

			// �����̐ݒ�
			m_apNumber[NUMBER_CLEAR_STAGE][nCntLogo]->SetNumber(nNumber);
		}
	}
	if (m_apNumber[NUMBER_STAGE_BONUS] == NULL)
	{// �����̐���
		int nStageBonus;
		nStageBonus = (int)log10f((float)m_nStageBonus) + 1;				// ���������߂�
		if (nStageBonus <= 0) { nStageBonus = 1; }							// ������0�̂Ƃ��P�ɂ���
		m_apNumber[NUMBER_STAGE_BONUS] = new CNumber*[nStageBonus];		// �i���o�[�̐���

		for (int nCntLogo = 0; nCntLogo < nStageBonus; nCntLogo++)
		{// ���������[�v
			int nNumber = m_nStageBonus % (int)powf(10.0f, (nCntLogo + 1) * 1.0f) / (int)powf(10.0f, nCntLogo * 1.0f);
			m_apNumber[NUMBER_STAGE_BONUS][nCntLogo] = CNumber::Create(D3DXVECTOR3(RESULT_ELSE_LOGO_POS.x + 300.0f + ((nStageBonus - 1) * (RESULT_NUMBER_WIDTH * 1.5f)) - (nCntLogo * RESULT_NUMBER_WIDTH * 1.5f), 340.0f, 0.0f),
				RESULT_NUMBER_COLOR, RESULT_NUMBER_WIDTH, RESULT_NUMBER_HEIGHT, nNumber, 6);

			// �����̐ݒ�
			m_apNumber[NUMBER_STAGE_BONUS][nCntLogo]->SetNumber(nNumber);
		}
	}


	if (m_apNumber[NUMBER_CLEAR_STAGE_BONUS] == NULL)
	{// �����̐���
		int nStageBonus;
		nStageBonus = (int)log10f((float)(RESULT_STAGE_CLEAR_BONUS)) + 1;	    // ���������߂�
		if (nStageBonus <= 0) { nStageBonus = 1; }							    // ������0�̂Ƃ��P�ɂ���
		m_apNumber[NUMBER_CLEAR_STAGE_BONUS] = new CNumber*[nStageBonus];		// �i���o�[�̐���

		for (int nCntLogo = 0; nCntLogo < nStageBonus; nCntLogo++)
		{// ���������[�v
			int nNumber = RESULT_STAGE_CLEAR_BONUS % (int)powf(10.0f, (nCntLogo + 1) * 1.0f) / (int)powf(10.0f, nCntLogo * 1.0f);
			m_apNumber[NUMBER_CLEAR_STAGE_BONUS][nCntLogo] = CNumber::Create(D3DXVECTOR3(RESULT_ELSE_LOGO_POS.x + 500.0f + ((nStageBonus - 1) * (RESULT_NUMBER_WIDTH * 1.5f)) - (nCntLogo * RESULT_NUMBER_WIDTH * 1.5f), 180.0f, 0.0f),
				RESULT_NUMBER_COLOR, RESULT_NUMBER_WIDTH, RESULT_NUMBER_HEIGHT, nNumber, 6);

			// �����̐ݒ�
			m_apNumber[NUMBER_CLEAR_STAGE_BONUS][nCntLogo]->SetNumber(nNumber);
		}
	}

	if (m_pMulti == NULL)
	{
		m_pMulti = CScene2DSquare::Create(D3DXVECTOR3(810.0f, 180.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 50.0f, 50.0f,6);
		if (m_pMulti != NULL)
		{
			m_pMulti->BindTexture(m_apTexture2);
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CResult::Uninit(void)
{
	// �e�N�X�`���̔j��
	CLogoPress::UnLoad();   // PRESSENTER
	CNumber::UnLoad();      // ����
	CCylinder::UnLoad();    // �R
	CDome::UnLoad();        // ��
	CField::UnLoad();       // �t�B�[���h
	CShadow::UnLoad();      // �e

	for (int nCntLogo = 0; nCntLogo < LOGO_MAX; nCntLogo++)
	{
		if (m_apLogo[nCntLogo] != NULL)
		{// �v���X���S�̔j��
			m_apLogo[nCntLogo]->Uninit();
			m_apLogo[nCntLogo] = NULL;
		}

		if (m_apNumber[nCntLogo] != NULL)
		{// �v���X���S�̔j��
			int nDigits = 0;

			// �g���Ă��錅�������߂�
			if (nCntLogo == NUMBER_TOTAL)		{ nDigits = (int)log10f((float)m_nScoreTotal) + 1; }	// ���v�X�R�A
			if (nCntLogo == NUMBER_CLEAR_STAGE)	{ nDigits = (int)log10f((float)m_nNumClearStage) + 1; }	// �N���A�X�e�[�W��
			if (nCntLogo == NUMBER_STAGE_BONUS)	{ nDigits = (int)log10f((float)m_nStageBonus) + 1; }	// �X�e�[�W�{�[�i�X

			if (nDigits <= 0)
			{// 0�ȉ���������1�ɂ���
				nDigits = 1;
			}

			for (int nCntNumber = 0; nCntNumber < nDigits; nCntNumber++)
			{// ���������m�ۂ��ꂽ�����[�v
				m_apNumber[nCntLogo][nCntNumber]->Uninit();
				m_apNumber[nCntLogo][nCntNumber] = NULL;
			}

			delete[] m_apNumber[nCntLogo];
			m_apNumber[nCntLogo] = NULL;
		}
	}

	if (m_pLogoPress != NULL)
	{// �v���X���S�̔j��
		m_pLogoPress->Uninit();
		m_pLogoPress = NULL;
	}

	// �}�b�v�̔j��
	if (m_pMap != NULL)
	{// ���������m�ۂ���Ă���
		m_pMap->Uninit();

		// �������̊J��
		delete m_pMap;
		m_pMap = NULL;
	}

	// �����̊J��
	if (m_pMulti != NULL)
	{
		m_pMulti->Uninit();
		m_pMulti = NULL;
	}

	// �����L���O�X�R�A�̐ݒ�
	CRanking::SetRankingScore(m_nScoreTotal);

	// ���U���g�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CResult::Update(void)
{
	m_nCounter++;  // �J�E���^�[��i�߂�

	// �X�R�A�̍��v���o��
	const int nScoreTotal = (m_nNumClearStage * RESULT_STAGE_CLEAR_BONUS) + m_nStageBonus;
	int nScoreTotalOld = (int)log10f((float)m_nScoreTotal) + 1;		// ���v�X�R�A�����̕ۑ�

	if (m_nCounter % RESULT_MODE_CHANGE_TIMING == 0)
	{// ��ʑJ�ڂ���^�C�~���O�ɂȂ���
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			CManager::GetFade()->SetFade(CManager::MODE_RANKING);
			CRanking::SetRankingScore(nScoreTotal);		// �����L���O�ɃX�R�A��ݒ�
		}
	}
	else
	{// ��ʑJ�ڂ���^�C�~���O�ł͂Ȃ�
		if (m_bChange == true)
		{
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
				if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
				{// ����{�^���������ꂽ
					CManager::GetFade()->SetFade(CManager::MODE_RANKING);
				}
				else
				{// �L�[�{�[�h�̌���{�^����������Ă��Ȃ�
					for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_12; nCntButton++)
					{// ��������{�^�����̐������J��Ԃ�
						if (CManager::GetJoyStick()->GetJoyStickDevice(0)->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
						{// �{�^���������ꂽ
							CManager::GetFade()->SetFade(CManager::MODE_RANKING);
						}
					}
				}
			}
		}
	}

	if (m_nCounter < (300))
	{// �w�肵���b���܂Ń����_��
		m_nScoreTotal = rand() % 90000;
	}
	else
	{// ���v�X�R�A��\������
		m_nScoreTotal = nScoreTotal;
		m_bChange = true;

		// �v���X���S�̐���
		if (m_pLogoPress == NULL)
		{// ��������
			m_pLogoPress = CLogoPress::Create(RESULT_LOGO_PRESS_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), RESULT_LOGO_PRESS_SIZE, 3);

			if (m_pLogoPress != NULL)
			{// �����ɐ���
			    // ���_�o�b�t�@�̎擾
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pLogoPress->GetVtxBuff();

				// ���_���
				VERTEX_2D *pVtx;

				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// ���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-RESULT_LOGO_PRESS_SIZE * 5.0f, -RESULT_LOGO_PRESS_SIZE, 0.0f) + RESULT_LOGO_PRESS_POS;
				pVtx[1].pos = D3DXVECTOR3(RESULT_LOGO_PRESS_SIZE * 5.0f, -RESULT_LOGO_PRESS_SIZE, 0.0f) + RESULT_LOGO_PRESS_POS;
				pVtx[2].pos = D3DXVECTOR3(-RESULT_LOGO_PRESS_SIZE * 5.0f, RESULT_LOGO_PRESS_SIZE, 0.0f) + RESULT_LOGO_PRESS_POS;
				pVtx[3].pos = D3DXVECTOR3(RESULT_LOGO_PRESS_SIZE * 5.0f, RESULT_LOGO_PRESS_SIZE, 0.0f) + RESULT_LOGO_PRESS_POS;

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();

				// ���_�o�b�t�@�̐ݒ�
				m_pLogoPress->SetVtxBuff(pVtxBuff);
			}
		}
	}

	// ���������߂�
	int nDigitsScoreTotal = (int)log10f((float)m_nScoreTotal) + 1;

	if (nDigitsScoreTotal <= 0)
	{// ������0�̂Ƃ��P�ɂ���
		nDigitsScoreTotal = 1;
	}

	if (m_apNumber[NUMBER_TOTAL] != NULL)
	{// ���v�X�R�A���g���Ă���Ƃ�
		for(int nCnt = 0; nCnt < nScoreTotalOld;nCnt++)
		{// ���������[�v
			if (m_apNumber[NUMBER_TOTAL][nCnt] != NULL)
			{
				m_apNumber[NUMBER_TOTAL][nCnt]->Uninit();
				m_apNumber[NUMBER_TOTAL][nCnt] = NULL;
			}
		}

		// �������̊J��
		delete[] m_apNumber[NUMBER_TOTAL];
		m_apNumber[NUMBER_TOTAL] = NULL;
	}

	if (m_apNumber[NUMBER_TOTAL] == NULL)
	{// ����������̂Ƃ�
		m_apNumber[NUMBER_TOTAL] = new CNumber*[nDigitsScoreTotal];	// �i���o�[�̐���

		if (m_apNumber[NUMBER_TOTAL] != NULL)
		{// �����ɐ���
			for (int nCntLogo = 0; nCntLogo < nDigitsScoreTotal; nCntLogo++)
			{// ���������[�v
				int nNumber = m_nScoreTotal % (int)powf(10.0f, (nCntLogo + 1) * 1.0f) / (int)powf(10.0f, nCntLogo * 1.0f);
				m_apNumber[NUMBER_TOTAL][nCntLogo] = CNumber::Create(D3DXVECTOR3(RESULT_ELSE_LOGO_POS.x + 300.0f + ((nDigitsScoreTotal - 1) * (RESULT_NUMBER_WIDTH * 1.5f)) - (nCntLogo * RESULT_NUMBER_WIDTH * 1.5f), 540, 0.0f),
					D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), RESULT_NUMBER_WIDTH, RESULT_NUMBER_HEIGHT, nNumber, 6);

				// �����̐ݒ�
				m_apNumber[NUMBER_TOTAL][nCntLogo]->SetNumber(nNumber);
			}
		}
	}

	CDebugProc::Print("���U���g�Ȃ�\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CResult::Draw(void)
{
}

//=============================================================================
//    �N���A�����X�e�[�W���̐ݒ�
//=============================================================================
const void CResult::SetNumClearStage(int nNumClearStage)
{
	m_nNumClearStage = nNumClearStage;
}

//=============================================================================
//    �N���A�����X�e�[�W�{�[�i�X�̐ݒ�
//=============================================================================
const void CResult::SetStageBonus(int nStageBonus)
{
	m_nStageBonus = nStageBonus;
}