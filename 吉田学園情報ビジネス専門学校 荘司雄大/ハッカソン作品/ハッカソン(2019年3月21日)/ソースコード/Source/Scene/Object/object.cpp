//*****************************************************************************
//
//     �z�u���̏���[object.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "object.h"
#include "manager.h"
#include "system.h"
#include "model.h"
#include "motion.h"
#include "modelManager.h"
#include "functionlib.h"
#include "collision.h"
#include "star.h"
#include "mode.h"

//*****************************************************************************
//    �}�N���錾
//*****************************************************************************
#define MAX_OBJECT_MOTION (1)                     // ���[�V�����̃f�[�^��
#define OBJECT_MAX_LIFE   (3)                     // �I�u�W�F�N�g�̗̑�

// �l�ǂݍ��݂�����ۂ̖ڈ�ƂȂ镶����
// ����
#define POS               "POS = "                // ���W��ǂݎ�鍇�}
#define ROT               "ROT = "                // ������ǂݎ�鍇�}
#define MOVE              "MOVE = "               // �ړ��ʂ�ǂݎ�鍇�}

// ���̑�
#define NUM_MODEL         "NUM_MODEL = "          // �ǂݍ��ރ��f���̐�
#define MODEL_FILENAME    "MODEL_FILENAME = "     // �ǂݍ��ރ��f���̃t�@�C����

// �z�u�����
#define MODELSET          "MODELSET"              // �z�u�����ǂݍ��݊J�n�̍��}
#define END_MODELSET      "END_MODELSET"          // �z�u�����ǂݍ��ݏI���̍��}
#define NUM_PARTS         "NUM_PARTS = "          // �z�u���̃p�[�c��

// �p�[�c���
#define PARTSSET          "PARTSSET"              // �p�[�c���ǂݍ��݊J�n�̍��}
#define END_PARTSSET      "END_PARTSSET"          // �p�[�c���ǂݍ��ݏI���̍��}
#define INDEX             "INDEX = "              // �ԍ���ǂݎ�鍇�}
#define PARENT            "PARENT = "             // �e�p�[�c�̔ԍ���ǂݎ�鍇�}
#define MODEL_IDX         "MODEL_IDX = "          // �g�p���郂�f���̔ԍ�

// ���[�V�������
#define MOTIONSET         "MOTIONSET"             // ���[�V�������ǂݍ��݊J�n�̍��}
#define END_MOTIONSET     "END_MOTIONSET"         // ���[�V�������ǂݍ��ݏI���̍��}
#define COLLISIONSET      "COLLISIONSET"          // ���[�V�����̓����蔻����ǂݍ��݊J�n�̍��}
#define END_COLLISIONSET  "END_COLLISIONSET"      // ���[�V�����̓����蔻����ǂݍ��ݏI���̍��}
#define ORBITSET          "ORBITSET"              // ���[�V�����̋O�Տ��ǂݍ��݊J�n�̍��}
#define END_ORBITSET      "END_ORBITSET"          // ���[�V�����̋O�Տ��ǂݍ��ݏI���̍��}
#define RANGE             "RANGE = "              // �͈͂�ǂݎ�鍇�}
#define XBLOCK            "XBLOCK = "             // �O�Ղ̉��̕�������ǂݎ�鍇�}
#define ZBLOCK            "ZBLOCK = "             // �O�Ղ̉��s�̕�������ǂݎ�鍇�}
#define OFFSET            "OFFSET = "             // �I�t�Z�b�g��ǂݎ�鍇�}
#define LOOP              "LOOP = "               // ���[�V���������[�v�Đ����邩�ǂ�����ǂݎ�鍇�}
#define BLEND             "BLEND = "              // ���[�V�����u�����h�̔{����ǂݎ�鍇�}
#define NUM_KEY           "NUM_KEY = "            // ���[�V�����̃L�[�t���[���̐���ǂݎ�鍇�}

// �U�����[�V�������
#define ATTACKSET         "ATTACKSET"             // �U�����[�V�������ǂݍ��݊J�n�̍��}
#define END_ATTACKSET     "END_ATTACKSET"         // �U�����[�V�������ǂݍ��ݏI���̍��}
#define NEXT_MOTION       "NEXT_MOTION = "        // ���̃��[�V�����̔ԍ�

// ���[�V�����̓����蔻����
#define NUM_COL           "NUM_COL = "            // ���[�V�����̓����蔻����̐���ǂݎ�鍇�}
#define COLLISION         "COLLISION"             // �����蔻����ǂݍ��݊J�n�̍��}
#define DAMAGE            "DAMAGE = "             // ���������ꍇ�̃_���[�W
#define END_COLLISION     "END_COLLISION"         // �����蔻����ǂݍ��ݏI���̍��}

// �O�Տ��
#define NUM_ORBIT         "NUM_ORBIT = "          // ���[�V�����̋O�Տ��̐���ǂݎ�鍇�}
#define ORBIT		      "ORBIT"                 // �O�Տ��ǂݍ��݊J�n�̍��}
#define END_ORBIT	      "END_ORBIT"             // �O�Տ��ǂݍ��ݏI���̍��}
#define TEX_INDEX         "TEX_INDEX = "          // �O�Ղɒ���t����e�N�X�`���̔ԍ���ǂݎ�鍇�}
#define OFFSET1           "OFFSET1 = "            // �P�ڂ̃I�t�Z�b�g��ǂݎ�鍇�}
#define OFFSET2           "OFFSET2 = "            // �Q�ڂ̃I�t�Z�b�g��ǂݎ�鍇�}
#define OFFSETAMP1        "OFFSETAMP1 = "         // �O�Ղ̃I�t�Z�b�g���������l��ǂݎ�鍇�}(�P��)
#define OFFSETAMP2        "OFFSETAMP2 = "         // �O�Ղ̃I�t�Z�b�g���������l��ǂݎ�鍇�}(�Q��)
#define COLUP             "COLUP = "              // �O�Ղ̒��_�㑤�̐F
#define COLDOWN           "COLDOWN = "            // �O�Ղ̒��_�����̐F
#define ALPHA_DECAY_UP    "ALPHA_DECAY_UP = "     // �O�Ղ̒��_�㑤�̓����x�����l
#define ALPHA_DECAY_DOWN  "ALPHA_DECAY_DOWN = "   // �O�Ղ̒��_�����̓����x�����l
#define START             "START = "              // �O�Ղ��o���n�߂�^�C�~���O��ǂݎ�鍇�}
#define FINISH            "FINISH = "             // �O�Ղ����܂��^�C�~���O��ǂݎ�鍇�}

// �L�[�t���[�����
#define KEYSET            "KEYSET"                // �L�[�t���[�����ǂݍ��݊J�n�̍��}
#define END_KEYSET        "END_KEYSET"            // �L�[�t���[�����ǂݍ��ݏI���̍��}
#define FRAME             "FRAME = "              // �L�[�t���[���̍Đ��t���[������ǂݎ�鍇�}

// �L�[���
#define KEY               "KEY"                   // �L�[���ǂݍ��݊J�n�̍��}
#define END_KEY           "END_KEY"               // �L�[���ǂݍ��ݏI���̍��}

// �����蔻����
#define NUM_COLLISION     "NUM_COLLISION = "      // �����蔻��̐�
#define COLLISIONSET      "COLLISIONSET"          // �����蔻����ǂݍ��݊J�n�̍��}
#define END_COLLISIONSET  "END_COLLISIONSET"      // �����蔻����ǂݍ��ݏI���̍��}
#define RETURN            "RETURN = "             // ���������ۂɉ����߂����ǂ���
#define TYPE              "TYPE = "               // ���
#define WIDTH             "WIDTH = "              // ����
#define HEIGHT            "HEIGHT = "             // �c��
#define DEPTH             "DEPTH = "              // ���s

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//*****************************************************************************
//    CObject�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CObject::CObject(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_nNumParts = 0;                            // �g�p����p�[�c��
	m_nNumCollision = 0;                        // �����蔻��̐�
	m_apModel = NULL;                           // ���f���N���X�ւ̃|�C���^
	m_pMotionManager = NULL;                    // ���[�V�����N���X�ւ̃|�C���^
	m_apCollision = NULL;                       // �R���W�����N���X�ւ̃|�C���^
	D3DXMatrixIdentity(&m_MtxWorld);            // ���[���h�}�g���b�N�X
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���݂̍��W
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �O��̍��W
	m_PosStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �������̍��W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ���݂̌���
	m_RotStd = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �������̌���
	m_Scale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �傫��
	m_RollSpeed = D3DXVECTOR3(0.0f,0.0f,0.0f);  // ��������]������X�s�[�h
	m_bBreak = false;                           // �󂹂�I�u�W�F�N�g���ǂ���
	m_nLife = 0;                                // �̗�
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CObject::~CObject()
{

}

