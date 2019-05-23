//*****************************************************************************
//
//     �c�@�̏���[stock.cpp]
//     Auther:Takuto Ishida
//
//*****************************************************************************
#include "stock.h"
#include "scene.h"
#include "scene2D.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "textureManager.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define STOCK_FRAME_TEXTURE	"data/TEXTURE/GAME/UI/stock.png"	// �^�C�}�[�̘g�e�N�X�`���̃p�X

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CStock::CStock(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_apNumber = NULL;                           // �����|���S���N���X�ւ̃|�C���^
	m_nDigit = 0;                                // �c�@�̌���
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �c�@�̍��W
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // �c�@�̐F
	m_fWidth = 0.0f;                             // �c�@�̐����|���S���P���̕�
	m_fHeight = 0.0f;                            // �c�@�̐����|���S���P���̕�
	m_nStock = 0;                                // �c�@
	m_pTexture = NULL;                           // �e�N�X�`���ւ̃|�C���^
	m_pTextureManager = NULL;					 // �e�N�X�`���Ǘ��N���X
	m_pFrame = NULL;							 // �g
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CStock::~CStock()
{

}

//=============================================================================
//    ��������
//=============================================================================
CStock *CStock::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nStock, int nPriority)
{
	CStock *pStock = NULL;                 // �c�@�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pStock == NULL)
		{// ����������ɂȂ��Ă���
			pStock = new CStock(nPriority);
			if (pStock != NULL)
			{// ���������m�ۂł���
			 // �e��l�̐ݒ�
				pStock->SetPos(pos);                    // �c�@�̍��W
				pStock->SetCol(col);                    // �c�@�̐F
				pStock->SetWidth(fWidth);               // �c�@�̐����|���S���P���̕�
				pStock->SetHeight(fHeight);             // �c�@�̐����|���S���P���̍���
				pStock->SetStock(nStock);               // �c�@

				if (FAILED(pStock->Init()))
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

	return pStock;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CStock::Init(void)
{
	// �e��l�̏�����
	D3DXVECTOR3 NumberPos = m_Pos;   // ���W
	D3DXCOLOR NumberCol = m_Col;     // �F
	float fNumberWidth = m_fWidth;   // ��
	float fNumberHeight = m_fHeight; // ����
	int nNumber = 0;                 // �\�����鐔���̌v�Z�p

	if (m_pFrame == NULL)
	{// ���������m�ۂł����Ԃł���
		m_pFrame = CScene2D::Create(m_Pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), m_fWidth * 2.5f, m_fHeight * 1.5f, 0.0f, 5);
		if (m_pFrame != NULL)
		{// ���������m�ۂł���
			LPDIRECT3DTEXTURE9 pTexture = NULL;
			D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), STOCK_FRAME_TEXTURE, &pTexture);
			m_pTextureManager = CTextureManager::Create(1);

			if (m_pTextureManager != NULL)
			{// �e�N�X�`���Ǘ��N���X�̐����ɐ���
				m_pTextureManager->BindTexture(pTexture, 0);
				m_pTextureManager->SetFileName(STOCK_FRAME_TEXTURE, 0);
				m_pFrame->BindTexture(m_pTextureManager->GetTexture(0));
				pTexture = NULL;
			}
		}
	}

	// �c�@�̌������v�Z���A���������������m��
	m_nDigit = (int)log10f((float)m_nStock) + 1;   // ���������߂�
	if (m_nDigit <= 0) { m_nDigit = 1; }	              // 0�ȉ��̂Ƃ�1�ɂ���

	if (m_apNumber == NULL)
	{// ���������m�ۂł����Ԃł���
		m_apNumber = new CNumber*[m_nDigit];  // �������������m��
		if (m_apNumber != NULL)
		{// ���������m�ۂł���
			for (int nCntStock = 0; nCntStock < m_nDigit; nCntStock++)
			{// �X�R�A�̌������J��Ԃ�
				m_apNumber[nCntStock] = NULL;   // �������񃁃�������ɂ���
				if (m_apNumber[nCntStock] == NULL)
				{// ���������m�ۂł����Ԃɂ���
				 // �\�����鐔���̌v�Z
					nNumber = m_nStock % ((int)powf(10.0f, (float)nCntStock) * 10) / (int)powf(10.0f, (float)nCntStock);

					// �����|���S���̃C���X�^���X��������
					m_apNumber[nCntStock] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, CNumber::STATE_NONE, nNumber, 3, GetPriority());

					// ���ɐi�߂�
					NumberPos.x -= m_fWidth + (m_fWidth * 0.5f);
				}
			}
		}
	}
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CStock::Uninit(void)
{
	// �����|���S���̊J������
	if (m_apNumber != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntStock = 0; nCntStock < m_nDigit; nCntStock++)
		{// �X�R�A�̌������J��Ԃ�
			if (m_apNumber[nCntStock] != NULL)
			{// ���������m�ۂ���Ă���
			 // �����|���S���̏I������
				m_apNumber[nCntStock]->Uninit();
				m_apNumber[nCntStock] = NULL;
			}
		}
		delete[] m_apNumber;
		m_apNumber = NULL;
	}

	// �e�N�X�`���Ǌ��N���X���J������
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// �c�@�̏�������
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CStock::Update(void)
{
	if(m_nStock <= 0)
	{// �c�@���Ȃ��Ȃ���
		if (CManager::GetMode() == CManager::MODE_GAME)
		{// �Q�[�����[�h����������
			CGame *pGame = CManager::GetGame();
			if (pGame->GetState() != CGame::STATE_END)
			{// �Q�[���I����ԂłȂ�
				pGame->SetState(CGame::STATE_END);
				//CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_FINISH);
			}
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CStock::Draw(void)
{

}

//=============================================================================
//    �����e�N�X�`����ݒ肷�鏈��
//=============================================================================
void CStock::SetStockTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
	if (m_pTexture != NULL)
	{// �e�N�X�`���ւ̃|�C���^���擾�ł���
		if (m_apNumber != NULL)
		{// �����|���S������������Ă���
			for (int nCntStock = 0; nCntStock < m_nDigit; nCntStock++)
			{// �c�@�̌������J��Ԃ�
				if (m_apNumber[nCntStock] != NULL)
				{// �����|���S������������Ă���
					m_apNumber[nCntStock]->BindTexture(m_pTexture);
				}
			}
		}
	}
}

//=============================================================================
//    �c�@�̍��W��ݒ肷�鏈��
//=============================================================================
void CStock::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �c�@�̐F��ݒ肷�鏈��
//=============================================================================
void CStock::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
//    �c�@�̐����|���S���P���̕���ݒ肷�鏈��
//=============================================================================
void CStock::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    �c�@�̐����|���S���P���̍�����ݒ肷�鏈��
//=============================================================================
void CStock::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    �c�@��ݒ肷�鏈��
//=============================================================================
void CStock::SetStock(const int nStock)
{
	m_nStock = nStock;
	if (m_apNumber != NULL)
	{// �����|���S������������Ă���
		for (int nCntStock = 0; nCntStock < m_nDigit; nCntStock++)
		{// �c�@�̌������J��Ԃ�
			m_apNumber[nCntStock]->SetNumber(nStock);
		}
	}
}

//=============================================================================
//    �c�@�̍��W���擾���鏈��
//=============================================================================
D3DXVECTOR3 CStock::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �c�@�̐F���擾���鏈��
//=============================================================================
D3DXCOLOR CStock::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    �c�@�̐����|���S���P���̕����擾���鏈��
//=============================================================================
float CStock::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    �c�@�̐����|���S���P���̍������擾���鏈��
//=============================================================================
float CStock::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    �c�@���擾���鏈��
//=============================================================================
int CStock::GetStock(void)
{
	return m_nStock;
}
