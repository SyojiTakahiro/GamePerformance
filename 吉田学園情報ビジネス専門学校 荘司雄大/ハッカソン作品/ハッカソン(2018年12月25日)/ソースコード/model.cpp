//*****************************************************************************
//
//     ���f���̏���[model.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CModel::CModel()
{
	// �e��l�̃N���A
	m_pTexture = NULL;                        // �e�N�X�`���ւ̃|�C���^
	m_pMesh = NULL;                           // ���b�V�����ւ̃|�C���^
	m_pBuffMat = NULL;                        // �}�e���A�����ւ̃|�C���^
	m_nNumMat = 0;                            // �}�e���A�����̐�
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���݂̍��W
	m_AddPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ���݂̍��W�ɉ�����l
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ����
	m_pParent = NULL;                         // �e���f���ւ̃|�C���^
	D3DXMatrixIdentity(&m_MtxWorld);          // ���[���h�}�g���b�N�X
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
//    ��������
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture)
{
	CModel *pModel = NULL;  // ���f���N���X�^�̃|�C���^
	if (pModel == NULL)
	{// ����������ɂȂ��Ă���
		pModel = new CModel;
		if (pModel != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pModel->Init(pos, rot, pMesh, pBuffMat, nNumMat, VtxMax, VtxMin, pTexture)))
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

	return pModel;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture)
{
	// �����_�����O�N���X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
	        // �e��l�̐ݒ�
			m_Pos = pos;           // ���W
			m_Rot = rot;           // ����
			m_VtxMax = VtxMax;     // ���_�̍ő�l
			m_VtxMin = VtxMin;     // ���_�̍ő�l
			BindModel(pMesh, pBuffMat, nNumMat, VtxMax, VtxMin, pTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CModel::Uninit(void)
{
	// �|�C���^����ɂ��Ă���
	// ���b�V�����
	if (m_pMesh != NULL)
	{
		m_pMesh = NULL;
	}
	// �}�e���A�����ւ̃|�C���^
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat = NULL;
	}
	// �e�N�X�`���ւ̃|�C���^
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}
	// �e���f���ւ̃|�C���^
	if (m_pParent != NULL)
	{
		m_pParent = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CModel::Draw(void)
{
	D3DMATERIAL9 matDef;   // ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;    // �}�e���A���f�[�^�ւ̃|�C���^

	// �����_�����O�N���X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();  	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���[���h�}�g���b�N�X�ݒ菈��
			SetMtxWorld(pDevice);

			// ���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			if (m_pBuffMat != NULL && m_pMesh != NULL)
			{// X�t�@�C�����烂�f���f�[�^���ǂݍ��߂Ă���
			 // �}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

				for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
				{// �ݒ肳��Ă����}�e���A���̐������J��Ԃ�
				    // �}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					if (m_pTexture != NULL)
					{// �e�N�X�`����񂪈����o���Ă���
					    // �e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, m_pTexture[nCntMat]);
					}
					else
					{// �e�N�X�`����񂪈����o���Ă��Ȃ�
					    // �e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, NULL);
					}

					// ���f��(�p�[�c)�̕`��
					m_pMesh->DrawSubset(nCntMat);
				}
			}

			// �ۑ����Ă����}�e���A���ɖ߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CModel::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans, mtxParent; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x + m_AddPos.x, m_Pos.y + m_AddPos.y, m_Pos.z + m_AddPos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// �e���f���̃��[���h�}�g���b�N�X���|�����킹��
	if (m_pParent != NULL)
	{// �e���f�������݂���
		mtxParent = m_pParent->GetMtxWorld();            // �e���f�����烏�[���h�}�g���b�N�X���擾����
	}
	else
	{// �e���f�������݂��Ȃ�
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);  // �f�o�C�X���烏�[���h�}�g���b�N�X���擾����
	}
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxParent);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    ���f�����ݒ菈��
//=============================================================================
void CModel::BindModel(const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat, const D3DXVECTOR3 VtxMax, const D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture)
{
	// ���f�����̊��蓖��
	if (pMesh != NULL)
	{// ���b�V����񂪓ǂݍ��߂�
		m_pMesh = pMesh;          // ���b�V�����ւ̃|�C���^
	}
	if (pBuffMat != NULL)
	{// �}�e���A�����ւ̃|�C���^��ǂݍ��߂�
		m_pBuffMat = pBuffMat;    // �}�e���A�����ւ̃|�C���^
	}
	if (nNumMat != -1)
	{// �}�e���A���̐���񂪓ǂݍ��߂�
		m_nNumMat = nNumMat;      // �}�e���A�����̐�
	}
	if (m_pTexture != NULL)
	{// �e�N�X�`���ւ̃|�C���^��񂪓ǂݍ��߂�
		m_pTexture = m_pTexture;  // �e�N�X�`���ւ̃|�C���^���
	}
	m_VtxMax = VtxMax;            // ���_�̍ő�l���
	m_VtxMin = VtxMin;            // ���_�̍ŏ��l���
}

//=============================================================================
//    �e�N�X�`���ւ̃|�C���^�ݒ菈��
//=============================================================================
void CModel::BindTexture(LPDIRECT3DTEXTURE9 *pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ���b�V�����ւ̃|�C���^�ݒ菈��
//=============================================================================
void CModel::SetMesh(const LPD3DXMESH pMesh)
{
	m_pMesh = pMesh;
}

//=============================================================================
//    �}�e���A�����ւ̃|�C���^�ݒ菈��
//=============================================================================
void CModel::SetBuffMat(const LPD3DXBUFFER pBuffMat)
{
	m_pBuffMat = pBuffMat;
}

//=============================================================================
//    �}�e���A�����̐��ݒ菈��
//=============================================================================
void CModel::SetNumMat(const DWORD nNumMat)
{
	m_nNumMat = nNumMat;
}

//=============================================================================
//    ���b�V���̒��_���W�ŏ��l�ݒ菈��
//=============================================================================
void CModel::SetVtxMin(const D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//=============================================================================
//    ���b�V���̒��_���W�ő�l�ݒ菈��
//=============================================================================
void CModel::SetVtxMax(const D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
//    ���݂̍��W�ݒ菈��
//=============================================================================
void CModel::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ���݂̍��W�ɉ�����l�ݒ菈��
//=============================================================================
void CModel::SetAddPos(const D3DXVECTOR3 Addpos)
{
	m_AddPos = Addpos;
}

//=============================================================================
//    �����ݒ菈��
//=============================================================================
void CModel::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CModel::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    �e���f���ւ̃|�C���^�ݒ菈��
//=============================================================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}

//=============================================================================
//   ���b�V���̒��_���W�ŏ��l�擾����
//=============================================================================
D3DXVECTOR3 CModel::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
//    ���b�V���̒��_���W�ő�l�擾����
//=============================================================================
D3DXVECTOR3 CModel::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
//    ���݂̍��W�擾����
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ���݂̍��W�ɉ�����l�擾����
//=============================================================================
D3DXVECTOR3 CModel::GetAddPos(void)
{
	return m_AddPos;
}

//=============================================================================
//    �����擾����
//=============================================================================
D3DXVECTOR3 CModel::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CModel::GetMtxWorld(void)
{
	return m_MtxWorld;
}

D3DXMATRIX *CModel::GetMtxWorldPointer(void)
{
	return &m_MtxWorld;
}

//=============================================================================
//    ���b�V�����擾����
//=============================================================================
LPD3DXMESH CModel::GetMesh(void)
{
	return m_pMesh;
}

//=============================================================================
//    �}�e���A�����擾����
//=============================================================================
LPD3DXBUFFER CModel::GetBuffMat(void)
{
	return m_pBuffMat;
}

//=============================================================================
//    �}�e���A���̐��擾����
//=============================================================================
DWORD CModel::GetNumMat(void)
{
	return m_nNumMat;
}

//=============================================================================
//    �e���f���ւ̃|�C���^�擾����
//=============================================================================
CModel *CModel::GetParent(void)
{
	return m_pParent;
}