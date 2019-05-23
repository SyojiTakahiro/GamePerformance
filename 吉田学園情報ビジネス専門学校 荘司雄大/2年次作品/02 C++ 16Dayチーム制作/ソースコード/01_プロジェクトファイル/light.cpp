//*****************************************************************************
//
//     ���C�g�̏���[light.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "light.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CLight::CLight()
{
	// ���C�g���N���A����
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ���C�g�̐������J��Ԃ�
		ZeroMemory(&m_Light[nCntLight], sizeof(D3DLIGHT9));
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CLight::~CLight()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CLight::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			D3DXVECTOR3 vecDir;  // �J�����̖@���ݒ�p

		    // ���C�g�̎�ނ�ݒ�
			m_Light[0].Type = D3DLIGHT_DIRECTIONAL;
			m_Light[1].Type = D3DLIGHT_DIRECTIONAL;
			m_Light[2].Type = D3DLIGHT_DIRECTIONAL;

			// ���C�g�̊g�U����ݒ�
			m_Light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			m_Light[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			m_Light[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

			// ���C�g�̕����̐ݒ�
			vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_Light[0].Direction = vecDir;
			vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_Light[1].Direction = vecDir;
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_Light[2].Direction = vecDir;

			// ���C�g��ݒ肷��
			for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
			{// ���C�g�̐������J��Ԃ�
				pDevice->SetLight(nCntLight, &m_Light[nCntLight]);
			}

			// �ݒ肵�����C�g��L���ɂ���
			for (int nCntLight = 0; nCntLight < 1; nCntLight++)
			{// ���C�g�̐������J��Ԃ�
				pDevice->LightEnable(nCntLight, TRUE);
			}
		}
	}
	else
	{// �����_�����O�N���X���擾�ł��Ȃ�����
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CLight::Uninit(void)
{
	// ���C�g���N���A����
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ���C�g�̐������J��Ԃ�
		ZeroMemory(&m_Light[nCntLight], sizeof(D3DLIGHT9));
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CLight::Update(void)
{

}