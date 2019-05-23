//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "effect.h"
#include "lifegauge.h"
#include "ultgauge.h"
#include "game.h"
#include "result.h"
#include "sound.h"
#include "Rush.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MODEL_FILENAME   "MODEL_FILENAME"
#define NUM_MODEL        "NUM_MODEL"
#define CHARACTERSET     "CHARACTERSET"
#define MOVE             "MOVE"
#define JUMP             "JUMP"
#define INDEX            "INDEX"
#define PARENT           "PARENT"
#define POS              "POS"
#define ROT              "ROT"
#define PARTSSET         "PARTSSET"
#define END_PARTSSET     "END_PARTSSET"
#define END_CHARACTERSET "END_CHARACTERSET"
#define MOTIONSET        "MOTIONSET"
#define LOOP             "LOOP"
#define NUM_KEY          "NUM_KEY"
#define KEYSET           "KEYSET"
#define FRAME            "FRAME"
#define KEY              "KEY"
#define END_KEY          "END_KEY"
#define END_KEYSET       "END_KEYSET"
#define END_MOTIONSET    "END_MOTIONSET"


#define PLAYER_INERTIA_NORMAL (0.3f)     // 通常時の慣性
#define PLAYER_INERTIA_JUMP   (0.012f)   // ジャンプ中の慣性
#define GRAVITY_POWER         (-0.4f)    // 重力
#define PlAYER_MAX_LIFE       (460)      // 体力

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void PlayerOperation(int nCntPlayer);          // プレイヤーの操作処理
void PlayerTransition(int nCntPlayer);         // プレイヤーの遷移処理
void PlayerAction(int nCntPlayer);             // プレイヤーがアクション状態の処理
void PlayerStateMent(int nCntPlayer);          // プレイヤーの状態遷移処理
void PlayerMotion(int nCntPlayer);             // モーション処理
void PlayerMotionBlend(int nCntPlayer);        // モーションブレンド処理
void PlayerMotionAdvance(int nCntPlayer);      // モーション進行処理
void SwitchKey(int nCntPlayer);                // キーフレームの切り替え処理
void SwitchMotion(int nCntPlayer);             // モーション切り替え処理

bool CollisionAttack(int nCntPlayer, D3DXVECTOR3 pos, float fRadius);
void CollisionPlayer(int nCntPlayer);
void PlayerAttack(int nCntPlayer);
void PlayerDamage(int nCntPlayer, int nDamage);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
Player  g_Player[MAX_PLAYER];                 // プレイヤー情報
int     g_nNumModel[MAX_PLAYER];              // そのプレイヤーのモデル数