//=============================================================================
//    ��������
//=============================================================================
CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, int nNumParts, int nNumCollision, CModel **pModel, CMotionManager *pMotionManager, CCollision **pCollision, bool bBreak, int nPriority)
{
	CObject *pObject = NULL;               // �z�u���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pObject == NULL)
		{// ����������ɂȂ��Ă���
			pObject = new CObject(nPriority);
			if (pObject != NULL)
			{// �C���X�^���X�𐶐��ł���
				// �e��l�̐ݒ�
				pObject->SetType(nType);                   // �z�u���̎��
				pObject->SetNumParts(nNumParts);           // �p�[�c��
				pObject->SetNumCollision(nNumCollision);   // �����蔻��̐�
				pObject->SetModel(pModel);                 // ���f���N���X�ւ̃|�C���^
				pObject->SetMotionManager(pMotionManager); // ���[�V�����N���X�ւ̃|�C���^
				pObject->SetCollision(pCollision);         // �R���W�����N���X�ւ̃|�C���^
				pObject->SetPos(pos);                      // ���݂̍��W
				pObject->SetPosOld(pos);                   // �O��̍��W
				pObject->SetPosStd(pos);                   // �������̍��W
				pObject->SetRot(rot);                      // ���݂̌���
				pObject->SetRotStd(rot);                   // �������̌���
				pObject->SetScale(scale);                  // �傫��
				pObject->SetRollSpeed(RollSpeed);          // ��]�X�s�[�h
				pObject->SetBreak(bBreak);                 // �j��ł��邩�ǂ���

				if (FAILED(pObject->Init()))
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

	return pObject;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CObject::Init(void)
{
	// �����蔻��𒲐�����
	CBoxCollider *pBoxCollider = NULL;
	CCircleCollider *pCirclcleCollider = NULL;
	CSphereCollider *pSphereCollider = NULL;
	if (m_apCollision != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
		{// �����蔻��̐������J��Ԃ�
			if (m_apCollision[nCntCol] != NULL)
			{// ���������m�ۂ���Ă���
				if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_BOX)
				{// �{�b�N�X�R���C�_�[��������
					pBoxCollider = (CBoxCollider*)m_apCollision[nCntCol];
					pBoxCollider->SetWidth(pBoxCollider->GetWidth() * m_Scale.x);
					pBoxCollider->SetHeight(pBoxCollider->GetHeight() * m_Scale.y);
					pBoxCollider->SetDepth(pBoxCollider->GetDepth() * m_Scale.z);
				}
				else if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_CIRCLE)
				{// �T�[�N���R���C�_�[��������
					pCirclcleCollider = (CCircleCollider*)m_apCollision[nCntCol];
				}
				else if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_SPHERE)
				{// �X�t�B�A�R���C�_�[��������
					pSphereCollider = (CSphereCollider*)m_apCollision[nCntCol];
				}
			}
		}
	}

	// �̗͂�ݒ�
	m_nLife = OBJECT_MAX_LIFE;

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CObject::Uninit(void)
{
	// ���f���̔j��
	if (m_apModel != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// �g�p�ł��郂�f�������J��Ԃ�
			if (m_apModel[nCntModel] != NULL)
			{// �|�C���^���m�ۂ���Ă���
				m_apModel[nCntModel]->Uninit();

				// �������̊J��
				delete m_apModel[nCntModel];
				m_apModel[nCntModel] = NULL;
			}
		}
		// �������̊J��
		delete[] m_apModel;
		m_apModel = NULL;
	}

	// ���[�V�����̔j��
	if (m_pMotionManager != NULL)
	{// �|�C���^���m�ۂ���Ă���
		m_pMotionManager->Uninit();

		// �������̊J��
		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}

	// �R���W�����̔j��
	if (m_apCollision != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
		{// �g�p�ł��郂�f�������J��Ԃ�
			if (m_apCollision[nCntCol] != NULL)
			{// �|�C���^���m�ۂ���Ă���
				m_apCollision[nCntCol]->Uninit();

				// �������̊J��
				delete m_apCollision[nCntCol];
				m_apCollision[nCntCol] = NULL;
			}
		}
		// �������̊J��
		delete[] m_apCollision;
		m_apCollision = NULL;
	}

	// ���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CObject::Update(void)
{
	if (m_pMotionManager != NULL)
	{
		m_pMotionManager->Update(m_apModel);
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CObject::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �@�����𐳋K������(�X�P�[�����O���ɖ@����񂪂�������̂�)
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);

		    // ���[���h�}�g���b�N�X�v�Z����
			SetMtxWorld(pDevice);

			if (m_apModel != NULL)
			{// ���������m�ۂł���
			    // ���f���̕`��
				for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
				{// ���f���𐶐����鐔�����J��Ԃ�
				    // ���f���̕`�揈��
					m_apModel[nCntModel]->Draw();
				}
			}

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
			if (m_apCollision != NULL)
			{// ���������m�ۂł���
				// �R���W�����̕`��
				for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
				{// �����蔻��̐������J��Ԃ�
					m_apCollision[nCntCol]->Draw();
				}
			}

			// �@���𐳋K�����Ȃ�
			pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, false);
		}
	}
}

//=============================================================================
//    �`��f�o�C�X�Ƀ��[���h�}�g���b�N�X��ݒ肷�鏈��
//=============================================================================
void CObject::SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxScale, mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

	// �傫���𔽉f
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxScale);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_MtxWorld, &m_MtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_MtxWorld);
}

//=============================================================================
//    �����蔻�菈��
//=============================================================================
bool CObject::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand, CScene *pRideObject)
{
	bool bCol = false;   // �����������ǂ���
	CBoxCollider *pBoxCollider = NULL;
	CCircleCollider *pCirclcleCollider = NULL;
	CSphereCollider *pSphereCollider = NULL;

	if (m_apCollision != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
		{// �����蔻��̐������J��Ԃ�
			if (m_apCollision[nCntCol] != NULL)
			{// ���������m�ۂ���Ă���
				if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_BOX)
				{// �{�b�N�X�R���C�_�[��������
					pBoxCollider = (CBoxCollider*)m_apCollision[nCntCol];
					if (pBoxCollider->Collision(pPos, pPosOld, pMove, ColRange, pLand) == true)
					{// ���ɓ������Ă���
						bCol = true;
					}
				}
				else if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_CIRCLE)
				{// �T�[�N���R���C�_�[��������
					pCirclcleCollider = (CCircleCollider*)m_apCollision[nCntCol];
					if (pCirclcleCollider->Collision(pPos, pPosOld, pMove, ColRange, pLand) == true)
					{// ���ɓ������Ă���
						bCol = true;
					}
				}
				else if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_SPHERE)
				{// �X�t�B�A�R���C�_�[��������
					pSphereCollider = (CSphereCollider*)m_apCollision[nCntCol];
					if (pSphereCollider->Collision(pPos, pPosOld, pMove, ColRange, pLand) == true)
					{// ���ɓ������Ă���
						bCol = true;
					}
				}
			}
		}
	}

	return bCol;
}


//=============================================================================
//    �z�u���̃_���[�W����
//=============================================================================
void CObject::Damage(int nDamage)
{
	if (m_bBreak == true)
	{// ����z�u���Ȃ�
		m_nLife -= nDamage;

		if (m_nLife <= 0)
		{// �̗͂��Ȃ��Ȃ���
			Uninit();
		}
	}
}

