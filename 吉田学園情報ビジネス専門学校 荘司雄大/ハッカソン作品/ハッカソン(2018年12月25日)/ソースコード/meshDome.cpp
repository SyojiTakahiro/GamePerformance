//*****************************************************************************
//
//     �����̏���[meshDome.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "meshDome.h"
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
CMeshDome::CMeshDome(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pVtxBuff = NULL;                          // ���_�o�b�t�@�ւ̃|�C���^
	m_pIdxBuff = NULL;                          // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	m_pTexture = NULL;	                        // �e�N�X�`���ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���b�V���h�[���̍��W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���b�V���h�[���̌���
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // ���b�V���h�[���̐F
	m_fRadius = 0.0f;                           // ���b�V���h�[���̔��a
	m_nXBlock = 0;                              // ���̕�����
	m_nYBlock = 0;                              // ���s�̕�����
	m_nNumVertex = 0;                           // ���_��
	m_nNumIndex = 0;                            // �C���f�b�N�X��
	m_nNumPolygon = 0;                          // �|���S����
	D3DXMatrixIdentity(&m_MtxWorld);            // ���[���h�}�g���b�N�X
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMeshDome::~CMeshDome()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMeshDome *CMeshDome::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority)
{
	CMeshDome *pMeshDome = NULL;           // ���b�V���h�[���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pMeshDome == NULL)
		{// ����������ɂȂ��Ă���
			pMeshDome = new CMeshDome(nPriority);
			if (pMeshDome != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pMeshDome->SetPos(pos);         // ���b�V���h�[���̍��W
				pMeshDome->SetRot(rot);         // ���b�V���h�[���̌���
				pMeshDome->SetCol(col);         // ���b�V���h�[���̐F
				pMeshDome->SetRadius(fRadius);  // ���b�V���h�[���̔��a
				pMeshDome->SetXBlock(nXBlock);  // ���̕�����
				pMeshDome->SetYBlock(nYBlock);  // �c�̕�����

				if (FAILED(pMeshDome->Init()))
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

	return pMeshDome;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMeshDome::Init(void)
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
			m_nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // ���_�����v�Z
			m_nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // �C���f�b�N�X����ݒ�
			m_nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // �`�悷��|���S������ݒ�

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
void CMeshDome::Uninit(void)
{
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
void CMeshDome::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CMeshDome::Draw(void)
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
			pDevice->SetTexture(0, m_pTexture);

			// �|���S���̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nNumVertex, 0, m_nNumPolygon);

			// �W�̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (m_nXBlock + 1), GetNumIndex(), (m_nXBlock));
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CMeshDome::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
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
void CMeshDome::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���(�W�̒��_�����l������)
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (m_nNumVertex + (m_nXBlock + 2)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	float fAngle = 0;   // �p�x
	float fRadius = 0;  // ���a
	float fDome = 0.0f; // �h�[���v�Z�p(���a���k�߂�{��)
	D3DXVECTOR3 pos;    // ���W
	float Utex;         // �e�N�X�`��U���W
	float Vtex;         // �e�N�X�`��V���W

	fAngle = 0.0f;                       // �p�x��0��
	fRadius = cosf(fDome) * m_fRadius;   // ���a��ݒ�
	fDome = 0.0f;                        // �{����ݒ�
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ���W��ݒ�
	Utex = 0.0f;                         // �e�N�X�`��U���W��ݒ�
	Vtex = -1.0f;                        // �e�N�X�`��V���W��ݒ�

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// ���������̕����������J��Ԃ�
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// ���������̕����������J��Ԃ�
			pos.x = sinf(fAngle) * fRadius;
			pos.y = sinf(fDome) * m_fRadius;
			pos.z = cosf(fAngle) * fRadius;

			// ���_���W
			pVtx[nCntH].pos = pos;

			// �@���x�N�g��
			pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

			// ���_�J���[
			pVtx[nCntH].col = GetCol();

			// �e�N�X�`�����W
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

			fAngle -= D3DX_PI / (m_nXBlock / 2);  // �p�x��i�߂�
			Utex += 1.0f / (m_nXBlock / 2);       // �e�N�X�`��U���W��i�߂�

			if (fAngle <= -D3DX_PI)
			{// �{�����~�����𒴂���
				fAngle += D3DX_PI * 2.0f;
			}
		}
		fDome += (D3DX_PI * 0.5f) / (m_nYBlock + 1); // ���a���k�߂�{�����グ��
		fRadius = cosf(fDome) * m_fRadius;           // ���a���R�T�C���J�[�u�ŏk�߂Ă���
		fAngle = 0.0f;                               // �p�x��0��
		Utex = 0.0f;                                 // �e�N�X�`��U���W��ݒ�
		Vtex -= 1.0f / (m_nYBlock);                  // �e�N�X�`��V���W��i�߂�
		pVtx += m_nXBlock + 1;                       // ���_�̐擪�̔ԍ���i�߂�
	}

	// �W�p�̒��_�����쐬
	fDome = D3DX_PI * 0.5f;   // ���a���k�߂�{�����グ��
	// ���_���W
	pos = D3DXVECTOR3(0.0f, sinf(fDome) * m_fRadius, 0.0f);
	pVtx[0].pos = pos;

	// �@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -2.2f);

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, -0.99f);
	pVtx++;

	fDome -= (D3DX_PI * 0.5f) / (m_nYBlock + 1);  // ���a���k�߂�{����߂��Ă���
	fRadius = cosf(fDome) * m_fRadius;            // ���a�v�Z��߂��Ă���
	for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
	{// ���������̕����������J��Ԃ�
		pos.x = sinf(fAngle) * fRadius;
		pos.y = sinf(fDome) * m_fRadius;
		pos.z = cosf(fAngle) * fRadius;

		// ���_���W
		pVtx[nCntH].pos = pos;

		// �@���x�N�g��
		pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -2.2f);

		// ���_�J���[
		pVtx[nCntH].col = m_Col;

		// �e�N�X�`�����W
		pVtx[nCntH].tex = D3DXVECTOR2(0.0f, -0.98f);

		fAngle += -D3DX_PI / (m_nXBlock / 2);  // �p�x��i�߂�
		Utex -= 1.0f / (m_nXBlock / 2);        // �e�N�X�`��U���W��i�߂�

		if (fAngle <= -D3DX_PI)
		{// �{�����~�����𒴂���
			fAngle += D3DX_PI * 2.0f;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �@���v�Z����
	MakeNormal();
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@��������
//=============================================================================
void CMeshDome::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * (m_nNumIndex + (m_nXBlock + 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx;       // �C���f�b�N�X�f�[�^�ւ̃|�C���^
	int nCntIdx = 0;  // �C���f�b�N�X�ԍ�

	// �C���f�b�N�X�o�b�t�@�����b�N��,�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < m_nYBlock; nCntIdxY++)
	{// �c�̕��������J��Ԃ�
		for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 1; nCntIdxX++, nCntIdx++)
		{// ���̕�����+�P�J��Ԃ�
		 // �㉺�̓���C���f�b�N�X�ԍ���ݒ�
			pIdx[0] = nCntIdx + m_nXBlock + 1; // ����
			pIdx[1] = nCntIdx;                 // �㑤

			pIdx += 2;  // 2���i�߂�
			if (nCntIdxY < m_nYBlock - 1 && nCntIdxX == m_nXBlock)
			{// 1 , �������c�̕������̍ŉ��w�ł͂Ȃ�
			 // 2 , ���̕��������ݒ肪�I�����
				pIdx[0] = nCntIdx;                       // �㑤
				pIdx[1] = nCntIdx + (m_nXBlock + 1) + 1; // ���̉���

				pIdx += 2; // 2���i�߂�
			}
		}
	}

	// �W�̃C���f�b�N�X��ݒ�
	nCntIdx += m_nXBlock + 1;
	// �W�̐擪�ԍ���ݒ�
	for (int nCntIdxX = 0; nCntIdxX < m_nXBlock + 2; nCntIdxX++, nCntIdx++)
	{// ���̕�����+2�J��Ԃ�
		pIdx[0] = nCntIdx;
		pIdx++;  // 1���i�߂�
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}

//=============================================================================
//    �@���v�Z����
//=============================================================================
void CMeshDome::MakeNormal(void)
{
	VERTEX_3D *pVtx;	                              // ���_�f�[�^�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // ���_�o�b�t�@

	D3DXVECTOR3 VecVer[3];	    // �@���v�Z�p�x�N�g��
	D3DXVECTOR3 *pNor = NULL;   // ���_�̖@��

	// �K�v�Ȑ��������I�Ɋm�ۂ���
	pNor = new D3DXVECTOR3[(m_nXBlock * m_nYBlock) * 2];

	// �O�ϗp�Ƀx�N�g�������
	if (pNor != NULL)
	{	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		int nCntVec1 = 0;                // �ԍ��v�Z�p(����̒��_�ԍ�)
		int nCntVec2 = (m_nXBlock + 1);  // �ԍ��v�Z�p(�����̒��_�ԍ�)
		int nCntVec3 = (m_nXBlock + 2);  // �ԍ��v�Z�p(�E���̒��_�ԍ�)
		int nCntVec4 = 1;                // �ԍ��v�Z�p(�E��̒��_�ԍ�)
		int nCntNor = 0;                 // �ԍ��v�Z�p(�@���̔ԍ�)
		for (int nCntV = 0; nCntV < m_nYBlock; nCntV++)
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
		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
		{// ���������̕����� + 1�����J��Ԃ�
			for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
			{// ���������̕����� + 1�����J��Ԃ�
				if (nCntH == 0)
				{// �E�[�������ꍇ
					if (nCntV == 0)
					{// ��ԍ���̒��_�ԍ��̏ꍇ
						pVtx[nCntH].nor = (pNor[0] + pNor[1]) / 2;  // OK
					}
					else if (nCntV == m_nYBlock)
					{// ��ԍ����̒��_�ԍ��̏ꍇ
						pVtx[nCntH].nor = pNor[((m_nXBlock * 2) * (m_nYBlock - 1))];  // OK
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
					else if (nCntV == m_nYBlock)
					{// ��ԉE���̒��_�ԍ��̏ꍇ
						pVtx[nCntH].nor = (pNor[((m_nXBlock * 2) * m_nYBlock) - 1] + pNor[((m_nXBlock * 2) * m_nYBlock) - 2]) / 2;  // OK
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
					else if (nCntV == m_nYBlock)
					{// ��ԉ��̒��_�ԍ��̏ꍇ
						pVtx[nCntH].nor = (pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nYBlock - 1))] + pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nYBlock - 1)) + 1] + pNor[((nCntH - 1) * 2) + ((m_nXBlock * 2) * (m_nYBlock - 1)) + 2]) / 3; // OK
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
//    �e�N�X�`���ݒ菈��
//=============================================================================
void CMeshDome::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ���_�o�b�t�@�ݒ菈��
//=============================================================================
void CMeshDome::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�ݒ菈��
//=============================================================================
void CMeshDome::SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff)
{
	m_pIdxBuff = pIdxBuff;
}

//=============================================================================
//    ���b�V���h�[���̍��W�ݒ菈��
//=============================================================================
void CMeshDome::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ���b�V���h�[���̌����ݒ菈��
//=============================================================================
void CMeshDome::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ���b�V���h�[���̐F�ݒ菈��
//=============================================================================
void CMeshDome::SetCol(const D3DXCOLOR col)
{
	// �F��ݒ�
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
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
//    ���b�V���h�[���̔��a�ݒ菈��
//=============================================================================
void CMeshDome::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//=============================================================================
//    ���̕������ݒ菈��
//=============================================================================
void CMeshDome::SetXBlock(const int nXBlock)
{
	m_nXBlock = nXBlock;
}

//=============================================================================
//    ���s�̕������ݒ菈��
//=============================================================================
void CMeshDome::SetYBlock(const int nYBlock)
{
	m_nYBlock = nYBlock;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CMeshDome::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    ���_�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CMeshDome::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    �C���f�b�N�X�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DINDEXBUFFER9 CMeshDome::GetIdxBuff(void)
{
	return m_pIdxBuff;
}

//=============================================================================
//    �e�N�X�`���擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CMeshDome::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ���b�V���h�[���̍��W�擾����
//=============================================================================
D3DXVECTOR3 CMeshDome::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ���b�V���h�[���̌����擾����
//=============================================================================
D3DXVECTOR3 CMeshDome::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ���b�V���h�[���̐F�擾����
//=============================================================================
D3DXCOLOR CMeshDome::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    ���b�V���h�[���̔��a�擾����
//=============================================================================
float CMeshDome::GetRadius(void)
{
	return m_fRadius;
}

//=============================================================================
//    ���̕������擾����
//=============================================================================
int CMeshDome::GetXBlock(void)
{
	return m_nXBlock;
}

//=============================================================================
//    ���s�̕������擾����
//=============================================================================
int CMeshDome::GetYBlock(void)
{
	return m_nYBlock;
}

//=============================================================================
//    ���_���擾����
//=============================================================================
int CMeshDome::GetNumVertex(void)
{
	return m_nNumVertex;
}

//=============================================================================
//    �C���f�b�N�X���擾����
//=============================================================================
int CMeshDome::GetNumIndex(void)
{
	return m_nNumIndex;
}

//=============================================================================
//    �|���S�����擾����
//=============================================================================
int CMeshDome::GetNumPolygon(void)
{
	return m_nNumPolygon;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CMeshDome::GetMtxWorld(void)
{
	return m_MtxWorld;
}