//=============================================================================
// 初期化処理
//=============================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	FILE  *pFile;           // テキストファイルへのポインタ
	char  str0[128];        // いらない文字
	char  xFileName[64];    // xファイルの名前
	D3DXVECTOR3 OffsetPos;  // 親モデルからの距離
	D3DXVECTOR3 OffsetRot;  // 親モデルからの向き
	float fJump;            // ジャンプ力
	float fMovement;        // 移動量
	int   nIdxParent;       // 親モデル
	int   nCntModel = 0;    // モデルを読み込んだ数を数える変数
	int   nCntParts = 0;    // モデルのパーツ情報を読み込んだ回数を数える変数
	int   nCntPlayer = 0;   // プレイヤーの数
	int   nCntMotion = 0;   // モーションの番号
	int   nCntKey = 0;      // キーフレームの番号を数える変数
	int   nPlaybackKey;     // フレーム数
	int   nCntKeyModel = 0; // モデルの数分のキーフレームを数える変数
	int   bLoop;            // モーションがループするかしないか
	int   nNumKey = 0;      // キーフレームの数
	D3DXVECTOR3 posAdd;     // 基準の位置に加える値
	D3DXVECTOR3 DestAngle;  // 目的の向き

	//-------------------
	//  プレイヤー1人目
	//-------------------
	// ファイルオープン
	pFile = fopen("data/TEXT/motion_00.txt", "r");

	if (pFile != NULL)
	{// ファイルが開けた
		while (fscanf(pFile, "%s", &str0[0]) != EOF)
		{// テキストファイルの末端まで文字列を改行か空白があるまで読み取る
			if (strcmp(&str0[0], NUM_MODEL) == 0)
			{// モデルの数が書かれていたら
				fscanf(pFile, "%s %d", &str0[0], &g_nNumModel[nCntPlayer]);
			}
			if (strcmp(&str0[0], MODEL_FILENAME) == 0)
			{// xファイルの拡張子が書かれていたら
				if (fscanf(pFile, "%s %s", &str0[0], &xFileName[0]) == 2)
				{// モデルのファイル名を読み取ったら
				    // xファイルの読み込み
					D3DXLoadMeshFromX(&xFileName[0],
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&g_Player[nCntPlayer].aModel[nCntModel].pBuffMat,
						NULL,
						&g_Player[nCntPlayer].aModel[nCntModel].nNumMat,
						&g_Player[nCntPlayer].aModel[nCntModel].pMesh);

					// マテリアル情報からテクスチャを引き出す
					D3DXMATERIAL* pMat = (D3DXMATERIAL*)g_Player[nCntPlayer].aModel[nCntModel].pBuffMat->GetBufferPointer();
					D3DMATERIAL9* pMatBuff = new D3DMATERIAL9[g_Player[nCntPlayer].aModel[nCntModel].nNumMat];
					g_Player[nCntPlayer].aModel[nCntModel].pTexture = new LPDIRECT3DTEXTURE9[g_Player[nCntPlayer].aModel[nCntModel].nNumMat];

					for (DWORD nCntMat = 0; nCntMat < g_Player[nCntPlayer].aModel[nCntModel].nNumMat; nCntMat++)
					{// 頂点数の数だけ繰り返し
					 // マテリアル情報を読み込む
						pMatBuff[nCntMat] = pMat[nCntMat].MatD3D;

						// 環境光を初期化する
						pMatBuff[nCntMat].Ambient = pMatBuff[nCntMat].Diffuse;

						// テクスチャ情報を初期化
						g_Player[nCntPlayer].aModel[nCntModel].pTexture[nCntMat] = NULL;

						// テクスチャの情報を読み込む
						if (pMat[nCntMat].pTextureFilename != NULL &&
							lstrlen(pMat[nCntMat].pTextureFilename) > 0)
						{// テクスチャのファイル名がある
							D3DXCreateTextureFromFile(pDevice,
								pMat[nCntMat].pTextureFilename,
								&g_Player[nCntPlayer].aModel[nCntModel].pTexture[nCntMat]);
						}

					}
				}
				nCntModel++;  // 種類が変わるので進めておく
			}
			if (strcmp(&str0[0], CHARACTERSET) == 0)
			{// キャラクター情報が書かれていたら
				while (strcmp(&str0[0], END_CHARACTERSET) != 0)
				{// キャラクター情報が終わるまで読み取る
					if (strcmp(&str0[0], MOVE) == 0)
					{// 移動力が書かれていたら
						if (fscanf(pFile, "%s %f", &str0[0], &fMovement) == 2)
						{// 移動力を読み取ったら
							g_Player[nCntPlayer].fMovement = fMovement;
						}
					}
					if (strcmp(&str0[0], JUMP) == 0)
					{// ジャンプ力が書かれていたら
						if (fscanf(pFile, "%s %f", &str0[0], &fJump) == 2)
						{// ジャンプ力を読み取ったら
							g_Player[nCntPlayer].fJumpPower = fJump;
						}
					}

					if (strcmp(&str0[0], PARTSSET) == 0)
					{// モデルのパーツ情報が書かれていたら
						int nCntSet = 0;
						int nCntPartsNum = 0;
						while (strcmp(&str0[0], END_PARTSSET) != 0)
						{// モデルのパーツ情報が終わるまで読み取る
							if (strcmp(&str0[0], INDEX) == 0)
							{// パーツ番号を読み取る
								fscanf(pFile, "%s %d", &str0[0], &nCntPartsNum);
							}
							else if (strcmp(&str0[0], PARENT) == 0)
							{// 親モデルの番号を読み取る
								if (fscanf(pFile, "%s %d", &str0[0], &nIdxParent) == 2)
								{// 親モデルの番号を読み取る
									g_Player[nCntPlayer].aModel[nCntPartsNum].nIdxModelParent = nIdxParent;
								}
							}
							else if (strcmp(&str0[0], POS) == 0)
							{// 親モデルからの距離を読み取る
								if (fscanf(pFile, "%s %f %f %f", &str0[0], &OffsetPos.x, &OffsetPos.y, &OffsetPos.z) == 4)
								{// 座標情報を読み取ったら
									g_Player[nCntPlayer].aModel[nCntPartsNum].pos = OffsetPos;
								}
							}
							else if (strcmp(&str0[0], ROT) == 0)
							{// 親モデルからの向きを読み取る
								if (fscanf(pFile, "%s %f %f %f", &str0[0], &OffsetRot.x, &OffsetRot.y, &OffsetRot.z) == 4)
								{// 向き情報を読み取ったら
									g_Player[nCntPlayer].aModel[nCntPartsNum].rot = OffsetRot;
								}
							}
							fscanf(pFile, "%s", &str0[0]); // ファイルを読み進める
							nCntSet++;                     // セットした回数を増やす
						}
					}
					fscanf(pFile, "%s", &str0[0]); // ファイルを読み進める
				}
			}
			if (strcmp(&str0[0], MOTIONSET) == 0)
			{// モーション情報が書かれていたら
				nCntKey = 0;    // キーフレームのポインタ座標を戻す
				while (strcmp(&str0[0], END_MOTIONSET) != 0)
				{// モーション情報が終わるまで読み取る
					if (strcmp(&str0[0], LOOP) == 0)
					{// ループするかしないかを読み取る
						if (fscanf(pFile, "%s %d", &str0[0], &bLoop) == 2)
						{// ループするかしないかを読み取れた
							if (bLoop == 1)
							{
								g_Player[nCntPlayer].aMotion[nCntMotion].bLoop = true;
							}
							else
							{
								g_Player[nCntPlayer].aMotion[nCntMotion].bLoop = false;
							}
						}
					}
					else if (strcmp(&str0[0], NUM_KEY) == 0)
					{// キーフレーム数を読み取る
						if (fscanf(pFile, "%s %d", &str0[0], &nNumKey) == 2)
						{// キーフレーム数を読み取れた
							g_Player[nCntPlayer].aMotion[nCntMotion].nNumKey = nNumKey;
						}
					}
					else if (strcmp(&str0[0], KEYSET) == 0)
					{// キーフレーム情報が書かれていたら
						nCntKeyModel = 0;
						while (strcmp(&str0[0], FRAME) != 0)
						{// キーフレーム数情報が終わるまで読み取る
							fscanf(pFile, "%s", &str0[0]); // ファイルを読み進める
						}
						if (fscanf(pFile, "%s %d", &str0[0], &nPlaybackKey) == 2)
						{// キーフレーム数を読み取れた
							g_Player[nCntPlayer].aMotion[nCntMotion].Key[nCntKey].nPlayBackKey = nPlaybackKey;
						}
						while (strcmp(&str0[0], END_KEYSET) != 0)
						{// キーフレーム情報が終わるまで読み取る
							if (strcmp(&str0[0], KEY) == 0)
							{// キーフレームの目的の位置情報が書かれていたら
								while (strcmp(&str0[0], END_KEY) != 0)
								{// キーフレームの目的の位置情報が終わるまで読み取る
									if (strcmp(&str0[0], POS) == 0)
									{// 座標情報を読み取る
										if (fscanf(pFile, "%s %f %f %f", &str0[0], &posAdd.x, &posAdd.y, &posAdd.z) == 4)
										{// 座標情報を読み取ったら
											g_Player[nCntPlayer].aMotion[nCntMotion].Key[nCntKey].posAdd[nCntKeyModel] = posAdd;
										}
									}
									else if (strcmp(&str0[0], ROT) == 0)
									{// 向き情報を読み取る
										if (fscanf(pFile, "%s %f %f %f", &str0[0], &DestAngle.x, &DestAngle.y, &DestAngle.z) == 4)
										{// 向き情報を読み取ったら
											g_Player[nCntPlayer].aMotion[nCntMotion].Key[nCntKey].DestAngle[nCntKeyModel] = DestAngle;
										}
									}
									fscanf(pFile, "%s", &str0[0]); // ファイルを読み進める
								}
								if (nCntKeyModel < MAX_MODEL - 1)
								{// モデルごとのキーフレームが用意できる数までよりポインタが進んでない
									nCntKeyModel++;
								}
							}
							fscanf(pFile, "%s", &str0[0]); // ファイルを読み進める
						}
						if (nCntKey < MAX_MODEL - 1)
						{// キーフレームが用意できる数までよりポインタが進んでない
							nCntKey++;
						}
					}
					fscanf(pFile, "%s", &str0[0]); // ファイルを読み進める
				}
				if (nCntMotion < MAX_MOTION - 1)
				{// モーションが用意できる数までよりポインタが進んでない
					nCntMotion++;   // モーション番号をずらす
				}
			}
		}
		// ファイルを閉じて他のプログラムからいじれるようにする
		fclose(pFile);
	}



	// 数値を初期化する
	nCntModel = 0;    // モデルを読み込んだ数を数える変数
	nCntParts = 0;    // モデルのパーツ情報を読み込んだ回数を数える変数
	nCntMotion = 0;   // モーションの番号
	nCntKey = 0;      // キーフレームの番号を数える変数
	nPlaybackKey;     // フレーム数
	nCntKeyModel = 0; // モデルの数分のキーフレームを数える変数
	bLoop;            // モーションがループするかしないか
	nNumKey = 0;      // キーフレームの数
	nCntModel = 0;    // モデル数


	nCntPlayer++;     // プレイヤーの人数を増やす

	//-------------------
	//  プレイヤー2人目
	//-------------------
	// ファイルオープン
	pFile = fopen("data/TEXT/motion_01.txt", "r");

	if (pFile != NULL)
	{// ファイルが開けた
		while (fscanf(pFile, "%s", &str0[0]) != EOF)
		{// テキストファイルの末端まで文字列を改行か空白があるまで読み取る
			if (strcmp(&str0[0], NUM_MODEL) == 0)
			{// モデルの数が書かれていたら
				fscanf(pFile, "%s %d", &str0[0], &g_nNumModel[nCntPlayer]);
			}
			if (strcmp(&str0[0], MODEL_FILENAME) == 0)
			{// xファイルの拡張子が書かれていたら
				if (fscanf(pFile, "%s %s", &str0[0], &xFileName[0]) == 2)
				{// モデルのファイル名を読み取ったら
				 // xファイルの読み込み
					D3DXLoadMeshFromX(&xFileName[0],
						D3DXMESH_SYSTEMMEM,
						pDevice,
						NULL,
						&g_Player[nCntPlayer].aModel[nCntModel].pBuffMat,
						NULL,
						&g_Player[nCntPlayer].aModel[nCntModel].nNumMat,
						&g_Player[nCntPlayer].aModel[nCntModel].pMesh);

					// マテリアル情報からテクスチャを引き出す
					D3DXMATERIAL* pMat = (D3DXMATERIAL*)g_Player[nCntPlayer].aModel[nCntModel].pBuffMat->GetBufferPointer();
					D3DMATERIAL9* pMatBuff = new D3DMATERIAL9[g_Player[nCntPlayer].aModel[nCntModel].nNumMat];
					g_Player[nCntPlayer].aModel[nCntModel].pTexture = new LPDIRECT3DTEXTURE9[g_Player[nCntPlayer].aModel[nCntModel].nNumMat];

					for (DWORD nCntMat = 0; nCntMat < g_Player[nCntPlayer].aModel[nCntModel].nNumMat; nCntMat++)
					{// 頂点数の数だけ繰り返し
					 // マテリアル情報を読み込む
						pMatBuff[nCntMat] = pMat[nCntMat].MatD3D;

						// 環境光を初期化する
						pMatBuff[nCntMat].Ambient = pMatBuff[nCntMat].Diffuse;

						// テクスチャ情報を初期化
						g_Player[nCntPlayer].aModel[nCntModel].pTexture[nCntMat] = NULL;

						// テクスチャの情報を読み込む
						if (pMat[nCntMat].pTextureFilename != NULL &&
							lstrlen(pMat[nCntMat].pTextureFilename) > 0)
						{// テクスチャのファイル名がある
							D3DXCreateTextureFromFile(pDevice,
								pMat[nCntMat].pTextureFilename,
								&g_Player[nCntPlayer].aModel[nCntModel].pTexture[nCntMat]);
						}

					}
				}
				nCntModel++;  // 種類が変わるので進めておく
			}
			if (strcmp(&str0[0], CHARACTERSET) == 0)
			{// キャラクター情報が書かれていたら
				while (strcmp(&str0[0], END_CHARACTERSET) != 0)
				{// キャラクター情報が終わるまで読み取る
					if (strcmp(&str0[0], MOVE) == 0)
					{// 移動力が書かれていたら
						if (fscanf(pFile, "%s %f", &str0[0], &fMovement) == 2)
						{// 移動力を読み取ったら
							g_Player[nCntPlayer].fMovement = fMovement;
						}
					}
					if (strcmp(&str0[0], JUMP) == 0)
					{// ジャンプ力が書かれていたら
						if (fscanf(pFile, "%s %f", &str0[0], &fJump) == 2)
						{// ジャンプ力を読み取ったら
							g_Player[nCntPlayer].fJumpPower = fJump;
						}
					}

					if (strcmp(&str0[0], PARTSSET) == 0)
					{// モデルのパーツ情報が書かれていたら
						int nCntSet = 0;
						int nCntPartsNum = 0;
						while (strcmp(&str0[0], END_PARTSSET) != 0)
						{// モデルのパーツ情報が終わるまで読み取る
							if (strcmp(&str0[0], INDEX) == 0)
							{// パーツ番号を読み取る
								fscanf(pFile, "%s %d", &str0[0], &nCntPartsNum);
							}
							else if (strcmp(&str0[0], PARENT) == 0)
							{// 親モデルの番号を読み取る
								if (fscanf(pFile, "%s %d", &str0[0], &nIdxParent) == 2)
								{// 親モデルの番号を読み取る
									g_Player[nCntPlayer].aModel[nCntPartsNum].nIdxModelParent = nIdxParent;
								}
							}
							else if (strcmp(&str0[0], POS) == 0)
							{// 親モデルからの距離を読み取る
								if (fscanf(pFile, "%s %f %f %f", &str0[0], &OffsetPos.x, &OffsetPos.y, &OffsetPos.z) == 4)
								{// 座標情報を読み取ったら
									g_Player[nCntPlayer].aModel[nCntPartsNum].pos = OffsetPos;
								}
							}
							else if (strcmp(&str0[0], ROT) == 0)
							{// 親モデルからの向きを読み取る
								if (fscanf(pFile, "%s %f %f %f", &str0[0], &OffsetRot.x, &OffsetRot.y, &OffsetRot.z) == 4)
								{// 向き情報を読み取ったら
									g_Player[nCntPlayer].aModel[nCntPartsNum].rot = OffsetRot;
								}
							}
							fscanf(pFile, "%s", &str0[0]); // ファイルを読み進める
							nCntSet++;                     // セットした回数を増やす
						}
					}
					fscanf(pFile, "%s", &str0[0]); // ファイルを読み進める
				}
			}
			if (strcmp(&str0[0], MOTIONSET) == 0)
			{// モーション情報が書かれていたら
				nCntKey = 0;    // キーフレームのポインタ座標を戻す
				while (strcmp(&str0[0], END_MOTIONSET) != 0)
				{// モーション情報が終わるまで読み取る
					if (strcmp(&str0[0], LOOP) == 0)
					{// ループするかしないかを読み取る
						if (fscanf(pFile, "%s %d", &str0[0], &bLoop) == 2)
						{// ループするかしないかを読み取れた
							if (bLoop == 1)
							{
								g_Player[nCntPlayer].aMotion[nCntMotion].bLoop = true;
							}
							else
							{
								g_Player[nCntPlayer].aMotion[nCntMotion].bLoop = false;
							}
						}
					}
					else if (strcmp(&str0[0], NUM_KEY) == 0)
					{// キーフレーム数を読み取る
						if (fscanf(pFile, "%s %d", &str0[0], &nNumKey) == 2)
						{// キーフレーム数を読み取れた
							g_Player[nCntPlayer].aMotion[nCntMotion].nNumKey = nNumKey;
						}
					}
					else if (strcmp(&str0[0], KEYSET) == 0)
					{// キーフレーム情報が書かれていたら
						nCntKeyModel = 0;
						while (strcmp(&str0[0], FRAME) != 0)
						{// キーフレーム数情報が終わるまで読み取る
							fscanf(pFile, "%s", &str0[0]); // ファイルを読み進める
						}
						if (fscanf(pFile, "%s %d", &str0[0], &nPlaybackKey) == 2)
						{// キーフレーム数を読み取れた
							g_Player[nCntPlayer].aMotion[nCntMotion].Key[nCntKey].nPlayBackKey = nPlaybackKey;
						}
						while (strcmp(&str0[0], END_KEYSET) != 0)
						{// キーフレーム情報が終わるまで読み取る
							if (strcmp(&str0[0], KEY) == 0)
							{// キーフレームの目的の位置情報が書かれていたら
								while (strcmp(&str0[0], END_KEY) != 0)
								{// キーフレームの目的の位置情報が終わるまで読み取る
									if (strcmp(&str0[0], POS) == 0)
									{// 座標情報を読み取る
										if (fscanf(pFile, "%s %f %f %f", &str0[0], &posAdd.x, &posAdd.y, &posAdd.z) == 4)
										{// 座標情報を読み取ったら
											g_Player[nCntPlayer].aMotion[nCntMotion].Key[nCntKey].posAdd[nCntKeyModel] = posAdd;
										}
									}
									else if (strcmp(&str0[0], ROT) == 0)
									{// 向き情報を読み取る
										if (fscanf(pFile, "%s %f %f %f", &str0[0], &DestAngle.x, &DestAngle.y, &DestAngle.z) == 4)
										{// 向き情報を読み取ったら
											g_Player[nCntPlayer].aMotion[nCntMotion].Key[nCntKey].DestAngle[nCntKeyModel] = DestAngle;
										}
									}
									fscanf(pFile, "%s", &str0[0]); // ファイルを読み進める
								}
								if (nCntKeyModel < MAX_MODEL - 1)
								{// モデルごとのキーフレームが用意できる数までよりポインタが進んでない
									nCntKeyModel++;
								}
							}
							fscanf(pFile, "%s", &str0[0]); // ファイルを読み進める
						}
						if (nCntKey < MAX_MODEL - 1)
						{// キーフレームが用意できる数までよりポインタが進んでない
							nCntKey++;
						}
					}
					fscanf(pFile, "%s", &str0[0]); // ファイルを読み進める
				}
				if (nCntMotion < MAX_MOTION - 1)
				{// モーションが用意できる数までよりポインタが進んでない
					nCntMotion++;   // モーション番号をずらす
				}
			}
		}
		// ファイルを閉じて他のプログラムからいじれるようにする
		fclose(pFile);
	}




	for (nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーの数だけ繰り返し
		g_Player[nCntPlayer].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);           // 移動量を初期化
		g_Player[nCntPlayer].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);            // 現在の向きを初期化
		g_Player[nCntPlayer].DiffAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);      // 差分を初期化
		g_Player[nCntPlayer].state = PLAYERSTATE_NORMAL;                     // 通常の状態に
		g_Player[nCntPlayer].radius = D3DXVECTOR3(11.5f, 30.0f, 11.5f);      // 当たり判定を取る範囲を初期化
		g_Player[nCntPlayer].ShadowCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.35f); // 影の色を設定
		g_Player[nCntPlayer].ShadowWidth = 15;                               // 影の幅を設定
		g_Player[nCntPlayer].ShadowDepth = 15;                               // 影の奥行を設定
		g_Player[nCntPlayer].bJump = false;                                  // ジャンプしていない状態に
		g_Player[nCntPlayer].bWeakAction = false;                            // 弱攻撃していない状態に
		g_Player[nCntPlayer].bStorongAction = false;                         // 強攻撃していない状態に
		g_Player[nCntPlayer].bHit = false;                                   // 攻撃がヒットしていない状態に
		g_Player[nCntPlayer].bControll = true;                               // 移動できる状態に
		g_Player[nCntPlayer].nLife = PlAYER_MAX_LIFE;                        // 体力
		g_Player[nCntPlayer].wAttackNumber = WEAK_ATTACK_NUMBER_0;           // 弱攻撃の番号
		g_Player[nCntPlayer].mState = MOTIONSTATE_NORMAL;                    // モーションの状態は通常の状態に

		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{// モデルのパーツ数分繰り返し
			g_Player[nCntPlayer].aModel[nCntModel].posStd = g_Player[nCntPlayer].aModel[nCntModel].pos;
			g_Player[nCntPlayer].aModel[nCntModel].rot = g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].DestAngle[nCntModel];
		}

		// 影の番号を設定
		g_Player[nCntPlayer].nIdxShadow = SetShadow(D3DXVECTOR3(g_Player[nCntPlayer].pos.x, 1.0f, g_Player[nCntPlayer].pos.z), g_Player[nCntPlayer].rot, g_Player[nCntPlayer].ShadowCol, g_Player[nCntPlayer].ShadowWidth, g_Player[nCntPlayer].ShadowDepth);

		// モーション遷移処理
		SwitchMotion(nCntPlayer);
	}

	g_Player[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);            // 現在の位置を初期化
	g_Player[0].posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 現在の位置を初期化
	g_Player[0].rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);  // 現在の位置を初期化
	g_Player[0].DestAngle = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);      // 目的の向きを初期化

	g_Player[1].pos = D3DXVECTOR3(200.0f, 0.0f, 0.0f);     // 現在の位置を初期化
	g_Player[1].posold = D3DXVECTOR3(200.0f, 0.0f, 0.0f);  // 現在の位置を初期化
	g_Player[1].rot = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);  // 現在の位置を初期化
	g_Player[1].DestAngle = D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f);      // 目的の向きを初期化
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitPlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーの数だけ繰り返し
		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{// モデルのパーツ数だけ繰り返し
		    // メッシュの破棄
			if (g_Player[nCntPlayer].aModel[nCntModel].pMesh != NULL)
			{
				g_Player[nCntPlayer].aModel[nCntModel].pMesh->Release();
				g_Player[nCntPlayer].aModel[nCntModel].pMesh = NULL;
			}

			// マテリアルの破棄
			if (g_Player[nCntPlayer].aModel[nCntModel].pBuffMat != NULL)
			{
				g_Player[nCntPlayer].aModel[nCntModel].pBuffMat->Release();
				g_Player[nCntPlayer].aModel[nCntModel].pBuffMat = NULL;
			}

			// テクスチャの破棄
			for (DWORD nCntMat = 0; nCntMat < g_Player[nCntPlayer].aModel[nCntModel].nNumMat; nCntMat++)
			{
				if (g_Player[nCntPlayer].aModel[nCntModel].pTexture[nCntMat] != NULL)
				{
					g_Player[nCntPlayer].aModel[nCntModel].pTexture[nCntMat]->Release();
					g_Player[nCntPlayer].aModel[nCntModel].pTexture[nCntMat] = NULL;
				}
			}
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePlayer(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーの数だけ繰り返し
		g_Player[nCntPlayer].posold = g_Player[nCntPlayer].pos;

		if (g_Player[nCntPlayer].bWeakAction != true && g_Player[nCntPlayer].bStorongAction != true && g_Player[nCntPlayer].bControll == true && g_Player[nCntPlayer].bUltimate != true)
		{// 動ける状態である
			// 移動処理
			PlayerOperation(nCntPlayer);
		}

		// 操作できるプレイヤーの遷移処理
		if (g_Player[nCntPlayer].bControll == true)
		{// プレイヤーが操作できる状態である
			PlayerTransition(nCntPlayer);
		}

		// プレイヤーの状態管理処理
		PlayerStateMent(nCntPlayer);

		// プレイヤーのアクション処理
		PlayerAction(nCntPlayer);

		if (g_Player[nCntPlayer].mState == MOTIONSTATE_NORMAL)
		{// 通常のモーション状態だったら
		    // モーション処理
			PlayerMotion(nCntPlayer);
		}
		else if (g_Player[nCntPlayer].mState == MOTIONSTATE_SWITCH)
		{// モーション切り替え状態だったら
		    // モーションブレンド処理
			PlayerMotionBlend(nCntPlayer);
		}

		if (g_Player[nCntPlayer].mState == MOTIONSTATE_NORMAL)
		{// キーフレームを進める状態だったら
			// モーション進行処理
			PlayerMotionAdvance(nCntPlayer);
		}

		// 壁の当たり判定
		CollisionPlayer(nCntPlayer);

		// プレイヤーの攻撃判定
		PlayerAttack(nCntPlayer);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;     // 計算用マトリックス
	D3DXVECTOR3 vecRot, vecTrans;               // 計算用モデルの向き,座標
	D3DMATERIAL9 matDef;                        // 現在のマテリアル保存用
	D3DXMATERIAL *pMat;                         // マテリアルデータへのポインタ
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{// プレイヤーの数だけ繰り返し
	   // ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_Player[nCntPlayer].mtxWorld);

		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player[nCntPlayer].rot.y, g_Player[nCntPlayer].rot.x, g_Player[nCntPlayer].rot.z);
		D3DXMatrixMultiply(&g_Player[nCntPlayer].mtxWorld, &g_Player[nCntPlayer].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_Player[nCntPlayer].pos.x, g_Player[nCntPlayer].pos.y, g_Player[nCntPlayer].pos.z);
		D3DXMatrixMultiply(&g_Player[nCntPlayer].mtxWorld, &g_Player[nCntPlayer].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_Player[nCntPlayer].mtxWorld);

		for (int nCntModel = 0; nCntModel < g_nNumModel[nCntPlayer]; nCntModel++)
		{// モデルのパーツ数だけ繰り返し
			if (g_Player[nCntPlayer].aModel[nCntModel].nIdxModelParent == -1)
			{// 親がいない
				mtxParent = g_Player[nCntPlayer].mtxWorld;     // プレイヤーのマトリックス情報をもらう
			}
			else
			{// 親がいる
				mtxParent = g_Player[nCntPlayer].aModel[g_Player[nCntPlayer].aModel[nCntModel].nIdxModelParent].mtxWorld;   // 親モデルのマトリックス情報をもらう
			}
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Player[nCntPlayer].aModel[nCntModel].mtxWorld);

			// 回転を反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player[nCntPlayer].aModel[nCntModel].rot.y, g_Player[nCntPlayer].aModel[nCntModel].rot.x, g_Player[nCntPlayer].aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_Player[nCntPlayer].aModel[nCntModel].mtxWorld, &g_Player[nCntPlayer].aModel[nCntModel].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Player[nCntPlayer].aModel[nCntModel].pos.x, g_Player[nCntPlayer].aModel[nCntModel].pos.y, g_Player[nCntPlayer].aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_Player[nCntPlayer].aModel[nCntModel].mtxWorld, &g_Player[nCntPlayer].aModel[nCntModel].mtxWorld, &mtxTrans);

			// 親の情報を自分に反映
			D3DXMatrixMultiply(&g_Player[nCntPlayer].aModel[nCntModel].mtxWorld, &g_Player[nCntPlayer].aModel[nCntModel].mtxWorld, &mtxParent);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Player[nCntPlayer].aModel[nCntModel].mtxWorld);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_Player[nCntPlayer].aModel[nCntModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)g_Player[nCntPlayer].aModel[nCntModel].nNumMat; nCntMat++)
			{// 設定されていたマテリアルの数だけ繰り返し
			 // マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// テクスチャの設定
				pDevice->SetTexture(0, g_Player[nCntPlayer].aModel[nCntModel].pTexture[nCntMat]);

				// モデル(パーツ)の描画
				g_Player[nCntPlayer].aModel[nCntModel].pMesh->DrawSubset(nCntMat);
			}

			// 保存していたマテリアルに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}
