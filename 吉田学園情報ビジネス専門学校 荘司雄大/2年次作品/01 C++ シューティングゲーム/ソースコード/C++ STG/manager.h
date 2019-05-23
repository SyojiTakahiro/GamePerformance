//=============================================================================
//
// 管理 [manager.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
// ヘッダファイルのインクルード
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//インクルードファイル

#include "main.h"

//*****************************************************************************
// オブジェクトクラス
//*****************************************************************************
class CPlayer;
class CGauge;
class CGauge2;
class CTitle;
class CTutorial;
class CGame;
class CClear;
class CGameover;
class CResult;
class CFade;
class CSound;
class CInputKeyboard;
class CInputJoypad;

class CManager
{
public:
	typedef enum
	{
		MODE_TITLE = 0,
		MODE_TUTORIAL,
		MODE_GAME,
		MODE_GAMESELECT,
		MODE_GAMECLEAR,
		MODE_GAMEOVER,
		MODE_RESULT,
		MODE_MAX
	}MODE;
	
	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CRenderer *GetRenderer(void);
	static CInputKeyboard *GetInputKeyboard(void);
	static CInputJoypad *GetInputJoypad(void);
	static CSound *GetSound(void);
	static void SetMode(MODE mode);
	static MODE GetMode(void);
	static CClear *GetClear(void);
	static CFade *GetFade(void);
//	static CPauseSelect *GetPauseMode(CPauseSelect::SELECTMODE selectMode);
protected:

private:
	static CRenderer *m_pRenderer;
	static CInputKeyboard *m_pInputKeyboard;
	static CInputJoypad *m_pInputJoypad;
	static CSound *m_pSound;

	static MODE m_mode;

	static CTitle *m_pTitle;
	static CTutorial *m_pTutorial;
	static CGame *m_pGame;
	static CClear *m_pClear;
	static CGameover *m_pGameover;
	static CResult *m_pResult;
	static CFade *m_pFade;
};

#endif