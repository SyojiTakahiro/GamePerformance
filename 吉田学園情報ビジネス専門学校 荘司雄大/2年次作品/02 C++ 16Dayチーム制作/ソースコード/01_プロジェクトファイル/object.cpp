//*****************************************************************************
//
//     �z�u���̏���[object.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "input.h"
#include "debuglog.h"

#include "player.h"
#include "shadow.h"
#include "debuglog.h"
#include "balloon.h"
#include "particle.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
// �����z�u���p
#define OBJECT_MOVE_TIMING            (180)

// �����o���|���S�����o���ʒu
#define OBJECT_BALLOON_POLYGON_WIDTH  (300.0f)
#define OBJECT_BALLOON_POLYGON_HEIGHT (130.0f)
#define OBJECT_BALLOON_POLYGON_POS    (D3DXVECTOR3((SCREEN_WIDTH / 2),OBJECT_BALLOON_POLYGON_HEIGHT,0.0f))
#define OBJECT_BALLOON_RANGE          (100.0f)

#define OBJECT_MODEL_FILENAME   "data/MODEL/object/object.x"  // �z�u�����f���̃t�@�C����

//*****************************************************************************
//     �ÓI�����o�ϐ�
//*****************************************************************************
LPD3DXMESH     CObject::m_apMesh = NULL;                                                     // ���b�V�����ւ̃|�C���^
LPD3DXBUFFER   CObject::m_apBuffMat = NULL;                                                  // �}�e���A�����ւ̃|�C���^
DWORD          CObject::m_anNumMat = NULL;                                                   // �}�e���A�����̐�
D3DXVECTOR3    CObject::m_aVtxMin = D3DXVECTOR3(100000.0f, 1000000.0f, 100000.0f);           // ���_���W�̍ŏ��l
D3DXVECTOR3    CObject::m_aVtxMax = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f);        // ���_���W�̍ŏ��l


LPD3DXMESH     CObjectMove::m_apMesh = NULL;                                                 // ���b�V�����ւ̃|�C���^
LPD3DXBUFFER   CObjectMove::m_apBuffMat = NULL;                                              // �}�e���A�����ւ̃|�C���^
DWORD          CObjectMove::m_anNumMat = NULL;                                               // �}�e���A�����̐�
D3DXVECTOR3    CObjectMove::m_aVtxMin = D3DXVECTOR3(100000.0f, 1000000.0f, 100000.0f);       // ���_���W�̍ŏ��l
D3DXVECTOR3    CObjectMove::m_aVtxMax = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f);    // ���_���W�̍ŏ��l

LPD3DXMESH     CObjectBalloon::m_apMesh = NULL;                                              // ���b�V�����ւ̃|�C���^
LPD3DXBUFFER   CObjectBalloon::m_apBuffMat = NULL;                                           // �}�e���A�����ւ̃|�C���^
DWORD          CObjectBalloon::m_anNumMat = NULL;                                            // �}�e���A�����̐�
D3DXVECTOR3    CObjectBalloon::m_aVtxMin = D3DXVECTOR3(100000.0f, 1000000.0f, 100000.0f);    // ���_���W�̍ŏ��l
D3DXVECTOR3    CObjectBalloon::m_aVtxMax = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f); // ���_���W�̍ŏ��l

LPD3DXMESH     CObjectGoal::m_apMesh = NULL;                                                 // ���b�V�����ւ̃|�C���^
LPD3DXBUFFER   CObjectGoal::m_apBuffMat = NULL;                                              // �}�e���A�����ւ̃|�C���^
DWORD          CObjectGoal::m_anNumMat = NULL;                                               // �}�e���A�����̐�
D3DXVECTOR3    CObjectGoal::m_aVtxMin = D3DXVECTOR3(100000.0f, 1000000.0f, 100000.0f);       // ���_���W�̍ŏ��l
D3DXVECTOR3    CObjectGoal::m_aVtxMax = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f);    // ���_���W�̍ŏ��l

LPD3DXMESH     CObjectTarget::m_apMesh = NULL;                                               // ���b�V�����ւ̃|�C���^
LPD3DXBUFFER   CObjectTarget::m_apBuffMat = NULL;                                            // �}�e���A�����ւ̃|�C���^
DWORD          CObjectTarget::m_anNumMat = NULL;                                             // �}�e���A�����̐�
D3DXVECTOR3    CObjectTarget::m_aVtxMin = D3DXVECTOR3(100000.0f, 1000000.0f, 100000.0f);     // ���_���W�̍ŏ��l
D3DXVECTOR3    CObjectTarget::m_aVtxMax = D3DXVECTOR3(-100000.0f, -1000000.0f, -100000.0f);  // ���_���W�̍ŏ��l

//*****************************************************************************
//     CObjecct�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CObject::CObject(int nPriority, OBJTYPE objType) : CSceneX(nPriority, objType)
{
	// �e��l�̃N���A
	m_pShadow = NULL;    // �e�N���X�ւ̃|�C���^
	m_bCollision = true; // �����蔻�����邩�ǂ���
	m_bBreak = false;    // �󂹂邩�ǂ���
	m_bPush = false;     // �����邩�ǂ���
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
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, bool bPush, bool bBreak, int nPriority)
{
	CObject *pObject = NULL;   // �z�u���N���X�^�̃|�C���^
	if (pObject == NULL)
	{// ����������ɂȂ��Ă���
		pObject = new CObject(nPriority);
		if (pObject != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pObject->Init(pos, rot, RollSpeed, bCollision, bPush, bBreak)))
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
HRESULT CObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, bool bPush, bool bBreak)
{
	// �e��l�̐ݒ�
	SetPos(pos);                // ���݂̍��W
	SetRot(rot);                // ���݂̌���
	SetVtxMin(m_aVtxMin);       // ���_���W�̍ŏ��l
	SetVtxMax(m_aVtxMax);       // ���_���W�̍ő�l
	m_RollSpeed = RollSpeed;    // ��]����X�s�[�h
	m_bCollision = bCollision;  // �����蔻�����邩�ǂ���
	m_bPush = bPush;            // �����邩�ǂ���
	m_bBreak = bBreak;          // �󂹂邩�ǂ���

	// ���ʂ̏���������
	CSceneX::Init();

	// �e�̐���
	if (m_pShadow == NULL)
	{// ���������m�ۂł����Ԃł���
		m_pShadow = CShadow::Create(D3DXVECTOR3(pos.x, 0.0f, pos.z), rot, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.75f), 70.0f, 4);
	}

	return S_OK;
}

