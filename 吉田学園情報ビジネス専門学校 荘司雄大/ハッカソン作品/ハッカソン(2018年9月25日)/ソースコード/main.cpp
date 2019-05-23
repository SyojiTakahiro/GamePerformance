//=============================================================================
//
// メイン処理 [main.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "main.h"
#include "fade.h"
#include "title.h"
#include "modeselect.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"
#include "rankingscore.h"
#include "pressenter.h"
#include "input.h"
#include "sound.h"
#include "camera.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"	        // ウインドウのクラス名
#define WINDOW_NAME		"RushFighting"	    // ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
#ifdef _DEBUG
void DrawFPS(void);
void DrawMode(void);
#endif

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3Dオブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// デバイスオブジェクト(描画に必要)
MODE                g_mode = MODE_TITLE;     // 画面(モード)の種類
LPD3DXFONT			g_pFont = NULL;			// フォントへのポインタ
#ifdef _DEBUG
int					g_nCountFPS = 0;		// FPSカウンタ
#endif

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	HWND hWnd;
	MSG msg;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	srand((unsigned int)time(0));

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの作成
	hWnd = CreateWindowEx(0,
							CLASS_NAME,
							WINDOW_NAME,
							WS_OVERLAPPEDWINDOW,
							CW_USEDEFAULT,
							CW_USEDEFAULT,
							(rect.right - rect.left),
							(rect.bottom - rect.top),
							NULL,
							NULL,
							hInstance,
							NULL);

	// 初期化処理(ウィンドウを作成してから行う)
	if(FAILED(Init(hInstance, hWnd, FALSE)))   // FALSEにするとフルスクリーン
	{
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwExecLastTime =
	dwFPSLastTime = timeGetTime();
	dwCurrentTime =
	dwFrameCount = 0;
#ifdef _DEBUG
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// システム時刻を取得(ミリ秒単位)
#endif

	// ウインドウの表示(初期化処理の後に呼ばないと駄目)
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// メッセージループ
	while(1)
	{
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
        }
		else
		{
			dwCurrentTime = timeGetTime();
#ifdef _DEBUG
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
			    // FPSを測定
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		// FPSを測定した時刻を保存
				dwFrameCount = 0;					// カウントをクリア
			}
#endif

			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 1/60秒経過
				dwExecLastTime = dwCurrentTime;

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;
			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=============================================================================
// プロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
    D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの作成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if(g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
    if(FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ワークをゼロクリア
	d3dpp.BackBufferCount			 = 1;							// バックバッファの数
	d3dpp.BackBufferWidth			 = SCREEN_WIDTH;				// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight			 = SCREEN_HEIGHT;				// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat			 = d3ddm.Format;				// バックバッファフォーマットはディスプレイモードに合わせて使う
	d3dpp.SwapEffect				 = D3DSWAPEFFECT_DISCARD;		// 映像信号に同期してフリップする
	d3dpp.EnableAutoDepthStencil	 = TRUE;						// デプスバッファ（Ｚバッファ）とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat	 = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed					 = bWindow;						// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// デバイスオブジェクトの生成
	// [デバイス作成制御]<描画>と<頂点処理>をハードウェアで行なう
	if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp, &g_pD3DDevice)))
	{
		// 上記の設定が失敗したら
		// [デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行なう
		if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&d3dpp, &g_pD3DDevice)))
		{
			// 上記の設定が失敗したら
			// [デバイス作成制御]<描画>と<頂点処理>をCPUで行なう
			if(FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
											D3DDEVTYPE_REF,
											hWnd,
											D3DCREATE_SOFTWARE_VERTEXPROCESSING,
											&d3dpp, &g_pD3DDevice)))
			{
				// 初期化失敗
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
    g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// 裏面をカリング
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);						// Zバッファを使用
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// αブレンドを行う
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// αソースカラーの指定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// αデスティネーションカラーの指定

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(U値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャアドレッシング方法(V値)を設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャ縮小フィルタモードを設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャ拡大フィルタモードを設定

	// テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// アルファブレンディング処理を設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// 最初のアルファ引数
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ２番目のアルファ引数

// デバックの際にモードを切り替える場合はここを変える
#ifdef _DEBUG
	g_mode = MODE_GAME;     // 画面(モード)の種類
#endif

	// 入力初期化処理
	InitInput(hInstance, hWnd);

	// フェード初期化処理
	InitFade(g_mode);

	// サウンド初期化処理
	InitSound(hWnd);

	// ランキングスコア初期化処理
	InitRankingScore();

	// PressEnter初期化処理
	InitPressEnter();

	// 画面設定
	SetMode(g_mode);

	// デバッグ表示用フォントを設定
	// pDevice         : デバイスへのポインタ
	// Height          : 文字の高さ
	// Width           : 文字の幅
	// Weight          : フォントの太さ
	// MipLevels       : ミップマップレベル数
	// Italic          : イタリックフォント
	// CharSet         : フォントの文字セット
	// OutputPrecision : 実際のフォントと目的のフォントのサイズおよび特性の一致方法を指定
	// Quality         : 実際のフォントと目的のフォントとの一致方法を指定
	// PitchAndFamily  : ピッチとファミリインデックス
	// pFacename       : フォントの名前を保持する文字列
	// ppFont          : フォントへのポインタ
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);
	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	// タイトル終了処理
	UninitTitle();

	// ゲーム終了処理
	UninitGame();

	// モード選択終了処理
	UninitModeSelect();

	// チュートリアル終了処理
	UninitTutorial();

	// リザルト終了処理
	UninitResult();

	// ランキング画面終了処理
	UninitRanking();

	// ランキングスコア終了処理
	UninitRankingScore();

	// PressEnter終了処理
	UninitPressEnter();

	// フェード終了処理
	UninitFade();

	// サウンド終了処理
	UninitSound();

	// 入力終了処理
	UninitInput();

	if(g_pD3DDevice != NULL)
	{// デバイスオブジェクトの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if(g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// 入力の更新処理
	UpdateInput();

	switch (g_mode)
	{// 画面(モード)によって切り替え
	case MODE_TITLE:       // タイトル画面
		UpdateTitle();
		UpdatePressEnter();
		break;
	case MODE_SELECT:      // モード選択画面
		UpdateModeSelect();
		break;
	case MODE_TUTORIAL:    // チュートリアル画面
		UpdateTutorial();
		UpdatePressEnter();
		break;
	case MODE_GAME:        // ゲーム画面
		UpdateGame();
		break;
	case MODE_RESULT:      // 結果画面
		UpdateResult();
		break;
	case MODE_RANKING:     // ランキング画面
		UpdateRanking();
		break;
	}

	// フェード更新処理
	UpdateFade();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

	// 描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		switch (g_mode)
		{// 画面(モード)によって切り替え
		case MODE_TITLE:       // タイトル画面
			DrawTitle();
			DrawPressEnter();
			break;
		case MODE_SELECT:      // モード選択画面
			DrawModeSelect();
			break;
		case MODE_TUTORIAL:    // チュートリアル画面
			DrawTutorial();
			DrawPressEnter();
			break;
		case MODE_GAME:        // ゲーム画面
			DrawGame();
			break;
		case MODE_RESULT:      // 結果画面
			DrawResult();
			break;
		case MODE_RANKING:     // ランキング画面
			DrawRanking();
			break;
		}
		// 白画面描画処理
		DrawFade();

#ifdef _DEBUG
		// FPS描画処理
		DrawFPS();

		// 現在のモード描画処理
		DrawMode();
#endif
		// 描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//=============================================================================
// モードの設定
//=============================================================================
void SetMode(MODE mode)
{
	UninitPressEnter();
	switch (g_mode)
	{// 画面(モード)によって切り替え
	case MODE_TITLE:       // タイトル画面
		UninitTitle();
		UninitPressEnter();
		StopSound(SOUND_LABEL_BGMTITLE);
		break;
	case MODE_SELECT:      // モード選択画面
		UninitModeSelect();
		break;
	case MODE_TUTORIAL:    // チュートリアル画面
		UninitTutorial();
		UninitPressEnter();
		StopSound(SOUND_LABEL_BGMTUTORIAL);
		break;
	case MODE_GAME:        // ゲーム画面
		UninitGame();
		StopSound(SOUND_LABEL_BGMGAME);
		break;
	case MODE_RESULT:      // 結果画面
		UninitResult();
		StopSound(SOUND_LABEL_BGMRESULT);
		break;
	case MODE_RANKING:     // ランキング画面
		UninitRanking();
		PlaySound(SOUND_LABEL_BGMTITLE);
		break;
	}

	switch (mode)
	{// 画面(モード)によって切り替え
	case MODE_TITLE:      // タイトル画面
		InitPressEnter();
		InitTitle();
		PlaySound(SOUND_LABEL_BGMTITLE);
		break;
	case MODE_SELECT:     // モード選択画面
		InitModeSelect();
		break;
	case MODE_TUTORIAL:   // チュートリアル画面
		InitPressEnter();
		InitTutorial();
		PlaySound(SOUND_LABEL_BGMTUTORIAL);
		break;
	case MODE_GAME:       // ゲーム画面
		InitGame();
		PlaySound(SOUND_LABEL_BGMGAME);
		break;
	case MODE_RESULT:     // 結果画面
		InitResult();
		PlaySound(SOUND_LABEL_BGMRESULT);
		break;
	case MODE_RANKING:    // ランキング画面
		InitRanking();
		break;
	}

	g_mode = mode;
}
//=============================================================================
// モードの取得
//=============================================================================
MODE GetMode(void)
{
	return g_mode;
}

#ifdef _DEBUG
//=============================================================================
// FPS表示処理
//=============================================================================
void DrawFPS(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	// 文字列を代入
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	// テキスト描画
	// hDC      : デバイスコンテキストのハンドル
	// lpString : 描画する文字列へのポインタ
	// nCount   : 文字列の文字数
	// lpRect   : 長方形領域
	// uFormat  : テキストの整形方法を指定
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
}
//=============================================================================
// モード表示処理
//=============================================================================
void DrawMode(void)
{
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	char aStr[256];

	// 文字列を代入
	switch (g_mode)
	{// 画面(モード)によって切り替え
	case MODE_TITLE:       // タイトル画面
		wsprintf(&aStr[0], "タイトル画面\n");
		break;
	case MODE_SELECT:      // モード選択画面
		wsprintf(&aStr[0], "モード選択画面\n");
		break;
	case MODE_TUTORIAL:    // チュートリアル画面
		wsprintf(&aStr[0], "チュートリアル画面\n");
		break;
	case MODE_GAME:        // ゲーム画面
		wsprintf(&aStr[0], "ゲーム画面\n");
		break;
	case MODE_RESULT:      // 結果画面
		wsprintf(&aStr[0], "結果画面\n");
		break;
	case MODE_RANKING:     // ランキング画面
		wsprintf(&aStr[0], "ランキング画面\n");
		break;
	}

	// テキスト描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(0xff, 0xff, 0xff, 0xff));
}
#endif
