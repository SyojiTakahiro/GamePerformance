//*****************************************************************************
//
//     システムの処理[system.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
// デバッグログクラス用
#define MAX_DEBUG_LOG             (10000)   // 文字列格納量の最大値

// 入力クラス用
#define NUM_KEY_MAX               (256)     // キーボードの最大数
#define MAX_DEVICE                (4)       // 接続できる入力デバイスの数

#define DI_JOY_STICK_INPUT        (1000)    // スティックの方向入力受付を行う範囲
#define DI_STICK_AXIS_MIN         (15.0f)   // スティックの入力を開始するスティックの軸の最小値

#define MIN_GAMEPAD_LEFT_THUMB_X  (25000)   // 左スティックの横方向の入力を受け付ける軸の最小値
#define MIN_GAMEPAD_LEFT_THUMB_Y  (25000)   // 左スティックの縦方向の入力を受け付ける軸の最小値
#define MIN_GAMEPAD_RIGHT_THUMB_X (25000)   // 右スティックの横方向の入力を受け付ける軸の最小値
#define MIN_GAMEPAD_RIGHT_THUMB_Y (25000)   // 右スティックの縦方向の入力を受け付ける軸の最小値
#define MIN_GAMEPAD_LEFT_TRIGGER  (130)     // LTボタンの入力を受け付ける値の最小値
#define MIN_GAMEPAD_RIGHT_TRIGGER (130)     // RTボタンの入力を受け付ける値の最小値

// ライトクラス用
#define MAX_LIGHT                 (3)       // ライトオブジェクトの総数

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CScene2D;
class CPlayer;

//*****************************************************************************
//    カメラクラスの定義
//*****************************************************************************
class CCamera
{
public:    // 誰でもアクセス可能
	CCamera();
	~CCamera();

	static CCamera *Create(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot, float fLength, D3DVIEWPORT9 ViewPort, CPlayer *pPlayer = NULL);

	HRESULT Init(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 rot, float fLength, D3DVIEWPORT9 ViewPort, CPlayer *pPlayer = NULL);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void SetPosV(const D3DXVECTOR3 posV);
	void SetPosVDest(const D3DXVECTOR3 posVDest);
	void SetPosVDiff(const D3DXVECTOR3 posVDiff);
	void SetPosVOld(const D3DXVECTOR3 posVOld);
	void SetPosR(const D3DXVECTOR3 posR);
	void SetPosRDest(const D3DXVECTOR3 posRDest);
	void SetPosRDiff(const D3DXVECTOR3 posRDiff);
	void SetMoveV(const D3DXVECTOR3 MoveV);
	void SetMoveR(const D3DXVECTOR3 MoveR);
	void SetRot(const D3DXVECTOR3 Rot);
	void SetRotDest(const D3DXVECTOR3 RotDest);
	void SetRotDiff(const D3DXVECTOR3 RotDiff);
	void SetRotOld(const D3DXVECTOR3 RotOld);
	void SetMtxProjection(const D3DXMATRIX MtxProjection);
	void SetMtxView(const D3DXMATRIX MtxView);