//=============================================================================
//    �z�u���̏I������
//=============================================================================
void CObject::Uninit(void)
{
	// �e�̏I������
	if (m_pShadow != NULL)
	{// ���������m�ۂ���Ă���
		m_pShadow->Uninit();
		m_pShadow = NULL;
	}

	// ���ʂ̏I������
	CSceneX::Uninit();
}

//=============================================================================
//    �z�u���̍X�V����
//=============================================================================
void CObject::Update(void)
{
	D3DXVECTOR3 rot = GetRot();

	rot.y += m_RollSpeed.y;
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
//    �z�u���̕`�揈��
//=============================================================================
void CObject::Draw(void)
{
	// ���ʂ̕`�揈��
	CSceneX::Draw();
}

//=============================================================================
//    �z�u���̓����蔻�菈��
//=============================================================================
bool CObject::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, CShadow *pShadow, float fHeight, float fRange)
{
	bool bLand = false;   // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ���

	// �����蔻��ɕK�v�Ȓl��z�u������擾
	D3DXVECTOR3 pos = GetPos();          // ���݂̍��W
	D3DXVECTOR3 posOld = GetPosOld();    // �O��̍��W
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
	    // �e�̈ʒu�����炷
		if (pShadow != NULL)
		{// �e����������Ă���
			if (pPos->y >= GetPos().y + VtxMax.y || pPosOld->y >= GetPos().y + VtxMax.y)
			{// �z�u���̏�ɂ���
				// �ʒu�����炷
				pShadow->SetPos(D3DXVECTOR3(pPos->x, pos.y + VtxMax.y + 1.0f, pPos->z));

				// ���_���̐ݒ�
				VERTEX_3D *pVtx;
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = pShadow->GetVtxBuff();
				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				for (int nCntVer = 0; nCntVer < 4; nCntVer++)
				{// �e�̒��_�����J��Ԃ�
					pVtx[nCntVer].pos.y = 0.0f; // �z�u���̏�ɉe���悹��̂ŃI�t�Z�b�g������0��
				}

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();

				// ���_�o�b�t�@�̐ݒ�
				pShadow->SetVtxBuff(pVtxBuff);
			}
		}

		// ����Ă��邩�ǂ����̔���J�n
		if (pPosOld->y >= pos.y + VtxMax.y && pPos->y <= pos.y + VtxMax.y)
		{// �z�u���ɏォ�������
			pPos->y = pos.y + VtxMax.y;
			pMove->y = 0.0f;
			bLand = true;
		}
		else if (pPosOld->y + fHeight <= pos.y + VtxMin.y && pPos->y + fHeight > pos.y + VtxMin.y)
		{// �z�u���ɉ����������
			pPos->y = pos.y + VtxMin.y - fHeight;
			pMove->y = 0.0f;
		}
	}

	// �e��l�̐ݒ�
	SetPos(pos);   // ���݂̍��W

	return bLand;   // ����Ă��邩�ǂ�����Ԃ�
}

//=============================================================================
//    �z�u���ɓ������Ă��邩�ǂ������肷�鏈��
//=============================================================================
bool CObject::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange)
{
	bool bCol = false;  // �������Ă��邩�ǂ���

	// �����蔻��ɕK�v�Ȓl��z�u������擾
	D3DXVECTOR3 pos = GetPos();          // ���݂̍��W
	D3DXVECTOR3 posOld = GetPosOld();    // �O��̍��W
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
	{// �x�N�g���͈͓̔��ɂ���
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
						if (m_bPush == true)
						{// ������I�u�W�F�N�g�ł���
							pos.x += pMove->x;
							pos.z += pMove->z;
						}
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
						bCol = true;
					}
				}
			}

			if (Rot.y == 0.0f)
			{// ��������]���Ă��Ȃ�(��`�Ŕ�������)
				if (pPos->z - fRange <= pos.z + VtxMax.z && pPos->z + fRange >= pos.z + VtxMin.z)
				{// �I�u�W�F�N�g��Z���̊Ԃɂ���
					if (pPosOld->x + fRange <= pos.x + VtxMin.x && pPos->x + fRange >= pos.x + VtxMin.x)
					{// �����炠������
						if (m_bPush == true)
						{// ������I�u�W�F�N�g�ł���
							float Move = 0.0f;
							if (pMove->y <= 0.0f)
							{// �W�����v���Ă��Ȃ�
								Move = pMove->x;
							}
							pPos->x = pos.x + VtxMin.x - fRange + Move;
							if (pMove->y <= 0.0f)
							{// �W�����v���Ă��Ȃ�
								Move = pMove->x * 1.1f;
							}
							pos.x += Move;
						}
						else
						{// �����Ȃ��I�u�W�F�N�g�ł���
							pPos->x = pos.x + VtxMin.x - fRange;
						}
						bCol = true;
					}
					else if (pPosOld->x - fRange >= pos.x + VtxMax.x && pPos->x - fRange <= pos.x + VtxMax.x)
					{// �E���炠������
						if (m_bPush == true)
						{// ������I�u�W�F�N�g�ł���
							float Move = 0.0f;
							if (pMove->y <= 0.0f)
							{// �W�����v���Ă��Ȃ�
								Move = pMove->x;
							}
							pPos->x = pos.x + VtxMax.x + fRange + Move;
							if (pMove->y <= 0.0f)
							{// �W�����v���Ă��Ȃ�
								Move = pMove->x * 1.1f;
							}
							pos.x += Move;
						}
						else
						{// �����Ȃ��I�u�W�F�N�g�ł���
							pPos->x = pos.x + VtxMax.x + fRange;
						}
						bCol = true;
					}
				}
				if (pPos->x - fRange <= pos.x + VtxMax.x && pPos->x + fRange >= pos.x + VtxMin.x)
				{// �I�u�W�F�N�g��X���̊Ԃɂ���
					if (pPosOld->z + fRange <= pos.z + VtxMin.z && pPos->z + fRange >= pos.z + VtxMin.z)
					{// ��납�炠������
						if (m_bPush == true)
						{// ������I�u�W�F�N�g�ł���
							float Move = 0.0f;
							if (pMove->y <= 0.0f)
							{// �W�����v���Ă��Ȃ�
								Move = pMove->z;
							}
							pPos->z = pos.z + VtxMin.z - fRange + Move;
							if (pMove->y <= 0.0f)
							{// �W�����v���Ă��Ȃ�
								Move = pMove->z * 1.1f;
							}
							pos.z += Move;
						}
						else
						{// �����Ȃ��I�u�W�F�N�g�ł���
							pPos->z = pos.z + VtxMin.z - fRange;
						}
						bCol = true;
					}
					else if (pPosOld->z - fRange >= pos.z + VtxMax.z && pPos->z - fRange <= pos.z + VtxMax.z)
					{// �O���炠������
						if (m_bPush == true)
						{// ������I�u�W�F�N�g�ł���
							float Move = 0.0f;
							if (pMove->y <= 0.0f)
							{// �W�����v���Ă��Ȃ�
								Move = pMove->z;
							}
							pPos->z = pos.z + VtxMax.z + fRange + Move;
							if (pMove->y <= 0.0f)
							{// �W�����v���Ă��Ȃ�
								Move = pMove->z * 1.1f;
							}
							pos.z += Move;
						}
						else
						{// �����Ȃ��I�u�W�F�N�g�ł���
							pPos->z = pos.z + VtxMax.z + fRange;
						}
						bCol = true;
					}
				}
			}
		}
	}

	// �l�̐ݒ�
	SetPos(pos);

	return bCol;
}

