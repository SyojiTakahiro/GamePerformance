//*****************************************************************************
//
//     �n�ʂ̏���[field.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "field.h"
#include "manager.h"
#include "renderer.h"

#include "input.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define FIELD_TEXTURE_NAME  "data/TEXTURE/COMMON/field000.jpg"  // �e�N�X�`���̃t�@�C����(�n��)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CField::m_apTexture = NULL;    // �n�ʂ̃e�N�X�`���摜

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CField::CField(int nPriority, OBJTYPE objType) : CScene3DMesh(nPriority, objType)
{
	// �e��l�̏�����
	m_fWidth = 0.0f;  // 1�u���b�N���̕�
	m_fDepth = 0.0f;  // 1�u���b�N���̉��s
	m_nXBlock = 0;    // ���̕�����
	m_nZBlock = 0;    // ���s�̕�����
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CField::~CField()
{

}

//=============================================================================
//    �n�ʂ̐�������
//=============================================================================
CField *CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fDepth, int nXBlock, int nZBlock, char *aVertexFileName, int nPriority)
{
	CField *pField = NULL;                 // �n�ʃN���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pField == NULL)
		{// ����������ɂȂ��Ă���
			pField = new CField(nPriority);
			if (pField != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pField->Init(pos, rot, col, fWidth, fDepth, nXBlock, nZBlock, aVertexFileName)))
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

	return pField;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �n�ʂ̃e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CField::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			D3DXCreateTextureFromFile(pDevice, FIELD_TEXTURE_NAME, &m_apTexture);
		}
	}
	return S_OK;
}

//=============================================================================
//    �n�ʂ̃e�N�X�`���������
//=============================================================================
void CField::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
//    �n�ʂ̏���������
//=============================================================================
HRESULT CField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fDepth, int nXBlock, int nZBlock, char *aVertexFileName)
{
	int nNumVertex;  // ���_���ݒ�p
	int nNumIndex;   // �C���f�b�N�X���v�Z�p
	int nNumPolygon; // �|���S�����v�Z�p

	// �e��l�̐ݒ�
	BindTexture(m_apTexture);                    // �e�N�X�`��
	SetPos(pos);                                 // ���W
	SetRot(rot);                                 // ����
	SetCol(col);                                 // �F
	m_fWidth = fWidth;                           // 1�u���b�N���̕�
	m_fDepth = fDepth;                           // 1�u���b�N���̉��s
	m_nXBlock = nXBlock;                         // ���̕�����
	m_nZBlock = nZBlock;                         // ���s�̕�����
	strcpy(m_aVertexFileName, aVertexFileName);  // ���_���̃X�N���v�g�t�@�C����

	// ���b�V���̍쐬
	// �쐬�ɕK�v�Ȓ��_��,�C���f�b�N�X��,�|���S�������v�Z
	nNumVertex = (m_nXBlock + 1) * (m_nZBlock + 1);                                                               // ���_�����v�Z
	nNumIndex = ((m_nXBlock + 1) * (m_nZBlock + 1)) + (2 * (m_nZBlock - 1)) + (m_nXBlock + 1) * (m_nZBlock - 1);  // �C���f�b�N�X����ݒ�
	nNumPolygon = ((m_nXBlock * m_nZBlock) * 2) + ((m_nZBlock - 1) * 4);                                          // �`�悷��|���S������ݒ�

	// �v�Z�����l��ݒ肷��
	SetNumVertex(nNumVertex);    // ���_��
	SetNumIndex(nNumIndex);      // �C���f�b�N�X��
	SetNumPolygon(nNumPolygon);  // �|���S����

	// ���ʂ̏���������
	CScene3DMesh::Init();

	return S_OK;
}

//=============================================================================
//    �n�ʂ̏I������
//=============================================================================
void CField::Uninit(void)
{
	// ���ʂ̏I������
	CScene3DMesh::Uninit();
}

//=============================================================================
//    �n�ʂ̍X�V����
//=============================================================================
void CField::Update(void)
{

}

//=============================================================================
//    �n�ʂ̕`�揈��
//=============================================================================
void CField::Draw(void)
{
	// ���ʂ̕`�揈��
	CScene3DMesh::Draw();
}

