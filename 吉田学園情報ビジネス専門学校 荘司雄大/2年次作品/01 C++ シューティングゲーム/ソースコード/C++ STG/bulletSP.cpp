//=============================================================================
//
// 必殺弾処理 [bulletSP.cpp]
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
// マクロ定義
//*****************************************************************************
#define BULLET_SPEED		(15)
//静的メンバ変数
CRenderer *m_pBulletSPRenderer = NULL;
LPDIRECT3DTEXTURE9 CBulletSP::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CBulletSP::CBulletSP() : CScene2D(3)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CBulletSP::~CBulletSP()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CBulletSP::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, SPBULLET_TEXTURE, &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CBulletSP::Unload(void)
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
HRESULT CBulletSP::Init(void)
{
	CScene2D::BindTexture(m_pTexture);

	//弾の初期設定
	CScene2D::Init();

	//オブジェクトの種類の取得
	CScene::SetObjType(CScene::OBJTYPE_EXBULLET);

	SetBulletSP(0);

	m_HitCounter = 0;

	m_DamageRecast = 60;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBulletSP::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CBulletSP::Update(void)
{
	//弾の寿命の減少
	m_nBulletLife--;

	//ポジションの取得
	D3DXVECTOR3 pos;
	pos = *GetPosition();

	//エフェクトの生成
	CEffect::Create(pos, D3DCOLOR_RGBA(255, 0, 150, 255), 30,100);
	CEffect::Create(pos, D3DCOLOR_RGBA(255, 0, 255, 255), 10,100);

	//弾の移動
	pos.x += m_move.x;	//弾の位置を移動させる
	pos.y += m_move.y;	//弾の位置を移動させる

	CManager manager;

	//キーボードの取得
	CInputKeyboard *pInput;
	pInput = manager.GetInputKeyboard();

	//ジョイパッドの取得
	CInputJoypad *pInputJoypad;
	pInputJoypad = CManager::GetInputJoypad();

	//サウンドの取得
	CSound *pSound;
	pSound = manager.GetSound();

		//上移動
	if (pInput->GetKeyboardPress(DIK_UP) == true)
	{
		pos.y -= 7;
	}
		//下移動
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
		//移動
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

	//アニメーション
	m_nCounterAnim++;
	if ((m_nCounterAnim % 4) == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % 10;
		SetBulletSP(m_nPatternAnim);
	}

	//弾の消滅
	if (m_nBulletLife <= 0 || pos.x > SCREEN_WIDTH)
	{
		pSound->StopSound(CSound::SOUND_LABEL_SE_SPSHOT);
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
					//フレーム毎のHIT判定
					m_HitCounter++;
					if ((m_HitCounter % 10) == 0)
					{
						//弾の当たり判定(敵)
						if (objType == CScene::OBJTYPE_ENEMY && m_bulletType == BULLETTYPE_PLAYER)
						{
							CEnemy *pEnemy = (CEnemy*)pScene;
							D3DXVECTOR3 EnemyPos = *pScene->GetPosition();

							if (pos.x > EnemyPos.x - 100
								&& pos.x < EnemyPos.x + 100
								&& pos.y > EnemyPos.y - 100
								&& pos.y < EnemyPos.y + 100)
							{
								//爆発アニメーション
								CExplosion::Create(EnemyPos, 50);
								pSound->PlaySound(CSound::SOUND_LABEL_SE_HIT000);
								//HIT対象の終了処理					
								pEnemy->CutLife(10);

								break;
							}
						}

						//弾の当たり判定(敵)
						if (objType == CScene::OBJTYPE_BOSS && m_bulletType == BULLETTYPE_PLAYER)
						{
							CBoss *pBoss = (CBoss*)pScene;
							D3DXVECTOR3 BossPos = *pScene->GetPosition();

							if (pos.x > BossPos.x - 100
								&& pos.x < BossPos.x + 100
								&& pos.y > BossPos.y - 100
								&& pos.y < BossPos.y + 100)
							{
		
								//爆発アニメーション
								CExplosion::Create(BossPos, 50);
								//HIT対象の終了処理					
								pBoss->CutBossLife(10);

								break;
							}
						}
						m_HitCounter = 0;
					}
					//弾の当たり判定(敵弾)
					if (objType == CScene::OBJTYPE_BULLET && m_bulletType == BULLETTYPE_PLAYER)
					{
						D3DXVECTOR3 BulletPos = *pScene->GetPosition();

						if (pos.x > BulletPos.x - 100
							&& pos.x < BulletPos.x + 100
							&& pos.y > BulletPos.y - 100
							&& pos.y < BulletPos.y + 100)
						{
							//HIT時の処理
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
// 描画処理
//=============================================================================
void CBulletSP::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// クリエイト
//=============================================================================
CBulletSP *CBulletSP::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size,int life, BULLETTYPE bulletType)
{
	CBulletSP *pBullet = NULL;
	if (pBullet == NULL)
	{
		// 動的確保
		pBullet = new CBulletSP;
		if (pBullet != NULL)
		{
			pBullet->m_posPolygon = pos;
			pBullet->m_move = move;
			pBullet->SetSize(size);
			pBullet->m_nBulletLife = life;
			pBullet->m_bulletType = bulletType;

			// 初期化処理
			pBullet->Init();
		}
	}
	return pBullet;
}