//=============================================================================
//    �z�u���̌�������]������X�s�[�h�擾����
//=============================================================================
D3DXVECTOR3 CObject::GetRollSpeed(void)
{
	return m_RollSpeed;
}

//=============================================================================
//    �z�u���̉e�擾����
//=============================================================================
CShadow *CObject::GetShadow(void)
{
	return m_pShadow;
}

//=============================================================================
//    �z�u���Ɠ����蔻�����邩�ǂ����擾����
//=============================================================================
bool CObject::GetCollision(void)
{
	return m_bCollision;
}

//=============================================================================
//    �z�u�������邩�ǂ����擾����
//=============================================================================
bool CObject::GetBreak(void)
{
	return m_bBreak;
}

//=============================================================================
//    �z�u���������邩�ǂ����擾����
//=============================================================================
bool CObject::GetPush(void)
{
	return m_bPush;
}


//*****************************************************************************
//     CObjecctMove�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CObjectMove::CObjectMove(int nPriority, OBJTYPE objType) : CObject(nPriority, objType)
{
	// �e��l�̃N���A
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �ړ���
	m_MoveCurrent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���݂̈ړ���
	m_nMoveCounter = 0;                             // �ړ��ʊǗ��J�E���^�[
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CObjectMove::~CObjectMove()
{

}

//=============================================================================
//    �����I�u�W�F�N�g�̐�������
//=============================================================================
CObjectMove *CObjectMove::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 move, int nPriority)
{
	CObjectMove *pObjectMove = NULL;   // �����z�u���N���X�^�̃|�C���^
	if (pObjectMove == NULL)
	{// ����������ɂȂ��Ă���
		pObjectMove = new CObjectMove(nPriority);
		if (pObjectMove != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pObjectMove->Init(pos, rot, RollSpeed, bCollision, move)))
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
//    �����z�u���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CObjectMove::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // x�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(OBJECT_MODEL_FILENAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_apBuffMat,
				NULL,
				&m_anNumMat,
				&m_apMesh);

			int nNumVtx;     // ���_��
			DWORD sizeFVF;   // ���_�t�H�[�}�b�g�̃T�C�Y
			BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^

			// ���_�����l��
			nNumVtx = m_apMesh->GetNumVertices();

			// ���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(m_apMesh->GetFVF());

			// ���_�o�b�t�@�����b�N
			m_apMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// ���ׂĂ̒��_���r���ă��f���̍ŏ��l,�ő�l�𔲂��o��
				D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // ���_���W�̑��

				if (Vtx.x < m_aVtxMin.x)
				{// ���o����X���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.x = Vtx.x;  // �ŏ��l�X�V
				}
				if (Vtx.y < m_aVtxMin.y)
				{// ���o����Y���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.y = Vtx.y;  // �ŏ��l�X�V
				}
				if (Vtx.z < m_aVtxMin.z)
				{// ���o����Z���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.z = Vtx.z;  // �ŏ��l�X�V
				}
				if (Vtx.x > m_aVtxMax.x)
				{// ���o����X���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.x = Vtx.x;  // �ő�l�X�V
				}
				if (Vtx.y > m_aVtxMax.y)
				{// ���o����Y���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.y = Vtx.y;  // �ő�l�X�V
				}
				if (Vtx.z > m_aVtxMax.z)
				{// ���o����Z���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.z = Vtx.z;  // �ő�l�X�V
				}

				pVtxBuff += sizeFVF;  // �T�C�Y���|�C���^��i�߂�
			}
			// ���_�o�b�t�@���A�����b�N
			m_apMesh->UnlockVertexBuffer();
		}
	}
	return S_OK;
}

//=============================================================================
//    �����z�u���̊J������
//=============================================================================
void CObjectMove::UnLoad(void)
{
	// ���b�V���̔j��
	if (m_apMesh != NULL)
	{
		m_apMesh->Release();
		m_apMesh = NULL;
	}

	// �}�e���A���̔j��
	if (m_apBuffMat != NULL)
	{
		m_apBuffMat->Release();
		m_apBuffMat = NULL;
	}
}

