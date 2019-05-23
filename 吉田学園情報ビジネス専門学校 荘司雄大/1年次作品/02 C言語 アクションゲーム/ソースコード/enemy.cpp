//=============================================================================
//
// 敵処理 [enemy.cpp]
// Author :Takahiro
//
//=============================================================================
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "life.h"
//#include "explosion.h"
#include "input.h"
#include "game.h"
#include "block.h"
#include "enemy.h"
#include "sound.h"
#include "score.h"
//#include "effect.h"
#include "particle.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_TYPE_ENEMY		(5)						// 敵の種類
#define POLYGON_TEXTURENAME0		"data/TEXTURE/enemy0000.png"
#define POLYGON_TEXTURENAME1		"data/TEXTURE/enemy0001.png"
#define POLYGON_TEXTURENAME2		"TEXTURE/enemy001.png"
#define POLYGON_TEXTURENAME3		"TEXTURE/enemy002.png"
#define POLYGON_TEXTURENAME4		"TEXTURE/enemy003.png"

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureEnemy[MAX_TYPE_ENEMY] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;					// 頂点バッファへのポインタ
ENEMY g_aEnemy[MAX_ENEMY];
int g_nNumEnemy;
float g_eSpin = 5.0;
float g_eSpin2 = 6.0;
int g_eBulletRecast = 30;

