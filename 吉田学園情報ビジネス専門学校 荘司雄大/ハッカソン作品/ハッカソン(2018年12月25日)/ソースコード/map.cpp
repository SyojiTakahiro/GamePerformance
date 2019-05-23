//*****************************************************************************
//
//     マップの処理[map.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "map.h"
#include "sky.h"
#include "mountain.h"
#include "object.h"
#include "meshField.h"
#include "modelManager.h"
#include "textureManager.h"
#include "functionlib.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define MAP_OBJECT_SAVEFILENAME "data/TEXT/MAP/OBJECT/save_object.txt"  // 配置物情報を保存するテキストファイル名

// スクリプトファイルの読み込み用のパス名
#define MAPSET               "MAPSET"                // マップセット開始の合図
#define END_MAPSET           "END_MAPSET"            // マップセット終了の合図
#define FIELDSET             "FIELDSET"              // 地面セット開始の合図
#define END_FIELDSET         "END_FIELDSET"          // 地面セット終了の合図
#define OBJECTSET            "OBJECTSET"             // 配置物セット開始の合図
#define END_OBJECTSET        "END_OBJECTSET"         // 配置物セット終了の合図
#define OBJECTMOVESET        "OBJECTMOVESET"         // 動く配置物セット開始の合図
#define END_OBJECTMOVESET    "END_OBJECTMOVESET"     // 動く配置物セット終了の合図
#define MOUNTAINSET          "MOUNTAINSET"           // 山セット開始の合図
#define END_MOUNTAINSET      "END_MOUNTAINSET"       // 山セット終了の合図
#define SKYSET               "SKYSET"                // 空セット開始の合図
#define END_SKYSET           "END_SKYSET"            // 空セット終了の合図

#define FIELD_FILENAME       "FIELD_FILENAME = "     // 地面のスクリプトファイル名
#define OBJECT_FILENAME      "OBJECT_FILENAME = "    // 配置物のスクリプトファイル名
#define MOUNTAIN_FILENAME    "MOUNTAIN_FILENAME = "  // 山のスクリプトファイル名
#define SKY_FILENAME         "SKY_FILENAME = "       // 空のスクリプトファイル名

#define NUM_TEXTURE          "NUM_TEXTURE = "        // 読み込むテクスチャの数
#define TEXTURE_FILENAME     "TEXTURE_FILENAME = "   // 読み込むテクスチャのファイル名
#define NUM_MODEL            "NUM_MODEL = "          // 読み込むモデルの数
#define MODEL_FILENAME       "MODEL_FILENAME = "     // 読み込むモデルファイル名

#define TEX_TYPE             "TEX_TYPE = "           // テクスチャの番号情報
#define TYPE                 "TYPE = "               // 種類情報
#define POS                  "POS = "                // 座標情報
#define POS_TARGET           "POS_TARGET = "         // 目的の位置
#define ROT                  "ROT = "                // 向き情報
#define COL                  "COL = "                // 色情報
#define MOVE                 "MOVE = "               // 移動量情報
#define MOVE_MAX             "MOVE_MAX = "           // 移動量を切り替える時間情報
#define PUSH                 "PUSH = "               // 押せるかどうか
#define BREAK                "BREAK = "              // 壊せるかどうか
#define ROLL_SPEED           "ROLL_SPEED = "         // 回転するスピード量情報
#define CARRY_TIME           "CARRY_TIME = "         // プレイヤーを運ぶのにかける時間
#define COLLISION            "COLLISION = "          // 当たり判定をするかどうか
#define WIDTH                "WIDTH = "              // 1ブロック分の幅情報
#define DEPTH                "DEPTH = "              // 1ブロック分奥行情報
#define RADIUS               "RADIUS = "             // 半径情報
#define HEIGHT               "HEIGHT = "             // 高さ情報
#define XBLOCK               "XBLOCK = "             // 横の分割数情報
#define YBLOCK               "YBLOCK = "             // 縦の分割数情報
#define ZBLOCK               "ZBLOCK = "             // 奥行の分割数情報
#define BINARY_FILENAME      "BINARY_FILENAME = "    // 頂点情報が格納されたバイナリファイル名
#define NEXT_MAPNAME         "NEXT_MAPNAME = "       // 次のマップデータのファイル名