//=============================================================================
//    �����z�u���̏���������
//=============================================================================
HRESULT CObjectMove::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 move)
{
	// ���ʂ̏���������
	CObject::Init(pos, rot, RollSpeed, bCollision, false, false);

	// �e��l�̐ݒ�
	m_Move = move;  // �ړ���

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
	m_nMoveCounter++;
	if (m_nMoveCounter % OBJECT_MOVE_TIMING == 0)
	{
		m_fMoveRivision = 0.0f;
		m_Move *= -1;
		m_nMoveCounter = 0;
	}

	// �ړ�������
	m_fMoveRivision += D3DX_PI / OBJECT_MOVE_TIMING;
	if(m_fMoveRivision > D3DX_PI)
	{
		m_fMoveRivision -= D3DX_PI * 2.0f;
	}
	m_MoveCurrent.x = sinf(m_fMoveRivision) * m_Move.x;
	m_MoveCurrent.y = sinf(m_fMoveRivision) * m_Move.y;
	m_MoveCurrent.z = sinf(m_fMoveRivision) * m_Move.z;
	pos += m_MoveCurrent;

	// ��������]������
	rot.y += GetRollSpeed().y;
	if (rot.y > D3DX_PI)
	{// �������~�����𒴂���
		rot.y -= D3DX_PI * 2.0f;
	}
	if (rot.y < -D3DX_PI)
	{// �������~�����𒴂���
		rot.y += D3DX_PI * 2.0f;
	}

	// �e��l�̐ݒ�
	SetPos(pos);        // ���݂̍��W
	SetPosOld(posOld);  // �O��̍��W
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
bool CObjectMove::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, CShadow *pShadow, float fHeight, float fRange)
{
	bool bLand = false;                // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ���
	D3DXVECTOR3 pos = GetPos();        // ���݂̍��W
	D3DXVECTOR3 posOld = GetPosOld();  // �O��̍��W
	D3DXVECTOR3 rot = GetRot();        // ���݂̌���
	D3DXVECTOR3 VtxMin = GetVtxMin();  // �ŏ��̒��_���W
	D3DXVECTOR3 VtxMax = GetVtxMax();  // �ő�̒��_���W

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
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, ObjectCollisionPos[nCntCol].x, ObjectCollisionPos[nCntCol].y, ObjectCollisionPos[nCntCol].z);
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &mtxTrans);

		// �e�̏����|�����킹��
		D3DXMatrixMultiply(&ObjectCollisionMtxWorld, &ObjectCollisionMtxWorld, &GetMtxWorld());

		// ���[���h���W�𔲂��o��
		ObjectCollisionWorldPos[nCntCol] = D3DXVECTOR3(ObjectCollisionMtxWorld._41, ObjectCollisionMtxWorld._42, ObjectCollisionMtxWorld._43);
	}

	D3DXVECTOR3 vecA, vecB;  // ����p�x�N�g��
	int nCntVecTrue = 0;     // �x�N�g���̔����ʂ�����
	bool bObject = false;    // �I�u�W�F�N�g�̒��ɂ��邩�ǂ���
	for (int nCntVec = 0; nCntVec < 4; nCntVec++)
	{// ���肷��x�N�g���̐������J��Ԃ�
		vecA = ObjectCollisionWorldPos[(nCntVec + 1) % 4] - ObjectCollisionWorldPos[nCntVec];
		vecB = *pPos - ObjectCollisionWorldPos[nCntVec];

		if ((vecA.x * vecB.z) - (vecA.z * vecB.x) <= 0)
		{// �x�N�g���̉E���ɂ���
			nCntVecTrue++;   // �����ʂ����񐔂����Z
			if (nCntVecTrue == 4)
			{// �S�Ẵx�N�g���̉E���ɂ���
				bObject = true;
				CDebugProc::Print("�����z�u���̒��ɂ��܂�\n");
			}
		}
	}

	if (bObject == true)
	{// �z�u���̒��ɂ���
	    // �e�̈ʒu�����炷
		if (pShadow != NULL)
		{// �e����������Ă���
			if (pPos->y >= GetPos().y + VtxMax.y - m_Move.y || pPosOld->y >= GetPos().y + VtxMax.y - m_Move.y
				|| pPos->y >= GetPos().y + VtxMax.y + m_Move.y || pPosOld->y >= GetPos().y + VtxMax.y + m_Move.y
				|| pPos->y >= GetPos().y + VtxMax.y || pPosOld->y >= GetPos().y + VtxMax.y)
			{// �z�u���̏�ɂ���
				// �ʒu�����炷
				pShadow->SetPos(D3DXVECTOR3(pPos->x, GetPos().y + VtxMax.y + 1.0f, pPos->z));

				// ���_���̐ݒ�
				VERTEX_3D *pVtx;
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = pShadow->GetVtxBuff();
				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				for (int nCntVer = 0; nCntVer < 4; nCntVer++)
				{// �e�̒��_�����J��Ԃ�
					pVtx[nCntVer].pos.y = 0.0f; // �����z�u���̏�ɉe���悹��̂ŃI�t�Z�b�g������0��
				}

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();

				// ���_�o�b�t�@�̐ݒ�
				pShadow->SetVtxBuff(pVtxBuff);
			}
		}

		// ����Ă��邩�ǂ����̔���J�n
		if (pPosOld->y >= pos.y + VtxMax.y && pPos->y <= pos.y + VtxMax.y
			|| m_Move.y >= 0.0f && pPosOld->y >= pos.y + VtxMax.y && pPos->y <= pos.y + VtxMax.y
			|| pPosOld->y >= posOld.y + VtxMax.y && pPos->y <= posOld.y + VtxMax.y
			|| m_Move.y >= 0.0f && pPosOld->y >= posOld.y + VtxMax.y && pPos->y <= posOld.y + VtxMax.y
			|| posOld.y + VtxMax.y <= pPos->y && pos.y + VtxMax.y >= pPos->y)
		{// �z�u���ɏォ�������
			pPos->y = pos.y + VtxMax.y + m_MoveCurrent.y;
			pMove->y = 0.0f;
			bLand = true;
		}
		else if (pPosOld->y + fHeight <= pos.y + VtxMin.y && pPos->y + fHeight > pos.y + VtxMin.y
			|| pPosOld->y + fHeight <= posOld.y + VtxMin.y && pPos->y + fHeight > posOld.y + VtxMin.y)
		{// �z�u���ɉ����������
			pPos->y = pos.y + VtxMin.y - fHeight;
			pMove->y = 0.0f;
		}
	}

	if (pPos->y <= pos.y + VtxMax.y && pPos->y + fHeight >= pos.y + VtxMin.y)
	{// �I�u�W�F�N�g��Y���̊Ԃɂ���
		if (bObject == true)
		{// �I�u�W�F�N�g�͈͓̔��ɂ���
			if (GetRot().y != 0.0f)
			{// �����ł���������]���Ă���(�x�N�g�����m�� = �ɂȂ��Ă��܂��̂ŕ����������藧���Ȃ�)
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
						CDebugProc::Print("%.1f\n", XPos);
						CDebugProc::Print("%.1f\n\n", ZPos);
					}
				}
			}
		}
		if (GetRot().y == 0.0f)
		{// ��������]���Ă��Ȃ�(��`�Ŕ�������)
			if (pPos->z <= pos.z + VtxMax.z && pPos->z >= pos.z + VtxMin.z)
			{// �I�u�W�F�N�g��Z���̊Ԃɂ���
				if (pPosOld->x <= pos.x + VtxMin.x && pPos->x >= pos.x + VtxMin.x
					|| posOld.x + VtxMin.x >= pPosOld->x && pos.x + VtxMin.x <= pPos->x)
				{// �����炠������
					pPos->x = pos.x + VtxMin.x;
				}
				else if (pPosOld->x >= pos.x + VtxMax.x && pPos->x <= pos.x + VtxMax.x
					|| posOld.x + VtxMax.x <= pPosOld->x && pos.x + VtxMax.x >= pPos->x)
				{// �E���炠������
					pPos->x = pos.x + VtxMax.x;
				}
			}
			if (pPos->x <= pos.x + VtxMax.x && pPos->x >= pos.x + VtxMin.x)
			{// �I�u�W�F�N�g��X���̊Ԃɂ���
				if (pPosOld->z <= pos.z + VtxMin.z && pPos->z >= pos.z + VtxMin.z
					|| posOld.z + VtxMin.z >= pPosOld->z && pos.z + VtxMin.z <= pPos->z)
				{// ��납�炠������
					pPos->z = pos.z + VtxMin.z;
				}
				else if (pPosOld->z >= pos.z + VtxMax.z && pPos->z <= pos.z + VtxMax.z
					|| posOld.z + VtxMax.z <= pPosOld->z && pos.z + VtxMax.z >= pPos->z)
				{// �O���炠������
					pPos->z = pos.z + VtxMax.z;
				}
			}
		}
	}

	return bLand;    // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ�����Ԃ�
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

