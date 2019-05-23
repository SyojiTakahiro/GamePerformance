//=============================================================================
//
// フェード処理 [fade.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "renderer.h"
#include "manager.h"

//*************************************
// フェードの状態
//*************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

class CFade
{
public:
	typedef enum
	{
		FADE_NONE = 0,		// 何もしていない状態
		FADE_IN,			// フェードイン状態
		FADE_OUT,			// フェードアウト状態
		FADE_MAX
	} FADE;


	CFade();
	~CFade();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetFade(CManager::MODE modeNext);
	static FADE GetFade(void);
	static CFade *Create(void);
private:
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;	// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// 頂点バッファへのポインタ
	static FADE				m_fade;					// フェード状態
	CManager::MODE			m_modeNext;				// 次のモード
	D3DXCOLOR				m_colorFade;			// フェード色
};
#endif