//=============================================================================
//    コンストラクタ
//=============================================================================
CMap::CMap()
{
	// 各種値の初期化
	m_pModelManager = NULL;    // モデル管轄クラスへのポインタ
	m_pTextureManager = NULL;  // テクスチャ管轄クラスへのポインタ
	m_pField = NULL;           // 地面クラスへのポインタ
	m_pMoutain = NULL;         // 円筒クラスへのポインタ
	m_pSky = NULL;             // 空クラスへのポインタ
}

//=============================================================================
//    デストラクタ
//=============================================================================
CMap::~CMap()
{

}

//=============================================================================
//    生成処理
//=============================================================================
CMap *CMap::Create(char *aFileName)
{
	CMap *pMap = NULL;  // マップクラス型のポインタ
	if (pMap == NULL)
	{// メモリが確保できる状態にある
		pMap = new CMap;
		if (pMap != NULL)
		{// メモリが確保できた
		 // 初期化処理を行う
			pMap->Init(aFileName);
		}
		else
		{// メモリが確保できなかった
			return NULL;
		}
	}
	else
	{// メモリが確保できる状態にない
		return NULL;
	}

	// インスタンスを返す
	return pMap;
}

//=============================================================================
//    初期化処理
//=============================================================================
void CMap::Init(char *aFileName)
{
	// マップの外部ファイル名を設定
	strcpy(m_aFileName, aFileName);

	// マップ情報を外部から読み込む
	FILE *pFile = NULL;   // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態である
		pFile = fopen(m_aFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		 // スクリプト読み込み用の変数を用意
			char *pLine = NULL;                         // 1行分読み取り用
			char *pStrCur = NULL;                       // 現在の先頭ポインタ
			char *pStr = NULL;                          // 先頭ポインタ保存用
			int nNumTex = 0;                            // テクスチャを読み込む数
			int nNumModel = 0;                          // モデルを読み込む数
			int nCntTex = 0;                            // テクスチャを読み込んだ回数
			int nCntModel = 0;                          // モデルを読み込んだ回数
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // テクスチャ読み取り用
			LPD3DXMESH pMesh = NULL;                    // メッシュ情報へのポインタ
			LPD3DXBUFFER pBuffMat = NULL;               // マテリアル情報へのポインタ
			DWORD nNumMat = 0;                          // マテリアル情報の数
			char aTexFileName[256] = "\0";              // テクスチャのファイル名

			if (pLine == NULL && pStr == NULL)
			{// メモリが確保できる状態である
				pLine = new char[256];
				pStr = new char[256];
				if (pLine != NULL && pStr != NULL)
				{// メモリを確保できた
				 // 文字列を初期化
					strcpy(pLine, "\0");
					strcpy(pStr, "\0");

					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// スクリプト読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// テクスチャの個数情報があった
								nNumTex = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTex >= 1)
								{// モデル数が1つ以上ある
									m_pTextureManager = CTextureManager::Create(nNumTex);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// テクスチャのファイルパス名情報があった
							 // テクスチャのファイルパス名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, TEXTURE_FILENAME);

								// テクスチャの読み込み
								D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);
								if (pTexture != NULL)
								{// テクスチャを読み込めた
									m_pTextureManager->BindTexture(pTexture, nCntTex);
									m_pTextureManager->SetFileName(pStr, nCntTex);
									pTexture = NULL;
									nCntTex++;
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, NUM_MODEL) == 0)
							{// テクスチャの個数情報があった
								nNumModel = CFunctionLib::ReadInt(pStrCur, NUM_MODEL);
								if (nNumModel >= 1)
								{// モデル数が1つ以上ある
									m_pModelManager = CModelManager::Create(nNumModel);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, MODEL_FILENAME) == 0)
							{// モデルのファイルパス名情報があった
							 // モデルのファイルパス名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MODEL_FILENAME);

								// モデルの読み込み
								// xファイルの読み込み
								D3DXLoadMeshFromX(pStr,
									D3DXMESH_SYSTEMMEM,
									CManager::GetRenderer()->GetDevice(),
									NULL,
									&pBuffMat,
									NULL,
									&nNumMat,
									&pMesh);

								// モデル管轄クラスにポインタを設定する
								m_pModelManager->SetMesh(pMesh, nCntModel);
								m_pModelManager->SetMaterial(CManager::GetRenderer()->GetDevice(), pBuffMat, nNumMat, nCntModel);

								// 使用した変数を初期化しておく
								pMesh = NULL;
								pBuffMat = NULL;
								nNumMat = 0;

								nCntModel++;  // モデルを読み込んだ回数を増やす
							}
							else if (CFunctionLib::Memcmp(pStrCur, FIELD_FILENAME) == 0)
							{// 地面のスクリプトファイル名があった
							 // 地面のスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, FIELD_FILENAME);
								FieldLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, OBJECT_FILENAME) == 0)
							{// 配置物のスクリプトファイル名があった
							 // 配置物のスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, OBJECT_FILENAME);
								ObjectLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOUNTAIN_FILENAME) == 0)
							{// 山のスクリプトファイル名があった
							 // 山のスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MOUNTAIN_FILENAME);
								MountainLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, SKY_FILENAME) == 0)
							{// 空のスクリプトファイル名があった
							    // 空のスクリプトファイル名を読み取る
								pStr = CFunctionLib::ReadString(pStrCur, pStr, SKY_FILENAME);
								SkyLoad(pStr, pStrCur, pLine);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// スクリプト読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}

					// メモリの開放
					if (pStr != NULL)
					{// メモリが確保されている
						delete[] pStr;
						pStr = NULL;
					}
					if (pLine != NULL)
					{// メモリが確保されている
						delete[] pLine;
						pLine = NULL;
					}
				}
			}
			fclose(pFile);
		}
	}
}

