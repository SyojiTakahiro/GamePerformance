//*****************************************************************************
//
//     �e�̏���[shadow.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "shadow.h"
#include "manager.h"
#include "renderer.h"

#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define SHADOW_TEXTURE_NAME  "data/TEXTURE/COMMON/shadow000.jpg"     // �e�N�X�`���̃t�@�C����(�e)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CShadow::m_apTexture = NULL;  // �e�̃e�N�X�`���摜

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CShadow::CShadow(int nPriority, OBJTYPE objType) : CScene3D(nPriority, objType)
{
	// �e��l�̃N���A
	m_fRadius = 0.0f;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
//    �e�̐�������
//=============================================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nPriority)
{
	CShadow *pShadow = NULL;               // �e�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pShadow == NULL)
		{// ����������ɂȂ��Ă���
			pShadow = new CShadow(nPriority);
			if (pShadow != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pShadow->Init(pos,rot,col,fRadius)))
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

	return pShadow;
}

//=============================================================================
//    �e�̃e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CShadow::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			D3DXCreateTextureFromFile(pDevice, SHADOW_TEXTURE_NAME, &m_apTexture);
		}
	}
	return S_OK;
}

//=============================================================================
//    �e�̃e�N�X�`���������
//=============================================================================
void CShadow::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius)
{
	// �e��l�̑��
	BindTexture(m_apTexture);   // �e�N�X�`���ւ̃|�C���^
	SetPos(pos);                // ���
	SetRot(rot);                // ����
	SetCol(col);                // �F
	m_fRadius = fRadius;        // ���a

	// ���ʂ̏���������
	CScene3D::Init();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CShadow::Uninit(void)
{
	// ���ʂ̏I������
	CScene3D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CShadow::Update(void)
{
	// �O��̍��W�X�V
	m_PosOld = GetPos();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CShadow::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���Z�����̐ݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// ���ʂ̕`�揈��
			CScene3D::Draw();

			// �����̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    �e�̒��_�o�b�t�@��������
//=============================================================================
void CShadow::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.75f) * m_fRadius, 0.0f, cosf(-D3DX_PI * 0.75f) * -m_fRadius);
	pVtx[1].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.75f) * m_fRadius, 0.0f, cosf(D3DX_PI * 0.75f) * -m_fRadius);
	pVtx[2].pos = D3DXVECTOR3(sinf(-D3DX_PI * 0.25f) * m_fRadius, 0.0f, cosf(-D3DX_PI * 0.25f) * -m_fRadius);
	pVtx[3].pos = D3DXVECTOR3(sinf(D3DX_PI * 0.25f) * m_fRadius, 0.0f, cosf(D3DX_PI * 0.25f) * -m_fRadius);

	// �@��
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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
//    �e�̔��a�ݒ菈��
//=============================================================================
void CShadow::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;
}

//=============================================================================
//    �e�̔��a�擾����
//=============================================================================
float CShadow::GetRadius(void)
{
	return m_fRadius;
}

//=============================================================================
//    �e�̑O��̍��W�擾����
//=============================================================================
D3DXVECTOR3 CShadow::GetPosOld(void)
{
	return m_PosOld;
}