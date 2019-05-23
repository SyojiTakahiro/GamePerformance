//*****************************************************************************
//
//     �Q�[�W�̏���[gauge.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "gauge.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define GAUGE_LIFE_CUTTIMING  (60)    // �̗̓Q�[�W�����炵���n�܂�^�C�~���O

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//*****************************************************************************
//    CGauge�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGauge::CGauge(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// �e��l�̃N���A
	m_fGaugeRate = 0.0f;   // �Q�[�W�̊���
	m_fRightWidth = 0.0f;  // �Q�[�W�̍������W
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGauge::~CGauge()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGauge *CGauge::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, int nPriority)
{
	CGauge *pGauge = NULL;                 // �Q�[�W�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pGauge == NULL)
		{// ����������ɂȂ��Ă���
			pGauge = new CGauge(nPriority);
			if (pGauge != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pGauge->SetPos(pos);         // �|���S���̍��W
				pGauge->SetCol(col);         // �|���S���̐F
				pGauge->SetRot(fRot);        // �|���S���̌���
				pGauge->SetWidth(fWidth);    // �|���S���̕�
				pGauge->SetHeight(fHeight);  // �|���S���̍���

				if (FAILED(pGauge->Init()))
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

	return pGauge;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGauge::Init(void)
{
	// �������Ɏ��s����
	if (FAILED(CSceneBillboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGauge::Uninit(void)
{
	// ���ʂ̕`�揈��
	CSceneBillboard::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGauge::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGauge::Draw(void)
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
			CSceneBillboard::Draw();

			// ���C�e�B���O�̐ݒ�����ɖ߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);
		}
	}
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CGauge::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), GetHeight(), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

	// ���_�J���[
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);

	// �e��l���i�[���Ă���
	m_fRightWidth = -GetWidth();  // �Q�[�W�̍����I�t�Z�b�g
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CGauge::SetGaugeRate(const float fGaugeRate)
{
	m_fGaugeRate = fGaugeRate;
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CGauge::SetRightWidth(const float fRightWidth)
{
	m_fRightWidth = fRightWidth;
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
float CGauge::GetGaugeRate(void)
{
	return m_fGaugeRate;
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
float CGauge::GetRightWidth(void)
{
	return m_fRightWidth;
}



//*****************************************************************************
//    CGaugeLife�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGaugeLife::CGaugeLife(int nPriority, OBJTYPE objType) : CGauge(nPriority, objType)
{
	// �e��l�̃N���A
	m_nCounter = 0;            // �Q�[�W�̌��炵���Ǘ�����J�E���^�[
	m_State = STATE_NONE;      // ���
	m_nLife = 0;               // �̗�
	m_pRedBill = NULL;         // �̗̓Q�[�W�����������ɕ\������Ԃ��Q�[�W
	m_pWhiteBill = NULL;       // �̗̓Q�[�W�̃t���[��
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGaugeLife::~CGaugeLife()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGaugeLife *CGaugeLife::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, int nLife, int nPriority)
{
	CGaugeLife *pGaugeLife = NULL;         // �̗̓Q�[�W�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pGaugeLife == NULL)
		{// ����������ɂȂ��Ă���
			pGaugeLife = new CGaugeLife(nPriority);
			if (pGaugeLife != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pGaugeLife->SetPos(pos);         // �|���S���̍��W
				pGaugeLife->SetCol(col);         // �|���S���̐F
				pGaugeLife->SetRot(fRot);        // �|���S���̌���
				pGaugeLife->SetWidth(fWidth);    // �|���S���̕�
				pGaugeLife->SetHeight(fHeight);  // �|���S���̍���
				pGaugeLife->SetLife(nLife);      // �̗�

				if (FAILED(pGaugeLife->Init()))
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

	return pGaugeLife;  // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGaugeLife::Init(void)
{
	// �������Ɏ��s����
	if (FAILED(CGauge::Init()))
	{
		return E_FAIL;
	}

	// �̗̓Q�[�W�̃t���[���𐶐�����
	if (m_pWhiteBill == NULL)
	{
		m_pWhiteBill = CGauge::Create(GetPos(), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, (GetWidth() * 1.04f), (GetHeight() * 1.1f),2);
	}

	// ���̐Ԃ��Q�[�W�𐶐�����
	if (m_pRedBill == NULL)
	{
		m_pRedBill = CGauge::Create(GetPos(), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, GetWidth(), GetHeight(), 2);
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGaugeLife::Uninit(void)
{
	// �̗̓Q�[�W�̃t���[���J������
	if (m_pWhiteBill != NULL)
	{
		m_pWhiteBill->Uninit();
		m_pWhiteBill = NULL;
	}

	// �Ԃ��Q�[�W�̊J������
	if (m_pRedBill != NULL)
	{
		m_pRedBill->Uninit();
		m_pRedBill = NULL;
	}

	// ���ʂ̕`�揈��
	CGauge::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGaugeLife::Update(void)
{
	if (m_State == STATE_CUT)
	{// �Q�[�W�����炷��Ԃ�������
		m_nCounter++;  // �J�E���^�[�𑝂₷
		if (m_nCounter >= GAUGE_LIFE_CUTTIMING)
		{// �Q�[�W�����炷�^�C�~���O�ɂȂ��Ă���
			// ���̐Ԃ��Q�[�W�����炷
			if (m_pRedBill != NULL)
			{// �Ԃ��Q�[�W����������Ă���
				// �Ԃ��Q�[�W�̕������������炷
				float fWidth = m_pRedBill->GetWidth();
				fWidth -= 0.5f;
				if (fWidth <= GetWidth())
				{// �Ԃ��Q�[�W�̕����̗̓Q�[�W���������
					fWidth = GetWidth();   // ����߂�
					m_State = STATE_NONE;  // ��Ԃ�߂�
					m_nCounter = 0;        // �J�E���^�[��߂�
				}
				m_pRedBill->SetWidth(fWidth);

				// �|���S���̕������炷
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pRedBill->GetVtxBuff();
				if (pVtxBuff != NULL)
				{// ���_�o�b�t�@����������Ă���
					// ���_���̐ݒ�
					VERTEX_3D *pVtx;

					// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// ���_���W
					pVtx[0].pos = D3DXVECTOR3(GetRightWidth(), GetHeight(), 0.0f);
					pVtx[1].pos = D3DXVECTOR3(fWidth, GetHeight(), 0.0f);
					pVtx[2].pos = D3DXVECTOR3(GetRightWidth(), -GetHeight(), 0.0f);
					pVtx[3].pos = D3DXVECTOR3(fWidth, -GetHeight(), 0.0f);

					// ���_�o�b�t�@���A�����b�N����
					pVtxBuff->Unlock();
				}
			}
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGaugeLife::Draw(void)
{
	// ���ʂ̕`�揈��
	CGauge::Draw();
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CGaugeLife::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL);

	// ���_���̐ݒ�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(-GetWidth(), GetHeight(), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-GetWidth(), -GetHeight(), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

	// ���_�J���[
	pVtx[0].col = GetCol();
	pVtx[1].col = GetCol();
	pVtx[2].col = GetCol();
	pVtx[3].col = GetCol();

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	pVtxBuff->Unlock();

	// ���_�o�b�t�@��ݒ肷��
	SetVtxBuff(pVtxBuff);

	// �e��l���i�[���Ă���
	SetRightWidth(-GetWidth());                   // �Q�[�W�̍����I�t�Z�b�g
	SetGaugeRate((GetWidth() * 2.0f) / m_nLife);  // �Q�[�W��{���̊���
}

//=============================================================================
//    �Q�[�W�����炷����
//=============================================================================
void CGaugeLife::CutGauge(const int nCutValue)
{
	// ��Ԃ�ݒ�
	m_State = STATE_CUT;

	// �Q�[�W�̕������炷
	float fWidth = GetWidth();  // ��
	fWidth -= GetGaugeRate() * nCutValue;
	SetWidth(fWidth);

	// �|���S���̕������炷
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
		// ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(GetRightWidth(), GetHeight(), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(GetRightWidth(), -GetHeight(), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �Q�[�W�𑝂₷����
//=============================================================================
void CGaugeLife::AddGauge(const int nAddValue)
{
	// �Q�[�W�̕��𑝂₷
	float fWidth = GetWidth();  // ��
	fWidth += GetGaugeRate() * nAddValue;
	SetWidth(fWidth);

	// �|���S���̕��𑝂₷
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();
	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	    // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(GetRightWidth(), GetHeight(), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(GetWidth(), GetHeight(), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(GetRightWidth(), -GetHeight(), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(GetWidth(), -GetHeight(), 0.0f);

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �Q�[�W�̍��W�����炷����
//=============================================================================
void CGaugeLife::SetGaugePos(const D3DXVECTOR3 pos)
{
	SetPos(pos);

	// �Ԃ��Q�[�W����������Ă���
	if (m_pRedBill != NULL)
	{
		m_pRedBill->SetPos(pos);
	}

	// �Q�[�W�̃t���[������������Ă���
	if (m_pWhiteBill != NULL)
	{
		m_pWhiteBill->SetPos(pos);
	}
}

//=============================================================================
//    ��Ԑݒ菈��
//=============================================================================
void CGaugeLife::SetState(const STATE State)
{
	m_State = State;
}

//=============================================================================
//    �̗͐ݒ菈��
//=============================================================================
void CGaugeLife::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    ��Ԏ擾����
//=============================================================================
CGaugeLife::STATE CGaugeLife::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �̗͎擾����
//=============================================================================
int CGaugeLife::GetLife(void)
{
	return m_nLife;
}