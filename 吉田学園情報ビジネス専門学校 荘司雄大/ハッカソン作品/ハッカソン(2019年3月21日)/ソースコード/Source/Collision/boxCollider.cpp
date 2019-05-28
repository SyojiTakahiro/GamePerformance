//*****************************************************************************
//
//     �����蔻��p�����f���̏���[boxCollider.cpp]
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
CBoxCollider::CBoxCollider()
{
	// �e��l�̃N���A
	for (int nCntVer = 0; nCntVer < 8; nCntVer++)
	{// ���̒��_�̐������J��Ԃ�
		m_Vertex[nCntVer].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Vertex[nCntVer].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
	for (int nCntIndex = 0; nCntIndex < 24; nCntIndex++)
	{// �`��Ɏg�p����C���f�b�N�X���̐������J��Ԃ�
		m_wIndex[nCntIndex] = 0;
	}
	m_fWidth = 0.0f;                          // ���̉���
	m_fHeight = 0.0f;                         // ���̍���
	m_fDepth = 0.0f;                          // ���̉��s
	m_bReturnFlag = false;                    // ���������ۂɖ߂����ǂ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBoxCollider::~CBoxCollider()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBoxCollider *CBoxCollider::Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth, bool bReturnFlag, int nType)
{
	CBoxCollider *pBoxCollider = NULL;      // �����蔻��p�����f���N���X�^�̃|�C���^
	if (pBoxCollider == NULL)
	{// ����������ɂȂ��Ă���
		pBoxCollider = new CBoxCollider;
		if (pBoxCollider != NULL)
		{// �C���X�^���X�𐶐��ł���
		    // �e��l�̐ݒ�
			pBoxCollider->SetPos(pos);                 // ���W
			pBoxCollider->SetWidth(fWidth);            // ���̉���
			pBoxCollider->SetHeight(fHeight);          // ���̍���
			pBoxCollider->SetDepth(fDepth);            // ���̉��s
			pBoxCollider->SetReturnFlag(bReturnFlag);  // ���������ۂɖ߂����ǂ���
			pBoxCollider->SetType(nType);              // ���

			if (FAILED(pBoxCollider->Init()))
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

	return pBoxCollider;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBoxCollider::Init(void)
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
void CBoxCollider::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CBoxCollider::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBoxCollider::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���[���h�}�g���b�N�X�̐ݒ菈��
			SetMtxWorld(pDevice);

#ifdef _DEBUG
			DWORD Culling;
			DWORD Lighting;

			// ���C�e�B���O���Ȃ��ݒ��
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// �J�����O���O���Ă���
			pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			// ���̕`��
			pDevice->DrawIndexedPrimitiveUP(D3DPT_LINELIST, 0, 8, 12, m_wIndex, D3DFMT_INDEX16, m_Vertex, sizeof(BOX_VERTEX));

			// �J�����O�̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_CULLMODE, Culling);

			// ���C�e�B���O�̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
#endif
		}
	}
}

//=============================================================================
//    ���_��񐶐�����
//=============================================================================
void CBoxCollider::MakeVertex(void)
{
	// ���_���W��ݒ�(���Ԃ� �㑤�� ���� -> �E�� -> -> ���O -> �E��
	//                       ������ ���� -> �E�� -> -> ���O -> �E��)
	m_Vertex[0].pos = D3DXVECTOR3(0.0f, m_fHeight, m_fDepth);
	m_Vertex[1].pos = D3DXVECTOR3(m_fWidth, m_fHeight, m_fDepth);
	m_Vertex[2].pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
	m_Vertex[3].pos = D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
	m_Vertex[4].pos = D3DXVECTOR3(0.0f, 0.0f, m_fDepth);
	m_Vertex[5].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fDepth);
	m_Vertex[6].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Vertex[7].pos = D3DXVECTOR3(m_fWidth, 0.0f, 0.0f);

	// ���_�J���[��ݒ�(�S�ĐԐF�ɐݒ�)
	m_Vertex[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[4].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[5].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[6].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_Vertex[7].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// �C���f�b�N�X����ݒ�
	m_wIndex[0] = 0;
	m_wIndex[1] = 1;
	m_wIndex[2] = 1;
	m_wIndex[3] = 3;
	m_wIndex[4] = 3;
	m_wIndex[5] = 2;
	m_wIndex[6] = 2;
	m_wIndex[7] = 0;
	m_wIndex[8] = 0;
	m_wIndex[9] = 4;
	m_wIndex[10] = 1;
	m_wIndex[11] = 5;
	m_wIndex[12] = 2;
	m_wIndex[13] = 6;
	m_wIndex[14] = 3;
	m_wIndex[15] = 7;
	m_wIndex[16] = 4;
	m_wIndex[17] = 5;
	m_wIndex[18] = 5;
	m_wIndex[19] = 7;
	m_wIndex[20] = 7;
	m_wIndex[21] = 6;
	m_wIndex[22] = 6;
	m_wIndex[23] = 4;
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CBoxCollider::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand)
{
	bool bHit = false;  // �����������ǂ���
	D3DXVECTOR3 pos = D3DXVECTOR3(GetMtxWorld()._41, GetMtxWorld()._42, GetMtxWorld()._43);

	// Y����̔���
	if (pPos->y < pos.y + m_fHeight && pPos->y + ColRange.y > pos.y)
	{// ���݂̍��������̒��ɂ���
	    // X����̔���
		if (pPos->z + ColRange.z > pos.z && pPos->z - ColRange.z < pos.z + m_fDepth)
		{// Z����ɂ��邩�ǂ�������
			if (pPosOld->x + ColRange.x <= pos.x && pPos->x + ColRange.x > pos.x)
			{// ���̍������������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->x = pos.x - ColRange.x;
				}
				bHit = true;
			}
			else if (pPosOld->x - ColRange.x >= pos.x + m_fWidth && pPos->x - ColRange.x < pos.x + m_fWidth)
			{// ���̉E�����������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->x = pos.x + m_fWidth + ColRange.x;
				}
				bHit = true;
			}
		}
		// Z����̔���
		if (pPos->x + ColRange.x > pos.x && pPos->x - ColRange.x < pos.x + m_fWidth)
		{// X����ɂ��邩�ǂ�������
			if (pPosOld->z - ColRange.z >= pos.z + m_fDepth && pPos->z - ColRange.z < pos.z + m_fDepth)
			{// ���̉������������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->z = pos.z + m_fDepth + ColRange.z;
				}
				bHit = true;
			}
			else if (pPosOld->z + ColRange.z <= pos.z && pPos->z + ColRange.z > pos.z)
			{// ���̎�O�����������
				if (m_bReturnFlag == true)
				{// �ʒu��߂��Ȃ��
					pPos->z = pos.z - ColRange.z;
				}
				bHit = true;
			}
		}
	}

	if (pPosOld->y >= pos.y + m_fHeight && pPos->y < pos.y + m_fHeight)
	{// �������ォ�甠�̓����ɓ����Ă��܂���
		if (pPos->x + ColRange.x > pos.x && pPos->x - ColRange.x < pos.x + m_fWidth
			&& pPos->z + ColRange.z > pos.z && pPos->z - ColRange.z < pos.z + m_fDepth)
		{// ���̓����ɂ��锻�肾����
			if (m_bReturnFlag == true)
			{// �ʒu��߂��Ȃ��
				pPos->y = pos.y + m_fHeight;
			}
			bHit = true;
			if (pLand != NULL)
			{
				*pLand = true;
			}
		}
	}
	else if (pPosOld->y + ColRange.y <= pos.y && pPos->y + ColRange.y > pos.y)
	{// �����������甠�̓����ɓ����Ă��܂���
		if (pPos->x + ColRange.x > pos.x && pPos->x - ColRange.x < pos.x + m_fWidth
			&& pPos->z + ColRange.z > pos.z && pPos->z - ColRange.z < pos.z + m_fDepth)
		{// ���̓����ɂ��锻�肾����
			if (m_bReturnFlag == true)
			{// �ʒu��߂��Ȃ��
				pPos->y = pos.y - ColRange.y;
				pMove->y = 0.0f;
			}
			bHit = true;
		}
	}

	return bHit;
}

//=============================================================================
//    ���̉����ݒ菈��
//=============================================================================
void CBoxCollider::SetWidth(const float fWidth)
{
	m_fWidth = fWidth;
}

//=============================================================================
//    ���̍����ݒ菈��
//=============================================================================
void CBoxCollider::SetHeight(const float fHeight)
{
	m_fHeight = fHeight;
}

//=============================================================================
//    ���̉��s�ݒ菈��
//=============================================================================
void CBoxCollider::SetDepth(const float fDepth)
{
	m_fDepth = fDepth;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����ݒ菈��
//=============================================================================
void CBoxCollider::SetReturnFlag(const bool bReturnFlag)
{
	m_bReturnFlag = bReturnFlag;
}

//=============================================================================
//    ���̉����擾����
//=============================================================================
float CBoxCollider::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    ���̍����擾����
//=============================================================================
float CBoxCollider::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ���̉��s�擾����
//=============================================================================
float CBoxCollider::GetDepth(void)
{
	return m_fDepth;
}

//=============================================================================
//    ���������ۂɖ߂����ǂ����擾����
//=============================================================================
bool CBoxCollider::GetReturnFlag(void)
{
	return m_bReturnFlag;
}