//*****************************************************************************
//     CObjecctBalloon�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CObjectBalloon::CObjectBalloon(int nPriority, OBJTYPE objType) : CObject(nPriority, objType)
{
	// �e��l�̃N���A
	m_nCount = 0;
	m_nType = 0;
	m_MapBalloon = BALLOON_TYPE_0;
	for (int nCntBalloon = 0; nCntBalloon < BALLOON_TYPE_MAX; nCntBalloon++)
	{
		m_pBalloon[nCntBalloon] = NULL;
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CObjectBalloon::~CObjectBalloon()
{

}

//=============================================================================
//    �����o���p�I�u�W�F�N�g�̐�������
//=============================================================================
CObjectBalloon *CObjectBalloon::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, BALLOON MapBalloon, char *pNextMapName, int nPriority)
{
	CObjectBalloon *pObjectBalloon = NULL;   // �����o���p�z�u���N���X�^�̃|�C���^
	if (pObjectBalloon == NULL)
	{// ����������ɂȂ��Ă���
		pObjectBalloon = new CObjectBalloon(nPriority);
		if (pObjectBalloon != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pObjectBalloon->Init(pos, rot, RollSpeed, bCollision, pNextMapName)))
			{// �������Ɏ��s����
				return NULL;
			}
			pObjectBalloon->m_MapBalloon = MapBalloon;
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

	return pObjectBalloon;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �����o���p�z�u���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CObjectBalloon::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // x�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(OBJECT_MODEL_FILENAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_apBuffMat,
				NULL,
				&m_anNumMat,
				&m_apMesh);

			int nNumVtx;     // ���_��
			DWORD sizeFVF;   // ���_�t�H�[�}�b�g�̃T�C�Y
			BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^

			// ���_�����l��
			nNumVtx = m_apMesh->GetNumVertices();

			// ���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(m_apMesh->GetFVF());

			// ���_�o�b�t�@�����b�N
			m_apMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// ���ׂĂ̒��_���r���ă��f���̍ŏ��l,�ő�l�𔲂��o��
				D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // ���_���W�̑��

				if (Vtx.x < m_aVtxMin.x)
				{// ���o����X���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.x = Vtx.x;  // �ŏ��l�X�V
				}
				if (Vtx.y < m_aVtxMin.y)
				{// ���o����Y���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.y = Vtx.y;  // �ŏ��l�X�V
				}
				if (Vtx.z < m_aVtxMin.z)
				{// ���o����Z���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.z = Vtx.z;  // �ŏ��l�X�V
				}
				if (Vtx.x > m_aVtxMax.x)
				{// ���o����X���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.x = Vtx.x;  // �ő�l�X�V
				}
				if (Vtx.y > m_aVtxMax.y)
				{// ���o����Y���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.y = Vtx.y;  // �ő�l�X�V
				}
				if (Vtx.z > m_aVtxMax.z)
				{// ���o����Z���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.z = Vtx.z;  // �ő�l�X�V
				}

				pVtxBuff += sizeFVF;  // �T�C�Y���|�C���^��i�߂�
			}
			// ���_�o�b�t�@���A�����b�N
			m_apMesh->UnlockVertexBuffer();
		}
	}
	return S_OK;
}

//=============================================================================
//    �����o���p�z�u���̊J������
//=============================================================================
void CObjectBalloon::UnLoad(void)
{
	// ���b�V���̔j��
	if (m_apMesh != NULL)
	{
		m_apMesh->Release();
		m_apMesh = NULL;
	}

	// �}�e���A���̔j��
	if (m_apBuffMat != NULL)
	{
		m_apBuffMat->Release();
		m_apBuffMat = NULL;
	}
}

//=============================================================================
//    �����o���p�z�u���̏���������
//=============================================================================
HRESULT CObjectBalloon::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, char *pNextMapName)
{
	// ���ʂ̏���������
	CObject::Init(pos, rot, RollSpeed, bCollision, false, false);

	// �e��l�̐ݒ�
	strcpy(m_aNextMapName, pNextMapName);
	m_nCount = 0;
	m_nType = 0;
	return S_OK;
}

//=============================================================================
//    �����o���p�z�u���̏I������
//=============================================================================
void CObjectBalloon::Uninit(void)
{
	// ���ʂ̏I������
	CObject::Uninit();
}

