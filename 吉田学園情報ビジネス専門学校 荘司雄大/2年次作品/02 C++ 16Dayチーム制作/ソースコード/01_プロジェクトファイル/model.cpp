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
#define XFILE_NAME  "data/MODEL/coin000.x"     // ���f���̃t�@�C����

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CModel::CModel()
{
	// �e��l�̃N���A
	m_pTexture = NULL;                          // �e�N�X�`���ւ̃|�C���^
	m_pMesh = NULL;                             // ���b�V�����ւ̃|�C���^
	m_pBuffMat = NULL;                          // �}�e���A�����ւ̃|�C���^
	m_nNumMat = 0;                              // �}�e���A�����̐�
	m_VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // ���_�̍ő�l
	m_VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // ���_�̍ŏ��l
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���݂̍��W
	m_InitPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���������̍��W
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �O��̍��W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ����
	D3DXMatrixIdentity(&m_MtxWorld);            // ���[���h�}�g���b�N�X
	m_pParent = NULL;                           // �e���f���ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
//    ���f���̐�������
//=============================================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 VtxMin, D3DXVECTOR3 VtxMax)
{
	CModel *pModel= NULL;   // ���f���N���X�^�̃|�C���^
	if (pModel == NULL)
	{// ����������ɂȂ��Ă���
		pModel = new CModel;
		if (pModel != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pModel->Init(pos, rot, pMesh, pBuffMat, nNumMat, VtxMin, VtxMin)))
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

	return pModel;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ���f���̏���������
//=============================================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 VtxMin, D3DXVECTOR3 VtxMax)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// �e��l�̐ݒ�
			m_InitPos = pos;   // ���������̍��W
			m_Pos = pos;       // ���݂̍��W
			m_Rot = rot;       // ���݂̌���
			m_VtxMin = VtxMin; // ���_���W�̍ŏ��l
			m_VtxMax = VtxMax; // ���_���W�̍ő�l

		    // ���f���̊��蓖��
			BindModel(pDevice, pMesh, pBuffMat, nNumMat);
		}
	}

	return S_OK;
}

//=============================================================================
//    ���f���̏I������
//=============================================================================
void CModel::Uninit(void)
{
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{// ���������m�ۂ���Ă���
		for (DWORD nCntMat = 0; nCntMat < m_nNumMat; nCntMat++)
		{
			if (m_pTexture[nCntMat] != NULL)
			{
				m_pTexture[nCntMat]->Release();
				m_pTexture[nCntMat] = NULL;
			}
		}
		delete[] m_pTexture;
		m_pTexture = NULL;
	}
}