//=============================================================================
// プレイヤーの操作処理
//=============================================================================
void PlayerOperation(int nCntPlayer)
{
	if (g_Player[nCntPlayer].bJump != true)
	{// ジャンプしていない
		g_Player[nCntPlayer].posold = g_Player[nCntPlayer].pos;   // 前回の位置更新
		if (GetXInputPress(nCntPlayer, XIJS_BUTTON_3) == true)
		{// 十字キー右入力がされた
			g_Player[nCntPlayer].move.x += sinf(D3DX_PI * 0.5f) *  g_Player[nCntPlayer].fMovement;
			if (g_Player[nCntPlayer].bJump != true)
			{
				g_Player[nCntPlayer].DestAngle.y = -D3DX_PI * 0.5f;
			}
		}
		else if (GetXInputPress(nCntPlayer, XIJS_BUTTON_2) == true)
		{// 十字キー左入力がされた
			g_Player[nCntPlayer].move.x -= sinf(D3DX_PI * 0.5f) *  g_Player[nCntPlayer].fMovement;
			if (g_Player[nCntPlayer].bJump != true)
			{
				g_Player[nCntPlayer].DestAngle.y = D3DX_PI * 0.5f;
			}
		}
		else if (GetXInputPress(nCntPlayer, XIJS_BUTTON_26) == true || GetXInputPress(nCntPlayer, XIJS_BUTTON_27) == true)
		{// 左アナログスティックの入力がされた
			// 移動処理
			g_Player[nCntPlayer].move.x += sinf(GetXInputLeftAxiz(nCntPlayer)) * g_Player[nCntPlayer].fMovement;
			if (g_Player[nCntPlayer].bJump != true)
			{
				if (GetXInputPress(nCntPlayer, XIJS_BUTTON_26) == true)
				{// 左に倒されている
					g_Player[nCntPlayer].DestAngle.y = D3DX_PI * 0.5f;
				}
				if (GetXInputPress(nCntPlayer, XIJS_BUTTON_27) == true)
				{// 右に倒されている
					g_Player[nCntPlayer].DestAngle.y = -D3DX_PI * 0.5f;
				}
			}
		}
	}
}
//=============================================================================
// プレイヤーの状態遷移処理
//=============================================================================
void PlayerTransition(int nCntPlayer)
{
	if (g_Player[nCntPlayer].bJump == false && g_Player[nCntPlayer].bWeakAction != true && g_Player[nCntPlayer].bStorongAction != true
		&& g_Player[nCntPlayer].bUltimate != true && g_Player[nCntPlayer].state != PLAYERSTATE_ULTIMATE_START)
	{// ジャンプしていないかつアクション状態でない
		if (g_Player[nCntPlayer].move.x <= 1.0f && g_Player[nCntPlayer].move.x >= -1.0f &&
			g_Player[nCntPlayer].move.z <= 1.0f && g_Player[nCntPlayer].move.z >= -1.0f)
		{// 移動していなかったら
			if (g_Player[nCntPlayer].state != PLAYERSTATE_NORMAL)
			{// 移動していない状態じゃなかったら
				g_Player[nCntPlayer].state = PLAYERSTATE_NORMAL;     // 通常の状態に
				// モーション切り替え処理
				SwitchMotion(nCntPlayer);
			}
		}
		if (g_Player[nCntPlayer].move.x > 1.0f || g_Player[nCntPlayer].move.x < -1.0f ||
			g_Player[nCntPlayer].move.z > 1.0f || g_Player[nCntPlayer].move.z < -1.0f)
		{// 移動していたら
			if (g_Player[nCntPlayer].state != PLAYERSTATE_MOVE)
			{// 移動している状態じゃなかったら
				g_Player[nCntPlayer].state = PLAYERSTATE_MOVE;       // 移動している状態に
				// モーション切り替え処理
				SwitchMotion(nCntPlayer);
			}
		}
	}

	if (g_Player[nCntPlayer].bDown == true)
	{// プレイヤーがダウンしている
		g_Player[nCntPlayer].nStateMentCounter++;
		if (g_Player[nCntPlayer].nStateMentCounter >= 30)
		{
			g_Player[nCntPlayer].nStateMentCounter = 0;
			g_Player[nCntPlayer].bDown = false;
		}
	}
}

