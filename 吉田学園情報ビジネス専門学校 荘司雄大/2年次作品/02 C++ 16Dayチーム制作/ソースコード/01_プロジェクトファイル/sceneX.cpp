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
#define XFILE_NAME  "data/MODEL/object/coin000.x"     // ���f���̃t�@�C����

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH     CSceneX::m_apMesh = NULL;                                              // ���b�V�����ւ̃|�C���^
LPD3DXBUFFER   CSceneX::m_apBuffMat = NULL;                                           // �}�e���A�����ւ̃|�C���^
DWORD          CSceneX::m_anNumMat = NULL;                                            // �}�e���A�����̐�
D3DXVECTOR3    CSceneX::m_aVtxMin = D3DXVECTOR3(100000.0f, 1000000.0f, 100000.0f);    // ���_���W�̍ŏ��l
D3DXVECTOR3    CSceneX::m_aVtxMax = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f); // ���_���W�̍ŏ��l

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
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���݂̍��W
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �O��̍��W
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
//    X�t�@�C�����f���̐�������
//=============================================================================
CSceneX *CSceneX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority)
{
	CSceneX *pSceneX = NULL;   // X�t�@�C�����f���N���X�^�̃|�C���^
	if (pSceneX == NULL)
	{// ����������ɂȂ��Ă���
		pSceneX = new CSceneX(nPriority);
		if (pSceneX != NULL)
		{// �C���X�^���X�𐶐��ł���
			pSceneX->SetPos(pos);
			pSceneX->SetRot(rot);
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

	return pSceneX;
}

//=============================================================================
//    X�t�@�C�����f���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CSceneX::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	     // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
	        // x�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(XFILE_NAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_apBuffMat,
				NULL,
				&m_anNumMat,
				&m_apMesh);

			int nNumVtx;     // ���_��
			DWORD sizeFVF;   // ���_�t�H�[�}�b�g�̃T�C�Y
			BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^

			// ���_�����l��
			nNumVtx = m_apMesh->GetNumVertices();

			// ���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(m_apMesh->GetFVF());

			// ���_�o�b�t�@�����b�N
			m_apMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// ���ׂĂ̒��_���r���ă��f���̍ŏ��l,�ő�l�𔲂��o��
				D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // ���_���W�̑��

				if (Vtx.x < m_aVtxMin.x)
				{// ���o����X���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.x = Vtx.x;  // �ŏ��l�X�V
				}
				if (Vtx.y < m_aVtxMin.y)
				{// ���o����Y���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.y = Vtx.y;  // �ŏ��l�X�V
				}
				if (Vtx.z < m_aVtxMin.z)
				{// ���o����Z���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.z = Vtx.z;  // �ŏ��l�X�V
				}
				if (Vtx.x > m_aVtxMax.x)
				{// ���o����X���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.x = Vtx.x;  // �ő�l�X�V
				}
				if (Vtx.y > m_aVtxMax.y)
				{// ���o����Y���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.y = Vtx.y;  // �ő�l�X�V
				}
				if (Vtx.z > m_aVtxMax.z)
				{// ���o����Z���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.z = Vtx.z;  // �ő�l�X�V
				}

				pVtxBuff += sizeFVF;  // �T�C�Y���|�C���^��i�߂�
			}
			// ���_�o�b�t�@���A�����b�N
			m_apMesh->UnlockVertexBuffer();
		}
	}

	return S_OK;
}

//=============================================================================
//    X�t�@�C�����f���̊J������
//=============================================================================
void CSceneX::UnLoad(void)
{
	// ���b�V���̔j��
	if (m_apMesh != NULL)
	{
		m_apMesh->Release();
		m_apMesh = NULL;
	}

	// �}�e���A���̔j��
	if (m_apBuffMat != NULL)
	{
		m_apBuffMat->Release();
		m_apBuffMat = NULL;
	}
}

//=============================================================================
//    X�t�@�C�����f���̏���������
//=============================================================================
HRESULT CSceneX::Init(void)
{

	return S_OK;
}