//=============================================================================
//    �����o���p�z�u���̍X�V����
//=============================================================================
void CObjectBalloon::Update(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();        // ���݂̍��W
	CManager *pManager = NULL;
	CGame *pGame = pManager->GetGame();
	int nTypeBalloon = -1;

	// �L�[�{�[�h���擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �W���C�p�b�h���擾
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	// XInput���擾
	CXInput *pXInput = CManager::GetXInput();

	CScene *pScene = NULL;
	CScene *pSceneBalloon = NULL;

	bool bUse = false;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{
		pScene = CScene::GetScene(nCntPriority);

		while (pScene != NULL)
		{
			CScene *pSceneNext = pScene->GetNext();

			CScene::OBJTYPE objType;

			//��ނ��i�[
			objType = pScene->GetObjType();

			if (objType == CScene::OBJTYPE_PLAYER)
			{
				CPlayer *pPlayer = (CPlayer*)pScene;

				//�v���C���[�̈ʒu���擾
				D3DXVECTOR3 PlayerPos = pPlayer->GetPos();

				//�v���C���[�����f���͈͓̔��ɓ������琁���o�����g��
				if (PlayerPos.x < pos.x + OBJECT_BALLOON_RANGE && PlayerPos.x > pos.x - OBJECT_BALLOON_RANGE &&
					PlayerPos.z < pos.z + OBJECT_BALLOON_RANGE && PlayerPos.z > pos.z - OBJECT_BALLOON_RANGE)
				{
					CDebugProc::Print("\n���̃}�b�v�f�[�^�̃t�@�C���� : %s\n\n", m_aNextMapName);
					//�g��
					m_nType = 0;

					if (m_MapBalloon == CObjectBalloon::BALLOON_TYPE_0)
					{
						nTypeBalloon = BALLOON_TYPE_0;
						if (bUse == false)
						{
							bUse = true;
							m_nCount += 1;
							if (m_nCount == 1)
							{
								//�����o���̐���
								m_pBalloon[BALLOON_TYPE_0] = CBalloon::Create(OBJECT_BALLOON_POLYGON_POS,
									D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), OBJECT_BALLOON_POLYGON_WIDTH, OBJECT_BALLOON_POLYGON_HEIGHT, CObjectBalloon::BALLOON_TYPE_0);
							}
						}
						if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
						{
							//ENTER����������Ă�
							pGame->SetMapName(m_aNextMapName);
							pGame->SetState(CGame::STATE_MAPCHANGE);
							CParEmitter::Create(GetPos(), 7, 6);
						}
					}
					else if (m_MapBalloon == CObjectBalloon::BALLOON_TYPE_1)
					{
						nTypeBalloon = BALLOON_TYPE_1;

						if (bUse == false)
						{
							bUse = true;
							m_nCount += 1;
							if (m_nCount == 1)
							{
								//�����o���̐���
								m_pBalloon[BALLOON_TYPE_1] = CBalloon::Create(OBJECT_BALLOON_POLYGON_POS,
									D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), OBJECT_BALLOON_POLYGON_WIDTH, OBJECT_BALLOON_POLYGON_HEIGHT, CObjectBalloon::BALLOON_TYPE_1);
							}
						}
						if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
						{
							//ENTER����������Ă�
							pGame->SetMapName(m_aNextMapName);
							pGame->SetState(CGame::STATE_MAPCHANGE);
							CParEmitter::Create(GetPos(), 7, 6);
						}
					}

					else if (m_MapBalloon == CObjectBalloon::BALLOON_TYPE_2)
					{
						nTypeBalloon = BALLOON_TYPE_2;

						if (bUse == false)
						{
							bUse = true;
							m_nCount += 1;
							if (m_nCount == 1)
							{
								//�����o���̐���
								m_pBalloon[BALLOON_TYPE_2] = CBalloon::Create(OBJECT_BALLOON_POLYGON_POS,
									D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), OBJECT_BALLOON_POLYGON_WIDTH, OBJECT_BALLOON_POLYGON_HEIGHT, CObjectBalloon::BALLOON_TYPE_2);
							}
						}
						if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
						{
							//ENTER����������Ă�
							pGame->SetMapName(m_aNextMapName);
							pGame->SetState(CGame::STATE_MAPCHANGE);
							CParEmitter::Create(GetPos(), 7, 6);
						}
					}
					else if (m_MapBalloon == CObjectBalloon::BALLOON_TYPE_3)
					{
						nTypeBalloon = BALLOON_TYPE_3;

						if (bUse == false)
						{
							bUse = true;
							m_nCount += 1;
							if (m_nCount == 1)
							{
								//�����o���̐���
								m_pBalloon[BALLOON_TYPE_3] = CBalloon::Create(OBJECT_BALLOON_POLYGON_POS,
									D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), OBJECT_BALLOON_POLYGON_WIDTH, OBJECT_BALLOON_POLYGON_HEIGHT, CObjectBalloon::BALLOON_TYPE_3);
							}
						}
						if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
						{
							//ENTER����������Ă�
							pGame->SetMapName(m_aNextMapName);
							pGame->SetState(CGame::STATE_MAPCHANGE);
							CParEmitter::Create(GetPos(), 7, 6);
						}
					}
					else if (m_MapBalloon == CObjectBalloon::BALLOON_TYPE_4)
					{
						nTypeBalloon = BALLOON_TYPE_4;

						if (bUse == false)
						{
							bUse = true;
							m_nCount += 1;
							if (m_nCount == 1)
							{
								//�����o���̐���
								m_pBalloon[BALLOON_TYPE_4] = CBalloon::Create(OBJECT_BALLOON_POLYGON_POS,
									D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), OBJECT_BALLOON_POLYGON_WIDTH, OBJECT_BALLOON_POLYGON_HEIGHT, CObjectBalloon::BALLOON_TYPE_4);
							}
						}
						if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == true)
						{
							pGame->SetMapName(m_aNextMapName);
							pGame->SetState(CGame::STATE_MAPCHANGE);

							CParEmitter::Create(GetPos(), 7, 6);
						}
					}
				}
				else
				{//�v���C���[�����f���͈̔͊O�������琁���o�����k��
				 //�k��
					m_nType = 1;
					bUse = false;
				}

				for (int nCntBalloon = 0; nCntBalloon < MAX_BALLOON; nCntBalloon++)
				{
					if (m_nType == 0 || m_nType == 1)
					{
						if (nTypeBalloon == nCntBalloon)
						{
							//�����o���̓������s����
							m_pBalloon[nCntBalloon]->SetType(m_nType);
						}
						else
						{
							if (m_pBalloon[nCntBalloon] != NULL)
							{
								m_pBalloon[nCntBalloon]->SetType(1);
							}
						}
					}
				}
			}
			pScene = pSceneNext;
		}
	}
	// �e��l�̐ݒ�
	SetPos(pos);        // ���݂̍��W
}

