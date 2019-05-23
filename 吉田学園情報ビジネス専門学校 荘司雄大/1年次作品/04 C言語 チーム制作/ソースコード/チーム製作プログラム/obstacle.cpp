//=============================================================================
//
// 障害物の処理処理 [obstacle.cpp]
// Author : 荘司雄大
//
//=============================================================================
#include "obstacle.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "shadow.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define OBSTACLE_NAME0	"data\\MODEL\\Bill\\pizzaya.x"			//テクスチャのファイル名
#define OBSTACLE_NAME1	"data\\MODEL\\Bill\\pizzaya横.x"		//テクスチャのファイル名
#define OBSTACLE_NAME2	"data\\MODEL\\Bill\\house000.x"			//テクスチャのファイル名
#define OBSTACLE_NAME3	"data\\MODEL\\Bill\\house000横.x"		//テクスチャのファイル名
#define OBSTACLE_NAME4	"data\\MODEL\\Bill\\house001.x"			//テクスチャのファイル名	 
#define OBSTACLE_NAME5	"data\\MODEL\\Bill\\house001横.x"		//テクスチャのファイル名	 
#define OBSTACLE_NAME6	"data\\MODEL\\Bill\\house002.x"			//テクスチャのファイル名
#define OBSTACLE_NAME7	"data\\MODEL\\Bill\\tatemono001.x"		//テクスチャのファイル名
#define OBSTACLE_NAME8	"data\\MODEL\\Bill\\tatemono001横.x"	//テクスチャのファイル名
#define OBSTACLE_NAME9	"data\\MODEL\\Bill\\tatemono002.x"		//テクスチャのファイル名
#define OBSTACLE_NAME10	"data\\MODEL\\Bill\\tatemono002横.x"	//テクスチャのファイル名
#define OBSTACLE_NAME11	"data\\MODEL\\Bill\\tatemono003.x"		//テクスチャのファイル名
#define OBSTACLE_NAME12	"data\\MODEL\\Bill\\tatemono003横.x"	//テクスチャのファイル名	 
#define OBSTACLE_NAME13	"data\\MODEL\\Bill\\tatemono004.x"		//テクスチャのファイル名
#define OBSTACLE_NAME14	"data\\MODEL\\Bill\\tatemono004横.x"	//テクスチャのファイル名
#define OBSTACLE_NAME15	"data\\MODEL\\Bill\\tatemono005.x"		//テクスチャのファイル名	
#define OBSTACLE_NAME16	"data\\MODEL\\Bill\\tatemono005横.x"	//テクスチャのファイル名	 
#define OBSTACLE_NAME17	"data\\MODEL\\Bill\\tatemono006.x"		//テクスチャのファイル名
#define OBSTACLE_NAME18	"data\\MODEL\\Bill\\tatemono006横.x"	//テクスチャのファイル名
#define OBSTACLE_NAME19	"data\\MODEL\\Bill\\tatemono008.x"		//テクスチャのファイル名
#define OBSTACLE_NAME20	"data\\MODEL\\Bill\\tatemono008横.x"	//テクスチャのファイル名
#define OBSTACLE_NAME21	"data\\MODEL\\Bill\\haikyo.x"			//テクスチャのファイル名
#define OBSTACLE_NAME22	"data\\MODEL\\Bill\\pokkuruzou.x"		//テクスチャのファイル名
#define OBSTACLE_NAME23	"data\\MODEL\\Bill\\hospital.x"			//テクスチャのファイル名
#define OBSTACLE_NAME24	"data\\MODEL\\Bill\\hospital横.x"		//テクスチャのファイル名
#define OBSTACLE_NAME25 "data\\MODEL\\Bill\\shinden.x"			//テクスチャのファイル名
#define OBSTACLE_NAME26	"data\\MODEL\\Bill\\shinden横.x"		//テクスチャのファイル名
#define OBSTACLE_NAME27	"data\\MODEL\\Bill\\skytree.x"			//テクスチャのファイル名
#define OBSTACLE_NAME28	"data\\MODEL\\Bill\\pizzanosyatou.x"	//テクスチャのファイル名
#define OBSTACLE_NAME29	"data\\MODEL\\Bill\\オフィスビル.x"		//テクスチャのファイル名
#define OBSTACLE_NAME30	"data\\MODEL\\Bill\\オフィスビル小.x"	//テクスチャのファイル名
#define OBSTACLE_NAME31	"data\\MODEL\\Bill\\オフィスビル小横.x"	//テクスチャのファイル名
#define OBSTACLE_NAME32	"data\\MODEL\\Bill\\オフィスビル大.x"	//テクスチャのファイル名
#define OBSTACLE_NAME33	"data\\MODEL\\Bill\\オフィスビル大横.x"	//テクスチャのファイル名
#define OBSTACLE_NAME34	"data\\MODEL\\Bill\\kaminarimon.x"		//テクスチャのファイル名
#define OBSTACLE_NAME35	"data\\MODEL\\Bill\\kaminarimon横.x"	//テクスチャのファイル名
#define OBSTACLE_NAME36	"data\\MODEL\\Bill\\工事車.x"					//テクスチャのファイル名
#define OBSTACLE_NAME37	"data\\MODEL\\Bill\\禁止エリアゾーン.x"			//テクスチャのファイル名
#define OBSTACLE_NAME38	"data\\MODEL\\Bill\\禁止エリアゾーンyoko.x"		//テクスチャのファイル名
#define OBSTACLE_NAME39	"data\\MODEL\\Bill\\工事員.x"					//テクスチャのファイル名
#define OBSTACLE_NAME40	"data\\MODEL\\Bill\\工事員yoko.x"				//テクスチャのファイル名
#define OBSTACLE_NAME41	"data\\MODEL\\Bill\\バリスタ.x"					//テクスチャのファイル名
#define OBSTACLE_NAME42	"data\\MODEL\\Bill\\バリスタyoko.x"				//テクスチャのファイル名
#define OBSTACLE_NAME43	"data\\MODEL\\Bill\\博士ハウス.x"				//テクスチャのファイル名
#define OBSTACLE_NAME44	"data\\MODEL\\Bill\\博士ハウスyoko.x"			//テクスチャのファイル名
#define OBSTACLE_NAME45	"data\\MODEL\\Bill\\博士.x"						//テクスチャのファイル名
#define OBSTACLE_NAME46	"data\\MODEL\\Bill\\博士yoko.x"					//テクスチャのファイル名
#define OBSTACLE_NAME47	"data\\MODEL\\Bill\\牛.x"						//テクスチャのファイル名
#define OBSTACLE_NAME48	"data\\MODEL\\Bill\\柵.x"						//テクスチャのファイル名
#define OBSTACLE_NAME49	"data\\MODEL\\Bill\\柵yoko.x"					//テクスチャのファイル名
#define OBSTACLE_NAME50 "data\\MODEL\\Bill\\たまごっち.x"				//テクスチャのファイル名
#define OBSTACLE_NAME51	"data\\MODEL\\Bill\\たまごっちyoko.x"			//テクスチャのファイル名
#define OBSTACLE_NAME52	"data\\MODEL\\Bill\\バスターソード.x"			//テクスチャのファイル名
#define OBSTACLE_NAME53	"data\\MODEL\\Bill\\バスターソードyoko.x"		//テクスチャのファイル名
#define OBSTACLE_NAME54	"data\\MODEL\\Bill\\hana.x"						//テクスチャのファイル名
#define OBSTACLE_NAME55	"data\\MODEL\\Bill\\hanayoko.x"					//テクスチャのファイル名
#define OBSTACLE_NAME56	"data\\MODEL\\Bill\\TENGA.x"					//テクスチャのファイル名
#define OBSTACLE_NAME57	"data\\MODEL\\Bill\\mm.x"						//テクスチャのファイル名
#define OBSTACLE_NAME58	"data\\MODEL\\Bill\\会社.x"						//テクスチャのファイル名
#define OBSTACLE_NAME59	"data\\MODEL\\Bill\\土.x"						//テクスチャのファイル名
#define OBSTACLE_NAME60	"data\\MODEL\\Bill\\pokutank.x"					//テクスチャのファイル名
#define OBSTACLE_NAME61	"data\\MODEL\\Bill\\poptank.x"					//テクスチャのファイル名

