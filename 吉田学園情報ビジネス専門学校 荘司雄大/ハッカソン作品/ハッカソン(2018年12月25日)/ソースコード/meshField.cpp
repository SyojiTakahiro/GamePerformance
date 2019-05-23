//*****************************************************************************
//
//     ���b�V���t�B�[���h�̏���[meshField.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "meshField.h"
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
CMeshField::CMeshField(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pVtxBuff = NULL;                          // ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;                          // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	m_pTexture = NULL;	                        // �e�N�X�`���ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���b�V���t�B�[���h�̍��W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���b�V���t�B�[���h�̌���
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // ���b�V���t�B�[���h�̐F
	m_fWidth = 0.0f;                            // 1�u���b�N���̕�
	m_fHeight = 0.0f;                           // 1�u���b�N������
	m_nXBlock = 0;                              // ���̕�����
	m_nZBlock = 0;                              // ���s�̕�����
	m_nNumVertex = 0;                           // ���_��
	m_nNumIndex = 0;                            // �C���f�b�N�X��
	m_nNumPolygon = 0;                          // �|���S����
	D3DXMatrixIdentity(&m_MtxWorld);            // ���[���h�}�g���b�N�X
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMeshField::~CMeshField()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMeshField *CMeshField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nZBlock, char *pFileName, int nPriority)
{
	CMeshField *pMeshField = NULL;         // ���b�V���t�B�[���h�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pMeshField == NULL)
		{// ����������ɂȂ��Ă���
			pMeshField = new CMeshField(nPriority);
			if (pMeshField != NULL)
			{// �C���X�^���X�𐶐��ł���
			 // �e��l�̑��
				pMeshField->SetPos(pos);                   // ���b�V���t�B�[���h�̍��W
				pMeshField->SetRot(rot);                   // ���b�V���t�B�[���h�̌���
				pMeshField->SetCol(col);                   // ���b�V���t�B�[���h�̐F
				pMeshField->SetWidth(fWidth);              // 1�u���b�N���̕�
				pMeshField->SetHeight(fHeight);            // 1�u���b�N���̍���
				pMeshField->SetXBlock(nXBlock);            // ���̕�����
				pMeshField->SetZBlock(nZBlock);            // ���s�̕�����
				pMeshField->SetBinaryFileName(pFileName);  // ���_��񂪊i�[���ꂽ�o�C�i���t�@�C����

				if (FAILED(pMeshField->Init()))
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

	return pMeshField;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMeshField::Init(void)
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
			m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);                                                               // ���_�����v�Z
			m_nNumIndex = ((m_nXBlock + 1) * (m_nZBlock + 1)) + (2 * (m_nZBlock - 1)) + (m_nXBlock + 1) * (m_nZBlock - 1);  // �C���f�b�N�X����ݒ�
			m_nNumPolygon = ((m_nXBlock * m_nZBlock) * 2) + ((m_nZBlock - 1) * 4);                                          // �`�悷��|���S������ݒ�

																															// ���_�o�b�t�@�̐���
			MakeVertex(pDevice);

			// �C���f�b�N�X�o�b�t�@�̐���
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
//    �I������
//=============================================================================
void CMeshField::Uninit(void)
{
	// ���_���̕ۑ�
	//SaveMesh();

	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �C���f�b�N�X�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pIdxBuff);

	// �����[�X����
	CScene::Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CMeshField::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CMeshField::Draw(void)
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

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

			// �C���f�b�N�X�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
			pDevice->SetIndices(m_pIdxBuff);

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			if (m_pTexture != NULL)
			{// ���������m�ۂ���Ă���
				pDevice->SetTexture(0, m_pTexture);
			}
			else
			{// ���������m�ۂ���Ă��Ȃ�
				pDevice->SetTexture(0, NULL);
			}

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CMeshField::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
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
//    ���_�o�b�t�@��������
//=============================================================================
void CMeshField::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�����t�@�C������ǂݎ��
	LoadMesh();
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@��������
//=============================================================================
void CMeshField::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;       // �C���f�b�N�X�f�[�^�ւ̃|�C���^
	int nCntIdx = 0;  // �C���f�b�N�X�ԍ�

	// �C���f�b�N�X�o�b�t�@�����b�N��,�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < m_nZBlock; nCntIdxY++)
	{// �c�̕��������J��Ԃ�
		for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
		{// ���̕�����+�P�J��Ԃ�
		 // �㉺�̓���C���f�b�N�X�ԍ���ݒ�
			pIdx[0] = nCntIdx + m_nXBlock + 1; // ����
			pIdx[1] = nCntIdx;                 // �㑤

			pIdx += 2;  // 2���i�߂�
			if (nCntIdxY < m_nZBlock - 1 && nCntIdxX == m_nXBlock)
			{// 1 , �������c�̕������̍ŉ��w�ł͂Ȃ�
			 // 2 , ���̕��������ݒ肪�I�����
				pIdx[0] = nCntIdx;                       // �㑤
				pIdx[1] = nCntIdx + (m_nXBlock + 1) + 1; // ���̉���

				pIdx += 2; // 2���i�߂�
			}
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    �@���v�Z����
//=============================================================================
void CMeshField::MakeNormal(void)
{
	VERTEX_3D *pVtx;	                              // ���_�f�[�^�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // ���_�o�b�t�@

	D3DXVECTOR3 VecVer[3];	    // �@���v�Z�p�x�N�g��
	D3DXVECTOR3 *pNor = NULL;   // ���_�̖@��

	// �K�v�Ȑ��������I�Ɋm�ۂ���
	pNor = new D3DXVECTOR3[(m_nXBlock * m_nZBlock) * 2];

	// �O�ϗp�Ƀx�N�g�������
	if (pNor != NULL)
	{	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		int nCntVec1 = 0;                // �ԍ��v�Z�p(����̒��_�ԍ�)
		int nCntVec2 = (m_nXBlock + 1);  // �ԍ��v�Z�p(�����̒��_�ԍ�)
		int nCntVec3 = (m_nXBlock + 2);  // �ԍ��v�Z�p(�E���̒��_�ԍ�)
		int nCntVec4 = 1;                // �ԍ��v�Z�p(�E��̒��_�ԍ�)
		int nCntNor = 0;                 // �ԍ��v�Z�p(�@���̔ԍ�)
		for (int nCntV = 0; nCntV < m_nZBlock; nCntV++)
		{// ���������̕����������J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
			{// ���������̕����������J��Ԃ�
				VecVer[0] = pVtx[nCntVec2].pos - pVtx[nCntVec1].pos;
				VecVer[1] = pVtx[nCntVec3].pos - pVtx[nCntVec1].pos;
				VecVer[2] = pVtx[nCntVec4].pos - pVtx[nCntVec1].pos;

				// �O�ς��g���ăx�N�g���ɂ���
				D3DXVec3Cross(&pNor[nCntNor], &VecVer[1], &VecVer[0]);
				D3DXVec3Cross(&pNor[nCntNor + 1], &VecVer[2], &VecVer[1]);

				// �o�Ă����x�N�g���𐳋K�����Ė@���ɂ���
				D3DXVec3Normalize(&pNor[nCntNor], &pNor[nCntNor]);
				D3DXVec3Normalize(&pNor[nCntNor + 1], &pNor[nCntNor + 1]);

				// �ԍ������炷
				nCntNor += 2;
				nCntVec1++;
				nCntVec2++;
				nCntVec3++;
				nCntVec4++;
			}
			// �E�[�͂����I����Ă���̂ō��[�ɂȂ�悤�ɔԍ������낦��
			nCntVec1++;
			nCntVec2++;
			nCntVec3++;
			nCntVec4++;
		}

		// �@������
		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				if (nCntH == 0)
				{// �E�[�������ꍇ
					if (nCntV == 0)
					{// ��ԍ���̒��_�ԍ��̏ꍇ
						pVtx[nCntH].nor = (pNor[0] + pNor[1]) / 2;  // OK
					}
					else if (nCntV == m_nZBlock)
					{// ��ԍ����̒��_�ԍ��̏ꍇ
						pVtx[nCntH].nor = pNor[((m_nXBlock * 2) * (m_nZBlock - 1))];  // OK
					}
					else
					{// ����ȊO�̒��_�ԍ��̏ꍇ
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2))] + pNor[nCntV * (m_nXBlock * 2)] + pNor[nCntV * (m_nXBlock * 2) + 1]) / 3;   // OK
					}
				}
				else if (nCntH == m_nXBlock)
				{// �E�[�������ꍇ
					if (nCntV == 0)
					{// ��ԉE��̒��_�ԍ��̏ꍇ
						pVtx[nCntH].nor = pNor[(m_nXBlock * 2) - 1];  // OK
					}
					else if (nCntV == m_nZBlock)
					{// ��ԉE���̒��_�ԍ��̏ꍇ
						pVtx[nCntH].nor = (pNor[((m_nXBlock * 2) * m_nZBlock) - 1] + pNor[((m_nXBlock * 2) * m_nZBlock) - 2]) / 2;  // OK
					}
					else
					{// ����ȊO�̒��_�ԍ��̏ꍇ
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2)] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2) + 1] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((m_nXBlock * 2) - 2) + 2]) / 3;  // OK
					}
				}
				else
				{// �^�񒆂̏ꍇ
					if (nCntV == 0)
					{// ��ԏ�̒��_�ԍ��̏ꍇ
						pVtx[nCntH].nor = (pNor[(nCntH * 2) - 1] + pNor[(nCntH * 2)] + pNor[(nCntH * 2) + 1]) / 3;   // OK
					}
					else if (nCntV == m_nZBlock)
					{// ��ԉ��̒��_�ԍ��̏ꍇ
						pVtx[nCntH].nor = (pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nZBlock - 1))] + pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nZBlock - 1)) + 1] + pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nZBlock - 1)) + 2]) / 3; // OK
					}
					else
					{// ����ȊO�̒��_�ԍ��̏ꍇ
						pVtx[nCntH].nor = (pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2)] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2) + 1] + pNor[((nCntV - 1) * (m_nXBlock * 2)) + ((nCntH - 1) * 2) + 2] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2)] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2) + 1] + pNor[(nCntV * (m_nXBlock * 2) + 1) + ((nCntH - 1) * 2) + 2]) / 6;  // OK
					}
				}
			}
			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		if (pNor != NULL)
		{// ���������m�ۂ���Ă���
			delete[] pNor;
			pNor = NULL;
		}

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    ���_��񂪊i�[���ꂽ�o�C�i���t�@�C�����ݒ菈��
//=============================================================================
void CMeshField::SetBinaryFileName(char *pFileName)
{
	strcpy(m_aBinaryFileName, pFileName);
}

//=============================================================================
//    �e�N�X�`���ݒ菈��
//=============================================================================
void CMeshField::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ���_�o�b�t�@�ݒ菈��
//=============================================================================
void CMeshField::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�ݒ菈��
//=============================================================================
void CMeshField::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    ���b�V���t�B�[���h�̍��W�ݒ菈��
//=============================================================================
void CMeshField::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ���b�V���t�B�[���h�̌����ݒ菈��
//=============================================================================
void CMeshField::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ���b�V���t�B�[���h�̐F�ݒ菈��
//=============================================================================
void CMeshField::SetCol(const D3DXCOLOR col)
{
	// �F��ݒ�
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
			    // ���_�J���[
				pVtx[nCntH].col = m_Col;
			}
			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1�u���b�N���̕��ݒ菈��
//=============================================================================
void CMeshField::SetWidth(const float fWidth)
{
	// ����ݒ�
	m_fWidth = fWidth;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	 // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float XPos = -(m_fWidth * m_nXBlock) / 2; // X���W�����ɐݒ�

		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].pos.x = XPos;

				XPos += m_fWidth;   // X���W�����ɐi�߂�
			}
			XPos -= m_fWidth * (m_nXBlock + 1); // X���W��i�߂Ă������߂�

			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    1�u���b�N���̍����ݒ菈��
//=============================================================================
void CMeshField::SetHeight(const float fHeight)
{
	// ������ݒ�
	m_fHeight = fHeight;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float ZPos = (m_fHeight * m_nZBlock) / 2; // Z���W�����ɐݒ�

		for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				pVtx[nCntH].pos.z = ZPos;
			}
			ZPos -= m_fHeight;   // Z���W����O�ɐi�߂�

			// �|�C���^��i�߂�
			pVtx += m_nXBlock + 1;
		}

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ���̕������ݒ菈��
//=============================================================================
void CMeshField::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;

	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
			m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);                                                               // ���_�����v�Z
			m_nNumIndex = ((m_nXBlock + 1) * (m_nZBlock + 1)) + (2 * (m_nZBlock - 1)) + (m_nXBlock + 1) * (m_nZBlock - 1);  // �C���f�b�N�X����ݒ�
			m_nNumPolygon = ((m_nXBlock * m_nZBlock) * 2) + ((m_nZBlock - 1) * 4);                                          // �`�悷��|���S������ݒ�

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

			// �����ɕK�v�ȕϐ���錾
			D3DXCOLOR col = GetCol();
			float XPos = -(m_fWidth * m_nXBlock) / 2; // X���W�����ɐݒ�
			float ZPos = (m_fHeight * m_nZBlock) / 2; // Z���W�����ɐݒ�
			float Utex = 0.0f;                        // �e�N�X�`����U���W���E��ɐݒ�
			float Vtex = 0.0f;                        // �e�N�X�`����V���W���E��ɐݒ�
			float YPos = 0.0f;                        // Y���W(���̓����_���Ōv�Z���܂�)

			for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
			{// ���������̕����� + 1�����J��Ԃ�
				for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
				{// ���������̕����� + 1�����J��Ԃ�
				 // ���_���W
					pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, ZPos);

					// ���_�J���[
					pVtx[nCntH].col = col;

					// �e�N�X�`�����W
					pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

					XPos += m_fWidth;             // X���W�����ɐi�߂�
					Utex += 1.0f;                 // �e�N�X�`����U���W��i�߂�
				}
				XPos -= m_fWidth * (m_nXBlock + 1); // X���W��i�߂Ă������߂�
				ZPos -= m_fHeight;                  // Z���W����O�ɐi�߂�
				Utex -= 1.0f * (m_nXBlock + 1);     // �e�N�X�`����U���W��i�߂Ă�������߂�
				Vtex += 1.0f;                       // �e�N�X�`����V���W��1���i�߂�

				// �|�C���^��i�߂�
				pVtx += m_nXBlock + 1;
			}

			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();

			// �@���v�Z����
			MakeNormal();

			// �C���f�b�N�X�o�b�t�@�̐���
			MakeIndex(pDevice);
		}
	}
}