	D3DXVECTOR3 GetPosV(void);
	D3DXVECTOR3 GetPosVDest(void);
	D3DXVECTOR3 GetPosVDiff(void);
	D3DXVECTOR3 GetPosVOld(void);
	D3DXVECTOR3 GetPosR(void);
	D3DXVECTOR3 GetPosRDest(void);
	D3DXVECTOR3 GetPosRDiff(void);
	D3DXVECTOR3 GetMoveV(void);
	D3DXVECTOR3 GetMoveR(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetRotDest(void);
	D3DXVECTOR3 GetRotDiff(void);
	D3DXVECTOR3 GetRotOld(void);
	D3DXMATRIX GetMtxProjection(void);
	D3DXMATRIX GetMtxView(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Title(void);
	void ViewMap(void);
	void Tutorial(void);
	void Setting(void);
	void Game(void);
	void Result(void);
	void CalcRot(void);
	void MovieCamera(void);

	CPlayer      *m_pPlayer;        // プレイヤークラスへのポインタ
	D3DXVECTOR3  m_PosV;            // 現在の視点
	D3DXVECTOR3  m_PosVDest;        // 目的の視点
	D3DXVECTOR3  m_PosVDiff;        // 現在の視点と目的の視点の差分
	D3DXVECTOR3  m_PosVOld;         // 前回の視点
	D3DXVECTOR3  m_PosR;            // 現在の注視点
	D3DXVECTOR3  m_PosRDest;        // 目的の注視点
	D3DXVECTOR3  m_PosRDiff;        // 現在の注視点と目的の注視点の差分
	D3DXVECTOR3  m_MoveV;           // 視点の移動量
	D3DXVECTOR3  m_MoveR;           // 注視点の移動量
	D3DXVECTOR3  m_Rot;             // 現在の向き
	D3DXVECTOR3  m_RotDest;         // 目的の向き
	D3DXVECTOR3  m_RotDiff;         // 目的の向きと現在の向きの差分
	D3DXVECTOR3  m_RotOld;          // 前回の向き
	D3DXVECTOR3  m_VecU;            // 上方向ベクトル
	D3DXMATRIX   m_MtxProjection;   // プロジェクションマトリックス
	D3DXMATRIX   m_MtxView;         // ビューマトリックス
	D3DVIEWPORT9 m_ViewPort;        // ビューポート(描画領域)
	float        m_fLength;         // 距離
	bool         m_bMoving;         // カメラを動かせるかどうか(動画用)
};

//*****************************************************************************
//     デバッグ情報クラスの定義
//*****************************************************************************
class CDebugProc
{
public:    // 誰でもアクセス可能
	CDebugProc();
	~CDebugProc();
	void Init(void);
	void Uninit(void);
	void Update(void);
	static void Print(char *fmt, ...);
	static void Draw(void);

	static void SetDisp(bool bDisp);
	static bool GetDisp(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	static LPD3DXFONT	m_pFont;		         // フォントへのポインタ
	static char         m_aStr[MAX_DEBUG_LOG];   // 文字列格納用変数
	static bool         m_bDisp;                 // 描画するかしないか
};


//*****************************************************************************
//    フェードクラスの定義
//*****************************************************************************
class CFade
{
public:     // 誰でもアクセス可能
	//------------------------
	//  フェードの種類
	//------------------------
	typedef enum
	{
		TYPE_MODECHANGE = 0,  // 画面遷移に使用する
		TYPE_SWITCHMAP,       // マップ切り替えに使用する
		TYPE_MAX
	}TYPE;

	//------------------------
	//  フェードの状態
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // 何もしていない状態
		STATE_IN,        // フェードイン状態
		STATE_STOP,      // フェードしたまま止める状態
		STATE_OUT,       // フェードアウト状態
		STATE_MAX
	}STATE;

	CFade();
	~CFade();

	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFade(int modeNext = 0);
	void SetFade(TYPE type = TYPE_SWITCHMAP);

	STATE GetState(void);
	CScene2D *GetScene2D(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	void FadeIn(void);
	void FadeStop(void);
	void FadeOut(void);

	static LPDIRECT3DTEXTURE9 m_pTexture;        // 文字テクスチャへのポインタ

	CScene2D           *m_pScene2D;       // フェードに使用するポリゴン
	CScene2D           *m_pLogo2D;        // 文字ポリゴン
	int                m_NextMode;        // 次に遷移するモード
	STATE              m_State;           // 状態
	TYPE               m_Type;            // 種類
	int                m_nCounter;        // フェードをどれだけ止めるか
};

//*****************************************************************************
//     Inputクラスの定義
//*****************************************************************************
class CInput
{
public:     // 誰でもアクセス可能
	CInput(int nNumDevice = 1);
	virtual ~CInput();
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;

protected:  // このクラスと派生クラスだけがアクセス可能
	static HWND           m_hWnd;        // ウィンドウハンドル
	static LPDIRECTINPUT8 m_pInput;      // DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8  *m_pDevice;    // 入力デバイスへのポインタ
	int                   m_nDeviceNum;  // 入力デバイスをいくつ生成したか
};

//*****************************************************************************
//     キーボードクラスの定義
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:     // 誰でもアクセス可能
	CInputKeyboard(int nNumDevice = 1);
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);
private:    // このクラスだけがアクセス可能
	BYTE   m_aKeyState[NUM_KEY_MAX];          // キーボードの入力情報(プレス情報)
	BYTE   m_aKeyStateTrigger[NUM_KEY_MAX];   // キーボードの入力情報(トリガー情報)
	BYTE   m_aKeyStateRelease[NUM_KEY_MAX];   // キーボードの入力情報(リリース情報)
	BYTE   m_aKeyStateRepeat[NUM_KEY_MAX];    // キーボードの入力情報(リピート情報)
	DWORD  m_aKeyboardCounter[NUM_KEY_MAX];   // キーボードが入力されてる間を数えるカウンター
};

//*****************************************************************************
//     マウスクラスの定義
//*****************************************************************************
class CInputMouse : public CInput
{
public:     // 誰でもアクセス可能
	//------------------------
	//  ボタンの列挙
	//------------------------
	typedef enum
	{
		DIMS_BUTTON_0 = 0,  // マウスの左ボタン
		DIMS_BUTTON_1,      // マウスの右ボタン
		DIMS_BUTTON_2,      // マウスのホイールボタン
		// 以下はボタン自体がないマウスも存在
		DIMS_BUTTON_3,
		DIMS_BUTTON_4,
		DIMS_BUTTON_5,
		DIMS_BUTTON_6,
		DIMS_BUTTON_7,
		DIMS_BUTTON_MAX
	}DIMS_BUTTON;

