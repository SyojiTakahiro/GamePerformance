//*****************************************************************************
//
//     �����|���S���̏���[number.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "number.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define NUMBER_TEXTURE_NAME "data/TEXTURE/COMMON/number.png"   // �e�N�X�`���̃t�@�C����(����)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;    // �e�N�X�`���ւ̃|�C���^

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CNumber::CNumber(int nPriority, OBJTYPE objType) : CScene2DSquare(nPriority, objType)
{
	// �e��l�̃N���A
	m_nNumber = 0;    // �\�����鐔��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
//    ��������
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber, int nPriority)
{
	CNumber *pNumber = NULL;   // �����|���S���N���X�^�̃|�C���^
	if (pNumber == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pNumber = new CNumber(nPriority); // �����|���S���N���X�̃C���X�^���X����
		if (pNumber != NULL)
		{// ���������m�ۂł���
		    // ����������
			if (FAILED(pNumber->Init(pos, col, fWidth, fHeight, nNumber)))
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

	return pNumber;
}

//=============================================================================
//    �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CNumber::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	 // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			D3DXCreateTextureFromFile(pDevice, NUMBER_TEXTURE_NAME, &m_pTexture);
		}
	}

	if (m_pTexture == NULL)
	{// �e�N�X�`�����ǂݍ��߂Ȃ�����
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`���̉������
//=============================================================================
void CNumber::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CNumber::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber)
{
	// �e��l�̐ݒ�
	m_nNumber = nNumber;  // �\�����鐔��

	// ���ʂ̏���������
	if (FAILED(CScene2DSquare::Init(pos, col, fWidth, fHeight)))
	{
		return E_FAIL;
	}

	// �e�N�X�`���̐ݒ�
	BindTexture(m_pTexture);

	SetNumber(nNumber);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CNumber::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CNumber::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CNumber::Draw(void)
{
	// ���ʂ̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
//    �����̐ݒ菈��
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	// �����̐ݒ�
	m_nNumber = nNumber;

	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���_����ݒ�
			VERTEX_2D *pVtx;
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// �e�N�X�`�����W
			pVtx[0].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 1.0f);

			// ���_�o�b�t�@���A�����b�N����
			pVtxBuff->Unlock();
		}
	}
}