//*****************************************************************************
//
//     �G�~�b�^�̏���[emitter.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "emitter.h"
#include "manager.h"
#include "camera.h"
#include "particle.h"
#include "effectManager.h"
#include "input.h"

#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************

//*****************************************************************************
//    CEmitterData�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CEmitterData::CEmitterData()
{
	m_nTexIdx = 0;     // �g�p����e�N�X�`���̔ԍ�
	m_nType = 0;       // �g�p����G�t�F�N�g�̔ԍ�
	m_nLife = 0;       // ����
	m_nLap = 0;        // 1��̕��o�ŏo���G�t�F�N�g�̌�
	m_nAppear = 0;     // �G�t�F�N�g���o���Ԋu
	m_bLoop = false;   // ���[�v���邩���Ȃ���
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
	m_pParData = NULL;                      // �p�[�e�B�N���f�[�^�ւ̃|�C���^
	m_pTexture = NULL;                      // �e�N�X�`���ւ̃|�C���^
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���W
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ����
	D3DXMatrixIdentity(&m_MtxWorld);        // ���[���h�}�g���b�N�X
	m_nTexIdx = 0;                          // �g�p����e�N�X�`���ԍ�
	m_nType = 0;                            // ���
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
//    ��������
//=============================================================================
CEmitter *CEmitter::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTexIdx, int nType, float fRangeMax, float fRangeMin, int nLife, int nLap, int nAppear, bool bLoop, CParData *pParData, LPDIRECT3DTEXTURE9 pTexture, int nPriority)
{
	CEmitter *pEmitter = NULL;             // �G�~�b�^�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pEmitter == NULL)
		{// ����������ɂȂ��Ă���
			pEmitter = new CEmitter(nPriority);
			if (pEmitter != NULL)
			{// �C���X�^���X�𐶐��ł���
				// �e��l�̐ݒ�
				pEmitter->SetParData(pParData);
				pEmitter->BindTexture(pTexture);
				pEmitter->SetPos(pos);
				pEmitter->SetRot(rot);
				pEmitter->SetTexIdx(nTexIdx);
				pEmitter->SetType(nType);
				pEmitter->SetRangeMax(fRangeMax);
				pEmitter->SetRangeMin(fRangeMin);
				pEmitter->SetLife(nLife);
				pEmitter->SetLap(nLap);
				pEmitter->SetAppear(nAppear);
				pEmitter->SetLoop(bLoop);

				if (FAILED(pEmitter->Init()))
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

	return pEmitter;  	// �C���X�^���X�̃A�h���X��Ԃ�
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
				SetParticle();
			}
		}

		// �ړ�����
		//Movement();
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
	CCamera *pCamera = CManager::GetCamera();

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
//    �p�[�e�B�N�����Z�b�g���鏈��
//=============================================================================
void CEmitter::SetParticle(void)
{
	// �����ɕK�v�ȃf�[�^���쐬
	D3DXVECTOR3 pos = m_Pos;                                  // ���W
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
		fAngle += fMinRot + m_Rot.y;
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

		// X����
		// �p�x�̎Z�o
		float fAngleMax = D3DXToRadian(m_fRangeMax);
		float fAngleMin = D3DXToRadian(m_fRangeMin);
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 0.1f)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + m_Rot.y;
		if (fAngle > D3DX_PI)
		{// �p�x���~�����𒴂��Ă���
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// �p�x���~�����𒴂��Ă���
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.x - MinMove.x);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.x;
			Move.x = sinf(fAngle) * fRandom;
		}
		else
		{
			Move.x = MinMove.x;
		}


		// Y����
		// �p�x�̎Z�o
		fAngleMax = D3DXToRadian(m_fRangeMax);
		fAngleMin = D3DXToRadian(m_fRangeMin);
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 1)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + m_Rot.y;
		if (fAngle > D3DX_PI)
		{// �p�x���~�����𒴂��Ă���
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// �p�x���~�����𒴂��Ă���
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.y - MinMove.y);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.y;
			Move.y = sinf(fAngle) * fRandom;
		}
		else
		{
			Move.y = MinMove.y;
		}

		// Z����
		// �p�x�̎Z�o
		fAngleMax = D3DXToRadian(m_fRangeMax);
		fAngleMin = D3DXToRadian(m_fRangeMin);
		fAngleMax *= 100.0f;
		fAngleMin *= 100.0f;
		fRandom = (fAngleMax - fAngleMin);
		if (fRandom > 0 && fRandom >= 1)
		{
			fAngle = (rand() % (int)fRandom) * 1.0f;
		}
		fAngle /= 100.0f;
		fAngle += fAngleMin + m_Rot.y;
		if (fAngle > D3DX_PI)
		{// �p�x���~�����𒴂��Ă���
			fAngle -= D3DX_PI * 2.0f;
		}
		if (fAngle < -D3DX_PI)
		{// �p�x���~�����𒴂��Ă���
			fAngle += D3DX_PI * 2.0f;
		}
		fRange = (MaxMove.z - MinMove.z);
		if (fRange > 0 && fRange >= 1)
		{
			fRandom = rand() % (int)fRange + MinMove.z;
			Move.z = cosf(fAngle) * fRandom;
		}
		else
		{
			Move.z = MinMove.z;
		}

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
		CParticle *pParticle = CParticle::Create(pos, Move, ChangeMove, InitCol, fWidth, fHeight, fRot, ChangeCol, fGravity, fSpread, fChangeRot, fBouncing, nLife, nRotPattern, bCollision, bDrawAddtive, bLighting);
		if (pParticle != NULL)
		{// �p�[�e�B�N���̐����ɐ�������
			pParticle->BindTexture(m_pTexture);
		}
	}
}

//=============================================================================
//    �p�[�e�B�N���f�[�^�ւ̃|�C���^�ݒ菈��
//=============================================================================
void CEmitter::SetParData(CParData *pParData)
{
	m_pParData = pParData;
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
//    �g�p����e�N�X�`���ԍ��ݒ菈��
//=============================================================================
void CEmitter::SetTexIdx(const int nTexIdx)
{
	m_nTexIdx = nTexIdx;
}

//=============================================================================
//    ��ސݒ菈��
//=============================================================================
void CEmitter::SetType(const int nType)
{
	m_nType = nType;
}

//=============================================================================
//    �G�t�F�N�g�𐶐�����͈͂̍ő�l�ݒ菈��
//=============================================================================
void CEmitter::SetRangeMax(const float fRangeMax)
{
	m_fRangeMax = fRangeMax;
}

//=============================================================================
//    �G�t�F�N�g�𐶐�����͈͂̍ŏ��l�ݒ菈��
//=============================================================================
void CEmitter::SetRangeMin(const float fRangeMin)
{
	m_fRangeMin = fRangeMin;
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
//    �g�p����e�N�X�`���ԍ��擾����
//=============================================================================
int CEmitter::GetTexIdx(void)
{
	return m_nTexIdx;
}

//=============================================================================
//    ��ގ擾����
//=============================================================================
int CEmitter::GetType(void)
{
	return m_nType;
}

//=============================================================================
//    �G�t�F�N�g�𐶐�����͈͂̍ő�l�擾����
//=============================================================================
float CEmitter::GetRangeMax(void)
{
	return m_fRangeMax;
}

//=============================================================================
//    �G�t�F�N�g�𐶐�����͈͂̍ŏ��l�擾����
//=============================================================================
float CEmitter::GetRangeMin(void)
{
	return m_fRangeMin;
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