	CInputMouse(int nNumDevice = 1);
	~CInputMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);

	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelease(int nButton);
	bool GetRepeat(int nButton);

	LONG GetMouseAxisX(void);
	LONG GetMouseAxisY(void);
	LONG GetMouseAxisZ(void);

	POINT GetPoint(void);
	POINT GetPointOld(void);

private:    // このクラスだけがアクセス可能
	BYTE   m_aMouseState[DIMS_BUTTON_MAX];         // マウスの入力情報(プレス情報)
	BYTE   m_aMouseStateTrigger[DIMS_BUTTON_MAX];  // マウスの入力情報(トリガー情報)
	BYTE   m_aMouseStateRelease[DIMS_BUTTON_MAX];  // マウスの入力情報(リリース情報)
	BYTE   m_aMouseStateRepeat[DIMS_BUTTON_MAX];   // マウスの入力情報(リピート情報)
	DWORD  m_aMouseCounter[DIMS_BUTTON_MAX];       // マウスが入力されてる間を数えるカウンター
	LONG   m_MouseXAxis;                           // マウスのX軸を格納する
	LONG   m_MouseYAxis;                           // マウスのY軸を格納する
	LONG   m_MouseZAxis;                           // マウスのZ軸を格納する
	HWND   m_hWnd;                                 // ウィンドウハンドル(カーソル座標を取得するために必要)
	POINT  m_Point;                                // 現在のマウスのカーソル座標
	POINT  m_PointOld;                             // 前回のマウスのカーソル座標
};

