//*****************************************************************************
//
//     �V�[��(3D)���b�V���̏���[scene3DMesh.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "scene3DMesh.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define MESH_TEXTURE_NAME  "data/TEXTURE/tank.png"     // �e�N�X�`���̃t�@�C����(���)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CScene3DMesh::m_apTexture = NULL;  // ���b�V���̃e�N�X�`���摜

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CScene3DMesh::CScene3DMesh(int nPriority, OBJTYPE objType) : CScene3D(nPriority, objType)
{
	// �e��l�̃N���A
	m_pIdxBuff = NULL;     // �C���f�b�N�X�o�b�t�@
	m_nNumIndex = 0;       // �C���f�b�N�X��
	m_nNumVertex = 0;      // ���_��
	m_nNumPolygon = 0;     // �|���S����
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CScene3DMesh::~CScene3DMesh()
{

}

//=============================================================================
//    ���b�V���̐�������
//=============================================================================
CScene3DMesh *CScene3DMesh::Create(int nPriority)
{
	CScene3DMesh *pScene3DMesh = NULL;     // �V�[��3D���b�V���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pScene3DMesh == NULL)
		{// ����������ɂȂ��Ă���
			pScene3DMesh = new CScene3DMesh(nPriority);
			if (pScene3DMesh != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pScene3DMesh->Init()))
				{// �������Ɏ��s����
					return NULL;
				}
				else
				{// �������ɐ�������
				    // �C���X�^���X�̃A�h���X��Ԃ�
					return pScene3DMesh;
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
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}
}

//=============================================================================
//    ���b�V���̃e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CScene3DMesh::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			D3DXCreateTextureFromFile(pDevice, MESH_TEXTURE_NAME, &m_apTexture);
		}
	}
	return S_OK;
}

//=============================================================================
//    ���b�V���̃e�N�X�`���������
//=============================================================================
void CScene3DMesh::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
//    ���b�V���̏���������
//=============================================================================
HRESULT CScene3DMesh::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// ���_�o�b�t�@�̍쐬
			MakeVertex(pDevice);

			// �C���f�b�N�X�o�b�t�@�̍쐬
			MakeIndex(pDevice);
		}
		else
		{// �f�o�C�X���擾�ł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// �����_�����O�N���X����������Ă��Ȃ�
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    ���b�V���̏I������
//=============================================================================
void CScene3DMesh::Uninit(void)
{
	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	// ���ʂ̏I������
	CScene3D::Uninit();
}

//=============================================================================
//    ���b�V���̍X�V����
//=============================================================================
void CScene3DMesh::Update(void)
{

}

//=============================================================================
//    ���b�V���̕`�揈��
//=============================================================================
void CScene3DMesh::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // ���_�o�b�t�@�̎擾
			LPDIRECT3DTEXTURE9 pTexture = GetTexture();       // �e�N�X�`���ւ̃|�C���^���擾

			// ���[���h�}�g���b�N�X�̐ݒ�
			SetMtxWorld(pDevice);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetIndices(m_pIdxBuff);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			if (pTexture != NULL)
			{
				pDevice->SetTexture(0, pTexture);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
		}
	}
}

//=============================================================================
//    ���b�V���̃��[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CScene3DMesh::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	// �e��l�̎擾
	D3DXMATRIX mtxWorld = GetMtxWorld();  // ���[���h�}�g���b�N�X
	D3DXVECTOR3 rot = GetRot();           // ����
	D3DXVECTOR3 pos = GetPos();           // ���W

	D3DXMATRIX mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	CScene3D::SetMtxWorld(mtxWorld);
}

//=============================================================================
//    ���b�V���̒��_�o�b�t�@��������
//=============================================================================
void CScene3DMesh::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // ���_�o�b�t�@�̎擾

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);
}

//=============================================================================
//    ���b�V���̃C���f�b�N�X�o�b�t�@��������
//=============================================================================
void CScene3DMesh::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);
}

//=============================================================================
//    ���b�V���̃C���f�b�N�X�o�b�t�@�ݒ菈��
//=============================================================================
void CScene3DMesh::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    ���b�V���̒��_���ݒ菈��
//=============================================================================
void CScene3DMesh::SetNumVertex(const int nNumVertex)
{
	m_nNumVertex = nNumVertex;
}

//=============================================================================
//    ���b�V���̃C���f�b�N�X���ݒ菈��
//=============================================================================
void CScene3DMesh::SetNumIndex(const int nNumIndex)
{
	m_nNumIndex = nNumIndex;
}

//=============================================================================
//    ���b�V���̃|���S�����ݒ菈��
//=============================================================================
void CScene3DMesh::SetNumPolygon(const int nNumPolygon)
{
	m_nNumPolygon = nNumPolygon;
}

//=============================================================================
//    ���b�V���̃C���f�b�N�X�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CScene3DMesh::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    ���b�V���̒��_���擾����
//=============================================================================
int CScene3DMesh::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    ���b�V���̃C���f�b�N�X���擾����
//=============================================================================
int CScene3DMesh::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    ���b�V���̃|���S�����擾����
//=============================================================================
int CScene3DMesh::GetNumPolygon(void)
{
	return m_nNumPolygon;
}
