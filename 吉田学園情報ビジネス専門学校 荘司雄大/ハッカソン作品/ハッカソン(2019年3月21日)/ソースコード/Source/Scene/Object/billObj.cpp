//*****************************************************************************
//
//     �z�u�p�r���{�[�h�̏���[billObj.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "object.h"
#include "manager.h"
#include "functionlib.h"
#include "system.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBillboardObject::CBillboardObject(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// �e��l�̃N���A
	m_nType = TYPE_NONE;     // ��ޔԍ�
	m_nTexIdx = 0;           // �e�N�X�`���ԍ�
	m_bDrawAddtive = false;  // ���Z�������邩���Ȃ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBillboardObject::~CBillboardObject()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBillboardObject *CBillboardObject::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, int nType, int nPriority)
{
	CBillboardObject *pBillboardObject = NULL;      // �z�u�r���{�[�h�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();            // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pBillboardObject == NULL)
		{// ����������ɂȂ��Ă���
			pBillboardObject = new CBillboardObject(nPriority);
			if (pBillboardObject != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pBillboardObject->Init(pos, col, fWidth, fHeight, fRot, bLighting, bDrawAddtive, nTexIdx, nType)))
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

	return pBillboardObject;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBillboardObject::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, int nType)
{
	// �e��l�̑��
	SetPos(pos);                    // �|���S���̍��W
	SetCol(col);                    // �|���S���̐F
	SetRot(fRot);                   // �|���S���̌���
	SetWidth(fWidth);               // �|���S���̕�
	SetHeight(fHeight);             // �|���S���̍���
	SetLighting(bLighting);         // ���C�e�B���O���邩���Ȃ���
	SetDrawAddtive(bDrawAddtive);   // ���Z�����ŕ`�悷�邩���Ȃ���
	SetTexIdx(nTexIdx);             // �g�p���Ă���e�N�X�`���̔ԍ�
	SetType(nType);                 // ��ޔԍ�

	// ���ʂ̏���������
	if (FAILED(CSceneBillboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBillboardObject::Uninit(void)
{
	// ���ʂ̏I������
	CSceneBillboard::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBillboardObject::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBillboardObject::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			if (m_bDrawAddtive == true)
			{// ���u�����f�B���O�����Z�����ɐݒ�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// �A���t�@�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // �L����
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);               // �����x��0
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ���傫�����̂�`�悷��

			// ���ʂ̕`�揈��
			CSceneBillboard::Draw();

			// �A���t�@�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

			// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//=============================================================================
//    ���[���h�}�g���b�N�X�v�Z����
//=============================================================================
void CBillboardObject::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxWorld, mtxView, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �J�����̌������擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// ���[���h�}�g���b�N�X�Ɍ�����ݒ�(�J�����̋t�s������邱�ƂŃJ�����̐��ʂɌ�����␳)
	mtxWorld._11 = mtxView._11;
	mtxWorld._12 = mtxView._21;
	mtxWorld._13 = mtxView._31;
	//mtxWorld._21 = mtxView._12;
	//mtxWorld._22 = mtxView._22;
	//mtxWorld._23 = mtxView._32;
	mtxWorld._31 = mtxView._13;
	mtxWorld._32 = mtxView._23;
	mtxWorld._33 = mtxView._33;

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
	CSceneBillboard::SetMtxWorld(mtxWorld);
}

//=============================================================================
//    ��ޔԍ��ݒ菈��
//=============================================================================
void CBillboardObject::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    �g�p���Ă���e�N�X�`���̔ԍ��ݒ菈��
//=============================================================================
void CBillboardObject::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    ���Z�����ŕ`�悷�邩���Ȃ����ݒ菈��
//=============================================================================
void CBillboardObject::SetDrawAddtive(const bool bDrawAddtive)
{
	m_bDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    ��ޔԍ��擾����
//=============================================================================
int CBillboardObject::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    �g�p���Ă���e�N�X�`���̔ԍ��擾����
//=============================================================================
int CBillboardObject::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    ���Z�����ŕ`�悷�邩���Ȃ����擾����
//=============================================================================
bool CBillboardObject::GetDrawAddtive(void)
{
	return m_bDrawAddtive;
}