//*****************************************************************************
//     ジョイスティッククラスの定義
//*****************************************************************************
class CJoyStick
{
public:     // 誰でもアクセス可能
	//------------------------
	//  ボタンの列挙
	//------------------------
	typedef enum
	{
		DIJS_BUTTON_0 = 0,  // Xボタン
		DIJS_BUTTON_1,      // Yボタン
		DIJS_BUTTON_2,      // Aボタン
		DIJS_BUTTON_3,      // Bボタン
		DIJS_BUTTON_4,      // LBボタン
		DIJS_BUTTON_5,      // RBボタン
		DIJS_BUTTON_6,      // LTボタン
		DIJS_BUTTON_7,      // RTボタン
		DIJS_BUTTON_8,      // 左スティック押し込み
		DIJS_BUTTON_9,      // 右スティック押し込み
		DIJS_BUTTON_10,     // BACKボタン
		DIJS_BUTTON_11,     // STARTボタン
		DIJS_BUTTON_12,
		DIJS_BUTTON_13,
		DIJS_BUTTON_14,
		DIJS_BUTTON_15,
		DIJS_BUTTON_16,
		DIJS_BUTTON_17,
		DIJS_BUTTON_18,
		DIJS_BUTTON_19,
		DIJS_BUTTON_20,      // 左アナログスティック上方向入力
		DIJS_BUTTON_21,      // 左アナログスティック下方向入力
		DIJS_BUTTON_22,      // 左アナログスティック右方向入力
		DIJS_BUTTON_23,      // 左アナログスティック左方向入力
		DIJS_BUTTON_24,      // 右アナログスティック上方向入力
		DIJS_BUTTON_25,      // 右アナログスティック下方向入力
		DIJS_BUTTON_26,      // 右アナログスティック右方向入力
		DIJS_BUTTON_27,      // 右アナログスティック左方向入力
		DIJS_BUTTON_28,      // 十字キー上方向入力
		DIJS_BUTTON_29,      // 十字キー下方向入力
		DIJS_BUTTON_30,      // 十字キー右方向入力
		DIJS_BUTTON_31,      // 十字キー左方向入力
		DIJS_BUTTON_MAX
	}DIJS_BUTTON;

	CJoyStick();
	~CJoyStick();
	HRESULT Init(void);
	void Uninit(void);
	void Update(LPDIRECTINPUTDEVICE8 pDevice);

	bool GetPress(int nButton);
	bool GetTrigger(int nButton);
	bool GetRelease(int nButton);
	bool GetRepeat(int nButton);
	float GetRadian(void);
	float GetLeftAxisX(void);
	float GetLeftAxisY(void);
	float GetLeftAxis(void);
	float GetRightAxisX(void);
	float GetRightAxisY(void);
	float GetRightAxis(void);

private:    // このクラスだけがアクセス可能
	BYTE   m_aJoyState[DIJS_BUTTON_MAX];          // ジョイスティックの入力情報(プレス情報)
	BYTE   m_aJoyStateTrigger[DIJS_BUTTON_MAX];   // ジョイスティックの入力情報(トリガー情報)
	BYTE   m_aJoyStateRelease[DIJS_BUTTON_MAX];   // ジョイスティックの入力情報(リリース情報)
	BYTE   m_aJoyStateRepeat[DIJS_BUTTON_MAX];    // ジョイスティックの入力情報(リピート情報)
	DWORD  m_aJoyCounter[DIJS_BUTTON_MAX];        // ジョイスティックの入力されてる間を数えるカウンター
	float  m_Radian;                              // 十字キーのラジアンを格納する
	float  m_LeftAxis_X;                          // 左アナログスティックのX軸を格納する
	float  m_LeftAxis_Y;                          // 左アナログスティックのY軸を格納する
	float  m_RightAxis_X;                         // 右アナログスティックのX軸を格納する
	float  m_RightAxis_Y;                         // 右アナログスティックのY軸を格納する
};

//*****************************************************************************
//     ジョイスティックオブジェクトクラスの定義
//*****************************************************************************
class CInputJoyStick : public CInput
{
public:     // 誰でもアクセス可能
	CInputJoyStick(int nNumDevice = 1);
	~CInputJoyStick();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	static BOOL PASCAL CallEnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);
	BOOL FAR PASCAL EnumJoyDeviceProc(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

	CJoyStick *GetJoyStickDevice(int nIdxDevice = 0);

private:    // このクラスだけがアクセス可能
	static int m_nNumAll;                        // コントローラーのデバイス数
	CJoyStick *m_apJoyStick[MAX_DEVICE];         // ジョイスティッククラス型のポインタ
};

