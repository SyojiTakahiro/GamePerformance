//*****************************************************************************
//
//     �G�~�b�^�̏���[emitter.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "effect.h"
#include "manager.h"
#include "system.h"
#include "mode.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define EMITTER_EDIT_EFFECTALPAH  (0.2f)     // �}�b�v�G�f�B�b�g���̃G�t�F�N�g�̓����x
#define EMITTER_MOVEMENT          (6.0f)     // �G�~�b�^�̈ړ���
#define EMITTER_ROTATION          (0.2f)     // �G�~�b�^�̉�]��

//*****************************************************************************
//    CEmitterData�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEmitterData::CEmitterData()
{
	m_nType = 0;                                 // �g�p����G�t�F�N�g�̎��
	m_nTexIdx = 0;                               // �g�p����e�N�X�`���̔ԍ�
	m_nEffectIdx = 0;                            // �g�p����G�t�F�N�g�̔ԍ�
	m_RangeMax = D3DXVECTOR3(0.0f,0.0f,0.0f);    // �G�t�F�N�g�𐶐�����͈͂̍ő�l
	m_RangeMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // �G�t�F�N�g�𐶐�����͈͂̍ŏ��l
	m_nLife = 0;                                 // ����
	m_nLap = 0;                                  // 1��̕��o�ŏo���G�t�F�N�g�̌�
	m_nAppear = 0;                               // �G�t�F�N�g���o���Ԋu
	m_bLoop = false;                             // ���[�v���邩���Ȃ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEmitterData::~CEmitterData()
{

}

//*****************************************************************************
//    CEmitter�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEmitter::CEmitter(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̐ݒ�
	m_pTexture = NULL;                      // �e�N�X�`���ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ����
	D3DXMatrixIdentity(&m_MtxWorld);        // ���[���h�}�g���b�N�X
	m_nType = 0;                            // �g�p����G�t�F�N�g�̎��
	m_nTexIdx = 0;                          // �g�p����e�N�X�`���ԍ�
	m_nEffectIdx = 0;                       // �g�p����G�t�F�N�g�̔ԍ�
	m_nLife = 0;                            // �G�~�b�^�̎���
	m_nLap = 0;                             // 1��̕��o�^�C�~���O�ɏo���G�t�F�N�g�̌�
	m_nAppear = 0;                          // ���o����Ԋu
	m_nCounter = 0;                         // ���o�^�C�~���O���Ǘ�����J�E���^�[
	m_bLoop = false;                        // ���[�v���邩���Ȃ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CEmitter::~CEmitter()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CEmitter::Init(void)
{
	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CEmitter::Uninit(void)
{
	// ���g�̃|�C���^���폜
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CEmitter::Update(void)
{
	if (m_bLoop != true)
	{// ���[�v���Ȃ��ݒ�Ȃ��
		m_nLife--;  // ���������炷
	}

	if (m_nLife <= 0 && m_bLoop == false)
	{// �������s���ă��[�v���Ȃ��ݒ�Ȃ��
		Uninit();
	}
	else
	{// �������܂�����
		m_nCounter++;  // �J�E���^�[�𑝂₷
		if (m_nCounter % m_nAppear == 0)
		{// ���o����^�C�~���O�ɂȂ�����
			for (int nCntParticle = 0; nCntParticle < m_nLap; nCntParticle++)
			{// 1��̕��o�ŏo�������J��Ԃ�
				SetEffect();
			}
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CEmitter::Draw(void)
{

}

//=============================================================================
//    �ړ�����
//=============================================================================
void CEmitter::Movement(void)
{
	// �L�[�{�[�h�̎擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �J�������擾
	CCamera *pCamera = CManager::GetBaseMode()->GetCamera(0);

	if (pKeyboard->GetPress(DIK_A) == true)
	{// �������̓��͂����ꂽ
		if (pKeyboard->GetPress(DIK_W) == true)
		{// �����ɏ�����̓��͂����ꂽ
		    // �ړ�����
			m_Pos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * EMITTER_MOVEMENT;
			m_Pos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * EMITTER_MOVEMENT;
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// �����ɉ������̓��͂����ꂽ
		    // �ړ�����
			m_Pos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * EMITTER_MOVEMENT;
			m_Pos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * EMITTER_MOVEMENT;
		}
		else
		{// ����������ĂȂ�
		    // �ړ�����
			m_Pos.x += sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * EMITTER_MOVEMENT;
			m_Pos.z += cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * EMITTER_MOVEMENT;
		}
	}
	else if (pKeyboard->GetPress(DIK_D) == true)
	{// �E�����̓��͂����ꂽ
		if (pKeyboard->GetPress(DIK_W) == true)
		{// �����ɏ�����̓��͂����ꂽ
		    // �ړ�����
			m_Pos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * EMITTER_MOVEMENT;
			m_Pos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.75f)) * EMITTER_MOVEMENT;
		}
		else if (pKeyboard->GetPress(DIK_S) == true)
		{// �����ɉ������̓��͂����ꂽ
		    // �ړ�����
			m_Pos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * EMITTER_MOVEMENT;
			m_Pos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.25f)) * EMITTER_MOVEMENT;
		}
		else
		{// ����������ĂȂ�
		    // �ړ�����
			m_Pos.x -= sinf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * EMITTER_MOVEMENT;
			m_Pos.z -= cosf(pCamera->GetRot().y - (D3DX_PI * 0.5f)) * EMITTER_MOVEMENT;
		}
	}
	else if (pKeyboard->GetPress(DIK_W) == true)
	{// ������̓��͂����ꂽ
	    // �ړ�����
		m_Pos.x += sinf(pCamera->GetRot().y) * EMITTER_MOVEMENT;
		m_Pos.z += cosf(pCamera->GetRot().y) * EMITTER_MOVEMENT;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{// �������̓��͂����ꂽ
	    // �ړ�����
		m_Pos.x += sinf(pCamera->GetRot().y + D3DX_PI) * EMITTER_MOVEMENT;
		m_Pos.z += cosf(pCamera->GetRot().y + D3DX_PI) * EMITTER_MOVEMENT;
	}

	if (pKeyboard->GetPress(DIK_R) == true)
	{// �c�����̓��͂����ꂽ
	    // �ړ�����
		m_Pos.y += EMITTER_MOVEMENT;
	}
	else if (pKeyboard->GetPress(DIK_V) == true)
	{// �c�����̓��͂����ꂽ
	    // �ړ�����
		m_Pos.y -= EMITTER_MOVEMENT;
	}

	if (pKeyboard->GetPress(DIK_Q) == true)
	{// Q�L�[�̓��͂����ꂽ
	    // ��]����
		m_Rot.y += EMITTER_ROTATION;
		if (m_Rot.y > D3DX_PI)
		{// �~�����𒴂���
			m_Rot.y -= D3DX_PI * 2.0f;
		}
	}
	else if (pKeyboard->GetPress(DIK_E) == true)
	{// E�L�[�̓��͂����ꂽ
	    // ��]����
		m_Rot.y -= EMITTER_ROTATION;
		if (m_Rot.y < -D3DX_PI)
		{// �~�����𒴂���
			m_Rot.y += D3DX_PI * 2.0f;
		}
	}
}

//=============================================================================
//    �e�N�X�`���ւ̃|�C���^�ݒ菈��
//=============================================================================
void CEmitter::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    ���W�ݒ菈��
//=============================================================================
void CEmitter::SetPos(const D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
//    �����ݒ菈��
//=============================================================================
void CEmitter::SetRot(const D3DXVECTOR3 rot)
{
	m_Rot = rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�ݒ菈��
//=============================================================================
void CEmitter::SetMtxWorld(const D3DXMATRIX mtxWorld)
{
	m_MtxWorld = mtxWorld;
}

//=============================================================================
//    �g�p����G�t�F�N�g�̎�ސݒ菈��
//=============================================================================
void CEmitter::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    �g�p����e�N�X�`���ԍ��ݒ菈��
//=============================================================================
void CEmitter::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    ��ސݒ菈��
//=============================================================================
void CEmitter::SetEffectIdx(const int nEffectIdx)
{
	m_nEffectIdx = nEffectIdx;
}

//=============================================================================
//    �G�t�F�N�g�𐶐�����͈͂̍ő�l�ݒ菈��
//=============================================================================
void CEmitter::SetRangeMax(const D3DXVECTOR3 RangeMax)
{
	m_RangeMax = RangeMax;
}

//=============================================================================
//    �G�t�F�N�g�𐶐�����͈͂̍ŏ��l�ݒ菈��
//=============================================================================
void CEmitter::SetRangeMin(const D3DXVECTOR3 RangeMin)
{
	m_RangeMin = RangeMin;
}

//=============================================================================
//    �����ݒ菈��
//=============================================================================
void CEmitter::SetLife(const int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
//    1��̕��o�Ő�������G�t�F�N�g�̌��ݒ菈��
//=============================================================================
void CEmitter::SetLap(const int nLap)
{
	m_nLap = nLap;
}

//=============================================================================
//    ���o����Ԋu�ݒ菈��
//=============================================================================
void CEmitter::SetAppear(const int nAppear)
{
	m_nAppear = nAppear;
}

//=============================================================================
//    ���[�v���邩���Ȃ����ݒ菈��
//=============================================================================
void CEmitter::SetLoop(const bool bLoop)
{
	m_bLoop = bLoop;
}

//=============================================================================
//    �e�N�X�`���ւ̃|�C���^�擾����
//=============================================================================
LPDIRECT3DTEXTURE9 CEmitter::GetTexture(void)
{
	return m_pTexture;
}

//=============================================================================
//    ���W�擾����
//=============================================================================
D3DXVECTOR3 CEmitter::GetPos(void)
{
	return m_Pos;
}

//=============================================================================
//    �����擾����
//=============================================================================
D3DXVECTOR3 CEmitter::GetRot(void)
{
	return m_Rot;
}

//=============================================================================
//    ���[���h�}�g���b�N�X�擾����
//=============================================================================
D3DXMATRIX CEmitter::GetMtxWorld(void)
{
	return m_MtxWorld;
}

//=============================================================================
//    �g�p����G�t�F�N�g�̎�ގ擾����
//=============================================================================
int CEmitter::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    �g�p����e�N�X�`���ԍ��擾����
//=============================================================================
int CEmitter::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    ��ގ擾����
//=============================================================================
int CEmitter::GetEffectIdx(void)
{
	return m_nEffectIdx;
}

//=============================================================================
//    �G�t�F�N�g�𐶐�����͈͂̍ő�l�擾����
//=============================================================================
D3DXVECTOR3 CEmitter::GetRangeMax(void)
{
	return m_RangeMax;
}

//=============================================================================
//    �G�t�F�N�g�𐶐�����͈͂̍ŏ��l�擾����
//=============================================================================
D3DXVECTOR3 CEmitter::GetRangeMin(void)
{
	return m_RangeMin;
}

//=============================================================================
//    �����擾����
//=============================================================================
int CEmitter::GetLife(void)
{
	return m_nLife;
}

//=============================================================================
//    1��̕��o�Ő�������G�t�F�N�g�̌��擾����
//=============================================================================
int CEmitter::GetLap(void)
{
	return m_nLap;
}

//=============================================================================
//    ���o����Ԋu�擾����
//=============================================================================
int CEmitter::GetAppear(void)
{
	return m_nAppear;
}

//=============================================================================
//    ���[�v���邩���Ȃ����擾����
//=============================================================================
bool CEmitter::GetLoop(void)
{
	return m_bLoop;
}

//*****************************************************************************
//    CParEmitter�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CParEmitter::CParEmitter(int nPriority, OBJTYPE objType) : CEmitter(nPriority, objType)
{
	// �e��l�̃N���A
	m_pParData = NULL;  // �p�[�e�B�N���f�[�^�N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CParEmitter::~CParEmitter()
{

}

//=============================================================================
//    ��������
//=============================================================================
CParEmitter *CParEmitter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexIdx, int nEffectIdx, D3DXVECTOR3 RangeMax, D3DXVECTOR3 RangeMin, int nLife, int nLap, int nAppear, bool bLoop, CParData *pParData, LPDIRECT3DTEXTURE9 pTexture, int nPriority)
{
	CParEmitter *pParEmitter = NULL;       // �p�[�e�B�N���G�~�b�^�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pParEmitter == NULL)
		{// ����������ɂȂ��Ă���
			pParEmitter = new CParEmitter(nPriority);
			if (pParEmitter != NULL)
			{// �C���X�^���X�𐶐��ł���
			 // �e��l�̐ݒ�
				pParEmitter->SetParData(pParData);
				pParEmitter->BindTexture(pTexture);
				pParEmitter->SetPos(pos);
				pParEmitter->SetRot(rot);
				pParEmitter->SetType(nType);
				pParEmitter->SetTexIdx(nTexIdx);
				pParEmitter->SetEffectIdx(nEffectIdx);
				pParEmitter->SetRangeMax(RangeMax);
				pParEmitter->SetRangeMin(RangeMin);
				pParEmitter->SetLife(nLife);
				pParEmitter->SetLap(nLap);
				pParEmitter->SetAppear(nAppear);
				pParEmitter->SetLoop(bLoop);

				if (FAILED(pParEmitter->Init()))
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

	return pParEmitter;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CParEmitter::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CEmitter::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CParEmitter::Uninit(void)
{
	// ���ʂ̏I������
	CEmitter::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CParEmitter::Update(void)
{
	// ���ʂ̍X�V����
	CEmitter::Update();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CParEmitter::Draw(void)
{
	// ���ʂ̕`�揈��
	CEmitter::Draw();
}

//=============================================================================
//    �p�[�e�B�N�����Z�b�g���鏈��
//=============================================================================
void CParEmitter::SetEffect(void)
{
	// �����ɕK�v�ȃf�[�^���쐬
	D3DXVECTOR3 pos = GetPos();                               // ���W
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �ړ���
	D3DXVECTOR3 ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �ړ��ʂ̕ω���
	D3DXVECTOR3 Range = D3DXVECTOR3(0.0f, 0.0f, 0.0f);        // ��������͈�
	float fMaxRot = 0.0f;                                     // �������̌���(�ő�l)
	float fMinRot = 0.0f;                                     // �������̌���(�ŏ��l)
	D3DXCOLOR InitCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // ���������̐F
	float fWidth = 0.0f;                                      // ��
	float fHeight = 0.0f;                                     // ����
	float fRot = 0.0f;                                        // ����
	D3DXCOLOR ChangeCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // �F�̕ω���
	float fGravity = 0.0f;                                    // �d��
	float fSpread = 0.0f;                                     // ���a�̕ω���
	float fChangeRot = 0.0f;                                  // �����̕ω���
	float fBouncing = 0.0f;                                   // �o�E���h��
	int nLife = 0;                                            // �̗�
	int nRotPattern = 0;                                      // ��]�̃p�^�[��
	bool bCollision = false;                                  // �����蔻������邩���Ȃ���
	bool bDrawAddtive = false;                                // ���Z�����ŕ`�悷�邩���Ȃ���
	bool bLighting = false;                                   // ���C�e�B���O���邩���Ȃ���

	// �f�[�^���쐬����
	if (m_pParData != NULL)
	{// ���������m�ۂ���Ă���
	    // �f�[�^���������镔���̓f�[�^������������Ă���
		fMaxRot = D3DXToRadian(m_pParData->GetMaxRot());
		fMinRot = D3DXToRadian(m_pParData->GetMinRot());
		Range = m_pParData->GetRange();
		ChangeMove = m_pParData->GetChangeMove();
		InitCol = m_pParData->GetInitCol();
		ChangeCol = m_pParData->GetChangeCol();
		fSpread = m_pParData->GetSpread();
		fGravity = m_pParData->GetGravity();
		fBouncing = m_pParData->GetBouncing();
		fChangeRot = m_pParData->GetRotSpeed();
		nRotPattern = m_pParData->GetRotPattern();
		bCollision = m_pParData->GetCollision();
		bDrawAddtive = m_pParData->GetDrawAddtive();
		bLighting = m_pParData->GetLighting();

		// �����_���ɏo�͂���Ƃ��낾���v�Z����
		float fRandom = 0.0f;
		float fRange = 0.0f;
		float fAngle = 0.0f;

		// ����
		fMaxRot *= 100.0f;
		fMinRot *= 100.0f;
		fRandom = (fMaxRot - fMinRot);
		if (fRandom > 0 && fRandom >= 0.1f)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fMinRot + GetRot().y;
		if (fAngle > D3DX_PI)
		{// �p�x���~�����𒴂��Ă���
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// �p�x���~�����𒴂��Ă���
			fAngle += D3DX_PI * 2.0f;
		}
		fRot = fAngle;

		// ���W�̌v�Z
		float fMag = D3DXToRadian(m_pParData->GetRangeMag());

		// X����
		fRange = (D3DX_PI * 2.0f) - (fMag - (D3DX_PI * 2.0f));
		if (fRange > 0 && fRange >= 0.0001f)
		{
			fRandom = (float)(rand() % ((int)fRange * 100));
			pos.x += sinf(fRandom / 100.0f) * Range.x;
		}

		// Y����
		fRange = (D3DX_PI * 2.0f) - (fMag - (D3DX_PI * 2.0f));
		if (fRange > 0 && fRange >= 0.0001f)
		{
			fRandom = (float)(rand() % ((int)fRange * 100));
			pos.y += sinf(fRandom / 100.0f) * Range.y;
		}

		// Z����
		fRange = (D3DX_PI * 2.0f) - (fMag - (D3DX_PI * 2.0f));
		if (fRange > 0 && fRange >= 0.0001f)
		{
			fRandom = (float)(rand() % ((int)fRange * 100));
			pos.z += cosf(fRandom / 100.0f) * Range.z;
		}

		// �ړ��ʂ̌v�Z
		D3DXVECTOR3 MaxMove = m_pParData->GetMaxMove();
		D3DXVECTOR3 MinMove = m_pParData->GetMinMove();
		D3DXVECTOR3 RangeMax = GetRangeMax();
		D3DXVECTOR3 RangeMin = GetRangeMin();
		D3DXVECTOR3 Range;
		D3DXVECTOR3 Move;
		D3DXMATRIX mtxRot, mtxTrans, mtxWorld;

		// �͈͂̌v�Z
		fRandom = RangeMax.x - RangeMin.x;
		if (fRandom >= 1.0f)
		{
			Range.x = rand() % (int)fRandom + RangeMin.x;
		}
		else
		{
			Range.x = RangeMin.x;
		}
		fRandom = RangeMax.y - RangeMin.y;
		if (fRandom >= 1.0f)
		{
			Range.y = rand() % (int)fRandom + RangeMin.y;
		}
		else
		{
			Range.y = RangeMin.y;
		}
		fRandom = RangeMax.z - RangeMin.z;
		if (fRandom >= 1.0f)
		{
			Range.z = rand() % (int)fRandom + RangeMin.z;
		}
		else
		{
			Range.z = RangeMin.z;
		}
		Range.x = D3DXToRadian(Range.x);
		Range.y = D3DXToRadian(Range.y);
		Range.z = D3DXToRadian(Range.z);
		Range += GetRot();

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Range.y, Range.x, Range.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �ړ��ʂ��v�Z����
		Move = MaxMove - MinMove;
		D3DXVec3TransformCoord(&Move, &Move, &mtxWorld);

		// �傫��
		// ��
		float MaxWidth = m_pParData->GetMaxWidth();
		float MinWidth = m_pParData->GetMinWidth();
		fRange = MaxWidth - MinWidth;
		if (fRange > 0 && fRange >= 1)
		{
			fWidth = rand() % (int)fRange + MinWidth;
		}
		else
		{
			fWidth = MinWidth;
		}

		// ����
		float MaxHeight = m_pParData->GetMaxHeight();
		float MinHeight = m_pParData->GetMinHeight();
		fRange = MaxHeight - MinHeight;
		if (fRange > 0 && fRange >= 1)
		{
			fHeight = rand() % (int)fRange + MinHeight;
		}
		else
		{
			fHeight = MinHeight;
		}

		// ����
		int MaxLife = m_pParData->GetMaxLife();
		int MinLife = m_pParData->GetMinLife();
		fRange = (float)(MaxLife - MinLife);
		if (fRange > 0 && fRange >= 1)
		{
			nLife = rand() % (int)fRange + MinLife;
		}
		else
		{
			nLife = MinLife;
		}

		// �p�[�e�B�N���̐���
		CParticle *pParticle = CParticle::Create(pos, Move, ChangeMove, InitCol, fWidth, fHeight, fRot, ChangeCol, fGravity, fSpread, fChangeRot, fBouncing, nLife, nRotPattern, bCollision, bDrawAddtive, bLighting, EFFECT_PRIORITY);
		if (pParticle != NULL)
		{// �p�[�e�B�N���̐����ɐ�������
			pParticle->BindTexture(GetTexture());
		}
	}
}

//=============================================================================
//    �p�[�e�B�N���f�[�^�ւ̃|�C���^�ݒ菈��
//=============================================================================
void CParEmitter::SetParData(CParData *pParData)
{
	m_pParData = pParData;
}

//=============================================================================
//    �p�[�e�B�N���f�[�^�ւ̃|�C���^�擾����
//=============================================================================
CParData *CParEmitter::GetParData(void)
{
	return m_pParData;
}

//*****************************************************************************
//    CRingEmitter�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRingEmitter::CRingEmitter(int nPriority, OBJTYPE objType) : CEmitter(nPriority, objType)
{
	// �e��l�̃N���A
	m_pRingData = NULL;  // �����O�G�t�F�N�g�f�[�^�N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRingEmitter::~CRingEmitter()
{

}

//=============================================================================
//    ��������
//=============================================================================
CRingEmitter *CRingEmitter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexIdx, int nEffectIdx, D3DXVECTOR3 RangeMax, D3DXVECTOR3 RangeMin, int nLife, int nLap, int nAppear, bool bLoop, CRingData *pRingData, LPDIRECT3DTEXTURE9 pTexture, int nPriority)
{
	CRingEmitter *pRingEmitter = NULL;     // �����O�G�t�F�N�g�G�~�b�^�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pRingEmitter == NULL)
		{// ����������ɂȂ��Ă���
			pRingEmitter = new CRingEmitter(nPriority);
			if (pRingEmitter != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̐ݒ�
				pRingEmitter->SetRingData(pRingData);
				pRingEmitter->BindTexture(pTexture);
				pRingEmitter->SetPos(pos);
				pRingEmitter->SetRot(rot);
				pRingEmitter->SetType(nType);
				pRingEmitter->SetTexIdx(nTexIdx);
				pRingEmitter->SetEffectIdx(nEffectIdx);
				pRingEmitter->SetRangeMax(RangeMax);
				pRingEmitter->SetRangeMin(RangeMin);
				pRingEmitter->SetLife(nLife);
				pRingEmitter->SetLap(nLap);
				pRingEmitter->SetAppear(nAppear);
				pRingEmitter->SetLoop(bLoop);

				if (FAILED(pRingEmitter->Init()))
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

	return pRingEmitter;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRingEmitter::Init(void)
{
	// ���ʂ̏���������
	if (FAILED(CEmitter::Init()))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRingEmitter::Uninit(void)
{
	// ���ʂ̏I������
	CEmitter::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRingEmitter::Update(void)
{
	// ���ʂ̍X�V����
	CEmitter::Update();
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRingEmitter::Draw(void)
{
	// ���ʂ̕`�揈��
	CEmitter::Draw();
}

//=============================================================================
//    �����O�G�t�F�N�g���Z�b�g���鏈��
//=============================================================================
void CRingEmitter::SetEffect(void)
{
	// �����ɕK�v�ȃf�[�^���쐬
	D3DXVECTOR3 pos = GetPos();                               // ���W
	D3DXVECTOR3 Move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // �ړ���
	D3DXVECTOR3 ChangeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   // �ړ��ʂ̕ω���
	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // ����
	D3DXCOLOR InitCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);    // ���������̐F
	float fHeight = 0.0f;                                     // ����
	float fRadius = 0.0f;                                     // ���a
	float fDiffusion = 0.0f;                                  // ��
	float fChangeHeight = 0.0f;                               // �����𖈃t���[���ǂꂭ�炢�ω������邩
	float fChangeRadius = 0.0f;                               // ���a�𖈃t���[���ǂꂭ�炢�ω������邩
	float fChangeDiffusion = 0.0f;                            // ���𖈃t���[���ǂꂭ�炢�ω������邩
	float fRot = 0.0f;                                        // ����
	D3DXCOLOR ChangeCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  // �F�̕ω���
	float fGravity = 0.0f;                                    // �d��
	float fSpread = 0.0f;                                     // ���a�̕ω���
	D3DXVECTOR3 RotSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // �����̕ω���
	float fBouncing = 0.0f;                                   // �o�E���h��
	int nLife = 0;                                            // �̗�
	int nXBlock = 0;                                          // ���̕�����
	int nYBlock = 0;                                          // �c�̕�����
	int nRotPattern = 0;                                      // ��]�̃p�^�[��
	bool bCulling = false;                                    // �J�����O�����邩���Ȃ���
	bool bDrawAddtive = false;                                // ���Z�����ŕ`�悷�邩���Ȃ���

	// �f�[�^���쐬����
	if (m_pRingData != NULL)
	{// ���������m�ۂ���Ă���
	    // �f�[�^���������镔���̓f�[�^������������Ă���
		rot = m_pRingData->GetRot();
		ChangeMove = m_pRingData->GetChangeMove();
		InitCol = m_pRingData->GetInitCol();
		ChangeCol = m_pRingData->GetChangeCol();
		fHeight = m_pRingData->GetHeight();
		fRadius = m_pRingData->GetRadius();
		fDiffusion = m_pRingData->GetDiffusion();
		fChangeHeight = m_pRingData->GetChangeHeight();
		fChangeRadius = m_pRingData->GetChangeRadius();
		fChangeDiffusion = m_pRingData->GetChangeDiffusion();
		RotSpeed = m_pRingData->GetRotSpeed();
		nXBlock = m_pRingData->GetXBlock();
		nYBlock = m_pRingData->GetYBlock();
		nRotPattern = m_pRingData->GetRotPattern();
		bCulling = m_pRingData->GetCulling();
		bDrawAddtive = m_pRingData->GetDrawAddtive();

		// �����_���ɏo�͂���Ƃ��낾���v�Z����
		float fRandom = 0.0f;
		float fRange = 0.0f;
		float fAngle = 0.0f;

		// �ړ��ʂ̌v�Z
		D3DXVECTOR3 MaxMove = m_pRingData->GetMaxMove();
		D3DXVECTOR3 MinMove = m_pRingData->GetMinMove();
		D3DXVECTOR3 RangeMax = GetRangeMax();
		D3DXVECTOR3 RangeMin = GetRangeMin();
		D3DXVECTOR3 Range;
		D3DXVECTOR3 Move;
		D3DXMATRIX mtxRot, mtxTrans, mtxWorld;

		// �������v�Z
		fRandom = RangeMax.x - RangeMin.x;
		if (fRandom >= 1.0f)
		{
			Range.x = rand() % (int)fRandom + RangeMin.x;
		}
		else
		{
			Range.x = RangeMin.x;
		}
		fRandom = RangeMax.y - RangeMin.y;
		if (fRandom >= 1.0f)
		{
			Range.y = rand() % (int)fRandom + RangeMin.y;
		}
		else
		{
			Range.y = RangeMin.y;
		}
		fRandom = RangeMax.z - RangeMin.z;
		if (fRandom >= 1.0f)
		{
			Range.z = rand() % (int)fRandom + RangeMin.z;
		}
		else
		{
			Range.z = RangeMin.z;
		}
		Range.x = D3DXToRadian(Range.x);
		Range.y = D3DXToRadian(Range.y);
		Range.z = D3DXToRadian(Range.z);

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, Range.y, Range.x, Range.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �ړ��ʂ��v�Z����
		Move = MaxMove - MinMove;
		D3DXVec3TransformCoord(&Move, &Move, &mtxWorld);

		// �������v�Z
		rot += Range;

		// ����
		int MaxLife = m_pRingData->GetMaxLife();
		int MinLife = m_pRingData->GetMinLife();
		fRange = (float)(MaxLife - MinLife);
		if (fRange > 0 && fRange >= 1)
		{
			nLife = rand() % (int)fRange + MinLife;
		}
		else
		{
			nLife = MinLife;
		}

		// �����O�G�t�F�N�g�̐���
		CRingEffect *pRingEffect = CRingEffect::Create(pos, rot, InitCol, fHeight, fRadius, fDiffusion, nXBlock, nYBlock, nLife, RotSpeed, nRotPattern, Move, ChangeMove, ChangeCol, fChangeHeight, fChangeRadius, fChangeDiffusion, bCulling, bDrawAddtive, EFFECT_PRIORITY);
		if (pRingEffect != NULL)
		{// �����O�G�t�F�N�g�̐����ɐ�������
			pRingEffect->BindTexture(GetTexture());
		}
	}
}

//=============================================================================
//    �����O�G�t�F�N�g�f�[�^�ւ̃|�C���^�ݒ菈��
//=============================================================================
void CRingEmitter::SetRingData(CRingData *pRingData)
{
	m_pRingData = pRingData;
}

//=============================================================================
//    �����O�G�t�F�N�g�f�[�^�ւ̃|�C���^�擾����
//=============================================================================
CRingData *CRingEmitter::GetRingData(void)
{
	return m_pRingData;
}