//=============================================================================
//    �����o���p�z�u���̕`�揈��
//=============================================================================
void CObjectBalloon::Draw(void)
{
	// ���ʂ̕`�揈��
	CObject::Draw();
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
CObjectGoal *CObjectGoal::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, int nPriority)
{
	CObjectGoal *pObjectGoal = NULL;   // �S�[���p�z�u���N���X�^�̃|�C���^
	if (pObjectGoal == NULL)
	{// ����������ɂȂ��Ă���
		pObjectGoal = new CObjectGoal(nPriority);
		if (pObjectGoal != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pObjectGoal->Init(pos, rot, RollSpeed, bCollision)))
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
//    �S�[���p�z�u���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CObjectGoal::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // x�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(OBJECT_MODEL_FILENAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_apBuffMat,
				NULL,
				&m_anNumMat,
				&m_apMesh);

			int nNumVtx;     // ���_��
			DWORD sizeFVF;   // ���_�t�H�[�}�b�g�̃T�C�Y
			BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^

			// ���_�����l��
			nNumVtx = m_apMesh->GetNumVertices();

			// ���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(m_apMesh->GetFVF());

			// ���_�o�b�t�@�����b�N
			m_apMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// ���ׂĂ̒��_���r���ă��f���̍ŏ��l,�ő�l�𔲂��o��
				D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // ���_���W�̑��

				if (Vtx.x < m_aVtxMin.x)
				{// ���o����X���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.x = Vtx.x;  // �ŏ��l�X�V
				}
				if (Vtx.y < m_aVtxMin.y)
				{// ���o����Y���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.y = Vtx.y;  // �ŏ��l�X�V
				}
				if (Vtx.z < m_aVtxMin.z)
				{// ���o����Z���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.z = Vtx.z;  // �ŏ��l�X�V
				}
				if (Vtx.x > m_aVtxMax.x)
				{// ���o����X���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.x = Vtx.x;  // �ő�l�X�V
				}
				if (Vtx.y > m_aVtxMax.y)
				{// ���o����Y���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.y = Vtx.y;  // �ő�l�X�V
				}
				if (Vtx.z > m_aVtxMax.z)
				{// ���o����Z���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.z = Vtx.z;  // �ő�l�X�V
				}

				pVtxBuff += sizeFVF;  // �T�C�Y���|�C���^��i�߂�
			}
			// ���_�o�b�t�@���A�����b�N
			m_apMesh->UnlockVertexBuffer();
		}
	}
	return S_OK;
}

//=============================================================================
//    �S�[���p�z�u���̊J������
//=============================================================================
void CObjectGoal::UnLoad(void)
{
	// ���b�V���̔j��
	if (m_apMesh != NULL)
	{
		m_apMesh->Release();
		m_apMesh = NULL;
	}

	// �}�e���A���̔j��
	if (m_apBuffMat != NULL)
	{
		m_apBuffMat->Release();
		m_apBuffMat = NULL;
	}
}

//=============================================================================
//    �S�[���p�z�u���̏���������
//=============================================================================
HRESULT CObjectGoal::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision)
{
	// ���ʂ̏���������
	CObject::Init(pos, rot, RollSpeed, bCollision, false, false);

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

//*****************************************************************************
//     CObjectTarget�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CObjectTarget::CObjectTarget(int nPriority, OBJTYPE objType) : CObject(nPriority, objType)
{
	// �e��l�̃N���A
	m_nMaxCarryTime = 0;                              // �v���C���[���^�Ԃ̂ɂ����鎞��
	m_nCounter = 0;                                   // �^�т��Ǘ�����J�E���^�[
	m_State = STATE_NONE;                             // ���
	m_PosTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // �ړI�̈ʒu
	m_DiffPosTarget = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �ړI�̈ʒu�Ƃ̍���
	m_pPlayer = NULL;                                 // �v���C���[�N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CObjectTarget::~CObjectTarget()
{

}

//=============================================================================
//    �W�b�v���C���p�I�u�W�F�N�g�̐�������
//=============================================================================
CObjectTarget *CObjectTarget::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 PosTarget, int nMaxCarry, int nPriority)
{
	CObjectTarget *pObjectTarget = NULL;   // �W�b�v���C���p�z�u���N���X�^�̃|�C���^
	if (pObjectTarget == NULL)
	{// ����������ɂȂ��Ă���
		pObjectTarget = new CObjectTarget(nPriority);
		if (pObjectTarget != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pObjectTarget->Init(pos, rot, RollSpeed, bCollision, PosTarget, nMaxCarry)))
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

	return pObjectTarget;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    �W�b�v���C���p�z�u���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CObjectTarget::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // x�t�@�C���̓ǂݍ���
			D3DXLoadMeshFromX(OBJECT_MODEL_FILENAME,
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_apBuffMat,
				NULL,
				&m_anNumMat,
				&m_apMesh);

			int nNumVtx;     // ���_��
			DWORD sizeFVF;   // ���_�t�H�[�}�b�g�̃T�C�Y
			BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^

			// ���_�����l��
			nNumVtx = m_apMesh->GetNumVertices();

			// ���_�t�H�[�}�b�g�̃T�C�Y���擾
			sizeFVF = D3DXGetFVFVertexSize(m_apMesh->GetFVF());

			// ���_�o�b�t�@�����b�N
			m_apMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

			for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
			{// ���ׂĂ̒��_���r���ă��f���̍ŏ��l,�ő�l�𔲂��o��
				D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // ���_���W�̑��

				if (Vtx.x < m_aVtxMin.x)
				{// ���o����X���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.x = Vtx.x;  // �ŏ��l�X�V
				}
				if (Vtx.y < m_aVtxMin.y)
				{// ���o����Y���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.y = Vtx.y;  // �ŏ��l�X�V
				}
				if (Vtx.z < m_aVtxMin.z)
				{// ���o����Z���W�̍ŏ��l�����܂ł̂���������
					m_aVtxMin.z = Vtx.z;  // �ŏ��l�X�V
				}
				if (Vtx.x > m_aVtxMax.x)
				{// ���o����X���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.x = Vtx.x;  // �ő�l�X�V
				}
				if (Vtx.y > m_aVtxMax.y)
				{// ���o����Y���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.y = Vtx.y;  // �ő�l�X�V
				}
				if (Vtx.z > m_aVtxMax.z)
				{// ���o����Z���W�̍ő�l�����܂ł̂����傫��
					m_aVtxMax.z = Vtx.z;  // �ő�l�X�V
				}

				pVtxBuff += sizeFVF;  // �T�C�Y���|�C���^��i�߂�
			}
			// ���_�o�b�t�@���A�����b�N
			m_apMesh->UnlockVertexBuffer();
		}
	}
	return S_OK;
}

