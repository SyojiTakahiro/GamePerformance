//*****************************************************************************
//
//     メッシュの処理[3DMesh.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _3DMESH_H_
#define _3DMESH_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    メッシュフィールドクラスの定義
//*****************************************************************************
class CMeshField : public CScene
{
public:    // 誰でもアクセス可能
	CMeshField(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHFIELD);
	~CMeshField();

	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nZBlock, int nTexSplitU, int nTexSplitV, char *pFileName, bool bCulling, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeNormal(void);

	void SetBinaryFileName(char *pFileName);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetXBlock(const int nXBlock);
	void SetZBlock(const int nZBlock);
	void SetTexSplitU(const int nTexSplitU);
	void SetTexSplitV(const int nTexSplitV);
	void SetCulling(const bool bCulling);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	char *GetBinaryFileName(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetXBlock(void);
	int GetZBlock(void);
	int GetTexSplitU(void);
	int GetTexSplitV(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	bool GetCulling(void);
	D3DXMATRIX GetMtxWorld(void);

	float GetPolyHeight(D3DXVECTOR3 pos, bool *pLand, D3DXVECTOR3 *pPolyNormal);
	void SetPolyHeight(const D3DXVECTOR3 pos, const float fValue, const float fRange);
	void SaveMesh(void);
	void LoadMesh(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	char                    m_aBinaryFileName[256];   // 頂点情報が格納されたバイナリファイル名
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;               // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;               // インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	              // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;                    // メッシュフィールドの座標
	D3DXVECTOR3             m_Rot;                    // メッシュフィールドの向き
	D3DXCOLOR               m_Col;                    // メッシュフィールドの色
	float                   m_fWidth;                 // 1ブロック分の幅
	float                   m_fHeight;                // 1ブロック分高さ
	int                     m_nXBlock;                // 横の分割数
	int                     m_nZBlock;                // 奥行の分割数
	int                     m_nTexSplitU;             // テクスチャU座標の分割数
	int                     m_nTexSplitV;             // テクスチャV座標の分割数
	int                     m_nNumVertex;             // 頂点数
	int                     m_nNumIndex;              // インデックス数
	int                     m_nNumPolygon;            // ポリゴン数
	bool                    m_bCulling;               // カリングするかどうか
	D3DXMATRIX              m_MtxWorld;               // ワールドマトリックス
};

//*****************************************************************************
//    メッシュウォールクラスの定義
//*****************************************************************************
class CMeshWall : public CScene
{
public:    // 誰でもアクセス可能
	CMeshWall(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHWALL);
	~CMeshWall();

	static CMeshWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Offset, D3DXVECTOR3 *pInterPoint = NULL);

	void SetTexIdx(const int nTexIdx);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetTexSplitU(const int nTexSplitU);
	void SetTexSplitV(const int nTexSplitV);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	int GetTexIdx(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetTexSplitU(void);
	int GetTexSplitV(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int                     m_nTexIdx;                // テクスチャ番号
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;               // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;               // インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	              // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;                    // メッシュウォールの座標
	D3DXVECTOR3             m_Rot;                    // メッシュウォールの向き
	D3DXCOLOR               m_Col;                    // メッシュウォールの色
	float                   m_fWidth;                 // 1ブロック分の幅
	float                   m_fHeight;                // 1ブロック分高さ
	int                     m_nXBlock;                // 横の分割数
	int                     m_nYBlock;                // 縦の分割数
	int                     m_nTexSplitU;             // テクスチャU座標の分割数
	int                     m_nTexSplitV;             // テクスチャV座標の分割数
	int                     m_nNumVertex;             // 頂点数
	int                     m_nNumIndex;              // インデックス数
	int                     m_nNumPolygon;            // ポリゴン数
	D3DXMATRIX              m_MtxWorld;               // ワールドマトリックス
	D3DXVECTOR3             m_WallNormal;             // 壁の法線
};

//*****************************************************************************
//    メッシュシリンダークラスの定義
//*****************************************************************************
class CMeshCylinder : public CScene
{
public:    // 誰でもアクセス可能
	CMeshCylinder(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHCYLINDER);
	~CMeshCylinder();
	static CMeshCylinder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeNormal(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetHeight(const float fHeight);
	void SetRadius(const float fRadius);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetHeight(void);
	float GetRadius(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;     // インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	    // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;          // メッシュシリンダーの座標
	D3DXVECTOR3             m_Rot;          // メッシュシリンダーの向き
	D3DXCOLOR               m_Col;          // メッシュシリンダーの色
	float                   m_fHeight;      // メッシュシリンダーの高さ
	float                   m_fRadius;      // メッシュシリンダーの半径
	int                     m_nXBlock;      // 横の分割数
	int                     m_nYBlock;      // 縦の分割数
	int                     m_nNumVertex;   // 頂点数
	int                     m_nNumIndex;    // インデックス数
	int                     m_nNumPolygon;  // ポリゴン数
	D3DXMATRIX              m_MtxWorld;     // ワールドマトリックス
};

//*****************************************************************************
//    メッシュドームクラスの定義
//*****************************************************************************
class CMeshDome : public CScene
{
public:    // 誰でもアクセス可能
	CMeshDome(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHDOME);
	~CMeshDome();
	static CMeshDome *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeNormal(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetRadius(const float fRadius);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetRadius(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;     // インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	    // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;          // メッシュドームの座標
	D3DXVECTOR3             m_Rot;          // メッシュドームの向き
	D3DXCOLOR               m_Col;          // メッシュドームの色
	float                   m_fRadius;      // メッシュドームの半径
	int                     m_nXBlock;      // 横の分割数
	int                     m_nYBlock;      // 縦の分割数
	int                     m_nNumVertex;   // 頂点数
	int                     m_nNumIndex;    // インデックス数
	int                     m_nNumPolygon;  // ポリゴン数
	D3DXMATRIX              m_MtxWorld;     // ワールドマトリックス
};

//*****************************************************************************
//    メッシュスフィアクラスの定義
//*****************************************************************************
class CMeshSphere : public CScene
{
public:    // 誰でもアクセス可能
	CMeshSphere(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHSPHERE);
	~CMeshSphere();
	static CMeshSphere *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeNormal(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetRadius(const float fRadius);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetRadius(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;          // 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffCover[2];  // 頂点バッファへのポインタ(蓋用)
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;          // インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	         // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;               // メッシュスフィアの座標
	D3DXVECTOR3             m_Rot;               // メッシュスフィアの向き
	D3DXCOLOR               m_Col;               // メッシュスフィアの色
	float                   m_fRadius;           // メッシュスフィアの半径
	int                     m_nXBlock;           // 横の分割数
	int                     m_nYBlock;           // 縦の分割数
	int                     m_nNumVertex;        // 頂点数
	int                     m_nNumIndex;         // インデックス数
	int                     m_nNumPolygon;       // ポリゴン数
	D3DXMATRIX              m_MtxWorld;          // ワールドマトリックス
};

//*****************************************************************************
//    メッシュオービットクラスの定義
//*****************************************************************************
class CMeshOrbit : public CScene
{
public:    // 誰でもアクセス可能
	CMeshOrbit(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHORBIT);
	~CMeshOrbit();

	static CMeshOrbit *Create(D3DXVECTOR3 OffsetPos1, D3DXVECTOR3 OffsetPos2, int nXBlock, int nZBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetOffsetPos(const D3DXVECTOR3 OffsetPos, int nIdx);
	void SetXBlock(const int nXBlock);
	void SetZBlock(const int nZBlock);
	void SetVertexPos(D3DXVECTOR3 pos, int nIdx);
	void SetVertexCol(D3DXCOLOR col, int nIdx);
	void SetVertexPos(D3DXVECTOR3 *pPos);
	void SetVertexCol(D3DXCOLOR *pCol);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetMtxParent(D3DXMATRIX *pMtxParent);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetOffsetPos(int nIdx);
	int GetXBlock(void);
	int GetZBlock(void);
	int GetNumVertex(void);
	int GetNumPolygon(void);
	D3DXVECTOR3 *GetVertexPos(void);
	D3DXCOLOR *GetVertexCol(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXMATRIX *GetMtxParent(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;      // 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	     // テクスチャへのポインタ
	int                     m_nXBlock;       // 横の分割数
	int                     m_nZBlock;       // 奥行の分割数
	int                     m_nNumVertex;    // 頂点数
	int                     m_nNumPolygon;   // ポリゴン数
	D3DXVECTOR3             *m_pVertexPos;   // 頂点の座標(頂点数分動的に確保)
	D3DXVECTOR3             m_aOffsetPos[2]; // 頂点のオフセット
	D3DXCOLOR               *m_pVertexCol;   // 頂点の色(頂点数分動的に確保)
	D3DXMATRIX              m_MtxWorld;      // ワールドマトリックス
	D3DXMATRIX              *m_pMtxParent;   // 親のマトリックスへのポインタ
};

//*****************************************************************************
//    メッシュリングクラスの定義
//*****************************************************************************
class CMeshRing : public CScene
{
public:    // 誰でもアクセス可能
	CMeshRing(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHRING);
	~CMeshRing();
	static CMeshRing *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeNormal(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetHeight(const float fHeight);
	void SetRadius(const float fRadius);
	void SetDiffusion(const float fDiffusion);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetHeight(void);
	float GetRadius(void);
	float GetDiffusion(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;     // インデックスバッファへのポインタ
	LPDIRECT3DTEXTURE9		m_pTexture;	    // テクスチャへのポインタ
	D3DXVECTOR3             m_Pos;          // メッシュリングの座標
	D3DXVECTOR3             m_Rot;          // メッシュリングの向き
	D3DXCOLOR               m_Col;          // メッシュリングの色
	float                   m_fHeight;      // メッシュリングの高さ
	float                   m_fRadius;      // メッシュリングの半径
	float                   m_fDiffusion;   // メッシュリングの拡散値
	int                     m_nXBlock;      // 横の分割数
	int                     m_nYBlock;      // 縦の分割数
	int                     m_nNumVertex;   // 頂点数
	int                     m_nNumIndex;    // インデックス数
	int                     m_nNumPolygon;  // ポリゴン数
	D3DXMATRIX              m_MtxWorld;     // ワールドマトリックス
};

#endif