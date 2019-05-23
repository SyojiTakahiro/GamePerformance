////=============================================================================
////
//// メイン処理 [main.h]
//// Author : 
////
////=============================================================================
//#ifndef _RESULT_H_
//#define _RESULT_H_
//
////*****************************************************************************
//// ヘッダファイルのインクルード
////*****************************************************************************
//#define _CRT_SECURE_NO_WARNINGS
//#include<stdio.h>						//インクルードファイル
//
//#include "main.h"
//
////*****************************************************************************
//// オブジェクトクラス
////*****************************************************************************
//
//class CResult
//{
//public:
//	CResult();
//	~CResult();
//	static HRESULT Load(void);	//テクスチャの読み込み
//	static void Unload(void);	//テクスチャの開放
//
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//	static CResult *Create(void);
//protected:
//
//private:
//	static LPDIRECT3DTEXTURE9	m_pTexture;				//共有テクスチャへのポインタ
//	CScene2D *m_pCScene2D;
//
//	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// 頂点バッファへのポインタ
//
//};
//
//#endif