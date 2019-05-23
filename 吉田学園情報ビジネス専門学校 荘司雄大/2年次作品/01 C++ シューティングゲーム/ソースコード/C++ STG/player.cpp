//=============================================================================
//
// プレイヤー処理 [player.cpp]
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
#include "enemy.h"
#include "bullet.h"
#include "bulletSP.h"
#include "bulletEX.h"
#include "effect.h"
#include "gauge.h"
#include "gauge2.h"
#include "sound.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define PLAYER_SPEED		(7)
//静的メンバ変数
CRenderer *m_pPlayerRenderer = NULL;
LPDIRECT3DTEXTURE9 CPlayer::m_pTexture = NULL;

bool g_bPlayerDelete;
//=============================================================================
// コンストラクタ
//=============================================================================
CPlayer::CPlayer() : CScene2D(3)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// ロード
//=============================================================================
HRESULT CPlayer::Load(void)
{
	CManager manager;

	//テクスチャの読み込み
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, PLAYER_TEXTURE, &m_pTexture);

	return S_OK;
}

//=============================================================================
// アンロード
//=============================================================================
void CPlayer::Unload(void)
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
HRESULT CPlayer::Init(void)
{
	CScene2D::BindTexture(m_pTexture);

	CScene2D::Init();

	//オブジェクトの種類の取得
	CScene::SetObjType(CScene::OBJTYPE_PLAYER);

	m_BulletRecast = 60;
	m_SlashRecast = 60;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void CPlayer::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// 更新処理
//=============================================================================
void CPlayer::Update(void)
{
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

	//ポジションの取得
	D3DXVECTOR3 pos;
	pos = *GetPosition();

	//弾の移動方向・弾速
	D3DXVECTOR3 move;
	move = D3DXVECTOR3(15,0,0);

	CEffect::Create(pos, D3DCOLOR_RGBA(255, 0, 0, 255), 20,5);

	if (CGauge::m_fGauge < 400 && CGauge2::m_fGauge2 > 200)
	{
		if (pInput->GetKeyboardTrigger(DIK_X) == true)
		{
			CEffect::Create(pos, D3DCOLOR_RGBA(0, 255, 0, 255), 80, 50);
		}
	}
	if (pInputJoypad != NULL)
	{
		if (CGauge::m_fGauge < 400 && CGauge2::m_fGauge2 > 200)
		{
			if (pInputJoypad->GetPress(8) == true )
			{
				CEffect::Create(pos, D3DCOLOR_RGBA(0, 255, 0, 255), 80, 50);
			}
		}
	}
	//アニメーション
	m_nCounterAnim++;
	//カウンターの更新
	if ((m_nCounterAnim % 4) == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % 10;
		SetPlayer(m_nPatternAnim);
	}

	//右移動
	if (pInput->GetKeyboardPress(DIK_D) == true)
	{
		pos.x += PLAYER_SPEED;
	}
	//左移動
	if (pInput->GetKeyboardPress(DIK_A) == true)
	{
		pos.x -= PLAYER_SPEED;
	}
	//上移動
	if (pInput->GetKeyboardPress(DIK_W) == true)
	{
		pos.y -= PLAYER_SPEED;
	}
	//下移動
	if (pInput->GetKeyboardPress(DIK_S) == true)
	{
		pos.y += PLAYER_SPEED;
	}

	//移動
	if (pInputJoypad != NULL)
	{
		if (pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_20) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_21) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_22) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_23) == true)
		{
			pos.x += sinf(pInputJoypad->GetLeftStickAxiz()) * 7;
			pos.y += cosf(pInputJoypad->GetLeftStickAxiz()) * 7;
		}

		if (pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_28) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_29) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_30) == true ||
			pInputJoypad->GetPress(CInputJoypad::DIJS_BUTTON_31) == true)
		{
			pos.x += sinf(pInputJoypad->GetRadian()) * 7;
			pos.y -= cosf(pInputJoypad->GetRadian()) * 7;
		}
	}

	//移動制限
	if (pos.x  < 20)
	{
		pos.x = 20;
	}
	if (pos.x + 20 > SCREEN_WIDTH)
	{
		pos.x = SCREEN_WIDTH - 20;
	}
	if (pos.y - 20 < 0)
	{
		pos.y = 20;
	}
	if (pos.y + 135 > SCREEN_HEIGHT)
	{
		pos.y = SCREEN_HEIGHT - 135;
	}

	//弾発射
	//通常弾
	m_BulletRecast++;
	m_SlashRecast++;
	if (pInput->GetKeyboardPress(DIK_SPACE) == true)
	{
		if (m_BulletRecast >= 10)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_SHOT);
			CBullet::Create(D3DXVECTOR3(pos.x + 20.0f,pos.y,0.0f), move, D3DCOLOR_RGBA(255, 255, 255, 255), 30, 200, CBullet::BULLETTYPE_PLAYER);
			m_BulletRecast = 0;
		}
	}
	if (pInputJoypad != NULL)
	{
		if (pInputJoypad->GetPress(7) == true||
			pInputJoypad->GetPress(5) == true)
		{
			if (m_BulletRecast >= 10)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SHOT);
				CBullet::Create(D3DXVECTOR3(pos.x + 20.0f, pos.y, 0.0f), move, D3DCOLOR_RGBA(255, 255, 255, 255), 30, 200, CBullet::BULLETTYPE_PLAYER);
				m_BulletRecast = 0;
			}
		}
	}

	//必殺技
	if (CGauge2::m_fGauge2 >= 400) 
	{
		if (pInput->GetKeyboardPress(DIK_Q) == true)
		{
			if (m_BulletRecast >= 5)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SPSHOT);
				CBulletSP::Create(D3DXVECTOR3(pos.x + 20.0f, pos.y, 0.0f),
							   	  D3DXVECTOR3(2, 0, 0),
								  200, 
								  400, 
								  CBulletSP::BULLETTYPE_PLAYER);

				CGauge2::m_fGauge2 -= 400;
				m_BulletRecast = 0;
			}
		}
	}
	if (pInputJoypad != NULL)
	{
		//必殺技
		if (CGauge2::m_fGauge2 >= 400)
		{
			if (pInputJoypad->GetTrigger(9) == true)
			{
				if (m_BulletRecast >= 5)
				{
					pSound->PlaySound(CSound::SOUND_LABEL_SE_SPSHOT);
					CBulletSP::Create(D3DXVECTOR3(pos.x + 20.0f, pos.y, 0.0f),
						D3DXVECTOR3(2, 0, 0),
						200,
						400,
						CBulletSP::BULLETTYPE_PLAYER);

					CGauge2::m_fGauge2 -= 400;
					m_BulletRecast = 0;
				}
			}
		}
	}

	//吸収弾
	if (pInput->GetKeyboardTrigger(DIK_E) == true)
	{
		if (m_SlashRecast >= 80)
		{
			pSound->PlaySound(CSound::SOUND_LABEL_SE_SLASH);
			CBulletEX::Create(D3DXVECTOR3(pos.x + 50.0f, pos.y, 0.0f), D3DXVECTOR3(0, 0, 0), 100,20, CBulletEX::BULLETTYPE_PLAYER);
			m_SlashRecast = 0;
		}
	}
	if (pInputJoypad != NULL)
	{
		if (pInputJoypad->GetTrigger(6) == true||
			pInputJoypad->GetTrigger(4) == true)
		{
			if (m_SlashRecast >= 80)
			{
				pSound->PlaySound(CSound::SOUND_LABEL_SE_SLASH);
				CBulletEX::Create(D3DXVECTOR3(pos.x + 50.0f, pos.y, 0.0f), D3DXVECTOR3(0, 0, 0), 100, 20, CBulletEX::BULLETTYPE_PLAYER);
				m_SlashRecast = 0;
			}
		}
	}

	SetPosition(pos);

}

//=============================================================================
// 描画処理
//=============================================================================
void CPlayer::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// クリエイト
//=============================================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos,float size)
{
	CPlayer *pPlayer = NULL;
	if (pPlayer == NULL)
	{
		// 動的確保
		pPlayer = new CPlayer;
		if (pPlayer != NULL)
		{
			pPlayer->m_posPolygon = pos;
			pPlayer->SetSize(size);

			// 初期化処理
			pPlayer->Init();
		}
	}
	return pPlayer;
}