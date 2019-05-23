//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : 荘司　雄大
//
//=============================================================================
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BULLET			(25600)					//弾の最大数
#define BULLET_WIDTH		(10)					//弾の幅
#define BULLET_HEIGHT		(10)					//弾の高さ
#define BULLET_TEXTURENAME	"data/TEXTURE/bullet000.png"	//読み込む背景テクスチャ

//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXCOLOR color;
	BULLETTYPE type;
	int nLife;			//寿命
	bool bUse;			//使用しているかどうか

}Bullet;

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// 頂点バッファへのポインタ
D3DXVECTOR3 g_posBullet;							//ポリゴンの位置
D3DXVECTOR3 g_moveBullet;							//ポリゴンの移動量
D3DXCOLOR g_color[5];

Bullet g_aBullet[MAX_BULLET];						//弾の情報


//=============================================================================
// 初期化処理
//=============================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	int nCntBullet;

	// デバイスの取得
	pDevice = GetDevice();

	// 弾の情報の初期化
	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_posBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_moveBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 30;
		g_aBullet[nCntBullet].bUse = false;
	}

	g_color[0] = D3DXCOLOR(1.0f, 0.0f, 0.0f,1.0f);
	g_color[1] = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	g_color[2] = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	g_color[3] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	g_color[4] = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	//g_color[5] = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURENAME,&g_pTextureBullet);

	// 頂点情報の作成
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,	//確保するバッファのサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2), -(BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
		pVtx[1].pos = D3DXVECTOR3((BULLET_WIDTH / 2), -(BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
		pVtx[2].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2), (BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
		pVtx[3].pos = D3DXVECTOR3((BULLET_WIDTH / 2), (BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			pVtx += 4;//頂点データのポインタを4つ進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBullet(void)
{
	// テクスチャの開放
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBullet(void)
{
	int nCntBullet;
	int nCntEnemy;
	ENEMY *pEnemy;

	VERTEX_2D *pVtx;//頂点情報へのポインタ

	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)//弾が使用される
		{
			//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;//弾の位置を移動させる
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;//弾の位置を移動させる


			//頂点座標の設定
			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2), -(BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3((BULLET_WIDTH / 2), -(BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2), (BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3((BULLET_WIDTH / 2), (BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;

			g_aBullet[nCntBullet].nLife--;

			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				//SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				g_aBullet[nCntBullet].bUse = false;
			}


			//頂点バッファをアンロックする
			g_pVtxBuffBullet->Unlock();

			pEnemy = GetEnemy();

			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{
						if (g_aBullet[nCntBullet].pos.x > pEnemy->pos.x - 80
							&& g_aBullet[nCntBullet].pos.x < pEnemy->pos.x + 80
							&& g_aBullet[nCntBullet].pos.y > pEnemy->pos.y - 80
							&& g_aBullet[nCntBullet].pos.y < pEnemy->pos.y + 80)
						{
							HitEnemy(nCntEnemy, 1);
							g_aBullet[nCntBullet].bUse = false;
						}
					}
				}
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20.0f, 30);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{
				Player *pPlayer;
				//プレイヤーの取得
				pPlayer = GetPlayer();
				if (pPlayer->bUse == true)
				{
					if (g_aBullet[nCntBullet].pos.x > pPlayer->pos.x - 10
						&& g_aBullet[nCntBullet].pos.x < pPlayer->pos.x + 10
						&& g_aBullet[nCntBullet].pos.y > pPlayer->pos.y - 10
						&& g_aBullet[nCntBullet].pos.y < pPlayer->pos.y + 5)
					{
						HitPlayer(1);

						//SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						g_aBullet[nCntBullet].bUse = false;
					}
				}
				SetEffect(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].color, 15.0f, 10);
			}
			if (g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH|| g_aBullet[nCntBullet].pos.x < 0 || g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT || g_aBullet[nCntBullet].pos.y < 0)
			{
				g_aBullet[nCntBullet].bUse = false;
			}
		}
	}
}


//=============================================================================
// 描画処理
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
    pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBullet);

	// ポリゴンの描画
	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntBullet * 4,//描画を開始する頂点インデックス
				2);
		}
	}
}

//=============================================================================
// 弾の設定処理
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type)
{
	int nCntBullet;
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない
			g_aBullet[nCntBullet].pos = pos;

			//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			//pVtx += 4;

			//頂点座標の設定
			pVtx[nCntBullet*4].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2), -(BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
			pVtx[nCntBullet*4+1].pos = D3DXVECTOR3((BULLET_WIDTH / 2), -(BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
			pVtx[nCntBullet*4+2].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2), (BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
			pVtx[nCntBullet*4+3].pos = D3DXVECTOR3((BULLET_WIDTH / 2), (BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;

			//頂点バッファをアンロックする
			g_pVtxBuffBullet->Unlock();


			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].color = g_color[rand() % 5];
			g_aBullet[nCntBullet].nLife = 1000;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].bUse = true;//使用している状態にする

			break;
		}
	}
}

