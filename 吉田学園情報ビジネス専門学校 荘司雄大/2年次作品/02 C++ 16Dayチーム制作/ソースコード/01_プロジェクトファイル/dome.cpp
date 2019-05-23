//*****************************************************************************
//
//     �����̏���[dome.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "dome.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define DOME_TEXTURE_NAME  "data/TEXTURE/COMMON/sky000.jpg"  // �e�N�X�`���̃t�@�C����(��)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CDome::m_apTexture = NULL;          // �����̃e�N�X�`���摜

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CDome::CDome(int nPriority, OBJTYPE objType) : CScene3DMesh(nPriority, objType)
{
	// �e��l�̏�����
	m_fRadius = 0.0f;        // �����̔��a
	m_nXBlock = 0;           // ���̕�����
	m_nYBlock = 0;           // �c�̕�����
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CDome::~CDome()
{

}

//=============================================================================
//    �����̐�������
//=============================================================================
CDome *CDome::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nXBlock, int nYBlock, int nPriority)
{
	CDome *pDome = NULL;                   // �����N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pDome == NULL)
		{// ����������ɂȂ��Ă���
			pDome = new CDome(nPriority);
			if (pDome != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pDome->Init(pos, rot, fRadius, nXBlock, nYBlock)))
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
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}

	return pDome;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �����̃e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CDome::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			D3DXCreateTextureFromFile(pDevice, DOME_TEXTURE_NAME, &m_apTexture);
		}
	}
	return S_OK;
}

//=============================================================================
//    �����̃e�N�X�`���������
//=============================================================================
void CDome::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
//    �����̏���������
//=============================================================================
HRESULT CDome::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nXBlock, int nYBlock)
{
	int nNumVertex;  // ���_���ݒ�p
	int nNumIndex;   // �C���f�b�N�X���v�Z�p
	int nNumPolygon; // �|���S�����v�Z�p

	// �e��l�̐ݒ�
	BindTexture(m_apTexture);  // �e�N�X�`��
	SetPos(pos);               // ���W
	SetRot(rot);               // ����
	m_fRadius = fRadius;       // �����̔��a
	m_nXBlock = nXBlock;       // ���̕�����
	m_nYBlock = nYBlock;       // �c�̕�����

	// ���b�V���̍쐬
	// �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
	nNumVertex = (m_nXBlock + 1) * (m_nYBlock + 1);                                                               // ���_�����v�Z
	nNumIndex = ((m_nXBlock + 1) * (m_nYBlock + 1)) + (2 * (m_nYBlock - 1)) + (m_nXBlock + 1) * (m_nYBlock - 1);  // �C���f�b�N�X����ݒ�
	nNumPolygon = ((m_nXBlock * m_nYBlock) * 2) + ((m_nYBlock - 1) * 4);                                          // �`�悷��|���S������ݒ�

	// �v�Z�����l��ݒ肷��
	SetNumVertex(nNumVertex);    // ���_��
	SetNumIndex(nNumIndex);      // �C���f�b�N�X��
	SetNumPolygon(nNumPolygon);  // �|���S����

	// ���ʂ̏���������
	CScene3DMesh::Init();

	return S_OK;
}

//=============================================================================
//    �����̏I������
//=============================================================================
void CDome::Uninit(void)
{
	// ���ʂ̏I������
	CScene3DMesh::Uninit();
}

//=============================================================================
//    �����̍X�V����
//=============================================================================
void CDome::Update(void)
{
	D3DXVECTOR3 rot = GetRot();  // �����̎擾

	// ��������]������
	rot.y += D3DX_PI / 14400;
	if (rot.y > D3DX_PI)
	{// �~�����𒴂���
		rot.y -= D3DX_PI * 2.0f;
	}

	// �����̐ݒ�
	SetRot(rot);
}

//=============================================================================
//    �����̕`�揈��
//=============================================================================
void CDome::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// ���ʂ̕`�揈��
			CScene3DMesh::Draw();

			// �W�̕`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLEFAN, 0, 0, (m_nXBlock + 1), GetNumIndex(), (m_nXBlock));
		}
	}
}

//=============================================================================
//    �����̒��_�o�b�t�@��������
//=============================================================================
void CDome::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	int nNumVertex = GetNumVertex();  // ���_��

	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;  // ���_�o�b�t�@�����p
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (nNumVertex + (m_nXBlock + 2)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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

	for (int nCntV = 0; nCntV <  m_nYBlock + 1; nCntV++)
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
			pVtx[nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

			fAngle -= D3DX_PI / (m_nXBlock / 2);  // �p�x��i�߂�
			Utex += 1.0f / (m_nXBlock / 2);    // �e�N�X�`��U���W��i�߂�

			if (fAngle <= -D3DX_PI)
			{// �{�����~�����𒴂���
				fAngle = D3DX_PI;
			}
		}
		fDome += 1.0f / m_nYBlock;           // ���a���k�߂�{�����グ��
		fRadius = cosf(fDome) * m_fRadius;   // ���a���R�T�C���J�[�u�ŏk�߂Ă���
		fAngle = 0.0f;                       // �p�x��0��
		Utex = 0.0f;                         // �e�N�X�`��U���W��ݒ�
		Vtex -= 1.0f / (m_nYBlock);          // �e�N�X�`��V���W��i�߂�
		pVtx += m_nXBlock + 1;               // ���_�̐擪�̔ԍ���i�߂�
	}

	// �W�p�̒��_�����쐬
	fDome += 2.0f / m_nYBlock;   // ���a���k�߂�{�����グ��
	// ���_���W
	pos = D3DXVECTOR3(0.0f, sinf(fDome) * m_fRadius, 0.0f);
	pVtx[0].pos = pos;

	// �@���x�N�g��
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -2.2f);

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, -0.99f);
	fDome -= 2.0f / m_nYBlock;   // ���a���k�߂�{����������
	pVtx++;

	fDome -= 1.0f / m_nYBlock;           // ���a���k�߂�{����߂��Ă���
	fRadius = cosf(fDome) * m_fRadius;   // ���a�v�Z��߂��Ă���
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
		pVtx[nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W
		pVtx[nCntH].tex = D3DXVECTOR2(0.0f, -0.98f);

		fAngle += -D3DX_PI / (m_nXBlock / 2);  // �p�x��i�߂�
		Utex -= 1.0f / (m_nXBlock / 2);    // �e�N�X�`��U���W��i�߂�

		if (fAngle <= -D3DX_PI)
		{// �{�����~�����𒴂���
			fAngle = D3DX_PI;
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//     �����̃C���f�b�N�X�o�b�t�@��������
//=============================================================================
void CDome::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	int nNumIndex = GetNumIndex();   // �C���f�b�N�X��

	// �C���f�b�N�X�o�b�t�@�̐���
	LPDIRECT3DINDEXBUFFER9 pIdxBuff = NULL;  // �C���f�b�N�X�o�b�t�@�����p
	pDevice->CreateIndexBuffer(sizeof(WORD) * (nNumIndex + (m_nXBlock + 2)),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&pIdxBuff,
		NULL);

	WORD *pIdx;       // �C���f�b�N�X�f�[�^�ւ̃|�C���^
	int nCntIdx = 0;  // �C���f�b�N�X�ԍ�

	// �C���f�b�N�X�o�b�t�@�����b�N��,�C���f�b�N�X�f�[�^�ւ̃|�C���^���擾
	pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

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
	pIdxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@��ݒ肷��
	SetIdxBuff(pIdxBuff);
}