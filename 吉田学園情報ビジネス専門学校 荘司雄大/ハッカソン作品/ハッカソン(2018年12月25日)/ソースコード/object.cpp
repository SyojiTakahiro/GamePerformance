//*****************************************************************************
//
//     �z�u���̏���[object.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "object.h"
#include "player.h"
#include "motion.h"
#include "enemy.h"
#include "debuglog.h"
#include "manager.h"
#include "game.h"
#include "result.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define OBJECT_DISP_TIMING  (240)    // �I�u�W�F�N�g���Ăяo���^�C�~���O

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************


//*****************************************************************************
//     CObjecct�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CObject::CObject(int nPriority, OBJTYPE objType) : CSceneX(nPriority, objType)
{
	// �e��l�̃N���A
	m_nDispCounter = 0;                           // �`����Ǘ�����J�E���^�[
	m_bDisp = true;                               // �`�悷�邩���Ȃ���
	m_ModelIdx = 0;                               // �g�p���Ă��郂�f���̔ԍ�
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // �O��̍��W
	m_RollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ��]����X�s�[�h
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
//    �z�u���̐�������
//=============================================================================
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, int nPriority)
{
	CObject *pObject = NULL;   // �z�u���N���X�^�̃|�C���^
	if (pObject == NULL)
	{// ����������ɂȂ��Ă���
		pObject = new CObject(nPriority);
		if (pObject != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pObject->Init(pos, rot, RollSpeed)))
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

	return pObject;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �z�u���̏���������
//=============================================================================
HRESULT CObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed)
{
	// �e��l�̐ݒ�
	SetPos(pos);                // ���݂̍��W
	SetRot(rot);                // ���݂̌���
	m_RotStd = rot;             // �������̌���
	m_RollSpeed = RollSpeed;    // ��]����X�s�[�h

	// ���ʂ̏���������
	if (FAILED(CSceneX::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �z�u���̏I������
//=============================================================================
void CObject::Uninit(void)
{
	// ���ʂ̏I������
	CSceneX::Uninit();
}

//=============================================================================
//    �z�u���̍X�V����
//=============================================================================
void CObject::Update(void)
{
	if (m_bDisp == false)
	{// �`�悵�Ȃ���Ԃ�������
		m_nDispCounter++;
		if (m_nDispCounter >= OBJECT_DISP_TIMING)
		{// �J�E���^�[������̒l�ɂȂ���
			m_bDisp = true;
			m_nDispCounter = 0;

			// �v���C���[�����ɂ��Ȃ�������
			CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
			CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
			CPlayer *pPlayer = NULL;             // �v���C���[�N���X�ւ̃|�C���^
			CEnemy *pEnemy = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// �`��D�揇�ʂ̐������J��Ԃ�
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// ����������ɂȂ�܂ŌJ��Ԃ�
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == OBJTYPE_PLAYER)
					{// �v���C���[��������
						pPlayer = (CPlayer*)pScene;
						if (pPlayer != NULL)
						{
							if (pPlayer->GetPos().y < GetPos().y + GetVtxMax().y && pPlayer->GetPos().y >= GetPos().y + GetVtxMin().y)
							{
								pPlayer->SetState(CPlayer::STATE_DEATH);
								pPlayer->GetMotionManager()->SwitchMotion(pPlayer->GetModel(), pPlayer->GetState());
							}
						}
					}
					else if (pScene->GetObjType() == OBJTYPE_ENEMY)
					{
						pEnemy = (CEnemy*)pScene;
						if (pEnemy != NULL)
						{
							if (pEnemy->GetPos().y < GetPos().y + GetVtxMax().y && pEnemy->GetPos().y > GetPos().y + GetVtxMin().y)
							{
								pEnemy->SetState(CEnemy::STATE_DEATH);
								pEnemy->GetMotionManager()->SwitchMotion(pEnemy->GetModel(), pEnemy->GetState());
							}
						}
					}
					pScene = pSceneNext;
				}
			}
		}
	}

	// �O��̍��W�X�V
	m_PosOld = GetPos();

	// �������擾����
	D3DXVECTOR3 rot = GetRot();

	// ��]������
	rot += m_RollSpeed;

	// �����`�F�b�N
	// X��
	if (rot.x > D3DX_PI)
	{// �������~�����𒴂���
		rot.x -= D3DX_PI * 2.0f;
	}
	if (rot.x < -D3DX_PI)
	{// �������~�����𒴂���
		rot.x += D3DX_PI * 2.0f;
	}

	// Y��
	if (rot.y > D3DX_PI)
	{// �������~�����𒴂���
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// �������~�����𒴂���
		rot.y += D3DX_PI * 2.0f;
	}

	// Z��
	if (rot.z > D3DX_PI)
	{// �������~�����𒴂���
		rot.z -= D3DX_PI * 2.0f;
	}
	if (rot.z < -D3DX_PI)
	{// �������~�����𒴂���
		rot.z += D3DX_PI * 2.0f;
	}

	// �����̐ݒ�
	SetRot(rot);
}

