//*****************************************************************************
//
//     �����O�G�t�F�N�g�̏���[ringEffect.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "effect.h"
#include "manager.h"
#include "system.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************


//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//*****************************************************************************
//    CRingData�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRingData::CRingData()
{
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);           // ����
	m_MaxMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �ړ��ʂ̍ő�l
	m_MinMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);       // �ړ��ʂ̍ŏ��l
	m_ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // �ړ��ʂ̕ω���
	m_InitCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   // �������̐F
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // �t���[�����̐F�̕ω���
	m_nMaxLife = 0;                                  // �����̍ő�l
	m_nMinLife = 0;                                  // �����̍ŏ��l
	m_fHeight = 0.0f;                                // ����
	m_fRadius = 0.0f;                                // ���a
	m_fDiffusion = 0.0f;                             // ��
	m_fChangeHeight = 0.0f;                          // �����𖈃t���[���ǂꂭ�炢�ω������邩
	m_fChangeRadius = 0.0f;                          // ���a���ǂꂭ�炢���t���[���ω������邩
	m_fChangeDiffusion = 0.0f;                       // �����O�̕����ǂꂭ�炢���t���[���ω������邩
	m_nMaxLife = 0;                                  // �����̍ő�l
	m_nMinLife = 0;                                  // �����̍ŏ��l
	m_nXBlock = 0;                                   // ���̕�����
	m_nYBlock = 0;                                   // �c�̕�����
	m_nRotPattern = 0;                               // ��]�̎��[ 0:���v��� 1:�����v��� 2:�����_�� ]
	m_RotSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // ��]����X�s�[�h
	m_bCulling = false;                              // �J�����O���邩�ǂ���[ 0:�Ȃ� 1:���� ]
	m_bDrawAddtive = false;                          // ���Z�������邩���Ȃ���[ 0:�Ȃ� 1:���� ]
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRingData::~CRingData()
{

}

//*****************************************************************************
//    CRingEffect�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRingEffect::CRingEffect(int nPriority, OBJTYPE objType) : CMeshRing(nPriority, objType)
{
	// �e��l�̃N���A
	m_nLife = 0;                                      // ����
	m_RotSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // ��]��
	m_nRotPattern = 0;                                // ��]�̃p�^�[��
	m_Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);           // �ړ���
	m_ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // �ړ��ʂ𖈃t���[���ǂꂭ�炢�ω������邩
	m_ChangeCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // �F�𖈃t���[���ǂꂭ�炢�ω������邩
	m_fChangeHeight = 0.0f;                           // �����𖈃t���[���ǂꂭ�炢�ω������邩
	m_fChangeRadius = 0.0f;                           // �傫�����ǂꂭ�炢���t���[���ω������邩
	m_fChangeDiffusion = 0.0f;                        // �����O�̕����ǂꂭ�炢���t���[���ω������邩
	m_bCulling = false;                               // �J�����O���邩�ǂ���
	m_bDrawAddtive = false;                           // ���Z�����ŕ`�悷�邩���Ȃ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRingEffect::~CRingEffect()
{

}

