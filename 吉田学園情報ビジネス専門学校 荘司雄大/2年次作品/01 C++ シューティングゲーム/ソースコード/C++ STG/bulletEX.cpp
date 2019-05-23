//=============================================================================
//
// �e�ؒf���� [bulletEX.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "bullet.h"
#include "bulletEX.h"
#include "enemy.h"
#include "explosion.h"
#include "effect.h"
#include "score.h"
#include "gauge.h"
#include "gauge2.h"
#include "game.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BULLET_SPEED		(15)
//�ÓI�����o�ϐ�
CRenderer *m_pBulletEXRenderer = NULL;
LPDIRECT3DTEXTURE9 CBulletEX::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBulletEX::CBulletEX() : CScene2D(3)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBulletEX::~CBulletEX()
{

}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CBulletEX::Load(void)
{
	CManager manager;

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, EXBULLET_TEXTURE, &m_pTexture);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CBulletEX::Unload(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBulletEX::Init(void)
{
	CScene2D::BindTexture(m_pTexture);

	//�e�̏����ݒ�
	CScene2D::Init();

	//�I�u�W�F�N�g�̎�ނ̎擾
	CScene::SetObjType(CScene::OBJTYPE_EXBULLET);

	SetBulletEX(0);

	m_DamageRecast = 60;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBulletEX::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CBulletEX::Update(void)
{
	m_nBulletLife--;

	//�|�W�V�����̎擾
	D3DXVECTOR3 pos;
	pos = *GetPosition();

	//�e�̈ړ�
	pos.x += m_move.x;	//�e�̈ʒu���ړ�������
	pos.y += m_move.y;	//�e�̈ʒu���ړ�������

	CManager manager;

	//�L�[�{�[�h�̎擾
	CInputKeyboard *pInput;
	pInput = manager.GetInputKeyboard();
	if (pInput->GetKeyboardPress(DIK_Q) == true)
	{
		//��ړ�
		if (pInput->GetKeyboardPress(DIK_W) == true)
		{
			pos.y -= 7;
		}
		//���ړ�
		if (pInput->GetKeyboardPress(DIK_S) == true)
		{
			pos.y += 7;
		}
	}
	SetPosition(pos);


	//�e�̏���
	if (m_nBulletLife <= 0)
	{
		Uninit();
	}
	else if (pos.x > SCREEN_WIDTH || pos.x < 0 || pos.y > SCREEN_HEIGHT - 120 || pos.y < 0)
	{
		Uninit();
	}
	else
	{
		for (int nCntPriority = 0; nCntPriority < MAX_PRI; nCntPriority++)
		{
			//Scene�̎�ނ��w��
			for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
			{
				CScene2D *pScene;
				pScene = (CScene2D*)CScene::GetScene(nCntPriority, nCntScene);
				if (pScene != NULL)
				{
					CScene::OBJTYPE objType;
					objType = pScene->GetObjType();

					//�e�̓����蔻��(�G�e)
					if (objType == CScene::OBJTYPE_BULLET && m_bulletType == BULLETTYPE_PLAYER)
					{
						D3DXVECTOR3 BulletPos = *pScene->GetPosition();

						if (pos.x > BulletPos.x - 100
							&& pos.x < BulletPos.x + 100
							&& pos.y > BulletPos.y - 100
							&& pos.y < BulletPos.y + 100)
						{

							//HIT���̏���
							CExplosion::Create(BulletPos, 50);
							CGauge2::m_fGauge2 += 8;
							pScene->Uninit();

							break;
						}
					}
				}
			}
		}
	}

	//�A�j���[�V����
	m_nCounterAnim++;
	if ((m_nCounterAnim % 4) == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % 5;
		SetBulletEX(m_nPatternAnim);
		//�p�^�[���I�����ɏI��
		if (m_nPatternAnim >= 5 - 1)
		{
			Uninit();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBulletEX::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �N���G�C�g
//=============================================================================
CBulletEX *CBulletEX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size, int life, BULLETTYPE bulletType)
{
	CBulletEX *pBullet = NULL;
	if (pBullet == NULL)
	{
		// ���I�m��
		pBullet = new CBulletEX;
		if (pBullet != NULL)
		{
			pBullet->m_posPolygon = pos;
			pBullet->m_move = move;
			pBullet->SetSize(size);
			pBullet->m_nBulletLife = life;
			pBullet->m_bulletType = bulletType;

			// ����������
			pBullet->Init();
		}
	}
	return pBullet;
}