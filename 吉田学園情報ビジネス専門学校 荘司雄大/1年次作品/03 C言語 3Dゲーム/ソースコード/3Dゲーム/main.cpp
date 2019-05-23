//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 
//
//=============================================================================
#include <iostream>
#include "main.h"
#include "camera.h"
#include "light.h"
#include "input.h"
#include "player.h"
#include "player2.h"
#include "shadow.h"
#include "bullet.h"
#include "meshField.h"
#include "meshWall.h"
#include "particle.h"
#include "t.h"
#include "game.h"
#include "singlegame.h"
#include "fade.h"
#include "sound.h"
#include "title.h"
#include "p1win.h"
#include "p2win.h"
#include "lose.h"
#include "building.h"
#include "rank.h"
#include "ranking.h"
#include "rankingbg.h"
#include "score.h"
#include "diutil.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"				// ウインドウのクラス名
#define WINDOW_NAME		"３Ｄポリゴンの描画"	// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
using namespace std;
void Uninit(void);
void Update(void);
void Draw(void);

//*****************************************************************************
// グローバル変数:
//*****************************************************************************
LPDIRECT3D9			g_pD3D = NULL;			// Direct3Dオブジェクト
LPDIRECT3DDEVICE9	g_pD3DDevice = NULL;	// デバイスオブジェクト(描画に必要)

MODE g_mode = MODE_TITLE;
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
	if(FAILED(Init(hInstance, hWnd, FALSE)))		//FALSE　フルスク
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

	//入力初期化処理
	InitKeyboard(hInstance, hWnd);

	
	/* DirectInputオブジェクトの初期化 */
	//if (InitDInput(hInstance, hWnd) != true) return FALSE;
	

	//// カメラの初期化処理
	//InitCamera();

	//// ライトの初期化処理
	//InitLight();

	//// ポリゴンの初期化処理
	//InitPolygon();

	//InitMeshField();

	//// 壁の初期化処理
	//InitWall();

	//InitMeshWall();

	//// 影の初期化処理
	//InitShadow();

	//// モデルの初期化処理
	//InitPlayer();
	//InitPlayer2();

	//// ポリゴンの初期化処理
	//InitBullet();

	//InitParticle();

	InitSound(hWnd);

	InitFade(g_mode);

	SetMode(g_mode);

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void Uninit(void)
{
	/*UninitTitle();
	UninitGame();
	UninitP1Win();
	UninitP2Win();
	UninitSound();*/

	//キーボードの終了処理
	UninitKeyboard();

	/* DirectInputオブジェクトの開放 */
	//ReleaseDInput();

	//// カメラの終了処理
	//UninitCamera();

	//// ライトの終了処理
	//UninitLight();

	//// 影の終了処理
	//UninitShadow();

	//// ポリゴンの終了処理
	//UninitPolygon();

	//UninitMeshField();

	//// ポリゴンの終了処理
	//UninitBullet();

	//UninitParticle();

	//// 壁の終了処理
	//UninitWall();

	//UninitMeshWall();

	//// モデルの終了処理
	//UninitPlayer();
	//UninitPlayer2();



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

	UninitFade();
}

//=============================================================================
// 更新処理
//=============================================================================
void Update(void)
{
	// キーボードの更新処理
	UpdateKeyboard();

	//// カメラの更新処理
	//UpdateCamera();

	//// ライトの更新処理
	//UpdateLight();

	//// ポリゴンの更新処理
	//UpdatePolygon();

	//UpdateMeshField();

	//// ポリゴンの更新処理
	//UpdateBullet();

 //   UpdateParticle();
	//// 壁の更新処理
	//UpdateWall();

	//UpdateMeshWall();

	//// 影の更新処理
	//UpdateShadow();

	//// モデルの更新処理
	//UpdatePlayer();
	//UpdatePlayer2();

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;
	case MODE_T:
		UpdateT();
		break;
	case MODE_SINGLEGAME:
		UpdateSingleGame();
		break;
	case MODE_GAME:
		UpdateGame();
		break;
	case MODE_P1WIN:
		UpdateP1Win();
		break;
	case MODE_P2WIN:
		UpdateP2Win();
		break;
	case MODE_LOSE:
		UpdateLose();
		break;
	case MODE_RANKING:
		UpdateRanking();
		break;
	}
	UpdateFade();
}

//=============================================================================
// 描画処理
//=============================================================================
void Draw(void)
{
	// バックバッファ＆Ｚバッファのクリア
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(0, 0, 100, 0), 1.0f, 0);

	// 描画の開始
	if(SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//// カメラの設定
		//SetCamera();

		//// ポリゴンの描画処理);

		//DrawMeshField();

		//// 壁の描画処理
		//DrawWall();

		//DrawMeshWall();

		//// 影の描画処理
		//DrawShadow();

		//// モデルの描画処理
		//DrawPlayer();
		//DrawPlayer2();
		//
		//// ポリゴンの描画処理
		//DrawBullet();

		//DrawParticle();
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;
		case MODE_T:
			DrawT();
			break;
		case MODE_SINGLEGAME:
			DrawSingleGame();
			break;
		case MODE_GAME:
			DrawGame();
			break;
		case MODE_P1WIN:
			DrawP1Win();
			break;
		case MODE_P2WIN:
			DrawP2Win();
			break;
		case MODE_LOSE:
			UpdateLose();
			break;
		case MODE_RANKING:
			DrawRanking();
			break;
		}
		DrawFade();
		// 描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void SetMode(MODE mode)
{
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;
	case MODE_T:
		UninitT();
		break;
	case MODE_SINGLEGAME:
		UninitSingleGame();
		break;
	case MODE_GAME:
		UninitGame();
		break;
	case MODE_P1WIN:
		UninitP1Win();
		break;
	case MODE_P2WIN:
		UninitP2Win();
		break;
	case MODE_LOSE:
		UpdateLose();
		break;
	case MODE_RANKING:
		UninitRanking();
		break;
	}
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		break;
	case MODE_T:
		InitT();
		break;
	case MODE_SINGLEGAME:
		InitSingleGame();
		break;
	case MODE_GAME:
		InitGame();
		break;
	case MODE_P1WIN:
		InitP1Win();
		break;
	case MODE_P2WIN:
		InitP2Win();
		break;
	case MODE_LOSE:
		UpdateLose();
		break;
	case MODE_RANKING:
		InitRanking();
		break;
	}
	g_mode = mode;
}

MODE GetMode(void)
{
	return g_mode;
}

//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
