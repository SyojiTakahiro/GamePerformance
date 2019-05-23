//=============================================================================
//
// ライト処理 [light.cpp]
// Author : Hodaka Niwa
//
//=============================================================================
#include "light.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_LIGHT  (3)        // ライトの数

//*****************************************************************************
// グローバル変数
//*****************************************************************************
D3DLIGHT9 g_light[MAX_LIGHT]; // ライトの情報

//=============================================================================
// ライトの初期化処理
//=============================================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	D3DXVECTOR3 vecDir;

	// ライトをクリアする
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ライトの数だけ繰り返し
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));
	}

	// ライトの種類を設定
	g_light[0].Type = D3DLIGHT_DIRECTIONAL;
	g_light[1].Type = D3DLIGHT_DIRECTIONAL;
	g_light[2].Type = D3DLIGHT_DIRECTIONAL;

	// ライトの拡散光を設定
	g_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_light[1].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
	g_light[2].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

	// ライトの方向の設定
	vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_light[0].Direction = vecDir;
	vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_light[1].Direction = vecDir;
	vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
	D3DXVec3Normalize(&vecDir, &vecDir);
	g_light[2].Direction = vecDir;

	// ライトを設定する
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ライトの数だけ繰り返し
		pDevice->SetLight(nCntLight, &g_light[nCntLight]);
	}

	// ライトを有効にする
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{// ライトの数だけ繰り返し
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

