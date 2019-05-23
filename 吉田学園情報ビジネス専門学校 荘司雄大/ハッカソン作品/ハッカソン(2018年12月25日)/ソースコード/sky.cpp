//*****************************************************************************
//
//     ��̏���[sky.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "sky.h"
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
CSky::CSky(int nPriority, OBJTYPE objType) : CMeshDome(nPriority, objType)
{
	// �e��l�̃N���A
	m_fRotSpeed = 0.0f;    // ��]����X�s�[�h
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSky::~CSky()
{

}

//=============================================================================
//    ��������
//=============================================================================
CSky *CSky::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, float fRotSpeed, int nPriority)
{
	CSky *pSky = NULL;                     // ��N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pSky == NULL)
		{// ����������ɂȂ��Ă���
			pSky = new CSky(nPriority);
			if (pSky != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pSky->Init(pos, rot, col, fRadius, nXBlock, nYBlock, fRotSpeed)))
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

	return pSky;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CSky::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, float fRotSpeed)
{
	// �e��l�̐ݒ�
	SetPos(pos);              // ���W
	SetRot(rot);              // ����
	SetCol(col);              // �F
	SetRadius(fRadius);       // ���a
	SetXBlock(nXBlock);       // ���̕�����
	SetYBlock(nYBlock);       // �c�̕�����
	m_fRotSpeed = fRotSpeed;  // ��]����X�s�[�h

	if (FAILED(CMeshDome::Init()))
	{// �������Ɏ��s����
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSky::Uninit(void)
{
	// ���ʂ̏I������
	CMeshDome::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CSky::Update(void)
{
	D3DXVECTOR3 rot = GetRot();

	rot.y += m_fRotSpeed;
	if (rot.y > D3DX_PI)
	{// �������~�����𒴂���
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// �������~�����𒴂���
		rot.y += D3DX_PI * 2.0f;
	}

	SetRot(rot);
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CSky::Draw(void)
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

			// ���C�e�B���O�̐ݒ���O��
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			pDevice->SetRenderState(D3DRS_LIGHTING, false);

			// ���ʂ̕`�揈��
			CMeshDome::Draw();

			// ���C�e�B���O�̐ݒ�����ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}
}

//=============================================================================
//    �@���v�Z����
//=============================================================================
void CSky::MakeNormal(void)
{

}