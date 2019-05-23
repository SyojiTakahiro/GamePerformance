//*****************************************************************************
//
//     �O�Ղ̏���[meshOrbit.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "meshOrbit.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CMeshOrbit::CMeshOrbit(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pVtxBuff = NULL;                          // ���_�o�b�t�@�ւ̃|�C���^
	m_pTexture = NULL;	                        // �e�N�X�`���ւ̃|�C���^
	m_nXBlock = 0;                              // ���̕�����
	m_nZBlock = 0;                              // ���s�̕�����
	m_nNumVertex = 0;                           // ���_��
	m_nNumPolygon = 0;                          // �|���S����
	m_pMtxParent = NULL;                        // �e�̃}�g���b�N�X�̃|�C���^
	m_pVertexPos = NULL;                        // ���_���W(���_�������I�Ɋm��)
	m_pVertexCol = NULL;                        // ���_�J���[(���_�������I�Ɋm��)
	D3DXMatrixIdentity(&m_MtxWorld);            // ���[���h�}�g���b�N�X
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMeshOrbit::~CMeshOrbit()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMeshOrbit *CMeshOrbit::Create(D3DXVECTOR3 OffsetPos1, D3DXVECTOR3 OffsetPos2, int nXBlock, int nZBlock, int nPriority)
{
	CMeshOrbit *pMeshOrbit= NULL;          // �O�ՃN���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pMeshOrbit == NULL)
		{// ����������ɂȂ��Ă���
			pMeshOrbit = new CMeshOrbit(nPriority);
			if (pMeshOrbit != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pMeshOrbit->SetOffsetPos(OffsetPos1,0);
				pMeshOrbit->SetOffsetPos(OffsetPos2,1);
				pMeshOrbit->SetXBlock(nXBlock);
				pMeshOrbit->SetZBlock(nZBlock);

				if (FAILED(pMeshOrbit->Init()))
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

	return pMeshOrbit;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMeshOrbit::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
			m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);                        // ���_�����v�Z
			m_nNumPolygon = ((m_nXBlock * m_nZBlock) * 2) + ((m_nZBlock - 1) * 4);   // �`�悷��|���S������ݒ�

			// ���_���W���̃������𓮓I�Ɋm��
			// ���_���W
			if (m_pVertexPos == NULL)
			{// ���������m�ۂł����Ԃł���
				m_pVertexPos = new D3DXVECTOR3[m_nNumVertex];
			}

			// ���_�J���[
			if (m_pVertexCol == NULL)
			{// ���������m�ۂł����Ԃł���
				m_pVertexCol = new D3DXCOLOR[m_nNumVertex];
			}

			// ���_�o�b�t�@�̐���
			MakeVertex(pDevice);
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
//    �I������
//=============================================================================
void CMeshOrbit::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �m�ۂ������_�f�[�^�̃��������J������
	// ���_���W
	if (m_pVertexPos != NULL)
	{// ���������m�ۂł����Ԃł���
		delete m_pVertexPos;
		m_pVertexPos = NULL;
	}

	// ���_�J���[
	if (m_pVertexCol != NULL)
	{// ���������m�ۂł����Ԃł���
		delete m_pVertexCol;
		m_pVertexCol = NULL;
	}

	// ���g�̃|�C���^��j������
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CMeshOrbit::Update(void)
{
	// ���_���W���v�Z����
	if (m_pVertexPos != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntVer = m_nNumVertex - 1; nCntVer > 1; nCntVer--)
		{// ���_���W���J��Ԃ�
			m_pVertexPos[nCntVer] = m_pVertexPos[nCntVer - 2];
		}

		for (int nCntVer = 0; nCntVer < 2; nCntVer++)
		{// ���Ă������_���W�̕������J��Ԃ�
			// ���_���W�����[���h�}�g���b�N�X�Ɋ|�����킹��
			//********************************************************************
			// D3DXVec3TransformCoord
			// ���e    : �x�N�g���ƃ}�g���b�N�X���|�����킹�A�x�N�g�����o�͂���
			// ��1���� : �o�͂����x�N�g��
			// ��2���� : ���̓x�N�g��
			// ��3���� : ���̓}�g���b�N�X
			//********************************************************************
			D3DXVec3TransformCoord(&m_pVertexPos[nCntVer], &m_aOffsetPos[nCntVer], m_pMtxParent);
		}
	}

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
		// ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVer = 0; nCntVer < m_nNumVertex; nCntVer++)
		{// ���_�����J��Ԃ�
			pVtx[nCntVer].pos = m_pVertexPos[nCntVer];
			pVtx[nCntVer].col = m_pVertexCol[nCntVer];
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CMeshOrbit::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			DWORD Lighting;
			DWORD CullMode;

			// ���C�e�B���O�̐ݒ���O��
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

			// �J�����O�̐ݒ���O��
			pDevice->GetRenderState(D3DRS_CULLMODE, &CullMode);
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		    // ���[���h�}�g���b�N�X�̐ݒ菈��
			SetMtxWorld(pDevice);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pTexture);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, m_nNumPolygon);

			// �J�����O�̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_CULLMODE, CullMode);

			// ���C�e�B���O�̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CMeshOrbit::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CMeshOrbit::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVer = 0; nCntVer < m_nNumVertex; nCntVer++)
	{// ���_�����J��Ԃ�
		// ���_���W
		m_pVertexPos[nCntVer] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCntVer].pos = m_pVertexPos[nCntVer];

		// ���_�J���[
		m_pVertexCol[nCntVer] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[nCntVer].col = m_pVertexCol[nCntVer];

		// �e�N�X�`�����W
		pVtx[nCntVer].tex = D3DXVECTOR2(0.0f + (nCntVer / 2) * (1.0f / (float)m_nXBlock), 0.0f + (nCntVer % 2) * 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    �e�N�X�`���ݒ菈��
//=============================================================================
void CMeshOrbit::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ���_�o�b�t�@�ݒ菈��
//=============================================================================
void CMeshOrbit::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    �I�t�Z�b�g�̍��W��ݒ肷�鏈��
//=============================================================================
void CMeshOrbit::SetOffsetPos(const D3DXVECTOR3 OffsetPos, int nIdx)
{
	m_aOffsetPos[nIdx] = OffsetPos;
}

//=============================================================================
//   ���_���W��ݒ肷�鏈��
//=============================================================================
void CMeshOrbit::SetVertexPos(D3DXVECTOR3 pos, int nIdx)
{
	m_pVertexPos[nIdx] = pos;
}

//=============================================================================
//   ���_�J���[��ݒ肷�鏈��
//=============================================================================
void CMeshOrbit::SetVertexCol(D3DXCOLOR col, int nIdx)
{
	m_pVertexCol[nIdx] = col;
}

//=============================================================================
//   ���_���W��ݒ肷�鏈��
//=============================================================================
void CMeshOrbit::SetVertexPos(D3DXVECTOR3 *pPos)
{
	m_pVertexPos = pPos;
}

//=============================================================================
//   ���_�J���[��ݒ肷�鏈��
//=============================================================================
void CMeshOrbit::SetVertexCol(D3DXCOLOR *pCol)
{
	m_pVertexCol = pCol;
}

//=============================================================================
//    ���̕������ݒ菈��
//=============================================================================
void CMeshOrbit::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;
}

//=============================================================================
//    ���s�̕������ݒ菈��
//=============================================================================
void CMeshOrbit::SetZBlock(const int nZBlock)
{
	m_nZBlock = nZBlock;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CMeshOrbit::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    �e�̃}�g���b�N�X�̃|�C���^�ݒ菈��
//=============================================================================
void CMeshOrbit::SetMtxParent(D3DXMATRIX *pMtxParent)
{
	m_pMtxParent = pMtxParent;
}

//=============================================================================
//    ���_�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshOrbit::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    �e�N�X�`���擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshOrbit::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    �I�t�Z�b�g�̋����擾����
//=============================================================================
D3DXVECTOR3 CMeshOrbit::GetOffsetPos(int nIdx)
{
	return m_aOffsetPos[nIdx];
}

//=============================================================================
//    ���̕������擾����
//=============================================================================
int CMeshOrbit::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    ���s�̕������擾����
//=============================================================================
int CMeshOrbit::GetZBlock(void)
{
	return m_nZBlock;
}

//=============================================================================
//    ���_���擾����
//=============================================================================
int CMeshOrbit::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    �|���S�����擾����
//=============================================================================
int CMeshOrbit::GetNumPolygon(void)
{
	return m_nNumPolygon;
}

//=============================================================================
//    ���_���W�擾����
//=============================================================================
D3DXVECTOR3 *CMeshOrbit::GetVertexPos(void)
{
	return m_pVertexPos;
}

//=============================================================================
//    ���_�J���[�擾����
//=============================================================================
D3DXCOLOR *CMeshOrbit::GetVertexCol(void)
{
	return m_pVertexCol;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CMeshOrbit::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    �e�̃}�g���b�N�X�̃|�C���^�擾����
//=============================================================================
D3DXMATRIX *CMeshOrbit::GetMtxParent(void)
{
	return m_pMtxParent;
}