//=============================================================================
//    �n�ʂ̒��_�o�b�t�@��������
//=============================================================================
void CField::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	FILE *pFile = NULL;               // �t�@�C���|�C���^
	int nNumVertex = GetNumVertex();  // ���_��

	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;  // ���_�o�b�t�@�����p
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(m_aVertexFileName, "rb");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // ���_���̎擾
			VERTEX_3D *pVtx;
			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// �o�C�i���t�@�C������t�B�[���h�̏���ǂݍ���
			fread(pVtx, sizeof(VERTEX_3D), GetNumVertex(), pFile);

			D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			for (int nCntV = 0; nCntV < m_nZBlock + 1; nCntV++)
			{// �c�̕����� + 1���J��Ԃ�
				for (int nCntH = 0; nCntH < m_nXBlock + 1; nCntH++)
				{// ���̕����� + 1���J��Ԃ�
					pVtx[nCntH].col = col;
				}
				pVtx += m_nXBlock + 1;  // �|�C���^��i�߂�
			}

			// ���_�o�b�t�@���A�����b�N����
			pVtxBuff->Unlock();

			// ���_�o�b�t�@��ݒ肷��
			SetVtxBuff(pVtxBuff);

			// �t�@�C������Ă���
			fclose(pFile);

			// �@���v�Z����
			MakeNormal();
		}
	}
}

//=============================================================================
//    �n�ʂ̃C���f�b�N�X�o�b�t�@��������
//=============================================================================
void CField::MakeIndex(const LPDIRECT3DDEVICE9 pDevice)
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
	pIdxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@��ݒ肷��
	SetIdxBuff(pIdxBuff);
}

//=============================================================================
//    �n�ʂ̖ʂ̍����擾����
//=============================================================================
float CField::GetPolyHeight(const D3DXVECTOR3 pos, D3DXVECTOR3 *pPolygonNormal)
{
	float fHeight = 0.0f;               // ����(�ŏI�I�ɂ��̒l��Ԃ�)
	float fRivision = 0.0f;             // �����̕␳�l(�@��������o���̂�0.0�̍����ɂ��낦�邽��)
	int nVertexIdxStart = 0;            // ����̒��_�ԍ�(���[�v���邽�тɂ���Ă���)
	D3DXMATRIX mtxRot,mtxTrans;         // �v�Z�p���[���h�}�g���b�N�X
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

	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
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
					// ���[���h�}�g���b�N�X�̏�����
					D3DXMatrixIdentity(&VertexMtxWorld[nCntVer]);

					// ��]�𔽉f
					D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);
					D3DXMatrixMultiply(&VertexMtxWorld[nCntVer], &VertexMtxWorld[nCntVer], &mtxRot);

					// �ʒu�𔽉f
					D3DXMatrixTranslation(&mtxTrans, VertexPos[nCntVer].x, VertexPos[nCntVer].y, VertexPos[nCntVer].z);
					D3DXMatrixMultiply(&VertexMtxWorld[nCntVer], &VertexMtxWorld[nCntVer], &mtxTrans);

					// �e�̏����|�����킹��
					D3DXMatrixMultiply(&VertexMtxWorld[nCntVer], &VertexMtxWorld[nCntVer], &GetMtxWorld());
				}

				// ���_�̃��[���h���W�𔲂��o��
				VertexWorldPos[0] = D3DXVECTOR3(VertexMtxWorld[0]._41, VertexMtxWorld[0]._42, VertexMtxWorld[0]._43);
				VertexWorldPos[1] = D3DXVECTOR3(VertexMtxWorld[1]._41, VertexMtxWorld[1]._42, VertexMtxWorld[1]._43);
				VertexWorldPos[2] = D3DXVECTOR3(VertexMtxWorld[2]._41, VertexMtxWorld[2]._42, VertexMtxWorld[2]._43);
				VertexWorldPos[3] = D3DXVECTOR3(VertexMtxWorld[3]._41, VertexMtxWorld[3]._42, VertexMtxWorld[3]._43);

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
					*pPolygonNormal = PolygonNormal;                        // �@���������Ă���

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
					*pPolygonNormal = PolygonNormal;                        // �@���������Ă���

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
	}

	return fHeight;
}

//=============================================================================
//    �n�ʂ̖@���v�Z����
//=============================================================================
void CField::MakeNormal(void)
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