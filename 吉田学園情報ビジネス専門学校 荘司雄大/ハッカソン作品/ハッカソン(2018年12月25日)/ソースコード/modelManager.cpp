//*****************************************************************************
//
//     ���f���Ǌ��̏���[modelManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "modelManager.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CModelManager::CModelManager()
{
	// �e��l�̃N���A
	m_nNumModel = 0;    // �ǂݍ��񂾃��f���̐�
	m_pMesh = NULL;     // ���f���̃��b�V�����
	m_pBuffMat = NULL;  // ���f���̃}�e���A�����
	m_pNumMat = NULL;   // ���f���̃}�e���A���̐����
	m_pVtxMax = NULL;   // ���_���W�̍ő�l�ւ̃|�C���^
	m_pVtxMin = NULL;   // ���_���W�̍ŏ��l�ւ̃|�C���^
	m_pTexture = NULL;  // �e�N�X�`���ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CModelManager::~CModelManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CModelManager *CModelManager::Create(int nNumModel)
{
	CModelManager *pModelManager = NULL;  // ���f���Ǌ��N���X�^�̃|�C���^
	if (pModelManager == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pModelManager = new CModelManager;
		if (pModelManager != NULL)
		{// ���������m�ۂł���
			pModelManager->SetNumModel(nNumModel);  // ���f���̐���ݒ�
													// �������������s��
			if (FAILED(pModelManager->Init()))
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
	return pModelManager;
}

//=============================================================================
//    ���f���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CModelManager::Init(void)
{
	if (m_nNumModel >= 1)
	{// �ǂݍ��ރ��f���̐���1�ȏ゠��
	    // �ǂݍ��ރ��f���̐��������������m�ۂ���
	    // ���b�V�����
		if (m_pMesh == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_pMesh = new LPD3DXMESH[m_nNumModel];
			if (m_pMesh != NULL)
			{// ���������m�ۂł���
				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{// �ǂݍ��ރ��f���̐������J��Ԃ�
					m_pMesh[nCntModel] = NULL;
				}
			}
		}

		// �}�e���A�����
		if (m_pBuffMat == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_pBuffMat = new LPD3DXBUFFER[m_nNumModel];
			if (m_pBuffMat != NULL)
			{// ���������m�ۂł���
				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{// �ǂݍ��ރ��f���̐������J��Ԃ�
					m_pBuffMat[nCntModel] = NULL;
				}
			}
		}

		// �}�e���A���̐����
		if (m_pNumMat == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_pNumMat = new DWORD[m_nNumModel];
		}

		// ���_���W�̍ő�l���
		if (m_pVtxMax == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_pVtxMax = new D3DXVECTOR3[m_nNumModel];
		}

		// ���_���W�̍ŏ��l���
		if (m_pVtxMin == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_pVtxMin = new D3DXVECTOR3[m_nNumModel];
		}

		// �e�N�X�`���ւ̃|�C���^���
		if (m_pTexture == NULL)
		{// ���������m�ۂł����Ԃł���
			m_pTexture = new LPDIRECT3DTEXTURE9*[m_nNumModel];
			if (m_pTexture != NULL)
			{// ���������m�ۂł���
				for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
				{// �ǂݍ��ރ��f���̐������J��Ԃ�
					m_pTexture[nCntModel] = NULL;
				}
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    ���f���̊J������
//=============================================================================
void CModelManager::Uninit(void)
{
	// �e�N�X�`���ւ̃|�C���^���̊J��
	if (m_pTexture != NULL)
	{// ���������m�ۂ���Ă���
		ReleaseTexture();
	}

	// ���b�V�����̊J��
	if (m_pMesh != NULL)
	{// ���������m�ۂ���Ă���
		ReleaseMesh();
	}

	// �}�e���A�����̊J��
	if (m_pBuffMat != NULL)
	{// ���������m�ۂ���Ă���
		ReleaseBuffMat();
	}

	// �}�e���A���̐����̊J��
	if (m_pNumMat != NULL)
	{// ���������m�ۂ���Ă���
		ReleaseNumMat();
	}

	// ���_�̍ő�l���̊J��
	if (m_pVtxMax != NULL)
	{// ���������m�ۂ���Ă���
		ReleaseVtxMax();
	}

	// ���_�̍ŏ��l���̊J��
	if (m_pVtxMin != NULL)
	{// ���������m�ۂ���Ă���
		ReleaseVtxMin();
	}
}

//=============================================================================
//    �ǂݍ��ރ��f���̐���ݒ肷��
//=============================================================================
void CModelManager::SetNumModel(int nNumModel)
{
	m_nNumModel = nNumModel;
}

//=============================================================================
//    ���f���̃��b�V������ݒ肷��
//=============================================================================
void CModelManager::SetMesh(LPD3DXMESH pMesh, int nIdx)
{
	m_pMesh[nIdx] = pMesh;

	if (m_pMesh[nIdx] != NULL)
	{// ���f�����ǂݍ��߂�
	    // �ŏ��l�ƍő�l��������
		m_pVtxMin[nIdx] = D3DXVECTOR3(10000000.0f, 10000000.0f, 10000000.0f);
		m_pVtxMax[nIdx] = D3DXVECTOR3(-10000000.0f, -10000000.0f, -10000000.0f);

		int nNumVtx;     // ���_��
		DWORD sizeFVF;   // ���_�t�H�[�}�b�g�̃T�C�Y
		BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^

		// ���_�����l��
		nNumVtx = m_pMesh[nIdx]->GetNumVertices();

		// ���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh[nIdx]->GetFVF());

		// ���_�o�b�t�@�����b�N
		m_pMesh[nIdx]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{// ���ׂĂ̒��_���r���ă��f���̍ŏ��l,�ő�l�𔲂��o��
			D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // ���_���W�̑��

			if (Vtx.x < m_pVtxMin[nIdx].x)
			{// ���o����X���W�̍ŏ��l�����܂ł̂���������
				m_pVtxMin[nIdx].x = Vtx.x;  // �ŏ��l�X�V
			}
			if (Vtx.y < m_pVtxMin[nIdx].y)
			{// ���o����Y���W�̍ŏ��l�����܂ł̂���������
				m_pVtxMin[nIdx].y = Vtx.y;  // �ŏ��l�X�V
			}
			if (Vtx.z < m_pVtxMin[nIdx].z)
			{// ���o����Z���W�̍ŏ��l�����܂ł̂���������
				m_pVtxMin[nIdx].z = Vtx.z;  // �ŏ��l�X�V
			}
			if (Vtx.x > m_pVtxMax[nIdx].x)
			{// ���o����X���W�̍ő�l�����܂ł̂����傫��
				m_pVtxMax[nIdx].x = Vtx.x;  // �ő�l�X�V
			}
			if (Vtx.y > m_pVtxMax[nIdx].y)
			{// ���o����Y���W�̍ő�l�����܂ł̂����傫��
				m_pVtxMax[nIdx].y = Vtx.y;  // �ő�l�X�V
			}
			if (Vtx.z > m_pVtxMax[nIdx].z)
			{// ���o����Z���W�̍ő�l�����܂ł̂����傫��
				m_pVtxMax[nIdx].z = Vtx.z;  // �ő�l�X�V
			}

			pVtxBuff += sizeFVF;  // �T�C�Y���|�C���^��i�߂�
		}
		// ���_�o�b�t�@���A�����b�N
		m_pMesh[nIdx]->UnlockVertexBuffer();
	}
}

//=============================================================================
//    ���f���̃}�e���A������ݒ肷��
//=============================================================================
void CModelManager::SetMaterial(LPDIRECT3DDEVICE9 pDevice, LPD3DXBUFFER pBuffMat, DWORD nNumMat, int nIdx)
{
	m_pBuffMat[nIdx] = pBuffMat;
	m_pNumMat[nIdx] = nNumMat;

	// �e�N�X�`�����𔲂��o��
	// �}�e���A����񂩂�e�N�X�`�����������o��
	if (m_pBuffMat != NULL)
	{// �}�e���A����񂪓ǂݍ��߂Ă���
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat[nIdx]->GetBufferPointer();  // �}�e���A�����̃|�C���^���擾
		D3DMATERIAL9* pMatBuff = new D3DMATERIAL9[nNumMat];                        // �}�e���A�����̐������|�C���^�𓮓I�m��
		m_pTexture[nIdx] = new LPDIRECT3DTEXTURE9[nNumMat];                        // �e�N�X�`�����ւ̃|�C���^���}�e���A�����̐��������I�m��

		for (DWORD nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
		{// ���_���̐������J��Ԃ�
		    // �}�e���A������ǂݍ���
			pMatBuff[nCntMat] = pMat[nCntMat].MatD3D;

			// ����������������
			pMatBuff[nCntMat].Ambient = pMatBuff[nCntMat].Diffuse;

			// �e�N�X�`���ւ̃|�C���^����ɂ��Ă���
			m_pTexture[nIdx][nCntMat] = NULL;

			// �e�N�X�`���̏���ǂݍ���
			if (pMat[nCntMat].pTextureFilename != NULL &&
				lstrlen(pMat[nCntMat].pTextureFilename) > 0)
			{// �e�N�X�`���̃t�@�C����������
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&m_pTexture[nIdx][nCntMat]);
			}
		}
		// �g�p�������������J�����Ă���
		delete[] pMatBuff;
		pMatBuff = NULL;
	}
}

//=============================================================================
//    ���f���̃}�e���A�����ւ̃|�C���^��ݒ肷��
//=============================================================================
void CModelManager::SetBuffMat(LPD3DXBUFFER pBuffMat, int nIdx)
{
	m_pBuffMat[nIdx] = pBuffMat;
}

//=============================================================================
//    ���f���̃}�e���A���̐�����ݒ肷��
//=============================================================================
void CModelManager::SetNumMat(DWORD nNumMat, int nIdx)
{
	m_pNumMat[nIdx] = nNumMat;
}

//=============================================================================
//    ���f���̒��_�̍ő�l����ݒ肷��
//=============================================================================
void CModelManager::SetVtxMax(D3DXVECTOR3 VtxMax, int nIdx)
{
	m_pVtxMax[nIdx] = VtxMax;
}

//=============================================================================
//    ���f���̒��_�̍ŏ��l����ݒ肷��
//=============================================================================
void CModelManager::SetVtxMin(D3DXVECTOR3 VtxMin, int nIdx)
{
	m_pVtxMin[nIdx] = VtxMin;
}

//=============================================================================
//    ���f���̃e�N�X�`���ւ̃|�C���^����ݒ肷��
//=============================================================================
void CModelManager::BindTexture(LPDIRECT3DTEXTURE9 *pTexture, int nIdx)
{
	m_pTexture[nIdx] = pTexture;
}

//=============================================================================
//    ���f���̃��b�V�������擾����
//=============================================================================
LPD3DXMESH CModelManager::GetMesh(int nIdx)
{
	LPD3DXMESH pMesh = NULL;  // ���b�V�����

	if (m_pMesh != NULL)
	{// ���������m�ۂ���Ă�
		pMesh = m_pMesh[nIdx];   // �ۑ����ꂽ���b�V��������
	}

	return pMesh;  // ���b�V������Ԃ�
}

//=============================================================================
//    ���f���̃}�e���A�������擾����
//=============================================================================
LPD3DXBUFFER CModelManager::GetBuffMat(int nIdx)
{
	LPD3DXBUFFER pBuffMat = NULL;  // �}�e���A�����

	if (m_pBuffMat != NULL)
	{// ���������m�ۂ���Ă�
		pBuffMat = m_pBuffMat[nIdx];   // �ۑ����ꂽ�}�e���A��������
	}

	return pBuffMat;  // �}�e���A������Ԃ�
}

//=============================================================================
//    ���f���̃}�e���A���̐������擾����
//=============================================================================
DWORD CModelManager::GetNumMat(int nIdx)
{
	DWORD nNumMat = 0;  // �}�e���A���̐����

	if (m_pNumMat != NULL)
	{// ���������m�ۂ���Ă�
		nNumMat = m_pNumMat[nIdx];   // �ۑ����ꂽ�}�e���A���̐�������
	}

	return nNumMat;  // �}�e���A���̐�����Ԃ�
}

//=============================================================================
//    ���f���̒��_�̍ő�l�����擾����
//=============================================================================
D3DXVECTOR3 CModelManager::GetVtxMax(int nIdx)
{
	D3DXVECTOR3 VtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���_�̍ő�l���

	if (m_pVtxMax != NULL)
	{// ���������m�ۂ���Ă�
		VtxMax = m_pVtxMax[nIdx];   // �ۑ����ꂽ���_�̍ő�l������
	}

	return VtxMax;  // ���_�̍ő�l����Ԃ�
}

//=============================================================================
//    ���f���̒��_�̍ŏ��l�����擾����
//=============================================================================
D3DXVECTOR3 CModelManager::GetVtxMin(int nIdx)
{
	D3DXVECTOR3 VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���_�̍ŏ��l���

	if (m_pVtxMin != NULL)
	{// ���������m�ۂ���Ă�
		VtxMin = m_pVtxMin[nIdx];   // �ۑ����ꂽ���_�̍ŏ��l������
	}

	return VtxMin;  // ���_�̍ŏ��l����Ԃ�
}

//=============================================================================
//    ���f���̃e�N�X�`���ւ̃|�C���^�����擾����
//=============================================================================
LPDIRECT3DTEXTURE9 *CModelManager::GetTexture(int nIdx)
{
	LPDIRECT3DTEXTURE9 *pTexture = NULL;  // �e�N�X�`�����ւ̃|�C���^

	if (m_pTexture != NULL)
	{// ���������m�ۂ���Ă�
		pTexture = m_pTexture[nIdx];      // �ۑ����ꂽ�e�N�X�`�����ւ̃|�C���^����
	}

	return pTexture;  // �e�N�X�`�����ւ̃|�C���^��Ԃ�
}

//=============================================================================
//    ���f���̃��b�V�������J������
//=============================================================================
void CModelManager::ReleaseMesh(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{// �ǂݍ��񂾃��f���̐������J��Ԃ�
		DIRECT_RELEASE(m_pMesh[nCntModel]);
	}
	delete[] m_pMesh;
	m_pMesh = NULL;
}

//=============================================================================
//    ���f���̃}�e���A�������J������
//=============================================================================
void CModelManager::ReleaseBuffMat(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{// �ǂݍ��񂾃��f���̐������J��Ԃ�
		DIRECT_RELEASE(m_pBuffMat[nCntModel]);
	}
	delete[] m_pBuffMat;
	m_pBuffMat = NULL;
}

//=============================================================================
//    ���f���̃}�e���A���̐������J������
//=============================================================================
void CModelManager::ReleaseNumMat(void)
{
	delete[] m_pNumMat;
	m_pNumMat = NULL;
}

//=============================================================================
//    ���f���̒��_�̍ő�l�����J������
//=============================================================================
void CModelManager::ReleaseVtxMax(void)
{
	delete[] m_pVtxMax;
	m_pVtxMax = NULL;
}

//=============================================================================
//    ���f���̒��_�̍ŏ��l�������J������
//=============================================================================
void CModelManager::ReleaseVtxMin(void)
{
	delete[] m_pVtxMin;
	m_pVtxMin = NULL;
}

//=============================================================================
//    ���f���̃e�N�X�`���ւ̃|�C���^�����J������
//=============================================================================
void CModelManager::ReleaseTexture(void)
{
	if (m_pTexture != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{// �ǂݍ��񂾃��f�������J��Ԃ�
			if (m_pTexture[nCntModel] != NULL)
			{// ���������m�ۂ���Ă���
				for (int nCntMat = 0; nCntMat < (int)m_pNumMat[nCntModel]; nCntMat++)
				{// �}�e���A�����̐������J�肩����
					DIRECT_RELEASE(m_pTexture[nCntModel][nCntMat]);
				}
				delete[] m_pTexture[nCntModel];
				m_pTexture[nCntModel] = NULL;
			}
		}
		delete[] m_pTexture;
		m_pTexture = NULL;
	}
}