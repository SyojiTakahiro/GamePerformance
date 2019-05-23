//*****************************************************************************
//
//     �X�R�A�̏���[score.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "score.h"
#include "scene2D.h"
#include "scene.h"
#include "renderer.h"
#include "manager.h"
#include "textureManager.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define SCORE_FRAME_TEXTURE	"data/TEXTURE/GAME/UI/score.png"	// �^�C�}�[�̘g�e�N�X�`���̃p�X

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CScore::CScore(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	for (int nCntScore = 0; nCntScore < SCORE_NUMBER_MAX; nCntScore++)
	{// �X�R�A�̌������J��Ԃ�
		m_apNumber[nCntScore] = NULL;
	}
	m_pTexture = NULL;                          // �����e�N�X�`���ւ̃|�C���^
	m_nScore = 0;                               // �X�R�A
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // �X�R�A�̍��W
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // �X�R�A�̐F
	m_fWidth = 0.0f;                            // �X�R�A�̐����|���S���P���̍���
	m_fHeight = 0.0f;                           // �X�R�A�̐����|���S���P���̍���
	m_pTextureManager = NULL;					 // �e�N�X�`���Ǘ��N���X
	m_pFrame = NULL;							 // �g
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
//    ��������
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority)
{
	CScore *pScore = NULL;                 // �X�R�A�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pScore == NULL)
		{// ����������ɂȂ��Ă���
			pScore = new CScore(nPriority);
			if (pScore != NULL)
			{// ���������m�ۂł���
				// �e��l�̐ݒ�
				pScore->SetPos(pos);        // �X�R�A�̍��W
				pScore->SetCol(col);        // �X�R�A�̐F
				pScore->SetWidth(fWidth);   // �X�R�A�̐����|���S���P���̕�
				pScore->SetHeight(fHeight); // �X�R�A�̐����|���S���P���̍���

				if (FAILED(pScore->Init()))
				{// �������Ɏ��s����
					return NULL;
				}
			}
			else
			{// ���������m�ۂł��Ȃ�����
				return NULL;
			}
		}
		else
		{// ����������ł͂Ȃ�
			return NULL;
		}
	}
	else
	{// ����ȏ㐶���ł��Ȃ�
		return NULL;
	}

	return pScore;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CScore::Init(void)
{
	// �e��l�̏�����
	D3DXVECTOR3 NumberPos = m_Pos;    // ���W
	D3DXCOLOR NumberCol = m_Col;      // �F
	float fNumberWidth = m_fWidth;    // ��
	float fNumberHeight = m_fHeight;  // ����
	int nNumber = 0;                  // �\�����鐔���̌v�Z�p
	m_nScore = 0;                     // �X�R�A

	if (m_pFrame == NULL)
	{// ���������m�ۂł����Ԃł���
		m_pFrame = CScene2D::Create(m_Pos - D3DXVECTOR3(m_fWidth + (m_fWidth * 0.5f * SCORE_NUMBER_MAX),0.0f,0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), m_fWidth * SCORE_NUMBER_MAX, m_fHeight * 1.5f, 0.0f, 5);
		if (m_pFrame != NULL)
		{// ���������m�ۂł���
			LPDIRECT3DTEXTURE9 pTexture = NULL;
			D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), SCORE_FRAME_TEXTURE, &pTexture);
			m_pTextureManager = CTextureManager::Create(1);

			if (m_pTextureManager != NULL)
			{// �e�N�X�`���Ǘ��N���X�̐����ɐ���
				m_pTextureManager->BindTexture(pTexture, 0);
				m_pTextureManager->SetFileName(SCORE_FRAME_TEXTURE, 0);
				m_pFrame->BindTexture(m_pTextureManager->GetTexture(0));
				pTexture = NULL;
			}
		}
	}

	for (int nCntScore = 0; nCntScore < SCORE_NUMBER_MAX; nCntScore++)
	{// �X�R�A�̌������J��Ԃ�
		if (m_apNumber[nCntScore] == NULL)
		{// ���������m�ۂł����Ԃɂ���
		    // �\�����鐔���̌v�Z
			nNumber = m_nScore % ((int)powf(10.0f, (float)nCntScore) * 10) / (int)powf(10.0f, (float)nCntScore);

			// �����|���S���̃C���X�^���X��������
			m_apNumber[nCntScore] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, CNumber::STATE_NONE, nNumber, 3, GetPriority());

			// ���ɐi�߂�
			NumberPos.x -= m_fWidth + (m_fWidth * 0.5f);
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < SCORE_NUMBER_MAX; nCntScore++)
	{// �X�R�A�̌������J��Ԃ�
		if (m_apNumber[nCntScore] != NULL)
		{// ���������m�ۂ���Ă���
		    // �����|���S���̏I������
			m_apNumber[nCntScore]->Uninit();
			m_apNumber[nCntScore] = NULL;
		}
	}

	// �e�N�X�`���Ǌ��N���X���J������
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// �X�R�A�̏�������
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CScore::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CScore::Draw(void)
{

}

