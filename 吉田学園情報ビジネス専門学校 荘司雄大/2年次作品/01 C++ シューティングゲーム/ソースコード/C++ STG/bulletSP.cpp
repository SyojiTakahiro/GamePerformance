//=============================================================================
//
// �K�E�e���� [bulletSP.cpp]
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
#include "bulletSP.h"
#include "enemy.h"
#include "boss.h"
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
CRenderer *m_pBulletSPRenderer = NULL;
LPDIRECT3DTEXTURE9 CBulletSP::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBulletSP::CBulletSP() : CScene2D(3)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBulletSP::~CBulletSP()
{

}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CBulletSP::Load(void)
{
	CManager manager;

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, SPBULLET_TEXTURE, &m_pTexture);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CBulletSP::Unload(void)
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
HRESULT CBulletSP::Init(void)
{
	CScene2D::BindTexture(m_pTexture);

	//�e�̏����ݒ�
	CScene2D::Init();

	//�I�u�W�F�N�g�̎�ނ̎擾
	CScene::SetObjType(CScene::OBJTYPE_EXBULLET);

	SetBulletSP(0);

	m_HitCounter = 0;

	m_DamageRecast = 60;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBulletSP::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CBulletSP::Update(void)
{
	//�e�̎����̌���
	m_nBulletLife--;

	//�|�W�V�����̎擾
	D3DXVECTOR3 pos;
	pos = *GetPosition();

	//�G�t�F�N�g�̐���
	CEffect::Create(pos, D3DCOLOR_RGBA(255, 0, 150, 255), 30,100);
	CEffect::Create(pos, D3DCOLOR_RGBA(255, 0, 255, 255), 10,100);

	//�e�̈ړ�
	pos.x += m_move.x;	//�e�̈ʒu���ړ�������
	pos.y += m_move.y;	//�e�̈ʒu���ړ�������

	CManager manager;

	//�L�[�{�[�h�̎擾
	CInputKeyboard *pInput;
	pInput = manager.GetInputKeyboard();

	//�W���C�p�b�h�̎擾
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetInputJoypad();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = manager.GetSound();

		//��ړ�
	if (pInput->GetKeyboardPress(DIK_UP) == true)
	{
		pos.y -= 7;
	}
		//���ړ�
	if (pInput->GetKeyboardPress(DIK_DOWN) == true)
	{
		pos.y += 7;
	}
	if (pInput->GetKeyboardPress(DIK_LEFT) == true)
	{
		pos.x -= 14;
	}
	if (pInput->GetKeyboardPress(DIK_RIGHT) == true)
	{
		pos.x += 5;
	}
	if (pInputJoypad != NULL)
	{
		//�ړ�
		if (pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_24) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_25) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_26) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_27) == true)
		{
			pos.x += sinf(pInputJoypad->GetRightStickAxiz()) * 7;
			pos.y += cosf(pInputJoypad->GetRightStickAxiz()) * 7;
		}
	}
	SetPosition(pos);

	//�A�j���[�V����
	m_nCounterAnim++;
	if ((m_nCounterAnim % 4) == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % 10;
		SetBulletSP(m_nPatternAnim);
	}

	//�e�̏���
	if (m_nBulletLife <= 0 || pos.x > SCREEN_WIDTH)
	{
		pSound->StopSound(CSound::SOUND_LABEL_SE_SPSHOT);
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
					//�t���[������HIT����
					m_HitCounter++;
					if ((m_HitCounter % 10) == 0)
					{
						//�e�̓����蔻��(�G)
						if (objType == CScene::OBJTYPE_ENEMY && m_bulletType == BULLETTYPE_PLAYER)
						{
							CEnemy *pEnemy = (CEnemy*)pScene;
							D3DXVECTOR3 EnemyPos = *pScene->GetPosition();

							if (pos.x > EnemyPos.x - 100
								&& pos.x < EnemyPos.x + 100
								&& pos.y > EnemyPos.y - 100
								&& pos.y < EnemyPos.y + 100)
							{
								//�����A�j���[�V����
								CExplosion::Create(EnemyPos, 50);
								pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT000);
								//HIT�Ώۂ̏I������					
								pEnemy->CutLife(10);

								break;
							}
						}

						//�e�̓����蔻��(�G)
						if (objType == CScene::OBJTYPE_BOSS && m_bulletType == BULLETTYPE_PLAYER)
						{
							CBoss *pBoss = (CBoss*)pScene;
							D3DXVECTOR3 BossPos = *pScene->GetPosition();

							if (pos.x > BossPos.x - 100
								&& pos.x < BossPos.x + 100
								&& pos.y > BossPos.y - 100
								&& pos.y < BossPos.y + 100)
							{
		
								//�����A�j���[�V����
								CExplosion::Create(BossPos, 50);
								//HIT�Ώۂ̏I������					
								pBoss->CutBossLife(10);

								break;
							}
						}
						m_HitCounter = 0;
					}
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
							pScene->Uninit();

							break;
						}
					}
				}
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CBulletSP::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �N���G�C�g
//=============================================================================
CBulletSP *CBulletSP::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size,int life, BULLETTYPE bulletType)
{
	CBulletSP *pBullet = NULL;
	if (pBullet == NULL)
	{
		// ���I�m��
		pBullet = new CBulletSP;
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