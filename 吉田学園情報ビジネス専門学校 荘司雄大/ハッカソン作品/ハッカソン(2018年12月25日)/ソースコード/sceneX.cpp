//*****************************************************************************
//
//     X�t�@�C�����f���̏���[sceneX.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "sceneX.h"
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
CSceneX::CSceneX(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pTexture = NULL;                        // �e�N�X�`���ւ̃|�C���^
	m_pMesh = NULL;                           // ���b�V�����ւ̃|�C���^
	m_pBuffMat = NULL;                        // �}�e���A�����ւ̃|�C���^
	m_nNumMat = 0;                            // �}�e���A�����̐�
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ����
	D3DXMatrixIdentity(&m_MtxWorld);          // ���[���h�}�g���b�N�X
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSceneX::~CSceneX()
{

}

//=============================================================================
//    ��������
//=============================================================================
CSceneX *CSceneX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, const D3DXVECTOR3 VtxMax, const D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture, int nPriority)
{
	CSceneX *pSceneX = NULL;               // X�t�@�C�����f���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pSceneX == NULL)
		{// ����������ɂȂ��Ă���
			pSceneX = new CSceneX(nPriority);
			if (pSceneX != NULL)
			{// �C���X�^���X�𐶐��ł���
				// �e��l�̐ݒ�
				pSceneX->SetPos(pos);           // ���W
				pSceneX->SetRot(rot);           // ����

				// ���f������ݒ�
				pSceneX->BindModel(pMesh, pBuffMat, nNumMat, VtxMax, VtxMin, pTexture);

				if (FAILED(pSceneX->Init()))
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
	}
	else
	{// ����ȏ�V�[�����쐬�ł��Ȃ�
		return NULL;
	}

	return pSceneX;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CSceneX::Init(void)
{
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSceneX::Uninit(void)
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

	Release();  // ���g�̃I�u�W�F�N�g��j��
}

//=============================================================================
//    �X�V����
//=============================================================================
void CSceneX::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CSceneX::Draw(void)
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
void CSceneX::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    ���f�����ݒ菈��
//=============================================================================
void CSceneX::BindModel(const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat, const D3DXVECTOR3 VtxMax, const D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture)
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
	if (pTexture != NULL)
	{// �e�N�X�`�����ւ̃|�C���^���ǂݍ��߂�
		m_pTexture = pTexture;    // �e�N�X�`�����ւ̃|�C���^
	}
	m_VtxMax = VtxMax;            // ���_�̍ő�l���
	m_VtxMin = VtxMin;            // ���_�̍ŏ��l���
}

//=============================================================================
//    ���b�V���̒��_���W�ŏ��l�ݒ菈��
//=============================================================================
void CSceneX::SetMesh(const LPD3DXMESH pMesh)
{
	m_pMesh = pMesh;
}

//=============================================================================
//    �}�e���A�����ւ̃|�C���^�ݒ菈��
//=============================================================================
void CSceneX::SetBuffMat(const LPD3DXBUFFER pBuffMat)
{
	m_pBuffMat = pBuffMat;
}

//=============================================================================
//    �}�e���A�����̐��ݒ菈��
//=============================================================================
void CSceneX::SetNumMat(const DWORD nNumMat)
{
	m_nNumMat = nNumMat;
}

//=============================================================================
//    ���b�V���̒��_���W�ŏ��l�ݒ菈��
//=============================================================================
void CSceneX::SetVtxMin(const D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//=============================================================================
//    ���b�V���̒��_���W�ő�l�ݒ菈��
//=============================================================================
void CSceneX::SetVtxMax(const D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
//    ���W�ݒ菈��
//=============================================================================
void CSceneX::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �����ݒ菈��
//=============================================================================
void CSceneX::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CSceneX::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//   ���b�V���̒��_���W�ŏ��l�擾����
//=============================================================================
D3DXVECTOR3 CSceneX::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
//    ���b�V���̒��_���W�ő�l�擾����
//=============================================================================
D3DXVECTOR3 CSceneX::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
//    �e�N�X�`�����ւ̃|�C���^�擾����
//=============================================================================
LPDIRECT3DTEXTURE9 *CSceneX::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ���W�擾����
//=============================================================================
D3DXVECTOR3 CSceneX::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �����擾����
//=============================================================================
D3DXVECTOR3 CSceneX::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CSceneX::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    ���b�V�����擾����
//=============================================================================
LPD3DXMESH CSceneX::GetMesh(void)
{
	return m_pMesh;
}

//=============================================================================
//    �}�e���A�����擾����
//=============================================================================
LPD3DXBUFFER CSceneX::GetBuffMat(void)
{
	return m_pBuffMat;
}

//=============================================================================
//    �}�e���A���̐��擾����
//=============================================================================
DWORD CSceneX::GetNumMat(void)
{
	return m_nNumMat;
}