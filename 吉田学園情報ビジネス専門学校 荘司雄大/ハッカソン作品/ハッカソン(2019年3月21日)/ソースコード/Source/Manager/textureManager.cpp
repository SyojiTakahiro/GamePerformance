//*****************************************************************************
//
//     �e�N�X�`���Ǌ��̏���[textureManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "textureManager.h"

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CTextureManager::CTextureManager()
{
	// �e��l�̃N���A
	m_pFileName = NULL;  // �e�N�X�`���̃t�@�C����
	m_nNumTexture = 0;   // �ǂݍ��񂾃e�N�X�`���̐�
	m_apTexture = NULL;  // �e�N�X�`���ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CTextureManager::~CTextureManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CTextureManager *CTextureManager::Create(int nNumTex)
{
	CTextureManager *pTextureManager = NULL;  // �e�N�X�`���Ǌ��N���X�^�̃|�C���^
	if (pTextureManager == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pTextureManager = new CTextureManager;
		if (pTextureManager != NULL)
		{// ���������m�ۂł���
			pTextureManager->SetNumTexture(nNumTex);  // �ǂݍ��ރe�N�X�`���̐���ݒ�
			// �������������s��
			if (FAILED(pTextureManager->Init()))
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
	{// ���������m�ۂł����ԂɂȂ�
		return NULL;
	}

	// �C���X�^���X��Ԃ�
	return pTextureManager;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CTextureManager::Init(void)
{
	if (m_nNumTexture >= 1)
	{// �ǂݍ��ރe�N�X�`���̐���1�ȏ゠��
	    // �ǂݍ��ރe�N�X�`���̐��������������m�ۂ���
		if (m_apTexture == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_apTexture = new LPDIRECT3DTEXTURE9[m_nNumTexture];
		}
		if (m_pFileName == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_pFileName = new char*[m_nNumTexture];
			if (m_pFileName != NULL)
			{// ���������m�ۂł���
				for (int nCntTex = 0; nCntTex < m_nNumTexture; nCntTex++)
				{// �ǂݍ��ރe�N�X�`���̐������J��Ԃ�
					m_pFileName[nCntTex] = new char[256];
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CTextureManager::Uninit(void)
{
	// �e�N�X�`�����̊J��
	if (m_apTexture != NULL)
	{// ���������m�ۂ���Ă���
		ReleaseTexture();
	}

	// �t�@�C�����̊J��
	if (m_pFileName != NULL)
	{// ���������m�ۂł����Ԃɂ���
		for (int nCntTex = 0; nCntTex < m_nNumTexture; nCntTex++)
		{// �ǂݍ��ރe�N�X�`���̐������J��Ԃ�
			delete[] m_pFileName[nCntTex];
			m_pFileName[nCntTex] = NULL;
		}
		delete[] m_pFileName;
		m_pFileName = NULL;
	}
}

//=============================================================================
//    �ǂݍ��ރe�N�X�`���̐��ݒ菈��
//=============================================================================
void CTextureManager::SetNumTexture(int nNumTex)
{
	m_nNumTexture = nNumTex;
}

//=============================================================================
//    �e�N�X�`���̃t�@�C�����ݒ菈��
//=============================================================================
void CTextureManager::SetFileName(char *pFileName, int nIdx)
{
	strcpy(m_pFileName[nIdx], pFileName);
}

//=============================================================================
//    �ǂݍ��ރe�N�X�`���̐��ݒ菈��
//=============================================================================
int CTextureManager::GetNumTexture(void)
{
	return m_nNumTexture;
}

//=============================================================================
//    �e�N�X�`���̃t�@�C�����擾����
//=============================================================================
char *CTextureManager::GetFileName(int nIdx)
{
	return m_pFileName[nIdx];
}

//=============================================================================
//    �e�N�X�`���̎擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CTextureManager::GetTexture(int nIdx)
{
	LPDIRECT3DTEXTURE9 pTexture = NULL;  // �e�N�X�`���ւ̃|�C���^
	if (m_apTexture != NULL)
	{// ���������m�ۂ���Ă���
		pTexture = m_apTexture[nIdx];
	}
	return pTexture;
}

//=============================================================================
//    �e�N�X�`���̊��蓖�ď���
//=============================================================================
void CTextureManager::BindTexture(LPDIRECT3DTEXTURE9 pTexture, int nIdx)
{
	m_apTexture[nIdx] = pTexture;
}

//=============================================================================
//    �e�N�X�`���̊J������
//=============================================================================
void CTextureManager::ReleaseTexture(void)
{
	for (int nCntTex = 0; nCntTex < m_nNumTexture; nCntTex++)
	{// �ǂݍ��񂾃e�N�X�`���̐������J��Ԃ�
		if (m_apTexture[nCntTex] != NULL)
		{// ���������m�ۂ���Ă���
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}

	delete[] m_apTexture;
	m_apTexture = NULL;
}