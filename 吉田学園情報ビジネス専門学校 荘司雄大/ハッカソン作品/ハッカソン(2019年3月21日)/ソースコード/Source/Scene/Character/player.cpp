//*****************************************************************************
//
//     �v���C���[�̏���[player.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "character.h"
#include "manager.h"
#include "system.h"
#include "3DPolygon.h"
#include "motion.h"
#include "mode.h"
#include "effect.h"
#include "bullet.h"
#include "star.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define PLAYER_SHADOW_PRIORITY       (9)                                // �v���C���[�̉e�̏����D�揇��
#define PLAYER_SHADOW_WIDTH          (18.0f)                            // �v���C���[�̉e�̕�
#define PLAYER_SHADOW_HEIGHT         (18.0f)                            // �v���C���[�̉e�̍���
#define PLAYER_SHADOW_COLOR          (D3DXCOLOR(1.0f,1.0f,1.0f,0.5f))   // �v���C���[�̉e�̐F

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CPlayer::CPlayer(int nPriority, OBJTYPE objType) : CCharacter(nPriority, objType)
{
	// �e��l�̐ݒ�
	m_State = STATE_NORMAL;    // ���
	m_nControllerIdx = 0;
	m_pCamera = NULL;
	m_bDamage = false;
	m_bAttack = false;
	m_nDamageCounter = 0;
	m_nNumScore = 0;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
//    ��������
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, int nControllerIdx, int nPriority)
{
	CPlayer *pPlayer = NULL;               // �v���C���[�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pPlayer == NULL)
		{// ����������ɂȂ��Ă���
			pPlayer = new CPlayer(nPriority);
			if (pPlayer != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pPlayer->Init(pos, rot, apModel, pMotionManager, pTextureManager, nNumParts, fAccel, fInertia, fJumpPower, fGravity, fRivisionRot, fColHeight, fColRange, nControllerIdx)))
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

	return pPlayer;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, int nControllerIdx)
{
	// �e��l�̐ݒ�
	SetPos(pos);                        // ���݂̍��W
	SetRot(rot);                        // ���݂̌���
	SetNumParts(nNumParts);             // �p�[�c��
	SetModel(apModel);                  // ���f���N���X�ւ̃|�C���^
	SetMotionManager(pMotionManager);   // ���[�V�����Ǌ��N���X�ւ̃|�C���^
	SetTextureManager(pTextureManager); // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	SetAccel(fAccel);                   // �����x
	SetInertia(fInertia);               // ����
	SetGravity(fGravity);               // �d��
	SetJumpPower(fJumpPower);           // �W�����v��
	SetRivisionRot(fRivisionRot);       // ������␳����{��
	SetColHeight(fColHeight);           // �����蔻�����鍂��
	SetColRange(fColRange);             // �����蔻������͈�
	SetControllerIdx(nControllerIdx);   // �R���g���[���[�̔ԍ�

	// ���f���̈ʒu���j���[�g�����̈ʒu�ɂ���
	SetDefaultPos();

	// ���f���̌������j���[�g�����̌����ɂ���
	SetDefaultRot();

	// �e���쐬����
	CShadow *pShadow = CShadow::Create(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_SHADOW_COLOR, PLAYER_SHADOW_WIDTH, PLAYER_SHADOW_HEIGHT, PLAYER_SHADOW_PRIORITY);
	if (pShadow != NULL)
	{
		SetShadow(pShadow);
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CPlayer::Uninit(void)
{
	// ���ʂ̏I������
	CCharacter::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CPlayer::Update(void)
{
	// �O�����Ă����I�u�W�F�N�g�ւ̃|�C���^��ۑ�
	SetLandSceneOld(GetLandScene());

	// ����Ă���I�u�W�F�N�g�ւ̃|�C���^����ɂ���
	SetLandScene(NULL);

	// �O��̍��W��ۑ�
	SetPosOld(GetPos());

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		if(CManager::GetGame()->GetState() == CGame::STATE_NORMAL)
		{
			// �ړ��ʌv�Z����
			InputMove();
		}
	}

	// �ړ�����
	Movement();

	if (GetShadow() != NULL)
	{
		GetShadow()->SetPos(D3DXVECTOR3(GetPos().x, 1.0f, GetPos().z));
	}

	// �����蔻�菈��
	Collision();

	// �A�N�V��������
	Action();

	// ��ԑJ�ڏ���
	Statement();

	if (GetMotionManager() != NULL)
	{// ���[�V�����Ǘ��N���X����������Ă���
		GetMotionManager()->Update(GetModel());
	}

	if (m_bDamage == true && m_bAttack == true)
	{
		if (GetPos().y <= 0.0f)
		{
			m_bDamage = false;
		}
	}
	else if (m_bDamage == false && m_bAttack == true)
	{
		m_nDamageCounter--;
		if (m_nDamageCounter <= 0)
		{
			m_bAttack = false;
		}
	}

	CDebugProc::Print("%.1f %.1f %.1f\n", GetPos().x, GetPos().y, GetPos().z);
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CPlayer::Draw(void)
{
	// ���ʂ̕`�揈��
	if (m_bAttack == true && m_bDamage == false)
	{
		if (m_nDamageCounter % 2 == 0)
		{
			CCharacter::Draw();
		}
	}
	else
	{
		CCharacter::Draw();
	}
}

//=============================================================================
//    ���͂ɂ��ړ��ʌv�Z����
//=============================================================================
void CPlayer::InputMove(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 DestRot = GetDestRot();   // �ړI�̌���
	D3DXVECTOR3 Move = GetMove();         // �ړ���
	float fAccel = GetAccel();            // �ړ��X�s�[�h

	// �L�[�{�[�h���擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �R���g���[���[���擾
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(m_nControllerIdx);

	// �J�������擾
	CCamera *pCamera = m_pCamera;

	//if (pKeyboard->GetPress(DIK_A) == true)
	//{// �������̓��͂����ꂽ
	//	if (pKeyboard->GetPress(DIK_W) == true)
	//	{// �����ɏ�����̓��͂����ꂽ
	//	    // �ړ�����
	//		Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * fAccel;
	//		Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * fAccel;

	//		// �ړI�̊p�x�ύX
	//		DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.75f);
	//	}
	//	else if (pKeyboard->GetPress(DIK_S) == true)
	//	{// �����ɉ������̓��͂����ꂽ
	//	    // �ړ�����
	//		Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * fAccel;
	//		Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * fAccel;

	//		// �ړI�̊p�x�ύX
	//		DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.25f);
	//	}
	//	else
	//	{// ����������ĂȂ�
	//	    // �ړ�����
	//		Move.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * fAccel;
	//		Move.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * fAccel;

	//		// �ړI�̊p�x�ύX
	//		DestRot.y = pCamera->GetRot().y + (D3DX_PI * 0.5f);
	//	}
	//}
	//else if (pKeyboard->GetPress(DIK_D) == true)
	//{// �E�����̓��͂����ꂽ
	//	if (pKeyboard->GetPress(DIK_W) == true)
	//	{// �����ɏ�����̓��͂����ꂽ
	//	    // �ړ�����
	//		Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * fAccel;
	//		Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * fAccel;

	//		// �ړI�̊p�x�ύX
	//		DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.75f);
	//	}
	//	else if (pKeyboard->GetPress(DIK_S) == true)
	//	{// �����ɉ������̓��͂����ꂽ
	//	    // �ړ�����
	//		Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * fAccel;
	//		Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * fAccel;

	//		// �ړI�̊p�x�ύX
	//		DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.25f);
	//	}
	//	else
	//	{// ����������ĂȂ�
	//	    // �ړ�����
	//		Move.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * fAccel;
	//		Move.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * fAccel;

	//		// �ړI�̊p�x�ύX
	//		DestRot.y = pCamera->GetRot().y + (D3DX_PI * -0.5f);
	//	}
	//}
	//else if (pKeyboard->GetPress(DIK_W) == true)
	//{// ������̓��͂����ꂽ
	//    // �ړ�����
	//	Move.x += sinf(pCamera->GetRot().y) * fAccel;
	//	Move.z += cosf(pCamera->GetRot().y) * fAccel;

	//	// �ړI�̊p�x�ύX
	//	DestRot.y = pCamera->GetRot().y + D3DX_PI;
	//}
	//else if (pKeyboard->GetPress(DIK_S) == true)
	//{// �������̓��͂����ꂽ
	//    // �ړ�����
	//	Move.x += sinf(pCamera->GetRot().y + D3DX_PI) * fAccel;
	//	Move.z += cosf(pCamera->GetRot().y + D3DX_PI) * fAccel;

	//	// �ړI�̊p�x�ύX
	//	DestRot.y = pCamera->GetRot().y;
	//}
	//else
	//{
		if (pJoyStick != NULL)
		{// �W���C�X�e�B�b�N���擾�ł���
			if (pJoyStick->GetLeftAxisX() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisX() <= -DI_STICK_AXIS_MIN
				|| pJoyStick->GetLeftAxisY() >= DI_STICK_AXIS_MIN || pJoyStick->GetLeftAxisY() <= -DI_STICK_AXIS_MIN)
			{// ���A�i���O���͂�����Ă���
			    // �X�e�B�b�N�̓|���ꂽ�����̔䗦���v�Z
				float fLengthX = (float)(sqrt(pJoyStick->GetLeftAxisX() * pJoyStick->GetLeftAxisX() + pJoyStick->GetLeftAxisY() * pJoyStick->GetLeftAxisY()) / DI_JOY_STICK_INPUT);

				// �R�T�C���J�[�u�Ŕ䗦��␳
				float fRivision = cosf(sqrtf(2) - fLengthX);
				if (fRivision < 1.0)
				{// �{����1�𒴂��Ă��Ȃ�
					fRivision += 0.09f;  // ����Ȃ�����␳
					if (fRivision >= 1.0f)
					{// �{����1�𒴂��Ă��܂���
						fRivision = 1.0f;
					}
				}

				// �ړ��ʌv�Z����
				//Move.x += sinf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (fAccel * fRivision);
				//Move.z -= cosf(pJoyStick->GetLeftAxis() - pCamera->GetRot().y) * (fAccel * fRivision);

				// �ړI�̊p�x�ύX
				float fValue = 0.0f;
				if (pJoyStick->GetLeftAxisX() >= DI_STICK_AXIS_MIN)
				{
					fValue = 0.05f;
				}
				if (pJoyStick->GetLeftAxisX() <= -DI_STICK_AXIS_MIN)
				{
					fValue = -0.05f;
				}

				DestRot.y += fValue * fRivision;
				if (DestRot.y > D3DX_PI)
				{
					DestRot.y -= D3DX_PI * 2.0f;
				}
				if (DestRot.y < -D3DX_PI)
				{
					DestRot.y += D3DX_PI *2.0f;
				}
			}
			else if (pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_28) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_29) == true
				|| pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_30) == true || pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_31) == true)
			{// �\���L�[���͂�����Ă���
			    // �ړ��ʌv�Z����
				Move.x += sinf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (fAccel);
				Move.z += cosf(pCamera->GetRot().y + pJoyStick->GetRadian()) * (fAccel);

				// �ړI�̊p�x�ύX
				DestRot.y = pCamera->GetRot().y + (pJoyStick->GetRadian() - D3DX_PI);
			}

			if (pJoyStick->GetPress(CJoyStick::DIJS_BUTTON_3) == true)
			{
				Move.x += sinf(pCamera->GetRot().y) * (fAccel);
				Move.z += cosf(pCamera->GetRot().y) * (fAccel);

				// �u�[�X�g�̃G�t�F�N�g����
				CreateBoostEffect();
			}
		}
	//}

	// �e��l�̐ݒ�
	SetDestRot(DestRot);    // �ړI�̌���
	SetMove(Move);          // �ړ���
}


