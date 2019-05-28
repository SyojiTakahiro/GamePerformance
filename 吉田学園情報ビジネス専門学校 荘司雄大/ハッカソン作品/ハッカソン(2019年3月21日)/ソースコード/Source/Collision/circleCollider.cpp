//*****************************************************************************
//
//     �����蔻��p�~�����f���̏���[CircleCollider.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "collision.h"
#include "manager.h"
#include "system.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************


//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CCircleCollider::CCircleCollider()
{
	// �e��l�̃N���A
	for (int nCntVer = 0; nCntVer < 8; nCntVer++)
	{// �~���̒��_�̐������J��Ԃ�
		m_Vertex[nCntVer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Vertex[nCntVer].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
	for (int nCntIndex = 0; nCntIndex < 24; nCntIndex++)
	{// �`��Ɏg�p����C���f�b�N�X���̐������J��Ԃ�
		m_wIndex[nCntIndex] = 0;
	}
	m_fHeight = 0.0f;                         // �~���̍���
	m_fColRange = 0.0f;                       // �~���̔��a
	m_bReturnFlag = false;                    // ���������ۂɖ߂����ǂ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CCircleCollider::~CCircleCollider()
{

}

//=============================================================================
//    ��������
//=============================================================================
CCircleCollider *CCircleCollider::Create(D3DXVECTOR3 pos, float fHeight, float fColRange, bool bReturnFlag, int nType)
{
	CCircleCollider *pCircleCollider = NULL;      // �����蔻��p�~�����f���N���X�^�̃|�C���^
	if (pCircleCollider == NULL)
	{// ����������ɂȂ��Ă���
		pCircleCollider = new CCircleCollider;
		if (pCircleCollider != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // �e��l�̐ݒ�
			pCircleCollider->SetPos(pos);                 // ���W
			pCircleCollider->SetHeight(fHeight);          // �~���̍���
			pCircleCollider->SetColRange(fColRange);      // �~���̔��a
			pCircleCollider->SetReturnFlag(bReturnFlag);  // ���������ۂɖ߂����ǂ���
			pCircleCollider->SetType(nType);              // ���

			if (FAILED(pCircleCollider->Init()))
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

	return pCircleCollider;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CCircleCollider::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���_�o�b�t�@�̐���
			MakeVertex();
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CCircleCollider::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CCircleCollider::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CCircleCollider::Draw(void)
{
#ifdef _DEBUG
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			DWORD Culling;
			DWORD Lighting;

			// ���C�e�B���O���Ȃ��ݒ��
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// �J�����O���O���Ă���
			pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// ���[���h�}�g���b�N�X�̐ݒ菈��
			SetMtxWorld(pDevice);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			// �~���̕`��
			pDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 8, 12, m_wIndex, D3DFMT_INDEX16, m_Vertex, sizeof(BOX_VERTEX));

			// �J�����O�̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

			// ���C�e�B���O�̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}

#endif
}

//=============================================================================
//    ���_��񐶐�����
//=============================================================================
void CCircleCollider::MakeVertex(void)
{
	//// ���_���W��ݒ�(���Ԃ� �㑤�� ���� -> �E�� -> -> ���O -> �E��
	////                       ������ ���� -> �E�� -> -> ���O -> �E��)
	//m_Vertex[0].pos = D3DXVECTOR3(0.0f, m_fHeight, m_fDepth);
	//m_Vertex[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth);
	//m_Vertex[2].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
	//m_Vertex[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	//m_Vertex[4].pos = D3DXVECTOR3(0.0f, 0.0f, m_fDepth);
	//m_Vertex[5].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth);
	//m_Vertex[6].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	//m_Vertex[7].pos = D3DXVECTOR3(m_fWidth, 0.0f, 0.0f);

	//// ���_�J���[��ݒ�(�S�ĐԐF�ɐݒ�)
	//m_Vertex[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[4].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[5].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[6].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//m_Vertex[7].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	//// �C���f�b�N�X����ݒ�
	//m_wIndex[0] = 0;
	//m_wIndex[1] = 1;
	//m_wIndex[2] = 1;
	//m_wIndex[3] = 3;
	//m_wIndex[4] = 3;
	//m_wIndex[5] = 2;
	//m_wIndex[6] = 2;
	//m_wIndex[7] = 0;
	//m_wIndex[8] = 0;
	//m_wIndex[9] = 4;
	//m_wIndex[10] = 1;
	//m_wIndex[11] = 5;
	//m_wIndex[12] = 2;
	//m_wIndex[13] = 6;
	//m_wIndex[14] = 3;
	//m_wIndex[15] = 7;
	//m_wIndex[16] = 4;
	//m_wIndex[17] = 5;
	//m_wIndex[18] = 5;
	//m_wIndex[19] = 7;
	//m_wIndex[20] = 7;
	//m_wIndex[21] = 6;
	//m_wIndex[22] = 6;
	//m_wIndex[23] = 4;
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CCircleCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand)
{
	bool bHit = false;  // �����������ǂ���
	D3DXVECTOR3 pos = GetPos();

	return bHit;
}

//=============================================================================
//    �~���̍����ݒ菈��
//=============================================================================
void CCircleCollider::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    �~���̔��a�ݒ菈��
//=============================================================================
void CCircleCollider::SetColRange(const float fColRange)
{
	m_fColRange = fColRange;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����ݒ菈��
//=============================================================================
void CCircleCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    �~���̍����擾����
//=============================================================================
float CCircleCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    �~���̔��a�擾����
//=============================================================================
float CCircleCollider::GetColRange(void)
{
	return m_fColRange;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����擾����
//=============================================================================
bool CCircleCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}