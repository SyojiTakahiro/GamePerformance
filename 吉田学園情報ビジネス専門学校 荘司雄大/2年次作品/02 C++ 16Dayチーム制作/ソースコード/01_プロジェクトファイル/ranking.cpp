//*****************************************************************************
//
//     �����L���O�̏���[ranking.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "ranking.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "number.h"
#include "logo.h"
#include "input.h"
#include "scene2DSquare.h"
#include "scene2D.h"
#include "map.h"

#include "debuglog.h"
#include "cylinder.h"
#include "dome.h"
#include "shadow.h"
#include "field.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define RANKING_RANK_TEXTURENAME	"data/TEXTURE/RANKING/Rank.png"	        	// �����L���O���ʗp�̃e�N�X�`���̃t�@�C����
#define RANKING_LOGO_TEXTURENAME	"data/TEXTURE/RANKING/RankingLogo.png"		// �����L���O���ʗp�̃e�N�X�`���̃t�@�C����
#define MAP_FILENAME				"MAP_FILENAME"								// �}�b�v�t�@�C����
#define INITFILENAME				"data/TEXT/MODE/game.ini"					// �Q�[���̃V�X�e���t�@�C����

#define RANKING_SCORE_INI          (100000 - nCntRanking * 20000)				// �����L���O�X�R�A�̌v�Z��

#define RANKING_MODE_CHANGETIMING  (600)										// �ǂꂭ�炢�̎��Ԃŏ���ɑJ�ڂ��邩

#define RANKING_SCORE_POS_INI      (D3DXVECTOR3(960.0f,180.0f,0.0f))			// �����L���O�X�R�A�̍��W(�����l)
#define RANKING_SCORE_POS_MAX      (1080.0f)									// �����L���O�X�R�A�̍��W�̍ő�l
#define RANKING_SCORE_COL_INI      (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))				// �����L���O�X�R�A�̐F(�����l)
#define RANKING_SCORE_WIDTH_INI    (60.0f)										// �����L���O�X�R�A�̕�(�����l)
#define RANKING_SCORE_HEIGHT_INI   (55.0f)										// �����L���O�X�R�A�̍���(�����l)
#define RANKING_SCORE_MOVE_INI     (D3DXVECTOR3(-15.0f,0.0f,0.0f))				// �����L���O�X�R�A�̈ړ���(�����l)

#define RANKING_RANK_POS_INI       (D3DXVECTOR3(360.0f,180.0f,0.0f))			// �����L���O���ʂ̍��W(�����l)
#define RANKING_RANK_COL_INI       (D3DXCOLOR(1.0f,1.0f,0.0f,1.0f))				// �����L���O���ʂ̐F(�����l)
#define RANKING_RANK_WIDTH_INI     (100.0f)										// �����L���O���ʂ̕�(�����l)
#define RANKING_RANK_HEIGHT_INI    (70.0f)										// �����L���O���ʂ̍���(�����l)
#define RANKING_RANK_POS_MAX       (260.0f)										// �����L���O���ʂ̍��W�̍ő�l