//=============================================================================
//    �A�N�V��������
//=============================================================================
void CPlayer::Action(void)
{
	// �R���g���[���[���擾
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(m_nControllerIdx);

	if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_7) == true)
	{
		D3DXVECTOR3 BulletPos = D3DXVECTOR3(0.0f, 30.0f, -30.0f);
		D3DXVec3TransformCoord(&BulletPos, &BulletPos, &GetMtxWorld());
		D3DXVECTOR3 BulletMove = D3DXVECTOR3(sinf(GetRot().y - D3DX_PI) * 40.0f, 0.0f, cosf(GetRot().y - D3DX_PI) * 40.0f);
		CBullet::Create(BulletPos, D3DXVECTOR3(GetRot().x, GetRot().y - D3DX_PI, GetRot().z), BulletMove, m_nControllerIdx);
	}
	else if (pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_6) == true)
	{
		D3DXVECTOR3 BulletPos = D3DXVECTOR3(0.0f, 30.0f, -30.0f);
		D3DXVec3TransformCoord(&BulletPos, &BulletPos, &GetMtxWorld());
		D3DXVECTOR3 BulletMove = D3DXVECTOR3(sinf(GetRot().y) * 20.0f, 0.0f, cosf(GetRot().y) * 20.0f);
		CBullet::Create(BulletPos, D3DXVECTOR3(GetRot().x, GetRot().y, GetRot().z), BulletMove, m_nControllerIdx);
	}
}

