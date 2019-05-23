//*****************************************************************************
//
//     �O�ՃG�t�F�N�g�̏���[orbitEffect.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "orbitEffect.h"
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
COrbitEffect::COrbitEffect(int nPriority, OBJTYPE objType) : CMeshOrbit(nPriority, objType)
{
	// �e��l�̃N���A
	m_fAlphaDecayUp = 0.0f;                          // ���_�̏㑤�̓����x�����l
 	m_fAlphaDecayDown = 0.0f;                        // ���_�̉����̓����x�����l
	m_ColUp = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);     // ���_�㑤�̐F
	m_ColDown = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // ���_�����̐F
	m_OffsetAmp[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �I�t�Z�b�g�̑����l(�P��)
	m_OffsetAmp[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �I�t�Z�b�g�̑����l(�Q��)
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
COrbitEffect::~COrbitEffect()
{

}

//=============================================================================
//    ��������
//=============================================================================
COrbitEffect *COrbitEffect::Create(D3DXVECTOR3 OffsetPos1, D3DXVECTOR3 OffsetPos2, D3DXVECTOR3 OffsetAmp1, D3DXVECTOR3 OffsetAmp2,
	D3DXCOLOR ColUp, D3DXCOLOR ColDown, float fAlphaDecayUp, float fAlphaDecayDown, int nXBlock, int nZBlock, D3DXMATRIX *pMtxParent, int nPriority)
{
	COrbitEffect *pOrbitEffect = NULL;     // �O�ՃG�t�F�N�g�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pOrbitEffect == NULL)
		{// ����������ɂȂ��Ă���
			pOrbitEffect = new COrbitEffect(nPriority);
			if (pOrbitEffect != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pOrbitEffect->SetOffsetPos(OffsetPos1, 0);
				pOrbitEffect->SetOffsetPos(OffsetPos2, 1);
				pOrbitEffect->SetOffsetAmp1(OffsetAmp1);
				pOrbitEffect->SetOffsetAmp2(OffsetAmp2);
				pOrbitEffect->SetColUp(ColUp);
				pOrbitEffect->SetColDown(ColDown);
				pOrbitEffect->SetAlphaDecayUp(fAlphaDecayUp);
				pOrbitEffect->SetAlphaDecayDown(fAlphaDecayDown);
				pOrbitEffect->SetXBlock(nXBlock);
				pOrbitEffect->SetZBlock(nZBlock);
				pOrbitEffect->SetMtxParent(pMtxParent);

				if (FAILED(pOrbitEffect->Init()))
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

	return pOrbitEffect;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT COrbitEffect::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CMeshOrbit::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void COrbitEffect::Uninit(void)
{
	// ���ʂ̏I������
	CMeshOrbit::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void COrbitEffect::Update(void)
{
	// ���ʂ̍X�V����
	CMeshOrbit::Update();

	D3DXVECTOR3 offset;
	for (int nCntOffset = 0; nCntOffset < 2; nCntOffset++)
	{// �c�̕��������J��Ԃ�
		offset = GetOffsetPos(nCntOffset);
		offset += m_OffsetAmp[nCntOffset];
		SetOffsetPos(offset, nCntOffset);
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void COrbitEffect::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// ���u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			// ���ʂ̕`�揈��
			CMeshOrbit::Draw();

			// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void COrbitEffect::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * GetNumVertex(),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W���v�Z���Ă���
	D3DXVECTOR3 VertexPos[2] = {};
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
		D3DXVec3TransformCoord(&VertexPos[nCntVer], &GetOffsetPos(nCntVer), GetMtxParent());
	}

	// ���_�ݒ�
	int nCntUp = 0;
	int nCntDown = 0;
	float fAlpha = 0.0f;
	for (int nCntVer = 0; nCntVer < GetNumVertex(); nCntVer++)
	{// ���_�����J��Ԃ�
	    // ���_���W
		SetVertexPos(VertexPos[(nCntVer % 2)], nCntVer);
		pVtx[nCntVer].pos = GetVertexPos()[nCntVer];

		// ���_�J���[
		if (nCntVer % 2 == 0)
		{// �����̎�
			fAlpha = m_ColUp.a - (nCntUp * m_fAlphaDecayUp);
			if (fAlpha <= 0.0f)
			{
				fAlpha = 0.0f;
			}
			SetVertexCol(D3DXCOLOR(m_ColUp.r, m_ColUp.g, m_ColUp.b, fAlpha), nCntVer);
			nCntUp++;
		}
		else
		{// ��̎�
			fAlpha = m_ColDown.a - (nCntDown * m_fAlphaDecayDown);
			if (fAlpha <= 0.0f)
			{
				fAlpha = 0.0f;
			}
			SetVertexCol(D3DXCOLOR(m_ColDown.r, m_ColDown.g, m_ColDown.b, fAlpha), nCntVer);
			nCntDown++;
		}
		pVtx[nCntVer].col = GetVertexCol()[nCntVer];

		// �e�N�X�`�����W
		pVtx[nCntVer].tex = D3DXVECTOR2(0.0f + (nCntVer / 2) * (1.0f / (float)GetXBlock()), 0.0f + (nCntVer % 2) * 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    ���_�㑤�̐F��ݒ�
//=============================================================================
void COrbitEffect::SetColUp(const D3DXCOLOR ColUp)
{
	m_ColUp = ColUp;
}

//=============================================================================
//    ���_�����̐F��ݒ�
//=============================================================================
void COrbitEffect::SetColDown(const D3DXCOLOR ColDown)
{
	m_ColDown = ColDown;
}

//=============================================================================
//    ���_�㑤�̓����x�̌����l��ݒ�
//=============================================================================
void COrbitEffect::SetAlphaDecayUp(const float fAlphaDecayUp)
{
	m_fAlphaDecayUp = fAlphaDecayUp;
}

//=============================================================================
//    ���_�����̓����x�̌����l��ݒ�
//=============================================================================
void COrbitEffect::SetAlphaDecayDown(const float fAlphaDecayDown)
{
	m_fAlphaDecayDown = fAlphaDecayDown;
}

//=============================================================================
//    �I�t�Z�b�g�̑����l(�P��)��ݒ�
//=============================================================================
void COrbitEffect::SetOffsetAmp1(const D3DXVECTOR3 OffsetAmp)
{
	m_OffsetAmp[0] = OffsetAmp;
}

//=============================================================================
//    �I�t�Z�b�g�̑����l(�Q��)��ݒ�
//=============================================================================
void COrbitEffect::SetOffsetAmp2(const D3DXVECTOR3 OffsetAmp)
{
	m_OffsetAmp[1] = OffsetAmp;
}

//=============================================================================
//    ���_�㑤�̐F���擾
//=============================================================================
D3DXCOLOR COrbitEffect::GetColUp(void)
{
	return m_ColUp;
}

//=============================================================================
//    ���_�����̐F���擾
//=============================================================================
D3DXCOLOR COrbitEffect::GetColDown(void)
{
	return m_ColDown;
}

//=============================================================================
//    ���_�㑤�̓����x�̌����l���擾
//=============================================================================
float COrbitEffect::GetAlphaDecayUp(void)
{
	return m_fAlphaDecayUp;
}

//=============================================================================
//    ���_�����̓����x�̌����l���擾
//=============================================================================
float COrbitEffect::GetAlphaDecayDown(void)
{
	return m_fAlphaDecayDown;
}

//=============================================================================
//    �I�t�Z�b�g�̑����l(�P��)���擾
//=============================================================================
D3DXVECTOR3 COrbitEffect::GetOffsetAmp1(void)
{
	return m_OffsetAmp[0];
}

//=============================================================================
//    �I�t�Z�b�g�̑����l(�Q��)���擾
//=============================================================================
D3DXVECTOR3 COrbitEffect::GetOffsetAmp2(void)
{
	return m_OffsetAmp[1];
}