//=============================================================================
// プレイヤーの状態遷移処理
//=============================================================================
void PlayerStateMent(int nCntPlayer)
{
	// 重力を加算する
	g_Player[nCntPlayer].move.y += GRAVITY_POWER;

	// 位置を移動
	g_Player[nCntPlayer].pos.x += g_Player[nCntPlayer].move.x;
	g_Player[nCntPlayer].pos.y += g_Player[nCntPlayer].move.y;
	g_Player[nCntPlayer].pos.z += g_Player[nCntPlayer].move.z;

	// 角度の修正
	g_Player[nCntPlayer].DiffAngle.y = g_Player[nCntPlayer].DestAngle.y - g_Player[nCntPlayer].rot.y;   // 現在の向きと目的の向きの差分を計算

	if (g_Player[nCntPlayer].DiffAngle.y > D3DX_PI)
	{// 差分がD3DX_PIを超えた
		g_Player[nCntPlayer].DiffAngle.y -= D3DX_PI * 2.0f;
	}
	if (g_Player[nCntPlayer].DiffAngle.y < -D3DX_PI)
	{// 差分が-D3DX_PIを超えた
		g_Player[nCntPlayer].DiffAngle.y += D3DX_PI * 2.0f;
	}

	g_Player[nCntPlayer].rot.y += g_Player[nCntPlayer].DiffAngle.y * 0.1f;

	if (g_Player[nCntPlayer].rot.y > D3DX_PI)
	{// 現在の向きがD3DX_PIを超えた
		g_Player[nCntPlayer].rot.y -= D3DX_PI * 2.0f;
	}
	if (g_Player[nCntPlayer].rot.y < -D3DX_PI)
	{// 現在の向きが-D3DX_PIを超えた
		g_Player[nCntPlayer].rot.y += D3DX_PI * 2.0f;
	}

	// 影の位置更新
	SetPositionShadow(g_Player[nCntPlayer].nIdxShadow, D3DXVECTOR3(g_Player[nCntPlayer].pos.x, 1.0f, g_Player[nCntPlayer].pos.z));

	// 影の色更新
	SetColShadow(g_Player[nCntPlayer].nIdxShadow, g_Player[nCntPlayer].ShadowCol);

	// 影の大きさ更新
	SetSizeShadow(g_Player[nCntPlayer].nIdxShadow, g_Player[nCntPlayer].ShadowWidth, g_Player[nCntPlayer].ShadowDepth);

	if (g_Player[nCntPlayer].pos.y < 0.0f)
	{// 着地した
		g_Player[nCntPlayer].pos.y = 0.0f;         // 位置を戻す
		g_Player[nCntPlayer].move.y = 0.0f;        // 重力を消す
		g_Player[nCntPlayer].ShadowWidth = 15;     // 影の幅を戻す
		g_Player[nCntPlayer].ShadowDepth = 15;     // 影の奥行を戻す
		g_Player[nCntPlayer].ShadowCol.a = 0.15f;  // 影の透明度を戻す
		if (g_Player[nCntPlayer].state == PLAYERSTATE_JUMP)
		{// ジャンプ状態だったら
			if (g_Player[nCntPlayer].bJump == true)
			{// ジャンプしていた
				if (g_Player[nCntPlayer].move.x <= 1.0f && g_Player[nCntPlayer].move.x >= -1.0f &&
					g_Player[nCntPlayer].move.z <= 1.0f && g_Player[nCntPlayer].move.z >= -1.0f)
				{// 移動していなかったら
					g_Player[nCntPlayer].state = PLAYERSTATE_NORMAL;        // 着地状態に
																			// モーション切り替え処理
					SwitchMotion(nCntPlayer);
					g_Player[nCntPlayer].move.x = 0.0f;   // 移動を無視する
					g_Player[nCntPlayer].move.z = 0.0f;   // 移動を無視する

				}
				else if (g_Player[nCntPlayer].move.x > 1.0f || g_Player[nCntPlayer].move.x < -1.0f ||
					g_Player[nCntPlayer].move.z > 1.0f || g_Player[nCntPlayer].move.z < -1.0f)
				{// 移動していたら
					g_Player[nCntPlayer].state = PLAYERSTATE_MOVE;           // 移動している状態に
																			 // モーション切り替え処理
					SwitchMotion(nCntPlayer);
				}
			}
			PlaySound(SOUND_LABEL_SE_LANDING);
		}
		else if (g_Player[nCntPlayer].state == PLAYERSTATE_DAMAGE_FLY)
		{// 吹っ飛び状態だったら
			g_Player[nCntPlayer].state = PLAYERSTATE_DAMAGE_DOWN;
			SwitchMotion(nCntPlayer);
			PlaySound(SOUND_LABEL_SE_DOWN);
			g_Player[nCntPlayer].move.x = 0.0f;
		}
		g_Player[nCntPlayer].bJump = false;      // ジャンプしていない判定に
	}

	// 慣性を働かせる
	if (g_Player[nCntPlayer].bJump == false)
	{// ジャンプしていなかったら
		g_Player[nCntPlayer].move.x += (0.0f - g_Player[nCntPlayer].move.x) * PLAYER_INERTIA_NORMAL;
		g_Player[nCntPlayer].move.z += (0.0f - g_Player[nCntPlayer].move.z) * PLAYER_INERTIA_NORMAL;
	}
	else if (g_Player[nCntPlayer].bJump == true)
	{// ジャンプしていたら
		g_Player[nCntPlayer].move.x += (0.0f - g_Player[nCntPlayer].move.x) * PLAYER_INERTIA_JUMP;
		g_Player[nCntPlayer].move.z += (0.0f - g_Player[nCntPlayer].move.z) * PLAYER_INERTIA_JUMP;
	}

	if (g_Player[nCntPlayer].bJump == true)
	{// ジャンプしていたら
		if (g_Player[nCntPlayer].move.y > 0.0f)
		{// 空中に上がっている
			g_Player[nCntPlayer].ShadowWidth -= 0.3f;
			g_Player[nCntPlayer].ShadowDepth -= 0.3f;
			g_Player[nCntPlayer].ShadowCol.a -= 0.006f;
		}
		else if (g_Player[nCntPlayer].move.y < 0.0f)
		{// 地上に落ちている
			g_Player[nCntPlayer].ShadowWidth += 0.3f;
			g_Player[nCntPlayer].ShadowDepth += 0.3f;
			g_Player[nCntPlayer].ShadowCol.a += 0.006f;
		}
	}
}

