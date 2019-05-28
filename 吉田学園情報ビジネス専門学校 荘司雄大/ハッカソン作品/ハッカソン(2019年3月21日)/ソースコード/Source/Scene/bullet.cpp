//*****************************************************************************
//
//     �e�̏���[bullet.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "bullet.h"
#include "manager.h"
#include "system.h"
#include "model.h"
#include "modelManager.h"
#include "object.h"
#include "character.h"
#include "map.h"

//*****************************************************************************
//    �}�N���錾
//*****************************************************************************
#define BULLET_HIT_LENGTH  (90.0f)                    // �e�̓����蔻�������͈�
#define BULLET_MODEL_FILENAME "data/MODEL/bullet.x"   // �e�̃��f����
#define BULLET_MAX_LIFE    (120)                      // �e�̗̑�

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
CModelManager *CBullet::m_pModelManager = NULL;

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBullet::CBullet(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_MtxWorld);
	m_pModel = NULL;
	m_nAttackPlayerIdx = 0;
	m_nLife = BULLET_MAX_LIFE;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nAttackPlayerIdx)
{
	CBullet *pBullet = NULL;               // �e�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pBullet == NULL)
		{// ����������ɂȂ��Ă���
			pBullet = new CBullet();
			if (pBullet != NULL)
			{// �C���X�^���X�𐶐��ł���
				pBullet->SetPos(pos);
				pBullet->SetRot(rot);
				pBullet->SetAttackPlayerIdx(nAttackPlayerIdx);
				pBullet->SetMove(move);
				if (FAILED(pBullet->Init()))
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

	return pBullet;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ���f���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CBullet::Load(void)
{
	LPD3DXMESH pMesh = NULL;                    // ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;               // �}�e���A�����ւ̃|�C���^
	DWORD nNumMat = 0;                          // �}�e���A�����̐�

	m_pModelManager = CModelManager::Create(1);

	// x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(BULLET_MODEL_FILENAME,
		D3DXMESH_SYSTEMMEM,
		CManager::GetRenderer()->GetDevice(),
		NULL,
		&pBuffMat,
		NULL,
		&nNumMat,
		&pMesh);

	// ���f���Ǌ��N���X�Ƀ|�C���^��ݒ肷��
	m_pModelManager->SetMesh(pMesh, 0);
	m_pModelManager->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, 0);

	return S_OK;
}

//=============================================================================
//    ���f���̊J������
//=============================================================================
void CBullet::UnLoad(void)
{
	if (m_pModelManager != NULL)
	{
		m_pModelManager->Uninit();

		delete m_pModelManager;
		m_pModelManager = NULL;
	}
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBullet::Init(void)
{
	// �̗͂�ݒ�
	m_nLife = BULLET_MAX_LIFE;

	// ���f���𐶐�����
	if (m_pModel == NULL)
	{
		m_pModel = CModel::Create(D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),m_pModelManager->GetMesh(0), m_pModelManager->GetBuffMat(0), m_pModelManager->GetNumMat(0), m_pModelManager->GetVtxMax(0), m_pModelManager->GetVtxMin(0), m_pModelManager->GetTexture(0));
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CBullet::Uninit(void)
{
	if (m_pModel != NULL)
	{
		m_pModel->Uninit();

		delete m_pModel;
		m_pModel = NULL;
	}
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBullet::Update(void)
{
	m_nLife--;
	if (m_nLife <= 0)
	{
		Uninit();
	}
	else
	{
		// �O��̍��W��ۑ�
		m_PosOld = m_Pos;

		// �e���ړ�������
		m_Pos += m_Move;

		// �����蔻��𔭐�������
		HitDecision();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBullet::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // �e�̃��[���h�}�g���b�N�X�v�Z����
			SetMtxWorld(pDevice);

			if (m_pModel != NULL)
			{// ���������m�ۂł���
				 // ���f���̕`�揈��
				m_pModel->Draw();
			}
		}
	}
}

//=============================================================================
//    �`��f�o�C�X�Ƀ��[���h�}�g���b�N�X��ݒ肷�鏈��
//=============================================================================
void CBullet::SetMtxWorld(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIX mtxRot, mtxTrans; // �v�Z�p�}�g���b�N�X

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_MtxWorld);

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
//    �e���v���C���[�ɓ����������ǂ����ݒ肷�鏈��
//=============================================================================
void CBullet::HitDecision(void)
{
	bool bDeath = false;
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	CObject *pObject = NULL;             // �z�u���N���X�ւ̃|�C���^
	CObjectStar *pObjectStar = NULL;
	CPlayer *pPlayer = NULL;
	for (int nCntPriority = FIELD_PRIORITY; nCntPriority < MAP_PRIORITY + 1; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (bDeath != true)
			{
				if (pScene->GetObjType() == OBJTYPE_OBJECT)
				{// �z�u���ɓ�������
					pObject = (CObject*)pScene;
					if (pObject->Collision(&m_Pos, &m_PosOld, &m_Move, D3DXVECTOR3(BULLET_HIT_LENGTH, BULLET_HIT_LENGTH, BULLET_HIT_LENGTH), NULL, this) == true)
					{
						pObject->Damage(1);
						bDeath = true;
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_OBJECTSTAR)
				{// �z�u���ɓ�������
					pObjectStar = (CObjectStar*)pScene;
					if (pObjectStar->Collision(&m_Pos, &m_PosOld, &m_Move, D3DXVECTOR3(BULLET_HIT_LENGTH, BULLET_HIT_LENGTH, BULLET_HIT_LENGTH), NULL, this) == true)
					{
						pObjectStar->Death();
						pObjectStar->Uninit();
						bDeath = true;
					}
				}
				else if (pScene->GetObjType() == OBJTYPE_PLAYER)
				{// �v���C���[�ɓ�������
					pPlayer = (CPlayer*)pScene;
					if (pPlayer->GetControllerIdx() != m_nAttackPlayerIdx)
					{// ���g��ł����v���C���[�ł͂Ȃ�
						bDeath = HitPlayer(pPlayer);
					}
				}
			}
			pScene = pSceneNext;
		}
	}

	if (bDeath == true)
	{// �t���O�������Ă���
		//CManager::GetSound()->PlaySound(5);
		Uninit();
	}
}

//=============================================================================
//    �v���C���[�Ɠ����������ǂ������肷�鏈��
//=============================================================================
bool CBullet::HitPlayer(CPlayer *pPlayer)
{
	bool bHit = false;
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	float fColRange = pPlayer->GetColRange();
	float fLength = 0.0f;
	float fHitRange = 0.0f;

	// �����蔻������͈͂��v�Z
	fHitRange = sqrtf((fColRange + BULLET_HIT_LENGTH) * (fColRange + BULLET_HIT_LENGTH));

	// �����蔻��������W�Ɣz�u���̍��W�Ƃ̋����𑪂�
	fLength = sqrtf((PlayerPos.x - m_Pos.x) * (PlayerPos.x - m_Pos.x) + (PlayerPos.y - m_Pos.y) * (PlayerPos.y - m_Pos.y) + (PlayerPos.z - m_Pos.z) * (PlayerPos.z - m_Pos.z));

	if (fLength <= fHitRange)
	{// �������͈͓��ł���
		bHit = true;
		pPlayer->Damage(m_Move);
	}

	return bHit;
}

//=============================================================================
//    ���W��ݒ肷�鏈��
//=============================================================================
void CBullet::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ������ݒ肷�鏈��
//=============================================================================
void CBullet::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    �ړ��ʂ�ݒ肷�鏈��
//=============================================================================
void CBullet::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    ���[���h�}�g���b�N�X��ݒ肷�鏈��
//=============================================================================
void CBullet::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    �U������v���C���[�̔ԍ���ݒ肷�鏈��
//=============================================================================
void CBullet::SetAttackPlayerIdx(const int nAttackPlayerIdx)
{
	m_nAttackPlayerIdx = nAttackPlayerIdx;
}

//=============================================================================
//    ���W��ݒ肷�鏈��
//=============================================================================
D3DXVECTOR3 CBullet::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ������ݒ肷�鏈��
//=============================================================================
D3DXVECTOR3 CBullet::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    �ړ��ʂ�ݒ肷�鏈��
//=============================================================================
D3DXVECTOR3 CBullet::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    ���[���h�}�g���b�N�X��ݒ肷�鏈��
//=============================================================================
D3DXMATRIX CBullet::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    �U������v���C���[�̔ԍ����擾���鏈��
//=============================================================================
int CBullet::GetAttackPlayerIdx(void)
{
	return m_nAttackPlayerIdx;
}