//=============================================================================
//    終了処理
//=============================================================================
void CMap::Uninit(void)
{
	// 配置物情報を保存する
	//SaveObject();

	// マップ管轄クラスを開放する
	if (m_pModelManager != NULL)
	{
		m_pModelManager->Uninit();

		// メモリの開放
		delete m_pModelManager;
		m_pModelManager = NULL;
	}

	// テクスチャ管轄クラスを開放する
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// メモリの開放
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// 地面の破棄
	if (m_pField != NULL)
	{// メモリが確保されている
		m_pField = NULL;
	}

	// 山の破棄
	if (m_pMoutain != NULL)
	{// メモリが確保されている
		m_pMoutain = NULL;
	}

	// 空の破棄
	if (m_pSky != NULL)
	{// メモリが確保されている
		m_pSky = NULL;
	}
}

//=============================================================================
//    地面情報読み込み処理
//=============================================================================
void CMap::FieldLoad(char *pFieldFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pFieldFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		 // 地面用の格納変数
			int nFieldTexIdx = 0;                                    // 地面のテクスチャ番号
			D3DXVECTOR3 FieldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 地面の座標
			D3DXVECTOR3 FieldRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 地面の向き
			D3DXCOLOR FieldCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // 地面の色
			float FieldWidth = 0.0f;                                 // 地面の1ブロック分の幅
			float FieldDepth = 0.0f;                                 // 地面の1ブロック分の奥行
			int FieldXblock = 0;                                     // 地面の横のブロック数
			int FieldZblock = 0;                                     // 地面の奥行のブロック数
			char aFileName[256] = "\0";                              // 地面の頂点情報が格納されたバイナリファイル名

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, FIELDSET) == 0)
					{// 地面情報読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// 使用するテクスチャの番号情報だった
								nFieldTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報だった
								FieldPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報だった
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								FieldRot.x = D3DXToRadian(Rot.x);
								FieldRot.y = D3DXToRadian(Rot.y);
								FieldRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// 色情報だった
								FieldCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, WIDTH) == 0)
							{// 1ブロック分の幅情報だった
								FieldWidth = CFunctionLib::ReadFloat(pStrCur, WIDTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, DEPTH) == 0)
							{// 1ブロック分の幅情報だった
								FieldDepth = CFunctionLib::ReadFloat(pStrCur, DEPTH);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// 横の分割数だった
								FieldXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ZBLOCK) == 0)
							{// 奥行の分割数だった
								FieldZblock = CFunctionLib::ReadInt(pStrCur, ZBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, BINARY_FILENAME) == 0)
							{// 頂点情報が格納されたバイナリファイル名だった
								pStrCur = CFunctionLib::ReadString(pStrCur, aFileName, BINARY_FILENAME);
								strcpy(aFileName, pStrCur);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_FIELDSET) == 0)
							{// 地面情報読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
					{// スクリプト読み込み終了の合図だった
						break;  // ループ終了
					}
				}
				// 地面を生成する
				m_pField = CMeshField::Create(FieldPos, FieldRot, FieldCol, FieldWidth, FieldDepth, FieldXblock, FieldZblock, aFileName);
				if (m_pField != NULL)
				{// 地面が生成できた
					m_pField->BindTexture(m_pTextureManager->GetTexture(nFieldTexIdx));
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    配置物情報読み込み処理
//=============================================================================
void CMap::ObjectLoad(char *pObjectFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pObjectFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		 // 配置物用の格納変数
			CObject *pObject = NULL;                                        // 配置物クラスへのポインタ
			CObjectMove *pObjectMove = NULL;                                // 動く配置物クラスへのポインタ
			int nObjectType = 0;                                            // 配置物のモデル番号
			D3DXVECTOR3 ObjectPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置物の座標
			D3DXVECTOR3 ObjectRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);          // 配置物の向き
			D3DXVECTOR3 ObjectRollSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 配置物の回転スピード
			D3DXVECTOR3 ObjectMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);         // 配置物の移動量
			int nObjectMaxCounter = 0;                                      // 配置物の移動量を切り替える時間

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, OBJECTSET) == 0)
					{// 配置物情報読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
							{// 使用するモデル番号情報だった
								nObjectType = CFunctionLib::ReadInt(pStrCur, TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報だった
								ObjectPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報だった
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								ObjectRot.x = D3DXToRadian(Rot.x);
								ObjectRot.y = D3DXToRadian(Rot.y);
								ObjectRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
							{// 回転するスピード情報だった
								D3DXVECTOR3 RotSpeed = CFunctionLib::ReadVector3(pStrCur, ROLL_SPEED);
								ObjectRollSpeed.x = D3DXToRadian(RotSpeed.x);
								ObjectRollSpeed.y = D3DXToRadian(RotSpeed.y);
								ObjectRollSpeed.z = D3DXToRadian(RotSpeed.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_OBJECTSET) == 0)
							{// 配置物情報読み込み終了の合図だった
							    // 配置物を生成する
								pObject = CObject::Create(ObjectPos, ObjectRot, ObjectRollSpeed,4);
								if (pObject != NULL)
								{
									pObject->BindModel(m_pModelManager->GetMesh(nObjectType), m_pModelManager->GetBuffMat(nObjectType), m_pModelManager->GetNumMat(nObjectType), m_pModelManager->GetVtxMax(nObjectType), m_pModelManager->GetVtxMin(nObjectType), m_pModelManager->GetTexture(nObjectType));
									pObject->SetModelIdx(nObjectType);
								}
								break;  // ループ終了
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, OBJECTMOVESET) == 0)
					{// 動く配置物情報読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TYPE) == 0)
							{// 使用するモデル番号情報だった
								nObjectType = CFunctionLib::ReadInt(pStrCur, TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報だった
								ObjectPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報だった
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								ObjectRot.x = D3DXToRadian(Rot.x);
								ObjectRot.y = D3DXToRadian(Rot.y);
								ObjectRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
							{// 回転するスピード情報だった
								D3DXVECTOR3 RotSpeed = CFunctionLib::ReadVector3(pStrCur, ROLL_SPEED);
								ObjectRollSpeed.x = D3DXToRadian(RotSpeed.x);
								ObjectRollSpeed.y = D3DXToRadian(RotSpeed.y);
								ObjectRollSpeed.z = D3DXToRadian(RotSpeed.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOVE) == 0)
							{// 移動量情報だった
								ObjectMove = CFunctionLib::ReadVector3(pStrCur, MOVE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MOVE_MAX) == 0)
							{// 移動量を切り替える時間情報だった
								nObjectMaxCounter = CFunctionLib::ReadInt(pStrCur, MOVE_MAX);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_OBJECTMOVESET) == 0)
							{// 動く配置物情報読み込み終了の合図だった
							    // 動く配置物を生成する
								pObjectMove = CObjectMove::Create(ObjectPos, ObjectRot, ObjectRollSpeed, ObjectMove, nObjectMaxCounter);
								if (pObjectMove != NULL)
								{
									pObjectMove->BindModel(m_pModelManager->GetMesh(nObjectType), m_pModelManager->GetBuffMat(nObjectType), m_pModelManager->GetNumMat(nObjectType), m_pModelManager->GetVtxMax(nObjectType), m_pModelManager->GetVtxMin(nObjectType), m_pModelManager->GetTexture(nObjectType));
									pObjectMove->SetModelIdx(nObjectType);
								}
								break;  // ループ終了
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
					{// スクリプト読み込み終了の合図だった
						break;  // ループ終了
					}
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    山情報読み込み処理
//=============================================================================
void CMap::MountainLoad(char *pMoutainFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pMoutainFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		 // 山用の格納変数
			int nMountainTexIdx = 0;                                    // 山のテクスチャ番号
			D3DXVECTOR3 MountainPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 山の座標
			D3DXVECTOR3 MountainRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 山の向き
			D3DXCOLOR MountainCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // 山の色
			float MountainRadius = 0.0f;                                // 山の半径
			float MountainHeight = 0.0f;                                // 山の高さ
			int MountainXblock = 0;                                     // 山の横のブロック数
			int MountainYblock = 0;                                     // 山の縦のブロック数

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, MOUNTAINSET) == 0)
					{// 山情報読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// 使用するテクスチャの番号情報だった
								nMountainTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報だった
								MountainPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報だった
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								MountainRot.x = D3DXToRadian(Rot.x);
								MountainRot.y = D3DXToRadian(Rot.y);
								MountainRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// 色情報だった
								MountainCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, RADIUS) == 0)
							{// 山の半径情報だった
								MountainRadius = CFunctionLib::ReadFloat(pStrCur, RADIUS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, HEIGHT) == 0)
							{// 山の高さ情報だった
								MountainHeight = CFunctionLib::ReadFloat(pStrCur, HEIGHT);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// 横の分割数だった
								MountainXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
							{// 縦の分割数だった
								MountainYblock = CFunctionLib::ReadInt(pStrCur, YBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_MOUNTAINSET) == 0)
							{// 山情報読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
					{// スクリプト読み込み終了の合図だった
						break;  // ループ終了
					}
				}
				// 山を生成する
				m_pMoutain = CMountain::Create(MountainPos, MountainRot, MountainCol, MountainHeight, MountainRadius, MountainXblock, MountainYblock);
				if (m_pMoutain != NULL)
				{// 山が生成できた
					m_pMoutain->BindTexture(m_pTextureManager->GetTexture(nMountainTexIdx));
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    空情報読み込み処理
//=============================================================================
void CMap::SkyLoad(char *pSkyFileName, char *pStrCur, char *pLine)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(pSkyFileName, "r");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		 // 空用の格納変数
			int nSkyTexIdx = 0;                                    // 空のテクスチャ番号
			D3DXVECTOR3 SkyPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 空の座標
			D3DXVECTOR3 SkyRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);    // 空の向き
			D3DXCOLOR SkyCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);  // 空の色
			float SkyRadius = 0.0f;                                // 空の半径
			int SkyXblock = 0;                                     // 空の横のブロック数
			int SkyYblock = 0;                                     // 空の縦のブロック数
			float fRotSpeed = 0.0f;                                // 空の回転するスピード

			pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
			if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
			{// スクリプト読み込み開始の合図だった
				while (1)
				{// ループ開始
					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
					if (CFunctionLib::Memcmp(pStrCur, SKYSET) == 0)
					{// 空情報読み込み開始の合図だった
						while (1)
						{// ループ開始
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // 有効な文字列を見つける
							if (CFunctionLib::Memcmp(pStrCur, TEX_TYPE) == 0)
							{// 使用するテクスチャの番号情報だった
								nSkyTexIdx = CFunctionLib::ReadInt(pStrCur, TEX_TYPE);
							}
							else if (CFunctionLib::Memcmp(pStrCur, POS) == 0)
							{// 座標情報だった
								SkyPos = CFunctionLib::ReadVector3(pStrCur, POS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROT) == 0)
							{// 向き情報だった
								D3DXVECTOR3 Rot = CFunctionLib::ReadVector3(pStrCur, ROT);
								SkyRot.x = D3DXToRadian(Rot.x);
								SkyRot.y = D3DXToRadian(Rot.y);
								SkyRot.z = D3DXToRadian(Rot.z);
							}
							else if (CFunctionLib::Memcmp(pStrCur, COL) == 0)
							{// 色情報だった
								SkyCol = CFunctionLib::ReadVector4(pStrCur, COL);
							}
							else if (CFunctionLib::Memcmp(pStrCur, RADIUS) == 0)
							{// 空の半径情報だった
								SkyRadius = CFunctionLib::ReadFloat(pStrCur, RADIUS);
							}
							else if (CFunctionLib::Memcmp(pStrCur, XBLOCK) == 0)
							{// 横の分割数だった
								SkyXblock = CFunctionLib::ReadInt(pStrCur, XBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, YBLOCK) == 0)
							{// 縦の分割数だった
								SkyYblock = CFunctionLib::ReadInt(pStrCur, YBLOCK);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ROLL_SPEED) == 0)
							{// 縦の分割数だった
								fRotSpeed = CFunctionLib::ReadFloat(pStrCur, ROLL_SPEED);
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SKYSET) == 0)
							{// 空情報読み込み終了の合図だった
								break;  // ループ終了
							}
						}
					}
					else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
					{// スクリプト読み込み終了の合図だった
						break;  // ループ終了
					}
				}
				// 空を生成する
				m_pSky = CSky::Create(SkyPos, SkyRot, SkyCol, SkyRadius, SkyXblock, SkyYblock, fRotSpeed);
				if (m_pSky != NULL)
				{// 空が生成できた
					m_pSky->BindTexture(m_pTextureManager->GetTexture(nSkyTexIdx));
				}
			}
			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    モデル情報を保存する処理
//=============================================================================
void CMap::SaveObject(void)
{
	FILE *pFile = NULL;  // ファイルポインタ
	if (pFile == NULL)
	{// ファイルポインタを確保できる状態にある
		pFile = fopen(MAP_OBJECT_SAVEFILENAME, "w");
		if (pFile != NULL)
		{// ファイルポインタを確保できた
		 // コメント部分を書き込み
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#       配置物情報スクリプトファイル[save_object.txt]\n");
			fprintf(pFile, "#       Auther : Hodaka Niwa\n");
			fprintf(pFile, "#\n");
			fprintf(pFile, "#******************************************************************************\n");
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n\n", SCRIPT);
			fprintf(pFile, "#------------------------------------------------------------------------------\n");
			fprintf(pFile, "#  モデル情報\n");
			fprintf(pFile, "#------------------------------------------------------------------------------\n");

			// オブジェクト情報を保存
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			CObject *pObject = NULL;
			CObjectMove *pObjectMove = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// 優先順位の数だけ繰り返し
				pScene = CScene::GetTop(nCntPriority);
				while (pScene != NULL)
				{// メモリが空になるまで繰り返し
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == CScene::OBJTYPE_OBJECT)
					{// 配置物クラスへのポインタだった
						pObject = (CObject*)pScene;
						if (pObject != NULL)
						{// オブジェクトへのポインタが取得できた
						 // 事前に計算をすませておく
							D3DXVECTOR3 ModelRot = pObject->GetRotStd();
							D3DXVECTOR3 ModelRollSpeed = pObject->GetRollSpeed();
							// 向き
							ModelRot.x = D3DXToDegree(ModelRot.x);
							ModelRot.y = D3DXToDegree(ModelRot.y);
							ModelRot.z = D3DXToDegree(ModelRot.z);

							// 回転するスピード
							ModelRollSpeed.x = D3DXToDegree(ModelRollSpeed.x);
							ModelRollSpeed.y = D3DXToDegree(ModelRollSpeed.y);
							ModelRollSpeed.z = D3DXToDegree(ModelRollSpeed.z);

							// 配置物の情報を書き込み
							fprintf(pFile, "%s\n", OBJECTSET);
							fprintf(pFile, "	%s%d						# 使用するモデルの番号\n", TYPE, pObject->GetModelIdx());
							fprintf(pFile, "	%s%.1f %.1f %.1f			# モデルの座標\n", POS, pObject->GetPos().x, pObject->GetPos().y, pObject->GetPos().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f				# モデルの向き\n", ROT, ModelRot.x, ModelRot.y, ModelRot.z);
							fprintf(pFile, "	%s%.1f %.1f %.1f		# モデルの回転するスピード\n", ROLL_SPEED, ModelRollSpeed.x, ModelRollSpeed.y, ModelRollSpeed.z);
							fprintf(pFile, "%s\n", END_OBJECTSET);
							fprintf(pFile, "\n");
						}
					}
					else if (pScene->GetObjType() == CScene::OBJTYPE_OBJECTMOVE)
					{// 動く配置物クラスへのポインタだった
						pObjectMove = (CObjectMove*)pScene;
						if (pObjectMove != NULL)
						{// オブジェクトへのポインタが取得できた
						 // 事前に計算をすませておく
							D3DXVECTOR3 ModelRot = pObjectMove->GetRotStd();
							D3DXVECTOR3 ModelRollSpeed = pObjectMove->GetRollSpeed();
							// 向き
							ModelRot.x = D3DXToDegree(ModelRot.x);
							ModelRot.y = D3DXToDegree(ModelRot.y);
							ModelRot.z = D3DXToDegree(ModelRot.z);

							// 回転するスピード
							ModelRollSpeed.x = D3DXToDegree(ModelRollSpeed.x);
							ModelRollSpeed.y = D3DXToDegree(ModelRollSpeed.y);
							ModelRollSpeed.z = D3DXToDegree(ModelRollSpeed.z);

							// 配置物の情報を書き込み
							fprintf(pFile, "%s\n", OBJECTMOVESET);
							fprintf(pFile, "	%s%d						# 使用するモデルの番号\n", TYPE, pObjectMove->GetModelIdx());
							fprintf(pFile, "	%s%.1f %.1f %.1f			# モデルの座標\n", POS, pObjectMove->GetPosStd().x, pObjectMove->GetPosStd().y, pObjectMove->GetPosStd().z);
							fprintf(pFile, "	%s%.1f %.1f %.1f				# モデルの向き\n", ROT, ModelRot.x, ModelRot.y, ModelRot.z);
							fprintf(pFile, "	%s%.1f %.1f %.1f		# モデルの回転するスピード\n", ROLL_SPEED, ModelRollSpeed.x, ModelRollSpeed.y, ModelRollSpeed.z);
							fprintf(pFile, "	%s%.1f %.1f %.1f				# モデルの移動量\n", MOVE, pObjectMove->GetMove().x, pObjectMove->GetMove().y, pObjectMove->GetMove().z);
							fprintf(pFile, "	%s%d					# 移動量を切り替える時間\n", MOVE_MAX, pObjectMove->GetMaxMoveCounter());
							fprintf(pFile, "%s\n", END_OBJECTMOVESET);
							fprintf(pFile, "\n");
						}
					}
					pScene = pSceneNext;
				}
			}


			// コメント部分を書き込み
			fprintf(pFile, "%s				#この行は絶対に消さないこと！\n", END_SCRIPT);

			// ファイルを閉じる
			fclose(pFile);
		}
	}
}