//=============================================================================
// プレイヤーがアクション状態の時の処理
//=============================================================================
void PlayerAction(int nCntPlayer)
{
	if (g_Player[nCntPlayer].bControll == true)
	{// 操作できる状態である
		if (GetKeyboardTrigger(DIK_SPACE) == true || GetXInputTrigger(nCntPlayer, XIJS_BUTTON_24) == true || GetXInputTrigger(nCntPlayer, XIJS_BUTTON_0) == true)
		{// ジャンプボタンが押された
			if (g_Player[nCntPlayer].bJump == false)
			{// ジャンプしていない
				g_Player[nCntPlayer].move.y += g_Player[nCntPlayer].fJumpPower;   // 上昇させる
				g_Player[nCntPlayer].move.x = 0.0f;
				if (GetXInputTrigger(nCntPlayer, XIJS_BUTTON_0) == true)
				{// 十字キー上入力がされている
					if (GetXInputTrigger(nCntPlayer, XIJS_BUTTON_2) == true)
					{// 十字キー左入力がされている
						g_Player[nCntPlayer].move.x = sinf((D3DX_PI * -0.5f)) * 5.0f;
					}
					else if (GetXInputTrigger(nCntPlayer, XIJS_BUTTON_3) == true)
					{// 十字キー右入力がされている
						g_Player[nCntPlayer].move.x = sinf((D3DX_PI * 0.5f)) * 5.0f;
					}
				}

				if (GetXInputTrigger(nCntPlayer, XIJS_BUTTON_24) == true)
				{// 左スティック上入力がされている
					if (GetXInputThumbLX(nCntPlayer) <= -3000.0f)
					{// 左スティック左に倒されてる
						g_Player[nCntPlayer].move.x = sinf((D3DX_PI * -0.5f)) * 5.0f;
					}
					else if (GetXInputThumbLX(nCntPlayer) >= 3000.0f)
					{// 左スティック右に倒されてる
						g_Player[nCntPlayer].move.x = sinf((D3DX_PI * 0.5f)) * 5.0f;
					}
				}

				PlaySound(SOUND_LABEL_SE_JUMP);
				g_Player[nCntPlayer].bJump = true;                                // ジャンプした状態に
				g_Player[nCntPlayer].state = PLAYERSTATE_JUMP;                    // ジャンプ状態に
				// モーション切り替え処理
				SwitchMotion(nCntPlayer);

				// 位置を移動
				g_Player[nCntPlayer].pos.x += g_Player[nCntPlayer].move.x;
				g_Player[nCntPlayer].pos.y += g_Player[nCntPlayer].move.y;
				g_Player[nCntPlayer].pos.z += g_Player[nCntPlayer].move.z;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetXInputTrigger(nCntPlayer, XIJS_BUTTON_11) == true)
		{// 弱攻撃ボタンが押された
			if (g_Player[nCntPlayer].bStorongAction != true && g_Player[nCntPlayer].bUltimate != true)
			{// 強攻撃をしていない
				if (g_Player[nCntPlayer].wAttackNumber < WEAK_ATTACK_NUMBER_MAX)
				{// 弱攻撃の派生が終わっていない
					if (g_Player[nCntPlayer].wAttackNumber == WEAK_ATTACK_NUMBER_0)
					{// 最初の攻撃ならば
						g_Player[nCntPlayer].state = (PLAYERSTATE)(g_Player[nCntPlayer].wAttackNumber + 2);
						g_Player[nCntPlayer].wAttackNumber++;
						SwitchMotion(nCntPlayer);
					}
					else
					{// それ以外の攻撃
						g_Player[nCntPlayer].state = (PLAYERSTATE)(g_Player[nCntPlayer].wAttackNumber + 2);
						g_Player[nCntPlayer].wAttackNumber++;
						SwitchMotion(nCntPlayer);
					}
					g_Player[nCntPlayer].bWeakAction = true;   // 弱攻撃している状態に
					g_Player[nCntPlayer].bHit = false;         // 攻撃がヒットしていない判定に
					PlaySound(SOUND_LABEL_SE_WEAKPUNCH);
				}
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetXInputTrigger(nCntPlayer, XIJS_BUTTON_10) == true)
		{// 強攻撃ボタンが押された
			if (g_Player[nCntPlayer].bWeakAction != true && g_Player[nCntPlayer].bUltimate != true)
			{// 弱攻撃をしていない
				if (g_Player[nCntPlayer].bStorongAction != true)
				{// 強攻撃をしていない
					g_Player[nCntPlayer].state = PLAYERSTATE_STORONG_ATTACK;
					SwitchMotion(nCntPlayer);
					g_Player[nCntPlayer].bStorongAction = true;
					g_Player[nCntPlayer].bHit = false;         // 攻撃がヒットしていない判定に
					PlaySound(SOUND_LABEL_SE_STORONGPUNCH);
				}
			}
		}

		if (GetXInputTrigger(nCntPlayer, XIJS_BUTTON_13) == true)
		{// 必殺攻撃ボタンが押された
			if (GetUltGaugeWidth(nCntPlayer) >= ULTGAUGE_WIDTH)
			{// 必殺ゲージが満タンである
			    // 必殺ゲージが満タンである
				g_Player[nCntPlayer].state = PLAYERSTATE_ULTIMATE_START;
				SwitchMotion(nCntPlayer);
				g_Player[nCntPlayer].bHit = false;         // 攻撃がヒットしていない判定に
			}
		}
	}
}

//=============================================================================
// プレイヤーのモーション
//=============================================================================
void PlayerMotion(int nCntPlayer)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{// モデルのパーツ数分繰り返し
	    // 現在の位置更新
		g_Player[nCntPlayer].aModel[nCntModel].pos.x += g_Player[nCntPlayer].aModel[nCntModel].posDiff.x / g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey;
		g_Player[nCntPlayer].aModel[nCntModel].pos.y += g_Player[nCntPlayer].aModel[nCntModel].posDiff.y / g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey;
		g_Player[nCntPlayer].aModel[nCntModel].pos.z += g_Player[nCntPlayer].aModel[nCntModel].posDiff.z / g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey;

		g_Player[nCntPlayer].aModel[nCntModel].rot.x += g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.x / g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey;

		if (g_Player[nCntPlayer].aModel[nCntModel].rot.x > D3DX_PI)
		{// 現在の向きがD3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].rot.x -= D3DX_PI * 2.0f;
		}
		if (g_Player[nCntPlayer].aModel[nCntModel].rot.x < -D3DX_PI)
		{// 現在の向きが-D3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].rot.x += D3DX_PI * 2.0f;
		}

		g_Player[nCntPlayer].aModel[nCntModel].rot.y += g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.y / g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey;

		if (g_Player[nCntPlayer].aModel[nCntModel].rot.y > D3DX_PI)
		{// 現在の向きがD3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].rot.y -= D3DX_PI * 2.0f;
		}
		if (g_Player[nCntPlayer].aModel[nCntModel].rot.y < -D3DX_PI)
		{// 現在の向きが-D3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].rot.y += D3DX_PI * 2.0f;
		}

		g_Player[nCntPlayer].aModel[nCntModel].rot.z += g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.z / g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey;

		if (g_Player[nCntPlayer].aModel[nCntModel].rot.z > D3DX_PI)
		{// 現在の向きがD3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].rot.z -= D3DX_PI * 2.0f;
		}
		if (g_Player[nCntPlayer].aModel[nCntModel].rot.z < -D3DX_PI)
		{// 現在の向きが-D3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].rot.z += D3DX_PI * 2.0f;
		}
	}
}
//=============================================================================
// プレイヤーのモーションブレンド処理
//=============================================================================
void PlayerMotionBlend(int nCntPlayer)
{
	// ブレンドカウンターを進める
	g_Player[nCntPlayer].nBlendCounter++;
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{// モデルのパーツ数分繰り返し
	    // 現在の位置更新
		g_Player[nCntPlayer].aModel[nCntModel].pos.x += g_Player[nCntPlayer].aModel[nCntModel].posDiff.x / (g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey * 0.3f);
		g_Player[nCntPlayer].aModel[nCntModel].pos.y += g_Player[nCntPlayer].aModel[nCntModel].posDiff.y / (g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey * 0.3f);
		g_Player[nCntPlayer].aModel[nCntModel].pos.z += g_Player[nCntPlayer].aModel[nCntModel].posDiff.z / (g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey * 0.3f);

		g_Player[nCntPlayer].aModel[nCntModel].rot.x += g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.x / (g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey * 0.3f);

		if (g_Player[nCntPlayer].aModel[nCntModel].rot.x > D3DX_PI)
		{// 現在の向きがD3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].rot.x -= D3DX_PI * 2.0f;
		}
		if (g_Player[nCntPlayer].aModel[nCntModel].rot.x < -D3DX_PI)
		{// 現在の向きが-D3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].rot.x += D3DX_PI * 2.0f;
		}
		g_Player[nCntPlayer].aModel[nCntModel].rot.y += g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.y / (g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey * 0.3f);

		if (g_Player[nCntPlayer].aModel[nCntModel].rot.y > D3DX_PI)
		{// 現在の向きがD3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].rot.y -= D3DX_PI * 2.0f;
		}
		if (g_Player[nCntPlayer].aModel[nCntModel].rot.y < -D3DX_PI)
		{// 現在の向きが-D3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].rot.y += D3DX_PI * 2.0f;
		}

		g_Player[nCntPlayer].aModel[nCntModel].rot.z += g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.z / (g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey * 0.3f);

		if (g_Player[nCntPlayer].aModel[nCntModel].rot.z > D3DX_PI)
		{// 現在の向きがD3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].rot.z -= D3DX_PI * 2.0f;
		}
		if (g_Player[nCntPlayer].aModel[nCntModel].rot.z < -D3DX_PI)
		{// 現在の向きが-D3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].rot.z += D3DX_PI * 2.0f;
		}
	}

	if (g_Player[nCntPlayer].nBlendCounter >= g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey * 0.3f)
	{// ブレンドカウンターが既定の値に
		g_Player[nCntPlayer].nBlendCounter = 0;            // ブレンドカウンターを初期化
		if (g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].nNumKey >= 2)
		{// キーフレームが2つ以上あったら
			g_Player[nCntPlayer].mState = MOTIONSTATE_NORMAL;  // モーションの状態を通常の状態に
			g_Player[nCntPlayer].nKey++;                       // キーフレームを進める
												   // キーフレーム切り替え処理
			SwitchKey(nCntPlayer);
		}
		else
		{// キーフレームがこれ以上ない
			g_Player[nCntPlayer].mState = MOTIONSTATE_STOP;    // モーションの状態をモーションしない状態に
		}
	}
}
//=============================================================================
// モーション進行処理
//=============================================================================
void PlayerMotionAdvance(int nCntPlayer)
{
	g_Player[nCntPlayer].nMotionCounter++;   // モーションカウンターを進める
	if (g_Player[nCntPlayer].nMotionCounter >= g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].nPlayBackKey)
	{// 再生フレーム数に達した
		if (g_Player[nCntPlayer].nKey == g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].nNumKey - 1)
		{// 現在のキーフレーム数がそのモーションのキーフレームの総数を上回った
			if (g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].bLoop == true)
			{// ループする判定だったら
				g_Player[nCntPlayer].nKey = 0;             // 現在のキーフレームを元に戻す
				// キーフレーム切り替え処理
				SwitchKey(nCntPlayer);
			}
			else
			{// ループしない判定だったら
				if (g_Player[nCntPlayer].bWeakAction == true || g_Player[nCntPlayer].bStorongAction == true)
				{// アクション状態だったら
					g_Player[nCntPlayer].state = PLAYERSTATE_NORMAL;  // 通常状態に

					// モーション切り替え処理
					SwitchMotion(nCntPlayer);
				}
				else if (g_Player[nCntPlayer].state == PLAYERSTATE_DAMAGE)
				{// ダメージ状態だったら
					if (g_Player[(nCntPlayer + 1) % MAX_PLAYER].bUltimate == false)
					{
						g_Player[nCntPlayer].state = PLAYERSTATE_NORMAL;
						// モーション切り替え処理
						SwitchMotion(nCntPlayer);
						g_Player[nCntPlayer].bControll = true;
					}
					else if(g_Player[(nCntPlayer + 1) % MAX_PLAYER].bUltimate == true)
					{
						g_Player[nCntPlayer].state = PLAYERSTATE_DAMAGE;
						// モーション切り替え処理
						SwitchMotion(nCntPlayer);
					}
				}
				else if (g_Player[nCntPlayer].state == PLAYERSTATE_DAMAGE_DOWN)
				{// ダウン状態だったら
					if (g_Player[nCntPlayer].nLife >= 0)
					{// 体力がまだある
						g_Player[nCntPlayer].state = PLAYERSTATE_DAMAGE_GETUP;
						// モーション切り替え処理
						SwitchMotion(nCntPlayer);
					}
				}
				else if (g_Player[nCntPlayer].state == PLAYERSTATE_DAMAGE_GETUP)
				{// 起き上がり状態だったら
					g_Player[nCntPlayer].state = PLAYERSTATE_NORMAL;
					// モーション切り替え処理
					SwitchMotion(nCntPlayer);
					g_Player[nCntPlayer].bControll = true;
				}
				else
				{// それ以外のモーションならば
					g_Player[nCntPlayer].mState = MOTIONSTATE_STOP;
				}
			}
		}
		else
		{
			g_Player[nCntPlayer].nKey++;         // 現在のキーフレームを進める

			// キーフレーム切り替え処理
			SwitchKey(nCntPlayer);
		}
		g_Player[nCntPlayer].nMotionCounter = 0;   // カウンターを戻す
	}
}
//=============================================================================
// キーフレーム切り替え処理
//=============================================================================
void SwitchKey(int nCntPlayer)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{// モデルのパーツ数分繰り返し
	 // 目標の座標設定
		g_Player[nCntPlayer].aModel[nCntModel].posDiff.x = g_Player[nCntPlayer].aModel[nCntModel].posStd.x - (g_Player[nCntPlayer].aModel[nCntModel].pos.x - g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].posAdd[nCntModel].x);
		g_Player[nCntPlayer].aModel[nCntModel].posDiff.y = g_Player[nCntPlayer].aModel[nCntModel].posStd.y - (g_Player[nCntPlayer].aModel[nCntModel].pos.y - g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].posAdd[nCntModel].y);
		g_Player[nCntPlayer].aModel[nCntModel].posDiff.z = g_Player[nCntPlayer].aModel[nCntModel].posStd.z - (g_Player[nCntPlayer].aModel[nCntModel].pos.z - g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].posAdd[nCntModel].z);

		// 角度の差分の修正
		g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.x = g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].DestAngle[nCntModel].x - g_Player[nCntPlayer].aModel[nCntModel].rot.x;   // 現在の向きと目的の向きの差分を計算

		if (g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.x > D3DX_PI)
		{// 差分がD3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.x -= D3DX_PI * 2.0f;
		}
		if (g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.x < -D3DX_PI)
		{// 差分が-D3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.x += D3DX_PI * 2.0f;
		}
		// 角度の差分の修正
		g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.y = g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].DestAngle[nCntModel].y - g_Player[nCntPlayer].aModel[nCntModel].rot.y;   // 現在の向きと目的の向きの差分を計算

		if (g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.y > D3DX_PI)
		{// 差分がD3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.y -= D3DX_PI * 2.0f;
		}
		if (g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.y < -D3DX_PI)
		{// 差分が-D3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.y += D3DX_PI * 2.0f;
		}

		// 角度の差分の修正
		g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.z = g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].DestAngle[nCntModel].z - g_Player[nCntPlayer].aModel[nCntModel].rot.z;   // 現在の向きと目的の向きの差分を計算

		if (g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.z > D3DX_PI)
		{// 差分がD3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.z -= D3DX_PI * 2.0f;
		}
		if (g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.z < -D3DX_PI)
		{// 差分が-D3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.z += D3DX_PI * 2.0f;
		}
	}
}