D3DXCOLOR g_eColor[2];
//=============================================================================
// 初期化処理
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	// デバイスの取得
	pDevice = GetDevice();

	// 敵の情報の初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 敵の移動量
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].fHeight = 50;
		g_aEnemy[nCntEnemy].fWidth = 50;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nLife = 5;
		g_aEnemy[nCntEnemy].bUse = false;
	}
	g_nNumEnemy = 0;

	//g_eColor[0] = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
	//g_eColor[1] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	// テクスチャの読み込み(5体分)
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME0, &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME1, &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME2, &g_apTextureEnemy[2]);
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME3, &g_apTextureEnemy[3]);
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME4, &g_apTextureEnemy[4]);
	

	// 頂点情報の作成
	VERTEX_2D *pVtx;

	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - (g_aEnemy[nCntEnemy].fHeight), g_aEnemy[nCntEnemy].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - (g_aEnemy[nCntEnemy].fHeight), g_aEnemy[nCntEnemy].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].tex = D3DXVECTOR2(0, 0);
		pVtx[1].tex = D3DXVECTOR2(1, 0);
		pVtx[2].tex = D3DXVECTOR2(0, 1);
		pVtx[3].tex = D3DXVECTOR2(1, 1);

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitEnemy(void)
{
	int nCntTex;

	// テクスチャの開放
	for (nCntTex = 0; nCntTex < MAX_TYPE_ENEMY; nCntTex++)
	{
		if (g_apTextureEnemy[nCntTex] != NULL)
		{
			g_apTextureEnemy[nCntTex]->Release();
			g_apTextureEnemy[nCntTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateEnemy(void)
{
	int nCntEnemy;
	float fData;
	Player *pPlayer;
	pPlayer = GetPlayer();

	VERTEX_2D *pVtx;
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{

		g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;		//前回の位置を保存
		g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

		if (g_aEnemy[nCntEnemy].bUse == true)		//敵が使用されている
		{

			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
				if (g_aEnemy[nCntEnemy].nType == 0)
				{
					g_aEnemy[nCntEnemy].pos.x -= pPlayer->move.x;
					fData = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);
					g_aEnemy[nCntEnemy].pos += D3DXVECTOR3(sinf(fData) * 1.0f, cosf(fData) * 1.0f, 0.0f);
					g_aEnemy[nCntEnemy].pos.y += 1.8f;	//重力
				}
			
				else if (g_aEnemy[nCntEnemy].nType == 1)
				{
					g_aEnemy[nCntEnemy].pos.x -= pPlayer->move.x;
					fData = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);
					g_aEnemy[nCntEnemy].pos += D3DXVECTOR3(sinf(fData) * 1.0f, cosf(fData) * 1.0f, 0.0f);

				}
				/*g_eBulletRecast++;
				if (rand() % 120 == 0)
				{
					g_eBulletRecast++;
					if (g_eBulletRecast >= 60)
					{
						fData = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(fData) * 5.0f, cosf(fData) * 5.0f, 0.0f), 10.0f, 10.0f, BULLETTYPE_ENEMY);
						g_eBulletRecast = 0;
					}
				}*/

				break;

			case ENEMYSTATE_DAMAGE:
					g_aEnemy[nCntEnemy].nCounterState--;
					if (g_aEnemy[nCntEnemy].nCounterState <= 0)
					{//色を戻す
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
						g_aEnemy[nCntEnemy].nCounterState = 0;

						pVtx += (nCntEnemy * 4);
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					}
				
				break;
			}
			
			g_aEnemy[nCntEnemy].pos.x += g_aEnemy[nCntEnemy].move.x;
			g_aEnemy[nCntEnemy].pos.y += g_aEnemy[nCntEnemy].move.y;

			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - (g_aEnemy[nCntEnemy].fHeight), g_aEnemy[nCntEnemy].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - (g_aEnemy[nCntEnemy].fHeight), g_aEnemy[nCntEnemy].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y + (g_aEnemy[nCntEnemy].fHeight), g_aEnemy[nCntEnemy].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y + (g_aEnemy[nCntEnemy].fHeight), g_aEnemy[nCntEnemy].pos.z);
			pVtx += 4;

			
			/*for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
			{
			
			}*/

			if (CollisionBlockE(&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, &g_aEnemy[nCntEnemy].move, g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].fHeight/*, &g_player.pBlock*/) == true)
			{

			}

			//アンロック
			g_pVtxBuffEnemy->Unlock();
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// ポリゴンの描画
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=============================================================================
// 敵の設定
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCntEnemy;
	//int nCntEffect;
	VERTEX_2D*pVtx;

	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].nCounterState = 0;
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			g_aEnemy[nCntEnemy].nLife = 6;

			if (g_aEnemy[nCntEnemy].nType == 0 || g_aEnemy[nCntEnemy].nType == 1 || g_aEnemy[nCntEnemy].nType == 2 || g_aEnemy[nCntEnemy].nType == 3)
			{
				pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - (g_aEnemy[nCntEnemy].fHeight), g_aEnemy[nCntEnemy].pos.z);
				pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y - (g_aEnemy[nCntEnemy].fHeight), g_aEnemy[nCntEnemy].pos.z);
				pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y + (g_aEnemy[nCntEnemy].fHeight), g_aEnemy[nCntEnemy].pos.z);
				pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth, g_aEnemy[nCntEnemy].pos.y + (g_aEnemy[nCntEnemy].fHeight), g_aEnemy[nCntEnemy].pos.z);
				pVtx += 4;
			}
			g_aEnemy[nCntEnemy].bUse = true;
			g_nNumEnemy++;
			break;
		}
	}
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// Hit時の処理
//=============================================================================
void HitEnemy(int nCntEnemy, int nDmage)
{
	ENEMY *pEnemy;
	pEnemy = GetEnemy();
	
		g_aEnemy[nCntEnemy].nLife -= nDmage;
		if (g_aEnemy[nCntEnemy].nLife <= 0)
		{//ライフが０以下になったとき消滅
			//SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			//PlaySound(SOUND_LABEL_SE_EXPLOSION);
			g_aEnemy[nCntEnemy].bUse = false;
			AddScore(100);
			g_nNumEnemy--;
			//{// パーティクル発生
			//	D3DXVECTOR3 movePar(1.0f, 1.0f, 1.0f);
			//	movePar.x = sinf(D3DX_PI * g_eSpin) * (rand() % 10 * 1.5f);
			//	movePar.y = cosf(D3DX_PI * g_eSpin) * (rand() % 10 * 1.5f);
			//	SetParticle(g_aEnemy[nCntEnemy].pos, movePar, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), rand() % 30, rand() % 25);
			//}
		}
		/*if (g_nNumEnemy <= 0)
		{
			SetGameState(GAMESTATE_CLEAR);
		}*/
		else
		{//ヒット時敵の色を変更
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
			g_aEnemy[nCntEnemy].nCounterState = 5;
			//PlaySound(SOUND_LABEL_SE_HIT000);
			VERTEX_2D *pVtx;
			g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
			pVtx += (nCntEnemy * 4);
			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

			for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
			{// パーティクル発生
				D3DXVECTOR3 movePar(1.0f, 1.0f, 1.0f);
				movePar.x = sinf(D3DX_PI * 1.01f) * (rand() % 10 * 1.5f);
				movePar.y = cosf(D3DX_PI * 1.01f) * (rand() % 3);
				SetParticle(g_aEnemy[nCntEnemy].pos, movePar, D3DXCOLOR(255, 0, 0, 255), rand() % 30, rand() % 25);
			}
			g_pVtxBuffEnemy->Unlock();
		}
	
}

