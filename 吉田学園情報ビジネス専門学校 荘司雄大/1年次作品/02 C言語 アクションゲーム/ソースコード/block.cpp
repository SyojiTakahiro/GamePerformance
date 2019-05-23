//=============================================================================
//
// ブロックの表示処理 [block.cpp]
// Author :Takahiro
//
//=============================================================================
#include "block.h"
#include "player.h"
#include "enemy.h"
#include "input.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_BLOCK		(256)		// ブロックの最大数
#define BLOCK_WIDTH		(25)		//ブロックの幅
#define BLOCK_HEIGHT	(25)		//ブロックの高さ
#define POLYGON_TEXTURENAME		"data/TEXTURE/block000.jpg"

//*****************************************************************************
// グローバル変数宣言
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBlock = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;	// 頂点バッファへのポインタ

Block g_Block[MAX_BLOCK];

D3DXVECTOR3 g_posBlock;
//=============================================================================
// 初期化処理
//=============================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	// デバイスの取得
	pDevice = GetDevice();

	// ブロックの情報の初期化
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_Block[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Block[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Block[nCntBlock].type = BLOCKTYPE_NORMAL;
		g_Block[nCntBlock].fWidth = 0;
		g_Block[nCntBlock].fHeight = 0;
		g_Block[nCntBlock].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME, &g_pTextureBlock);

	// 頂点情報の作成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	VERTEX_2D*pVtx;

	//頂点バッファをロック
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y, g_Block[nCntBlock].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + (g_Block[nCntBlock].fWidth), g_Block[nCntBlock].pos.y, g_Block[nCntBlock].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, g_Block[nCntBlock].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + (g_Block[nCntBlock].fWidth), g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, g_Block[nCntBlock].pos.z);


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
	//アンロック
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitBlock(void)
{
	// テクスチャの開放
	if (g_pTextureBlock != NULL)
	{
		g_pTextureBlock->Release();
		g_pTextureBlock = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBlock(void)
{
	Player *pPlayer;
	pPlayer = GetPlayer();

	float fBlockTexHeight;
	float fBlockTexWidth;
	int nCntBlock;

	VERTEX_2D*pVtx;
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntData = 0; nCntData < MAX_BLOCK; nCntData++)
	{
		if (g_Block[nCntData].bUse == true)
		{
			g_Block[nCntData].pos += g_Block[nCntData].move;

			switch (g_Block[nCntData].type)
			{
			case BLOCKTYPE_MOVE://移動
				if (g_Block[nCntData].PosOld.x - g_Block[nCntData].pos.x <= -150 ||
					g_Block[nCntData].PosOld.x - g_Block[nCntData].pos.x >= 150)
				{
					g_Block[nCntData].move.x *= -1;
				}
				if (g_Block[nCntData].PosOld.y - g_Block[nCntData].pos.y <= -250 ||
					g_Block[nCntData].PosOld.y - g_Block[nCntData].pos.y >= 250)
				{
					g_Block[nCntData].move.y *= -1;
				}
				break;
			}
		}
	}
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_Block[nCntBlock].bUse == true)
		{
			g_Block[nCntBlock].pos.x -= pPlayer->move.x;
			//g_Block[nCntBlock].pos.y -= pPlayer->move.y ;

			fBlockTexHeight = g_Block[nCntBlock].fHeight / 25;
			fBlockTexWidth = g_Block[nCntBlock].fWidth / 25;

			//
			pVtx[0].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y, g_Block[nCntBlock].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + (g_Block[nCntBlock].fWidth), g_Block[nCntBlock].pos.y, g_Block[nCntBlock].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, g_Block[nCntBlock].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + (g_Block[nCntBlock].fWidth), g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight, g_Block[nCntBlock].pos.z);


			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(fBlockTexWidth, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, fBlockTexHeight);
			pVtx[3].tex = D3DXVECTOR2(fBlockTexWidth, fBlockTexHeight);

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
			pVtx += 4;
		}
	}
	//
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBlock(void)
{
	int nCntBlock;
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスの取得
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBlock);

	// ポリゴンの描画
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_Block[nCntBlock].bUse == true)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCntBlock, 2);
		}
	}
}

