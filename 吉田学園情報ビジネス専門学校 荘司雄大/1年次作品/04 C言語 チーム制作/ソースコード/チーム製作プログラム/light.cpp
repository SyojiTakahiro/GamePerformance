//=============================================================================
//
// ライト処理 [light.cpp]
// Author :  Meguro Mikiya
//
//=============================================================================
#include "light.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	MAX_LIGHT			(3)	// ライトの最大数


//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DLIGHT9	g_aLight[MAX_LIGHT];

//=============================================================================
// ライトの初期化処理
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXVECTOR3 vecDir;

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		// ライトをクリアする
		ZeroMemory(&g_aLight[nCntLight], sizeof(D3DLIGHT9));
	}

	// ライトの種類を設定
	g_aLight[0].Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光を設定
	g_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_aLight[0].Direction = vecDir;

	// ライトの種類を設定
	g_aLight[1].Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光を設定
	g_aLight[1].Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(-0.7f, -0.4f, -0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_aLight[1].Direction = vecDir;

	// ライトの種類を設定
	g_aLight[2].Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光を設定
	g_aLight[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.8f, -0.1f, 0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_aLight[2].Direction = vecDir;

	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		//０のライトに情報を入れる
		// ライトを設定する
		pDevice->SetLight(nCntLight, &g_aLight[nCntLight]);
		// ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}
}

//=============================================================================
// ライトの終了処理
//=============================================================================
void UninitLight(void)
{
}

//=============================================================================
// ライトの更新処理
//=============================================================================
void UpdateLight(void)
{
}

