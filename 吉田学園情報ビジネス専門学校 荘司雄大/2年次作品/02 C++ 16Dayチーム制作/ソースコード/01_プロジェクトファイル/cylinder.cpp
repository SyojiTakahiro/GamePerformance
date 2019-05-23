//*****************************************************************************
//
//     �~���̏���[cylinder.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "cylinder.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define CYLINDER_TEXTURE_NAME  "data/TEXTURE/COMMON/mountain000.png"  // �e�N�X�`���̃t�@�C����(�R)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CCylinder::m_apTexture = NULL;          // �~���̃e�N�X�`���摜

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CCylinder::CCylinder(int nPriority, OBJTYPE objType) : CScene3DMesh(nPriority, objType)
{
	// �e��l�̏�����
	m_fRadius = 0.0f; // �~���̔��a
	m_fHeight = 0.0f; // 1�u���b�N���̉��s
	m_nXBlock = 0;    // ���̕�����
	m_nYBlock = 0;    // �c�̕�����
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CCylinder::~CCylinder()
{

}

//=============================================================================
//    �~���̐�������
//=============================================================================
CCylinder *CCylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nXBlock, int nYBlock, int nPriority)
{
	CCylinder *pCylinder = NULL;           // �~���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pCylinder == NULL)
		{// ����������ɂȂ��Ă���
			pCylinder = new CCylinder(nPriority);
			if (pCylinder != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pCylinder->Init(pos, rot, fRadius, fHeight, nXBlock, nYBlock)))
				{// �������Ɏ��s����
					return NULL;
				}
				else
				{// �������ɐ�������
				    // �C���X�^���X�̃A�h���X��Ԃ�
					return pCylinder;
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
//    �~���̃e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CCylinder::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			D3DXCreateTextureFromFile(pDevice, CYLINDER_TEXTURE_NAME, &m_apTexture);
		}
	}
	return S_OK;
}

//=============================================================================
//    �~���̃e�N�X�`���������
//=============================================================================
void CCylinder::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
//    �~���̏���������
//=============================================================================
HRESULT CCylinder::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nXBlock, int nYBlock)
{
	int nNumVertex;  // ���_���ݒ�p
	int nNumIndex;   // �C���f�b�N�X���v�Z�p
	int nNumPolygon; // �|���S�����v�Z�p

	// �e��l�̐ݒ�
	BindTexture(m_apTexture);  // �e�N�X�`��
	SetPos(pos);               // ���W
	SetRot(rot);               // ����
	m_fRadius = fRadius;       // �~���̔��a
	m_fHeight = fHeight;       // 1�u���b�N���̉��s
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
//    �~���̏I������
//=============================================================================
void CCylinder::Uninit(void)
{
	// ���ʂ̏I������
	CScene3DMesh::Uninit();
}

//=============================================================================
//    �~���̍X�V����
//=============================================================================
void CCylinder::Update(void)
{

}

//=============================================================================
//    �~���̕`�揈��
//=============================================================================
void CCylinder::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
	        // �A���t�@�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // �L����
			pDevice->SetRenderState(D3DRS_ALPHAREF, 200);             // �����x��200
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ���傫�����̂�`�悷��

			// ���ʂ̕`�揈��
			CScene3DMesh::Draw();

			// �A���t�@�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  // ������
		}
	}
}

//=============================================================================
//    �~���̒��_�o�b�t�@��������
//=============================================================================
void CCylinder::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	int nNumVertex = GetNumVertex();  // ���_��

	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;  // ���_�o�b�t�@�����p
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVertex,
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
	D3DXVECTOR3 pos;    // ���W
	float Utex;         // �e�N�X�`��U���W
	float Vtex;         // �e�N�X�`��V���W

	fAngle = 0.0f;                          // �p�x��0��
	fRadius = m_fRadius;                    // ���a��ݒ�
	pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���W��ݒ�
	Utex = 0.0f;                            // �e�N�X�`��U���W��ݒ�
	Vtex = 0.0f;                            // �e�N�X�`��V���W��ݒ�

	for (int nCntV = 0; nCntV < m_nYBlock + 1; nCntV++)
	{// ���������̕����������J��Ԃ�
		for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
		{// ���������̕����������J��Ԃ�
			pos.x = sinf(fAngle) * fRadius;
			pos.z = cosf(fAngle) * fRadius;

			// ���_���W
			pVtx[nCntH].pos = pos;

			// �@���x�N�g��
			pVtx[nCntH].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

			// ���_�J���[
			pVtx[nCntH].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W
			pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

			fAngle -= D3DX_PI / (m_nXBlock / 2.0f);  // �p�x��i�߂�
			Utex += 1.0f / (m_nXBlock / 2.0f);       // �e�N�X�`��U���W��i�߂�

			if (fAngle > D3DX_PI)
			{// �p�x���~�����𒴂���
				fAngle -= D3DX_PI * 2.0f;
			}
		}
		pos.y += m_fHeight / m_nYBlock;  // ���W���グ��
		fAngle = 0.0f;                   // �p�x��0�ɖ߂�
		Utex = 0.0f;                     // �e�N�X�`��U���W��߂�
		Vtex -= 1.0f / m_nYBlock;        // �e�N�X�`��V���W��i�߂�
		pVtx += m_nXBlock + 1;           // ���_�o�b�t�@�̔ԍ������̕��������i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//     �~���̃C���f�b�N�X�o�b�t�@��������
//=============================================================================
void CCylinder::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
{
	int nNumIndex = GetNumIndex();   // �C���f�b�N�X��

	// �C���f�b�N�X�o�b�t�@�̐���
	LPDIRECT3DINDEXBUFFER9 pIdxBuff = NULL;  // �C���f�b�N�X�o�b�t�@�����p
	pDevice->CreateIndexBuffer(sizeof(WORD) * nNumIndex,
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

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	pIdxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@��ݒ肷��
	SetIdxBuff(pIdxBuff);
}