//=============================================================================
//
// 弾切断処理 [bulletEX.cpp]
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
// マクロ定義
//*****************************************************************************
#define BULLET_SPEED		(15)
//静的メンバ変数
CRenderer *m_pBulletEXRenderer = NULL;
LPDIRECT3DTEXTURE9 CBulletEX::m_pTexture = NULL;

//=============================================================================
// コンストラクタ
//=============================================================================
CBulletEX::CBulletEX() : CScene2D(3)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CBulletEX::~CBulletEX()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CBulletEX::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, EXBULLET_TEXTURE, &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CBulletEX::Unload(void)
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
HRESULT CBulletEX::Init(void)
{
	CScene2D::BindTexture(m_pTexture);

	//弾の初期設定
	CScene2D::Init();

	//オブジェクトの種類の取得
	CScene::SetObjType(CScene::OBJTYPE_EXBULLET);

	SetBulletEX(0);

	m_DamageRecast = 60;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CBulletEX::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CBulletEX::Update(void)
{
	m_nBulletLife--;

	//ポジションの取得
	D3DXVECTOR3 pos;
	pos = *GetPosition();

	//弾の移動
	pos.x += m_move.x;	//弾の位置を移動させる
	pos.y += m_move.y;	//弾の位置を移動させる

	CManager manager;

	//キーボードの取得
	CInputKeyboard *pInput;
	pInput = manager.GetInputKeyboard();
	if (pInput->GetKeyboardPress(DIK_Q) == true)
	{
		//上移動
		if (pInput->GetKeyboardPress(DIK_W) == true)
		{
			pos.y -= 7;
		}
		//下移動
		if (pInput->GetKeyboardPress(DIK_S) == true)
		{
			pos.y += 7;
		}
	}
	SetPosition(pos);


	//弾の消滅
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
			//Sceneの種類を指定
			for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
			{
				CScene2D *pScene;
				pScene = (CScene2D*)CScene::GetScene(nCntPriority, nCntScene);
				if (pScene != NULL)
				{
					CScene::OBJTYPE objType;
					objType = pScene->GetObjType();

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
							CGauge2::m_fGauge2 += 8;
							pScene->Uninit();

							break;
						}
					}
				}
			}
		}
	}

	//アニメーション
	m_nCounterAnim++;
	if ((m_nCounterAnim % 4) == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % 5;
		SetBulletEX(m_nPatternAnim);
		//パターン終了時に終了
		if (m_nPatternAnim >= 5 - 1)
		{
			Uninit();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void CBulletEX::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// クリエイト
//=============================================================================
CBulletEX *CBulletEX::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, float size, int life, BULLETTYPE bulletType)
{
	CBulletEX *pBullet = NULL;
	if (pBullet == NULL)
	{
		// 動的確保
		pBullet = new CBulletEX;
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