//=============================================================================
// ブロックの設定処理
//=============================================================================
void SetBlock(D3DXVECTOR3 pos, D3DXCOLOR col,float fWidth,float fHeight,BLOCKTYPE type)
{
	int nCntBlock;
	float fBlockTexHeight;
	float fBlockTexWidth;
	VERTEX_2D*pVtx;

	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_Block[nCntBlock].bUse == false)
		{
			g_Block[nCntBlock].fWidth = fWidth;
			g_Block[nCntBlock].fHeight = fHeight;
			g_Block[nCntBlock].pos = pos;
			g_Block[nCntBlock].bUse = true;

			fBlockTexHeight = g_Block[nCntBlock].fHeight/50;
			fBlockTexWidth = g_Block[nCntBlock].fWidth/50;

			pVtx[0].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y, g_Block[nCntBlock].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + (g_Block[nCntBlock].fWidth), g_Block[nCntBlock].pos.y, g_Block[nCntBlock].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x, g_Block[nCntBlock].pos.y+g_Block[nCntBlock].fHeight, g_Block[nCntBlock].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_Block[nCntBlock].pos.x + (g_Block[nCntBlock].fWidth), g_Block[nCntBlock].pos.y+ g_Block[nCntBlock].fHeight, g_Block[nCntBlock].pos.z);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(fBlockTexWidth, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, fBlockTexHeight);
			pVtx[3].tex = D3DXVECTOR2(fBlockTexWidth, fBlockTexHeight);

			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);


			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffBlock->Unlock();
}

//=============================================================================
//ブロックにぶつかったときの処理
//=============================================================================