//*****************************************************************************
//     XInputクラスの定義
//*****************************************************************************
class CXInput
{
public:     // 誰でもアクセス可能
	//------------------------
	//  ボタンの列挙
	//------------------------
	// XINPUT_GAMEPAD_DPAD_UP          // 十字キー上入力
	// XINPUT_GAMEPAD_DPAD_DOWN        // 十字キー下入力
	// XINPUT_GAMEPAD_DPAD_LEFT        // 十字キー左入力
	// XINPUT_GAMEPAD_DPAD_RIGHT       // 十字キー右入力
	// XINPUT_GAMEPAD_START            // STARTボタン
	// XINPUT_GAMEPAD_BACK             // BACKボタン
	// XINPUT_GAMEPAD_LEFT_THUMB       // 左スティックボタン
	// XINPUT_GAMEPAD_RIGHT_THUMB      // 右スティックボタン
	// XINPUT_GAMEPAD_LEFT_SHOULDER    // LBボタン
	// XINPUT_GAMEPAD_RIGHT_SHOULDER   // RBボタン
	// XINPUT_GAMEPAD_A                // Aボタン
	// XINPUT_GAMEPAD_B                // Bボタン
	// XINPUT_GAMEPAD_X                // Xボタン
	// XINPUT_GAMEPAD_Y                // Yボタン
	typedef enum
	{
		XIJS_BUTTON_0,     // 十字キー上入力
		XIJS_BUTTON_1,     // 十字キー下入力
		XIJS_BUTTON_2,     // 十字キー左入力
		XIJS_BUTTON_3,     // 十字キー右入力
		XIJS_BUTTON_4,     // STARTボタン
		XIJS_BUTTON_5,     // BACKボタン
		XIJS_BUTTON_6,     // 左スティック押し込み入力
		XIJS_BUTTON_7,     // 右スティック押し込み入力
		XIJS_BUTTON_8,     // LBボタン
		XIJS_BUTTON_9,     // RBボタン
		XIJS_BUTTON_10,    // Aボタン
		XIJS_BUTTON_11,    // Bボタン
		XIJS_BUTTON_12,    // Xボタン
		XIJS_BUTTON_13,    // Yボタン
		XIJS_BUTTON_14,    // LTボタン
		XIJS_BUTTON_15,    // RTボタン
		XIJS_BUTTON_16,    // 左スティック上入力
		XIJS_BUTTON_17,    // 左スティック下入力
		XIJS_BUTTON_18,    // 左スティック左入力
		XIJS_BUTTON_19,    // 左スティック右入力
		XIJS_BUTTON_20,    // 右スティック上入力
		XIJS_BUTTON_21,    // 右スティック下入力
		XIJS_BUTTON_22,    // 右スティック左入力
		XIJS_BUTTON_23,    // 右スティック右入力

		// 以下割り当てなし
		XIJS_BUTTON_24,
		XIJS_BUTTON_25,
		XIJS_BUTTON_26,
		XIJS_BUTTON_27,
		XIJS_BUTTON_28,
		XIJS_BUTTON_29,
		XIJS_BUTTON_30,
		XIJS_BUTTON_31,
		XIJS_BUTTON_MAX,
	}XIJS_BUTTON;

	CXInput();
	~CXInput();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	BYTE GetLeftTrigger(int nCntPad);
	BYTE GetRightTrigger(int nCntPad);
	float GetLeftAxiz(int nCntPad);
	float GetRightAxiz(int nCntPad);
	SHORT GetThumbLX(int nCntPad);
	SHORT GetThumbLY(int nCntPad);
	SHORT GetThumbRX(int nCntPad);
	SHORT GetThumbRY(int nCntPad);

	bool GetPress(int nCntPad, XIJS_BUTTON Button);
	bool GetTrigger(int nCntPad, XIJS_BUTTON Button);
	bool GetRelease(int nCntPad, XIJS_BUTTON Button);
	bool GetRepeat(int nCntPad, XIJS_BUTTON Button);

private:    // このクラスだけがアクセス可能
	typedef struct
	{
		XINPUT_STATE m_State;     // ゲームパッドの状態
		bool m_bConnected;        // 接続されたかどうか
	}CONTROLER_STATE;