#define OBSTACLE_SIZE		(5.0f)							// モデルの大きさ
#define MAX_OBSTACLE		(100)							// オブジェクトの最大数
#define MAX_OBSTACLE_TYPE   (OBSTACLE_TYPE_MAX )			// オブジェクトのタイプ数


//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPD3DXMESH		    g_pMeshObstacle[MAX_OBSTACLE_TYPE];	              // メッシュ情報（頂点情報）へのポインタ
LPD3DXBUFFER	    g_pBuffMatObstacle[MAX_OBSTACLE_TYPE];	          // マテリアル情報へのポインタ
LPDIRECT3DTEXTURE9* g_pTextureObstacle[MAX_OBSTACLE_TYPE];            // テクスチャ情報へのポインタ
DWORD			    g_nNumMatObstacle[MAX_OBSTACLE_TYPE] = {};		  // マテリアル情報の数
OBSTACLE	        g_aObstacle[MAX_OBSTACLE_TYPE][MAX_OBSTACLE];     // 障害物の情報
int g_nMoveTimer;
//=============================================================================
// 初期化処理
//=============================================================================
void InitObstacle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	g_nMoveTimer = 0;
	char aXFileName[MAX_OBSTACLE_TYPE][64] = { 
		OBSTACLE_NAME0,
		OBSTACLE_NAME1,
		OBSTACLE_NAME2,
		OBSTACLE_NAME3,
		OBSTACLE_NAME4,
		OBSTACLE_NAME5,
		OBSTACLE_NAME6,
		OBSTACLE_NAME7,
		OBSTACLE_NAME8,
		OBSTACLE_NAME9,
		OBSTACLE_NAME10,
		OBSTACLE_NAME11,
		OBSTACLE_NAME12,
		OBSTACLE_NAME13,
		OBSTACLE_NAME14,
		OBSTACLE_NAME15,
		OBSTACLE_NAME16,
		OBSTACLE_NAME17,
		OBSTACLE_NAME18,
		OBSTACLE_NAME19,
		OBSTACLE_NAME20,
		OBSTACLE_NAME21,
		OBSTACLE_NAME22,
		OBSTACLE_NAME23,
		OBSTACLE_NAME24,
		OBSTACLE_NAME25,
		OBSTACLE_NAME26,
		OBSTACLE_NAME27,
		OBSTACLE_NAME28,
		OBSTACLE_NAME29,
		OBSTACLE_NAME30,
		OBSTACLE_NAME31,
		OBSTACLE_NAME32,
		OBSTACLE_NAME33,
		OBSTACLE_NAME34,
		OBSTACLE_NAME35,
		OBSTACLE_NAME36,
		OBSTACLE_NAME37,
		OBSTACLE_NAME38,
		OBSTACLE_NAME39,
		OBSTACLE_NAME40,
		OBSTACLE_NAME41,
		OBSTACLE_NAME42,
		OBSTACLE_NAME43,
		OBSTACLE_NAME44,
		OBSTACLE_NAME45,
		OBSTACLE_NAME46,
		OBSTACLE_NAME47,
		OBSTACLE_NAME48,
		OBSTACLE_NAME49,
		OBSTACLE_NAME50,
		OBSTACLE_NAME51,
		OBSTACLE_NAME52,
		OBSTACLE_NAME53,
		OBSTACLE_NAME54,
		OBSTACLE_NAME55,
		OBSTACLE_NAME56,
		OBSTACLE_NAME57,
		OBSTACLE_NAME58,
		OBSTACLE_NAME59,
		OBSTACLE_NAME60,
		OBSTACLE_NAME61,

	};

	for (int nCntType = 0; nCntType < MAX_OBSTACLE_TYPE; nCntType++)
	{// 障害物の種類だけ繰り返し
	 // Xファイルの読み込み
		D3DXLoadMeshFromX(aXFileName[nCntType],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_pBuffMatObstacle[nCntType],
			NULL,
			&g_nNumMatObstacle[nCntType],
			&g_pMeshObstacle[nCntType]);

		// マテリアル情報からテクスチャを引き出す
		D3DXMATERIAL* pMat = (D3DXMATERIAL*)g_pBuffMatObstacle[nCntType]->GetBufferPointer();
		D3DMATERIAL9* pMatBuff = new D3DMATERIAL9[g_nNumMatObstacle[nCntType]];
		g_pTextureObstacle[nCntType] = new LPDIRECT3DTEXTURE9[g_nNumMatObstacle[nCntType]];

		for (DWORD nCntMat = 0; nCntMat < g_nNumMatObstacle[nCntType]; nCntMat++)
		{// 頂点数の数だけ繰り返し 
		 // マテリアル情報を読み込む
			pMatBuff[nCntMat] = pMat[nCntMat].MatD3D;

			// 環境光を初期化する
			pMatBuff[nCntMat].Ambient = pMatBuff[nCntMat].Diffuse;

			// テクスチャ情報を初期化
			g_pTextureObstacle[nCntType][nCntMat] = NULL;

			// テクスチャの情報を読み込む
			if (pMat[nCntMat].pTextureFilename != NULL &&
				lstrlen(pMat[nCntMat].pTextureFilename) > 0)
			{// テクスチャのファイル名がある
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_pTextureObstacle[nCntType][nCntMat]);
			}
		}

		for (int nCntObstacle = 0; nCntObstacle < MAX_OBSTACLE; nCntObstacle++)
		{// 障害物の数だけ繰り返し
		 // 障害物情報の初期化
			g_aObstacle[nCntType][nCntObstacle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 座標を初期化
			g_aObstacle[nCntType][nCntObstacle].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向きをを初期化
			g_aObstacle[nCntType][nCntObstacle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 移動量をを初期化
			g_aObstacle[nCntType][nCntObstacle].nType = (OBSTACLE_TYPE)nCntType;		// 種類をを初期化
			g_aObstacle[nCntType][nCntObstacle].bUse = false;							// 使用していない状態にする

			if (nCntObstacle == 0)
			{// 1回目だったら
				int nNumVtx;	 // 頂点数
				DWORD sizeFVF;	 // 頂点フォーマットのサイズ
				BYTE *pVtxBuff;  // 頂点バッファへのポインタ

				// 最小値の設定
				g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
				g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);

				// 頂点数を取得
				nNumVtx = g_pMeshObstacle[nCntType]->GetNumVertices();

				// 頂点フォーマットのサイズを取得
				sizeFVF = D3DXGetFVFVertexSize(g_pMeshObstacle[nCntType]->GetFVF());

				// 頂点バッファをロック
				g_pMeshObstacle[nCntType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

				for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
				{// モデルの頂点数だけ繰り返し
					D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;	// 頂点座標の代入

					if (vtx.x < g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x)
					{// X座標がより小さい
						g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x = vtx.x;
					}
					if (vtx.y < g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.y)
					{// Y座標がより小さい
						g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.y = vtx.y;
					}
					if (vtx.z < g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z)
					{// Z座標がより小さい
						g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z = vtx.z;
					}

					if (vtx.x > g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x)
					{// X座標がより大きい
						g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x = vtx.x;
					}
					if (vtx.y > g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.y)
					{// Y座標がより大きい
						g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.y = vtx.y;
					}
					if (vtx.z > g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z)
					{// Z座標がより大きい
						g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z = vtx.z;
					}

					pVtxBuff += sizeFVF;	// サイズ分ポインタを進める
				}

				// 頂点バッファをアンロック
				g_pMeshObstacle[nCntType]->UnlockVertexBuffer();
			}
			else
			{// それ以降
				g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle = g_aObstacle[nCntType][nCntObstacle - 1].vtxMaxObstacle;
				g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle = g_aObstacle[nCntType][nCntObstacle - 1].vtxMinObstacle;
			}
		}
	}
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitObstacle(void)
{
	for (int nCntType = 0; nCntType < MAX_OBSTACLE_TYPE; nCntType++)
	{
		// メッシュの開放
		if (g_pMeshObstacle[nCntType] != NULL)
		{
			g_pMeshObstacle[nCntType]->Release();
			g_pMeshObstacle[nCntType] = NULL;
		}
		// マテリアルの開放
		if (g_pBuffMatObstacle[nCntType] != NULL)
		{
			g_pBuffMatObstacle[nCntType]->Release();
			g_pBuffMatObstacle[nCntType] = NULL;
		}
		// テクスチャの開放
		for (DWORD nCntMat = 0; nCntMat < g_nNumMatObstacle[nCntType]; nCntMat++)
		{
			if (g_pTextureObstacle[nCntType][nCntMat] != NULL)
			{
				g_pTextureObstacle[nCntType][nCntMat]->Release();
				g_pTextureObstacle[nCntType][nCntMat] = NULL;
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateObstacle(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();
	Camera *pCamera;
	pCamera = GetCamera();
	
	int nCntCollision = 0;
	int nTrueObj = 0;
	//現在のモードを取得
	MODE Mode = GetMode();

	switch (Mode)
	{
	case MODE_GAME:
		for (int nCntType = 0; nCntType < MAX_OBSTACLE_TYPE; nCntType++)
		{// 障害物の種類の数だけ繰り返し
			for (int nCntObstacle = 0; nCntObstacle < MAX_OBSTACLE; nCntObstacle++)
			{// 障害物の数だけ繰り返し
				if (g_aObstacle[nCntType][nCntObstacle].bUse == true)
				{// 障害物が使用されている
					nTrueObj++;
					//カメラとの距離を求める
					double length = pow((g_aObstacle[nCntType][nCntObstacle].pos.x - pCamera->posV.x)
						*(g_aObstacle[nCntType][nCntObstacle].pos.x - pCamera->posV.x)
						+ (g_aObstacle[nCntType][nCntObstacle].pos.z - pCamera->posV.z)
						*(g_aObstacle[nCntType][nCntObstacle].pos.z - pCamera->posV.z), 0.5);

					//float型に変換
					g_aObstacle[nCntType][nCntObstacle].fLength = (float)length;

					//プレイヤーとの距離を求める
					double lengthPlayer = pow((g_aObstacle[nCntType][nCntObstacle].pos.x - pPlayer->pos.x)
						*(g_aObstacle[nCntType][nCntObstacle].pos.x - pPlayer->pos.x)
						+ (g_aObstacle[nCntType][nCntObstacle].pos.z - pPlayer->pos.z)
						*(g_aObstacle[nCntType][nCntObstacle].pos.z - pPlayer->pos.z), 0.5);
					//float型に変換
					g_aObstacle[nCntType][nCntObstacle].fLengthPlayer = (float)lengthPlayer;

					//プレイヤーとの距離を求める
					double lengthMaxX = pow(((g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x  )- pPlayer->vtxMinBike.x)
						*((g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x) - pPlayer->vtxMinBike.x)
						+(g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z - pPlayer->vtxMinBike.z)
						*(g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z - pPlayer->vtxMinBike.z), 0.5);
					//float型に変換
					g_aObstacle[nCntType][nCntObstacle].LengthMax.x = (float)lengthMaxX;

					//プレイヤーとの距離を求める
					double lengthMinX = pow(((g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x) - pPlayer->vtxMaxBike.x)
						*((g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x) - pPlayer->vtxMaxBike.x)
						+ (g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z - pPlayer->vtxMaxBike.z)
						*(g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z - pPlayer->vtxMaxBike.z), 0.5);
					//float型に変換
					g_aObstacle[nCntType][nCntObstacle].LengthMin.x = (float)lengthMinX;

					//if (g_aObstacle[nCntType][nCntObstacle].LengthMax.x > 200)
					//{
					//	nCntCollision++;
					//	if (nCntCollision > 100)
					//	{
					//		pPlayer->ChangeState = CHANGESTATE_POSSIBLE;
					//	}
					//}

					g_aObstacle[nCntType][nCntObstacle].vtxMaxBike.x = (pPlayer->pos.x + pPlayer->vtxMaxBike.x) - (g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x);
					g_aObstacle[nCntType][nCntObstacle].vtxMinBike.x = (pPlayer->pos.x + pPlayer->vtxMinBike.x) - (g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x);
					g_aObstacle[nCntType][nCntObstacle].vtxMaxBike.z = (pPlayer->pos.z + pPlayer->vtxMaxBike.z) - (g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z);
					g_aObstacle[nCntType][nCntObstacle].vtxMinBike.z = (pPlayer->pos.z + pPlayer->vtxMinBike.z) - (g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z);

					////プレイヤーと最大頂点の距離を求める
					//double dLengthMin = pow((g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x - pPlayer->pos.x)*(g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x - pPlayer->pos.x)
					//	+ (g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z - pPlayer->pos.z)*(g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z - pPlayer->pos.z), 0.5);
					////float型に変換
					//float fLengthMin = (float)dLengthMin;

					//if (fLengthMax < 300 && fLengthMin < 300)
					//{
					//	pPlayer->ChangeState = CHANGESTATE_IMPOSSIBLE;
					//}


					if (pPlayer->pos.z - pPlayer->vtxMaxBike.z <= g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z && pPlayer->pos.z + pPlayer->vtxMaxBike.z >= g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z)
					{// 障害物のZ座標の中にいる
						if (pPlayer->posOld.x + pPlayer->vtxMaxBike.x <= g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x
							&& pPlayer->pos.x + pPlayer->vtxMaxBike.x > g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x)
						{// X座標の中に左から入った
							pPlayer->ChangeState = CHANGESTATE_IMPOSSIBLE;
						}
						else if (pPlayer->posOld.x - pPlayer->vtxMaxBike.x >= g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x
							&& pPlayer->pos.x - pPlayer->vtxMaxBike.x < g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x)
						{// X座標の中に右から入った
							pPlayer->ChangeState = CHANGESTATE_IMPOSSIBLE;
						}
					}
					if (pPlayer->pos.x - pPlayer->vtxMaxBike.x <= g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x && pPlayer->pos.x + pPlayer->vtxMaxBike.x >= g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x)
					{// 障害物のX座標の中にいる
						if (pPlayer->posOld.z + pPlayer->vtxMaxBike.z <= g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z
							&& pPlayer->pos.z + pPlayer->vtxMaxBike.z > g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z)
						{// Z座標の中に前から入った
							pPlayer->ChangeState = CHANGESTATE_IMPOSSIBLE;
						}
						else if (pPlayer->posOld.z - pPlayer->vtxMaxBike.z >= g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z
							&& pPlayer->pos.z - pPlayer->vtxMaxBike.z < g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z)
						{// Z座標の中に後ろから入った
							pPlayer->ChangeState = CHANGESTATE_IMPOSSIBLE;
						}
					}
				}
			}
		}
		for (int nCntType = 0; nCntType < MAX_OBSTACLE_TYPE; nCntType++)
		{// 障害物の種類の数だけ繰り返し
			for (int nCntObstacle = 0; nCntObstacle < MAX_OBSTACLE; nCntObstacle++)
			{// 障害物の数だけ繰り返し
				if (g_aObstacle[nCntType][nCntObstacle].bUse == true)
				{// 障害物が使用されている

					if (g_aObstacle[nCntType][nCntObstacle].fLengthPlayer > 300 /*|| g_aObstacle[nCntType][nCntObstacle].LengthMin.x < 80*/)
					{
						nCntCollision++;
						if (nCntCollision == nTrueObj)
						{							
							pPlayer->ChangeState = CHANGESTATE_POSSIBLE;
						}
					}
				}
			}
		}

		break;
	case MODE_TITLE:
		for (int nCntType = 0; nCntType < MAX_OBSTACLE_TYPE; nCntType++)
		{// 障害物の種類の数だけ繰り返し
			for (int nCntObstacle = 0; nCntObstacle < MAX_OBSTACLE; nCntObstacle++)
			{// 障害物の数だけ繰り返し
				if (g_aObstacle[nCntType][nCntObstacle].bUse == true)
				{// 障害物が使用されている
				 //カメラとの距離を求める
					double length = pow((g_aObstacle[nCntType][nCntObstacle].pos.x - pCamera->posV.x)*(g_aObstacle[nCntType][nCntObstacle].pos.x - pCamera->posV.x)
						+ (g_aObstacle[nCntType][nCntObstacle].pos.z - pCamera->posV.z)*(g_aObstacle[nCntType][nCntObstacle].pos.z - pCamera->posV.z), 0.5);
					//float型に変換
					g_aObstacle[nCntType][nCntObstacle].fLength = (float)length;
				}
			}
		}
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawObstacle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	//現在のモードを取得
	MODE Mode = GetMode();


	for (int nCntType = 0; nCntType< MAX_OBSTACLE_TYPE; nCntType++)
	{// 障害物の種類の数だけ繰り返し
		for (int nCntObstacle = 0; nCntObstacle < MAX_OBSTACLE; nCntObstacle++)
		{// 障害物の数だけ繰り返し
			if (g_aObstacle[nCntType][nCntObstacle].bUse == true)
			{// 障害物が使用されている
				switch (Mode)
				{
				case MODE_GAME:
					if (g_aObstacle[nCntType][nCntObstacle].fLength < 2100)
					{//モデルとプレイヤーの距離で描画するか決める

						// ワールドマトリックスの初期化
						D3DXMatrixIdentity(&g_aObstacle[nCntType][nCntObstacle].mtxWorld);

						// 回転を反映
						D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObstacle[nCntType][nCntObstacle].rot.y, g_aObstacle[nCntType][nCntObstacle].rot.x, g_aObstacle[nCntType][nCntObstacle].rot.z);
						D3DXMatrixMultiply(&g_aObstacle[nCntType][nCntObstacle].mtxWorld, &g_aObstacle[nCntType][nCntObstacle].mtxWorld, &mtxRot);

						// 位置を反映
						D3DXMatrixTranslation(&mtxTrans, g_aObstacle[nCntType][nCntObstacle].pos.x, g_aObstacle[nCntType][nCntObstacle].pos.y, g_aObstacle[nCntType][nCntObstacle].pos.z);
						D3DXMatrixMultiply(&g_aObstacle[nCntType][nCntObstacle].mtxWorld, &g_aObstacle[nCntType][nCntObstacle].mtxWorld, &mtxTrans);

						// ワールドマトリックスの設定
						pDevice->SetTransform(D3DTS_WORLD, &g_aObstacle[nCntType][nCntObstacle].mtxWorld);

						// 現在のマテリアルを取得
						pDevice->GetMaterial(&matDef);

						// マテリアルデータへのポインタを取得
						pMat = (D3DXMATERIAL*)g_pBuffMatObstacle[nCntType]->GetBufferPointer();
						for (int nCntMat = 0; nCntMat < (int)g_nNumMatObstacle[nCntType]; nCntMat++)
						{
							// マテリアルの設定
							pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

							// テクスチャの設定
							pDevice->SetTexture(0, g_pTextureObstacle[nCntType][nCntMat]);

							// モデル(パーツ)の描画
							g_pMeshObstacle[nCntType]->DrawSubset(nCntMat);
						}
						// マテリアルをデフォルトに戻す
						pDevice->SetMaterial(&matDef);
					}
					break;
				case MODE_TITLE:
					if (g_aObstacle[nCntType][nCntObstacle].fLength < 2300)
					{//モデルとプレイヤーの距離で描画するか決める
					// ワールドマトリックスの初期化
						D3DXMatrixIdentity(&g_aObstacle[nCntType][nCntObstacle].mtxWorld);

						// 回転を反映
						D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aObstacle[nCntType][nCntObstacle].rot.y, g_aObstacle[nCntType][nCntObstacle].rot.x, g_aObstacle[nCntType][nCntObstacle].rot.z);
						D3DXMatrixMultiply(&g_aObstacle[nCntType][nCntObstacle].mtxWorld, &g_aObstacle[nCntType][nCntObstacle].mtxWorld, &mtxRot);

						// 位置を反映
						D3DXMatrixTranslation(&mtxTrans, g_aObstacle[nCntType][nCntObstacle].pos.x, g_aObstacle[nCntType][nCntObstacle].pos.y, g_aObstacle[nCntType][nCntObstacle].pos.z);
						D3DXMatrixMultiply(&g_aObstacle[nCntType][nCntObstacle].mtxWorld, &g_aObstacle[nCntType][nCntObstacle].mtxWorld, &mtxTrans);

						// ワールドマトリックスの設定
						pDevice->SetTransform(D3DTS_WORLD, &g_aObstacle[nCntType][nCntObstacle].mtxWorld);

						// 現在のマテリアルを取得
						pDevice->GetMaterial(&matDef);

						// マテリアルデータへのポインタを取得
						pMat = (D3DXMATERIAL*)g_pBuffMatObstacle[nCntType]->GetBufferPointer();
						for (int nCntMat = 0; nCntMat < (int)g_nNumMatObstacle[nCntType]; nCntMat++)
						{
							// マテリアルの設定
							pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

							// テクスチャの設定
							pDevice->SetTexture(0, g_pTextureObstacle[nCntType][nCntMat]);

							// モデル(パーツ)の描画
							g_pMeshObstacle[nCntType]->DrawSubset(nCntMat);
						}
						// マテリアルをデフォルトに戻す
						pDevice->SetMaterial(&matDef);
					}
				}
			}
		}
	}
}
//=============================================================================
// 障害物の設定処理
//=============================================================================
void SetObstacle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	for (int nCntObstacle = 0; nCntObstacle < MAX_OBSTACLE; nCntObstacle++)
	{// 障害物の数だけ繰り返し
		if (g_aObstacle[nType][nCntObstacle].bUse == false)
		{// 障害物が使用されていない
		 // 位置・向きの初期設定
			g_aObstacle[nType][nCntObstacle].pos = pos;
			g_aObstacle[nType][nCntObstacle].rot = rot;
			//　影のセット
			//g_aModel[nCntModel].nIdxShadow = SetShadow(g_aModel[nCntModel].pos, D3DXVECTOR3(0, 0, 0), 20.0f);
			//g_aObstacle[nType][nCntObstacle].nIdxShadow = SetShadow(g_aObstacle[nType][nCntObstacle].pos, D3DXVECTOR3(0, 0, 0), 20.0f);
			g_aObstacle[nType][nCntObstacle].bUse = true;
			//g_nNumObstacle++;
			break;
		}
	}

}

//=============================================================================
// 障害物の取得
//=============================================================================
OBSTACLE *GetObstacle(void)
{
	return &g_aObstacle[0][0];
}

//=============================================================================
// 障害物の当たり判定
//=============================================================================
bool CollisionObstacle(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 radius)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	bool bLand = false;  // 乗ったかどうか

	for (int nCntType = 0; nCntType < MAX_OBSTACLE_TYPE; nCntType++)
	{// 障害物の種類だけ繰り返し
		for (int nCntObstacle = 0; nCntObstacle < MAX_OBSTACLE; nCntObstacle++)
		{// 障害物の数だけ繰り返し
			if (g_aObstacle[nCntType][nCntObstacle].bUse == true)
			{// 障害物が使用されている
				if (pPos->y <= g_aObstacle[nCntType][nCntObstacle].pos.y + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.y && pPos->y + radius.y > g_aObstacle[nCntType][nCntObstacle].pos.y + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.y
					|| pPos->y + radius.y > g_aObstacle[nCntType][nCntObstacle].pos.y + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.y && pPos->y < g_aObstacle[nCntType][nCntObstacle].pos.y + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.y
					|| pPos->y + radius.y > g_aObstacle[nCntType][nCntObstacle].pos.y + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.y && pPos->y < g_aObstacle[nCntType][nCntObstacle].pos.y + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.y)
				{// 障害物のY座標の中にいる
					if (pPos->z - radius.z <= g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z && pPos->z + radius.z >= g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z)
					{// 障害物のZ座標の中にいる
						if (pPosOld->x + radius.x <= g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x
							&& pPos->x + radius.x > g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x)
						{// X座標の中に左から入った
							pPos->x = g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x - radius.x;
							pMove->x = 0.0f;
							bLand = true;
						}
						else if (pPosOld->x - radius.x >= g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x
							&& pPos->x - radius.x < g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x)
						{// X座標の中に右から入った
							pPos->x = g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x + radius.x + 0.1f;
							pMove->x = 0.0f;
							bLand = true;
						}
					}
					if (pPos->x - radius.x <= g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x && pPos->x + radius.x >= g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x)
					{// 障害物のX座標の中にいる
						if (pPosOld->z + radius.z <= g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z
							&& pPos->z + radius.z > g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z)
						{// Z座標の中に前から入った
							pPos->z = g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z - radius.z;
							pMove->z = 0.0f;
							bLand = true;
						}
						else if (pPosOld->z - radius.z >= g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z
							&& pPos->z - radius.z < g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z)
						{// Z座標の中に後ろから入った
							pPos->z = g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z + radius.z + 0.1f;
							pMove->z = 0.0f;
							bLand = true;
						}
					}
				}
			}
		}
	}

	return bLand;   // 乗ったかどうかを返す
}

//=============================================================================
// 障害物のバイク用の当たり判定
//=============================================================================
bool CollisionObstacleBike(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 radius)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	bool bLand = false;  // 乗ったかどうか

	for (int nCntType = 0; nCntType < MAX_OBSTACLE_TYPE; nCntType++)
	{// 障害物の種類だけ繰り返し
		for (int nCntObstacle = 0; nCntObstacle < MAX_OBSTACLE; nCntObstacle++)
		{// 障害物の数だけ繰り返し
			if (g_aObstacle[nCntType][nCntObstacle].bUse == true)
			{// 障害物が使用されている
				if (pPos->y <= g_aObstacle[nCntType][nCntObstacle].pos.y + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.y && pPos->y + radius.y > g_aObstacle[nCntType][nCntObstacle].pos.y + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.y
					|| pPos->y + radius.y > g_aObstacle[nCntType][nCntObstacle].pos.y + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.y && pPos->y < g_aObstacle[nCntType][nCntObstacle].pos.y + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.y
					|| pPos->y + radius.y > g_aObstacle[nCntType][nCntObstacle].pos.y + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.y && pPos->y < g_aObstacle[nCntType][nCntObstacle].pos.y + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.y)
				{// 障害物のY座標の中にいる
					if (pPos->z - radius.z <= g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z && pPos->z + radius.z >= g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z)
					{// 障害物のZ座標の中にいる
						if (pPosOld->x + radius.x <= g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x
							&& pPos->x + radius.x > g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x)
						{// X座標の中に左から入った
							bLand = true;
						}
						else if (pPosOld->x - radius.x >= g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x
							&& pPos->x - radius.x < g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x)
						{// X座標の中に右から入った
							bLand = true;
						}
					}
					if (pPos->x - radius.x <= g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.x && pPos->x + radius.x >= g_aObstacle[nCntType][nCntObstacle].pos.x + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.x)
					{// 障害物のX座標の中にいる
						if (pPosOld->z + radius.z <= g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z
							&& pPos->z + radius.z > g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMinObstacle.z)
						{// Z座標の中に前から入った	
							bLand = true;
						}
						else if (pPosOld->z - radius.z >= g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z
							&& pPos->z - radius.z < g_aObstacle[nCntType][nCntObstacle].pos.z + g_aObstacle[nCntType][nCntObstacle].vtxMaxObstacle.z)
						{// Z座標の中に後ろから入った					
							bLand = true;
						}
					}
				}
			}
		}
	}
	return bLand;   // 乗ったかどうかを返す
}