//=============================================================================
//    ���f���̍X�V����
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
//    ���f���̕`�揈��
//=============================================================================
void CModel::Draw(void)
{
	D3DMATERIAL9 matDef;   // ���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;    // �}�e���A���f�[�^�ւ̃|�C���^

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ���[���h�}�g���b�N�X�̌v�Z����
	CalcMtxWorld(pDevice);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{// �ݒ肳��Ă����}�e���A���̐������J��Ԃ�
	 // �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		if (m_pTexture[nCntMat] != NULL)
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

	// �ۑ����Ă����}�e���A���ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
//    ���f���̃��[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CModel::CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
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
		mtxParent = m_pParent->GetMtxWorld();  // �e���f�����烏�[���h�}�g���b�N�X���擾����
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
//    ���f���̃��f�����ݒ菈��
//=============================================================================
void CModel::BindModel(const LPDIRECT3DDEVICE9 pDevice, const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat)
{
	// ���f�����̊��蓖��
	if (pMesh != NULL)
	{// ���b�V����񂪓ǂݍ��߂�
		m_pMesh = pMesh;       // ���b�V�����ւ̃|�C���^
	}
	if (pBuffMat != NULL)
	{// �}�e���A�����ւ̃|�C���^��ǂݍ��߂�
		m_pBuffMat = pBuffMat; // �}�e���A�����ւ̃|�C���^
	}
	if (nNumMat != -1)
	{// �}�e���A���̐���񂪓ǂݍ��߂�
		m_nNumMat = nNumMat;   // �}�e���A�����̐�
	}

	// �}�e���A����񂩂�e�N�X�`�����������o��
	if (m_pBuffMat != NULL)
	{// �}�e���A����񂪓ǂݍ��߂Ă���
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();  // �}�e���A�����̃|�C���^���擾
		D3DMATERIAL9* pMatBuff = new D3DMATERIAL9[m_nNumMat];                // �}�e���A�����̐������|�C���^�𓮓I�m��
		m_pTexture = new LPDIRECT3DTEXTURE9[m_nNumMat];                      // �e�N�X�`�����ւ̃|�C���^���}�e���A�����̐��������I�m��

		for (DWORD nCntMat = 0; nCntMat < m_nNumMat; nCntMat++)
		{// ���_���̐������J��Ԃ�
		    // �}�e���A������ǂݍ���
			pMatBuff[nCntMat] = pMat[nCntMat].MatD3D;

			// ����������������
			pMatBuff[nCntMat].Ambient = pMatBuff[nCntMat].Diffuse;

			// �e�N�X�`������������
			m_pTexture[nCntMat] = NULL;

			// �e�N�X�`���̏���ǂݍ���
			if (pMat[nCntMat].pTextureFilename != NULL &&
				lstrlen(pMat[nCntMat].pTextureFilename) > 0)
			{// �e�N�X�`���̃t�@�C����������
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&m_pTexture[nCntMat]);
			}
		}

		delete[] pMatBuff;
	}
}

//=============================================================================
//    ���f���̒��_���W�ŏ��l�ݒ菈��
//=============================================================================
void CModel::SetVtxMin(const D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//=============================================================================
//    ���f���̒��_���W�ő�l�ݒ菈��
//=============================================================================
void CModel::SetVtxMax(const D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
//    ���f���̌��݂̍��W�ݒ菈��
//=============================================================================
void CModel::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ���f���̃I�t�Z�b�g�ɉ�������W�ݒ菈��
//=============================================================================
void CModel::SetAddPos(const D3DXVECTOR3 AddPos)
{
	m_AddPos = AddPos;
}

//=============================================================================
//    ���f���̑O��̍��W�ݒ菈��
//=============================================================================
void CModel::SetPosOld(const D3DXVECTOR3 posOld)
{
	m_PosOld = posOld;
}

//=============================================================================
//    ���f���̌����ݒ菈��
//=============================================================================
void CModel::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ���f���̃��[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CModel::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    ���f���̐e���f���ݒ菈��
//=============================================================================
void CModel::SetParent(CModel *pModel)
{
	m_pParent = pModel;
}

//=============================================================================
//    ���f���̒��_���W�ŏ��l�擾����
//=============================================================================
D3DXVECTOR3 CModel::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
//    ���f���̒��_���W�ő�l�擾����
//=============================================================================
D3DXVECTOR3 CModel::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
//    ���f���̌��݂̍��W�擾����
//=============================================================================
D3DXVECTOR3 CModel::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ���f���̃I�t�Z�b�g�ɉ�������W�擾����
//=============================================================================
D3DXVECTOR3 CModel::GetAddPos(void)
{
	return m_AddPos;
}

//=============================================================================
//    ���f���̏��������̍��W�擾����
//=============================================================================
D3DXVECTOR3 CModel::GetInitPos(void)
{
	return m_InitPos;
}

//=============================================================================
//    ���f���̑O��̍��W�ݒ菈��
//=============================================================================
D3DXVECTOR3 CModel::GetPosOld(void)
{
	return m_PosOld;
}

//=============================================================================
//    ���f���̌����擾����
//=============================================================================
D3DXVECTOR3 CModel::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ���f���̃��[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CModel::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    ���f���̃��b�V�����擾����
//=============================================================================
LPD3DXMESH CModel::GetMesh(void)
{
	return m_pMesh;
}

//=============================================================================
//    ���f���̃}�e���A�����擾����
//=============================================================================
LPD3DXBUFFER CModel::GetBuffMat(void)
{
	return m_pBuffMat;
}

//=============================================================================
//    ���f���̃}�e���A���̐��擾����
//=============================================================================
DWORD CModel::GetNumMat(void)
{
	return m_nNumMat;
}

//=============================================================================
//    ���f���̃e�N�X�`�����擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CModel::GetTexture(int nIdxTex)
{
	return m_pTexture[nIdxTex];
}