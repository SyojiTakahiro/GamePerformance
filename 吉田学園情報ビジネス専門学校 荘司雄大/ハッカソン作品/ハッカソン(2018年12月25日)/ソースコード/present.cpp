//*****************************************************************************
//
//     �v���[���g�̏���[stock.cpp]
//     Auther:Takuto Ishida
//
//*****************************************************************************
#include "present.h"
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
CPresent::CPresent(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_apNumber = NULL;                           // �����|���S���N���X�ւ̃|�C���^
	m_nDigit = 0;                                // �v���[���g�̌���
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �v���[���g�̍��W
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // �v���[���g�̐F
	m_fWidth = 0.0f;                             // �v���[���g�̐����|���S���P���̕�
	m_fHeight = 0.0f;                            // �v���[���g�̐����|���S���P���̕�
	m_nPresent = 0;                              // �v���[���g
	m_pTexture = NULL;                           // �e�N�X�`���ւ̃|�C���^
	m_pTextureManager = NULL;					 // �e�N�X�`���Ǘ��N���X
	m_pFrame = NULL;							 // �g
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CPresent::~CPresent()
{

}

//=============================================================================
//    ��������
//=============================================================================
CPresent *CPresent::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPresent, int nPriority)
{
	CPresent *pPresent = NULL;                 // �v���[���g�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pPresent == NULL)
		{// ����������ɂȂ��Ă���
			pPresent = new CPresent(nPriority);
			if (pPresent != NULL)
			{// ���������m�ۂł���
			 // �e��l�̐ݒ�
				pPresent->SetPos(pos);                    // �v���[���g�̍��W
				pPresent->SetCol(col);                    // �v���[���g�̐F
				pPresent->SetWidth(fWidth);               // �v���[���g�̐����|���S���P���̕�
				pPresent->SetHeight(fHeight);             // �v���[���g�̐����|���S���P���̍���
				pPresent->SetPresent(nPresent);               // �v���[���g

				if (FAILED(pPresent->Init()))
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

	return pPresent;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CPresent::Init(void)
{
	// �e��l�̏�����
	D3DXVECTOR3 NumberPos = m_Pos;   // ���W
	D3DXCOLOR NumberCol = m_Col;     // �F
	float fNumberWidth = m_fWidth;   // ��
	float fNumberHeight = m_fHeight; // ����
	int nNumber = 0;                 // �\�����鐔���̌v�Z�p

	if (m_pFrame == NULL)
	{// ���������m�ۂł����Ԃł���
		m_pFrame = CScene2D::Create(m_Pos + D3DXVECTOR3(-20.0f,0.0f,0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), m_fWidth * 2.5f, m_fHeight * 1.5f, 0.0f, 5);
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

	// �v���[���g�̌������v�Z���A���������������m��
	m_nDigit = (int)log10f((float)10) + 1;   // ���������߂�
	if (m_nDigit <= 0) { m_nDigit = 1; }	         // 0�ȉ��̂Ƃ�1�ɂ���

	if (m_apNumber == NULL)
	{// ���������m�ۂł����Ԃł���
		m_apNumber = new CNumber*[m_nDigit];  // �������������m��
		if (m_apNumber != NULL)
		{// ���������m�ۂł���
			for (int nCntPresent = 0; nCntPresent < m_nDigit; nCntPresent++)
			{// �X�R�A�̌������J��Ԃ�
				m_apNumber[nCntPresent] = NULL;   // �������񃁃�������ɂ���
				if (m_apNumber[nCntPresent] == NULL)
				{// ���������m�ۂł����Ԃɂ���
				    // �\�����鐔���̌v�Z
					nNumber = m_nPresent % ((int)powf(10.0f, (float)nCntPresent) * 10) / (int)powf(10.0f, (float)nCntPresent);

					// �����|���S���̃C���X�^���X��������
					m_apNumber[nCntPresent] = CNumber::Create(NumberPos, NumberCol, fNumberWidth, fNumberHeight, 0.0f, CNumber::STATE_NONE, nNumber, 3, GetPriority());

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
void CPresent::Uninit(void)
{
	// �����|���S���̊J������
	if (m_apNumber != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntPresent = 0; nCntPresent < m_nDigit; nCntPresent++)
		{// �X�R�A�̌������J��Ԃ�
			if (m_apNumber[nCntPresent] != NULL)
			{// ���������m�ۂ���Ă���
			 // �����|���S���̏I������
				m_apNumber[nCntPresent]->Uninit();
				m_apNumber[nCntPresent] = NULL;
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

	// �v���[���g�̏�������
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CPresent::Update(void)
{
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CPresent::Draw(void)
{

}

//=============================================================================
//    �����e�N�X�`����ݒ肷�鏈��
//=============================================================================
void CPresent::SetPresentTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
	if (m_pTexture != NULL)
	{// �e�N�X�`���ւ̃|�C���^���擾�ł���
		if (m_apNumber != NULL)
		{// �����|���S������������Ă���
			for (int nCntPresent = 0; nCntPresent < m_nDigit; nCntPresent++)
			{// �v���[���g�̌������J��Ԃ�
				if (m_apNumber[nCntPresent] != NULL)
				{// �����|���S������������Ă���
					m_apNumber[nCntPresent]->BindTexture(m_pTexture);
				}
			}
		}
	}
}

//=============================================================================
//    �v���[���g�̍��W��ݒ肷�鏈��
//=============================================================================
void CPresent::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �v���[���g�̐F��ݒ肷�鏈��
//=============================================================================
void CPresent::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
//    �v���[���g�̐����|���S���P���̕���ݒ肷�鏈��
//=============================================================================
void CPresent::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    �v���[���g�̐����|���S���P���̍�����ݒ肷�鏈��
//=============================================================================
void CPresent::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    �v���[���g��ݒ肷�鏈��
//=============================================================================
void CPresent::SetPresent(const int nPresent)
{
	int nNumber = 0;
	m_nPresent = nPresent;
	if (m_apNumber != NULL)
	{// �����|���S������������Ă���
		for (int nCntPresent = 0; nCntPresent < m_nDigit; nCntPresent++)
		{// �c�@�̌������J��Ԃ�
		    // �\�����鐔���̌v�Z
			nNumber = m_nPresent % ((int)powf(10.0f, (float)nCntPresent) * 10) / (int)powf(10.0f, (float)nCntPresent);
			m_apNumber[nCntPresent]->SetNumber(nNumber);
		}
	}
}

//=============================================================================
//    �v���[���g�̍��W���擾���鏈��
//=============================================================================
D3DXVECTOR3 CPresent::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �v���[���g�̐F���擾���鏈��
//=============================================================================
D3DXCOLOR CPresent::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    �v���[���g�̐����|���S���P���̕����擾���鏈��
//=============================================================================
float CPresent::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    �v���[���g�̐����|���S���P���̍������擾���鏈��
//=============================================================================
float CPresent::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    �v���[���g���擾���鏈��
//=============================================================================
int CPresent::GetPresent(void)
{
	return m_nPresent;
}
