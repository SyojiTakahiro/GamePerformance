//*****************************************************************************
//
//     �J�����̏���[camera.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "system.h"
#include "manager.h"
#include "mode.h"
#include "character.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
// ����p�J�����̃}�N��
#define CAMERA_MOVIE_MODE            (false)                 // ����p�̃J�����ɐ؂�ւ���(�����ő���ł���J�����ɂ���)
#define CAMERA_MOVIE_MOVE            (6.0f)                  // ����p�J�����̈ړ���

// �J�����N�����}�N��
#define CAMERA_ANGEL_VIEW            (30.0f)                 // �J�����̉�p
#define CAMERA_ZNER                  (10.0f)                 // �J�����̈�ԋ߂�������
#define CAMERA_ZFAR                  (25000.0f)              // �J�����̈�ԉ���������
#define CAMERA_ANGLE_SPEED           (0.01f)                 // �J�������}�E�X�ŉ�]������X�s�[�h�{��
#define CAMERA_MOVE_SPEED            (1.3f)                  // �J�������}�E�X�ňړ�������X�s�[�h�{��
#define CAMERA_LENGTH_MIN            (40.0f)                 // ���_�����_�̋����̍ŏ��l
#define CAMERA_LENGTH_MAX            (400.0f)                // ���_�����_�̋����̍ő�l
#define CAMERA_LENGTH_INI            (200.0f)                // �J���������̏����l
#define CAMERA_POSY_UP               (30.0f)                 // �J�����̍������グ��␳�l
#define CAMERA_POSY_RANGE            (300.0f)                // �J����Y���W�͈̔�
#define CAMERA_POSY_MIN              (50.0f)                 // �J����Y���W�̍ŏ��l

// �Q�[�����̃J�����p�}�N��
#define CAMERA_ANGLEY_MIN            (-0.8f)                 // �J�����c�ړ��̊p�x�ŏ��l
#define CAMERA_ANGLEY_MAX            (D3DX_PI * 0.5f)        // �J�����c�ړ��̊p�x�ő�l
#define CAMERA_MOVESPEED_XZ          (0.8f)                  // �J�����𓮂��������̈ړ��X�s�[�h(XZ����)
#define CAMERA_MOVESPEED_Y           (0.8f)                  // �J�����𓮂��������̈ړ��X�s�[�h(�c����)
#define CAMERA_ROLLSPEED_Z           (0.01f)                 // �J�����̌����𓮂��������̈ړ��X�s�[�h(Z��)
#define CAMERA_ROLLSPEED_Y           (0.025f)                // �J�����̌����𓮂��������̈ړ��X�s�[�h(Y��)
#define CAMERA_ROT_RIVISION_X        (0.9f)                 // �J�����̌�����␳����{��(X��)
#define CAMERA_ROT_RIVISION_Y        (0.9f)                 // �J�����̌�����␳����{��(Y��)
#define CAMERA_ROT_RIVISION_Z        (0.9f)                 // �J�����̌�����␳����{��(Z��)

// �ړ��{���}�N��
#define CAMERA_ANGLE_MOVERIVISION_X  (0.00003f)             // �J�������Q�[���p�b�h�œ��������Ƃ��̉��ړ��̔{��
#define CAMERA_ANGLE_MOVERIVISION_Y  (0.00003f)             // �J�������Q�[���p�b�h�œ��������Ƃ��̏c�ړ��̔{��
#define CAMERA_GAME_LENGTH	         (230.0f)                // �Q�[�����̃J�����̋���
#define CAMERA_LOCKON_LENGTH         (230.0f)                // ���b�N�I�����̃Q�[�����̃J�����̋���

// �^�C�g���̃J�����p�}�N��
#define CAMERA_TITLE_LENGTH_0         (300.0f)               // �^�C�g���̎��_�����_�Ԃ̋���
#define CAMERA_TITLE_POSYUP_0         (70.0f)                // �^�C�g���̎��_�����_��(Y����)�̍�
#define CAMERA_TITLEPOSR_MOVESPEED_XZ (0.003f)               // �J�����̒����_���^�C�g�����ɓ����������̈ړ��X�s�[�h(XZ����)
#define CAMERA_TITLEPOSR_MOVESPEED_Y  (0.003f)               // �J�����̒����_���^�C�g�����ɓ����������̈ړ��X�s�[�h(�c����)

