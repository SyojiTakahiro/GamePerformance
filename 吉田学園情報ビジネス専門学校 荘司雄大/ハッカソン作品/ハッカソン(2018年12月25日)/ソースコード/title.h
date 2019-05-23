//*****************************************************************************
//
//     タイトルの処理[title.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CScene2D;
class CScene2DPress;
class CTextureManager;
class CMap;

//*****************************************************************************
//    タイトルクラスの定義
//*****************************************************************************
class CTitle : public CScene
{
public:     // 誰でもアクセス可能
	//--------------------
	//  テクスチャの番号
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // PressEnter用
		TEX_NUMBER_1,      // タイトルロゴ用
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CTitle(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_TITLE);
	~CTitle();

	static CTitle *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	CMap            *m_pMap;              // マップクラスへのポインタ
	int             m_nCounter;           // 遷移を管理するカウンター
	CScene2D        *m_pTitleLogo;        // タイトルロゴ用の2Dポリゴンクラスへのポインタ
	CScene2DPress   *m_pPress;            // プレスエンター用の2Dポリゴンクラスへのポインタ
	CTextureManager *m_pTextureManager;   // テクスチャ管理クラスへのポインタ
};

#endif