//=============================================================================
//    �z�u���̎�ނ�ݒ肷��
//=============================================================================
void CObject::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    �p�[�c����ݒ肷��
//=============================================================================
void CObject::SetNumParts(const int nNumParts)
{
	m_nNumParts = nNumParts;
}

//=============================================================================
//    �����蔻��̐���ݒ肷��
//=============================================================================
void CObject::SetNumCollision(const int nNumCollision)
{
	m_nNumCollision = nNumCollision;
}

//=============================================================================
//    ���f���N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CObject::SetModel(CModel **pModel)
{
	m_apModel = pModel;
}

//=============================================================================
//    ���[�V�����N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CObject::SetMotionManager(CMotionManager *pMotionManager)
{
	m_pMotionManager = pMotionManager;
}

//=============================================================================
//    �R���W�����N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CObject::SetCollision(CCollision **pCollision)
{
	m_apCollision = pCollision;
}

//=============================================================================
//    ���݂̍��W��ݒ肷��
//=============================================================================
void CObject::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �O��̍��W��ݒ肷��
//=============================================================================
void CObject::SetPosOld(const D3DXVECTOR3 posold)
{
	m_PosOld = posold;
}

//=============================================================================
//    �������̍��W��ݒ肷��
//=============================================================================
void CObject::SetPosStd(const D3DXVECTOR3 posstd)
{
	m_PosStd = posstd;
}

//=============================================================================
//    ���݂̌�����ݒ肷��
//=============================================================================
void CObject::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    �������̌�����ݒ肷��
//=============================================================================
void CObject::SetRotStd(const D3DXVECTOR3 rotstd)
{
	m_RotStd = rotstd;
}

//=============================================================================
//    �傫����ݒ肷��
//=============================================================================
void CObject::SetScale(const D3DXVECTOR3 scale)
{
	m_Scale = scale;
}

//=============================================================================
//    ��]�X�s�[�h��ݒ肷��
//=============================================================================
void CObject::SetRollSpeed(const D3DXVECTOR3 RollSpeed)
{
	m_RollSpeed = RollSpeed;
}

//=============================================================================
//    �j��ł��邩�ǂ�����ݒ肷��
//=============================================================================
void CObject::SetBreak(const bool bBreak)
{
	m_bBreak = bBreak;
}

//=============================================================================
//    �z�u���̎�ނ��擾����
//=============================================================================
int CObject::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    �p�[�c�����擾����
//=============================================================================
int CObject::GetNumParts(void)
{
	return m_nNumParts;
}

//=============================================================================
//    �����蔻��̐����擾����
//=============================================================================
int CObject::GetNumCollision(void)
{
	return m_nNumCollision;
}

//=============================================================================
//    ���f���N���X�ւ̃|�C���^���擾����
//=============================================================================
CModel **CObject::GetModel(void)
{
	return m_apModel;
}

//=============================================================================
//    ���[�V�����N���X�ւ̃|�C���^���擾����
//=============================================================================
CMotionManager *CObject::GetMotionManager(void)
{
	return m_pMotionManager;
}

//=============================================================================
//    �R���W�����N���X�ւ̃|�C���^���擾����
//=============================================================================
CCollision **CObject::GetCollision(void)
{
	return m_apCollision;
}

//=============================================================================
//    ���݂̍��W���擾����
//=============================================================================
D3DXVECTOR3 CObject::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �O��̍��W���擾����
//=============================================================================
D3DXVECTOR3 CObject::GetPosOld(void)
{
	return m_PosOld;
}

//=============================================================================
//    �������̍��W���擾����
//=============================================================================
D3DXVECTOR3 CObject::GetPosStd(void)
{
	return m_PosStd;
}

//=============================================================================
//    ���݂̌������擾����
//=============================================================================
D3DXVECTOR3 CObject::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    �������̌������擾����
//=============================================================================
D3DXVECTOR3 CObject::GetRotStd(void)
{
	return m_RotStd;
}

//=============================================================================
//    �傫�����擾����
//=============================================================================
D3DXVECTOR3 CObject::GetScale(void)
{
	return m_Scale;
}

//=============================================================================
//    ��]�X�s�[�h���擾����
//=============================================================================
D3DXVECTOR3 CObject::GetRollSpeed(void)
{
	return m_RollSpeed;
}

//=============================================================================
//    �j��ł��邩�ǂ������擾����
//=============================================================================
bool CObject::GetBreak(void)
{
	return m_bBreak;
}

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CObjectStar::CObjectStar(int nPriority, OBJTYPE objType) : CObject(nPriority, objType)
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CObjectStar::~CObjectStar()
{

}

//=============================================================================
//    �j��ł��邩�ǂ������擾����
//=============================================================================
CObjectStar *CObjectStar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, int nNumParts, int nNumCollision, CModel **pModel, CMotionManager *pMotionManager, CCollision **pCollision, bool bBreak, int nPriority)
{
	CObjectStar *pObjectGoal = NULL;       // �S�[���p�z�u���N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pObjectGoal == NULL)
		{// ����������ɂȂ��Ă���
			pObjectGoal = new CObjectStar(nPriority);
			if (pObjectGoal != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̐ݒ�
				pObjectGoal->SetType(nType);                   // �z�u���̎��
				pObjectGoal->SetNumParts(nNumParts);           // �p�[�c��
				pObjectGoal->SetNumCollision(nNumCollision);   // �����蔻��̐�
				pObjectGoal->SetModel(pModel);                 // ���f���N���X�ւ̃|�C���^
				pObjectGoal->SetMotionManager(pMotionManager); // ���[�V�����N���X�ւ̃|�C���^
				pObjectGoal->SetCollision(pCollision);         // �R���W�����N���X�ւ̃|�C���^
				pObjectGoal->SetPos(pos);                      // ���݂̍��W
				pObjectGoal->SetPosOld(pos);                   // �O��̍��W
				pObjectGoal->SetPosStd(pos);                   // �������̍��W
				pObjectGoal->SetRot(rot);                      // ���݂̌���
				pObjectGoal->SetRotStd(rot);                   // �������̌���
				pObjectGoal->SetScale(scale);                  // �傫��
				pObjectGoal->SetRollSpeed(RollSpeed);          // ��]�X�s�[�h
				pObjectGoal->SetBreak(bBreak);                 // �j��ł��邩�ǂ���

				if (FAILED(pObjectGoal->Init()))
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

	return pObjectGoal;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CObjectStar::Init(void)
{
	if (FAILED(CObject::Init()))
	{// �������Ɏ��s����
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CObjectStar::Uninit(void)
{
	// ���ʂ̏I������
	CObject::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CObjectStar::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CObjectStar::Draw(void)
{
	// ���ʂ̏I������
	CObject::Draw();
}

//=============================================================================
//    ���񂾂Ƃ��̏���
//=============================================================================
void CObjectStar::Death(void)
{
	// ���𐶐�����
	for (int nCntStar = 0; nCntStar < 10; nCntStar++)
	{
		float fAngle = (rand() % 628 * 0.01f) - D3DX_PI;
		CStar::Create(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(sinf(fAngle) * 10.0f, 20.0f, cosf(fAngle) * 10.0f));
	}

	if (CManager::GetMode() == CManager::MODE_GAME)
	{
		CManager::GetGame()->SetStarObject();
	}
}

//*****************************************************************************
//    CObjectManager�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CObjectManager::CObjectManager()
{
	// �e��l�̃N���A
	strcpy(m_aFileName, "\0");   // �t�@�C����
	m_nNumParts = 0;             // �p�[�c��
	m_nNumCollision = 0;         // �����蔻��̐�
	m_pNumParent = NULL;         // �e���f���̔ԍ�
	m_pNumColParent = NULL;      // �����蔻��̐e�ԍ�
	m_pNumModelIdx = NULL;       // �g�p���郂�f���̔ԍ�
	m_apModel = NULL;            // ���f���N���X�ւ̃|�C���^
	m_pModelManager = NULL;      // ���f���Ǌ��N���X�ւ̃|�C���^
	m_pMotionManager = NULL;     // ���[�V�����N���X�ւ̃|�C���^
	m_apCollision = NULL;        // �R���W�����N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CObjectManager::~CObjectManager()
{

}

//=============================================================================
//    ��������
//=============================================================================
CObjectManager *CObjectManager::Create(char *pFileName, CModelManager *pModelManager)
{
	CObjectManager *pObjectManager = NULL;  // �v���C���[�Ǌ��N���X�^�̃|�C���^
	if (pObjectManager == NULL)
	{// ����������ɂȂ��Ă���
		pObjectManager = new CObjectManager;
		if (pObjectManager != NULL)
		{// �C���X�^���X�𐶐��ł���
			pObjectManager->SetFileName(pFileName);
			pObjectManager->SetModelManager(pModelManager);
			if (FAILED(pObjectManager->Init()))
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

	return pObjectManager;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CObjectManager::Init(void)
{
	FILE *pFile = NULL;  // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(GetFileName(), "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                         // 1�s���ǂݎ��p
			char *pStrCur = NULL;                       // ���݂̐擪�|�C���^
			char *pStr = NULL;                          // �擪�|�C���^�ۑ��p
			int nNumModel = 0;                          // �ǂݍ��ރ��f����
			int nCntModel = 0;                          // ���f����ǂݍ��񂾉񐔂𐔂���
			int nCntMotion = 0;                         // ���[�V������ǂݍ��񂾉񐔂𐔂���
			CMotion *pMotion[MAX_OBJECT_MOTION] = {};   // ���[�V�����N���X�ւ̃|�C���^
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // �e�N�X�`�����ւ̃|�C���^
			LPD3DXMESH pMesh = NULL;                    // ���b�V�����ւ̃|�C���^
			LPD3DXBUFFER pBuffMat = NULL;               // �}�e���A�����ւ̃|�C���^
			DWORD nNumMat = 0;                          // �}�e���A�����̐�

			if (pLine == NULL && pStr == NULL)
			{// ���������m�ۂł����Ԃł���
				pLine = new char[256];
				pStr = new char[256];
				strcpy(pLine, "\0");
				strcpy(pStr, "\0");
				if (pLine != NULL && pStr != NULL)
				{// ���������m�ۂ���Ă���
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, MODELSET) == 0)
							{// ���f�����ǂݍ��݊J�n�̍��}��������
								m_apModel = ReadModelSet(pLine, pStrCur, pFile, &m_nNumParts);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COLLISIONSET) == 0)
							{// �����蔻����ǂݍ��݊J�n�̍��}��������
								m_apCollision = ReadCollisionSet(pLine, pStrCur, pFile, &m_nNumCollision);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOTIONSET) == 0)
							{// ���[�V�������ǂݍ��݊J�n�̍��}��������
								pMotion[nCntMotion] = ReadMotionSet(pLine, pStrCur, pFile, &m_nNumParts, pMotion[nCntMotion], nCntMotion);
								nCntMotion++;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}
				}
			}

			// �������̊J��
			if (pStr != NULL)
			{// ���������m�ۂ���Ă���
				delete[] pStr;
				pStr = NULL;
			}
			if (pLine != NULL)
			{// ���������m�ۂ���Ă���
				delete[] pLine;
				pLine = NULL;
			}

			// �K�v�ȃN���X���쐬���Ă���
			// ���[�V�����Ǌ��N���X
			if (m_pMotionManager == NULL)
			{// �|�C���^���m�ۂ���Ă���
				m_pMotionManager = CMotionManager::Create(MAX_OBJECT_MOTION, m_nNumParts);
				if (m_pMotionManager != NULL)
				{// ���[�V�����Ǘ��N���X���쐬�ł���
					for (int nCntMotion = 0; nCntMotion < MAX_OBJECT_MOTION; nCntMotion++)
					{// ��Ԃ̐������J�肩����
						m_pMotionManager->SetMotion(pMotion[nCntMotion], nCntMotion);
					}
				}
			}
			fclose(pFile);
		}
		else
		{// �t�@�C���|�C���^���m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// �t�@�C���|�C���^���m�ۂł����ԂłȂ�
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CObjectManager::Uninit(void)
{
	// ���f���̔j��
	if (m_apModel != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntModel = 0; nCntModel < m_nNumParts; nCntModel++)
		{// �g�p�ł��郂�f�������J��Ԃ�
			if (m_apModel[nCntModel] != NULL)
			{// �|�C���^���m�ۂ���Ă���
				m_apModel[nCntModel]->Uninit();

				// �������̊J��
				delete m_apModel[nCntModel];
				m_apModel[nCntModel] = NULL;
			}
		}
		// �������̊J��
		delete[] m_apModel;
		m_apModel = NULL;
	}

	// ���[�V�����N���X�̔j��
	if (m_pMotionManager != NULL)
	{// ���������m�ۂ���Ă���
		m_pMotionManager->Uninit();

		delete m_pMotionManager;
		m_pMotionManager = NULL;
	}

	// �R���W�����N���X�̔j��
	if (m_apCollision != NULL)
	{// ���������m�ۂ���Ă���
		for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
		{// �g�p�ł��郂�f�������J��Ԃ�
			if (m_apCollision[nCntCol] != NULL)
			{// �|�C���^���m�ۂ���Ă���
				m_apCollision[nCntCol]->Uninit();

				// �������̊J��
				delete m_apCollision[nCntCol];
				m_apCollision[nCntCol] = NULL;
			}
		}
		// �������̊J��
		delete[] m_apCollision;
		m_apCollision = NULL;
	}

	// �e���f���̔ԍ�
	if (m_pNumParent != NULL)
	{// ���������m�ۂ���Ă���
		delete[] m_pNumParent;
		m_pNumParent = NULL;
	}

	// �����蔻��̐e�̔ԍ�
	if (m_pNumColParent != NULL)
	{// ���������m�ۂ���Ă���
		delete[] m_pNumColParent;
		m_pNumColParent = NULL;
	}

	// �g�p���郂�f���̔ԍ�
	if (m_pNumModelIdx != NULL)
	{// ���������m�ۂ���Ă���
		delete[] m_pNumModelIdx;
		m_pNumModelIdx = NULL;
	}
}

//=============================================================================
//    �z�u���𐶐����鏈��
//=============================================================================
void CObjectManager::CreateObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, bool bBreak, int nPriority)
{
	CModel **apModelCpy = NULL;                     // ���f���N���X�R�s�[�p
	CMotionAttack *pMotionAttack = NULL;            // �U�����[�V�����f�[�^�N���X�R�s�[�p
	CMotionCollision **pMotionCollision = NULL;     // ���[�V�����̓����蔻��f�[�^�N���X�R�s�[�p
	CMotionOrbit **pMotionOrbit = NULL;             // ���[�V�����̋O�Ճf�[�^�N���X�R�s�[�p
	CMotion *pMotionCpy[MAX_OBJECT_MOTION] = {};    // ���[�W�����N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;       // ���[�V�����Ǌ��N���X�R�s�[�p
	CCollision **pCollisionCpy = NULL;              // �����蔻��N���X�R�s�[�p
	CBoxCollider *pBoxCollider = NULL;              // �{�b�N�X�R���C�_�[�N���X
	CCircleCollider *pCircleCollider = NULL;        // �T�[�N���R���C�_�[�N���X
	CSphereCollider *pSphereCollider = NULL;        // �X�t�B�A�R���C�_�[�N���X
	CObject *pObject = NULL;                        // �z�u���N���X�ւ̃|�C���^

	if (m_apModel != NULL && m_pMotionManager != NULL)
	{// ���f���ւ̃|�C���^�ƃ��[�V�����N���X�ւ̃|�C���^�ƃ��f���Ǌ��N���X�ւ̃|�C���^���쐬����Ă���
	    // ���f�������R�s�[����
		if (apModelCpy == NULL)
		{// ���������m�ۂ���Ă���
			apModelCpy = new CModel*[m_nNumParts];
			if (apModelCpy != NULL)
			{// ���������m�ۂł���
				for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
				{// �p�[�c�����J��Ԃ�
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// ���������m�ۂł����Ԃł���
						apModelCpy[nCntParts] = CModel::Create(m_apModel[nCntParts]->GetPos(), m_apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(m_pNumModelIdx[nCntParts]), m_pModelManager->GetBuffMat(m_pNumModelIdx[nCntParts]), m_pModelManager->GetNumMat(m_pNumModelIdx[nCntParts]), m_pModelManager->GetVtxMax(m_pNumModelIdx[nCntParts]), m_pModelManager->GetVtxMin(m_pNumModelIdx[nCntParts]), m_pModelManager->GetTexture(m_pNumModelIdx[nCntParts]));
						if (apModelCpy[nCntParts] != NULL)
						{// ���������m�ۂł���
							if (m_pNumParent[nCntParts] != -1)
							{// �e���f�������݂���
								apModelCpy[nCntParts]->SetParent(apModelCpy[m_pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// ���[�V���������R�s�[����
		if (pMotionManagerCpy == NULL && m_pMotionManager != NULL)
		{// ���������m�ۂł����Ԃł���
			pMotionManagerCpy = CMotionManager::Create(MAX_OBJECT_MOTION, m_nNumParts);
			if (pMotionManagerCpy != NULL)
			{// ���[�V�����Ǘ��N���X���쐬�ł���
				for (int nCntMotion = 0; nCntMotion < MAX_OBJECT_MOTION; nCntMotion++)
				{// �v���C���[�̏�Ԃ̐������J��Ԃ�
					if (pMotionCpy[nCntMotion] == NULL && m_pMotionManager->GetMotion()[nCntMotion] != NULL)
					{// ���������m�ۂł����Ԃł���
						pMotionCpy[nCntMotion] = CMotion::Create(m_nNumParts, m_pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), m_pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), m_pMotionManager->GetMotion()[nCntMotion]->GetLoop(), m_pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// ���������m�ۂł���
							for (int nCntKey = 0; nCntKey < m_pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// �L�[�t���[���̑������J��Ԃ�
								for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
								{// �L�[�t���[���̑������J��Ԃ�
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(m_pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(m_pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// �U�����[�V�����f�[�^�N���X�̃R�s�[
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetAttack() != NULL)
							{// �U�����[�V�����f�[�^�N���X����������Ă���
								pMotionAttack = new CMotionAttack;
								if (pMotionAttack != NULL)
								{// ���������m�ۂł���
								    // �e��l�̐ݒ�
									pMotionAttack->SetNextMotionIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetNextMotionIdx());
									pMotionAttack->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetStartTiming());
									pMotionAttack->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetFinishTiming());
									pMotionCpy[nCntMotion]->SetAttack(pMotionAttack);
								}
							}

							// �����蔻��f�[�^�N���X�̃R�s�[
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData() >= 1)
							{// �����蔻��f�[�^���P�ȏ゠��
								pMotionCollision = new CMotionCollision*[m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData()];
								if (pMotionCollision != NULL)
								{// ���������m�ۂł���
									for (int nCntCol = 0; nCntCol < m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData(); nCntCol++)
									{// �����蔻��f�[�^�̐������J��Ԃ�
										pMotionCollision[nCntCol] = new CMotionCollision;
										if (pMotionCollision[nCntCol] != NULL)
										{// ���������m�ۂł���
											pMotionCollision[nCntCol]->SetModelIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetModelIdx());
											pMotionCollision[nCntCol]->SetOffsetPos(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetOffsetPos());
											pMotionCollision[nCntCol]->SetRange(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetRange());
											pMotionCollision[nCntCol]->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetStartTiming());
											pMotionCollision[nCntCol]->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetFinishTiming());
											pMotionCollision[nCntCol]->SetDamage(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetDamage());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetCollision(pMotionCollision);
								pMotionCpy[nCntMotion]->SetNumColData(m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData());
							}

							// �O�Ճf�[�^�N���X�̃R�s�[
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData() >= 1)
							{// �����蔻��f�[�^���P�ȏ゠��
								pMotionOrbit = new CMotionOrbit*[m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData()];
								if (pMotionOrbit != NULL)
								{// ���������m�ۂł���
									for (int nCntOrbit = 0; nCntOrbit < m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData(); nCntOrbit++)
									{// �����蔻��f�[�^�̐������J��Ԃ�
										pMotionOrbit[nCntOrbit] = new CMotionOrbit;
										if (pMotionOrbit[nCntOrbit] != NULL)
										{// ���������m�ۂł���
											pMotionOrbit[nCntOrbit]->SetModelIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetModelIdx());
											pMotionOrbit[nCntOrbit]->SetTexIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetTexIdx());
											pMotionOrbit[nCntOrbit]->SetOffsetPos1(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos1());
											pMotionOrbit[nCntOrbit]->SetOffsetPos2(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos2());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp1(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp1());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp2(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp2());
											pMotionOrbit[nCntOrbit]->SetColUp(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColUp());
											pMotionOrbit[nCntOrbit]->SetColDown(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColDown());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayUp());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayDown());
											pMotionOrbit[nCntOrbit]->SetXBlock(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetXBlock());
											pMotionOrbit[nCntOrbit]->SetYBlock(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetYBlock());
											pMotionOrbit[nCntOrbit]->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetStartTiming());
											pMotionOrbit[nCntOrbit]->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetFinishTiming());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetOrbit(pMotionOrbit);
								pMotionCpy[nCntMotion]->SetNumOrbitData(m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());
							}

							// ���[�V�����Ǌ��N���X�Ƀ|�C���^��n��
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// �����蔻������R�s�[����
		CBoxCollider *pBoxColliderCpy = NULL;           // �{�b�N�X�R���C�_�[�N���X�R�s�[�p
		CCircleCollider *pCircleColliderCpy = NULL;     // �T�[�N���R���C�_�[�N���X�R�s�[�p
		CSphereCollider *pSphereColliderCpy = NULL;     // �X�t�B�A�R���C�_�[�N���X�R�s�[�p
		if (pCollisionCpy == NULL && m_apCollision != NULL)
		{// ���������m�ۂł����Ԃł���
			pCollisionCpy = new CCollision*[m_nNumCollision];
			for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
			{// �����蔻��̐������J��Ԃ�
				if (m_apCollision[nCntCol] != NULL)
				{// ���������m�ۂł��Ă���
					if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_BOX)
					{// �{�b�N�X�R���C�_�[��������
						pBoxCollider = (CBoxCollider*)m_apCollision[nCntCol];
						pBoxColliderCpy = CBoxCollider::Create(pBoxCollider->GetPos(), pBoxCollider->GetWidth(), pBoxCollider->GetHeight(), pBoxCollider->GetDepth(), pBoxCollider->GetReturnFlag(), pBoxCollider->GetType());
						if (pBoxColliderCpy != NULL)
						{// ���������m�ۂł���
							if (m_pNumColParent[nCntCol] != -1)
							{// �e���f�������݂���
								pBoxColliderCpy->SetParent(pCollisionCpy[m_pNumParent[nCntCol]]);
							}
							pCollisionCpy[nCntCol] = pBoxColliderCpy;
						}
					}
				}
			}
		}

		// �z�u���N���X���쐬����
		if (pObject == NULL)
		{// ���������m�ۂł����Ԃł���
			pObject = CObject::Create(pos, rot, scale, RollSpeed, nType, m_nNumParts, m_nNumCollision, apModelCpy, pMotionManagerCpy, pCollisionCpy, bBreak, nPriority);
		}
	}
}

//=============================================================================
//    �X�^�[�z�u���𐶐����鏈��
//=============================================================================
void CObjectManager::CreateStarObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, bool bBreak, int nPriority)
{
	CModel **apModelCpy = NULL;                     // ���f���N���X�R�s�[�p
	CMotionAttack *pMotionAttack = NULL;            // �U�����[�V�����f�[�^�N���X�R�s�[�p
	CMotionCollision **pMotionCollision = NULL;     // ���[�V�����̓����蔻��f�[�^�N���X�R�s�[�p
	CMotionOrbit **pMotionOrbit = NULL;             // ���[�V�����̋O�Ճf�[�^�N���X�R�s�[�p
	CMotion *pMotionCpy[MAX_OBJECT_MOTION] = {};    // ���[�W�����N���X�R�s�[�p
	CMotionManager *pMotionManagerCpy = NULL;       // ���[�V�����Ǌ��N���X�R�s�[�p
	CCollision **pCollisionCpy = NULL;              // �����蔻��N���X�R�s�[�p
	CBoxCollider *pBoxCollider = NULL;              // �{�b�N�X�R���C�_�[�N���X
	CCircleCollider *pCircleCollider = NULL;        // �T�[�N���R���C�_�[�N���X
	CSphereCollider *pSphereCollider = NULL;        // �X�t�B�A�R���C�_�[�N���X
	CObjectStar *pObject = NULL;                        // �z�u���N���X�ւ̃|�C���^

	if (m_apModel != NULL && m_pMotionManager != NULL)
	{// ���f���ւ̃|�C���^�ƃ��[�V�����N���X�ւ̃|�C���^�ƃ��f���Ǌ��N���X�ւ̃|�C���^���쐬����Ă���
	 // ���f�������R�s�[����
		if (apModelCpy == NULL)
		{// ���������m�ۂ���Ă���
			apModelCpy = new CModel*[m_nNumParts];
			if (apModelCpy != NULL)
			{// ���������m�ۂł���
				for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
				{// �p�[�c�����J��Ԃ�
					apModelCpy[nCntParts] = NULL;
					if (apModelCpy[nCntParts] == NULL)
					{// ���������m�ۂł����Ԃł���
						apModelCpy[nCntParts] = CModel::Create(m_apModel[nCntParts]->GetPos(), m_apModel[nCntParts]->GetRot(), m_pModelManager->GetMesh(m_pNumModelIdx[nCntParts]), m_pModelManager->GetBuffMat(m_pNumModelIdx[nCntParts]), m_pModelManager->GetNumMat(m_pNumModelIdx[nCntParts]), m_pModelManager->GetVtxMax(m_pNumModelIdx[nCntParts]), m_pModelManager->GetVtxMin(m_pNumModelIdx[nCntParts]), m_pModelManager->GetTexture(m_pNumModelIdx[nCntParts]));
						if (apModelCpy[nCntParts] != NULL)
						{// ���������m�ۂł���
							if (m_pNumParent[nCntParts] != -1)
							{// �e���f�������݂���
								apModelCpy[nCntParts]->SetParent(apModelCpy[m_pNumParent[nCntParts]]);
							}
						}
					}
				}
			}
		}

		// ���[�V���������R�s�[����
		if (pMotionManagerCpy == NULL && m_pMotionManager != NULL)
		{// ���������m�ۂł����Ԃł���
			pMotionManagerCpy = CMotionManager::Create(MAX_OBJECT_MOTION, m_nNumParts);
			if (pMotionManagerCpy != NULL)
			{// ���[�V�����Ǘ��N���X���쐬�ł���
				for (int nCntMotion = 0; nCntMotion < MAX_OBJECT_MOTION; nCntMotion++)
				{// �v���C���[�̏�Ԃ̐������J��Ԃ�
					if (pMotionCpy[nCntMotion] == NULL && m_pMotionManager->GetMotion()[nCntMotion] != NULL)
					{// ���������m�ۂł����Ԃł���
						pMotionCpy[nCntMotion] = CMotion::Create(m_nNumParts, m_pMotionManager->GetMotion()[nCntMotion]->GetNumKey(), m_pMotionManager->GetMotion()[nCntMotion]->GetBlendMag(), m_pMotionManager->GetMotion()[nCntMotion]->GetLoop(), m_pMotionManager->GetMotion()[nCntMotion]->GetPlayBackKey());
						if (pMotionCpy[nCntMotion] != NULL)
						{// ���������m�ۂł���
							for (int nCntKey = 0; nCntKey < m_pMotionManager->GetMotion()[nCntMotion]->GetNumKey(); nCntKey++)
							{// �L�[�t���[���̑������J��Ԃ�
								for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
								{// �L�[�t���[���̑������J��Ԃ�
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(m_pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetAddPos());
									pMotionCpy[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(m_pMotionManager->GetMotion()[nCntMotion]->GetKeyframe()[nCntKey][nCntParts]->GetDestRot());
								}
							}

							// �U�����[�V�����f�[�^�N���X�̃R�s�[
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetAttack() != NULL)
							{// �U�����[�V�����f�[�^�N���X����������Ă���
								pMotionAttack = new CMotionAttack;
								if (pMotionAttack != NULL)
								{// ���������m�ۂł���
								 // �e��l�̐ݒ�
									pMotionAttack->SetNextMotionIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetNextMotionIdx());
									pMotionAttack->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetStartTiming());
									pMotionAttack->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetAttack()->GetFinishTiming());
									pMotionCpy[nCntMotion]->SetAttack(pMotionAttack);
								}
							}

							// �����蔻��f�[�^�N���X�̃R�s�[
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData() >= 1)
							{// �����蔻��f�[�^���P�ȏ゠��
								pMotionCollision = new CMotionCollision*[m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData()];
								if (pMotionCollision != NULL)
								{// ���������m�ۂł���
									for (int nCntCol = 0; nCntCol < m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData(); nCntCol++)
									{// �����蔻��f�[�^�̐������J��Ԃ�
										pMotionCollision[nCntCol] = new CMotionCollision;
										if (pMotionCollision[nCntCol] != NULL)
										{// ���������m�ۂł���
											pMotionCollision[nCntCol]->SetModelIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetModelIdx());
											pMotionCollision[nCntCol]->SetOffsetPos(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetOffsetPos());
											pMotionCollision[nCntCol]->SetRange(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetRange());
											pMotionCollision[nCntCol]->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetStartTiming());
											pMotionCollision[nCntCol]->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetFinishTiming());
											pMotionCollision[nCntCol]->SetDamage(m_pMotionManager->GetMotion()[nCntMotion]->GetCollision()[nCntCol]->GetDamage());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetCollision(pMotionCollision);
								pMotionCpy[nCntMotion]->SetNumColData(m_pMotionManager->GetMotion()[nCntMotion]->GetNumColData());
							}

							// �O�Ճf�[�^�N���X�̃R�s�[
							if (m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData() >= 1)
							{// �����蔻��f�[�^���P�ȏ゠��
								pMotionOrbit = new CMotionOrbit*[m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData()];
								if (pMotionOrbit != NULL)
								{// ���������m�ۂł���
									for (int nCntOrbit = 0; nCntOrbit < m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData(); nCntOrbit++)
									{// �����蔻��f�[�^�̐������J��Ԃ�
										pMotionOrbit[nCntOrbit] = new CMotionOrbit;
										if (pMotionOrbit[nCntOrbit] != NULL)
										{// ���������m�ۂł���
											pMotionOrbit[nCntOrbit]->SetModelIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetModelIdx());
											pMotionOrbit[nCntOrbit]->SetTexIdx(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetTexIdx());
											pMotionOrbit[nCntOrbit]->SetOffsetPos1(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos1());
											pMotionOrbit[nCntOrbit]->SetOffsetPos2(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetPos2());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp1(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp1());
											pMotionOrbit[nCntOrbit]->SetOffsetAmp2(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetOffsetAmp2());
											pMotionOrbit[nCntOrbit]->SetColUp(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColUp());
											pMotionOrbit[nCntOrbit]->SetColDown(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetColDown());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayUp());
											pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetAlphaDecayDown());
											pMotionOrbit[nCntOrbit]->SetXBlock(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetXBlock());
											pMotionOrbit[nCntOrbit]->SetYBlock(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetYBlock());
											pMotionOrbit[nCntOrbit]->SetStartTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetStartTiming());
											pMotionOrbit[nCntOrbit]->SetFinishTiming(m_pMotionManager->GetMotion()[nCntMotion]->GetOrbit()[nCntOrbit]->GetFinishTiming());
										}
									}
								}
								pMotionCpy[nCntMotion]->SetOrbit(pMotionOrbit);
								pMotionCpy[nCntMotion]->SetNumOrbitData(m_pMotionManager->GetMotion()[nCntMotion]->GetNumOrbitData());
							}

							// ���[�V�����Ǌ��N���X�Ƀ|�C���^��n��
							pMotionManagerCpy->SetMotion(pMotionCpy[nCntMotion], nCntMotion);
						}
					}
				}
			}
		}

		// �����蔻������R�s�[����
		CBoxCollider *pBoxColliderCpy = NULL;           // �{�b�N�X�R���C�_�[�N���X�R�s�[�p
		CCircleCollider *pCircleColliderCpy = NULL;     // �T�[�N���R���C�_�[�N���X�R�s�[�p
		CSphereCollider *pSphereColliderCpy = NULL;     // �X�t�B�A�R���C�_�[�N���X�R�s�[�p
		if (pCollisionCpy == NULL && m_apCollision != NULL)
		{// ���������m�ۂł����Ԃł���
			pCollisionCpy = new CCollision*[m_nNumCollision];
			for (int nCntCol = 0; nCntCol < m_nNumCollision; nCntCol++)
			{// �����蔻��̐������J��Ԃ�
				if (m_apCollision[nCntCol] != NULL)
				{// ���������m�ۂł��Ă���
					if (m_apCollision[nCntCol]->GetType() == CCollision::TYPE_BOX)
					{// �{�b�N�X�R���C�_�[��������
						pBoxCollider = (CBoxCollider*)m_apCollision[nCntCol];
						pBoxColliderCpy = CBoxCollider::Create(pBoxCollider->GetPos(), pBoxCollider->GetWidth(), pBoxCollider->GetHeight(), pBoxCollider->GetDepth(), pBoxCollider->GetReturnFlag(), pBoxCollider->GetType());
						if (pBoxColliderCpy != NULL)
						{// ���������m�ۂł���
							if (m_pNumColParent[nCntCol] != -1)
							{// �e���f�������݂���
								pBoxColliderCpy->SetParent(pCollisionCpy[m_pNumParent[nCntCol]]);
							}
							pCollisionCpy[nCntCol] = pBoxColliderCpy;
						}
					}
				}
			}
		}

		// �z�u���N���X���쐬����
		if (pObject == NULL)
		{// ���������m�ۂł����Ԃł���
			pObject = CObjectStar::Create(pos, rot, scale, RollSpeed, nType, m_nNumParts, m_nNumCollision, apModelCpy, pMotionManagerCpy, pCollisionCpy, bBreak, nPriority);
		}
	}
}

//=============================================================================
//    �z�u������ǂݎ�鏈��
//=============================================================================
CModel **CObjectManager::ReadModelSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts)
{
	int nCntParts = 0;
	CModel **apModel = NULL;
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, NUM_PARTS) == 0)
		{// �p�[�c����񂪂���
			*pNumParts = (CFunctionLib::ReadInt(pStrCur, NUM_PARTS));
			m_pNumParent = new int[*pNumParts];
			m_pNumModelIdx = new int[*pNumParts];
			if (apModel == NULL)
			{// ���������m�ۂł����Ԃł���
				apModel = new CModel*[*pNumParts];
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARTSSET) == 0)
		{// �p�[�c���ǂݍ��݊J�n�̍��}������
			if (nCntParts < *pNumParts)
			{// �܂��p�[�c����ǂݍ��߂�
				apModel[nCntParts] = ReadPartsSet(pLine, pStrCur, pFile, pNumParts, m_pNumParent, apModel);
				nCntParts++;
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_MODELSET) == 0)
		{// �z�u�����ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return apModel;
}