bool CollisionEnemy(D3DXVECTOR3 *pePos, D3DXVECTOR3 *pePosOld, float fWidth, float fHeight)
{
	//Item *pItem;
	int nCntEnemy;
	//pItem = GetItem();
	bool bLand = false;

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			if (pePos->x - (fWidth / 2) <= g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth
				&& pePos->x + (fWidth / 2) >= g_aEnemy[nCntEnemy].pos.x)
			{//アイテムの横幅
				if (pePosOld->y <= g_aEnemy[nCntEnemy].pos.y
					&&pePos->y >= g_aEnemy[nCntEnemy].pos.y)
				{//上からめり込んだ
					bLand = true;
					for (int nCntParticle = 0; nCntParticle < 70; nCntParticle++)
					{// パーティクル発生
						D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);

						SetParticle(g_aEnemy[nCntEnemy].pos, movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 10 + 30.0f, rand() % 20);
					}
					CutLife(1);
				//	SetGameState(GAMESTATE_END);

				}
				else if (pePosOld->y - fHeight >= g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].fHeight
					&& pePos->y - fHeight <= g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].fHeight)
				{//下からめりこんだ。
					bLand = true;
					for (int nCntParticle = 0; nCntParticle < 70; nCntParticle++)
					{// パーティクル発生
						D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);

						SetParticle(g_aEnemy[nCntEnemy].pos, movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 10 + 30.0f, rand() % 20);
					}
					CutLife(1);
					//SetGameState(GAMESTATE_END);
				}
			}
			//アイテムの縦幅
			if (//頭から足が壁にぶつかった
				pePosOld->y - fHeight <= g_aEnemy[nCntEnemy].pos.y
				&&pePosOld->y >= g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].fHeight
				//足が壁にぶつかった
				|| pePosOld->y - fHeight > g_aEnemy[nCntEnemy].pos.y
				&&pePosOld->y - fHeight < g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].fHeight
				//頭が壁にぶつかった
				|| pePosOld->y > g_aEnemy[nCntEnemy].pos.y
				&&pePosOld->y < g_aEnemy[nCntEnemy].pos.y + g_aEnemy[nCntEnemy].fHeight)
			{
				//左からめり込んだ
				if (pePosOld->x <= g_aEnemy[nCntEnemy].pos.x
					&& pePos->x + fWidth / 2 >= g_aEnemy[nCntEnemy].pos.x)
				{
					for (int nCntParticle = 0; nCntParticle < 70; nCntParticle++)
					{// パーティクル発生
						D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);

						SetParticle(g_aEnemy[nCntEnemy].pos, movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 10 + 30.0f, rand() % 20);
					}
					CutLife(1);
					//SetGameState(GAMESTATE_END);

				}
				//右からめり込んだ
				else if (pePosOld->x > g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth
					&&pePos->x - fWidth / 2 < g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fWidth)
				{
					for (int nCntParticle = 0; nCntParticle < 70; nCntParticle++)
					{// パーティクル発生
						D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);

						SetParticle(g_aEnemy[nCntEnemy].pos, movePar, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), rand() % 10 + 30.0f, rand() % 20);
					}
					CutLife(1);
					//SetGameState(GAMESTATE_END);
				}
			}
		}
	}
	return bLand;
}


//=============================================================================
// 敵の取得
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}