//=============================================================================
//    ���s�̕������ݒ菈��
//=============================================================================
void CMeshField::SetZBlock(const int nZBlock)
{
	m_nZBlock = nZBlock;

	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
			m_nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);                                                               // ���_�����v�Z
			m_nNumIndex = ((m_nXBlock + 1) * (m_nZBlock + 1)) + (2 * (m_nZBlock - 1)) + (m_nXBlock + 1) * (m_nZBlock - 1);  // �C���f�b�N�X����ݒ�
			m_nNumPolygon = ((m_nXBlock * m_nZBlock) * 2) + ((m_nZBlock - 1) * 4);                                          // �`�悷��|���S������ݒ�

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

			// �����ɕK�v�ȕϐ���錾
			D3DXCOLOR col = GetCol();
			float XPos = -(m_fWidth * m_nXBlock) / 2; // X���W�����ɐݒ�
			float ZPos = (m_fHeight * m_nZBlock) / 2; // Z���W�����ɐݒ�
			float Utex = 0.0f;                        // �e�N�X�`����U���W���E��ɐݒ�
			float Vtex = 0.0f;                        // �e�N�X�`����V���W���E��ɐݒ�
			float YPos = 0.0f;                        // Y���W(���̓����_���Ōv�Z���܂�)

			for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
			{// ���������̕����� + 1�����J��Ԃ�
				for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
				{// ���������̕����� + 1�����J��Ԃ�
				 // ���_���W
					pVtx[nCntH].pos = D3DXVECTOR3(XPos, YPos, ZPos);

					// ���_�J���[
					pVtx[nCntH].col = col;

					// �e�N�X�`�����W
					pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

					XPos += m_fWidth;             // X���W�����ɐi�߂�
					Utex += 1.0f;                 // �e�N�X�`����U���W��i�߂�
				}
				XPos -= m_fWidth * (m_nXBlock + 1); // X���W��i�߂Ă������߂�
				ZPos -= m_fHeight;                  // Z���W����O�ɐi�߂�
				Utex -= 1.0f * (m_nXBlock + 1);     // �e�N�X�`����U���W��i�߂Ă�������߂�
				Vtex += 1.0f;                       // �e�N�X�`����V���W��1���i�߂�

				// �|�C���^��i�߂�
				pVtx += m_nXBlock + 1;
			}

			// ���_�o�b�t�@���A�����b�N����
			m_pVtxBuff->Unlock();

			// �@���v�Z����
			MakeNormal();

			// �C���f�b�N�X�o�b�t�@�̐���
			MakeIndex(pDevice);
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CMeshField::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    ���_��񂪊i�[���ꂽ�o�C�i���t�@�C�����擾����
//=============================================================================
char *CMeshField::GetBinaryFileName(void)
{
	return m_aBinaryFileName;
}

//=============================================================================
//    ���_�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshField::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CMeshField::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    �e�N�X�`���擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshField::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ���b�V���t�B�[���h�̍��W�擾����
//=============================================================================
D3DXVECTOR3 CMeshField::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ���b�V���t�B�[���h�̌����擾����
//=============================================================================
D3DXVECTOR3 CMeshField::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ���b�V���t�B�[���h�̐F�擾����
//=============================================================================
D3DXCOLOR CMeshField::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    1�u���b�N���̕��擾����
//=============================================================================
float CMeshField::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    1�u���b�N���̍����擾����
//=============================================================================
float CMeshField::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    ���̕������擾����
//=============================================================================
int CMeshField::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    ���s�̕������擾����
//=============================================================================
int CMeshField::GetZBlock(void)
{
	return m_nZBlock;
}

//=============================================================================
//    ���_���擾����
//=============================================================================
int CMeshField::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    �C���f�b�N�X���擾����
//=============================================================================
int CMeshField::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    �|���S�����擾����
//=============================================================================
int CMeshField::GetNumPolygon(void)
{
	return m_nNumPolygon;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CMeshField::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    ���b�V���t�B�[���h�̃��[���h���W�ɂ����鍂���擾���鏈��
//=============================================================================
float CMeshField::GetPolyHeight(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;               // ����(�ŏI�I�ɂ��̒l��Ԃ�)
	float fRivision = 0.0f;             // �����̕␳�l(�@��������o���̂�0.0�̍����ɂ��낦�邽��)
	int nVertexIdxStart = 0;            // ����̒��_�ԍ�(���[�v���邽�тɂ���Ă���)
	D3DXMATRIX mtxRot, mtxTrans;         // �v�Z�p���[���h�}�g���b�N�X
	D3DXMATRIX VertexMtxWorld[4];       // ���_�̃��[���h�}�g���b�N�X4��(��],�ʒu���܂߂Čv�Z����)
	D3DXVECTOR3 VertexPos[4];           // ���_�̍��W4��(���̍��W�̓��[�J�����W�Ȃ̂Ōv�Z�ɂ͎g��Ȃ�)
	D3DXVECTOR3 VertexWorldPos[4];      // ���_�̃��[���h���W4��(���[���h�}�g���b�N�X���甲���o��)
	D3DXVECTOR3 VertexLeftWorldPos[3];  // �����̃|���S���̒��_�̃��[���h���W3��(���[���h�}�g���b�N�X���甲���o��)
	D3DXVECTOR3 VertexRightWorldPos[3]; // �E���̃|���S���̒��_�̃��[���h���W3��(���[���h�}�g���b�N�X���甲���o��)
	D3DXVECTOR3 PolygonVerPos[3];       // �|���S���̍��W(����p�̖@���𓱂����߂ɕK�v)
	D3DXVECTOR3 PolygonNormal;          // �|���S���̖@��(����p�̖@���Ȃ̂Ō����ڂ̖@���Ƃ͈قȂ�)
	D3DXVECTOR3 PolygonVector[2];       // �|���S���̖@��������o�����߂̃x�N�g��
	D3DXVECTOR3 VecPosToPolygon;        // �����̍��W�ƃ|���S���Ƃ̃x�N�g��
	D3DXVECTOR3 VectorVerToVer;         // �O�σx�N�g��1�{��(���_���璸�_�ւ̃x�N�g��)
	D3DXVECTOR3 VectorVerToPos;         // �O�σx�N�g��2�{��(���_��������̍��W�ւ̃x�N�g��)
	int nCntLeftPolygon = 0;            // �����̃|���S���̃x�N�g�������ʂ�����
	int nCntRightPolygon = 0;           // �E���̃|���S���̃x�N�g�������ʂ�����

										// ���_�f�[�^�ւ̃|�C���^��錾
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = 0; nCntV < m_nZBlock; nCntV++)
	{// ���s�̕��������J��Ԃ�
		for (int nCntH = 0; nCntH < m_nXBlock; nCntH++)
		{// ���̕��������J��Ԃ�
		 // ���_�̃��[�J�����W�𔲂��o��
			VertexPos[0] = pVtx[nVertexIdxStart].pos;
			VertexPos[1] = pVtx[nVertexIdxStart + 1].pos;
			VertexPos[2] = pVtx[nVertexIdxStart + (m_nXBlock + 1)].pos;
			VertexPos[3] = pVtx[nVertexIdxStart + (m_nXBlock + 1) + 1].pos;

			// ���_�̃��[���h�}�g���b�N�X�����o��
			for (int nCntVer = 0; nCntVer < 4; nCntVer++)
			{// 1�u���b�N���̒��_�����J��Ԃ�
			 //   // ���[���h�}�g���b�N�X�̏�����
				//D3DXMatrixIdentity(&VertexMtxWorld[nCntVer]);

				//// ��]�𔽉f
				//D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);
				//D3DXMatrixMultiply(&VertexMtxWorld[nCntVer], &VertexMtxWorld[nCntVer], &mtxRot);

				//// �ʒu�𔽉f
				//D3DXMatrixTranslation(&mtxTrans, VertexPos[nCntVer].x, VertexPos[nCntVer].y, VertexPos[nCntVer].z);
				//D3DXMatrixMultiply(&VertexMtxWorld[nCntVer], &VertexMtxWorld[nCntVer], &mtxTrans);

				//// �e�̏����|�����킹��
				//D3DXMatrixMultiply(&VertexMtxWorld[nCntVer], &VertexMtxWorld[nCntVer], &GetMtxWorld());

				// ���_�̃��[���h���W�𔲂��o��
				D3DXVec3TransformCoord(&VertexWorldPos[nCntVer], &VertexPos[nCntVer], &GetMtxWorld());
			}

			// ���_�̃��[���h���W�𔲂��o��
			//VertexWorldPos[0] = D3DXVECTOR3(VertexMtxWorld[0]._41, VertexMtxWorld[0]._42, VertexMtxWorld[0]._43);
			//VertexWorldPos[1] = D3DXVECTOR3(VertexMtxWorld[1]._41, VertexMtxWorld[1]._42, VertexMtxWorld[1]._43);
			//VertexWorldPos[2] = D3DXVECTOR3(VertexMtxWorld[2]._41, VertexMtxWorld[2]._42, VertexMtxWorld[2]._43);
			//VertexWorldPos[3] = D3DXVECTOR3(VertexMtxWorld[3]._41, VertexMtxWorld[3]._42, VertexMtxWorld[3]._43);

			// �����̃|���S���̔���J�n
			// �����̃|���S���̍��W��ݒ�
			VertexLeftWorldPos[0] = VertexWorldPos[0];
			VertexLeftWorldPos[1] = VertexWorldPos[3];
			VertexLeftWorldPos[2] = VertexWorldPos[2];
			for (int nCntLeft = 0; nCntLeft < 3; nCntLeft++)
			{// �|���S���̒��_�����J��Ԃ�
			    // �O�ς̃x�N�g�������o��
				VectorVerToVer = VertexLeftWorldPos[(nCntLeft + 1) % 3] - VertexLeftWorldPos[nCntLeft];
				VectorVerToPos = pos - VertexLeftWorldPos[nCntLeft];

				if ((VectorVerToVer.x * VectorVerToPos.z) - (VectorVerToVer.z * VectorVerToPos.x) <= 0)
				{// �����̍��W�����_�ƒ��_�̃x�N�g���̉E���ɂ���
					nCntLeftPolygon++;  // �����ʂ����񐔂�i�߂�
				}
			}
			if (nCntLeftPolygon == 3)
			{// �O�ς̔����S�Ēʂ���
			    // ����p�̒��_���W�����
				PolygonVerPos[0] = D3DXVECTOR3(VertexLeftWorldPos[2].x, VertexLeftWorldPos[2].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[2].z);
				PolygonVerPos[1] = D3DXVECTOR3(VertexLeftWorldPos[1].x, VertexLeftWorldPos[1].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[1].z);
				PolygonVerPos[2] = D3DXVECTOR3(VertexLeftWorldPos[0].x, VertexLeftWorldPos[0].y - VertexLeftWorldPos[2].y, VertexLeftWorldPos[0].z);

				// �O�σx�N�g�������
				PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
				PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

				// �O�σx�N�g������@��������o��
				D3DXVec3Cross(&PolygonNormal, &PolygonVector[0], &PolygonVector[1]);
				D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);

				// ����̎d����0�ɖ߂��������Ōv�Z���Ă���̂ŕ␳�����邽�߂ɒl��ێ�
				fRivision = VertexLeftWorldPos[2].y;

				// �����̍��W�ƒ��_���W�̃x�N�g�����v�Z
				VecPosToPolygon = pos - PolygonVerPos[0];

				// ���ς̊T�O�𗘗p���č���������o��
				float DotX = (PolygonNormal.x * VecPosToPolygon.x);     // X�����̓��ς��v�Z
				float DotZ = (PolygonNormal.z * VecPosToPolygon.z);     // Z�����̓��ς��v�Z
				float Dot = -DotX - DotZ;                               // X������Z�����̓��ϓ��m������
				fHeight = (Dot / PolygonNormal.y) + fRivision;          // ���������̂�Y�����̖@���Ŋ����ĕ␳�l�𑫂��Ă�����

																		// ��������p�x�����߂�
				float VecA = sqrtf((VecPosToPolygon.x * VecPosToPolygon.x) + (fHeight * fHeight) + (VecPosToPolygon.z * VecPosToPolygon.z));
				float VecB = sqrtf((PolygonNormal.x * PolygonNormal.x) + (PolygonNormal.y * PolygonNormal.y) + (PolygonNormal.z * PolygonNormal.z));
				float DotY = (PolygonNormal.y * fHeight);
				float cosSita = (DotX + DotY + DotZ) / (VecA * VecB);
				float fAngle = acosf(cosSita);
			}

			// �E���̃|���S���̔���J�n
			// �E���̃|���S���̍��W��ݒ�
			VertexRightWorldPos[0] = VertexWorldPos[0];
			VertexRightWorldPos[1] = VertexWorldPos[1];
			VertexRightWorldPos[2] = VertexWorldPos[3];
			for (int nCntLeft = 0; nCntLeft < 3; nCntLeft++)
			{// �|���S���̒��_�����J��Ԃ�
			    // �O�ς̃x�N�g�������o��
				VectorVerToVer = VertexRightWorldPos[(nCntLeft + 1) % 3] - VertexRightWorldPos[nCntLeft];
				VectorVerToPos = pos - VertexRightWorldPos[nCntLeft];

				if ((VectorVerToVer.x * VectorVerToPos.z) - (VectorVerToVer.z * VectorVerToPos.x) <= 0)
				{// �����̍��W�����_�ƒ��_�̃x�N�g���̉E���ɂ���
					nCntRightPolygon++;  // �����ʂ����񐔂�i�߂�
				}
			}
			if (nCntRightPolygon == 3)
			{// �O�ς̔����S�Ēʂ���
			    // ����p�̒��_���W�����
				PolygonVerPos[0] = D3DXVECTOR3(VertexRightWorldPos[1].x, VertexRightWorldPos[1].y - VertexRightWorldPos[1].y, VertexRightWorldPos[1].z);
				PolygonVerPos[1] = D3DXVECTOR3(VertexRightWorldPos[0].x, VertexRightWorldPos[0].y - VertexRightWorldPos[1].y, VertexRightWorldPos[0].z);
				PolygonVerPos[2] = D3DXVECTOR3(VertexRightWorldPos[2].x, VertexRightWorldPos[2].y - VertexRightWorldPos[1].y, VertexRightWorldPos[2].z);

				// �O�σx�N�g�������
				PolygonVector[0] = PolygonVerPos[1] - PolygonVerPos[0];
				PolygonVector[1] = PolygonVerPos[2] - PolygonVerPos[0];

				// �O�σx�N�g������@��������o��
				D3DXVec3Cross(&PolygonNormal, &PolygonVector[1], &PolygonVector[0]);
				D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);

				// ����̎d����0�ɖ߂��������Ōv�Z���Ă���̂ŕ␳�����邽�߂ɒl��ێ�
				fRivision = VertexRightWorldPos[1].y;

				// �����̍��W�ƒ��_���W�̃x�N�g�����v�Z
				VecPosToPolygon = pos - PolygonVerPos[0];

				// ���ς̊T�O�𗘗p���č���������o��
				float DotX = (PolygonNormal.x * VecPosToPolygon.x);     // X�����̓��ς��v�Z
				float DotZ = (PolygonNormal.z * VecPosToPolygon.z);     // Z�����̓��ς��v�Z
				float Dot = -DotX - DotZ;                               // X������Z�����̓��ϓ��m������
				fHeight = (Dot / PolygonNormal.y) + fRivision;          // ���������̂�Y�����̖@���Ŋ����ĕ␳�l�𑫂��Ă�����

																		// ��������p�x�����߂�
				float VecA = sqrtf((VecPosToPolygon.x * VecPosToPolygon.x) + (fHeight * fHeight) + (VecPosToPolygon.z * VecPosToPolygon.z));
				float VecB = sqrtf((PolygonNormal.x * PolygonNormal.x) + (PolygonNormal.y * PolygonNormal.y) + (PolygonNormal.z * PolygonNormal.z));
				float DotY = (PolygonNormal.y * fHeight);
				float cosSita = (DotX + DotY + DotZ) / (VecA * VecB);
				float fAngle = acosf(cosSita);
			}

			// ����p�̕ϐ������������Ă���
			if (nCntLeftPolygon == 3 || nCntRightPolygon == 3)
			{// �|���S���ɏ���Ă���Ɣ��肳��Ă���
				break;
			}
			else
			{// �܂��|���S���ɏ���Ă��Ȃ��Ɣ��肳��Ă���
				nCntLeftPolygon = 0;
				nCntRightPolygon = 0;
				nVertexIdxStart++;
			}
		}
		if (nCntLeftPolygon == 3 || nCntRightPolygon == 3)
		{// �|���S���ɏ���Ă���Ɣ��肳��Ă���
			break;
		}
		else
		{// �܂��|���S���ɏ���Ă��Ȃ��Ɣ��肳��Ă���
		    // �E�[�̃u���b�N�ɔ��肷��ׂ��|���S���͑��݂��Ȃ��̂Ŕԍ������炵�Ă���
			nVertexIdxStart++;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	return fHeight;
}

//=============================================================================
//    �n�ʂ̖ʂ̍����ݒ菈��
//=============================================================================
void CMeshField::SetPolyHeight(const D3DXVECTOR3 pos, const float fValue, const float fRange)
{
	// ���_���̐ݒ�
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	VERTEX_3D *pVtx;
	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
	{// ���������̕����� + 1�����J��Ԃ�
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// ���������̕����� + 1�����J��Ԃ�
			D3DXVECTOR3 WorldVerPos = pVtx[nCntH].pos;
			D3DXVec3TransformCoord(&WorldVerPos, &WorldVerPos, &m_MtxWorld);

			// �����̍��W���璸�_�܂ł̋������o��
			float fLength = sqrtf((WorldVerPos.x - pos.x) * (WorldVerPos.x - pos.x) + (WorldVerPos.z - pos.z) * (WorldVerPos.z - pos.z));

			if (fLength < fRange)
			{// ���������a�͈͓̔��ł���
			    // ���������a���̂ǂꂭ�炢�̔䗦�ɓ����邩���v�Z
				float fRivision = fLength / fRange;

				// �R�T�C���J�[�u�Ɣ䗦���g���Ăǂꂭ�炢�������邩���v�Z
				float fHeight = cosf((D3DX_PI * 0.5f) * fRivision) * fValue;
				pVtx[nCntH].pos.y += fHeight;
			}
		}
		pVtx += m_nXBlock + 1;
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);

	// �@���v�Z����
	MakeNormal();
}

//=============================================================================
//    �n�ʂ̒��_���O���t�@�C���ۑ�����
//=============================================================================
void CMeshField::SaveMesh(void)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(m_aBinaryFileName, "wb");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // ���_���̎擾
			VERTEX_3D *pVtx;
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // ���_�o�b�t�@�����p

			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// �o�C�i���t�@�C���Ƀt�B�[���h�̏�����������
			fwrite(pVtx, sizeof(VERTEX_3D), GetNumVertex(), pFile);

			// �t�@�C������Ă���
			fclose(pFile);

			// ���_�o�b�t�@���A�����b�N����
			pVtxBuff->Unlock();
		}
	}
}

//=============================================================================
//    �n�ʂ̒n�ʂ̒��_���O���t�@�C���ǂݍ��ݏ���
//=============================================================================
void CMeshField::LoadMesh(void)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(m_aBinaryFileName, "rb");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // ���_���̎擾
			VERTEX_3D *pVtx;
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // ���_�o�b�t�@�����p

			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// �o�C�i���t�@�C������t�B�[���h�̏���ǂݍ���
			fread(pVtx, sizeof(VERTEX_3D), GetNumVertex(), pFile);

			// ���_�o�b�t�@���A�����b�N����
			pVtxBuff->Unlock();

			// �t�@�C������Ă���
			fclose(pFile);

			// �@���v�Z����
			MakeNormal();
		}
	}
}