//=============================================================================
// モーション切り替え処理
//=============================================================================
void SwitchMotion(int nCntPlayer)
{
	g_Player[nCntPlayer].nMotionCounter = 0;          // モーションカウンターをリセット
	g_Player[nCntPlayer].nKey = 0;                    // 現在のキーフレームをリセット
	g_Player[nCntPlayer].mState = MOTIONSTATE_SWITCH; // モーション切り替え状態に
	if (g_Player[nCntPlayer].state != PLAYERSTATE_STORONG_ATTACK
		&& g_Player[nCntPlayer].state != PLAYERSTATE_WEAK_ATTACK_0
		&& g_Player[nCntPlayer].state != PLAYERSTATE_WEAK_ATTACK_1
		&& g_Player[nCntPlayer].state != PLAYERSTATE_WEAK_ATTACK_2
		&& g_Player[nCntPlayer].state != PLAYERSTATE_ULTIMATE
		&& g_Player[nCntPlayer].state != PLAYERSTATE_ULTIMATE_START)
	{// 攻撃のアクションではない
		if (g_Player[nCntPlayer].bWeakAction == true)
		{
			g_Player[nCntPlayer].bWeakAction = false;
			g_Player[nCntPlayer].wAttackNumber = 0;
		}
		if (g_Player[nCntPlayer].bStorongAction == true)
		{
			g_Player[nCntPlayer].bStorongAction = false;
		}
		if (g_Player[nCntPlayer].bUltimate == true)
		{
			g_Player[nCntPlayer].bUltimate = false;
		}
	}

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{// モデルのパーツ数分繰り返し
	 // 目標の座標設定
		g_Player[nCntPlayer].aModel[nCntModel].posDiff.x = g_Player[nCntPlayer].aModel[nCntModel].posStd.x - (g_Player[nCntPlayer].aModel[nCntModel].pos.x - g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].posAdd[nCntModel].x);
		g_Player[nCntPlayer].aModel[nCntModel].posDiff.y = g_Player[nCntPlayer].aModel[nCntModel].posStd.y - (g_Player[nCntPlayer].aModel[nCntModel].pos.y - g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].posAdd[nCntModel].y);
		g_Player[nCntPlayer].aModel[nCntModel].posDiff.z = g_Player[nCntPlayer].aModel[nCntModel].posStd.z - (g_Player[nCntPlayer].aModel[nCntModel].pos.z - g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].posAdd[nCntModel].z);

		// 角度の差分の修正
		g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.x = g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].DestAngle[nCntModel].x - g_Player[nCntPlayer].aModel[nCntModel].rot.x;   // 現在の向きと目的の向きの差分を計算

		if (g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.x > D3DX_PI)
		{// 差分がD3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.x -= D3DX_PI * 2.0f;
		}
		if (g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.x < -D3DX_PI)
		{// 差分が-D3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.x += D3DX_PI * 2.0f;
		}
		// 角度の差分の修正
		g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.y = g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].DestAngle[nCntModel].y - g_Player[nCntPlayer].aModel[nCntModel].rot.y;   // 現在の向きと目的の向きの差分を計算

		if (g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.y > D3DX_PI)
		{// 差分がD3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.y -= D3DX_PI * 2.0f;
		}
		if (g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.y < -D3DX_PI)
		{// 差分が-D3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.y += D3DX_PI * 2.0f;
		}

		// 角度の差分の修正
		g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.z = g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].Key[g_Player[nCntPlayer].nKey].DestAngle[nCntModel].z - g_Player[nCntPlayer].aModel[nCntModel].rot.z;   // 現在の向きと目的の向きの差分を計算

		if (g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.z > D3DX_PI)
		{// 差分がD3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.z -= D3DX_PI * 2.0f;
		}
		if (g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.z < -D3DX_PI)
		{// 差分が-D3DX_PIを超えた
			g_Player[nCntPlayer].aModel[nCntModel].DiffAngle.z += D3DX_PI * 2.0f;
		}
	}
}
//=============================================================================
// プレイヤーの取得
//=============================================================================
Player *GetPlayer(int nCntPlayer)
{
	return &g_Player[nCntPlayer];
}