//=============================================================================
//    �W�b�v���C���p�z�u���̊J������
//=============================================================================
void CObjectTarget::UnLoad(void)
{
	// ���b�V���̔j��
	if (m_apMesh != NULL)
	{
		m_apMesh->Release();
		m_apMesh = NULL;
	}

	// �}�e���A���̔j��
	if (m_apBuffMat != NULL)
	{
		m_apBuffMat->Release();
		m_apBuffMat = NULL;
	}
}

//=============================================================================
//    �W�b�v���C���p�z�u���̏���������
//=============================================================================
HRESULT CObjectTarget::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, bool bCollision, D3DXVECTOR3 PosTarget, int nMaxCarry)
{
	// ���ʂ̏���������
	CObject::Init(pos, rot, RollSpeed, bCollision, false, false);

	// �e��l�̐ݒ�
	m_PosTarget = PosTarget;       // �ړI�̈ʒu
	m_nMaxCarryTime = nMaxCarry;   // �^�Ԃ̂ɂ����鎞��

	// ���݂̈ʒu�ƖړI�̈ʒu�̍��������߂�
	m_DiffPosTarget = m_PosTarget - pos;

	return S_OK;
}

//=============================================================================
//    �W�b�v���C���p�z�u���̏I������
//=============================================================================
void CObjectTarget::Uninit(void)
{
	// ���ʂ̏I������
	CObject::Uninit();
}

//=============================================================================
//    �W�b�v���C���p�z�u���̍X�V����
//=============================================================================
void CObjectTarget::Update(void)
{
	if (m_State == STATE_CARRY)
	{// �v���C���[���^�ԏ�Ԃł���
		Carry();
	}
}

//=============================================================================
//    �W�b�v���C���p�z�u���̕`�揈��
//=============================================================================
void CObjectTarget::Draw(void)
{
	// ���ʂ̕`�揈��
	CObject::Draw();
}

//=============================================================================
//    �W�b�v���C���p�z�u���̓����蔻�菈��
//=============================================================================
bool CObjectTarget::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, CShadow *pShadow, float fHeight, float fRange, CScene *pScene)
{
	bool bCatch = false;   // �I�u�W�F�N�g�ɂ��܂��Ă��邩�ǂ���

	// �����蔻��ɕK�v�Ȓl��z�u������擾
	D3DXVECTOR3 pos = GetPos();          // ���݂̍��W
	D3DXVECTOR3 posOld = GetPosOld();    // �O��̍��W
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
	    // �e�̈ʒu�����炷
		if (pShadow != NULL)
		{// �e����������Ă���
			if (pPos->y >= GetPos().y + VtxMax.y || pPosOld->y >= GetPos().y + VtxMax.y)
			{// �z�u���̏�ɂ���
			    // �ʒu�����炷
				pShadow->SetPos(D3DXVECTOR3(pPos->x, pos.y + VtxMax.y + 1.0f, pPos->z));

				// ���_���̐ݒ�
				VERTEX_3D *pVtx;
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = pShadow->GetVtxBuff();
				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

				for (int nCntVer = 0; nCntVer < 4; nCntVer++)
				{// �e�̒��_�����J��Ԃ�
					pVtx[nCntVer].pos.y = 0.0f; // �z�u���̏�ɉe���悹��̂ŃI�t�Z�b�g������0��
				}

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();

				// ���_�o�b�t�@�̐ݒ�
				pShadow->SetVtxBuff(pVtxBuff);
			}
		}

		// ����Ă��邩�ǂ����̔���J�n
		if (pPosOld->y >= pos.y + VtxMax.y && pPos->y <= pos.y + VtxMax.y)
		{// �z�u���ɏォ�������
			pPos->y = pos.y + VtxMax.y;
			pMove->y = 0.0f;
		}
		else if (pPosOld->y + fHeight <= pos.y + VtxMin.y && pPos->y + fHeight > pos.y + VtxMin.y)
		{// �z�u���ɉ����������
			pPos->y = pos.y + VtxMin.y - fHeight;
			if (m_State == STATE_NONE)
			{// �v���C���[���^��ł��Ȃ���Ԃ�������
				pPos->x = pos.x;
				pPos->z = pos.z;
				pMove->y = 0.0f;
				m_State = STATE_CARRY;  // �^�ԏ�Ԃ�
				if (pScene != NULL)
				{// �v���C���[�̃|�C���^���擾�ł��Ă���
					if (pScene->GetObjType() == OBJTYPE_PLAYER)
					{// �v���C���[�̃|�C���^��������
						m_pPlayer = (CPlayer*)pScene;
						if (m_pPlayer != NULL)
						{// �v���C���[�N���X�ւ̃|�C���^���擾�ł���
							m_pPlayer->SetRot(GetRot());
						}
					}
				}
				bCatch = true;
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_ZIPLINE);
			}
		}
	}

	// �e��l�̐ݒ�
	SetPos(pos);   // ���݂̍��W

	return bCatch;   // ���܂������ǂ�����Ԃ�
}

//=============================================================================
//    �v���C���[���^�ԏ���
//=============================================================================
void CObjectTarget::Carry(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();  // ���݂̍��W

	// ���݂̈ʒu�ɉ�����
	pos += m_DiffPosTarget / (float)m_nMaxCarryTime;

	// �v���C���[���^��
	if (m_pPlayer != NULL)
	{// �v���C���[�N���X���擾�ł��Ă���
		m_pPlayer->SetPos(D3DXVECTOR3(pos.x, pos.y - m_pPlayer->GetColHeight(), pos.z));
	}

	// ���݂̈ʒu��ݒ�
	SetPos(pos);

	m_nCounter++;
	if (m_nCounter % m_nMaxCarryTime == 0)
	{// �ړI�̈ʒu�ɓ��B����
		m_State = STATE_FINISH;
		// �v���C���[�̏�Ԃ�߂�
		if (m_pPlayer != NULL)
		{// �v���C���[�N���X���擾�ł��Ă���
			m_pPlayer->SetState(CPlayer::STATE_NORMAL);
		}
		m_pPlayer = NULL;
		CManager::GetSound()->StopSound(CSound::SOUND_LABEL_SE_ZIPLINE);
		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_CARRYFINISH);
	}
}