//*****************************************************************************
//
//     マネージャーの処理[manager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    前方宣言(実体がない宣言)
//*****************************************************************************
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoyStick;
class CXInput;
class CSound;
class CLight;
class CCamera;
class CDebugProc;
class CFade;
class CBaseMode;
class CTitle;
class CViewMap;
class CTutorial;
class CSetting;
class CGame;
class CResult;

//*****************************************************************************
//    マネージャクラスの定義
//*****************************************************************************
class CManager
{
public:    // 誰でもアクセス可能
	//------------------------
	//  モードの種類
	//------------------------
	typedef enum
	{
		MODE_TITLE = 0,  // タイトル画面
		MODE_VIEW_MAP,   // マップ紹介画面
		MODE_TUTORIAL,   // チュートリアル画面
		MODE_SETTING,    // セッティング画面
		MODE_GAME,       // ゲーム画面
		MODE_RESULT,     // リザルト画面
		MODE_MAX
	}MODE;

	CManager();
	~CManager();
	HRESULT Init(HINSTANCE hInstance,HWND hWnd,bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void SetMode(int mode);
	static MODE GetMode(void);

	static CRenderer *GetRenderer(void);
	static CLight *GetLight(void);
	static CInputKeyboard *GetKeyboard(void);
	static CInputMouse *GetMouse(void);
	static CInputJoyStick *GetJoyStick(void);
	static CXInput *GetXInput(void);
	static CSound *GetSound(void);

	static CFade *GetFade(void);
	static CBaseMode *GetBaseMode(void);
	static CTitle *GetTitle(void);
	static CViewMap *GetViewMap(void);
	static CTutorial *GetTutorial(void);
	static CSetting *GetSetting(void);
	static CGame *GetGame(void);
	static CResult *GetResult(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static CRenderer         *m_pRenderer;  // レンダリングクラス型のポインタ
	static CLight            *m_pLight;     // ライトクラス型のポインタ
	static CInputKeyboard    *m_pKeyboard;  // キーボードクラス型のポインタ
	static CInputMouse       *m_pMouse;     // マウスクラス型のポインタ
	static CInputJoyStick    *m_pJoyStick;  // ジョイスティッククラス型のポインタ
	static CXInput           *m_pXInput;    // XInputクラス型のポインタ
	static CSound            *m_pSound;     // サウンドクラス型のポインタ
	static CDebugProc        *m_pDebugProc; // デバッグ情報クラス型のポインタ
	static CFade             *m_pFade;      // フェードクラス型のポインタ
	static CBaseMode         *m_pModeBase;  // モードベースクラス型のポインタ
	static MODE              m_Mode;        // 現在のモード
};

#if _DEBUG
//*****************************************************************************
//    プロトタイプ宣言
//*****************************************************************************
bool GetUpdate(void);

#endif

#endif