//=============================================================================
//    ��ԑJ�ڏ���
//=============================================================================
void CPlayer::Statement(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 Move = GetMove();

	//if (Move.x <= 0.1f && Move.x >= -0.1f
	//	&& Move.z <= 0.1f && Move.z >= -0.1f)
	//{// �ړ����Ă��Ȃ�
	//	if (m_State != STATE_NORMAL)
	//	{// �ʏ��Ԃł͂Ȃ�
	//	    // ��Ԃ�ʏ�̏�Ԃ�
	//		m_State = STATE_NORMAL;

	//		// ���[�V�����؂�ւ�����
	//		GetMotionManager()->SwitchMotion(GetModel(), m_State);
	//	}
	//}
	//else
	//{// �ړ����Ă���
	//	if (m_State != STATE_MOVE)
	//	{// ������Ԃł͂Ȃ�
	//		m_State = STATE_MOVE;

	//		// ���[�V�����؂�ւ�����
	//		GetMotionManager()->SwitchMotion(GetModel(), m_State);
	//	}
	//}
}

//=============================================================================
//    �_���[�W���󂯂��Ƃ��̏���
//=============================================================================
void CPlayer::Damage(D3DXVECTOR3 BulletMove)
{
	if (m_bAttack == false)
	{// �_���[�W�����炤��ԂȂ��
		m_bAttack = true;
		m_bDamage = true;
		m_nDamageCounter = 180;

		D3DXVECTOR3 Move = BulletMove;
		Move.y = 20.0f;
		SetMove(Move);

		for (int nCntStar = 0; nCntStar < 10; nCntStar++)
		{
			m_nNumScore--;
			if (m_nNumScore <= 0)
			{
				m_nNumScore = 0;
			}
			else
			{
				float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
				CStar::Create(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(fAngle) * 10.0f, 20.0f, cosf(fAngle) * 10.0f));
			}
		}
	}
}