//=============================================================================
//    �p�[�c����ǂݎ�鏈��
//=============================================================================
CModel *CObjectManager::ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel)
{
	int nIdx = 0;                                      // �p�[�c�ԍ����
	int nParent = 0;                                   // �e���f���̃p�[�c�ԍ�
	int nModelIdx = 0;
	CModel *pModel = NULL;                             // ���f���N���X�ւ̃|�C���^
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // ���f���̍��W���
	D3DXVECTOR3 Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // ���f���̌������
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
		{// �p�[�c�ԍ���񂪂���
			nIdx = CFunctionLib::ReadInt(pStrCur, INDEX);
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARENT) == 0)
		{// �e���f���̃p�[�c�ԍ���񂪂���
			pNumParent[nIdx] = nParent = CFunctionLib::ReadInt(pStrCur, PARENT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, MODEL_IDX) == 0)
		{// ���f���̔ԍ���񂪂���
			m_pNumModelIdx[nIdx] = nModelIdx = CFunctionLib::ReadInt(pStrCur, MODEL_IDX);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���f���̍��W��񂪂���
			Pos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ���f���̌�����񂪂���
			Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_PARTSSET) == 0)
		{// �p�[�c���ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	if (pModel == NULL)
	{// ���������m�ۂł����Ԃł���
		pModel = CModel::Create(Pos, Rot, m_pModelManager->GetMesh(m_pNumModelIdx[nIdx]), m_pModelManager->GetBuffMat(m_pNumModelIdx[nIdx]), m_pModelManager->GetNumMat(m_pNumModelIdx[nIdx]), m_pModelManager->GetVtxMax(nIdx), m_pModelManager->GetVtxMin(m_pNumModelIdx[nIdx]), m_pModelManager->GetTexture(m_pNumModelIdx[nIdx]));
		if (pModel != NULL)
		{// ���f���|�C���^���m�ۂ��ꂽ
			if (nParent != -1)
			{// �e���f�������݂���
				pModel->SetParent(apModel[nParent]);
			}
		}
	}

	return pModel;
}

