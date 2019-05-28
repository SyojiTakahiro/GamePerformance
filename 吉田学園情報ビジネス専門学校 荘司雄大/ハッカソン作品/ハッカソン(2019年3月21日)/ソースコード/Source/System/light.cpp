//*****************************************************************************
//
//     ���C�g�̏���[light.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "system.h"
#include "manager.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#ifdef _DEBUG  // �f�o�b�O���̏���
#define LIGHT_DEBUG_VECTOR_UPDOWN  (0.01f)  // ���C�g�̌��������ǂꂭ�炢��������
#define LIGHT_DEBUG_COLOR_UPDOWN   (0.01f)  // ���C�g�̐F�����ǂꂭ�炢�ω������邩
#endif

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

#ifdef _DEBUG  // �f�o�b�O���̏���
	m_nSelectLight = 0;   // ���ݑI������Ă��郉�C�g�̐F
#endif
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
			m_Light[1].Diffuse = D3DXCOLOR(0.23f, 0.23f, 0.23f, 1.0f);
			m_Light[2].Diffuse = D3DXCOLOR(0.23f, 0.23f, 0.23f, 1.0f);

			// ���C�g�̕����̐ݒ�
			vecDir = D3DXVECTOR3(-0.56f, -0.47f, 1.0f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_Light[0].Direction = vecDir;
			vecDir = D3DXVECTOR3(0.6f, -0.14f, -0.19f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_Light[1].Direction = vecDir;
			vecDir = D3DXVECTOR3(-0.81f, -0.18f, -1.0f);
			D3DXVec3Normalize(&vecDir, &vecDir);
			m_Light[2].Direction = vecDir;

			// ���C�g��ݒ肷��
			for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
			{// ���C�g�̐������J��Ԃ�
				pDevice->SetLight(nCntLight, &m_Light[nCntLight]);
			}

			// �ݒ肵�����C�g��L���ɂ���
			for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
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
#ifdef _DEBUG  // �f�o�b�O���̏���
	if (GetUpdate() == false)
	{// �X�V��~����������
		CInputKeyboard *pKeyboard = CManager::GetKeyboard();
		if (pKeyboard != NULL)
		{// �L�[�{�[�h���擾�ł���
			if (pKeyboard->GetTrigger(DIK_UP))
			{// ��������L�[�������ꂽ
				m_nSelectLight = (m_nSelectLight + 1) % MAX_LIGHT;
			}
			else if (pKeyboard->GetTrigger(DIK_DOWN))
			{// ��󉺕����L�[�������ꂽ
				m_nSelectLight = (m_nSelectLight + (MAX_LIGHT - 1)) % MAX_LIGHT;
			}
			else if (pKeyboard->GetPress(DIK_T))
			{// T�L�[�������ꂽ
				m_Light[m_nSelectLight].Direction.x += LIGHT_DEBUG_VECTOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_G))
			{// G�L�[�������ꂽ
				m_Light[m_nSelectLight].Direction.x -= LIGHT_DEBUG_VECTOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_Y))
			{// Y�L�[�������ꂽ
				m_Light[m_nSelectLight].Direction.y += LIGHT_DEBUG_VECTOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_H))
			{// H�L�[�������ꂽ
				m_Light[m_nSelectLight].Direction.y -= LIGHT_DEBUG_VECTOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_U))
			{// U�L�[�������ꂽ
				m_Light[m_nSelectLight].Direction.z += LIGHT_DEBUG_VECTOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_J))
			{// J�L�[�������ꂽ
				m_Light[m_nSelectLight].Direction.z -= LIGHT_DEBUG_VECTOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_Q))
			{// Q�L�[�������ꂽ
				m_Light[m_nSelectLight].Diffuse.r += LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_A))
			{// A�L�[�������ꂽ
				m_Light[m_nSelectLight].Diffuse.r -= LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_W))
			{// W�L�[�������ꂽ
				m_Light[m_nSelectLight].Diffuse.g += LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_S))
			{// S�L�[�������ꂽ
				m_Light[m_nSelectLight].Diffuse.g -= LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_E))
			{// E�L�[�������ꂽ
				m_Light[m_nSelectLight].Diffuse.b += LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_D))
			{// D�L�[�������ꂽ
				m_Light[m_nSelectLight].Diffuse.b -= LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_R))
			{// R�L�[�������ꂽ
				m_Light[m_nSelectLight].Diffuse.a += LIGHT_DEBUG_COLOR_UPDOWN;
			}
			else if (pKeyboard->GetPress(DIK_F))
			{// F�L�[�������ꂽ
				m_Light[m_nSelectLight].Diffuse.a -= LIGHT_DEBUG_COLOR_UPDOWN;
			}
		}

		// ���C�g���`�F�b�N
		// ���C�g�̌���
		// X��
		if (m_Light[m_nSelectLight].Direction.x < -1.0f)
		{
			m_Light[m_nSelectLight].Direction.x = -1.0f;
		}
		else if (m_Light[m_nSelectLight].Direction.x > 1.0f)
		{
			m_Light[m_nSelectLight].Direction.x = 1.0f;
		}

		// Y��
		if (m_Light[m_nSelectLight].Direction.y < -1.0f)
		{
			m_Light[m_nSelectLight].Direction.y = -1.0f;
		}
		else if (m_Light[m_nSelectLight].Direction.y > 1.0f)
		{
			m_Light[m_nSelectLight].Direction.y = 1.0f;
		}

		// Z��
		if (m_Light[m_nSelectLight].Direction.z < -1.0f)
		{
			m_Light[m_nSelectLight].Direction.z = -1.0f;
		}
		else if (m_Light[m_nSelectLight].Direction.z > 1.0f)
		{
			m_Light[m_nSelectLight].Direction.z = 1.0f;
		}

		// ���C�g�̐F
		// �ԐF
		if (m_Light[m_nSelectLight].Diffuse.r < 0.0f)
		{
			m_Light[m_nSelectLight].Diffuse.r = 0.0f;
		}
		else if (m_Light[m_nSelectLight].Diffuse.r > 1.0f)
		{
			m_Light[m_nSelectLight].Diffuse.r = 1.0f;
		}

		// �ΐF
		if (m_Light[m_nSelectLight].Diffuse.g < 0.0f)
		{
			m_Light[m_nSelectLight].Diffuse.g = 0.0f;
		}
		else if (m_Light[m_nSelectLight].Diffuse.g > 1.0f)
		{
			m_Light[m_nSelectLight].Diffuse.g = 1.0f;
		}

		// �F
		if (m_Light[m_nSelectLight].Diffuse.b < 0.0f)
		{
			m_Light[m_nSelectLight].Diffuse.b = 0.0f;
		}
		else if (m_Light[m_nSelectLight].Diffuse.b > 1.0f)
		{
			m_Light[m_nSelectLight].Diffuse.b = 1.0f;
		}

		// �����F
		if (m_Light[m_nSelectLight].Diffuse.a < 0.0f)
		{
			m_Light[m_nSelectLight].Diffuse.a = 0.0f;
		}
		else if (m_Light[m_nSelectLight].Diffuse.a > 1.0f)
		{
			m_Light[m_nSelectLight].Diffuse.a = 1.0f;
		}

		// ���C�g��ݒ肷��
		CManager::GetRenderer()->GetDevice()->SetLight(m_nSelectLight, &m_Light[m_nSelectLight]);

		// ���C�g�̏���`��
		CDebugProc::Print("���ݑI������Ă��郉�C�g�ԍ� : %d\n", m_nSelectLight);
		CDebugProc::Print("���C�g�̐F   : ( %.3f %.3f %.3f %.3f )\n", m_Light[m_nSelectLight].Diffuse.r, m_Light[m_nSelectLight].Diffuse.g, m_Light[m_nSelectLight].Diffuse.b, m_Light[m_nSelectLight].Diffuse.a);
		CDebugProc::Print("���C�g�̌��� : ( %.3f %.3f %.3f )\n", m_Light[m_nSelectLight].Direction.x, m_Light[m_nSelectLight].Direction.y, m_Light[m_nSelectLight].Direction.z);
	}
#endif
}