bool CollisionBlock(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fWidth, float fHeight)
{
	int nCntBlock;
	bool bLand = false;

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{	//足が着いたとき
		if (g_Block[nCntBlock].bUse == true)
		{//あたり判定
			if (pPos->x - (fWidth / 2) <= g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth
				&& pPos->x + (fWidth / 2) >= g_Block[nCntBlock].pos.x)
			{//ブロックの横幅
				if (pPosOld->y <= g_Block[nCntBlock].pos.y
					&&pPos->y >= g_Block[nCntBlock].pos.y)
				{//上からめり込んだ
					bLand = true;
					pPos->y = g_Block[nCntBlock].pos.y;
					pMove->y = 0.0f;
				}
			else if (pPosOld->y - fHeight >= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight
				&& pPos->y - fHeight <= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight)
			{//下からめりこんだ。
				bLand = true;
				pPos->y = g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight + fHeight;
				pMove->y = 0.0f;
			}
		}
		//ブロックの縦幅
			if (//頭から足が壁にぶつかった
				pPosOld->y - fHeight <= g_Block[nCntBlock].pos.y
				&&pPosOld->y >= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight
				//足が壁にぶつかった
				|| pPosOld->y - fHeight > g_Block[nCntBlock].pos.y
				&&pPosOld->y - fHeight < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight
				//頭が壁にぶつかった
				|| pPosOld->y > g_Block[nCntBlock].pos.y
				&&pPosOld->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight)
			{
				//左からめり込んだ
				if (pPosOld->x <= g_Block[nCntBlock].pos.x
					&& pPos->x + fWidth / 2 >= g_Block[nCntBlock].pos.x)
				{
					pPos->x = g_Block[nCntBlock].pos.x - fWidth / 2 - 1;
					pMove->x = 0.0f;
				}
				//右からめり込んだ
				else if (pPosOld->x > g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth
					&&pPos->x - fWidth / 2 < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
				{
					pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + fWidth / 2 + 1;
					pMove->x = 0.0f;
				}
			}
		}
	}
	return bLand;
}

bool CollisionBlockE(D3DXVECTOR3 *ePos, D3DXVECTOR3 *ePosOld, D3DXVECTOR3 *eMove, float fWidth, float fHeight)
{
	int nCntBlock;
	bool bLand = false;

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{	//足が着いたとき
		if (g_Block[nCntBlock].bUse == true)
		{//あたり判定
			if (ePos->x - (fWidth / 2) <= g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth
				&& ePos->x + (fWidth / 2) >= g_Block[nCntBlock].pos.x)
			{//ブロックの横幅
				if (ePosOld->y <= g_Block[nCntBlock].pos.y
					&&ePos->y >= g_Block[nCntBlock].pos.y)
				{//上からめり込んだ
					bLand = true;
					ePos->y = g_Block[nCntBlock].pos.y;
					eMove->y = 0.0f;
				}
				else if (ePosOld->y - fHeight >= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight
					&& ePos->y - fHeight <= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight)
				{//下からめりこんだ。
					bLand = true;
					ePos->y = g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight + fHeight;
					eMove->y = 0.0f;
				}
			}
			//ブロックの縦幅
			if (//頭から足が壁にぶつかった
				ePosOld->y - fHeight <= g_Block[nCntBlock].pos.y
				&&ePosOld->y >= g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight
				//足が壁にぶつかった
				|| ePosOld->y - fHeight > g_Block[nCntBlock].pos.y
				&&ePosOld->y - fHeight < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight
				//頭が壁にぶつかった
				|| ePosOld->y > g_Block[nCntBlock].pos.y
				&&ePosOld->y < g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight)
			{
				//左からめり込んだ
				if (ePosOld->x <= g_Block[nCntBlock].pos.x
					&& ePos->x + fWidth / 2 >= g_Block[nCntBlock].pos.x)
				{
					ePos->x = g_Block[nCntBlock].pos.x - fWidth / 2 - 1;
					eMove->x = 0.0f;
				}
				//右からめり込んだ
				else if (ePosOld->x > g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth
					&&ePos->x - fWidth / 2 < g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth)
				{
					ePos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + fWidth / 2 + 1;
					eMove->x = 0.0f;
				}
			}
		}
	}
	return bLand;
}

/*if (g_Block[nCntBlock].pos.y -g_Block[nCntBlock].fHeight< pPos->y +20
			&&g_Block[nCntBlock].pos.x - g_Block[nCntBlock].fWidth< pPos->x + 10
			&&g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth > pPos->x - 10
			&&g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight > pPos->y)
		{
			bLand = true;
			pPos->y = g_Block[nCntBlock].pos.y - g_Block[nCntBlock].fHeight-20;
			pMove->y = 0.0f;
		}*/
		//頭がぶつかったとき
		//else if (g_Block[nCntBlock].pos.y - g_Block[nCntBlock].fHeight< pPos->y
		//	&&g_Block[nCntBlock].pos.x - g_Block[nCntBlock].fWidth< pPos->x + 10
		//	&&g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth > pPos->x - 10
		//	&&g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight > pPos->y-40)
		//{
		//	bLand = true;
		//	pPos->y = g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight+40;
		//	pMove->y = 0.0f;
		//}

		////ブロック左に衝突時
		//else if (g_Block[nCntBlock].pos.y - g_Block[nCntBlock].fHeight< pPos->y+10
		//	&&g_Block[nCntBlock].pos.x - g_Block[nCntBlock].fWidth< pPos->x+20
		//	&&g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth > pPos->x
		//	&&g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight > pPos->y-30)
		//{
		//	bLand = true;
		//	pPos->x = g_Block[nCntBlock].pos.x - g_Block[nCntBlock].fWidth -15;
		//	pMove->x = 0.0f;
		//}

		////ブロック右に衝突時
		//else if (g_Block[nCntBlock].pos.y - g_Block[nCntBlock].fHeight< pPos->y + 10
		//	&& g_Block[nCntBlock].pos.x - g_Block[nCntBlock].fWidth< pPos->x 
		//	&& g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth > pPos->x - 20
		//	&&g_Block[nCntBlock].pos.y + g_Block[nCntBlock].fHeight > pPos->y - 30)
		//{
		//	bLand = true;
		//	pPos->x = g_Block[nCntBlock].pos.x + g_Block[nCntBlock].fWidth + 15;
		//	pMove->x = 0.0f;
		//}