//=============================================================================
//    �X�R�A�̃e�N�X�`����ݒ肷�鏈��
//=============================================================================
void CScore::SetScoreTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;

	if (m_pTexture != NULL)
	{// �e�N�X�`���ւ̃|�C���^���擾�ł���
		for (int nCntScore = 0; nCntScore < SCORE_NUMBER_MAX; nCntScore++)
		{// �X�R�A�̌������J��Ԃ�
			if (m_apNumber[nCntScore] != NULL)
			{// ���������m�ۂ���Ă���
				m_apNumber[nCntScore]->BindTexture(pTexture);
			}
		}
	}
}

//=============================================================================
//    �X�R�A�̉��Z
//=============================================================================
void CScore::AddScore(int nScore)
{
	int nSetNumber;   // �\�����鐔��

	// �X�R�A�̉��Z
	m_nScore += nScore;

	// �e�N�X�`�����W�̐؂�ւ�����
	for (int nCntScore = 0; nCntScore < SCORE_NUMBER_MAX; nCntScore++)
	{// �X�R�A�̌������J��Ԃ�
	    // �\�����鐔���̌v�Z
		nSetNumber = m_nScore % ((int)powf(10.0f, (float)nCntScore) * 10) / (int)powf(10.0f, (float)nCntScore);

		// �����|���S���̐����ݒ菈��
		m_apNumber[nCntScore]->SetNumber(nSetNumber);
	}
}

//=============================================================================
//    �X�R�A�̍��W�̐ݒ�
//=============================================================================
void CScore::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �X�R�A�̎擾
//=============================================================================
void CScore::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
//    �X�R�A�̐����|���S���P���̕��̐ݒ�
//=============================================================================
void CScore::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    �X�R�A�̐����|���S���P���̍����̐ݒ�
//=============================================================================
void CScore::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    �X�R�A�̎擾
//=============================================================================
void CScore::SetScore(const int nScore)
{
	m_nScore = nScore;

	// �e�N�X�`�����W�̐؂�ւ�����
	int nSetNumber = 0; // �\�����鐔���̌v�Z�p
	for (int nCntScore = 0; nCntScore < SCORE_NUMBER_MAX; nCntScore++)
	{// �X�R�A�̌������J��Ԃ�
	    // �\�����鐔���̌v�Z
		nSetNumber = m_nScore % ((int)powf(10.0f, (float)nCntScore) * 10) / (int)powf(10.0f, (float)nCntScore);

		// �����|���S���̐����ݒ菈��
		m_apNumber[nCntScore]->SetNumber(nSetNumber);
	}
}

//=============================================================================
//    �X�R�A�̍��W�̎擾
//=============================================================================
D3DXVECTOR3 CScore::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �X�R�A�̐F�̎擾
//=============================================================================
D3DXCOLOR CScore::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    �X�R�A�̐����|���S���P���̕��̎擾
//=============================================================================
float CScore::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    �X�R�A�̐����|���S���P���̍����̎擾
//=============================================================================
float CScore::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    �X�R�A�̎擾
//=============================================================================
int CScore::GetScore(void)
{
	return m_nScore;
}