//=============================================================================
//    �u�[�X�g�̃G�t�F�N�g�𐶐�����
//=============================================================================
void CPlayer::CreateBoostEffect(void)
{
	// �G�t�F�N�g���o���ʒu��ݒ�
	D3DXVECTOR3 EffectPos;
	EffectPos = D3DXVECTOR3(0.0f,45.0f,50.0f);
	D3DXVec3TransformCoord(&EffectPos, &EffectPos, &GetMtxWorld());
	CManager::GetBaseMode()->GetEffectManager()->SetEffect(EffectPos, GetRot(), 0);

	EffectPos = D3DXVECTOR3(30.0f, 60.0f, 50.0f);
	D3DXVec3TransformCoord(&EffectPos, &EffectPos, &GetMtxWorld());
	CManager::GetBaseMode()->GetEffectManager()->SetEffect(EffectPos, GetRot(), 0);

	EffectPos = D3DXVECTOR3(-30.0f, 60.0f, 50.0f);
	D3DXVec3TransformCoord(&EffectPos, &EffectPos, &GetMtxWorld());
	CManager::GetBaseMode()->GetEffectManager()->SetEffect(EffectPos, GetRot(), 0);

	EffectPos = D3DXVECTOR3(0.0f, 45.0f, 60.0f);
	D3DXVec3TransformCoord(&EffectPos, &EffectPos, &GetMtxWorld());
	CManager::GetBaseMode()->GetEffectManager()->SetEffect(EffectPos, GetRot(), 0);

	EffectPos = D3DXVECTOR3(30.0f, 60.0f, 60.0f);
	D3DXVec3TransformCoord(&EffectPos, &EffectPos, &GetMtxWorld());
	CManager::GetBaseMode()->GetEffectManager()->SetEffect(EffectPos, GetRot(), 0);

	EffectPos = D3DXVECTOR3(-30.0f, 60.0f, 60.0f);
	D3DXVec3TransformCoord(&EffectPos, &EffectPos, &GetMtxWorld());
	CManager::GetBaseMode()->GetEffectManager()->SetEffect(EffectPos, GetRot(), 0);
}

//=============================================================================
//    �X�R�A�����Z����
//=============================================================================
void CPlayer::AddScore(void)
{
	m_nNumScore++;
}

//=============================================================================
//    ��Ԃ�ݒ肷��
//=============================================================================
void CPlayer::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    �R���g���[���[�̔ԍ���ݒ肷��
//=============================================================================
void CPlayer::SetControllerIdx(const int nIdx)
{
	m_nControllerIdx = nIdx;
}

//=============================================================================
//    �J�����N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CPlayer::SetCamera(CCamera *pCamera)
{
	m_pCamera = pCamera;
}

//=============================================================================
//    ��Ԃ��擾����
//=============================================================================
CPlayer::STATE CPlayer::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �R���g���[���[�̔ԍ����擾����
//=============================================================================
int CPlayer::GetControllerIdx(void)
{
	return m_nControllerIdx;
}

//=============================================================================
//    �J�����N���X�ւ̃|�C���^���擾����
//=============================================================================
CCamera *CPlayer::GetCamera(void)
{
	return m_pCamera;
}

int CPlayer::GetScore(void)
{
	return m_nNumScore;
}