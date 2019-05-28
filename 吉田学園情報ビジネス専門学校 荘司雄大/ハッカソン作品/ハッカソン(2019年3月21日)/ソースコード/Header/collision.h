//*****************************************************************************
//
//     コリジョンの処理[collision.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    コリジョンクラスの定義
//*****************************************************************************
class CCollision
{
public:    // 誰でもアクセス可能
	//--------------------
	//  種類
	//--------------------
	typedef enum
	{
		TYPE_NONE = -1,   // 登録なし
		TYPE_BOX,         // ボックス
		TYPE_SPHERE,      // スフィア
		TYPE_CIRCLE,      // サークル
		TYPE_MAX
	}TYPE;

	CCollision();
	~CCollision();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void SetType(const int nType);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetParent(CCollision *pParent);

	int GetType(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	CCollision *GetParent(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int         m_nType;       // 当たり判定の種類
	D3DXVECTOR3 m_Pos;         // 座標
	D3DXVECTOR3 m_Rot;         // 向き
	D3DXMATRIX  m_MtxWorld;    // ワールドマトリックス
	CCollision  *m_pParent;    // 親のポインタ
};

//*****************************************************************************
//    ボックスコライダークラスの定義
//*****************************************************************************
class CBoxCollider : public CCollision
{
public:    // 誰でもアクセス可能
	CBoxCollider();
	~CBoxCollider();

	static CBoxCollider *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth, bool bReturnFlag, int nType);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand = NULL);

	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetDepth(const float fDepth);
	void SetReturnFlag(const bool bReturnFlag);

	float GetWidth(void);
	float GetHeight(void);
	float GetDepth(void);
	bool GetReturnFlag(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void MakeVertex(void);

	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
	}BOX_VERTEX;

	BOX_VERTEX              m_Vertex[8];    // 箱の頂点情報
	WORD                    m_wIndex[24];   // 箱の描画に使用するインデックス情報
	float                   m_fWidth;       // 箱の横幅
	float                   m_fHeight;      // 箱の高さ
	float                   m_fDepth;       // 箱の奥行
	bool                    m_bReturnFlag;  // 当たった際に戻すかどうか
};

//*****************************************************************************
//    サークルコライダークラスの定義
//*****************************************************************************
class CCircleCollider : public CCollision
{
public:    // 誰でもアクセス可能
	CCircleCollider();
	~CCircleCollider();

	static CCircleCollider *Create(D3DXVECTOR3 pos, float fHeight, float fColRange, bool bReturnFlag, int nType);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand = NULL);

	void SetHeight(const float fHeight);
	void SetColRange(const float fColRange);
	void SetReturnFlag(const bool bReturnFlag);

	float GetHeight(void);
	float GetColRange(void);
	bool GetReturnFlag(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void MakeVertex(void);

	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
	}BOX_VERTEX;

	BOX_VERTEX              m_Vertex[8];    // 円筒の頂点情報
	WORD                    m_wIndex[24];   // 円筒の描画に使用するインデックス情報
	float                   m_fHeight;      // 円筒の高さ
	float                   m_fColRange;    // 円筒の半径
	bool                    m_bReturnFlag;  // 当たった際に戻すかどうか
};

//*****************************************************************************
//    スフィアコライダークラスの定義
//*****************************************************************************
class CSphereCollider : public CCollision
{
public:    // 誰でもアクセス可能
	CSphereCollider();
	~CSphereCollider();

	static CSphereCollider *Create(D3DXVECTOR3 pos, float fColRange, bool bReturnFlag, int nType);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand = NULL);

	void SetColRange(const float fColRange);
	void SetReturnFlag(const bool bReturnFlag);

	float GetColRange(void);
	bool GetReturnFlag(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void MakeVertex(void);

	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
	}BOX_VERTEX;

	BOX_VERTEX              m_Vertex[8];    // 球の頂点情報
	WORD                    m_wIndex[24];   // 球の描画に使用するインデックス情報
	float                   m_fColRange;    // 球の半径
	bool                    m_bReturnFlag;  // 当たった際に戻すかどうか
};

#endif