//=============================================================================
//    X�t�@�C�����f���̏I������
//=============================================================================
void CSceneX::Uninit(void)
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
		delete m_pTexture;
		m_pTexture = NULL;
	}

	Release();  // ���g�̃I�u�W�F�N�g��j��
}

//=============================================================================
//    X�t�@�C�����f���̍X�V����
//=============================================================================
void CSceneX::Update(void)
{

}

//=============================================================================
//    X�t�@�C�����f���̕`�揈��
//=============================================================================
void CSceneX::Draw(void)
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
//    X�t�@�C�����f���̃��[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CSceneX::CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
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
	SetMtxWorld(m_MtxWorld);
}

//=============================================================================
//    X�t�@�C�����f���̃��f�����ݒ菈��
//=============================================================================
void CSceneX::BindModel(const LPDIRECT3DDEVICE9 pDevice, const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat)
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
	if (nNumMat >= 0)
	{// �}�e���A���̐���񂪓ǂݍ��߂�
		m_nNumMat = nNumMat;   // �}�e���A�����̐�
	}

	// �}�e���A����񂩂�e�N�X�`�����������o��
	if (m_pBuffMat != NULL && m_nNumMat >= 0)
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
//    X�t�@�C�����f���̒��_���W�ŏ��l�ݒ菈��
//=============================================================================
void CSceneX::SetVtxMin(const D3DXVECTOR3 VtxMin)
{
	m_VtxMin = VtxMin;
}

//=============================================================================
//    X�t�@�C�����f���̒��_���W�ő�l�ݒ菈��
//=============================================================================
void CSceneX::SetVtxMax(const D3DXVECTOR3 VtxMax)
{
	m_VtxMax = VtxMax;
}

//=============================================================================
//    X�t�@�C�����f���̌��݂̍��W�ݒ菈��
//=============================================================================
void CSceneX::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    X�t�@�C�����f���̑O��̍��W�ݒ菈��
//=============================================================================
void CSceneX::SetPosOld(const D3DXVECTOR3 posOld)
{
	m_PosOld = posOld;
}

//=============================================================================
//    X�t�@�C�����f���̌����ݒ菈��
//=============================================================================
void CSceneX::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    X�t�@�C�����f���̃��[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CSceneX::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    X�t�@�C�����f���̒��_���W�ŏ��l�擾����
//=============================================================================
D3DXVECTOR3 CSceneX::GetVtxMin(void)
{
	return m_VtxMin;
}

//=============================================================================
//    X�t�@�C�����f���̒��_���W�ő�l�擾����
//=============================================================================
D3DXVECTOR3 CSceneX::GetVtxMax(void)
{
	return m_VtxMax;
}

//=============================================================================
//    X�t�@�C�����f���̌��݂̍��W�擾����
//=============================================================================
D3DXVECTOR3 CSceneX::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    X�t�@�C�����f���̑O��̍��W�ݒ菈��
//=============================================================================
D3DXVECTOR3 CSceneX::GetPosOld(void)
{
	return m_PosOld;
}

//=============================================================================
//    X�t�@�C�����f���̌����擾����
//=============================================================================
D3DXVECTOR3 CSceneX::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    X�t�@�C�����f���̃��[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CSceneX::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    X�t�@�C�����f���̃��b�V�����擾����
//=============================================================================
LPD3DXMESH CSceneX::GetMesh(void)
{
	return m_pMesh;
}

//=============================================================================
//    X�t�@�C�����f���̃}�e���A�����擾����
//=============================================================================
LPD3DXBUFFER CSceneX::GetBuffMat(void)
{
	return m_pBuffMat;
}

//=============================================================================
//    X�t�@�C�����f���̃}�e���A���̐��擾����
//=============================================================================
DWORD CSceneX::GetNumMat(void)
{
	return m_nNumMat;
}

//=============================================================================
//    X�t�@�C�����f���̃e�N�X�`�����擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CSceneX::GetTexture(int nIdxTex)
{
	return m_pTexture[nIdxTex];
}