//=============================================================================
//    ��������
//=============================================================================
CRingEffect *CRingEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock,
	int nLife, D3DXVECTOR3 RotSpeed, int nRotPattern, D3DXVECTOR3 Move, D3DXVECTOR3 ChangeMove, D3DXCOLOR ChangeCol, float fChangeHeight,
	float fChangeRadius, float fChangeDiffusion, bool bCulling, bool bDrawAddtive, int nPriority)
{
	CRingEffect *pRingEffect = NULL;       // �����O�G�t�F�N�g�̌^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pRingEffect == NULL)
		{// ����������ɂȂ��Ă���
			pRingEffect = new CRingEffect(nPriority);
			if (pRingEffect != NULL)
			{// �C���X�^���X�𐶐��ł���
				if (FAILED(pRingEffect->Init(pos, rot, col, fHeight, fRadius, fDiffusion, nXBlock, nYBlock, nLife, RotSpeed, nRotPattern, Move, ChangeMove, ChangeCol, fChangeHeight, fChangeRadius, fChangeDiffusion, bCulling, bDrawAddtive)))
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

	return pRingEffect;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRingEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock,
	int nLife, D3DXVECTOR3 RotSpeed, int nRotPattern, D3DXVECTOR3 Move, D3DXVECTOR3 ChangeMove, D3DXCOLOR ChangeCol, float fChangeHeight,
	float fChangeRadius, float fChangeDiffusion, bool bCulling, bool bDrawAddtive)
{
	// �e��l�̐ݒ�
	SetPos(pos);                           // �����O�G�t�F�N�g�̍��W
	SetRot(rot);                           // �����O�G�t�F�N�g�̌���
	SetCol(col);                           // �����O�G�t�F�N�g�̐F
	SetHeight(fHeight);                    // �����O�G�t�F�N�g�̍���
	SetRadius(fRadius);                    // �����O�G�t�F�N�g�̔��a
	SetDiffusion(fDiffusion);              // �����O�G�t�F�N�g�̍���
	SetXBlock(nXBlock);                    // ���̕�����
	SetYBlock(nYBlock);                    // �c�̕�����
	SetLife(nLife);                        // ����
	SetRotSpeed(RotSpeed);                 // ��]��
	SetRotPattern(nRotPattern);            // ��]�̎��
	SetMove(Move);                         // �ړ���
	SetChangeMove(ChangeMove);             // �ړ��ʂ𖈃t���[���ǂꂭ�炢�ω������邩
	SetChangeCol(ChangeCol);               // �F�𖈃t���[���ǂꂭ�炢�ω������邩
	SetChangeHeight(fChangeHeight);        // �����𖈃t���[���ǂꂭ�炢�ω������邩
	SetChangeRadius(fChangeRadius);        // ���a�𖈃t���[���ǂꂭ�炢�ω������邩
	SetChangeDiffusion(fChangeDiffusion);  // ���𖈃t���[���ǂꂭ�炢�ω������邩
	SetCulling(bCulling);                  // �J�����O���邩���Ȃ���
	SetDrawAddtive(bDrawAddtive);          // ���Z�����ŕ`�悷�邩���Ȃ���

										   // ���ʂ̏���������
	if (FAILED(CMeshRing::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRingEffect::Uninit(void)
{
	// ���ʂ̏I������
	CMeshRing::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRingEffect::Update(void)
{
	// �e��l�̎擾
	D3DXVECTOR3 pos = GetPos();        // ���W
	D3DXVECTOR3 rot = GetRot();        // ����
	D3DXCOLOR col = GetCol();          // �F
	float fHeight = GetHeight();       // ����
	float fRadius = GetRadius();       // ���a
	float fDiffusion = GetDiffusion(); // ��

									   // ���������炷
	m_nLife--;

	// �ړ��ʂ����Z����
	pos += m_Move;

	// �ړ��ʂ�ω�������
	m_Move += m_ChangeMove;

	// �F��ω�������
	col += m_ChangeCol;

	// ������ω�������
	fHeight += m_fChangeHeight;

	// ���a��ω�������
	fRadius += m_fChangeRadius;

	// ����ω�������
	fDiffusion += m_fChangeDiffusion;

	if (m_nLife <= 0 || col.a <= 0.0f)
	{// �������Ȃ��Ȃ���
		Uninit();
	}
	else
	{// �������܂�����
	 // ������ω�������
		if (m_nRotPattern == 0)
		{// 0�̃p�^�[��(���v���)�Ȃ�
			rot -= m_RotSpeed;
		}
		else if (m_nRotPattern == 1)
		{// 1�̃p�^�[��(�����v���)�Ȃ�
			rot += m_RotSpeed;
		}
		else if (m_nRotPattern == 2)
		{// 2�̃p�^�[��(�����_��)�Ȃ�
			int nRandom = rand() % 2;
			if (nRandom == 0)
			{
				rot += m_RotSpeed;
			}
			else
			{
				rot -= m_RotSpeed;
			}
		}

		// �����̕␳
		// X��
		if (rot.x > D3DX_PI)
		{
			rot.x -= D3DX_PI * 2.0f;
		}
		if (rot.x < -D3DX_PI)
		{
			rot.x += D3DX_PI * 2.0f;
		}
		// Y��
		if (rot.y > D3DX_PI)
		{
			rot.y -= D3DX_PI * 2.0f;
		}
		if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2.0f;
		}
		// Z��
		if (rot.z > D3DX_PI)
		{
			rot.z -= D3DX_PI * 2.0f;
		}
		if (rot.z < -D3DX_PI)
		{
			rot.z += D3DX_PI * 2.0f;
		}


		// �e��l�̐ݒ�
		SetPos(pos);              // ���W
		SetRot(rot);              // ����
		SetCol(col);              // �F
		SetHeight(fHeight);       // ����
		SetRadius(fRadius);       // ���a
		SetDiffusion(fDiffusion); // ��

								  // ���_��������������
		ChangeVertex();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRingEffect::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	 // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			DWORD Culling;   // �J�����O�̐ݒ�

			if (m_bCulling == false)
			{// �J�����O���Ȃ��ݒ�Ȃ��
				pDevice->GetRenderState(D3DRS_CULLMODE, &Culling);
				pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
			if (m_bDrawAddtive == true)
			{// ���Z�����ŕ`�悷�邩�Ȃ��
			 // ���u�����f�B���O�����Z�����ɐݒ�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			// Z�e�X�g�𖳌��ɂ���
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);      // �[�x�o�b�t�@�̏������ݐݒ�𖳌���
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);  // �ȉ��̂��̂�`�悷��

																	 // ���ʂ̕`�揈��
			CMeshRing::Draw();

			if (m_bCulling == false)
			{// �J�����O���Ȃ��ݒ�Ȃ��
				pDevice->SetRenderState(D3DRS_CULLMODE, Culling);
			}
			if (m_bDrawAddtive == true)
			{// ���Z�����ŕ`�悷�邩�Ȃ��
			 // ���u�����f�B���O�����ɖ߂�
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}

			// Z�e�X�g��L���ɂ���(�f�t�H���g�̒l��)
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);      // �[�x�o�b�t�@�ւ̏������݂�L����
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL); // �ȉ��̂��̂�`�悷��
		}
	}
}

//=============================================================================
//    ���_��񏑂������̏���
//=============================================================================
void CRingEffect::ChangeVertex(void)
{
	// ���_�o�b�t�@�̎擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();

	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
	 // ���_���̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		float fAngle = 0;                    // �p�x
		float fRadius = 0;                   // ���a
		D3DXVECTOR3 pos;                     // ���W
		D3DXCOLOR col = GetCol();            // �F
		int nXBlock = GetXBlock();           // ���̕�����
		int nYBlock = GetYBlock();           // ���̕�����
		float fHeight = GetHeight();         // ����
		float fDiffusion = GetDiffusion();   // �g�U�l
		float Utex;                          // �e�N�X�`��U���W
		float Vtex;                          // �e�N�X�`��V���W

		fAngle = 0.0f;                          // �p�x��0��
		fRadius = GetRadius();                  // ���a��ݒ�
		pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // ���W��ݒ�
		Utex = 0.0f;                            // �e�N�X�`��U���W��ݒ�
		Vtex = 0.0f;                            // �e�N�X�`��V���W��ݒ�

		for (int nCntV = 0; nCntV < nYBlock + 1; nCntV++)
		{// ���������̕����������J��Ԃ�
			for (int nCntH = 0; nCntH < nXBlock + 1; nCntH++)
			{// ���������̕����������J��Ԃ�
				pos.x = sinf(fAngle) * fRadius;
				pos.z = cosf(fAngle) * fRadius;

				// ���_���W
				pVtx[nCntH].pos = pos;

				// ���_�J���[
				pVtx[nCntH].col = col;

				// �e�N�X�`�����W
				pVtx[nCntH].tex = D3DXVECTOR2(Utex, Vtex);

				fAngle -= D3DX_PI / (nXBlock / 2.0f);  // �p�x��i�߂�
				Utex += 1.0f / (nXBlock / 2.0f);       // �e�N�X�`��U���W��i�߂�

				if (fAngle <= -D3DX_PI)
				{// �p�x���~�����𒴂���
					fAngle += D3DX_PI * 2.0f;
				}
			}
			col.a -= 0.2f;             // �����x��������
			pos.y += fHeight;          // ���W����ɂ�����
			fRadius += fDiffusion;     // ���a���L����
			fAngle = 0.0f;             // �p�x��0�ɖ߂�
			Utex = 0.0f;               // �e�N�X�`��U���W��߂�
			Vtex += 1.0f / nYBlock;    // �e�N�X�`��V���W��i�߂�
			pVtx += nXBlock + 1;       // ���_�o�b�t�@�̔ԍ������̕��������i�߂�
		}

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �����ݒ菈��
//=============================================================================
void CRingEffect::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    ��]�ʐݒ菈��
//=============================================================================
void CRingEffect::SetRotSpeed(const D3DXVECTOR3 RotSpeed)
{
	m_RotSpeed = RotSpeed;
}

//=============================================================================
//    ��]�̎�ސݒ菈��
//=============================================================================
void CRingEffect::SetRotPattern(const int nRotPattern)
{
	m_nRotPattern = nRotPattern;
}

//=============================================================================
//    �ړ��ʐݒ菈��
//=============================================================================
void CRingEffect::SetMove(const D3DXVECTOR3 Move)
{
	m_Move = Move;
}

//=============================================================================
//    �ړ��ʂ𖈃t���[���ǂꂭ�炢�ω������邩�ݒ菈��
//=============================================================================
void CRingEffect::SetChangeMove(const D3DXVECTOR3 ChangeMove)
{
	m_ChangeMove = ChangeMove;
}

//=============================================================================
//    �F�𖈃t���[���ǂꂭ�炢�ω������邩�ݒ菈��
//=============================================================================
void CRingEffect::SetChangeCol(const D3DXCOLOR ChangeCol)
{
	m_ChangeCol = ChangeCol;
}

//=============================================================================
//    �����𖈃t���[���ǂꂭ�炢�ω������邩�ݒ菈��
//=============================================================================
void CRingEffect::SetChangeHeight(const float fChangeHeight)
{
	m_fChangeHeight = fChangeHeight;
}

//=============================================================================
//    �傫���𖈃t���[���ǂꂭ�炢�ω������邩�ݒ菈��
//=============================================================================
void CRingEffect::SetChangeRadius(const float fChangeRadius)
{
	m_fChangeRadius = fChangeRadius;
}

//=============================================================================
//    ���𖈃t���[���ǂꂭ�炢�ω������邩�ݒ菈��
//=============================================================================
void CRingEffect::SetChangeDiffusion(const float fChangeDiffusion)
{
	m_fChangeDiffusion = fChangeDiffusion;
}

//=============================================================================
//    �J�����O���邩�ǂ����ݒ菈��
//=============================================================================
void CRingEffect::SetCulling(const bool bCulling)
{
	m_bCulling = bCulling;
}

//=============================================================================
//    ���Z�����ŕ`�悷�邩���Ȃ����ݒ菈��
//=============================================================================
void CRingEffect::SetDrawAddtive(const bool bDrawAddtive)
{
	m_bDrawAddtive = bDrawAddtive;
}

//=============================================================================
//    �����擾����
//=============================================================================
int CRingEffect::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    ��]�ʎ擾����
//=============================================================================
D3DXVECTOR3 CRingEffect::GetRotSpeed(void)
{
	return m_RotSpeed;
}

//=============================================================================
//    ��]�̎�ގ擾����
//=============================================================================
int CRingEffect::GetRotPattern(void)
{
	return m_nRotPattern;
}

//=============================================================================
//    �ړ��ʎ擾����
//=============================================================================
D3DXVECTOR3 CRingEffect::GetMove(void)
{
	return m_Move;
}

//=============================================================================
//    �ړ��ʂ𖈃t���[���ǂꂭ�炢�ω������邩�擾����
//=============================================================================
D3DXVECTOR3 CRingEffect::GetChangeMove(void)
{
	return m_ChangeMove;
}

//=============================================================================
//    �F�𖈃t���[���ǂꂭ�炢�ω������邩�擾����
//=============================================================================
D3DXCOLOR CRingEffect::GetChangeCol(void)
{
	return m_ChangeCol;
}

//=============================================================================
//    �����𖈃t���[���ǂꂭ�炢�ω������邩�擾����
//=============================================================================
float CRingEffect::GetChangeHeight(void)
{
	return m_fChangeHeight;
}

//=============================================================================
//    �傫���𖈃t���[���ǂꂭ�炢�ω������邩�擾����
//=============================================================================
float CRingEffect::GetChangeRadius(void)
{
	return m_fChangeRadius;
}

//=============================================================================
//    ���𖈃t���[���ǂꂭ�炢�ω������邩�擾����
//=============================================================================
float CRingEffect::GetChangeDiffusion(void)
{
	return m_fChangeDiffusion;
}

//=============================================================================
//    �J�����O���邩�ǂ����擾����
//=============================================================================
bool CRingEffect::GetCulling(void)
{
	return m_bCulling;
}

//=============================================================================
//    ���Z�����ŕ`�悷�邩���Ȃ����擾����
//=============================================================================
bool CRingEffect::GetDrawAddtive(void)
{
	return m_bDrawAddtive;
}