	CONTROLER_STATE m_aGamePad[MAX_DEVICE];                                // ゲームパッドの情報
	DWORD           m_wButtons[MAX_DEVICE];                                // ボタンの状態
	BYTE            m_bLeftTrigger[MAX_DEVICE];                            // LTボタンの状態
	BYTE            m_bRightTrigger[MAX_DEVICE];                           // RTボタンの状態
	SHORT           m_sThumbLX[MAX_DEVICE];                                // 左スティックのX軸
	SHORT           m_sThumbLY[MAX_DEVICE];                                // 左スティックのY軸
	SHORT           m_sThumbRX[MAX_DEVICE];                                // 右スティックのX軸
	SHORT           m_sThumbRY[MAX_DEVICE];                                // 右スティックのY軸
	BYTE            m_aGamePadState[MAX_DEVICE][XIJS_BUTTON_MAX];          // ゲームパッドの入力情報(プレス情報)
	BYTE            m_aGamePadStateTrigger[MAX_DEVICE][XIJS_BUTTON_MAX];   // ゲームパッドの入力情報(トリガー情報)
	BYTE            m_aGamePadStateRelease[MAX_DEVICE][XIJS_BUTTON_MAX];   // ゲームパッドの入力情報(リリース情報)
	BYTE            m_aGamePadStateRepeat[MAX_DEVICE][XIJS_BUTTON_MAX];    // ゲームパッドの入力情報(リピート情報)
	DWORD           m_aGamePadCounter[MAX_DEVICE][XIJS_BUTTON_MAX];        // ゲームパッドの入力されてる間を数えるカウンター
};

//*****************************************************************************
//    ライトクラスの定義
//*****************************************************************************
class CLight
{
public:    // 誰でもアクセス可能
	CLight();
	~CLight();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DLIGHT9 m_Light[MAX_LIGHT];   // ライトの情報

#ifdef _DEBUG  // デバッグに必要な変数
	int       m_nSelectLight;       // 現在選択しているライトの番号
#endif
};

//*****************************************************************************
//    レンダリングクラスの定義
//*****************************************************************************
class CRenderer
{
public:     // 誰でもアクセス可能
	typedef enum
	{
		TARGET_DEFAULT = 0,   // デフォルトの設定
		TARGET_TEXTURE,       // テクスチャ
		TARGET_MAX
	}TARGET;

	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void);

protected:  // このクラスと派生クラスだけがアクセス可能

private:    // このクラスだけがアクセス可能
	void DefaultRender(void);
	void TextureRender(void);

	LPDIRECT3D9				m_pD3D;			        // Direct3Dオブジェクト
	LPDIRECT3DDEVICE9		m_pD3DDevice;	        // Deviceオブジェクト(描画に必要)
	LPDIRECT3DSURFACE9      m_pZBufferSurface;      // Zバッファへのポインタ(テクスチャにレンダリングする際にZバッファを切り替えるため)
	LPDIRECT3DTEXTURE9      m_pTexture;             // テクスチャへのポインタ(このテクスチャにオブジェクトを描画する)
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;             // 頂点バッファへのポインタ(オブジェクトを描画したテクスチャを使用する)
	TARGET                  m_Target;               // オブジェクトを描画するレンダリングターゲット
	float                   m_fRot;                 // ポリゴンの向き
	float                   m_fAngle;               // ポリゴンを出す角度
	float                   m_fWidth;               // ポリゴンの幅
	float                   m_fHeight;              // ポリゴンの高さ
	float                   m_fLength;              // ポリゴンの長さ
};

//*****************************************************************************
//     サウンドクラスの定義
//*****************************************************************************
class CSound
{
public:    // 誰でもアクセス可能
	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(int nIdx, bool bPause = false);
	void StopSound(int nIdx, bool bPause = false);
	void StopSound(void);
	void SetVoluem(int nIdx, float fValue);

private:   // このクラスだけがアクセス可能
	HRESULT CreatePointer(void);
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;						// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice;	// マスターボイスへのポインタ
	IXAudio2SourceVoice **m_apSourceVoice;    	// ソースボイスへのポインタ
	BYTE **m_apDataAudio;					    // オーディオデータへのポインタ
	DWORD *m_apSizeAudio;					    // オーディオデータサイズへのポインタ
	int m_nNumSound;                            // サウンドデータの数

	//------------------------
	//  各音素材のパラメータ
	//------------------------
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;
	SOUNDPARAM *m_apSoundParam;   // 音素材のパラメータへのポインタ
};

#endif