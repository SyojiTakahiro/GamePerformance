//*****************************************************************************
//
//     チュートリアルの処理[tutorial.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

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
//    チュートリアルクラスの定義
//*****************************************************************************
class CTutorial : public CScene
{
public:     // 誰でもアクセス可能
	//--------------------
	//  テクスチャの番号
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // PressEnter用
		TEX_NUMBER_1,  // PressEnter用
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CTutorial(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_TUTORIAL);
	~CTutorial();

	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	CScene2D        *m_pScene;
	int             m_nCounter;           // 遷移を管理するカウンター
	CScene2DPress   *m_pPress;            // プレスエンター用の2Dポリゴンクラスへのポインタ
	CTextureManager *m_pTextureManager;   // テクスチャ管理クラスへのポインタ
};

#endif