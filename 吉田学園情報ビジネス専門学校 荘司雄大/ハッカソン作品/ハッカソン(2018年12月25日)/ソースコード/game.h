//*****************************************************************************
//
//     ゲームの処理[game.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define GAME_TIMER_PRIORITY    (5)    // タイマーの処理優先順位
#define GAME_SCORE_PRIORITY    (5)    // スコアの処理優先順位

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CPause;
class CTextureManager;
class CMap;
class CTimer;
class CScore;
class CUI;

//*****************************************************************************
//    ゲームクラスの定義
//*****************************************************************************
class CGame : public CScene
{
public:     // 誰でもアクセス可能
	//-----------------
	//  状態
	//-----------------
	typedef enum
	{
		STATE_NORMAL = 0,   // 通常状態
		STATE_PLAYER_DEATH, // プレイヤーが死んでいる状態
		STATE_MAP_CLEAR,    // マップをクリアした状態
		STATE_MAPCHANGE,    // マップを切り替えている状態
		STATE_END,
		STATE_MAX
	}STATE;

	//--------------------
	//  テクスチャの番号
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // ポーズ背景用
		TEX_NUMBER_1,      // ポーズ選択項目用(CONTINUE)
		TEX_NUMBER_2,      // ポーズ選択項目用(RETRY)
		TEX_NUMBER_3,      // ポーズ選択項目用(QUIT)
		TEX_NUMBER_4,      // プレゼント用ビルボード
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CGame(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_GAME);
	~CGame();

	static CGame *Create(void);
	static void LoadGoalModel(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool GetMapClear(void);
	void SetMapClear(bool bMapClear);
	void SetMap(void);

	void BindPauseTexture(CPause *pPause);
	void SetState(STATE state);
	STATE GetState(void);

	int GetNumStageClear(void);
	int GetNumStock(void);

	void CutPresentNum(void);
	void AddScore(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	LPD3DXMESH            m_pMesh;       // メッシュ情報へのポインタ
	LPD3DXBUFFER          m_pBuffMat;    // マテリアル情報へのポインタ
	DWORD                 m_nNumMat;     // マテリアル情報の数
	D3DXVECTOR3           m_VtxMax;      // 頂点座標の最大値へのポインタ
	D3DXVECTOR3           m_VtxMin;      // 頂点座標の最小値へのポインタ

	int             m_nNumStock;          // プレイヤーの残機数
	int             m_nNumPresent;        // プレゼントの残り数
	int             m_nNumStageClear;     // ステージクリアした数
	CUI             *m_pUi;               // UIクラスへのポインタ
	CMap            *m_pMap;              // マップクラスへのポインタ
	STATE           m_State;              // 状態
	int             m_nStateCounter;      // 状態を管理するカウンター
	CTextureManager *m_pTextureManager;   // テクスチャ管理クラスへのポインタ
	bool            m_bMapClear;          // マップをクリアしたかどうか
};

#endif