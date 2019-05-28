//*****************************************************************************
//
//     モードの処理[mode.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MODE_H_
#define _MODE_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     前方宣言
//*****************************************************************************
class CScene2D;
class CScene2DFlash;
class CScene2DPress;
class CTextureManager;
class CPlayerManager;
class CPlayer;
class CEffectManager;
class CMap;
class CCamera;
class CPause;

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define MAX_CAMERA_NUM   (2)
#define MAX_TEXTURE		 (3)

//*****************************************************************************
//     モードベースクラスの定義
//*****************************************************************************
class CBaseMode
{
public:    // 誰でもアクセス可能
	CBaseMode();
	~CBaseMode();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	void SetModeCounter(const int nModeCounter);
	void SetPress(CScene2DPress *pPress);
	void SetTextureManager(CTextureManager *pTextureManager);
	void SetModeChange(const bool bModeChange);
	void SetPlayerManager(CPlayerManager *pPlayerManager);
	void SetPlayer(CPlayer *pPlayer);
	void SetEffectManager(CEffectManager *pEffectManager);
	void SetMap(CMap *pMap);
	void SetCamera(CCamera *pCamera, int nIdx);

	int GetModeCounter(void);
	CScene2DPress *GetPress(void);
	CTextureManager *GetTextureManager(void);
	bool GetModeChange(void);
	CPlayerManager *GetPlayerManager(void);
	CPlayer *GetPlayer(void);
	CEffectManager *GetEffectManager(void);
	CMap *GetMap(void);
	CCamera *GetCamera(int nIdx);

protected: // このクラスと派生クラスだけがアクセス可能
	void ModeChange(int nAutoNext, int nPressNext, int nChangeTiming);
	void ReleaseTextureManager(void);
	void ReleasePlayerManager(void);
	void ReleaseEffectManager(void);
	void ReleaseMap(void);
	void ReleaseCamera(void);

private:   // このクラスだけがアクセス可能
	CScene2DPress   *m_pPress;                  // PRESSENTERクラスへのポインタ
	CTextureManager *m_pTextureManager;         // テクスチャ管轄クラスへのポインタ
	int             m_nModeCounter;             // モードカウンター(主に自動画面遷移に使用)
	bool            m_bModeChange;              // 画面遷移をしているかどうか
	CPlayerManager  *m_pPlayerManager;          // プレイヤー管轄クラスへのポインタ
	CPlayer         *m_pPlayer;                 // プレイヤークラスへのポインタ
	CEffectManager  *m_pEffectManager;          // エフェクト管轄クラスへのポインタ
	CMap            *m_pMap;                    // マップ管轄クラスへのポインタ
	CCamera         *m_pCamera[MAX_CAMERA_NUM]; // カメラクラスへのポインタ
};

//*****************************************************************************
//     タイトルクラスの定義
//*****************************************************************************
class CTitle : public CBaseMode
{
public:    // 誰でもアクセス可能
	CTitle();
	~CTitle();

	static CTitle *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CScene2D *m_pBg[MAX_TEXTURE];                  // セッティング画面の背景
};

//*****************************************************************************
//     マップ紹介モードクラスの定義
//*****************************************************************************
class CViewMap : public CBaseMode
{
public:    // 誰でもアクセス可能
	CViewMap();
	~CViewMap();

	static CViewMap *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能

};

//*****************************************************************************
//     チュートリアルクラスの定義
//*****************************************************************************
class CTutorial : public CBaseMode
{
public:    // 誰でもアクセス可能
	CTutorial();
	~CTutorial();

	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};

//*****************************************************************************
//     セッティングクラスの定義
//*****************************************************************************
class CSetting : public CBaseMode
{
public:    // 誰でもアクセス可能
	//--------------------
	//  状態
	//--------------------
	typedef enum
	{
		STATE_CHECKJOIN = 0,   // 参加受付状態
		STATE_MAPSELECT,       // マップ選択画面
		STATE_MAX
	}STATE;

	CSetting();
	~CSetting();

	static CSetting *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CheckJoin(void);
	void MapSelect(void);

	STATE m_State;                    // 状態
	int m_nSelectMapIdx;              // マップの番号
	CScene2D *m_pBg;                  // セッティング画面の背景
	CScene2D *m_pJoin[2];             // プレイヤー参加表示用ポリゴン
	CScene2DFlash *m_pMapSelect[3];   // マップ選択表示用ポリゴン
	CScene2DPress *m_pJoinPress;      // 参加受付表示用ポリゴン
	bool m_bJoin[2];                  // 参戦するかどうか(人数分)
	int m_nNumPlayer;                 // 参加する人数
	int m_nOnePlayer;
};

//*****************************************************************************
//     ゲームクラスの定義
//*****************************************************************************
class CGame : public CBaseMode
{
public:    // 誰でもアクセス可能
	CGame();
	~CGame();

	//--------------------
	//  状態
	//--------------------
	typedef enum
	{
		STATE_NORMAL = 0,   // 通常状態
		STATE_END,          // 終了状態
		STATE_MAX
	}STATE;

	static CGame *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ReleasePause(void);
	void SetStarObject(void);

	static void SettingPlayer(const int nControllerIdx);
	static void SetGameMapIdx(const int nGameMapIdx);
	static void SetNumPlayer(const int nNumPlayer);

	void SetState(const STATE state);
	void SetPause(const bool bPause);
	void SetPauseController(const int nPauseController);

	STATE GetState(void);
	bool GetPause(void);
	int GetPauseController(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Normal(void);
	void End(void);

	typedef struct
	{
		bool bUse;
		int nControllerIdx;
	}PLAYER_SETTING;

	static int m_nNumPlayer;
	static int m_nGameMapIdx;
	static PLAYER_SETTING m_PlayerSetting[2];

	STATE m_State;            // 状態
	int m_nStateCounter;      // 状態を管理するカウンター
	bool m_bPause;            // ポーズ中かどうか
	CPause *m_pPause;         // ポーズクラスへのポインタ
	int m_nPauseController;   // ポーズを押したコントローラー

	CPlayer *m_pPlayer[2];
};

//*****************************************************************************
//     リザルトクラスの定義
//*****************************************************************************
class CResult : public CBaseMode
{
public:    // 誰でもアクセス可能
	CResult();
	~CResult();

	static CResult *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetNumPlayer(const int nNumPlayer);
	static void SetWinnerIdx(const int nWinnerIdx);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static int m_nNumPlayer;
	static int m_nWinnerIdx;

	CScene2D *m_pBg[2];
};

#endif