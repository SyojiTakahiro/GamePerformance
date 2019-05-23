//=============================================================================
//
// ボスエネミー処理 [boss.cpp]
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
#include "boss.h"
#include "explosion.h"
#include "score.h"
#include "effect.h"
#include "gauge2.h"
#include "fade.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_BOSS			(3)

//敵
#define BOSS_TEXTURE1		"data/TEXTURE/enemy0000.png"
#define BOSS_TEXTURE2		"data/TEXTURE/enemy0002.jpg"
#define BOSS_TEXTURE3		"data/TEXTURE/enemy0001.png"

//静的メンバ変数
CRenderer *m_pBossRenderer = NULL;
LPDIRECT3DTEXTURE9 CBoss::m_apTexture[MAX_BOSS] = {};
int CBoss::m_nLife = 0;

bool g_bBossDelete;
//=============================================================================
// コンストラクタ
//=============================================================================
CBoss::CBoss() : CScene2D(3)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CBoss::~CBoss()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CBoss::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, BOSS_TEXTURE1, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, BOSS_TEXTURE2, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, BOSS_TEXTURE3, &m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CBoss::Unload(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_BOSS; nCntEnemy++)
	{
		// テクスチャの破棄
		if (m_apTexture[nCntEnemy] != NULL)
		{
			m_apTexture[nCntEnemy]->Release();
			m_apTexture[nCntEnemy] = NULL;
		}
	}
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT CBoss::Init(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_BOSS; nCntEnemy++)
	{
		if (m_BossType == BOSSTYPE_1)
		{
			CScene2D::BindTexture(m_apTexture[0]);
		}
		else if (m_BossType == BOSSTYPE_2)
		{
			CScene2D::BindTexture(m_apTexture[1]);
		}
		else if (m_BossType == BOSSTYPE_3)
		{
			CScene2D::BindTexture(m_apTexture[2]);
		}
	}

	CScene2D::Init();

	//オブジェクトの種類の取得
	CScene::SetObjType(CScene::OBJTYPE_BOSS);

	m_BulletRecast1 = 0;
	m_BulletRecast2 = 0;
	m_BulletRecast3 = 0;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBoss::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CBoss::Update(void)
{
	CManager manager;

	//ポジションの取得
	D3DXVECTOR3 pos;
	pos = *GetPosition();

	CEffect::Create(pos, D3DCOLOR_RGBA(0, 0, 255, 255), 20, 5);

	//弾の移動方向・弾速
	D3DXVECTOR3 move;
	move = D3DXVECTOR3(-3, 0, 0);

	if (m_BossType == BOSSTYPE_1)
	{
		pos.x -= 1;
		if (pos.x == 1000)
		{
			pos.x += 1;
		}

		m_BulletRecast1++;
		m_BulletRecast2++;
		m_BulletRecast3++;

		if (m_BulletRecast1 >= 40)
		{

			CBullet::Create(pos, move, D3DCOLOR_RGBA(255, 0, 255, 255), 30, 250, CBullet::BULLETTYPE_ENEMY);

			m_BulletRecast1 = 0;
		}

		if (m_BulletRecast2 >= 8)
		{
			CBullet::Create(pos,
				D3DXVECTOR3(sinf((rand() % 200 / 100.0f) - 2.3f) * 5, cosf((rand() % 200 / 100.0f) - 2.3f) * 5, 0.0f),
				D3DCOLOR_RGBA(255, 255, 0, 255),
				30,
				250,
				CBullet::BULLETTYPE_ENEMY);

			m_BulletRecast2 = 0;
		}

		if (m_BulletRecast2 >= 7)
		{
			CBullet::Create(pos,
				D3DXVECTOR3(sinf((rand() % 230 / 100.0f) - 2.7f) * 5, cosf((rand() % 230 / 100.0f) - 2.7f) * 5, 0.0f),
				D3DCOLOR_RGBA(0, 255, 250, 255),
				30,
				250,
				CBullet::BULLETTYPE_ENEMY);

			m_BulletRecast3 = 0;
		}
	}

	SetPosition(pos);
}

//=============================================================================
// 描画処理
//=============================================================================
void CBoss::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// クリエイト
//=============================================================================
CBoss *CBoss::Create(D3DXVECTOR3 pos, float size, int nLife, BOSSTYPE bossType)
{
	CBoss *pEnemy = NULL;
	if (pEnemy == NULL)
	{
		// 動的確保
		pEnemy = new CBoss;
		if (pEnemy != NULL)
		{
			pEnemy->m_posPolygon = pos;
			pEnemy->SetSize(size);
			pEnemy->SetBossLife(nLife);
			pEnemy->m_BossType = bossType;
			// 初期化処理
			pEnemy->Init();
		}
	}
	return pEnemy;
}

//=============================================================================
// ボスのライフを設定
//=============================================================================
void CBoss::SetBossLife(int nLife)
{
	m_nLife = nLife;
}

//=============================================================================
// ボスのライフを減らす
//=============================================================================
void CBoss::CutBossLife(int nDamage)
{
	CManager manager;
	CFade *pFade = CManager::GetFade();

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
						pScore->AddScore(10000000);
					}
				}
			}
		}
	}

}

//=============================================================================
// ライフ情報を取得する
//=============================================================================
int CBoss::GetBossLife(void)
{
	return m_nLife;
}