//=============================================================================
//    ���[�V��������ǂݎ�鏈��
//=============================================================================
CMotion *CObjectManager::ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion)
{
	int nCntKeySet = 0;                           // �L�[�t���[���t���[������ǂݍ��񂾉�
	CMotionAttack *pMotionAttack = NULL;          // �U�����[�V�����f�[�^�N���X�ւ̃|�C���^
	CMotionCollision **pMotionCollision = NULL;   // ���[�V�����̓����蔻��f�[�^�N���X�ւ̃|�C���^
	int nNumColData = 0;                          // �����蔻��f�[�^�̐�
	CMotionOrbit **pMotionOrbit = NULL;           // ���[�V�����̋O�Ճf�[�^�N���X�ւ̃|�C���^
	int nNumOrbitData = 0;                        // �O�Ճf�[�^�̐�
	bool bLoop = false;                           // ���[�v���邩���Ȃ���
	float fBlendMag = 0.0f;                       // �u�����h�{��
	int nNumKey = 0;                              // �L�[�t���[���̑���
	int *pPlaybackKey = NULL;                     // �Đ��t���[����(��������̂œ��I�m��)
	D3DXVECTOR3 **pAddPos = NULL;                 // ���f���̍��W
	D3DXVECTOR3 **pDestRot = NULL;                // ���f���̌���
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, LOOP) == 0)
		{// ���[�v���邩���Ȃ����̏�񂪂���
			bLoop = CFunctionLib::ReadBool(pStrCur, LOOP);
		}
		else if (CFunctionLib::Memcmp(pStrCur, BLEND) == 0)
		{// �u�����h�{���̏�񂪂���
			fBlendMag = CFunctionLib::ReadFloat(pStrCur, BLEND);
		}
		else if (CFunctionLib::Memcmp(pStrCur, NUM_KEY) == 0)
		{// �L�[�t���[���̑����̏�񂪂���
			nNumKey = (CFunctionLib::ReadInt(pStrCur, NUM_KEY));
			if (nNumKey >= 1)
			{// 1�ȏ�L�[�t���[��������
			    // �K�v�ȕϐ��𓮓I�Ɋm�ۂ���
			    // �Đ��t���[����
				pPlaybackKey = new int[nNumKey];

				// ���W
				if (pAddPos == NULL)
				{// ���������m�ۂł����Ԃł���
					pAddPos = new D3DXVECTOR3*[nNumKey];
					if (pAddPos != NULL)
					{// ���������m�ۂł���
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// �L�[�t���[���̑������J��Ԃ�
							pAddPos[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}

				if (pDestRot == NULL)
				{// ���������m�ۂł����Ԃł���
				 // ����
					pDestRot = new D3DXVECTOR3*[nNumKey];
					if (pDestRot != NULL)
					{// ���������m�ۂł���
						for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
						{// �L�[�t���[���̑������J��Ԃ�
							pDestRot[nCntKey] = new D3DXVECTOR3[*pNumParts];
						}
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, ATTACKSET) == 0)
		{// �����蔻����ǂݍ��݊J�n�̍��}������
			pMotionAttack = ReadMotionAttackSet(pLine, pStrCur, pFile);
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISIONSET) == 0)
		{// �����蔻����ǂݍ��݊J�n�̍��}������
			pMotionCollision = ReadMotionCollisionSet(pLine, pStrCur, pFile, &nNumColData);
		}
		else if (CFunctionLib::Memcmp(pStrCur, ORBITSET) == 0)
		{// �O�Տ��ǂݍ��݊J�n�̍��}������
			pMotionOrbit = ReadMotionOrbitSet(pLine, pStrCur, pFile, &nNumOrbitData);
		}
		else if (CFunctionLib::Memcmp(pStrCur, KEYSET) == 0)
		{// �L�[�t���[�����ǂݍ��݊J�n�̍��}������
			ReadKeySet(pLine, pStrCur, pFile, nCntKeySet, pPlaybackKey, pAddPos[nCntKeySet], pDestRot[nCntKeySet]);
			nCntKeySet++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_MOTIONSET) == 0)
		{// ���[�V�������ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	if (pMotion == NULL)
	{// ���������m�ۂł����Ԃł���
		pMotion = CMotion::Create(*pNumParts, nNumKey, fBlendMag, bLoop, pPlaybackKey);
		if (pMotion != NULL)
		{// ���������m�ۂł���
		    // �U�����[�V�����f�[�^�N���X��ݒ肷��
			pMotion->SetAttack(pMotionAttack);

			// �����蔻��f�[�^�N���X��ݒ肷��
			pMotion->SetCollision(pMotionCollision);
			pMotion->SetNumColData(nNumColData);

			// �O�Ճf�[�^�N���X��ݒ肷��
			pMotion->SetOrbit(pMotionOrbit);
			pMotion->SetNumOrbitData(nNumOrbitData);

			for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
			{// �L�[�t���[���̑������J��Ԃ�
				for (int nCntParts = 0; nCntParts < *pNumParts; nCntParts++)
				{// �L�[�t���[���̑������J��Ԃ�
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetAddPos(pAddPos[nCntKey][nCntParts]);
					pMotion->GetKeyframe()[nCntKey][nCntParts]->SetDestRot(pDestRot[nCntKey][nCntParts]);
				}
			}
		}
	}

	// ���������m�ۂ������J������
	// �Đ��t���[����
	if (pPlaybackKey != NULL)
	{// ���������m�ۂ���Ă���
		delete[] pPlaybackKey;
		pPlaybackKey = NULL;
	}

	// ���W
	if (pAddPos != NULL)
	{// ���������m�ۂł���
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// �L�[�t���[���̑������J��Ԃ�
			delete[] pAddPos[nCntKey];
			pAddPos[nCntKey] = NULL;
		}
		delete[] pAddPos;
		pAddPos = NULL;
	}

	// ����
	if (pDestRot != NULL)
	{// ���������m�ۂł���
		for (int nCntKey = 0; nCntKey < nNumKey; nCntKey++)
		{// �L�[�t���[���̑������J��Ԃ�
			delete[] pDestRot[nCntKey];
			pDestRot[nCntKey] = NULL;
		}
		delete[] pDestRot;
		pDestRot = NULL;
	}

	return pMotion;
}

//=============================================================================
//    ���[�V�����̍U�����[�V��������ǂݎ�鏈��
//=============================================================================
CMotionAttack *CObjectManager::ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile)
{
	CMotionAttack *pMotionAttack = NULL;   // �U�����[�V�����f�[�^�N���X�ւ̃|�C���^
	pMotionAttack = new CMotionAttack;     // ���������m�ۂ���
	if (pMotionAttack != NULL)
	{// ���������m�ۂł���
		while (1)
		{// ���[�v�J�n
			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
			if (CFunctionLib::Memcmp(pStrCur, NEXT_MOTION) == 0)
			{// ���̃��[�V�����̔ԍ���񂾂���
				pMotionAttack->SetNextMotionIdx(CFunctionLib::ReadInt(pStrCur, NEXT_MOTION));
			}
			else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
			{// �O�Ղ��o���n�߂�^�C�~���O��񂾂���
				pMotionAttack->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
			}
			else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
			{// �O�Ղ����܂��^�C�~���O��񂾂���
				pMotionAttack->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
			}
			else if (CFunctionLib::Memcmp(pStrCur, END_ATTACKSET) == 0)
			{// �U�����[�V�����f�[�^�ǂݍ��ݏI���̍��}������
				break;  // ���[�v�I��
			}
		}
	}

	return pMotionAttack;
}

