//*****************************************************************************
//
//     ���̏���[star.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "star.h"
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
#define STAR_HIT_LENGTH  (90.0f)                    // ���̓����蔻�������͈�
#define STAR_MODEL_FILENAME "data/MODEL/star.x"     // ���̃��f����
#define STAR_MAX_LIFE    (240)                      // ���̗̑�

#define STAR_GRAVITY     (-0.4f)                    // ���̏d��
#define STAR_INERTIA     (0.04f)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
CModelManager *CStar::m_pModelManager = NULL;

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CStar::CStar(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_PosOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_MtxWorld);
	m_pModel = NULL;
	m_nAttackPlayerIdx = 0;
	m_bGetOk = false;
	m_nLife = STAR_MAX_LIFE;
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CStar::~CStar()
{

}

//=============================================================================
//    ��������
//=============================================================================
CStar *CStar::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Move)
{
	CStar *pStar = NULL;               // �e�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pStar == NULL)
		{// ����������ɂȂ��Ă���
			pStar = new CStar();
			if (pStar != NULL)
			{// �C���X�^���X�𐶐��ł���
				pStar->SetPos(pos);
				pStar->SetRot(rot);
				pStar->SetMove(Move);
				if (FAILED(pStar->Init()))
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

	return pStar;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ���f���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CStar::Load(void)
{
	LPD3DXMESH pMesh = NULL;                    // ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER pBuffMat = NULL;               // �}�e���A�����ւ̃|�C���^
	DWORD nNumMat = 0;                          // �}�e���A�����̐�

	m_pModelManager = CModelManager::Create(1);

	// x�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(STAR_MODEL_FILENAME,
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
void CStar::UnLoad(void)
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
HRESULT CStar::Init(void)
{
	// ���f���𐶐�����
	if (m_pModel == NULL)
	{
		m_pModel = CModel::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pModelManager->GetMesh(0), m_pModelManager->GetBuffMat(0), m_pModelManager->GetNumMat(0), m_pModelManager->GetVtxMax(0), m_pModelManager->GetVtxMin(0), m_pModelManager->GetTexture(0));
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CStar::Uninit(void)
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
void CStar::Update(void)
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

		// �ړ�������
		m_Pos += m_Move;

		// ������t����
		m_Move.y += STAR_GRAVITY;

		// �ړ��ʂɊ�����������
		m_Move.x += (0.0f - m_Move.x) * STAR_INERTIA;
		m_Move.z += (0.0f - m_Move.z) * STAR_INERTIA;

		if (m_Pos.y <= 0.0f)
		{
			m_Pos.y = 0.0f;
			m_bGetOk = true;
		}
		if (m_Pos.x >= 2000.0f)
		{
			m_Pos.x = 2000.0f;
		}
		else if (m_Pos.x <= -2000.0f)
		{
			m_Pos.x = -2000.0f;
		}
		if (m_Pos.z >= 2000.0f)
		{
			m_Pos.z = 2000.0f;
		}
		else if (m_Pos.z <= -2000.0f)
		{
			m_Pos.z = -2000.0f;
		}

		// �����蔻��𔭐�������
		HitDecision();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CStar::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���̃��[���h�}�g���b�N�X�v�Z����
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
void CStar::SetMtxWorld(LPDIRECT3DDEVICE9 pDevice)
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
//    �����v���C���[�ɓ����������ǂ����ݒ肷�鏈��
//=============================================================================
void CStar::HitDecision(void)
{
	bool bDeath = false;
	CScene *pScene = NULL;               // �V�[���N���X�ւ̃|�C���^
	CScene *pSceneNext = NULL;           // ���̃V�[���N���X�ւ̃|�C���^
	CObject *pObject = NULL;             // �z�u���N���X�ւ̃|�C���^
	CPlayer *pPlayer = NULL;
	for (int nCntPriority = FIELD_PRIORITY; nCntPriority < MAP_PRIORITY + 1; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = CScene::GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (bDeath != true)
			{
				if (pScene->GetObjType() == OBJTYPE_PLAYER)
				{// �v���C���[�ɓ�������
					pPlayer = (CPlayer*)pScene;
					bDeath = HitPlayer(pPlayer);
				}
			}
			pScene = pSceneNext;
		}
	}

	if (bDeath == true)
	{// �t���O�������Ă���
		Uninit();
	}
}

//=============================================================================
//    �v���C���[�Ɠ����������ǂ������肷�鏈��
//=============================================================================
bool CStar::HitPlayer(CPlayer *pPlayer)
{
	bool bHit = false;
	D3DXVECTOR3 PlayerPos = pPlayer->GetPos();
	float fColRange = pPlayer->GetColRange();
	float fLength = 0.0f;
	float fHitRange = 0.0f;

	// �����蔻������͈͂��v�Z
	fHitRange = sqrtf((fColRange + STAR_HIT_LENGTH) * (fColRange + STAR_HIT_LENGTH));

	// �����蔻��������W�Ɣz�u���̍��W�Ƃ̋����𑪂�
	fLength = sqrtf((PlayerPos.x - m_Pos.x) * (PlayerPos.x - m_Pos.x) + (PlayerPos.y - m_Pos.y) * (PlayerPos.y - m_Pos.y) + (PlayerPos.z - m_Pos.z) * (PlayerPos.z - m_Pos.z));

	if (fLength <= fHitRange)
	{// �������͈͓��ł���
		if (m_bGetOk == true)
		{// �擾�ł���Ȃ�
			bHit = true;
			pPlayer->AddScore();
		}
	}

	return bHit;
}

//=============================================================================
//    ���W��ݒ肷�鏈��
//=============================================================================
void CStar::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    ������ݒ肷�鏈��
//=============================================================================
void CStar::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    �ړ��ʂ�ݒ肷�鏈��
//=============================================================================
void CStar::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    ���[���h�}�g���b�N�X��ݒ肷�鏈��
//=============================================================================
void CStar::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    ���W��ݒ肷�鏈��
//=============================================================================
D3DXVECTOR3 CStar::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    ������ݒ肷�鏈��
//=============================================================================
D3DXVECTOR3 CStar::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    �ړ��ʂ�ݒ肷�鏈��
//=============================================================================
D3DXVECTOR3 CStar::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    ���[���h�}�g���b�N�X��ݒ肷�鏈��
//=============================================================================
D3DXMATRIX CStar::GetMtxWorld(void)
{
	return m_MtxWorld;
}