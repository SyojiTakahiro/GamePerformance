//*****************************************************************************
//
//     �e(�X�v���C�g)�̏���[shadow.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "3DPolygon.h"
#include "manager.h"
#include "system.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define SHADOW_TEXUTRE_NAME "data/TEXTURE/COMMON/shadow000.png"   // �e�e�N�X�`���ւ̑��΃p�X��

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CShadow::m_pShadowTexture = NULL;   // �e�N�X�`���ւ̃|�C���^

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CShadow::CShadow(int nPriority, OBJTYPE objType) : CScene3D(nPriority, objType)
{
	// �e��l�̃N���A
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
//    ��������
//=============================================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nPriority)
{
	CShadow *pShadow = NULL;               // �N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pShadow == NULL)
		{// ����������ɂȂ��Ă���
			pShadow = new CShadow(nPriority);
			if (pShadow != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pShadow->SetPos(pos);         // �|���S���̍��W
				pShadow->SetRot(rot);         // �|���S���̌���
				pShadow->SetCol(col);         // �|���S���̐F
				pShadow->SetWidth(fWidth);    // �|���S���̕�
				pShadow->SetHeight(fHeight);  // �|���S���̍���

				if (FAILED(pShadow->Init()))
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

	return pShadow;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �e�N�X�`���ǂݍ��ݏ���
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
		    // �e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), SHADOW_TEXUTRE_NAME, &m_pShadowTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`���J������
//=============================================================================
void CShadow::UnLoad(void)
{
	DIRECT_RELEASE(m_pShadowTexture)
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CShadow::Init(void)
{
	// �e�e�N�X�`����ݒ肷��
	BindTexture(m_pShadowTexture);

	if (FAILED(CScene3D::Init()))
	{// �������Ɏ��s����
		return E_FAIL;
	}

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
			// ���Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// �A���t�@�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // �L����
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);               // �����x��0
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ���傫�����̂�`�悷��

			// ���ʂ̏���
			CScene3D::Draw();

			// �A���t�@�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// ���Z�����̐ݒ��߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    ���_�o�b�t�@��������
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

	// �I�t�Z�b�g�̒��������߂�
	float fLength = 0.0f;
	float fAngle = 0.0f;
	fLength = sqrtf((GetWidth() * GetWidth()) + (GetHeight() * GetHeight()));

	// �I�t�Z�b�g�̊p�x�����߂�
	fAngle = atan2f(GetWidth(), GetHeight());

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-sinf(fAngle) * fLength, 0.0f, cosf(fAngle) * fLength);
	pVtx[1].pos = D3DXVECTOR3(sinf(fAngle) * fLength, 0.0f, cosf(fAngle) * fLength);
	pVtx[2].pos = D3DXVECTOR3(-sinf(fAngle) * fLength, 0.0f, -cosf(fAngle) * fLength);
	pVtx[3].pos = D3DXVECTOR3(sinf(fAngle) * fLength, 0.0f, -cosf(fAngle) * fLength);

	// �@������
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
//    �e�̒��_���N���ɍ��킹�鏈��
//=============================================================================
void CShadow::SetVertexHeight(D3DXVECTOR3 pos, D3DXVECTOR3 *pLandNormal)
{
	// ���W��ݒ�
	SetPos(pos);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@���擾�ł���
		if (pLandNormal != NULL)
		{// ���������m�ۂ���Ă���
			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntVer = 0; nCntVer < 4; nCntVer++)
			{// �e�̒��_�����J��Ԃ�
				float DotX = (pLandNormal->x * pVtx[nCntVer].pos.x);       // X�����̓��ς��v�Z
				float DotZ = (pLandNormal->z * pVtx[nCntVer].pos.z);       // Z�����̓��ς��v�Z
				float Dot = -DotX - DotZ;                                  // X������Z�����̓��ϓ��m������
				pVtx[nCntVer].pos.y = (Dot / pLandNormal->y);              // ���������̂�Y�����̖@���Ŋ���
			}

			// ���_�o�b�t�@���A�����b�N����
			pVtxBuff->Unlock();
		}
	}
}