//=============================================================================
//    配置物を配置する処理
//=============================================================================
void CMap::SetObject(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 RollSpeed, int nModelIdx)
{
	// 配置物を生成する
	CObject *pObject = CObject::Create(Pos, Rot, RollSpeed);
	if (pObject != NULL)
	{
		pObject->BindModel(m_pModelManager->GetMesh(nModelIdx), m_pModelManager->GetBuffMat(nModelIdx), m_pModelManager->GetNumMat(nModelIdx), m_pModelManager->GetVtxMax(nModelIdx), m_pModelManager->GetVtxMin(nModelIdx), m_pModelManager->GetTexture(nModelIdx));
		pObject->SetModelIdx(nModelIdx);
	}
}

//=============================================================================
//    動く配置物を配置する処理
//=============================================================================
void CMap::SetObjectMove(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 RollSpeed, int nModelIdx, D3DXVECTOR3 Move, int nMaxMoveCounter)
{
	// 動く配置物を生成する
	CObjectMove *pObjectMove = CObjectMove::Create(Pos, Rot, RollSpeed, Move, nMaxMoveCounter);
	if (pObjectMove != NULL)
	{
		pObjectMove->BindModel(m_pModelManager->GetMesh(nModelIdx), m_pModelManager->GetBuffMat(nModelIdx), m_pModelManager->GetNumMat(nModelIdx), m_pModelManager->GetVtxMax(nModelIdx), m_pModelManager->GetVtxMin(nModelIdx), m_pModelManager->GetTexture(nModelIdx));
		pObjectMove->SetModelIdx(nModelIdx);
	}
}

//=============================================================================
//    読み込んだマップのファイル名を取得する処理
//=============================================================================
char *CMap::GetMapName(void)
{
	return m_aFileName;
}

//=============================================================================
//    モデル管轄クラスへのポインタを取得する処理
//=============================================================================
CModelManager *CMap::GetModelManager(void)
{
	return m_pModelManager;
}