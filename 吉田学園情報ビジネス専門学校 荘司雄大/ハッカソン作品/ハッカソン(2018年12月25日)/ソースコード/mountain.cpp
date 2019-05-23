//*****************************************************************************
//
//     �R�̏���[mountain.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "mountain.h"
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
CMountain::CMountain(int nPriority, OBJTYPE objType) : CMeshCylinder(nPriority, objType)
{
	// �e��l�̃N���A
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CMountain::~CMountain()
{

}

//=============================================================================
//    ��������
//=============================================================================
CMountain *CMountain::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, int nPriority)
{
	CMountain *pMountain = NULL;           // �R�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pMountain == NULL)
		{// ����������ɂȂ��Ă���
			pMountain = new CMountain(nPriority);
			if (pMountain != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pMountain->Init(pos, rot, col, fHeight, fRadius, nXBlock, nYBlock)))
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

	return pMountain;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CMountain::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock)
{
	// �e��l�̐ݒ�
	SetPos(pos);         // ���W
	SetRot(rot);         // ����
	SetCol(col);         // �F
	SetHeight(fHeight);  // ����
	SetRadius(fRadius);  // ���a
	SetXBlock(nXBlock);  // ���̕�����
	SetYBlock(nYBlock);  // �c�̕�����

	if (FAILED(CMeshCylinder::Init()))
	{// �������Ɏ��s����
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CMountain::Uninit(void)
{
	// ���ʂ̏I������
	CMeshCylinder::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CMountain::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CMountain::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	 // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			DWORD Lighting;

			// �A���t�@�e�X�g��L���ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);     // �L����
			pDevice->SetRenderState(D3DRS_ALPHAREF, 200);             // �����x��200
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER); // ���傫�����̂�`�悷��

			// ���C�e�B���O�̐ݒ���O��
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// ���ʂ̕`�揈��
			CMeshCylinder::Draw();

			// ���C�e�B���O�̐ݒ�����ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

			// �A���t�@�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);  // ������
		}
	}
}

//=============================================================================
//    �@���v�Z����
//=============================================================================
void CMountain::MakeNormal(void)
{

}