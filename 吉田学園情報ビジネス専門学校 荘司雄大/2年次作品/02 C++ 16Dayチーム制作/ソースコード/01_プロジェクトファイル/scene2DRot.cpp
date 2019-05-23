//*****************************************************************************
//
//     �V�[��(2D)��]�̏���[scene2DRot.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "scene2DRot.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//   �R���X�g���N�^
//=============================================================================
CScene2DRot::CScene2DRot(int nPriority, OBJTYPE objType) : CScene2D(nPriority, objType)
{
	// �e��l�̃N���A
	m_fRotation = 0.0f;  // ����
	m_fScale = 0.0f;     // ���݂̑傫��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CScene2DRot::~CScene2DRot()
{

}

//=============================================================================
//    ��������
//=============================================================================
CScene2DRot *CScene2DRot::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, float fRotation, int nPriority)
{
	CScene2DRot *pScene2DRot = NULL;      // �V�[��2D��]�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();  // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pScene2DRot == NULL)
		{// ����������ɂȂ��Ă���
			pScene2DRot = new CScene2DRot(nPriority);
			if (pScene2DRot != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pScene2DRot->Init(pos, col, fScale, fRotation)))
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

	return pScene2DRot;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CScene2DRot::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, float fRotation)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	 // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �e��l�̐ݒ�
			SetPos(pos);              // ���W
			SetCol(col);              // �F
			m_fRotation = fRotation;  // ��]��
			m_fScale = fScale;        // ���݂̑傫��

			// ���_�o�b�t�@�̐���
			CScene2D::Init();
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
void CScene2DRot::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CScene2DRot::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CScene2DRot::Draw(void)
{
	// ���ʂ̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
//    �|���S���̒��_�o�b�t�@��������
//=============================================================================
void CScene2DRot::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_����ݒ�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(GetPos().x + sinf(-D3DX_PI * 0.75f + m_fRotation) * m_fScale, GetPos().y + cosf(-D3DX_PI * 0.75f + m_fRotation) * m_fScale, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetPos().x + sinf(D3DX_PI * 0.75f + m_fRotation) * m_fScale, GetPos().y + cosf(D3DX_PI * 0.75f + m_fRotation) * m_fScale, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(GetPos().x + sinf(-D3DX_PI * 0.25f + m_fRotation) * m_fScale, GetPos().y + cosf(-D3DX_PI * 0.25f + m_fRotation) * m_fScale, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetPos().x + sinf(D3DX_PI * 0.25f + m_fRotation) * m_fScale, GetPos().y + cosf(D3DX_PI * 0.25f + m_fRotation) * m_fScale, 0.0f);

	// ���_�e�N�X�`��
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@�̐ݒ�
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    �|���S���̒��_�ݒ菈��
//=============================================================================
void CScene2DRot::SetVtxBuffPos(D3DXVECTOR3 pos)
{
	// ���W��n��
	SetPos(pos);

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene2D::GetVtxBuff(); // ���_���擾
	if (pVtxBuff != NULL)
	{// ���_�����擾�ł���
	    // ���_���ݒ�
		VERTEX_2D *pVtx;
		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.75f + m_fRotation) * m_fScale, pos.y + cosf(-D3DX_PI * 0.75f + m_fRotation) * m_fScale, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.75f + m_fRotation) * m_fScale, pos.y + cosf(D3DX_PI * 0.75f + m_fRotation) * m_fScale, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.25f + m_fRotation) * m_fScale, pos.y + cosf(-D3DX_PI * 0.25f + m_fRotation) * m_fScale, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.25f + m_fRotation) * m_fScale, pos.y + cosf(D3DX_PI * 0.25f + m_fRotation) * m_fScale, 0.0f);

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �|���S���̒��_�J���[����
//=============================================================================
void CScene2DRot::SetVtxBuffCol(D3DXCOLOR col)
{
	// �F��n��
	SetCol(col);

	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CScene2D::GetVtxBuff(); // ���_���擾
	if (pVtxBuff != NULL)
	{// ���_�����擾�ł���
	    // ���_���ݒ�
		VERTEX_2D *pVtx;
		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �|���S���̌����ݒ菈��
//=============================================================================
void CScene2DRot::SetRotation(const float fRotation)
{
	m_fRotation = fRotation;
}

//=============================================================================
//    �|���S���̌��݂̑傫���ݒ菈��
//=============================================================================
void CScene2DRot::SetScale(const float fScale)
{
	m_fScale = fScale;
}

//=============================================================================
//    �|���S���̌����擾����
//=============================================================================
float CScene2DRot::GetRotation(void)
{
	return m_fRotation;
}

//=============================================================================
//    �|���S���̌��݂̑傫���擾����
//=============================================================================
float CScene2DRot::GetScale(void)
{
	return m_fScale;
}
