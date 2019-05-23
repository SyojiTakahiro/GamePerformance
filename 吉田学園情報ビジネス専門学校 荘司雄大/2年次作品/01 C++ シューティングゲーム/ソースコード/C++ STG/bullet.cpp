//=============================================================================
//
// �e���� [bullet.cpp]
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
#include "enemy.h"
#include "boss.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
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
CRenderer *m_pBulletRenderer = NULL;
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

bool g_bBulletDelete;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBullet::CBullet() : CScene2D(3)
{
	
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CBullet::Load(void)
{
	CManager manager;

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURE, &m_pTexture);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CBullet::Unload(void)
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
HRESULT CBullet::Init(void)
{
	CScene2D::BindTexture(m_pTexture);

	//�e�̏����ݒ�
	CScene2D::Init();

	//�I�u�W�F�N�g�̎�ނ̎擾
	CScene::SetObjType(CScene::OBJTYPE_BULLET);

	m_nPlayerLife = 470;
	m_DamageRecast = 60;

	CScene2D::SetColor(m_col);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBullet::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CBullet::Update(void)
{
	m_nBulletLife--;

	CManager manager;

	//�|�W�V�����̎擾
	D3DXVECTOR3 pos;
	pos = *GetPosition();

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = manager.GetSound();

	//�e�̈ړ�
	pos.x += m_move.x;	//�e�̈ʒu���ړ�������
	pos.y += m_move.y;	//�e�̈ʒu���ړ�������

	SetPosition(pos);

	CEffect::Create(pos, D3DCOLOR_RGBA(255, 255, 255, 255), 10, 6);

	//�e�̏���
	if (m_nBulletLife <= 0)
	{
		Uninit();
		CExplosion::Create(pos, 50);
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

					//�e�̓����蔻��(�G)

					if (objType == CScene::OBJTYPE_ENEMY && m_bulletType == BULLETTYPE_PLAYER)
					{
						CEnemy *pEnemy = (CEnemy*)pScene;
						D3DXVECTOR3 EnemyPos = *pScene->GetPosition();

						if (pos.x > EnemyPos.x - 50
							&& pos.x < EnemyPos.x + 50
							&& pos.y > EnemyPos.y - 50
							&& pos.y < EnemyPos.y + 50)
						{
							pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT000);
							for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
							{
								CParticle::Create(EnemyPos,
									D3DXVECTOR3(sinf(rand() % 628 / 100.0f) * (rand() % 5 + 1), cosf(rand() % 628 / 100.0f) * (rand() % 5 + 1), 0.0f),
									D3DCOLOR_RGBA(255, 0, 0, 255),
									10,
									30);
							}
								
							Uninit();

							pEnemy->CutLife(1);

							break;
						}
					}

					//�e�̓����蔻��(�G)

					if (objType == CScene::OBJTYPE_BOSS && m_bulletType == BULLETTYPE_PLAYER)
					{
						CBoss *pBoss = (CBoss*)pScene;
						D3DXVECTOR3 BossPos = *pScene->GetPosition();

						if (pos.x > BossPos.x - 200
							&& pos.x < BossPos.x + 200
							&& pos.y > BossPos.y - 200
							&& pos.y < BossPos.y + 200)
						{
							pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT000);
							for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
							{
								CParticle::Create(BossPos,
									D3DXVECTOR3(sinf(rand() % 628 / 100.0f) * (rand() % 5 + 1), cosf(rand() % 628 / 100.0f) * (rand() % 5 + 1), 0.0f),
									D3DCOLOR_RGBA(255, 0, 0, 255),
									10,
									30);
							}
							
							Uninit();

							pBoss->CutBossLife(1);

							break;
						}
					}

					//�e�̓����蔻��(���@)
					if (objType == CScene::OBJTYPE_PLAYER && m_bulletType == BULLETTYPE_ENEMY)
					{
						D3DXVECTOR3 PlayerPos = *pScene->GetPosition();

						if (pos.x > PlayerPos.x - 20
							&& pos.x < PlayerPos.x + 20
							&& pos.y > PlayerPos.y - 20
							&& pos.y < PlayerPos.y + 20)
						{
							//�_���[�W
							pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT000);
							CExplosion::Create(PlayerPos, 50);
							Uninit();

							m_nPlayerLife -= 30;

							CGauge *pGauge;
							pGauge = CGame::GetGauge();

							pGauge->CutLife(30.0f);

							if (CGauge::m_fGauge <= 0)
							{
								pScene->Uninit();
								CExplosion::Create(PlayerPos, 50);
							}
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
void CBullet::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �N���G�C�g
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,float size,int life, BULLETTYPE bulletType)
{
		CBullet *pBullet = NULL;
		if (pBullet == NULL)
		{
			// ���I�m��
			pBullet = new CBullet;
			if (pBullet != NULL)
			{
				pBullet->m_posPolygon = pos;
				pBullet->m_move = move;
				pBullet->m_col = col;
				pBullet->SetSize(size);
				pBullet->m_nBulletLife = life;
				pBullet->m_bulletType = bulletType;

				// ����������
				pBullet->Init();
			}
		}
		return pBullet;
}