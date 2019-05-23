//*****************************************************************************
//
//     3D�|���S���̏���[scene3D.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "scene3D.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"


#include "player.h"
#include "model.h"
#include "camera.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CScene3D::CScene3D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pTexture = NULL;                         // �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;                         // ���_�o�b�t�@�ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // �|���S���̍��W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // �|���S���̌���
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // �|���S���̐F
	m_fWidth = 0.0f;                           // �|���S���̕�
	m_fHeight = 0.0f;                          // �|���S���̍���
	m_fAngle = 0.0f;                           // �|���S���̊p�x
	m_fLength = 0.0f;                          // �|���S���̒���
	D3DXMatrixIdentity(&m_MtxWorld);           // ���[���h�}�g���b�N�X
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CScene3D::~CScene3D()
{

}

//=============================================================================
//    ��������
//=============================================================================
CScene3D *CScene3D::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nPriority)
{
	CScene3D *pScene3D = NULL;             // 3D�|���S���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pScene3D == NULL)
		{// ����������ɂȂ��Ă���
			pScene3D = new CScene3D(nPriority);
			if (pScene3D != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pScene3D->SetPos(pos);         // �|���S���̍��W
				pScene3D->SetRot(rot);         // �|���S���̌���
				pScene3D->SetCol(col);         // �|���S���̐F
				pScene3D->SetWidth(fWidth);    // �|���S���̕�
				pScene3D->SetHeight(fHeight);  // �|���S���̍���

				if (FAILED(pScene3D->Init()))
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

	return pScene3D;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CScene3D::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
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
void CScene3D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �����[�X����
	CScene::Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CScene3D::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CScene3D::Draw(void)
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

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pTexture);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CScene3D::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
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
void CScene3D::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �I�t�Z�b�g�̒��������߂�
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

	// �I�t�Z�b�g�̊p�x�����߂�
	m_fAngle = atan2f(m_fWidth, m_fHeight);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
	pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
	pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);
	pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);

	// �@���v�Z�p�x�N�g��
	D3DXVECTOR3 vecVer[4];

	// ���_�̖@��
	D3DXVECTOR3 nor1, nor2;

	// �O�ϗp�Ƀx�N�g�������
	vecVer[0] = pVtx[0].pos - pVtx[2].pos;
	vecVer[1] = pVtx[1].pos - pVtx[2].pos;
	vecVer[2] = pVtx[2].pos - pVtx[1].pos;
	vecVer[3] = pVtx[3].pos - pVtx[1].pos;

	// �x�N�g�����O�ςŌv�Z���Ė@�����o��
	D3DXVec3Cross(&nor1, &vecVer[0], &vecVer[1]);
	D3DXVec3Cross(&nor2, &vecVer[3], &vecVer[2]);
	D3DXVec3Normalize(&nor1, &nor1);
	D3DXVec3Normalize(&nor2, &nor2);

	// �@������
	pVtx[0].nor = nor1;
	pVtx[1].nor = (nor1 + nor2) / 2;
	pVtx[2].nor = (nor1 + nor2) / 2;
	pVtx[3].nor = nor2;

	// ���_�J���[
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    �e�N�X�`���ݒ菈��
//=============================================================================
void CScene3D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ���_�o�b�t�@�ݒ菈��
//=============================================================================
void CScene3D::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    �|���S���̍��W�ݒ菈��
//=============================================================================
void CScene3D::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �|���S���̌����ݒ菈��
//=============================================================================
void CScene3D::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    �|���S���̐F�ݒ菈��
//=============================================================================
void CScene3D::SetCol(const D3DXCOLOR col)
{
	// �F��ݒ�
	m_Col = col;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[
		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �|���S���̕��ݒ菈��
//=============================================================================
void CScene3D::SetWidth(const float fWidth)
{
	// ����ݒ�
	m_fWidth = fWidth;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �I�t�Z�b�g�̒��������߂�
		m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

		// �I�t�Z�b�g�̊p�x�����߂�
		m_fAngle = atan2f(m_fWidth, m_fHeight);

		// ���_���W
		pVtx[0].pos.x = -sinf(m_fAngle) * m_fLength;
		pVtx[1].pos.x = sinf(m_fAngle) * m_fLength;
		pVtx[2].pos.x = -sinf(m_fAngle) * m_fLength;
		pVtx[3].pos.x = sinf(m_fAngle) * m_fLength;

	    // ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �|���S���̍����ݒ菈��
//=============================================================================
void CScene3D::SetHeight(const float fHeight)
{
	// ������ݒ�
	m_fHeight = fHeight;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł��Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �I�t�Z�b�g�̒��������߂�
		m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

		// �I�t�Z�b�g�̊p�x�����߂�
		m_fAngle = atan2f(m_fWidth, m_fHeight);

		// ���_���W
		pVtx[0].pos.y = cosf(m_fAngle) * m_fLength;
		pVtx[1].pos.y = cosf(m_fAngle) * m_fLength;
		pVtx[2].pos.y = -cosf(m_fAngle) * m_fLength;
		pVtx[3].pos.y = -cosf(m_fAngle) * m_fLength;

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �|���S���̊p�x�ݒ菈��
//=============================================================================
void CScene3D::SetAngle(const float fAngle)
{
	// �p�x��ݒ�
	m_fAngle = fAngle;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
		pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
		pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �|���S���̒����ݒ菈��
//=============================================================================
void CScene3D::SetLength(const float fLength)
{
	// ������ݒ�
	m_fLength = fLength;

	if (m_pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
		pVtx[1].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, cosf(m_fAngle) * m_fLength);
		pVtx[2].pos = D3DXVECTOR3(-sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);
		pVtx[3].pos = D3DXVECTOR3(sinf(m_fAngle) * m_fLength, 0.0f, -cosf(m_fAngle) * m_fLength);

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CScene3D::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    ���_�o�b�t�@�擾����
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene3D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    �e�N�X�`���擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CScene3D::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    �|���S���̍��W�擾����
//=============================================================================
D3DXVECTOR3 CScene3D::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �|���S���̌����擾����
//=============================================================================
D3DXVECTOR3 CScene3D::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    �|���S���̐F�擾����
//=============================================================================
D3DXCOLOR CScene3D::GetCol(void)
{
	return m_Col;
}

//=============================================================================
//    �|���S���̕��擾����
//=============================================================================
float CScene3D::GetWidth(void)
{
	return m_fWidth;
}

//=============================================================================
//    �|���S���̍����擾����
//=============================================================================
float CScene3D::GetHeight(void)
{
	return m_fHeight;
}

//=============================================================================
//    �|���S���̊p�x�擾����
//=============================================================================
float CScene3D::GetAngle(void)
{
	return m_fAngle;
}

//=============================================================================
//    �|���S���̒����擾����
//=============================================================================
float CScene3D::GetLength(void)
{
	return m_fLength;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CScene3D::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    �|���S���̃��[���h���W�ɂ����鍂���擾���鏈��
//=============================================================================
float CScene3D::GetPolyHeight(D3DXVECTOR3 pos)
{
	float fHeight = 0.0f;        // ����
	D3DXVECTOR3 PolygonNormal;   // ���_�̖@��
	D3DXVECTOR3 PolygonPos;      // �v���C���[�Ƃ̃x�N�g�������|���S���̒��_���W
	D3DXVECTOR3 Vec;             // �v���C���[�ƒ��_���W�Ƃ̃x�N�g��
	D3DXVECTOR3 VerPos[4];       // 3D�|���S���̒��_���W
	D3DXVECTOR3 VerNor[4];       // 3D�|���S���̒��_�@��
	D3DXMATRIX VerMtxWorld[4];   // 3D�|���S���̒��_���[���h�}�g���b�N�X
	bool bLeftPolygon = false;   // �����̃|���S���ɏ���Ă��邩�ǂ���
	bool bRightPolygon = false;  // �E���̃|���S���ɏ���Ă��邩�ǂ���
	int nCntLeftPolygon = 0;     // �����|���S���̃x�N�g���̔����ʂ�����
	int nCntRightPolygon = 0;    // �E���|���S���̃x�N�g���̔����ʂ�����

	// ���_�����擾����
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���肷��x�N�g���̍��W���擾
	VerPos[0] = pVtx[0].pos;
	VerPos[1] = pVtx[1].pos;
	VerPos[2] = pVtx[2].pos;
	VerPos[3] = pVtx[3].pos;

	// ����Ɏg���@�����擾
	VerNor[0] = pVtx[0].nor;
	VerNor[1] = pVtx[1].nor;
	VerNor[2] = pVtx[2].nor;
	VerNor[3] = pVtx[3].nor;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// 3D�|���S���̌������l�����Ē��_�̃��[���h�}�g���b�N�X���v�Z����
	D3DXMATRIX mtxRot, mtxTrans;        // �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxParent = m_MtxWorld;  // 3D�|���S���̃��[���h�}�g���b�N�X���擾
	D3DXVECTOR3 VerRot = m_Rot;         // 3D�|���S���̌������擾
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// ���肷�钸�_�̐������J��Ԃ�
	 // ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&VerMtxWorld[nCntVer]);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, VerRot.y, VerRot.x, VerRot.z);
		D3DXMatrixMultiply(&VerMtxWorld[nCntVer], &VerMtxWorld[nCntVer], &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, VerPos[nCntVer].x, VerPos[nCntVer].y, VerPos[nCntVer].z);
		D3DXMatrixMultiply(&VerMtxWorld[nCntVer], &VerMtxWorld[nCntVer], &mtxTrans);

		// �e�̃��[���h�}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&VerMtxWorld[nCntVer], &VerMtxWorld[nCntVer], &mtxParent);
	}


	// �e��l�̎擾
	D3DXVECTOR3 WorldPolygonPos[4] =
	{// �������܂߂�3D�|���S���̒��_���[���h���W
		D3DXVECTOR3(VerMtxWorld[0]._41,VerMtxWorld[0]._42,VerMtxWorld[0]._43),
		D3DXVECTOR3(VerMtxWorld[1]._41,VerMtxWorld[1]._42,VerMtxWorld[1]._43),
		D3DXVECTOR3(VerMtxWorld[2]._41,VerMtxWorld[2]._42,VerMtxWorld[2]._43),
		D3DXVECTOR3(VerMtxWorld[3]._41,VerMtxWorld[3]._42,VerMtxWorld[3]._43),
	};

	D3DXVECTOR3 WorldLeftPolygonPos[3] =
	{// �������܂߂�3D�|���S��(����)�̃��[���h���W
		WorldPolygonPos[1],
		WorldPolygonPos[2],
		WorldPolygonPos[0],
	};
	D3DXVECTOR3 WorldRightPolygonPos[3] =
	{// �������܂߂�3D�|���S��(�E��)�̃��[���h���W
		WorldPolygonPos[1],
		WorldPolygonPos[3],
		WorldPolygonPos[2],
	};

	// �v�Z�p�O�σx�N�g��
	D3DXVECTOR3 VecVerToVer, VecPlayerToVer;
	// �����̃|���S���ɏ���Ă��邩����
	for (int nCntVer = 0; nCntVer < 3; nCntVer++)
	{// ���肷�钸�_�̐������J��Ԃ�
		VecVerToVer = WorldRightPolygonPos[(nCntVer + 1) % 3] - WorldRightPolygonPos[nCntVer];
		VecPlayerToVer = pos - WorldRightPolygonPos[nCntVer];

		if ((VecVerToVer.x * VecPlayerToVer.z) - (VecVerToVer.z * VecPlayerToVer.x) <= 0)
		{// �x�N�g���̉E���ɂ���
			nCntRightPolygon++;
		}
	}

	// �E���̃|���S���ɏ���Ă��邩����
	for (int nCntVer = 0; nCntVer < 3; nCntVer++)
	{// ���肷�钸�_�̐������J��Ԃ�
		VecVerToVer = WorldLeftPolygonPos[(nCntVer + 1) % 3] - WorldLeftPolygonPos[nCntVer];
		VecPlayerToVer = pos - WorldLeftPolygonPos[nCntVer];

		if ((VecVerToVer.x * VecPlayerToVer.z) - (VecVerToVer.z * VecPlayerToVer.x) <= 0)
		{// �x�N�g���̉E���ɂ���
			nCntLeftPolygon++;
		}
	}

	// ����ɕK�v�ȕϐ���錾
	D3DXVECTOR3 PolygonVerPos[3];
	D3DXVECTOR3 VecPolygon[2];
	D3DXVECTOR3 nor;
	float fData = 0.0f;

	if (nCntLeftPolygon == 3)
	{// �����̃|���S���̂��ׂẴx�N�g���̓����ɂ���
		bLeftPolygon = true;
		PolygonVerPos[0] = D3DXVECTOR3(WorldPolygonPos[0].x, WorldPolygonPos[0].y - WorldPolygonPos[0].y, WorldPolygonPos[0].z);
		PolygonVerPos[1] = D3DXVECTOR3(WorldPolygonPos[1].x, WorldPolygonPos[1].y - WorldPolygonPos[0].y, WorldPolygonPos[1].z);
		PolygonVerPos[2] = D3DXVECTOR3(WorldPolygonPos[2].x, WorldPolygonPos[2].y - WorldPolygonPos[0].y, WorldPolygonPos[2].z);

		VecPolygon[0] = PolygonVerPos[1] - PolygonVerPos[0];
		VecPolygon[1] = PolygonVerPos[2] - PolygonVerPos[0];

		D3DXVec3Cross(&PolygonNormal, &VecPolygon[0], &VecPolygon[1]);
		D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);
		fData = WorldPolygonPos[0].y;

		// �����̍��W�ƒ��_���W�̃x�N�g�����v�Z
		Vec = pos - PolygonVerPos[0];

		// ���ς̊T�O�𗘗p���č���������o��
		float DotX = (PolygonNormal.x * Vec.x);      // X�����̓��ς��v�Z
		float DotZ = (PolygonNormal.z * Vec.z);      // Z�����̓��ς��v�Z
		float Dot = -DotX - DotZ;                    // X������Z�����̓��ϓ��m������
		fHeight = (Dot / PolygonNormal.y) + fData;   // ���������̂�Y�����̖@���Ŋ���
	}
	if (nCntRightPolygon == 3)
	{// �E���̃|���S���̂��ׂẴx�N�g���̓����ɂ���
		bRightPolygon = true;
		PolygonVerPos[0] = D3DXVECTOR3(WorldPolygonPos[3].x, WorldPolygonPos[3].y - WorldPolygonPos[3].y, WorldPolygonPos[3].z);
		PolygonVerPos[1] = D3DXVECTOR3(WorldPolygonPos[1].x, WorldPolygonPos[1].y - WorldPolygonPos[3].y, WorldPolygonPos[1].z);
		PolygonVerPos[2] = D3DXVECTOR3(WorldPolygonPos[2].x, WorldPolygonPos[2].y - WorldPolygonPos[3].y, WorldPolygonPos[2].z);

		VecPolygon[0] = PolygonVerPos[1] - PolygonVerPos[0];
		VecPolygon[1] = PolygonVerPos[2] - PolygonVerPos[0];

		D3DXVec3Cross(&PolygonNormal, &VecPolygon[0], &VecPolygon[1]);
		D3DXVec3Normalize(&PolygonNormal, &PolygonNormal);
		fData = WorldPolygonPos[3].y;

		// �����̍��W�ƒ��_���W�̃x�N�g�����v�Z
		Vec = pos - PolygonVerPos[0];

		// ���ς̊T�O�𗘗p���č���������o��
		float DotX = (PolygonNormal.x * Vec.x);      // X�����̓��ς��v�Z
		float DotZ = (PolygonNormal.z * Vec.z);      // Z�����̓��ς��v�Z
		float Dot = -DotX - DotZ;                    // X������Z�����̓��ϓ��m������
		fHeight = (Dot / PolygonNormal.y) + fData;   // ���������̂�Y�����̖@���Ŋ���
	}

	return fHeight;  // ������Ԃ�
}