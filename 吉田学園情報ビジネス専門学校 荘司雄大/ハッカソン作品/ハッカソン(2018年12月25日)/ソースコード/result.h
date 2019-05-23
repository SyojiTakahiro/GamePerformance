//*****************************************************************************
//
//     リザルトの処理[result.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

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

//*****************************************************************************
//    リザルトクラスの定義
//*****************************************************************************
class CResult : public CScene
{
public:     // 誰でもアクセス可能
	//--------------------
	//  RESULT
	//--------------------
	typedef enum
	{
		RESULT_GAMEOVER = 0,
		RESULT_CLEAR,
		RESULT_MAX
	}RESULT;

	//--------------------
	//  テクスチャの番号
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // PressEnter用
		TEX_NUMBER_1,      // PressEnter用
		TEX_NUMBER_2,      // PressEnter用
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CResult(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_RESULT);
	~CResult();

	static CResult *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetResult(RESULT result);
	static RESULT GetResult(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	static RESULT m_Result;                 // クリアしたかどうか
	CScene2D          *m_pScene;
	int             m_nCounter;             // 遷移を管理するカウンター
	CScene2DPress   *m_pPress;              // プレスエンター用の2Dポリゴンクラスへのポインタ
	CTextureManager *m_pTextureManager;     // テクスチャ管理クラスへのポインタ
};

#endif