//=============================================================================
//    �z�u���̕`�揈��
//=============================================================================
void CObject::Draw(void)
{
	if (m_bDisp == true)
	{// �`�悷���ԂȂ��
		// ���ʂ̕`�揈��
		CSceneX::Draw();
	}
}

//=============================================================================
//    �z�u���̓����蔻�菈��
//=============================================================================
bool CObject::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject)
{
	bool bLand = false;   // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ���

	if (m_bDisp == true)
	{// �`�悷���ԂȂ��
		// �����蔻��ɕK�v�Ȓl��z�u������擾
		D3DXVECTOR3 pos = GetPos();          // ���݂̍��W
		D3DXVECTOR3 Rot = GetRot();          // ���݂̌���
		D3DXVECTOR3 VtxMin = GetVtxMin();    // ���_�̍��W�ŏ��l
		D3DXVECTOR3 VtxMax = GetVtxMax();    // ���_�̍��W�ő�l

		// �O�ς̔���ɕK�v�Ȓl��p��
		D3DXVECTOR3 ObjectCollisionPos[4] =
		{// �����蔻��p�̎l�p(���[�J�����W)
			D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMax.z + fRange),
			D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMax.z + fRange),
			D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMin.z - fRange),
			D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMin.z - fRange),
		};

		D3DXVECTOR3 ObjectCollisionWorldPos[4];  // �����蔻��p�̎l�p(���[���h���W)
		D3DXMATRIX ObjectCollisionMtxWorld;      // �v�Z�p�}�g���b�N�X

		for (int nCntCol = 0; nCntCol < 4; nCntCol++)
		{// ���肷��l�p�̍��W���J��Ԃ�
			D3DXMATRIX ObjectCollisionMtxWorld, mtxRot, mtxTrans;  // �v�Z�p�}�g���b�N�X

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&ObjectCollisionMtxWorld);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
			D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, ObjectCollisionPos[nCntCol].x, ObjectCollisionPos[nCntCol].y, ObjectCollisionPos[nCntCol].z);
			D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxTrans);

			// �e�̏����|�����킹��
			D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &GetMtxWorld());

			// ���[���h���W�𔲂��o��
			ObjectCollisionWorldPos[nCntCol] = D3DXVECTOR3(ObjectCollisionMtxWorld._41, ObjectCollisionMtxWorld._42, ObjectCollisionMtxWorld._43);
		}

		D3DXVECTOR3 vecA, vecB;        // ����p�x�N�g��
		int nCntVecTrue = 0;           // �x�N�g���̔����ʂ�����
		bool bObjectRand = false;      // �I�u�W�F�N�g�̒��ɂ��邩�ǂ���
		for (int nCntVec = 0; nCntVec < 4; nCntVec++)
		{// ���肷��x�N�g���̐������J��Ԃ�
			vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
			vecB = *pPos - ObjectCollisionWorldPos[nCntVec];

			if ((vecA.x * vecB.z) - (vecA.z * vecB.x) <= 0)
			{// �x�N�g���̉E���ɂ���
				nCntVecTrue++;   // �����ʂ����񐔂����Z
				if (nCntVecTrue == 4)
				{// �S�Ẵx�N�g���̉E���ɂ���
					bObjectRand = true;
				}
			}
		}

		if (bObjectRand == true)
		{// �z�u���̒��ɂ���
			// ����Ă��邩�ǂ����̔���J�n
			if (pPosOld->y >= pos.y + VtxMax.y && pPos->y <= pos.y + VtxMax.y)
			{// �z�u���ɏォ�������
				pPos->y = pos.y + VtxMax.y;
				pMove->y = 0.0f;

				if (pRideObject->GetObjType() == OBJTYPE_PLAYER)
				{// ����Ă����I�u�W�F�N�g���v���C���[������
					CPlayer *pPlayer = (CPlayer*)pRideObject;

					// �K�v�Ȓl���擾����
					CPlayer::STATE State = pPlayer->GetState();                     // ���
					D3DXVECTOR3 Rot = pPlayer->GetRot();                            // ���݂̌���
					D3DXVECTOR3 DestRot = pPlayer->GetDestRot();                    // �ړI�̌���
					D3DXVECTOR3 Move = pPlayer->GetMove();                          // �ړ���
					D3DXVECTOR3 OffsetPos = pPlayer->GetLandOffsetPos();            // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g����
					D3DXVECTOR3 OffsetAdd = pPlayer->GetLandOffsetAdd();            // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g�����ɉ��Z����l
					D3DXVECTOR3 OffsetRot = pPlayer->GetLandOffsetRot();            // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g����
					D3DXVECTOR3 OffsetRotStart = pPlayer->GetLandOffsetRotStart();  // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g�����ɉ��Z����l
					D3DXVECTOR3 ObjRot = GetRot();                                  // �I�u�W�F�N�g�̌���


					//if (pPlayer->GetLand() == false)
					//{// �O�����Ă��Ȃ�����
					//	// ������ۑ�
					//	OffsetPos = D3DXVECTOR3(pPos->x - pos.x, 0.0f, pPos->z - pos.z); // �I�t�Z�b�g���W��ۑ�
					//	OffsetRot = Rot - ObjRot;                                        // �v���C���[�̃I�t�Z�b�g������ۑ�
					//	if (OffsetRot.y > D3DX_PI)
					//	{// �~�����𒴂���
					//		OffsetRot.y -= D3DX_PI * 2.0f;
					//	}
					//	if (OffsetRot.y < -D3DX_PI)
					//	{// �~�����𒴂���
					//		OffsetRot.y += D3DX_PI * 2.0f;
					//	}
					//	OffsetRotStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �I�u�W�F�N�g�̃X�^�[�g������������
					//	OffsetAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �I�t�Z�b�g�ɉ�����l��������
					//}


					//// ��������]�ɉ��킹��
					//if (State != CPlayer::STATE_MOVE)
					//{// �ړ���Ԃł͂Ȃ�
					//	Rot = OffsetRot + ObjRot;    // ������␳
					//	if (Rot.y > D3DX_PI)
					//	{// �~�����𒴂���
					//		Rot.y -= D3DX_PI * 2.0f;
					//	}
					//	if (Rot.y < -D3DX_PI)
					//	{// �~�����𒴂���
					//		Rot.y += D3DX_PI * 2.0f;
					//	}
					//	DestRot.y = Rot.y;  // �ړI�̌������␳
					//}

					//// �I�t�Z�b�g�̌����ɔz�u���̉�]�X�s�[�h�����Z����
					//OffsetRotStart.y += m_RollSpeed.y;  // �I�u�W�F�N�g����]����X�s�[�h�ɍ��킹�ĕ␳
					//if (OffsetRotStart.y > D3DX_PI)
					//{// �~�����𒴂���
					//	OffsetRotStart.y -= D3DX_PI * 2.0f;
					//}
					//else if (OffsetRotStart.y < -D3DX_PI)
					//{// �~�����𒴂���
					//	OffsetRotStart.y += D3DX_PI * 2.0f;
					//}
					//// �I�t�Z�b�g�ʒu��Y����]�ɉ��킹���ړ��ʂ��v�Z����
					//OffsetAdd.x += cosf(0.0f) * (cosf(OffsetRotStart.y) * Move.x + -sinf(OffsetRotStart.y) * Move.z) + sinf(0.0f) * (sinf(OffsetRotStart.y) * Move.x + cosf(OffsetRotStart.y) * Move.z);
					//OffsetAdd.z += -sinf(0.0f) * (cosf(OffsetRotStart.y) * Move.x + -sinf(OffsetRotStart.y) * Move.z) + cosf(0.0f) * (sinf(OffsetRotStart.y) * Move.x + cosf(OffsetRotStart.y) * Move.z);

					//// �ʒu��Y����]�ɉ��킹�Ĉړ�������
					//float fMtxX = cosf(OffsetRotStart.y) * (OffsetPos.x + (OffsetAdd.x * 1.5f)) + sinf(OffsetRotStart.y) * (OffsetPos.z + (OffsetAdd.z * 1.5f));
					//float fMtxZ = -sinf(OffsetRotStart.y) * (OffsetPos.x + (OffsetAdd.x * 1.5f)) + cosf(OffsetRotStart.y) * (OffsetPos.z + (OffsetAdd.z * 1.5f));
					//pPos->x = fMtxX + pos.x;
					//pPos->z = fMtxZ + pos.z;

					// �l��ݒ肷��
					pPlayer->SetRot(Rot);                              // ���݂̌���
					pPlayer->SetDestRot(DestRot);                      // �ړI�̌���
					pPlayer->SetLandOffsetPos(OffsetPos);              // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g����
					pPlayer->SetLandOffsetAdd(OffsetAdd);              // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g�����ɉ��Z����l
					pPlayer->SetLandOffsetRot(OffsetRot);              // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g����
					pPlayer->SetLandOffsetRotStart(OffsetRotStart);    // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g�����ɉ��Z����l
				}
				else if (pRideObject->GetObjType() == OBJTYPE_ENEMY)
				{// ����Ă����I�u�W�F�N�g���G������
					CEnemy *pEnemy = (CEnemy*)pRideObject;

					// �K�v�Ȓl���擾����
					CEnemy::STATE State = pEnemy->GetState();                      // ���
					D3DXVECTOR3 Rot = pEnemy->GetRot();                            // ���݂̌���
					D3DXVECTOR3 DestRot = pEnemy->GetDestRot();                    // �ړI�̌���
					D3DXVECTOR3 OffsetPos = pEnemy->GetLandOffsetPos();            // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g����
					D3DXVECTOR3 OffsetAdd = pEnemy->GetLandOffsetAdd();            // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g�����ɉ��Z����l
					D3DXVECTOR3 OffsetRot = pEnemy->GetLandOffsetRot();            // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g����
					D3DXVECTOR3 OffsetRotStart = pEnemy->GetLandOffsetRotStart();  // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g�����ɉ��Z����l
					D3DXVECTOR3 ObjRot = GetRot();                                 // �I�u�W�F�N�g�̌���


					//if (pEnemy->GetLand() == false)
					//{// �O�����Ă��Ȃ�����
					//	// ������ۑ�
					//	OffsetPos = D3DXVECTOR3(pPos->x - pos.x, 0.0f, pPos->z - pos.z); // �I�t�Z�b�g���W��ۑ�
					//	OffsetRot = Rot - ObjRot;                                        // �v���C���[�̃I�t�Z�b�g������ۑ�
					//	if (OffsetRot.y > D3DX_PI)
					//	{// �~�����𒴂���
					//		OffsetRot.y -= D3DX_PI * 2.0f;
					//	}
					//	if (OffsetRot.y < -D3DX_PI)
					//	{// �~�����𒴂���
					//		OffsetRot.y += D3DX_PI * 2.0f;
					//	}
					//	OffsetRotStart = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �I�u�W�F�N�g�̃X�^�[�g������������
					//	OffsetAdd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �I�t�Z�b�g�ɉ�����l��������
					//}


					//// ��������]�ɉ��킹��
					//if (State != CPlayer::STATE_MOVE)
					//{// �ړ���Ԃł͂Ȃ�
					//	Rot = OffsetRot + ObjRot;    // ������␳
					//	if (Rot.y > D3DX_PI)
					//	{// �~�����𒴂���
					//		Rot.y -= D3DX_PI * 2.0f;
					//	}
					//	if (Rot.y < -D3DX_PI)
					//	{// �~�����𒴂���
					//		Rot.y += D3DX_PI * 2.0f;
					//	}
					//	DestRot.y = Rot.y;  // �ړI�̌������␳
					//}

					//// �I�t�Z�b�g�̌����ɔz�u���̉�]�X�s�[�h�����Z����
					//OffsetRotStart.y += m_RollSpeed.y;  // �I�u�W�F�N�g����]����X�s�[�h�ɍ��킹�ĕ␳
					//if (OffsetRotStart.y > D3DX_PI)
					//{// �~�����𒴂���
					//	OffsetRotStart.y -= D3DX_PI * 2.0f;
					//}
					//else if (OffsetRotStart.y < -D3DX_PI)
					//{// �~�����𒴂���
					//	OffsetRotStart.y += D3DX_PI * 2.0f;
					//}

					//// �ʒu��Y����]�ɉ��킹�Ĉړ�������
					//float fMtxX = cosf(OffsetRotStart.y) * (OffsetPos.x + (OffsetAdd.x * 1.5f)) + sinf(OffsetRotStart.y) * (OffsetPos.z + (OffsetAdd.z * 1.5f));
					//float fMtxZ = -sinf(OffsetRotStart.y) * (OffsetPos.x + (OffsetAdd.x * 1.5f)) + cosf(OffsetRotStart.y) * (OffsetPos.z + (OffsetAdd.z * 1.5f));
					//pPos->x = fMtxX + pos.x;
					//pPos->z = fMtxZ + pos.z;

					// �l��ݒ肷��
					pEnemy->SetRot(Rot);                              // ���݂̌���
					pEnemy->SetDestRot(DestRot);                      // �ړI�̌���
					pEnemy->SetLandOffsetPos(OffsetPos);              // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g����
					pEnemy->SetLandOffsetAdd(OffsetAdd);              // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g�����ɉ��Z����l
					pEnemy->SetLandOffsetRot(OffsetRot);              // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g����
					pEnemy->SetLandOffsetRotStart(OffsetRotStart);    // ����Ă���I�u�W�F�N�g����̃I�t�Z�b�g�����ɉ��Z����l
				}
				// ����Ă��锻��ɂ���
				bLand = true;
			}
			else if (pPosOld->y + fHeight <= pos.y + VtxMin.y && pPos->y + fHeight > pos.y + VtxMin.y)
			{// �z�u���ɉ����������
				pPos->y = pos.y + VtxMin.y - fHeight;
				pMove->y = 0.0f;
			}
			else
			{// �z�u���ɉ�������ォ��������Ă��Ȃ�
				if (pPos->y <= pos.y + VtxMax.y && pPos->y + fHeight >= pos.y + VtxMin.y)
				{// �I�u�W�F�N�g��Y���̊Ԃɂ���
					if (Rot.y != 0.0f)
					{// ��������]���Ă��Ȃ�(��`�Ŕ�������)
						for (int nCntVec = 0; nCntVec < 4; nCntVec++)
						{// ���肷��x�N�g���̐������J��Ԃ�
							vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
							vecB = *pPosOld - ObjectCollisionWorldPos[nCntVec];
							if ((vecA.x * vecB.z) - (vecA.z * vecB.x) >= -1)
							{// �x�N�g���̍����ɂ���
								// �O��̍��W�𗘗p���Ĕz�u���Ƃ̃x�N�g���������A���ɂ���x�N�g���̌�_�����߂�
								D3DXVECTOR3 Pos1 = *pPosOld;                                    // �v���C���[�̑O��̍��W
								D3DXVECTOR3 Pos2 = pos;                                         // �z�u���̍��W
								D3DXVECTOR3 Pos3 = ObjectCollisionWorldPos[(nCntVec + 1) % 4];  // �x�N�g������Ɉ��������������_���W(1��)
								D3DXVECTOR3 Pos4 = ObjectCollisionWorldPos[nCntVec];            // �x�N�g������Ɉ��������������_���W(2��)

								// �����̕���������A����������g�݁AXZ���ʏ�̌�_�����߂�
								float VecLine1 = (Pos2.z - Pos1.z) / (Pos2.x - Pos1.x);
								float VecLine2 = (Pos4.z - Pos3.z) / (Pos4.x - Pos3.x);
								float XPos = (VecLine1 * Pos1.x - Pos1.z - VecLine2 * Pos3.x + Pos3.z) / (VecLine1 - VecLine2);
								float ZPos = (Pos2.z - Pos1.z) / (Pos2.x - Pos1.x) * (XPos - Pos1.x) + Pos1.z;
								pPos->x = XPos;
								pPos->z = ZPos;
							}
						}
					}

					if (Rot.y == 0.0f)
					{// ��������]���Ă��Ȃ�(��`�Ŕ�������)
						if (pPos->z - fRange <= pos.z + VtxMax.z && pPos->z + fRange >= pos.z + VtxMin.z)
						{// �I�u�W�F�N�g��Z���̊Ԃɂ���
							if (pPosOld->x + fRange <= pos.x + VtxMin.x && pPos->x + fRange >= pos.x + VtxMin.x)
							{// �����炠������
								pPos->x = pos.x + VtxMin.x - fRange;
							}
							else if (pPosOld->x - fRange >= pos.x + VtxMax.x && pPos->x - fRange <= pos.x + VtxMax.x)
							{// �E���炠������
								pPos->x = pos.x + VtxMax.x + fRange;
							}
						}
						if (pPos->x - fRange <= pos.x + VtxMax.x && pPos->x + fRange >= pos.x + VtxMin.x)
						{// �I�u�W�F�N�g��X���̊Ԃɂ���
							if (pPosOld->z + fRange <= pos.z + VtxMin.z && pPos->z + fRange >= pos.z + VtxMin.z)
							{// ��납�炠������
								pPos->z = pos.z + VtxMin.z - fRange;
							}
							else if (pPosOld->z - fRange >= pos.z + VtxMax.z && pPos->z - fRange <= pos.z + VtxMax.z)
							{// �O���炠������
								pPos->z = pos.z + VtxMax.z + fRange;
							}
						}
					}
				}
			}
		}
	}

	return bLand;
}