//*****************************************************************************
//    CCamera�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CCamera::CCamera()
{
	// �e��l�̃N���A
	m_pPlayer = NULL;                           // �v���C���[�N���X�ւ̃|�C���^
	m_PosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ���݂̎��_
	m_PosVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړI�̒����_
	m_PosVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ���݂̎��_�ƖړI�̒����_�̍���
	m_PosVOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �O��̎n�_
	m_PosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ���݂̒����_
	m_PosRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // �ړI�̒����_
	m_PosRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f); // ���݂̒����_�ƖړI�̒����_�̍���
	m_MoveV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���_�̈ړ���
	m_MoveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �����_�̈ړ���
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���݂̌���
	m_RotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �ړI�̌���
	m_RotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �ړI�̌����ƌ��݂̌����̍���
	m_RotOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �O��̌���
	m_VecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ������x�N�g��
	D3DXMatrixIdentity(&m_MtxProjection);       // �v���W�F�N�V�����}�g���b�N�X
	D3DXMatrixIdentity(&m_MtxView);             // �r���[�}�g���b�N�X
	m_fLength = 0.0f;                           // ����
	m_bMoving = false;                          // �J�����𓮂����邩�ǂ���
	m_ViewPort.X = 0;                           // �`��̈捶��X���W
	m_ViewPort.Y = 0;                           // �`��̈捶��Y���W
	m_ViewPort.Width = 0;                       // �`��̈�̕�
	m_ViewPort.Height = 0;                      // �`��̈�̍���
	m_ViewPort.MinZ = 0.0f;                     // �`��̈�[�x�o�b�t�@�ŏ��l
	m_ViewPort.MaxZ = 0.0f;                     // �`��̈�[�x�o�b�t�@�ő�l
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
//    ��������
//=============================================================================
CCamera *CCamera::Create(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot, float fLength, D3DVIEWPORT9 ViewPort, CPlayer *pPlayer)
{
	CCamera *pCamera = NULL;  // �J�����N���X�^�̃|�C���^
	if (pCamera == NULL)
	{// ����������ɂȂ��Ă���
		pCamera = new CCamera;
		if (pCamera != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pCamera->Init(posV, posR, rot, fLength, ViewPort, pPlayer)))
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

	return pCamera;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CCamera::Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot, float fLength, D3DVIEWPORT9 ViewPort, CPlayer *pPlayer)
{
	// �e��l�̏�����
	m_PosV = posV;                           // ���_
	m_PosR = posR;                           // �����_
	m_Rot = rot;                             // ����
	m_VecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);  // ������x�N�g��
	m_bMoving = CAMERA_MOVIE_MODE;           // �J�����������ő���ł��邩�ǂ���

	// ���_�ƒ����_�Ƃ̋������v�Z
	m_fLength = fLength;
	m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
	m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
	m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;

	// �r���[�|�[�g�ݒ�
	m_ViewPort = ViewPort;

	// �ǔ��Ώۂ�ݒ�
	m_pPlayer = pPlayer;
	if (m_pPlayer != NULL)
	{
		m_pPlayer->SetCamera(this);
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CCamera::Uninit(void)
{

}

//=============================================================================
//    �X�V����
//=============================================================================
void CCamera::Update(void)
{

#if _DEBUG  // �f�o�b�O���̏���
	if (GetUpdate() == false)
	{// �X�V����������
		CInputMouse *pMouse = CManager::GetMouse();

		if (pMouse != NULL)
		{// �}�E�X���擾�ł���
			if (pMouse->GetMouseAxisZ() != 0)
			{// �}�E�X�̃z�C�[���{�^�����X�N���[�����ꂽ
				m_fLength += 0.1f * pMouse->GetMouseAxisZ();
				if (m_fLength < CAMERA_LENGTH_MIN)
				{// ���������̒l���߂��Ȃ���
					m_fLength = CAMERA_LENGTH_MIN;
				}
				else if (m_fLength > CAMERA_LENGTH_MAX)
				{// ���������̒l��艓���Ȃ���
					m_fLength = CAMERA_LENGTH_MAX;
				}

				// ���_�̌v�Z
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}

			if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE && pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
			{// �}�E�X�̍��{�^���ƉE�{�^���������ɉ����ꂽ
				if (pMouse->GetMouseAxisX() != 0 || pMouse->GetMouseAxisY() != 0)
				{// �}�E�X����������Ă���
					// �����ɉ����ꂽ�Ƃ��̈ʒu���獡�̃}�E�X�J�[�\���̈ʒu���王�_�����_���ړ�������������v�Z
					float fAngle = atan2f(((float)pMouse->GetPoint().x - (float)pMouse->GetPointOld().x), ((float)pMouse->GetPoint().y - (float)pMouse->GetPointOld().y));
					fAngle -= m_Rot.y;
					if (fAngle < -D3DX_PI)
					{// �������~�����𒴂���
						fAngle += D3DX_PI * 2.0f;
					}
					if (fAngle > D3DX_PI)
					{// �������~�����𒴂���
						fAngle -= D3DX_PI * 2.0f;
					}

					// �}�E�X��X��,Y������ړ��ʂ��v�Z
					float fMovement = sqrtf((float)pMouse->GetMouseAxisX() * (float)pMouse->GetMouseAxisX() + (float)pMouse->GetMouseAxisY() * (float)pMouse->GetMouseAxisY());
					fMovement *= CAMERA_MOVE_SPEED;

					// ���_���ړ�������
					m_PosR.x += sinf(fAngle) * fMovement;
					m_PosR.z -= cosf(fAngle) * fMovement;

					// �����_���ړ�������
					m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
					m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
				}
			}
			else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE)
			{// �}�E�X�̍��N���b�N�݂̂������ꂽ
				// �������}�E�X�̑��x�ɍ��킹�ĉ�]������
				// �c��
				m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
				if (m_Rot.y > D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.y -= D3DX_PI * 2.0f;
				}
				if (m_Rot.y < -D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.y += D3DX_PI * 2.0f;
				}

				// ����
				m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
				if (m_Rot.z < -D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.z = -D3DX_PI;
				}
				if (m_Rot.z > 0.0f)
				{// �������~�����𒴂���
					m_Rot.z = 0.0f;
				}

				// ���_�̈ʒu���v�Z����
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}
			else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
			{// �}�E�X�̉E�N���b�N�݂̂������ꂽ
				// �������}�E�X�̑��x�ɍ��킹�ĉ�]������
				// �c��
				m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
				if (m_Rot.y > D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.y -= D3DX_PI * 2.0f;
				}
				if (m_Rot.y < -D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.y += D3DX_PI * 2.0f;
				}

				// ����
				m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
				if (m_Rot.z < -D3DX_PI)
				{// �������~�����𒴂���
					m_Rot.z = -D3DX_PI;
				}
				if (m_Rot.z > 0.0f)
				{// �������~�����𒴂���
					m_Rot.z = 0.0f;
				}

				// �����_�̈ʒu���v�Z����
				m_PosR.x = m_PosV.x + sinf(m_Rot.y) * m_fLength;
				m_PosR.y = m_PosV.y - cosf(m_Rot.z) * m_fLength;
				m_PosR.z = m_PosV.z + cosf(m_Rot.y) * m_fLength;
			}
		}
		// ������@�\��
		CDebugProc::Print("\n( �J�����̑�����@ )\n");
		CDebugProc::Print("[ �J�����̎��_�ړ�   ]   : �}�E�X���N���b�N\n");
		CDebugProc::Print("[ �J�����̒����_�ړ� ]   : �}�E�X�E�N���b�N\n");
		CDebugProc::Print("[ �J�����̈ړ�       ]   : �}�E�X���E�����N���b�N\n");
		CDebugProc::Print("[ �J�����̊g��k��   ]   : �}�E�X�z�C�[��\n");
	}
	else
	{// �X�V���Ȃ��
		// �O�����ۑ�
		m_PosVOld = m_PosV;   // ���_
		m_RotOld = m_Rot;     // ����

		if (CManager::GetMode() == CManager::MODE_TITLE)
		{// �^�C�g����ʂȂ��
			Title();
		}
		else if (CManager::GetMode() == CManager::MODE_VIEW_MAP)
		{// �}�b�v�Љ��ʂȂ��
			ViewMap();
		}
		else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
		{// �`���[�g���A����ʂȂ��
			Tutorial();
		}
		else if (CManager::GetMode() == CManager::MODE_SETTING)
		{// �Z�b�e�B���O��ʂȂ��
			Setting();
		}
		else if (CManager::GetMode() == CManager::MODE_GAME)
		{// �Q�[����ʂȂ��
			Game();
		}
		else if (CManager::GetMode() == CManager::MODE_RESULT)
		{// ���U���g��ʂȂ��
			Result();
		}
	}

	CDebugProc::Print("\n�J�����̎��_   : ( %.1f %.1f %.1f )\n", m_PosV.x, m_PosV.y, m_PosV.z);
	CDebugProc::Print("�J�����̒����_ : ( %.1f %.1f %.1f )\n", m_PosR.x, m_PosR.y, m_PosR.z);
	CDebugProc::Print("�J�����̌���   : ( %.2f %.2f %.2f )\n\n", m_Rot.x, m_Rot.y, m_Rot.z);

#else
if (CManager::GetMode() == CManager::MODE_TITLE)
{// �^�C�g����ʂȂ��
	Title();
}
else if (CManager::GetMode() == CManager::MODE_VIEW_MAP)
{// �}�b�v�Љ��ʂȂ��
	ViewMap();
}
else if (CManager::GetMode() == CManager::MODE_TUTORIAL)
{// �`���[�g���A����ʂȂ��
	Tutorial();
}
else if (CManager::GetMode() == CManager::MODE_SETTING)
{// �Z�b�e�B���O��ʂȂ��
	Setting();
}
else if (CManager::GetMode() == CManager::MODE_GAME)
{// �Q�[����ʂȂ��
	Game();
}
else if (CManager::GetMode() == CManager::MODE_RESULT)
{// ���U���g��ʂȂ��
	Result();
}
#endif
}

