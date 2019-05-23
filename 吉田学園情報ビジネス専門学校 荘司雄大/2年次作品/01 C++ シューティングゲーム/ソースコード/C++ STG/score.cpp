//=============================================================================
//
// �X�R�A���� [Score.cpp]
// Author : Takahiro Syoji
//
//=============================================================================
#include "number.h"
#include "score.h"
#include "scene.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define SCORE_POS (D3DXVECTOR3(1100, 700, 0.0f))

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScore::CScore()
{
	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		m_apNumber[nCntScore] = NULL;
	}

	m_nScore = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScore::Init(void)
{
	int nNumber;

	SetObjType(OBJTYPE_SCORE);
	D3DXVECTOR3 pos;
	pos = m_pos;

	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		m_apNumber[nCntScore] = CNumber::Create(pos);

		// �����̌v�Z
		nNumber = m_nScore % (int)powf(10.0f, (float)(MAX_NUMBER - nCntScore)) * 10 / (int)powf(10.0f, (float)(MAX_NUMBER - nCntScore));

		m_apNumber[nCntScore]->SetNumber(nNumber);

		pos.x += 30;
	}

	m_nScore = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{
			m_apNumber[nCntScore]->Uninit();

			delete m_apNumber[nCntScore];
			m_apNumber[nCntScore] = NULL;
		}
	}
	//�����[�X
	Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CScore::Update(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void CScore::Draw(void)
{
	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		m_apNumber[nCntScore]->Draw();
	}
}

//=============================================================================
// �N���G�C�g
//=============================================================================
CScore *CScore::Create(D3DXVECTOR3 pos)
{
	CScore *pScore = NULL;
	if (pScore == NULL)
	{
		// ���I�m��
		pScore = new CScore;

		if (pScore != NULL)
		{
			pScore->m_pos = pos;

			// ����������
			pScore->Init();
		}
	}
	return pScore;
}

//=============================================================================
// �Z�b�g�X�R�A
//=============================================================================
void CScore::AddScore(int nScore)
{
	int nNumber;

	m_nScore += nScore;

	for (int nCntScore = 0; nCntScore < MAX_NUMBER; nCntScore++)
	{
		// �����̌v�Z
		nNumber = m_nScore % (int)powf(10.0f, (float)(MAX_NUMBER - nCntScore)) * 10 / (int)powf(10.0f, (float)(MAX_NUMBER - nCntScore));

		m_apNumber[nCntScore]->SetNumber(nNumber);
	}
}

int CScore::GetScore(void)
{
	return m_nScore;
}