//=============================================================================
//    �z�u���̐F�𓧖��ɂ��鏈��
//=============================================================================
void CObject::SetTransParent(float fAlpha)
{
	LPD3DXBUFFER pBuffMat = GetBuffMat();  // �}�e���A�����ւ̃|�C���^
	DWORD nNumMat = GetNumMat();           // �}�e���A�����̐�
	D3DXMATERIAL *pMat;                    // �}�e���A���f�[�^�ւ̃|�C���^

	if (pBuffMat != NULL)
	{// �}�e���A�����ւ̃|�C���^���擾�ł���
	    // �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)nNumMat; nCntMat++)
		{// �ݒ肳��Ă����}�e���A���̐������J��Ԃ�
			pMat[nCntMat].MatD3D.Diffuse.a = fAlpha;
		}
	}
}

//=============================================================================
//    �z�u���̉�]�X�s�[�h�ݒ菈��
//=============================================================================
void CObject::SetRollSpeed(const D3DXVECTOR3 RollSpeed)
{
	m_RollSpeed = RollSpeed;
}

//=============================================================================
//    �z�u���̑O��̍��W�ݒ菈��
//=============================================================================
void CObject::SetPosOld(const D3DXVECTOR3 PosOld)
{
	m_PosOld = PosOld;
}