//=============================================================================
//    �J�����̐ݒ�
//=============================================================================
void CCamera::SetCamera(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();	// �f�o�C�X�̎擾
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// �r���[�|�[�g(�`��̈�)�̐ݒ�
			pDevice->SetViewport(&m_ViewPort);

		    // �v���W�F�N�V�����}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_MtxProjection);

			// �v���W�F�N�V�����}�g���b�N�X���쐬
			D3DXMatrixPerspectiveFovLH(&m_MtxProjection,
				D3DXToRadian(CAMERA_ANGEL_VIEW),
				(float)m_ViewPort.Width / (float)m_ViewPort.Height,
				CAMERA_ZNER,
				CAMERA_ZFAR);

			// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_PROJECTION, &m_MtxProjection);

			// �r���[�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_MtxView);

			// �r���[�}�g���b�N�X���쐬
			D3DXMatrixLookAtLH(&m_MtxView,
				&m_PosV,
				&m_PosR,
				&m_VecU);

			// �r���[�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_VIEW, &m_MtxView);
		}
	}
}

//=============================================================================
//    �^�C�g����ʂ̃J��������
//=============================================================================
void CCamera::Title(void)
{

}

//=============================================================================
//    �}�b�v�Љ��ʂ̃J��������
//=============================================================================
void CCamera::ViewMap(void)
{

}

