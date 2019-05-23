//*****************************************************************************
//
//     プレゼント用ビルボードの処理[Billpresent.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _BILLPRESENT_H_
#define _BILLPRESENT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    プレゼント用ビルボードクラスの定義
//*****************************************************************************
class CBillPresent : public CSceneBillboard
{
public:    // 誰でもアクセス可能
	CBillPresent(int nPriority = 3, OBJTYPE objType = OBJTYPE_BILLPRESENT);
	~CBillPresent();

	static CBillPresent *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Collision(D3DXVECTOR3 pos);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};

#endif