//=============================================================================
//    ���[�V�����̓����蔻�����ǂݎ�鏈��
//=============================================================================
CMotionCollision **CObjectManager::ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData)
{
	int nCntCollision = 0;                       // �����蔻��f�[�^��ǂݍ��񂾉�
	CMotionCollision **pMotionCollision = NULL;  // ���[�V�����̓����蔻��f�[�^�N���X�ւ̃|�C���^

	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, NUM_COL) == 0)
		{// �����蔻��f�[�^�̐�������
			*pNumColData = CFunctionLib::ReadInt(pStrCur, NUM_COL);
			if (*pNumColData >= 1)
			{// �����蔻��f�[�^���P�ȏ゠��
				pMotionCollision = new CMotionCollision*[*pNumColData];
				if (pMotionCollision != NULL)
				{// ���������m�ۂł���
					for (int nCntCol = 0; nCntCol < *pNumColData; nCntCol++)
					{// �����蔻��f�[�^�̐������J��Ԃ�
						pMotionCollision[nCntCol] = new CMotionCollision;
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
		{// �����蔻��f�[�^�ǂݍ��݊J�n�̍��}������
			while (1)
			{// ���[�v�J�n
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
				if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
				{// �����蔻�����郂�f���̔ԍ���񂾂���
					pMotionCollision[nCntCollision]->SetModelIdx(CFunctionLib::ReadInt(pStrCur, INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET) == 0)
				{// �����蔻�����郂�f������̃I�t�Z�b�g������񂾂���
					pMotionCollision[nCntCollision]->SetOffsetPos(CFunctionLib::ReadVector3(pStrCur, OFFSET));
				}
				else if (CFunctionLib::Memcmp(pStrCur, RANGE) == 0)
				{// �����蔻������͈͏�񂾂���
					pMotionCollision[nCntCollision]->SetRange(CFunctionLib::ReadFloat(pStrCur, RANGE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
				{// �����蔻������n�߂�^�C�~���O��񂾂���
					pMotionCollision[nCntCollision]->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
				}
				else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
				{// �����蔻����I������^�C�~���O��񂾂���
					pMotionCollision[nCntCollision]->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
				}
				else if (CFunctionLib::Memcmp(pStrCur, DAMAGE) == 0)
				{// �����������̃_���[�W�ʏ�񂾂���
					pMotionCollision[nCntCollision]->SetDamage(CFunctionLib::ReadInt(pStrCur, DAMAGE));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_COLLISION) == 0)
				{// �����蔻��f�[�^�ǂݍ��ݏI���̍��}������
					nCntCollision++;
					break;  // ���[�v�I��
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_COLLISIONSET) == 0)
		{// �����蔻����ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return pMotionCollision;
}

//=============================================================================
//    ���[�V�����̋O�Տ���ǂݎ�鏈��
//=============================================================================
CMotionOrbit **CObjectManager::ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData)
{
	int nCntOrbit = 0;                   // �O�Ճf�[�^��ǂݍ��񂾉�
	CMotionOrbit **pMotionOrbit = NULL;  // ���[�V�����̋O�Ճf�[�^�N���X�ւ̃|�C���^

	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, NUM_ORBIT) == 0)
		{// �O�Ճf�[�^�̐�������
			*pNumOrbitData = CFunctionLib::ReadInt(pStrCur, NUM_ORBIT);
			if (*pNumOrbitData >= 1)
			{// �����蔻��f�[�^���P�ȏ゠��
				pMotionOrbit = new CMotionOrbit*[*pNumOrbitData];
				if (pMotionOrbit != NULL)
				{// ���������m�ۂł���
					for (int nCntOr = 0; nCntOr < *pNumOrbitData; nCntOr++)
					{// �����蔻��f�[�^�̐������J��Ԃ�
						pMotionOrbit[nCntOr] = new CMotionOrbit;
					}
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, ORBIT) == 0)
		{// �O�Ճf�[�^�ǂݍ��݊J�n�̍��}������
			while (1)
			{// ���[�v�J�n
				pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
				if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
				{// �O�Ղ��o�����f���̔ԍ���񂾂���
					pMotionOrbit[nCntOrbit]->SetModelIdx(CFunctionLib::ReadInt(pStrCur, INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, TEX_INDEX) == 0)
				{// �O�Ղɒ���t����e�N�X�`���̔ԍ���񂾂���
					pMotionOrbit[nCntOrbit]->SetTexIdx(CFunctionLib::ReadInt(pStrCur, TEX_INDEX));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET1) == 0)
				{// �O�Ղ��o�����f������̃I�t�Z�b�g�������(�P��)������
					pMotionOrbit[nCntOrbit]->SetOffsetPos1(CFunctionLib::ReadVector3(pStrCur, OFFSET1));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSET2) == 0)
				{// �O�Ղ��o�����f������̃I�t�Z�b�g�������(�Q��)������
					pMotionOrbit[nCntOrbit]->SetOffsetPos2(CFunctionLib::ReadVector3(pStrCur, OFFSET2));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSETAMP1) == 0)
				{// �O�Ղ̃I�t�Z�b�g���������l(1��)������
					pMotionOrbit[nCntOrbit]->SetOffsetAmp1(CFunctionLib::ReadVector3(pStrCur, OFFSETAMP1));
				}
				else if (CFunctionLib::Memcmp(pStrCur, OFFSETAMP2) == 0)
				{// �O�Ղ̃I�t�Z�b�g���������l(2��)������
					pMotionOrbit[nCntOrbit]->SetOffsetAmp2(CFunctionLib::ReadVector3(pStrCur, OFFSETAMP2));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLUP) == 0)
				{// �O�Ղ̒��_�㑤�̐F������
					pMotionOrbit[nCntOrbit]->SetColUp(CFunctionLib::ReadVector4(pStrCur, COLUP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, COLDOWN) == 0)
				{// �O�Ղ̒��_�����̐F������
					pMotionOrbit[nCntOrbit]->SetColDown(CFunctionLib::ReadVector4(pStrCur, COLDOWN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ALPHA_DECAY_UP) == 0)
				{// �O�Ղ̒��_�㑤�̓����x�����l������
					pMotionOrbit[nCntOrbit]->SetAlphaDecayUp(CFunctionLib::ReadFloat(pStrCur, ALPHA_DECAY_UP));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ALPHA_DECAY_DOWN) == 0)
				{// �O�Ղ̒��_�����̓����x�����l������
					pMotionOrbit[nCntOrbit]->SetAlphaDecayDown(CFunctionLib::ReadFloat(pStrCur, ALPHA_DECAY_DOWN));
				}
				else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
				{// �O�Ղ̉��̕�������񂾂���
					pMotionOrbit[nCntOrbit]->SetXBlock(CFunctionLib::ReadInt(pStrCur, XBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, ZBLOCK) == 0)
				{// �O�Ղ̏c�̕�������񂾂���
					pMotionOrbit[nCntOrbit]->SetYBlock(CFunctionLib::ReadInt(pStrCur, ZBLOCK));
				}
				else if (CFunctionLib::Memcmp(pStrCur, START) == 0)
				{// �O�Ղ��o���n�߂�^�C�~���O��񂾂���
					pMotionOrbit[nCntOrbit]->SetStartTiming(CFunctionLib::ReadInt(pStrCur, START));
				}
				else if (CFunctionLib::Memcmp(pStrCur, FINISH) == 0)
				{// �O�Ղ����܂��^�C�~���O��񂾂���
					pMotionOrbit[nCntOrbit]->SetFinishTiming(CFunctionLib::ReadInt(pStrCur, FINISH));
				}
				else if (CFunctionLib::Memcmp(pStrCur, END_ORBIT) == 0)
				{// �O�Ճf�[�^�ǂݍ��ݏI���̍��}������
					nCntOrbit++;
					break;  // ���[�v�I��
				}
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_ORBITSET) == 0)
		{// �O�Տ��ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return pMotionOrbit;
}

//=============================================================================
//    �L�[�t���[������ǂݎ�鏈��
//=============================================================================
void CObjectManager::ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	int nCntKey = 0;
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, FRAME) == 0)
		{// �Đ��t���[����������
			pPlaybackKey[nCntKeySet] = CFunctionLib::ReadInt(pStrCur, FRAME);
		}
		if (CFunctionLib::Memcmp(pStrCur, KEY) == 0)
		{// �L�[���ǂݍ��݊J�n�̍��}������
			ReadKey(pLine, pStrCur, pFile, nCntKey, AddPos, DestRot);
			nCntKey++;
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEYSET) == 0)
		{// �L�[�t���[�����ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �L�[����ǂݎ�鏈��
//=============================================================================
void CObjectManager::ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot)
{
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// ���W������
			AddPos[nCntKey] = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
		{// ����������
			DestRot[nCntKey] = CFunctionLib::ReadVector3(pStrCur, ROT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_KEY) == 0)
		{// �L�[���ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}
}

//=============================================================================
//    �����蔻�����ǂݎ�鏈��
//=============================================================================
CCollision **CObjectManager::ReadCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumCollision)
{
	int nCntCol = 0;
	CCollision **apCollision = NULL;
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, NUM_COLLISION) == 0)
		{// �����蔻��̐���񂪂���
			*pNumCollision = (CFunctionLib::ReadInt(pStrCur, NUM_COLLISION));
			m_pNumColParent = new int[*pNumCollision];
			if (apCollision == NULL)
			{// ���������m�ۂł����Ԃł���
				apCollision = new CCollision*[*pNumCollision];
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, COLLISION) == 0)
		{// �����蔻��f�[�^���ǂݍ��݊J�n�̍��}������
			if (nCntCol < *pNumCollision)
			{// �܂������蔻��f�[�^����ǂݍ��߂�
				apCollision[nCntCol] = ReadCollision(pLine, pStrCur, pFile, apCollision);
				nCntCol++;
			}
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_COLLISIONSET) == 0)
		{// �z�u�����ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	return apCollision;
}

//=============================================================================
//    �����蔻��f�[�^��ǂݎ�鏈��
//=============================================================================
CCollision *CObjectManager::ReadCollision(char *pLine, char *pStrCur, FILE *pFile, CCollision **apCollision)
{
	int nIdx = 0;
	int nType = 0;                                     // ���
	int nParent = 0;                                   // �e���f���̃p�[�c�ԍ�
	CCollision *pCollision = NULL;                     // �����蔻��N���X�ւ̃|�C���^
	D3DXVECTOR3 Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �����蔻��̍��W���
	bool bReturnFlag = false;                          // ���������ۂɉ����߂����ǂ���
	float fWidth = 0.0f;                               // ����
	float fHeight = 0.0f;                              // �c��
	float fDepth = 0.0f;                               // ���s
	while (1)
	{// ���[�v�J�n
		pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
		if (CFunctionLib::Memcmp(pStrCur, INDEX) == 0)
		{// �ԍ���񂪂���
			nIdx = CFunctionLib::ReadInt(pStrCur, INDEX);
		}
		else if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
		{// ��ޏ�񂪂���
			nType = CFunctionLib::ReadInt(pStrCur, TYPE);
		}
		else if (CFunctionLib::Memcmp(pStrCur, PARENT) == 0)
		{// �e�̔ԍ���񂪂���
			m_pNumColParent[nIdx] = nParent = CFunctionLib::ReadInt(pStrCur, PARENT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
		{// �����蔻��̍��W��񂪂���
			Pos = CFunctionLib::ReadVector3(pStrCur, POS);
		}
		else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
		{// ������񂪂���
			fWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
		}
		else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
		{// �c����񂪂���
			fHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
		}
		else if (CFunctionLib::Memcmp(pStrCur, DEPTH) == 0)
		{// ���s��񂪂���
			fDepth = CFunctionLib::ReadFloat(pStrCur, DEPTH);
		}
		else if (CFunctionLib::Memcmp(pStrCur, RETURN) == 0)
		{// ���������ۂɉ����߂����ǂ�����񂪂���
			bReturnFlag = CFunctionLib::ReadBool(pStrCur, RETURN);
		}
		else if (CFunctionLib::Memcmp(pStrCur, END_COLLISION) == 0)
		{// �����蔻��f�[�^���ǂݍ��ݏI���̍��}������
			break;  // ���[�v�I��
		}
	}

	if (pCollision == NULL)
	{// ���������m�ۂł����Ԃł���
		if (nType == CCollision::TYPE_BOX)
		{// �����f����������
			pCollision = CBoxCollider::Create(Pos, fWidth, fHeight, fDepth, bReturnFlag, nType);
		}
		if (pCollision != NULL)
		{// ���f���|�C���^���m�ۂ��ꂽ
			if (nParent != -1)
			{// �e���f�������݂���
				pCollision->SetParent(apCollision[nParent]);
			}
		}
	}

	return pCollision;
}

//=============================================================================
//    �t�@�C�������擾����
//=============================================================================
void CObjectManager::SetFileName(const char *pFileName)
{
	strcpy(m_aFileName, pFileName);
}

//=============================================================================
//    �p�[�c�����擾����
//=============================================================================
void CObjectManager::SetNumParts(const int nNumParts)
{
	m_nNumParts = nNumParts;
}

//=============================================================================
//    �����蔻��̐���ݒ肷��
//=============================================================================
void CObjectManager::SetNumCollision(const int nNumCollision)
{
	m_nNumCollision = nNumCollision;
}

//=============================================================================
//    ���f���N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CObjectManager::SetModel(CModel **pModel)
{
	m_apModel = pModel;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CObjectManager::SetModelManager(CModelManager *pModelManager)
{
	m_pModelManager = pModelManager;
}

//=============================================================================
//    �R���W�����N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CObjectManager::SetCollision(CCollision **pCollision)
{
	m_apCollision = pCollision;
}

//=============================================================================
//    ���[�V�����N���X�ւ̃|�C���^��ݒ肷��
//=============================================================================
void CObjectManager::SetMotionManager(CMotionManager *pMotionManager)
{
	m_pMotionManager = pMotionManager;
}

//=============================================================================
//    �t�@�C�������擾����
//=============================================================================
char *CObjectManager::GetFileName(void)
{
	return &m_aFileName[0];
}

//=============================================================================
//    �p�[�c�����擾����
//=============================================================================
int CObjectManager::GetNumParts(void)
{
	return m_nNumParts;
}

//=============================================================================
//    �����蔻��̐����擾����
//=============================================================================
int CObjectManager::GetNumCollision(void)
{
	return m_nNumCollision;
}

//=============================================================================
//    ���f���N���X�ւ̃|�C���^���擾����
//=============================================================================
CModel **CObjectManager::GetModel(void)
{
	return m_apModel;
}

//=============================================================================
//    ���f���Ǌ��N���X�ւ̃|�C���^���擾����
//=============================================================================
CModelManager *CObjectManager::GetModelManager(void)
{
	return m_pModelManager;
}

//=============================================================================
//    ���[�V�����N���X�ւ̃|�C���^���擾����
//=============================================================================
CMotionManager *CObjectManager::GetMotionManager(void)
{
	return m_pMotionManager;
}

//=============================================================================
//    �R���W�����N���X�ւ̃|�C���^���擾����
//=============================================================================
CCollision **CObjectManager::GetCollision(void)
{
	return m_apCollision;
}
