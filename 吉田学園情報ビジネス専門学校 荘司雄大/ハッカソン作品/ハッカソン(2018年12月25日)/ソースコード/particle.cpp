//*****************************************************************************
//
//     �p�[�e�B�N���̏���[particle.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "functionlib.h"

//*****************************************************************************
//    CParData�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CParData::CParData()
{
	m_Range = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // ��������͈�
	m_fRangeMag = 0.0f;                              // ��������͈͂̔{��
	m_fMaxRot = 0.0f;                                // �������̌���(�ő�l)
	m_fMinRot = 0.0f;                                // �������̌���(�ŏ��l)
	m_MaxMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �ړ��ʂ̍ő�l
	m_MinMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �ړ��ʂ̍ŏ��l
	m_ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �ړ��ʂ̕ω���
	m_InitCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // �������̐F
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // �t���[�����̐F�̕ω���
	m_fMaxWidth = 0.0f;                              // ���̍ő�l
	m_fMinWidth = 0.0f;                              // ���̍ŏ��l
	m_fMaxHeight = 0.0f;                             // �����̍ő�l
	m_fMinHeight = 0.0f;                             // �����̍ŏ��l
	m_fSpread = 0.0f;                                // �傫���̕ω���
	m_nMaxLife = 0;                                  // �����̍ő�l
	m_nMinLife = 0;                                  // �����̍ŏ��l
	m_fGravity = 0.0f;                               // ���t���[��������d��
	m_fBouncing = 0.0f;                              // �o�E���h��
	m_nRotPattern = 0;                               // ��]�̎��[ 0:���v��� 1:�����v��� 2:�����_�� ]
	m_fRotSpeed = 0.0f;                              // ��]����X�s�[�h
	m_bCollision = false;                            // �����蔻������邩�ǂ���
	m_bDrawAddtive = true;                           // ���Z�������邩���Ȃ���[ 0:�Ȃ� 1:���� ]
	m_bLighting = false;                             // ���C�e�B���O���邩���Ȃ���[ 0:�Ȃ� 1:���� ]
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CParData::~CParData()
{

}

//*****************************************************************************
//    CParticle�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CParticle::CParticle(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// �e��l�̃N���A
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // �ǂꂭ�炢�F���ω����邩
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // �ړ���
	m_ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �ړ��ʂ̕ω���
	m_fGravity = 0.0f;                               // �d��
	m_fSpread = 0.0f;                                // �ǂꂭ�炢�ό`���邩
	m_fChangeRot = 0.0f;                             // �ǂꂭ�炢��]���邩
	m_fBouncing = 0.0f;                              // �n�ʂɂ����ۂɃo�E���h����l
	m_nLife = 0;                                     // ����
	m_nRotPattern = 0;                               // ��]�̎��
	m_bCollision = false;                            // �o�E���h���邩�ǂ���
	m_bDrawAddtive = false;                          // ���Z�����ŕ`�悷�邩�ǂ���
	m_bLighting = false;                             // ���C�e�B���O���邩�ǂ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
//    ��������
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col,
	float fWidth, float fHeight, float fRot, D3DXCOLOR ChangeCol, float fGravity, float fSpread,
	float fChangeRot, float fBouncing, int nLife, int nRotPattern,
	bool bCol, bool bDrawAddtive, bool bLighting, int nPriority)
{
	CParticle *pParticle = NULL;  // �p�[�e�B�N���N���X�^�̃|�C���^
	if (pParticle == NULL)
	{// ����������ɂȂ��Ă���
		pParticle = new CParticle(nPriority);
		if (pParticle != NULL)
		{// �C���X�^���X�𐶐��ł���
			if (FAILED(pParticle->Init(pos, move, ChangeMove, col, fWidth, fHeight, fRot, ChangeCol, fGravity, fSpread, fChangeRot, fBouncing, nLife, nRotPattern, bCol, bDrawAddtive, bLighting)))
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

	return pParticle;   // �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CParticle::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col,
	float fWidth, float fHeight, float fRot, D3DXCOLOR ChangeCol, float fGravity, float fSpread,
	float fChangeRot, float fBouncing, int nLife, int nRotPattern,
	bool bCol, bool bDrawAddtive, bool bLighting)
{
	// �e��l�̐ݒ�
	SetPos(pos);                       // ���W
	SetCol(col);                       // �F
	SetWidth(fWidth);                  // ��
	SetHeight(fHeight);                // ����
	SetRot(fRot);                      // ����
	m_Move = move;                     // �ړ���
	m_ChangeMove = ChangeMove;         // �ړ��ʂ̕ω���
	m_ChangeCol = ChangeCol;           // �F
	m_fGravity = fGravity;             // ���a
	m_fSpread = fSpread;               // ���a�̕ω���
	m_fChangeRot = fChangeRot;         // �����̕ω���
	m_fBouncing = fBouncing;           // �o�E���h��
	m_nLife = nLife;                   // ����
	m_nRotPattern = nRotPattern;       // ��]�̃p�^�[��
	m_bCollision = bCol;               // �o�E���h���邩���Ȃ���
	m_bDrawAddtive = bDrawAddtive;     // ���Z�����ŕ`�悷�邩���Ȃ���
	m_bLighting = bLighting;           // ���C�e�B���O���邩���Ȃ���

	// ���ʂ̏���������
	if (FAILED(CSceneBillboard::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CParticle::Uninit(void)
{
	// ���ʂ̏I������
	CSceneBillboard::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CParticle::Update(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();  // ���W
	D3DXCOLOR col = GetCol();    // �F
	float fRot = GetRot();       // ����
	float fRadius = GetLength(); // ���a

	m_nLife--;               // ���������炷
	fRadius += m_fSpread;    // �傫���̍X�V

	if (m_nLife <= 0 || fRadius < 0.0f)
	{// ������0�ȉ��ɂȂ���
		Uninit();	// �I������
	}
	else
	{// �܂��\�����Ԃ�����
		m_Move.y -= m_fGravity;		 // �d�͂�������

		// �e��l�̍X�V
		pos += m_Move;				 // �ʒu�̍X�V
		col += m_ChangeCol;          // �F�̍X�V

		// �ړ��ʂ�ω�������
		m_Move.x += (m_Move.x * m_ChangeMove.x);
		m_Move.y += (m_Move.y * m_ChangeMove.y);
		m_Move.z += (m_Move.z * m_ChangeMove.z);

		// �����蔻�����邩�`�F�b�N
		if (pos.y <= 0 && m_bCollision == true)
		{// �o�E���h
			m_Move.y += (m_fBouncing - pos.y) * 2.0f;
			m_fBouncing = pos.y;   // �l���L��
		}

		// ��������]������
		if (m_nRotPattern == 0)
		{// ���v���
			fRot -= m_fChangeRot;
		}
		else if (m_nRotPattern == 1)
		{// �����v���
			fRot += m_fChangeRot;
		}
		else if (m_nRotPattern == 2)
		{// �����_��
			if (rand() % 2 == 0)
			{// ���v���
				fRot += m_fChangeRot;
			}
			else
			{// �����v���
				fRot -= m_fChangeRot;
			}
		}
		if (fRot > D3DX_PI)
		{// �������~�����𒴂���
			fRot -= D3DX_PI * 2.0f;
		}
		if (fRot > D3DX_PI)
		{// �������~�����𒴂���
			fRot += D3DX_PI * 2.0f;
		}

		// �e��l�̐ݒ�
		SetPos(pos);          // ���W
		SetCol(col);          // �F
		SetRot(fRot);         // ����
		SetLength(fRadius);   // �傫��
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CParticle::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer *pRenderer = CManager::GetRenderer();
	if (pRenderer != NULL)
	{// �����_�����O�N���X���擾�ł���
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			DWORD Lighting;

		    // Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			if (m_bDrawAddtive == true)
			{// ���u�����f�B���O�����Z�����ɐݒ�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// ���C�e�B���O�̐ݒ�
			pDevice->GetRenderState(D3DRS_LIGHTING, &Lighting);
			if (m_bLighting == false)
			{// ���C�e�B���O���O��
				pDevice->SetRenderState(D3DRS_LIGHTING, false);
			}

			// ���ʂ̕`�揈��
			CSceneBillboard::Draw();


			// ���C�e�B���O��߂�
			pDevice->SetRenderState(D3DRS_LIGHTING, Lighting);

			// ���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			// Z�e�X�g�����̒l�ɖ߂�
			pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		}
	}
}

//=============================================================================
//    ���_�o�b�t�@��������
//=============================================================================
void CParticle::MakeVertex(const LPDIRECT3DDEVICE9 pDevice)
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

	// �I�t�Z�b�g�̒��������߂�
	float fLength = sqrtf((GetWidth() * GetWidth()) + (GetHeight() * GetHeight()));
	SetLength(fLength);

	// �I�t�Z�b�g�̊p�x�����߂�
	float fAngle = atan2f(GetWidth(), GetHeight());
	SetAngle(fAngle);

	// ���_�̉�]���l�����č��W���v�Z
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(GetRot(), -sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	YPos[0] = CFunctionLib::RotationVectorY(GetRot(), -sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	XPos[1] = CFunctionLib::RotationVectorX(GetRot(), sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	YPos[1] = CFunctionLib::RotationVectorY(GetRot(), sinf(fAngle) * fLength, cosf(fAngle) * fLength);
	XPos[2] = CFunctionLib::RotationVectorX(GetRot(), -sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	YPos[2] = CFunctionLib::RotationVectorY(GetRot(), -sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	XPos[3] = CFunctionLib::RotationVectorX(GetRot(), sinf(fAngle) * fLength, -cosf(fAngle) * fLength);
	YPos[3] = CFunctionLib::RotationVectorY(GetRot(), sinf(fAngle) * fLength, -cosf(fAngle) * fLength);

	// ���_���W
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// ���_�̐������J��Ԃ�
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f);
	}

	// �@��
	pVtx[0].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[1].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[2].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);
	pVtx[3].nor = D3DXVECTOR3(-0.22f, 0.87f, -0.44f);

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

	// ���_�o�b�t�@�̐ݒ�
	SetVtxBuff(pVtxBuff);
}

//=============================================================================
//    �ړ��ʐݒ菈��
//=============================================================================
void CParticle::SetMove(const D3DXVECTOR3 move)
{
	m_Move = move;
}

//=============================================================================
//    �ړ��ʂ̕ω��ʐݒ菈��
//=============================================================================
void CParticle::SetChangeMove(const D3DXVECTOR3 ChangeMove)
{
	m_ChangeMove = ChangeMove;
}

//=============================================================================
//    �F�̕ω��ʐݒ菈��
//=============================================================================
void CParticle::SetChangeCol(const D3DXCOLOR ChangeCol)
{
	m_ChangeCol = ChangeCol;
}

//=============================================================================
//    �����ݒ菈��
//=============================================================================
void CParticle::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    ��]�p�^�[���ݒ菈��
//=============================================================================
void CParticle::SetRotPattern(const int nRotPattern)
{
	m_nRotPattern = nRotPattern;
}

//=============================================================================
//    �d�͗ʐݒ菈��
//=============================================================================
void CParticle::SetGravity(const float fGravity)
{
	m_fGravity = fGravity;
}

//=============================================================================
//    �傫���̕ω��ʐݒ菈��
//=============================================================================
void CParticle::SetSpread(const float fSpread)
{
	m_fSpread = fSpread;
}

//=============================================================================
//    �����̕ω��ʐݒ菈��
//=============================================================================
void CParticle::SetChangeRot(const float fChangeRot)
{
	m_fChangeRot = fChangeRot;
}

//=============================================================================
//    �o�E���h�ʐݒ菈��
//=============================================================================
void CParticle::SetBouncing(const float fBouncing)
{
	m_fBouncing = fBouncing;
}

//=============================================================================
//    �����蔻�����邩�ݒ菈��
//=============================================================================
void CParticle::SetCollision(const bool bCollision)
{
	m_bCollision = bCollision;
}

//=============================================================================
//    ���Z�����ŕ`�悷�邩���Ȃ����ݒ菈��
//=============================================================================
void CParticle::SetDrawAddtive(const bool bDrawAddtive)
{
	m_bDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    �ړ��ʎ擾����
//=============================================================================
D3DXVECTOR3 CParticle::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    �ړ��ʂ̕ω��ʎ擾����
//=============================================================================
D3DXVECTOR3 CParticle::GetChangeMove(void)
{
	return m_ChangeMove;
}

//=============================================================================
//    �F�̕ω��ʎ擾����
//=============================================================================
D3DXCOLOR CParticle::GetChangeCol(void)
{
	return m_ChangeCol;
}

//=============================================================================
//    �����擾����
//=============================================================================
int CParticle::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    ��]�p�^�[���擾����
//=============================================================================
int CParticle::GetRotPattern(void)
{
	return m_nRotPattern;
}

//=============================================================================
//    �d�͗ʎ擾����
//=============================================================================
float CParticle::GetGravity(void)
{
	return m_fGravity;
}

//=============================================================================
//    �傫���̕ω��ʎ擾����
//=============================================================================
float CParticle::GetSpread(void)
{
	return m_fSpread;
}

//=============================================================================
//    �����̕ω��ʎ擾����
//=============================================================================
float CParticle::GetChangeRot(void)
{
	return m_fChangeRot;
}

//=============================================================================
//    �o�E���h�ʎ擾����
//=============================================================================
float CParticle::GetBouncing(void)
{
	return m_fBouncing;
}

//=============================================================================
//    �����蔻�����邩�擾����
//=============================================================================
bool CParticle::GetCollision(void)
{
	return m_bCollision;
}

//=============================================================================
//    ���Z�����ŕ`�悷�邩���Ȃ����擾����
//=============================================================================
bool CParticle::GetDrawAddtive(void)
{
	return m_bDrawAddtive;
}