//=============================================================================
//
// �G���� [enemy.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "score.h"
#include "effect.h"
#include "gauge2.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************


//�G
#define ENEMY_TEXTURE1		"data/TEXTURE/enemy001.png"
#define ENEMY_TEXTURE2		"data/TEXTURE/enemy002.png"
#define ENEMY_TEXTURE3		"data/TEXTURE/enemy003.png"
#define ENEMY_TEXTURE4		"data/TEXTURE/enemy004.png"
#define ENEMY_TEXTURE5		"data/TEXTURE/enemy005.png"
#define ENEMY_TEXTURE6		"data/TEXTURE/enemy006.png"
#define ENEMY_TEXTURE_TUTORIAL	"data/TEXTURE/kakashi.png"

//�ÓI�����o�ϐ�
CRenderer *m_pEnemyRenderer = NULL;
LPDIRECT3DTEXTURE9 CEnemy::m_apTexture[MAX_ENEMY] = {};

bool g_bEnemyDelete;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEnemy::CEnemy() : CScene2D(2)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CEnemy::Load(void)
{
	CManager manager;

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE1, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE2, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE3, &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE4, &m_apTexture[3]);
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE5, &m_apTexture[4]);
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE6, &m_apTexture[5]);
	D3DXCreateTextureFromFile(pDevice, ENEMY_TEXTURE_TUTORIAL, &m_apTexture[6]);



	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CEnemy::Unload(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCntEnemy] != NULL)
		{
			m_apTexture[nCntEnemy]->Release();
			m_apTexture[nCntEnemy] = NULL;
		}
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CEnemy::Init(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (m_enemyType == ENEMYTYPE_1)
		{
			CScene2D::BindTexture(m_apTexture[0]);
		}
		else if (m_enemyType == ENEMYTYPE_2)
		{
			CScene2D::BindTexture(m_apTexture[1]);
		}
		else if (m_enemyType == ENEMYTYPE_3)
		{
			CScene2D::BindTexture(m_apTexture[2]);
		}
		else if (m_enemyType == ENEMYTYPE_4)
		{
			CScene2D::BindTexture(m_apTexture[3]);
		}
		else if (m_enemyType == ENEMYTYPE_5)
		{
			CScene2D::BindTexture(m_apTexture[4]);
		}
		else if (m_enemyType == ENEMYTYPE_6)
		{
			CScene2D::BindTexture(m_apTexture[5]);
		}
		else if (m_enemyType == ENEMYTYPE_TUTORIAL)
		{
			CScene2D::BindTexture(m_apTexture[6]);
		}
	}

	CScene2D::Init();

	//�I�u�W�F�N�g�̎�ނ̎擾
	CScene::SetObjType(CScene::OBJTYPE_ENEMY);

	m_BulletRecast = 60;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEnemy::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CEnemy::Update(void)
{
	CManager manager;

	//�|�W�V�����̎擾
	D3DXVECTOR3 pos;
	pos = *GetPosition();

	CEffect::Create(pos, D3DCOLOR_RGBA(0, 0, 255, 255), 20,5);

	//�e�̈ړ������E�e��
	D3DXVECTOR3 move;
	move = D3DXVECTOR3(-3, 0, 0);

	if (m_enemyType == ENEMYTYPE_1)
	{
		pos.x -= 1;
		m_BulletRecast++;
		if (m_BulletRecast >= 60)
		{

			CBullet::Create(pos, move, D3DCOLOR_RGBA(255, 0, 255, 255),30, 350, CBullet::BULLETTYPE_ENEMY);

			m_BulletRecast = 0;
		}
	}

	if (m_enemyType == ENEMYTYPE_2)
	{
		pos.x -= 1;
		m_BulletRecast++;
		if (m_BulletRecast >= 8)
		{
			CBullet::Create(pos, 
				D3DXVECTOR3(sinf(rand() % 628 / 100.0f) * 5, cosf(rand() % 628 / 100.0f) * 5, 0.0f),
				D3DCOLOR_RGBA(255, 105, 0, 255),
				30,
				250, 
				CBullet::BULLETTYPE_ENEMY);

			m_BulletRecast = 0;
		}
	}

	if (m_enemyType == ENEMYTYPE_3)
	{
		pos.x -= 1;
		m_BulletRecast++;
		if (m_BulletRecast >= 12)
		{
			CBullet::Create(pos, 
				D3DXVECTOR3(sinf((rand() % 230 / 100.0f) - 2.7f) * 5, cosf((rand() % 230 / 100.0f) - 2.7f) * 5, 0.0f),
				D3DCOLOR_RGBA(255, 200, 100, 255),
				30, 
				250, 
				CBullet::BULLETTYPE_ENEMY);

			m_BulletRecast = 0;
		}
	}

	if (m_enemyType == ENEMYTYPE_4)
	{
		pos.x -= 5;
		m_BulletRecast++;
		if (m_BulletRecast >= 9)
		{
			CBullet::Create(pos,pos,D3DCOLOR_RGBA(20, 0, 20, 255),30,250,CBullet::BULLETTYPE_ENEMY);

			m_BulletRecast = 0;
		}
	}

	if (m_enemyType == ENEMYTYPE_5)
	{
		pos.y -= 1;
		m_BulletRecast++;
		if (m_BulletRecast >= 10)
		{
			CBullet::Create(pos, move, D3DCOLOR_RGBA(255, 50, 50, 255), 30, 300, CBullet::BULLETTYPE_ENEMY);

			m_BulletRecast = 0;
		}
	}

	if (m_enemyType == ENEMYTYPE_6)
	{
		pos.y += 1;
		m_BulletRecast++;
		if (m_BulletRecast >= 10)
		{
			CBullet::Create(pos, move, D3DCOLOR_RGBA(255, 50, 50, 255), 30, 300, CBullet::BULLETTYPE_ENEMY);

			m_BulletRecast = 0;
		}
	}

	if (m_enemyType == ENEMYTYPE_TUTORIAL)
	{
		m_BulletRecast++;
		if (m_BulletRecast >= 60)
		{
			CBullet::Create(pos, move, D3DCOLOR_RGBA(200, 120, 100, 255), 60, 250, CBullet::BULLETTYPE_ENEMY);

			m_BulletRecast = 0;
		}
	}
	SetPosition(pos);
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEnemy::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �N���G�C�g
//=============================================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos,float size,int nLife, ENEMYTYPE enemyType)
{
	CEnemy *pEnemy = NULL;
	if (pEnemy == NULL)
	{
		// ���I�m��
		pEnemy = new CEnemy;
		if (pEnemy != NULL)
		{
			pEnemy->m_posPolygon = pos;
			pEnemy->SetSize(size);
			pEnemy->SetLife(nLife);
			pEnemy->m_enemyType = enemyType;
			// ����������
			pEnemy->Init();
		}
	}
	return pEnemy;
}

//=============================================================================
// �G�̃��C�t��ݒ�
//=============================================================================
void CEnemy::SetLife(int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
// �G�̃��C�t�����炷
//=============================================================================
void CEnemy::CutLife(int nDamage)
{
	CManager manager;

	//�T�E���h�̎擾
	CSound *pSound;
	pSound = manager.GetSound();

	m_nLife -= nDamage;

	for (int nCntPriority = 0; nCntPriority < MAX_PRI; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{
			CScene2D *pScene2;
			pScene2 = (CScene2D*)CScene::GetScene(nCntPriority, nCntScene);
			if (pScene2 != NULL)
			{
				CScene::OBJTYPE objType;
				objType = pScene2->GetObjType();

				if (objType == CScene::OBJTYPE_SCORE)
				{
					CScore *pScore = (CScore*)pScene2;
					pScore->AddScore(nDamage * 100);
				}
			}
		}
	}

	if (m_nLife <= 0)
	{
		Uninit();
		CGauge2::m_fGauge2 += 10;

		pSound->PlaySound(CSound::SOUND_LABEL_SE_EXPLOSION);

		for (int nCntPriority = 0; nCntPriority < MAX_PRI; nCntPriority++)
		{
			for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
			{
				CScene2D *pScene2;
				pScene2 = (CScene2D*)CScene::GetScene(nCntPriority, nCntScene);
				if (pScene2 != NULL)
				{
					CScene::OBJTYPE objType;
					objType = pScene2->GetObjType();

					if (objType == CScene::OBJTYPE_SCORE)
					{
						CScore *pScore = (CScore*)pScene2;
						pScore->AddScore(1000000);
					}
				}
			}
		}
	}

}

//=============================================================================
// �G�̃��C�t���擾����
//=============================================================================
int CEnemy::GetLife(void)
{
	return m_nLife;
}