#define RANKING_LOGO_POS_INI       (D3DXVECTOR3(SCREEN_WIDTH / 2,50.0f,0.0f))	// �����L���O���S�̍��W(�����l)
#define RANKING_LOGO_COL_INI       (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))				// �����L���O���S�̐F(�����l)
#define RANKING_LOGO_SCALE_INI     (120.0f)										// �����L���O���S�̑傫��(�����l)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
CRanking::STATE CRanking::m_State[MAX_RANKING];     // �����L���O�X�R�A�̏��
int CRanking::m_nRankingScore[MAX_RANKING] = { 100000, 80000, 60000,40000,20000 };         // �����L���O�̃X�R�A
LPDIRECT3DTEXTURE9 CRanking::m_apTexture[MAX_RANKING_TEXTURE] = {};   // �����L���O���ʗp�̃e�N�X�`��
int CRanking::m_nScore = 0;		// �ŐV�̃X�R�A

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRanking::CRanking(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐������J��Ԃ�
		m_apNumber[nCntRanking] = NULL;			// �����L���O�X�R�A
		m_apSceneSquare[nCntRanking] = NULL;	// ���ʗp�̃e�N�X�`��
		m_aScoreDigits[nCntRanking] = 0;		// �X�R�A�̌���
	}

	m_pMap = NULL;							// �}�b�v�N���X�̃|�C���^�ϐ�
	m_pLogo = NULL;                         // �����L���O���S�N���X�^�̃|�C���^
	m_nCounter = 0;                         // ��ʑJ�ڊǗ��J�E���^�[
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
			pRanking->Init();
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
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐������J��Ԃ�
		m_nRankingScore[nCntRanking] = RANKING_SCORE_INI;
	}
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

	// �ŐV�̃X�R�A���L��
	m_nScore = nScore;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRanking::Init(void)
{
	// �I�u�W�F�N�g�̃^�C�v���w��
	SetObjType(OBJTYPE_RANKING);

	// �e�N�X�`���̓ǂ݂���
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
			D3DXCreateTextureFromFile(pDevice, RANKING_RANK_TEXTURENAME, &m_apTexture[0]);
			D3DXCreateTextureFromFile(pDevice, RANKING_LOGO_TEXTURENAME, &m_apTexture[1]);
		}
	}

	D3DXVECTOR3 NumberPos = RANKING_SCORE_POS_INI;   // ���W
	D3DXCOLOR NumberCol = RANKING_SCORE_COL_INI;     // �F
	float fNumberWidth = RANKING_SCORE_WIDTH_INI;    // ��
	float fNumberHeight = RANKING_SCORE_HEIGHT_INI;  // ����
	int nNumber = 0;                                 // �\�����鐔���̌v�Z�p

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐������J��Ԃ�
		m_aScoreDigits[nCntRanking] = (int)log10f((float)m_nRankingScore[nCntRanking]) + 1;	// ���������߂�
		if (m_aScoreDigits[nCntRanking] <= 0) { m_aScoreDigits[nCntRanking] = 1; }			// 0�ȉ��̂Ƃ�1�ɂ���
		m_apNumber[nCntRanking] = new CNumber*[m_aScoreDigits[nCntRanking]];				// �������������m��

		for (int nCntScore = 0; nCntScore < m_aScoreDigits[nCntRanking]; nCntScore++)
		{// �����L���O�X�R�A�̌������J��Ԃ�
		    // �\�����鐔���̌v�Z
			nNumber = m_nRankingScore[nCntRanking] % ((int)powf(10.0f, (float)nCntScore) * 10) / (int)powf(10.0f, (float)nCntScore);

			// �����|���S���̃C���X�^���X��������
			m_apNumber[nCntRanking][nCntScore] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, nNumber,6);

			// ���ɐi�߂�
			NumberPos.x -= RANKING_SCORE_WIDTH_INI + (RANKING_SCORE_WIDTH_INI * 0.5f);
		}
		// ���ɐi�߂�
		NumberPos = RANKING_SCORE_POS_INI;
		NumberPos.y += (RANKING_SCORE_HEIGHT_INI + (RANKING_SCORE_HEIGHT_INI * 1.2f)) * (nCntRanking + 1);
	}

	// �����L���O���S�̃C���X�^���X����
	if(m_pLogo == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pLogo = CLogo::Create(RANKING_LOGO_POS_INI, RANKING_LOGO_COL_INI, RANKING_LOGO_SCALE_INI, 6);
		m_pLogo->BindTexture(m_apTexture[1]);	// �e�N�X�`���̐ݒ�

		// ���_�o�b�t�@�̎擾
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pLogo->GetVtxBuff();

		// ���_���
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(-RANKING_LOGO_SCALE_INI * 3.0f, -RANKING_LOGO_SCALE_INI, 0.0f) + RANKING_LOGO_POS_INI;
		pVtx[1].pos = D3DXVECTOR3(RANKING_LOGO_SCALE_INI * 3.0f, -RANKING_LOGO_SCALE_INI, 0.0f) + RANKING_LOGO_POS_INI;
		pVtx[2].pos = D3DXVECTOR3(-RANKING_LOGO_SCALE_INI * 3.0f, RANKING_LOGO_SCALE_INI, 0.0f) + RANKING_LOGO_POS_INI;
		pVtx[3].pos = D3DXVECTOR3(RANKING_LOGO_SCALE_INI * 3.0f, RANKING_LOGO_SCALE_INI, 0.0f) + RANKING_LOGO_POS_INI;

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();

		// ���_�o�b�t�@�̐ݒ�
		m_pLogo->SetVtxBuff(pVtxBuff);
	}

	// �����L���O���ʃ|���S���̃C���X�^���X����
	D3DXVECTOR3 RankPos = RANKING_RANK_POS_INI;   // ���W
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�̌������J��Ԃ�
		if (m_apSceneSquare[nCntRanking] == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_apSceneSquare[nCntRanking] = CScene2DSquare::Create(RankPos, RANKING_RANK_COL_INI, RANKING_RANK_WIDTH_INI, RANKING_RANK_HEIGHT_INI, 6);
			if (m_apSceneSquare[nCntRanking] != NULL)
			{// ���������m�ۂł���
				// �e�N�X�`�������蓖��
				m_apSceneSquare[nCntRanking]->BindTexture(m_apTexture[0]);

				// �e�N�X�`�����W�����炷
				VERTEX_2D *pVtx;
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apSceneSquare[nCntRanking]->GetVtxBuff();

				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				// �J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f + (nCntRanking * (1.0f / MAX_RANKING)));
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f + (nCntRanking * (1.0f / MAX_RANKING)));
				pVtx[2].tex = D3DXVECTOR2(0.0f, (1.0f / MAX_RANKING) + (nCntRanking * (1.0f / MAX_RANKING)));
				pVtx[3].tex = D3DXVECTOR2(1.0f, (1.0f / MAX_RANKING) + (nCntRanking * (1.0f / MAX_RANKING)));

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();
			}
			RankPos = RANKING_RANK_POS_INI;
			RankPos.y += (RANKING_RANK_HEIGHT_INI + (RANKING_RANK_HEIGHT_INI * 0.75f)) * (nCntRanking + 1);
		}

		int nNumber;	// �v�Z���ʂ�����
		for (int nCntDigits = 0; nCntDigits < m_aScoreDigits[nCntRanking]; nCntDigits++)
		{// �������J��Ԃ�
			nNumber = m_nRankingScore[nCntRanking] % (int)powf(10.0f, (nCntDigits + 1) * 1.0f) / (int)powf(10.0f, nCntDigits * 1.0f);
			m_apNumber[nCntRanking][nCntDigits]->SetNumber(nNumber);	// �����̐ݒ�
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRanking::Uninit(void)
{
	// �e�N�X�`���̊J��
	CNumber::UnLoad();      // ����
	CCylinder::UnLoad();    // �R
	CDome::UnLoad();        // ��
	CField::UnLoad();       // �t�B�[���h
	CShadow::UnLoad();      // �e

	// �}�b�v�̔j��
	if (m_pMap != NULL)
	{// ���������m�ۂ���Ă���
		m_pMap->Uninit();

		// �������̊J��
		delete m_pMap;
		m_pMap = NULL;
	}

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�X�R�A�̐������J��Ԃ�
		if (m_apNumber[nCntRanking] != NULL)
		{
			for (int nCntScore = 0; nCntScore < m_aScoreDigits[nCntRanking]; nCntScore++)
			{// �����L���O�X�R�A�̌������J��Ԃ�
				if (m_apNumber[nCntRanking][nCntScore] != NULL)
				{// ���������m�ۂ���Ă���
					// �����|���S���̏I������
					m_apNumber[nCntRanking][nCntScore]->Uninit();
					m_apNumber[nCntRanking][nCntScore] = NULL;
				}
			}
			delete[] m_apNumber[nCntRanking];
			m_apNumber[nCntRanking] = NULL;
		}

		if (m_apSceneSquare[nCntRanking] != NULL)
		{// ���������m�ۂ���Ă���
			// ���ʃ|���S���̏I������
			m_apSceneSquare[nCntRanking]->Uninit();
			m_apSceneSquare[nCntRanking] = NULL;
		}

		// ��Ԃ�߂��Ă���
		m_State[nCntRanking] = STATE_NONE;
	}

	if (m_pLogo != NULL)
	{// ���������m�ۂ���Ă���
		// �����L���O���S�̏I������
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	// �ŐV�̃X�R�A��������
	m_nScore = 0;

	// �����L���O�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRanking::Update(void)
{
	if (m_pLogo != NULL)
	{
		m_pLogo->Update();
	}

	// ��ʑJ�ڂ܂ł̃J�E���g
	m_nCounter++;

	if (m_nCounter % RANKING_MODE_CHANGETIMING == 0)
	{// ��ʑJ�ڂ���^�C�~���O�ɂȂ���
		CManager::GetFade()->SetFade(CManager::MODE_TITLE);
	}
	else
	{// ��ʑJ�ڂ���^�C�~���O�ł͂Ȃ�
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// ����{�^���������ꂽ
				CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			}
			else
			{// �L�[�{�[�h�̌���{�^����������Ă��Ȃ�
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_12; nCntButton++)
				{// ��������{�^�����̐������J��Ԃ�
					if (CManager::GetJoyStick()->GetJoyStickDevice(0)->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// �{�^���������ꂽ
						CManager::GetFade()->SetFade(CManager::MODE_TITLE);
					}
				}
			}
		}
	}

	if (m_nCounter % 10 == 0)
	{// �_�ł�����
		m_bFlash = m_bFlash ? false : true;
	}

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++)
	{// �����L���O�̌������J��Ԃ�
		if (m_nRankingScore[nCntRanking] == m_nScore)
		{// �ŐV�̃X�R�A��_�ł�����
			for (int nCntDigits = 0; nCntDigits < m_aScoreDigits[nCntRanking]; nCntDigits++)
			{// �������J��Ԃ�
				if(m_bFlash)
				{// �_�ł�����
					m_apNumber[nCntRanking][nCntDigits]->SetVtxBuffCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				}
				else
				{// �_�ł�����
					m_apNumber[nCntRanking][nCntDigits]->SetVtxBuffCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				}
			}
			break;
		}
	}

	CDebugProc::Print("�����L���O�Ȃ�\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRanking::Draw(void)
{

}