//=============================================================================
//    �z�u���̌���(������)�ݒ菈��
//=============================================================================
void CObject::SetRotStd(const D3DXVECTOR3 RotStd)
{
	m_RotStd = RotStd;
}

//=============================================================================
//    �z�u����`�悷�邩���Ȃ�������
//=============================================================================
void CObject::SetDisp(const bool bDisp)
{
	m_bDisp = bDisp;
}

//=============================================================================
//    �z�u���̉�]�X�s�[�h�擾����
//=============================================================================
D3DXVECTOR3 CObject::GetRollSpeed(void)
{
	return m_RollSpeed;
}

//=============================================================================
//    �z�u���̑O��̍��W�擾����
//=============================================================================
D3DXVECTOR3 CObject::GetPosOld(void)
{
	return m_PosOld;
}

//=============================================================================
//    �z�u���̌���(������)����
//=============================================================================
D3DXVECTOR3 CObject::GetRotStd(void)
{
	return m_RotStd;
}

//=============================================================================
//    �z�u���̌���(������)����
//=============================================================================
bool CObject::GetDisp(void)
{
	return m_bDisp;
}

//*****************************************************************************
//     CObjectMove�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CObjectMove::CObjectMove(int nPriority, OBJTYPE objType) : CObject(nPriority, objType)
{
	// �e��l�̐ݒ�
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �ړ���
	m_MoveCurrent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���݂̈ړ���
	m_nCounter = 0;                                 // �ړ��ʂ��Ǘ�����J�E���^�[
	m_nMaxMoveCounter = 0;                          // �ړ��ʂ��ő�l�ɂȂ鎞��
	m_fRivisionMove = 0.0f;                         // �ړ��̔{��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CObjectMove::~CObjectMove()
{

}

//=============================================================================
//    �����z�u���̐�������
//=============================================================================
CObjectMove *CObjectMove::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter, int nPriority)
{
	CObjectMove *pObjectMove = NULL;   // �z�u���N���X�^�̃|�C���^
	if (pObjectMove == NULL)
	{// ����������ɂȂ��Ă���
		pObjectMove = new CObjectMove(nPriority);
		if (pObjectMove != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pObjectMove->Init(pos, rot, RollSpeed, Move, nMaxMoveCounter)))
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

	return pObjectMove;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �����z�u���̏���������
//=============================================================================
HRESULT CObjectMove::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter)
{
	// �e��l�̐ݒ�
	m_Move = Move;                                              // �ړ���
	m_nMaxMoveCounter = nMaxMoveCounter;                        // �ړ��ʂ��ő�l�ɂȂ鎞��
	m_nCounter = nMaxMoveCounter / 2;                           // �ړ��ʂ��Ǘ�����J�E���^�[
	m_fRivisionMove = (D3DX_PI / nMaxMoveCounter) * m_nCounter; // �ړ��ʂ�␳����{��
	m_PosStd = pos;                                             // ���f���̍��W(������)

	// ���ʂ̏���������
	if (FAILED(CObject::Init(pos, rot, RollSpeed)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �����z�u���̏I������
//=============================================================================
void CObjectMove::Uninit(void)
{
	// ���ʂ̏I������
	CObject::Uninit();
}

//=============================================================================
//    �����z�u���̍X�V����
//=============================================================================
void CObjectMove::Update(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();        // ���݂̍��W
	D3DXVECTOR3 posOld = GetPosOld();  // �O��̍��W
	D3DXVECTOR3 rot = GetRot();        // ���݂̌���

	// �O��̍��W�X�V
	posOld = pos;

	// �ړ��ʂ𔽓]
	m_nCounter++;
	if (m_nMaxMoveCounter != 0)
	{// �ő�l��0�ł͂Ȃ�
		if (m_nCounter % m_nMaxMoveCounter == 0)
		{
			m_fRivisionMove = 0.0f;
			m_Move *= -1;
			m_nCounter = 0;
		}

		// �ړ�������
		m_fRivisionMove += D3DX_PI / m_nMaxMoveCounter;

		// �ړ��ʂ�ݒ�
		m_MoveCurrent.x = sinf(m_fRivisionMove) * m_Move.x;
		m_MoveCurrent.y = sinf(m_fRivisionMove) * m_Move.y;
		m_MoveCurrent.z = sinf(m_fRivisionMove) * m_Move.z;
		pos += m_MoveCurrent;

		if (m_fRivisionMove > D3DX_PI)
		{
			m_fRivisionMove -= D3DX_PI * 2.0f;
		}
	}

	// ���W�̐ݒ�
	SetPos(pos);
	SetPosOld(posOld);

	// ��]������
	rot += GetRollSpeed();

	// �����`�F�b�N
	// X��
	if (rot.x > D3DX_PI)
	{// �������~�����𒴂���
		rot.x -= D3DX_PI * 2.0f;
	}
	if (rot.x < -D3DX_PI)
	{// �������~�����𒴂���
		rot.x += D3DX_PI * 2.0f;
	}

	// Y��
	if (rot.y > D3DX_PI)
	{// �������~�����𒴂���
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// �������~�����𒴂���
		rot.y += D3DX_PI * 2.0f;
	}

	// Z��
	if (rot.z > D3DX_PI)
	{// �������~�����𒴂���
		rot.z -= D3DX_PI * 2.0f;
	}
	if (rot.z < -D3DX_PI)
	{// �������~�����𒴂���
		rot.z += D3DX_PI * 2.0f;
	}

	// �����̐ݒ�
	SetRot(rot);
}

//=============================================================================
//    �����z�u���̕`�揈��
//=============================================================================
void CObjectMove::Draw(void)
{
	// ���ʂ̕`�揈��
	CObject::Draw();
}

//=============================================================================
//    �����z�u���̓����蔻�菈��
//=============================================================================
bool CObjectMove::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject)
{
	bool bLand = false;                // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ���
	D3DXVECTOR3 pos = GetPos();        // ���݂̍��W
	D3DXVECTOR3 posOld = GetPosOld();  // �O��̍��W
	D3DXVECTOR3 rot = GetRot();        // ���݂̌���
	D3DXVECTOR3 VtxMin = GetVtxMin();  // �ŏ��̒��_���W
	D3DXVECTOR3 VtxMax = GetVtxMax();  // �ő�̒��_���W

	if (pPos->x <= pos.x + VtxMax.x && pPos->x >= pos.x + VtxMin.x)
	{// �z�u���̒��ɂ���
		if (pPos->y >= posOld.y + VtxMax.y && pPos->y <= pos.y + VtxMax.y
			|| pPosOld->y >= posOld.y + VtxMax.y && pPosOld->y <= pos.y + VtxMax.y
			|| posOld.y + VtxMax.y <= pPosOld->y && pos.y + VtxMax.y >= pPos->y
			|| m_Move.y <= 0.0f && posOld.y + VtxMax.y <= pPosOld->y && pos.y + VtxMax.y >= pPos->y)
		{
			pPos->y = pos.y + VtxMax.y + m_MoveCurrent.y;
			pMove->y = 0.0f;
			bLand = true;
		}
	}

	return bLand;    // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ�����Ԃ�
}

//=============================================================================
//    �����z�u���̈ړ��ʐݒ菈��
//=============================================================================
void CObjectMove::SetMove(const D3DXVECTOR3 Move)
{
	m_Move = Move;
}

//=============================================================================
//    �����z�u���̌��݂̈ړ��ʐݒ菈��
//=============================================================================
void CObjectMove::SetMoveCurrent(const D3DXVECTOR3 MoveCurrent)
{
	m_MoveCurrent = MoveCurrent;
}

//=============================================================================
//    �����z�u���̈ړ��ʂ��Ǘ�����J�E���^�[�ݒ菈��
//=============================================================================
void CObjectMove::SetCounter(const int nCounter)
{
	m_nCounter = nCounter;
}

//=============================================================================
//    �����z�u���̈ړ��ʂ�؂�ւ��鎞�Ԑݒ菈��
//=============================================================================
void CObjectMove::SetMaxMoveCounter(const int nMaxMoveCounter)
{
	m_nMaxMoveCounter = nMaxMoveCounter;
}

//=============================================================================
//    �����z�u���̈ړ��ʂ̔{������
//=============================================================================
void CObjectMove::SetRivisionMove(const float fRivisionMove)
{
	m_fRivisionMove = fRivisionMove;
}

//=============================================================================
//    �����z�u���̍��W(������)�擾����
//=============================================================================
void CObjectMove::SetPosStd(const D3DXVECTOR3 PosStd)
{
	m_PosStd = PosStd;
}

//=============================================================================
//    �����z�u���̈ړ��ʎ擾����
//=============================================================================
D3DXVECTOR3 CObjectMove::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    �����z�u���̌��݂̈ړ��ʎ擾����
//=============================================================================
D3DXVECTOR3 CObjectMove::GetMoveCurrent(void)
{
	return m_MoveCurrent;
}

//=============================================================================
//    �����z�u���̈ړ��ʂ��Ǘ�����J�E���^�[����
//=============================================================================
int CObjectMove::GetCounter(void)
{
	return m_nCounter;
}

//=============================================================================
//    �����z�u���̈ړ��ʂ�؂�ւ��鎞�Ԏ擾����
//=============================================================================
int CObjectMove::GetMaxMoveCounter(void)
{
	return m_nMaxMoveCounter;
}

//=============================================================================
//    �����z�u���̈ړ��ʂ̔{���擾����
//=============================================================================
float CObjectMove::GetRivisionMove(void)
{
	return m_fRivisionMove;
}

//=============================================================================
//    �����z�u���̍��W(������)�擾����
//=============================================================================
D3DXVECTOR3 CObjectMove::GetPosStd(void)
{
	return m_PosStd;
}

//*****************************************************************************
//     CObjecctGoal�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CObjectGoal::CObjectGoal(int nPriority, OBJTYPE objType) : CObject(nPriority, objType)
{
	// �e��l�̃N���A
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CObjectGoal::~CObjectGoal()
{

}

//=============================================================================
//    �S�[���p�I�u�W�F�N�g�̐�������
//=============================================================================
CObjectGoal *CObjectGoal::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, int nPriority)
{
	CObjectGoal *pObjectGoal = NULL;   // �S�[���p�z�u���N���X�^�̃|�C���^
	if (pObjectGoal == NULL)
	{// ����������ɂȂ��Ă���
		pObjectGoal = new CObjectGoal(nPriority);
		if (pObjectGoal != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pObjectGoal->Init(pos, rot, RollSpeed)))
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

	return pObjectGoal;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �S�[���p�z�u���̏���������
//=============================================================================
HRESULT CObjectGoal::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed)
{
	// ���ʂ̏���������
	CObject::Init(pos, rot, RollSpeed);

	return S_OK;
}

//=============================================================================
//    �S�[���p�z�u���̏I������
//=============================================================================
void CObjectGoal::Uninit(void)
{
	// ���ʂ̏I������
	CObject::Uninit();
}

//=============================================================================
//    �S�[���p�z�u���̍X�V����
//=============================================================================
void CObjectGoal::Update(void)
{

}

//=============================================================================
//    �S�[���p�z�u���̕`�揈��
//=============================================================================
void CObjectGoal::Draw(void)
{
	// ���ʂ̕`�揈��
	CObject::Draw();
}

//=============================================================================
//    �S�[���p�z�u���̔z�u���̓����蔻�菈��
//=============================================================================
bool CObjectGoal::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject)
{
	bool bLand = false;   // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ���
	 // �����蔻��ɕK�v�Ȓl��z�u������擾
	D3DXVECTOR3 pos = GetPos();          // ���݂̍��W
	D3DXVECTOR3 Rot = GetRot();          // ���݂̌���
	D3DXVECTOR3 VtxMin = GetVtxMin();    // ���_�̍��W�ŏ��l
	D3DXVECTOR3 VtxMax = GetVtxMax();    // ���_�̍��W�ő�l

	// �O�ς̔���ɕK�v�Ȓl��p��
	D3DXVECTOR3 ObjectCollisionPos[4] =
	{// �����蔻��p�̎l�p(���[�J�����W)
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMax.z + fRange),
		D3DXVECTOR3(VtxMax.x + fRange,0.0f,VtxMin.z - fRange),
		D3DXVECTOR3(VtxMin.x - fRange,0.0f,VtxMin.z - fRange),
	};

	D3DXVECTOR3 ObjectCollisionWorldPos[4];  // �����蔻��p�̎l�p(���[���h���W)
	D3DXMATRIX ObjectCollisionMtxWorld;      // �v�Z�p�}�g���b�N�X

	for (int nCntCol = 0; nCntCol < 4; nCntCol++)
	{// ���肷��l�p�̍��W���J��Ԃ�
		D3DXMATRIX ObjectCollisionMtxWorld, mtxRot, mtxTrans;  // �v�Z�p�}�g���b�N�X

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&ObjectCollisionMtxWorld);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Rot.y, Rot.x, Rot.z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, ObjectCollisionPos[nCntCol].x, ObjectCollisionPos[nCntCol].y, ObjectCollisionPos[nCntCol].z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxTrans);

		// �e�̏����|�����킹��
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &GetMtxWorld());

		// ���[���h���W�𔲂��o��
		ObjectCollisionWorldPos[nCntCol] = D3DXVECTOR3(ObjectCollisionMtxWorld._41, ObjectCollisionMtxWorld._42, ObjectCollisionMtxWorld._43);
	}

	D3DXVECTOR3 vecA, vecB;        // ����p�x�N�g��
	int nCntVecTrue = 0;           // �x�N�g���̔����ʂ�����
	bool bObjectRand = false;      // �I�u�W�F�N�g�̒��ɂ��邩�ǂ���
	for (int nCntVec = 0; nCntVec < 4; nCntVec++)
	{// ���肷��x�N�g���̐������J��Ԃ�
		vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
		vecB = *pPos - ObjectCollisionWorldPos[nCntVec];

		if ((vecA.x * vecB.z) - (vecA.z * vecB.x) <= 0)
		{// �x�N�g���̉E���ɂ���
			nCntVecTrue++;   // �����ʂ����񐔂����Z
			if (nCntVecTrue == 4)
			{// �S�Ẵx�N�g���̉E���ɂ���
				bObjectRand = true;
			}
		}
	}

	if (bObjectRand == true)
	{// �z�u���̒��ɂ���
		if (pPos->y <= pos.y + VtxMax.y && pPos->y + 50.0f >= pos.y + VtxMin.y)
		{
			CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GOALHIT);
			if (CManager::GetMode() == CManager::MODE_GAME)
			{// �Q�[�����[�h����������
				if (CManager::GetGame()->GetNumStageClear() < 2)
				{
					CManager::GetGame()->SetState(CGame::STATE_MAPCHANGE);
				}
				else
				{
					if (CManager::GetGame()->GetState() != CGame::STATE_END)
					{
						CManager::GetGame()->SetState(CGame::STATE_END);
						CResult::SetResult(CResult::RESULT_CLEAR);
					}
				}
			}
		}
	}

	return bLand;
}