//=============================================================================
//    �`���[�g���A����ʂ̃J��������
//=============================================================================
void CCamera::Tutorial(void)
{

}

//=============================================================================
//    �Z�b�e�B���O��ʂ̃J��������
//=============================================================================
void CCamera::Setting(void)
{

}

//=============================================================================
//    �Q�[����ʂ̃J��������
//=============================================================================
void CCamera::Game(void)
{
	// �O��̎��_���W��ۑ�
	m_PosVOld = m_PosV;

	//  �J�����̋�����ݒ�
	m_fLength = CAMERA_GAME_LENGTH;
	//if (m_pPlayer != NULL)
	//{// �v���C���[�N���X���擾�ł���
	//    // �W���C�p�b�h���擾
	//	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(m_pPlayer->GetControllerIdx());
	//	if (pJoyStick != NULL)
	//	{// �W���C�X�e�B�b�N���擾�ł���
	//		if (pJoyStick->GetRightAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetRightAxisX() <= -DI_STICK_AXIS_MIN
	//			|| pJoyStick->GetRightAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetRightAxisY() <= -DI_STICK_AXIS_MIN)
	//		{// �E�A�i���O���͂�����Ă���
	//			float RightAxisX = pJoyStick->GetRightAxisX();
	//			float RightAxisY = pJoyStick->GetRightAxisY();

	//			// �ړI�̊p�x�ύX
	//			m_RotDest.y += RightAxisX * CAMERA_ANGLE_MOVERIVISION_X;
	//			m_RotDest.z += RightAxisY * CAMERA_ANGLE_MOVERIVISION_Y;

	//			if (m_RotDest.y > D3DX_PI)
	//			{// �������~�����𒴂���
	//				m_RotDest.y -= D3DX_PI * 2.0f;
	//			}
	//			if (m_RotDest.y < -D3DX_PI)
	//			{// �������~�����𒴂���
	//				m_RotDest.y += D3DX_PI * 2.0f;
	//			}

	//			if (m_RotDest.z < CAMERA_ANGLEY_MIN)
	//			{// �������ő�l�𒴂���
	//				m_RotDest.z = CAMERA_ANGLEY_MIN;
	//			}
	//			if (m_RotDest.z > CAMERA_ANGLEY_MAX)
	//			{// �������ŏ��l�𒴂���
	//				m_RotDest.z = CAMERA_ANGLEY_MAX;
	//			}
	//		}
	//	}
	//	if (CManager::GetKeyboard()->GetPress(DIK_LEFT) == true)
	//	{
	//		m_RotDest.y -= CAMERA_ROLLSPEED_Y;
	//		if (m_RotDest.y > D3DX_PI)
	//		{// �������~�����𒴂���
	//			m_RotDest.y -= D3DX_PI * 2.0f;
	//		}
	//		if (m_RotDest.y < -D3DX_PI)
	//		{// �������~�����𒴂���
	//			m_RotDest.y += D3DX_PI * 2.0f;
	//		}
	//	}
	//	else if (CManager::GetKeyboard()->GetPress(DIK_RIGHT) == true)
	//	{
	//		m_RotDest.y += CAMERA_ROLLSPEED_Y;
	//		if (m_RotDest.y > D3DX_PI)
	//		{// �������~�����𒴂���
	//			m_RotDest.y -= D3DX_PI * 2.0f;
	//		}
	//		if (m_RotDest.y < -D3DX_PI)
	//		{// �������~�����𒴂���
	//			m_RotDest.y += D3DX_PI * 2.0f;
	//		}
	//	}
	//	if (CManager::GetKeyboard()->GetPress(DIK_UP) == true)
	//	{
	//		m_RotDest.z -= CAMERA_ROLLSPEED_Z;
	//		if (m_RotDest.z < CAMERA_ANGLEY_MIN)
	//		{// �������ő�l�𒴂���
	//			m_RotDest.z = CAMERA_ANGLEY_MIN;
	//		}
	//		if (m_RotDest.z > CAMERA_ANGLEY_MAX)
	//		{// �������ŏ��l�𒴂���
	//			m_RotDest.z = CAMERA_ANGLEY_MAX;
	//		}
	//	}
	//	else if (CManager::GetKeyboard()->GetPress(DIK_DOWN) == true)
	//	{
	//		m_RotDest.z += CAMERA_ROLLSPEED_Z;
	//		if (m_RotDest.z < CAMERA_ANGLEY_MIN)
	//		{// �������ő�l�𒴂���
	//			m_RotDest.z = CAMERA_ANGLEY_MIN;
	//		}
	//		if (m_RotDest.z > CAMERA_ANGLEY_MAX)
	//		{// �������ŏ��l�𒴂���
	//			m_RotDest.z = CAMERA_ANGLEY_MAX;
	//		}
	//	}
	//}

	if (m_pPlayer != NULL)
	{// �v���C���[�N���X���擾�ł���
	    // �v���C���[�̏���ۑ�
		D3DXVECTOR3 PlayerPos = m_pPlayer->GetPos();
		D3DXVECTOR3 PlayerRot = m_pPlayer->GetRot();

		// �ړI�̌�����ݒ�
		m_RotDest.y = PlayerRot.y - D3DX_PI;

		// �ړI�̒����_�̍��W��ݒ�
		// �����̌v�Z
		CalcRot();

		// �����_���v���C���[�̈ʒu�ɂ���
		m_PosRDest.x = PlayerPos.x - sinf(m_Rot.y + D3DX_PI) * m_fLength;
		m_PosRDest.y = PlayerPos.y + sinf(m_Rot.z + D3DX_PI) * m_fLength;
		m_PosRDest.z = PlayerPos.z - cosf(m_Rot.y + D3DX_PI) * m_fLength;

		// �����_�������グ��
		m_PosRDest.y = CAMERA_POSY_UP + 20.0f;

		// ���_�̖ړI�ʒu���v�Z����
		m_PosVDest.x = PlayerPos.x - sinf(m_Rot.y) * 300.0f;
		m_PosVDest.y = PlayerPos.y - sinf(m_Rot.z + D3DX_PI) * 30.0f + 130.0f;
		m_PosVDest.z = PlayerPos.z - cosf(m_Rot.y) * 300.0f;

		// ���݂̎��_�ƖړI�̎��_�̍������v�Z
		m_PosVDiff = m_PosVDest - m_PosV;

		// ���݂̒����_�ƖړI�̒����_�̍������v�Z
		m_PosRDiff = m_PosRDest - m_PosR;

		// ���݂̎��_��ړI�̎��_�ɋ߂Â���
		m_PosV.x += m_PosVDiff.x * CAMERA_MOVESPEED_XZ;
		m_PosV.y += m_PosVDiff.y * CAMERA_MOVESPEED_Y;
		m_PosV.z += m_PosVDiff.z * CAMERA_MOVESPEED_XZ;

		// ���݂̒����_��ړI�̒����_�ɋ߂Â���
		m_PosR.x += m_PosRDiff.x * CAMERA_MOVESPEED_XZ;
		m_PosR.y += m_PosRDiff.y * CAMERA_MOVESPEED_Y;
		m_PosR.z += m_PosRDiff.z * CAMERA_MOVESPEED_XZ;
	}
}

