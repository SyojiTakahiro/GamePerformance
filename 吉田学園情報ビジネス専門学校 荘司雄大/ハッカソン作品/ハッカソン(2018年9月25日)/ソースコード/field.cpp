//=============================================================================
//
// 地面の処理 [field.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "field.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FIELD_TEXTURENAME    "FIELD_TEXTURENAME"
#define FIELDSET             "FIELDSET"
#define END_FIELDSET         "END_FIELDSET"
#define MAX_FIELD            (30)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void MakeVertexField(LPDIRECT3DDEVICE9 pDevice);
void MakeIndexField(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9      g_pTextureField[MAX_FIELD] = {}; // テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField[MAX_FIELD] = {}; // 頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9  g_pIdxBuffField[MAX_FIELD] = {}; // インデックスバッファへのポインタ
Field                   g_aField[MAX_FIELD];             // フィールド情報
int                     g_SetField;                      // フィールドをセットした回数
int                     g_SetTexField;                   // フィールドのテクスチャをセットした回数

//=============================================================================
// 初期化処理
//=============================================================================
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	  // デバイスの取得

	FILE *pFile;           // テキストファイルへのポインタ
	char str0[128];        // いらない文字
	char TexFileName[64];  // xファイルの名前
	int TexType;           // テクスチャの種類
	D3DXVECTOR3 pos;       // フィールドの座標
	float Side;            // 幅
	float Depth;           // 奥行
	int XBlock;            // 横の分割数
	int ZBlock;            // 奥行の分割数
	g_SetTexField = 0;     // テクスチャファイルを読み込んだ回数を初期化
	g_SetField = 0;        // セットした回数を初期化

	// テキストファイルを開く
	pFile = fopen("data/TEXT/map.txt", "r");

	if (pFile != NULL)
	{// テキストファイルがある
		while (fscanf(pFile, "%s", &str0[0]) != EOF)
		{// テキストファイルの末端まで文字列を改行か空白があるまで読み取る
			if (strcmp(&str0[0], FIELD_TEXTURENAME) == 0)
			{// フィールドのテクスチャのファイル名が書かれていたら
				if (fscanf(pFile, "%s %s", &str0[0], &TexFileName[0]) == 2)
				{// フィールドのテクスチャのファイル名を読み取ったら
				 // テクスチャの読み込み
					D3DXCreateTextureFromFile(pDevice,
						TexFileName,
						&g_pTextureField[g_SetTexField]);
				}
				g_SetTexField++;    // テクスチャ情報を読み込んだ回数を増やす
			}
			if (strcmp(&str0[0], FIELDSET) == 0)
			{// フィールドのセット情報が書かれていたら
				int nCntSet = 0;  // 情報を設定した回数
				while (strcmp(&str0[0], END_FIELDSET) != 0)
				{// フィールドの設定が終わるまで繰り返し
					if (nCntSet == FIELDSET_TEXTYPE)
					{// 1回目だったら
						if (fscanf(pFile, "%s %s %d", &str0[0], &str0[0], &TexType) == 3)
						{// 座標情報を読み取ったら
							g_aField[g_SetField].TexType = TexType;  // テクスチャの種類を設定
						}
					}
					else if (nCntSet == FIELDSET_POS)
					{// 2回目だったら
						if (fscanf(pFile, "%s %f %f %f", &str0[0], &pos.x, &pos.y, &pos.z) == 4)
						{// 座標情報を読み取ったら
							g_aField[g_SetField].pos = D3DXVECTOR3(pos.x, pos.y, pos.z);  // 位置を設定
						}
					}
					else if (nCntSet == FIELDSET_ROT)
					{// 3回目だったら
						if (fscanf(pFile, "%s %f %f %f",&str0[0], &pos.x, &pos.y, &pos.z) == 4)
						{// 向き情報を読み取ったら
							g_aField[g_SetField].rot = D3DXVECTOR3(pos.x, pos.y, pos.z);  // 向きを設定
						}
					}
					else if (nCntSet == FIELDSET_SIDE)
					{// 4回目だったら
						if (fscanf(pFile, "%s %f", &str0[0], &Side) == 2)
						{// 幅の長さ情報を読み取ったら
							g_aField[g_SetField].Side = Side;  // 幅を設定
						}
					}
					else if (nCntSet == FIELDSET_DEPTH)
					{// 5回目だったら
						if (fscanf(pFile, "%s %f", &str0[0], &Depth) == 2)
						{// 奥行の長さ情報を読み取ったら
							g_aField[g_SetField].Depth = Depth;  // 奥行を設定
						}
					}
					else if (nCntSet == FIELDSET_XBLOCK)
					{// 6回目だったら
						if (fscanf(pFile,  "%s %d", &str0[0], &XBlock) == 2)
						{// 横の分割数情報を読み取ったら
							g_aField[g_SetField].XBlock = XBlock;  // 横の分割数を設定
						}
					}
					else if (nCntSet == FIELDSET_ZBLOCK)
					{// 7回目だったら
						if (fscanf(pFile, "%s %d", &str0[0], &ZBlock) == 2)
						{// 奥行の分割数情報を読み取ったら
							g_aField[g_SetField].ZBlock = ZBlock;  // 奥行の分割数を設定
						}
					}
					fscanf(pFile, "%s", &str0[0]);
					nCntSet++;  // セットする情報を変えるために増やす
				}

				// 頂点数,インデックス数,ポリゴン数の設定
				g_aField[g_SetField].nNumVertex = (g_aField[g_SetField].XBlock + 1) * (g_aField[g_SetField].ZBlock + 1);
				g_aField[g_SetField].nNumIndex = ((g_aField[g_SetField].XBlock + 1) * (g_aField[g_SetField].ZBlock + 1)) + (2 * (g_aField[g_SetField].ZBlock - 1)) + (g_aField[g_SetField].XBlock + 1) * (g_aField[g_SetField].ZBlock - 1);
				g_aField[g_SetField].nNumPolygon = ((g_aField[g_SetField].XBlock * g_aField[g_SetField].ZBlock) * 2) + ((g_aField[g_SetField].ZBlock - 1) * 4);
			    
                // 頂点情報の作成
				MakeVertexField(pDevice);

				// インデックス情報の作成
				MakeIndexField(pDevice);
				
				g_SetField++;  // フィールド情報をセットした数を増やす
			}
		}
	}
	// ファイルを閉じて他のプログラムからいじれるようにする
	fclose(pFile);
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitField(void)
{
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{// フィールドをセットできる分繰り返し
		// テクスチャの開放
		if (g_pTextureField[nCntField] != NULL)
		{
			g_pTextureField[nCntField]->Release();
			g_pTextureField[nCntField] = NULL;
		}

		// 頂点バッファの開放
		if (g_pVtxBuffField[nCntField] != NULL)
		{
			g_pVtxBuffField[nCntField]->Release();
			g_pVtxBuffField[nCntField] = NULL;
		}

		// インデックスバッファの開放
		if (g_pIdxBuffField[nCntField] != NULL)
		{
			g_pIdxBuffField[nCntField]->Release();
			g_pIdxBuffField[nCntField] = NULL;
		}
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateField(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;                // 計算用マトリックス

	for (int nCntField = 0; nCntField < g_SetField; nCntField++)
	{// フィールドをセットした回数分繰り返し
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aField[nCntField].mtxWorld);
		// 回転を反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aField[nCntField].rot.y, g_aField[nCntField].rot.x, g_aField[nCntField].rot.z);
		D3DXMatrixMultiply(&g_aField[nCntField].mtxWorld, &g_aField[nCntField].mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_aField[nCntField].pos.x, g_aField[nCntField].pos.y, g_aField[nCntField].pos.z);
		D3DXMatrixMultiply(&g_aField[nCntField].mtxWorld, &g_aField[nCntField].mtxWorld, &mtxTrans);

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aField[nCntField].mtxWorld);

		// 頂点バッファをデバイスのデータストリームにバインド
		pDevice->SetStreamSource(0, g_pVtxBuffField[nCntField], 0, sizeof(VERTEX_3D));

		// インデックスバッファをデバイスのデータストリームにバインド
		pDevice->SetIndices(g_pIdxBuffField[nCntField]);

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureField[g_aField[nCntField].TexType]);

		// ポリゴンの描画
		pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_aField[nCntField].nNumVertex, 0, g_aField[nCntField].nNumPolygon);
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
void MakeVertexField(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファを生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aField[g_SetField].nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffField[g_SetField],
		NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし,頂点データへのポインタを取得
	g_pVtxBuffField[g_SetField]->Lock(0, 0, (void**)&pVtx, 0);

	float XPos = -(g_aField[g_SetField].Side / 2) * g_aField[g_SetField].XBlock;
	float ZPos = (g_aField[g_SetField].Depth / 2) * g_aField[g_SetField].ZBlock;
	float Utex = 0.0f;
	float Vtex = 0.0f;

	for (int nCntVertex = 0; nCntVertex < g_aField[g_SetField].nNumVertex; nCntVertex++)
	{// 頂点数だけ繰り返し
		if (nCntVertex != 0 && nCntVertex % (g_aField[g_SetField].XBlock + 1) == 0)
		{// 横の分割数分設定し終えている
			XPos -= g_aField[g_SetField].Side * (g_aField[g_SetField].XBlock + 1); // X座標を進めていた分戻す 
			ZPos -= g_aField[g_SetField].Depth;                      // Z座標を手前に進める
			Utex -= 1.0f * (g_aField[g_SetField].XBlock + 1);         // テクスチャのU座標を進めていた分を戻す
			Vtex += 1.0f;                                // テクスチャのV座標を1つ分進める
		}
		// 頂点座標
		pVtx[nCntVertex].pos = D3DXVECTOR3(XPos, 0.0f, ZPos);

		// 法線ベクトル
		pVtx[nCntVertex].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラー
		pVtx[nCntVertex].col = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

		// テクスチャ座標
		pVtx[nCntVertex].tex = D3DXVECTOR2(Utex, Vtex);

		XPos += g_aField[g_SetField].Side;  // X座標を横に進める
		Utex += 1.0f;          // テクスチャのU座標を進める
	}

	// 頂点バッファをアンロックする
	g_pVtxBuffField[g_SetField]->Unlock();
}
//=============================================================================
// インデックス情報の作成
//=============================================================================
void MakeIndexField(LPDIRECT3DDEVICE9 pDevice)
{
	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_aField[g_SetField].nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffField[g_SetField],
		NULL);

	WORD *pIdx;       // インデックスデータへのポインタ
	int nCntIdx = 0;  // インデックス番号

	// インデックスバッファをロックし,インデックスデータへのポインタを取得
	g_pIdxBuffField[g_SetField]->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdxY = 0; nCntIdxY < g_aField[g_SetField].ZBlock; nCntIdxY++)
	{// 縦の分割数分繰り返し
		for (int nCntIdxX = 0; nCntIdxX < g_aField[g_SetField].XBlock + 1; nCntIdxX++, nCntIdx++)
		{// 横の分割数+１繰り返し
		 // 上下の二つ分インデックス番号を設定
			pIdx[0] = nCntIdx + g_aField[g_SetField].XBlock + 1; // 下側
			pIdx[1] = nCntIdx;                      // 上側

			pIdx += 2;  // 2つ分進める
			if (nCntIdxY < g_aField[g_SetField].ZBlock - 1 && nCntIdxX == g_aField[g_SetField].XBlock)
			{// 1 , 下側が縦の分割数の最下層ではない
			 // 2 , 横の分割数分設定が終わった
				pIdx[0] = nCntIdx;                            // 上側
				pIdx[1] = nCntIdx + (g_aField[g_SetField].XBlock + 1) + 1; // 次の下側

				pIdx += 2; // 2つ分進める
			}
		}
	}

	// インデックスバッファをアンロックする
	g_pIdxBuffField[g_SetField]->Unlock();
}