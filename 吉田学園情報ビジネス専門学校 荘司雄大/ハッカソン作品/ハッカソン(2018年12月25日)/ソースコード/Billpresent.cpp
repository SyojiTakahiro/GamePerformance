//*****************************************************************************
//
//     �v���[���g�p�r���{�[�h�̏���[Billpresent.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "Billpresent.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "effectManager.h"

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CBillPresent::CBillPresent(int nPriority, OBJTYPE objType) : CSceneBillboard(nPriority, objType)
{
	// �e��l�̃N���A
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CBillPresent::~CBillPresent()
{

}

//=============================================================================
//    ��������
//=============================================================================
CBillPresent *CBillPresent::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, int nPriority)
{
	CBillPresent *pBillPresent = NULL;     // �v���[���g�p�r���{�[�h�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pBillPresent == NULL)
		{// ����������ɂȂ��Ă���
			pBillPresent = new CBillPresent(nPriority);
			if (pBillPresent != NULL)
			{// �C���X�^���X�𐶐��ł���
			    // �e��l�̑��
				pBillPresent->SetPos(pos);         // �|���S���̍��W
				pBillPresent->SetCol(col);         // �|���S���̐F
				pBillPresent->SetRot(fRot);        // �|���S���̌���
				pBillPresent->SetWidth(fWidth);    // �|���S���̕�
				pBillPresent->SetHeight(fHeight);  // �|���S���̍���

				if (FAILED(pBillPresent->Init()))
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

	return pBillPresent;  	// �C���X�^���X�̃A�h���X��Ԃ�
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CBillPresent::Init(void)
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
void CBillPresent::Uninit(void)
{
	// ���ʂ̕`�揈��
	CSceneBillboard::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CBillPresent::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CBillPresent::Draw(void)
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
//    �����蔻�菈��
//=============================================================================
void CBillPresent::Collision(D3DXVECTOR3 pos)
{
	// �K�v�ȕϐ���ǂݍ���
	D3DXVECTOR3 BillPos = GetPos();
	float fWidth = GetWidth();
	float fHeight = GetHeight();

	if (pos.x >= BillPos.x - fWidth && pos.x <= BillPos.x + fWidth)
	{// X����̒��ɂ���
		if (pos.y >= BillPos.y - fHeight && pos.y <= BillPos.y + fHeight)
		{// Y����̒��ɂ���
			if (CManager::GetMode() == CManager::MODE_GAME)
			{
				CManager::GetGame()->CutPresentNum();
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESENT);
				CManager::GetGame()->AddScore();

				CEffectManager *pEffectManager = NULL;
				// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^����肵�Ă���
				if (pEffectManager == NULL)
				{// �|�C���^���m�ۂł����Ԃł���
					CScene *pScene = NULL;
					CScene *pSceneNext = NULL;
					for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
					{// �D�揇�ʂ̐������J��Ԃ�
						pScene = GetTop(nCntPriority);
						while (pScene != NULL)
						{// ����������ɂȂ�܂ŌJ��Ԃ�
							pSceneNext = pScene->GetNext();
							if (pScene->GetObjType() == OBJTYPE_EFFECTMANAGER)
							{// �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^������
								pEffectManager = (CEffectManager*)pScene;
								pEffectManager->SetEffect(GetPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_PRESENT);
							}
							pScene = pSceneNext;
						}
					}
				}

				Uninit();
			}
		}
	}
}