//=============================================================================
//    ���U���g��ʂ̃J��������
//=============================================================================
void CCamera::Result(void)
{

}

//=============================================================================
//    �J�����̓�������(����p)
//=============================================================================
void CCamera::MovieCamera(void)
{
	// �L�[�{�[�h���擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard->GetPress(DIK_J) == true)
	{// �������̓��͂����ꂽ
		if (pKeyboard->GetPress(DIK_I) == true)
		{// �����ɏ�����̓��͂����ꂽ
		    // �ړ�����
			m_PosR.x += sinf(m_Rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z += cosf(m_Rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVIE_MOVE;

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else if (pKeyboard->GetPress(DIK_K) == true)
		{// �����ɉ������̓��͂����ꂽ
		    // �ړ�����
			m_PosR.x += sinf(m_Rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z += cosf(m_Rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVIE_MOVE;

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else
		{// ����������ĂȂ�
		    // �ړ�����
			m_PosR.x += sinf(m_Rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z += cosf(m_Rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVIE_MOVE;

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
	}
	else if (pKeyboard->GetPress(DIK_L) == true)
	{// �E�����̓��͂����ꂽ
		if (pKeyboard->GetPress(DIK_I) == true)
		{// �����ɏ�����̓��͂����ꂽ
		    // �ړ�����
			m_PosR.x -= sinf(m_Rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z -= cosf(m_Rot.y - (D3DX_PI * 0.75f)) * CAMERA_MOVIE_MOVE;

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else if (pKeyboard->GetPress(DIK_K) == true)
		{// �����ɉ������̓��͂����ꂽ
		    // �ړ�����
			m_PosR.x -= sinf(m_Rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z -= cosf(m_Rot.y - (D3DX_PI * 0.25f)) * CAMERA_MOVIE_MOVE;

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else
		{// ����������ĂȂ�
		    // �ړ�����
			m_PosR.x -= sinf(m_Rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVIE_MOVE;
			m_PosR.z -= cosf(m_Rot.y - (D3DX_PI * 0.5f)) * CAMERA_MOVIE_MOVE;

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
	}
	else if (pKeyboard->GetPress(DIK_I) == true)
	{// ������̓��͂����ꂽ
	    // �ړ�����
		m_PosR.x += sinf(m_Rot.y) * CAMERA_MOVIE_MOVE;
		m_PosR.z += cosf(m_Rot.y) * CAMERA_MOVIE_MOVE;

		// ���_�̌v�Z
		m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
		m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
		m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
	}
	else if (pKeyboard->GetPress(DIK_K) == true)
	{// �������̓��͂����ꂽ
	    // �ړ�����
		m_PosR.x += sinf(m_Rot.y + D3DX_PI) * CAMERA_MOVIE_MOVE;
		m_PosR.z += cosf(m_Rot.y + D3DX_PI) * CAMERA_MOVIE_MOVE;

		// ���_�̌v�Z
		m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
		m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
		m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
	}
	else if (pKeyboard->GetPress(DIK_U) == true)
	{// ������̓��͂����ꂽ
	    // �ړ�����
		m_PosR.y += 2.0f;

		// ���_�̌v�Z
		m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
		m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
		m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
	}
	else if (pKeyboard->GetPress(DIK_O) == true)
	{// �������̓��͂����ꂽ
	    // �ړ�����
		m_PosR.y -= 2.0f;

		// ���_�̌v�Z
		m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
		m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
		m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
	}

	CInputMouse *pMouse = CManager::GetMouse();

	if (pMouse != NULL)
	{// �}�E�X���擾�ł���
		if (pMouse->GetMouseAxisZ() != 0)
		{// �}�E�X�̃z�C�[���{�^�����X�N���[�����ꂽ
			m_fLength += 0.1f * pMouse->GetMouseAxisZ();
			if (m_fLength < CAMERA_LENGTH_MIN)
			{// ���������̒l���߂��Ȃ���
				m_fLength = CAMERA_LENGTH_MIN;
			}
			else if (m_fLength > CAMERA_LENGTH_MAX)
			{// ���������̒l��艓���Ȃ���
				m_fLength = CAMERA_LENGTH_MAX;
			}

			// ���_�̌v�Z
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}

		if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE && pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
		{// �}�E�X�̍��{�^���ƉE�{�^���������ɉ����ꂽ
			if (pMouse->GetMouseAxisX() != 0 || pMouse->GetMouseAxisY() != 0)
			{// �}�E�X����������Ă���
			 // �����ɉ����ꂽ�Ƃ��̈ʒu���獡�̃}�E�X�J�[�\���̈ʒu���王�_�����_���ړ�������������v�Z
				float fAngle = atan2f(((float)pMouse->GetPoint().x - (float)pMouse->GetPointOld().x), ((float)pMouse->GetPoint().y - (float)pMouse->GetPointOld().y));
				fAngle -= m_Rot.y;
				if (fAngle < -D3DX_PI)
				{// �������~�����𒴂���
					fAngle += D3DX_PI * 2.0f;
				}
				if (fAngle > D3DX_PI)
				{// �������~�����𒴂���
					fAngle -= D3DX_PI * 2.0f;
				}

				// �}�E�X��X��,Y������ړ��ʂ��v�Z
				float fMovement = sqrtf((float)pMouse->GetMouseAxisX() * (float)pMouse->GetMouseAxisX() + (float)pMouse->GetMouseAxisY() * (float)pMouse->GetMouseAxisY());
				fMovement *= CAMERA_MOVE_SPEED;

				// ���_���ړ�������
				m_PosR.x += sinf(fAngle) * fMovement;
				m_PosR.z -= cosf(fAngle) * fMovement;

				// �����_���ړ�������
				m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
				m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
			}
		}
		else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_0) == TRUE)
		{// �}�E�X�̍��N���b�N�݂̂������ꂽ
		    // �������}�E�X�̑��x�ɍ��킹�ĉ�]������
		    // �c��
			m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
			if (m_Rot.y > D3DX_PI)
			{// �������~�����𒴂���
				m_Rot.y -= D3DX_PI * 2.0f;
			}
			if (m_Rot.y < -D3DX_PI)
			{// �������~�����𒴂���
				m_Rot.y += D3DX_PI * 2.0f;
			}

			// ����
			m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
			if (m_Rot.z < -D3DX_PI)
			{// �������~�����𒴂���
				m_Rot.z = -D3DX_PI;
			}
			if (m_Rot.z > 0.0f)
			{// �������~�����𒴂���
				m_Rot.z = 0.0f;
			}

			// ���_�̈ʒu���v�Z����
			m_PosV.x = m_PosR.x - sinf(m_Rot.y) * m_fLength;
			m_PosV.y = m_PosR.y + cosf(m_Rot.z) * m_fLength;
			m_PosV.z = m_PosR.z - cosf(m_Rot.y) * m_fLength;
		}
		else if (pMouse->GetPress(CInputMouse::DIMS_BUTTON_1) == TRUE)
		{// �}�E�X�̉E�N���b�N�݂̂������ꂽ
		 // �������}�E�X�̑��x�ɍ��킹�ĉ�]������
		    // �c��
			m_Rot.y += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisX();
			if (m_Rot.y > D3DX_PI)
			{// �������~�����𒴂���
				m_Rot.y -= D3DX_PI * 2.0f;
			}
			if (m_Rot.y < -D3DX_PI)
			{// �������~�����𒴂���
				m_Rot.y += D3DX_PI * 2.0f;
			}

			// ����
			m_Rot.z += CAMERA_ANGLE_SPEED * pMouse->GetMouseAxisY();
			if (m_Rot.z < -D3DX_PI)
			{// �������~�����𒴂���
				m_Rot.z = -D3DX_PI;
			}
			if (m_Rot.z > 0.0f)
			{// �������~�����𒴂���
				m_Rot.z = 0.0f;
			}

			// �����_�̈ʒu���v�Z����
			m_PosR.x = m_PosV.x + sinf(m_Rot.y) * m_fLength;
			m_PosR.y = m_PosV.y - cosf(m_Rot.z) * m_fLength;
			m_PosR.z = m_PosV.z + cosf(m_Rot.y) * m_fLength;
		}
	}
}

//=============================================================================
//    �J�����̌��݂̌������v�Z���鏈��
//=============================================================================
void CCamera::CalcRot(void)
{
	// �������v�Z
	m_RotDiff = m_RotDest - m_Rot;

	// �����`�F�b�N
	// X��
	if (m_RotDiff.x > D3DX_PI)
	{// �~�����𒴂���
		m_RotDiff.x -= D3DX_PI * 2.0f;
	}
	if (m_RotDiff.x < -D3DX_PI)
	{// �~�����𒴂���
		m_RotDiff.x += D3DX_PI * 2.0f;
	}

	// Y��
	if (m_RotDiff.y > D3DX_PI)
	{// �~�����𒴂���
		m_RotDiff.y -= D3DX_PI * 2.0f;
	}
	if (m_RotDiff.y < -D3DX_PI)
	{// �~�����𒴂���
		m_RotDiff.y += D3DX_PI * 2.0f;
	}

	// Z��
	if (m_Rot.z > D3DX_PI)
	{// �~�����𒴂���
		m_Rot.z -= D3DX_PI * 2.0f;
	}
	if (m_Rot.z < -D3DX_PI)
	{// �~�����𒴂���
		m_Rot.z += D3DX_PI * 2.0f;
	}

	// �����ɉ��Z����
	m_Rot.x += m_RotDiff.x * CAMERA_ROT_RIVISION_X;
	m_Rot.y += m_RotDiff.y * CAMERA_ROT_RIVISION_Y;
	m_Rot.z += m_RotDiff.z * CAMERA_ROT_RIVISION_Z;

	// ���݂̌����`�F�b�N
	// X��
	if (m_Rot.x > D3DX_PI)
	{// �~�����𒴂���
		m_Rot.x -= D3DX_PI * 2.0f;
	}
	if (m_Rot.x < -D3DX_PI)
	{// �~�����𒴂���
		m_Rot.x += D3DX_PI * 2.0f;
	}

	// Y��
	if (m_Rot.y > D3DX_PI)
	{// �~�����𒴂���
		m_Rot.y -= D3DX_PI * 2.0f;
	}
	if (m_Rot.y < -D3DX_PI)
	{// �~�����𒴂���
		m_Rot.y += D3DX_PI * 2.0f;
	}

	// Z��
	if (m_Rot.z > D3DX_PI)
	{// �~�����𒴂���
		m_Rot.z -= D3DX_PI * 2.0f;
	}
	if (m_Rot.z < -D3DX_PI)
	{// �~�����𒴂���
		m_Rot.z += D3DX_PI * 2.0f;
	}
}

//=============================================================================
//    �J�����̌��݂̎��_�̐ݒ菈��
//=============================================================================
void CCamera::SetPosV(const D3DXVECTOR3 posV)
{
	m_PosV = posV;
}

//=============================================================================
//    �J�����̖ړI�̎��_�̐ݒ菈��
//=============================================================================
void CCamera::SetPosVDest(const D3DXVECTOR3 posVDest)
{
	m_PosVDest = posVDest;
}

//=============================================================================
//    �J�����̌��݂̎��_�ƖړI�̎��_�̍����ݒ菈��
//=============================================================================
void CCamera::SetPosVDiff(const D3DXVECTOR3 posVDiff)
{
	m_PosVDiff = posVDiff;
}

//=============================================================================
//    �J�����̑O��̎��_�̐ݒ菈��
//=============================================================================
void CCamera::SetPosVOld(const D3DXVECTOR3 posVOld)
{
	m_PosVOld = posVOld;
}

//=============================================================================
//    �J�����̌��݂̒����_�̐ݒ菈��
//=============================================================================
void CCamera::SetPosR(const D3DXVECTOR3 posR)
{
	m_PosR = posR;
}

//=============================================================================
//    �J�����̖ړI�̒����_�̐ݒ菈��
//=============================================================================
void CCamera::SetPosRDest(const D3DXVECTOR3 posRDest)
{
	m_PosRDest = posRDest;
}

//=============================================================================
//    �J�����̌��݂̒����_�ƖړI�̒����_�̍����ݒ菈��
//=============================================================================
void CCamera::SetPosRDiff(const D3DXVECTOR3 posRDiff)
{
	m_PosRDiff = posRDiff;
}

//=============================================================================
//    �J�����̎��_�̈ړ��ʂ̎擾����
//=============================================================================
void CCamera::SetMoveV(const D3DXVECTOR3 MoveV)
{
	m_MoveV = MoveV;
}

//=============================================================================
//    �J�����̒����_�̈ړ��ʂ̎擾����
//=============================================================================
void CCamera::SetMoveR(const D3DXVECTOR3 MoveR)
{
	m_MoveR = MoveR;
}

//=============================================================================
//    �J�����̌��݂̌����̐ݒ菈��
//=============================================================================
void CCamera::SetRot(const D3DXVECTOR3 Rot)
{
	m_Rot = Rot;
}

//=============================================================================
//    �J�����̖ړI�̌����̐ݒ菈��
//=============================================================================
void CCamera::SetRotDest(const D3DXVECTOR3 RotDest)
{
	m_RotDest = RotDest;
}

//=============================================================================
//    �J�����̖ړI�̌����ƌ��݂̌����̍����̐ݒ菈��
//=============================================================================
void CCamera::SetRotDiff(const D3DXVECTOR3 RotDiff)
{
	m_RotDiff = RotDiff;
}

//=============================================================================
//    �J�����̑O��̌����̐ݒ菈��
//=============================================================================
void CCamera::SetRotOld(const D3DXVECTOR3 RotOld)
{
	m_RotOld = RotOld;
}

//=============================================================================
//    �J�����̌����̐ݒ菈��
//=============================================================================
void CCamera::SetMtxProjection(const D3DXMATRIX MtxProjection)
{
	m_MtxProjection = MtxProjection;
}

//=============================================================================
//    �J�����̌����̐ݒ菈��
//=============================================================================
void CCamera::SetMtxView(const D3DXMATRIX MtxView)
{
	m_MtxView = MtxView;
}

//=============================================================================
//    �J�����̌��݂̎��_�̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosV(void)
{
	return m_PosV;
}

//=============================================================================
//    �J�����̖ړI�̎��_�̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosVDest(void)
{
	return m_PosVDest;
}

//=============================================================================
//    �J�����̌��݂̎��_�ƖړI�̎��_�̍����擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosVDiff(void)
{
	return m_PosVDiff;
}

//=============================================================================
//    �J�����̑O��̎��_�̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosVOld(void)
{
	return m_PosVOld;
}

//=============================================================================
//    �J�����̌��݂̒����_�̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosR(void)
{
	return m_PosR;
}

//=============================================================================
//    �J�����̖ړI�̒����_�̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosRDest(void)
{
	return m_PosRDest;
}

//=============================================================================
//    �J�����̌��݂̒����_�ƖړI�̒����_�̍����擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetPosRDiff(void)
{
	return m_PosRDiff;
}

//=============================================================================
//    �J�����̎��_�̈ړ��ʂ̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetMoveV(void)
{
	return m_MoveV;
}

//=============================================================================
//    �J�����̒����_�̈ړ��ʂ̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetMoveR(void)
{
	return m_MoveR;
}

//=============================================================================
//    �J�����̌��݂̌����̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    �J�����̖ړI�̌����̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetRotDest(void)
{
	return m_RotDest;
}

//=============================================================================
//    �J�����̖ړI�̌����ƌ��݂̌����̍����̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetRotDiff(void)
{
	return m_RotDiff;
}

//=============================================================================
//    �J�����̑O��̌����̎擾����
//=============================================================================
D3DXVECTOR3 CCamera::GetRotOld(void)
{
	return m_RotOld;
}

//=============================================================================
//    �J�����̃v���W�F�N�V�����}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CCamera::GetMtxProjection(void)
{
	return m_MtxProjection;
}

//=============================================================================
//    �J�����̃r���[�}�g���b�N�X�̎擾����
//=============================================================================
D3DXMATRIX CCamera::GetMtxView(void)
{
	return m_MtxView;
}