//=============================================================================
// プレイヤー同士の攻撃判定処理
//=============================================================================
void PlayerAttack(int nCntPlayer)
{
	if (g_Player[nCntPlayer].bWeakAction == true)
	{// 弱攻撃をしている
		SetEffect(D3DXVECTOR3(g_Player[nCntPlayer].aModel[5].mtxWorld._41, g_Player[nCntPlayer].aModel[5].mtxWorld._42, g_Player[nCntPlayer].aModel[5].mtxWorld._43), D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f), 10.0f, 10);
		SetEffect(D3DXVECTOR3(g_Player[nCntPlayer].aModel[5].mtxWorld._41, g_Player[nCntPlayer].aModel[5].mtxWorld._42, g_Player[nCntPlayer].aModel[5].mtxWorld._43), D3DXCOLOR(1.0f, 0.0f, 0.2f, 1.0f), 15.0f, 10);
	}
	else if (g_Player[nCntPlayer].bStorongAction == true)
	{// 強攻撃をしている
		if (g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].nNumKey >= 3)
		{// 当たり判定開始タイミングになった
			SetEffect(D3DXVECTOR3(g_Player[nCntPlayer].aModel[11].mtxWorld._41, g_Player[nCntPlayer].aModel[11].mtxWorld._42, g_Player[nCntPlayer].aModel[11].mtxWorld._43), D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f), 13.0f, 10);
			SetEffect(D3DXVECTOR3(g_Player[nCntPlayer].aModel[11].mtxWorld._41, g_Player[nCntPlayer].aModel[11].mtxWorld._42, g_Player[nCntPlayer].aModel[11].mtxWorld._43), D3DXCOLOR(1.0f, 0.0f, 0.2f, 1.0f), 18.0f, 10);
		}
	}
	else if (g_Player[nCntPlayer].bUltimate == true)
	{// 必殺攻撃をしている
		SetEffect(D3DXVECTOR3(g_Player[nCntPlayer].aModel[5].mtxWorld._41, g_Player[nCntPlayer].aModel[5].mtxWorld._42, g_Player[nCntPlayer].aModel[5].mtxWorld._43), D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f), 10.0f, 10);
		SetEffect(D3DXVECTOR3(g_Player[nCntPlayer].aModel[5].mtxWorld._41, g_Player[nCntPlayer].aModel[5].mtxWorld._42, g_Player[nCntPlayer].aModel[5].mtxWorld._43), D3DXCOLOR(1.0f, 0.0f, 0.2f, 1.0f), 15.0f, 10);
		SetEffect(D3DXVECTOR3(g_Player[nCntPlayer].aModel[8].mtxWorld._41, g_Player[nCntPlayer].aModel[8].mtxWorld._42, g_Player[nCntPlayer].aModel[8].mtxWorld._43), D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f), 13.0f, 10);
		SetEffect(D3DXVECTOR3(g_Player[nCntPlayer].aModel[8].mtxWorld._41, g_Player[nCntPlayer].aModel[8].mtxWorld._42, g_Player[nCntPlayer].aModel[8].mtxWorld._43), D3DXCOLOR(1.0f, 0.0f, 0.2f, 1.0f), 18.0f, 10);

		if (GetXInputTrigger(nCntPlayer, XIJS_BUTTON_11) == true)
		{// Bボタンが押されている
			PlayerDamage((nCntPlayer + 1) % MAX_PLAYER, 5);
		}

		if (GetUltGaugeState(nCntPlayer) == ULTGAUGE_NOMAL)
		{// 必殺ゲージが空になった
		   // 攻撃側の状態を戻す
			g_Player[nCntPlayer].state = PLAYERSTATE_NORMAL;
			SwitchMotion(nCntPlayer);

			g_Player[(nCntPlayer + 1) % MAX_PLAYER].bHit = false;

			// 防衛側をぶっ飛ばす
			g_Player[(nCntPlayer + 1) % MAX_PLAYER].state = PLAYERSTATE_DAMAGE_FLY;
			SwitchMotion((nCntPlayer + 1) % MAX_PLAYER);
			g_Player[(nCntPlayer + 1) % MAX_PLAYER].move.x -= sinf(g_Player[nCntPlayer].rot.y) * 20.0f;
			g_Player[(nCntPlayer + 1) % MAX_PLAYER].move.y += 7.0f;
			g_Player[(nCntPlayer + 1) % MAX_PLAYER].bDown = true;

			SetRush(false);
		}
	}

	if (g_Player[(nCntPlayer + 1) % MAX_PLAYER].bDown == false)
	{// プレイヤーがダウンしていない
		if (g_Player[nCntPlayer].bHit == false)
		{// 攻撃がヒットしていない
			if (g_Player[nCntPlayer].bWeakAction == true)
			{// 弱攻撃をしている
				D3DXVECTOR3 ColPos = D3DXVECTOR3(g_Player[nCntPlayer].aModel[5].mtxWorld._41, g_Player[nCntPlayer].aModel[5].mtxWorld._42, g_Player[nCntPlayer].aModel[5].mtxWorld._43);
				if (g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].nNumKey >= 1)
				{// 当たり判定開始タイミングになった
					CollisionAttack((nCntPlayer + 1) % MAX_PLAYER, ColPos, 15.0f);
				}
			}

			if (g_Player[nCntPlayer].bStorongAction == true)
			{// 弱攻撃をしている
				if (g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].nNumKey >= 3)
				{// 当たり判定開始タイミングになった
					CollisionAttack((nCntPlayer + 1) % MAX_PLAYER, D3DXVECTOR3(g_Player[nCntPlayer].aModel[11].mtxWorld._41, g_Player[nCntPlayer].aModel[11].mtxWorld._42, g_Player[nCntPlayer].aModel[11].mtxWorld._43), 15.0f);
				}
			}
			if (g_Player[nCntPlayer].state == PLAYERSTATE_ULTIMATE_START)
			{// 必殺技開始状態だったら
				D3DXVECTOR3 ColPos = D3DXVECTOR3(g_Player[nCntPlayer].aModel[5].mtxWorld._41, g_Player[nCntPlayer].aModel[5].mtxWorld._42, g_Player[nCntPlayer].aModel[5].mtxWorld._43);
				if (g_Player[nCntPlayer].aMotion[g_Player[nCntPlayer].state].nNumKey >= 1)
				{// 当たり判定開始タイミングになった
					if (CollisionAttack((nCntPlayer + 1) % MAX_PLAYER, ColPos, 15.0f) == true)
					{// ヒットした
						g_Player[nCntPlayer].state = PLAYERSTATE_ULTIMATE;
						SwitchMotion(nCntPlayer);
						g_Player[nCntPlayer].bUltimate = true;
						SetUltGaugeState(ULTGAUGE_USE, nCntPlayer);

						g_Player[(nCntPlayer + 1) % MAX_PLAYER].state = PLAYERSTATE_DAMAGE;
						SwitchMotion((nCntPlayer + 1) % MAX_PLAYER);
					}
				}
			}
		}
	}
}

