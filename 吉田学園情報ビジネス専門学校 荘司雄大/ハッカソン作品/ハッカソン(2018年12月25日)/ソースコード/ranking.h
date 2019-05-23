//*****************************************************************************
//
//     ランキングの処理[ranking.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define MAX_RANKING  (5)    // ランキングスコアの数

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CMap;
class CNumber;
class CScene2D;
class CTextureManager;

//*****************************************************************************
//    ランキングクラスの定義
//*****************************************************************************
class CRanking : public CScene
{
public:     // 誰でもアクセス可能
	//------------------------
	//  ランキングスコアの状態
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // 何もない状態
		STATE_UPDATE,    // 更新された状態
		STATE_MAX
	}STATE;

	//--------------------
	//  テクスチャの番号
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // 数字用
		TEX_NUMBER_1,      // 順位用
		TEX_NUMBER_2,      // ランキングロゴ用
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CRanking(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_RANKING);
	~CRanking();

	static CRanking *Create(void);
	static void RankingScoreInit(void);
	static void SetRankingScore(int nScore);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	static int      m_nRankingScore[MAX_RANKING];   // ランキングスコア
	static STATE    m_State[MAX_RANKING];           // ランキングスコアの状態
	CMap            *m_pMap;                        // マップクラスへのポインタ
	CScene2D        *m_pRankingLogo;                // ランキングロゴ表示用のポリゴン
	CScene2D        *m_apRankingRank[MAX_RANKING];  // ランキング順位表示用のポリゴン
	CNumber         **m_apNumber[MAX_RANKING];      // ランキングスコア表示用のポリゴン
	int             m_aDigit[MAX_RANKING];          // ランキングスコアの桁数
	int             m_nCounter;                     // 遷移を管理するカウンター
	D3DXVECTOR3     m_Move[MAX_RANKING];            // ランキングスコアの移動量
	CTextureManager *m_pTextureManager;             // テクスチャ管理クラスへのポインタ
};

#endif