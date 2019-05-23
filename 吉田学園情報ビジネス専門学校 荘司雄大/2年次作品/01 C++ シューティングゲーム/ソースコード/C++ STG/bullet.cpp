//=============================================================================
//
// 弾処理 [bullet.cpp]
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
// マクロ定義
//*****************************************************************************
#define BULLET_SPEED		(15)
//静的メンバ変数
CRenderer *m_pBulletRenderer = NULL;
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

bool g_bBulletDelete;
//=============================================================================
// コンストラクタ
//=============================================================================
CBullet::CBullet() : CScene2D(3)
{
	
}

//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CBullet::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURE, &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CBullet::Unload(void)
{
	// テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBullet::Init(void)
{
	CScene2D::BindTexture(m_pTexture);

	//弾の初期設定
	CScene2D::Init();

	//オブジェクトの種類の取得
	CScene::SetObjType(CScene::OBJTYPE_BULLET);

	m_nPlayerLife = 470;
	m_DamageRecast = 60;

	CScene2D::SetColor(m_col);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBullet::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CBullet::Update(void)
{
	m_nBulletLife--;

	CManager manager;

	//ポジションの取得
	D3DXVECTOR3 pos;
	pos = *GetPosition();

	//サウンドの取得
	CSound *pSound;
	pSound = manager.GetSound();

	//弾の移動
	pos.x += m_move.x;	//弾の位置を移動させる
	pos.y += m_move.y;	//弾の位置を移動させる

	SetPosition(pos);

	CEffect::Create(pos, D3DCOLOR_RGBA(255, 255, 255, 255), 10, 6);

	//弾の消滅
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
			//Sceneの種類を指定
			for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
			{
				CScene2D *pScene;
				pScene = (CScene2D*)CScene::GetScene(nCntPriority, nCntScene);
				if (pScene != NULL)
				{
					CScene::OBJTYPE objType;
					objType = pScene->GetObjType();

					//弾の当たり判定(敵)

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

					//弾の当たり判定(敵)

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

					//弾の当たり判定(自機)
					if (objType == CScene::OBJTYPE_PLAYER && m_bulletType == BULLETTYPE_ENEMY)
					{
						D3DXVECTOR3 PlayerPos = *pScene->GetPosition();

						if (pos.x > PlayerPos.x - 20
							&& pos.x < PlayerPos.x + 20
							&& pos.y > PlayerPos.y - 20
							&& pos.y < PlayerPos.y + 20)
						{
							//ダメージ
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
// 描画処理
//=============================================================================
void CBullet::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// クリエイト
//=============================================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col,float size,int life, BULLETTYPE bulletType)
{
		CBullet *pBullet = NULL;
		if (pBullet == NULL)
		{
			// 動的確保
			pBullet = new CBullet;
			if (pBullet != NULL)
			{
				pBullet->m_posPolygon = pos;
				pBullet->m_move = move;
				pBullet->m_col = col;
				pBullet->SetSize(size);
				pBullet->m_nBulletLife = life;
				pBullet->m_bulletType = bulletType;

				// 初期化処理
				pBullet->Init();
			}
		}
		return pBullet;
}