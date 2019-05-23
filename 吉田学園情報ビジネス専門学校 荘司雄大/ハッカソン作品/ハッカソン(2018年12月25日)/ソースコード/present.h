//*****************************************************************************
//
//     プレゼントの処理[present.h]
//     Auther:Takuto Ishida
//
//*****************************************************************************
#ifndef _PRESENT_H_
#define _PRESENT_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     前方宣言
//*****************************************************************************
class CNumber;
class CTextureManager;
class CScene2D;

//*****************************************************************************
//     タイマークラスの定義
//*****************************************************************************
class CPresent : public CScene
{
public:    // 誰でもアクセス可能
	CPresent(int nPriority = 5, OBJTYPE objType = OBJTYPE_PRESENT);
	~CPresent();

	static CPresent *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nStock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPresentTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetPresent(const int nStock);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetPresent(void);

private:   // このクラスだけがアクセス可能
	LPDIRECT3DTEXTURE9 m_pTexture;         // 数字テクスチャへのポインタ
	D3DXVECTOR3        m_Pos;              // プレゼントの座標
	D3DXCOLOR          m_Col;              // プレゼントの色
	float              m_fWidth;           // プレゼントの数字ポリゴン１つ分の幅
	float              m_fHeight;          // プレゼントの数字ポリゴン１つ分の高さ
	CNumber            **m_apNumber;       // 数字ポリゴンクラス型のポインタ
	int                m_nDigit;           // プレゼントの桁数
	int                m_nPresent;         // プレゼント
	CScene2D		   *m_pFrame;		   // 枠
	CTextureManager	   *m_pTextureManager; // テクスチャ管理クラスのポインタ
};

#endif