//=============================================================================
// プレイヤー同士の当たり判定処理
//=============================================================================
bool CollisionAttack(int nCntPlayer, D3DXVECTOR3 pos, float fRadius)
{
	bool bCol = false;  // 当たったかどうか
	if (pos.y - fRadius < g_Player[nCntPlayer].pos.y + 70.0f
		&& pos.y + fRadius > g_Player[nCntPlayer].pos.y)
	{// 武器が敵のY座標の中にいる
		if (pos.x - fRadius < g_Player[nCntPlayer].pos.x + 20.0f
			&& pos.x + fRadius >  g_Player[nCntPlayer].pos.x - 20.0f)
		{// 攻撃した側が防衛側のX座標の中にいる
			int nDamage = 0;
			if (g_Player[(nCntPlayer + 1) % MAX_PLAYER].bWeakAction == true)
			{// 弱攻撃だったら
				nDamage = 10;
				SetUltGauge(30.0f, (nCntPlayer + 1) % MAX_PLAYER);
			}
			if (g_Player[(nCntPlayer + 1) % MAX_PLAYER].bStorongAction == true)
			{// 強攻撃だったら
				nDamage = 20;
				SetUltGauge(50.0f, (nCntPlayer + 1) % MAX_PLAYER);
			}
			if (g_Player[(nCntPlayer + 1) % MAX_PLAYER].state == PLAYERSTATE_ULTIMATE_START)
			{// 強攻撃だったら
				nDamage = 5;
				g_Player[(nCntPlayer + 1) % MAX_PLAYER].bUltimate = true;
				SetRush(true);
			}
			// ダメージ処理
			PlayerDamage(nCntPlayer, nDamage);
			g_Player[(nCntPlayer + 1) % MAX_PLAYER].bHit = true;  // 攻撃がヒットした判定に
			bCol = true;  // 当たった判定にする
		}
	}

	return bCol;
}

//=============================================================================
// プレイヤーのダメージ処理
//=============================================================================
void PlayerDamage(int nCntPlayer, int nDamage)
{
	// 体力を減らす
	g_Player[nCntPlayer].nLife -= nDamage;
	g_Player[nCntPlayer].bControll = false;

	// 体力ゲージを減らす
	SetLifeGauge((float)nDamage, nCntPlayer);

	if (g_Player[nCntPlayer].nLife <= 0)
	{// 体力が0になった
		g_Player[nCntPlayer].move.x -= sinf(g_Player[(nCntPlayer + 1) % MAX_PLAYER].rot.y) * 20.0f;
		g_Player[nCntPlayer].move.y += 7.0f;
		g_Player[nCntPlayer].bDown = true;

		g_Player[nCntPlayer].state = PLAYERSTATE_DAMAGE_FLY;
		SwitchMotion(nCntPlayer);
		if (GetGameState() != GAMESTATE_END)
		{// ゲーム終了状態でない
			SetGameState(GAMESTATE_END);
			RESULT result = (RESULT)((nCntPlayer + 1) % MAX_PLAYER);
			SetResult(result);
		}
	}
	else
	{// 体力が残っている
		if (g_Player[(nCntPlayer + 1) % MAX_PLAYER].bUltimate == false)
		{// 必殺状態ではない
			if (g_Player[(nCntPlayer + 1) % MAX_PLAYER].state == PLAYERSTATE_WEAK_ATTACK_2 || g_Player[(nCntPlayer + 1) % MAX_PLAYER].state == PLAYERSTATE_STORONG_ATTACK)
			{// 強攻撃か弱攻撃の3発目がヒットした
				g_Player[nCntPlayer].move.x -= sinf(g_Player[(nCntPlayer + 1) % MAX_PLAYER].rot.y) * 20.0f;
				g_Player[nCntPlayer].move.y += 7.0f;
				g_Player[nCntPlayer].bDown = true;

				g_Player[nCntPlayer].state = PLAYERSTATE_DAMAGE_FLY;
				SwitchMotion(nCntPlayer);
				PlaySound(SOUND_LABEL_SE_DAMAGE);
			}
			else
			{// それ以外の攻撃がヒットした
				g_Player[nCntPlayer].move.x -= sinf(g_Player[(nCntPlayer + 1) % MAX_PLAYER].rot.y) * 2.0f;

				g_Player[nCntPlayer].state = PLAYERSTATE_DAMAGE;
				SwitchMotion(nCntPlayer);

				PlaySound(SOUND_LABEL_SE_DAMAGE);
			}
		}
	}
}

//=============================================================================
// プレイヤーの状態遷移処理
//=============================================================================
void CollisionPlayer(int nCntPlayer)
{
	if (g_Player[nCntPlayer].pos.x >= 850.0f)
	{
		g_Player[nCntPlayer].pos.x = 850.0f;
	}
	if (g_Player[nCntPlayer].pos.x <= -850.0f)
	{
		g_Player[nCntPlayer].pos.x = -850.0f;
	}

	if (g_Player[nCntPlayer].pos.y <= g_Player[(nCntPlayer + 1) % MAX_PLAYER].pos.y + 100.0f)
	{
		if (g_Player[nCntPlayer].posold.x >= g_Player[(nCntPlayer + 1) % MAX_PLAYER].pos.x + 30.0f && g_Player[nCntPlayer].pos.x < g_Player[(nCntPlayer + 1) % MAX_PLAYER].pos.x + 30.0f)
		{
			g_Player[nCntPlayer].pos.x = g_Player[(nCntPlayer + 1) % MAX_PLAYER].pos.x + 30.0f;
		}
		if (g_Player[nCntPlayer].posold.x <= g_Player[(nCntPlayer + 1) % MAX_PLAYER].pos.x - 30.0f && g_Player[nCntPlayer].pos.x > g_Player[(nCntPlayer + 1) % MAX_PLAYER].pos.x - 30.0f)
		{
			g_Player[nCntPlayer].pos.x = g_Player[(nCntPlayer + 1) % MAX_PLAYER].pos.x - 30.0f;
		}
	}
}