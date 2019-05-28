//*****************************************************************************
//
//     オブジェクトの処理[object.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "3DPolygon.h"
#include "3DMesh.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CModel;
class CMotionAttack;
class CMotionCollision;
class CMotionOrbit;
class CMotion;
class CMotionManager;
class CModelManager;
class CCollision;

//*****************************************************************************
//    配置ビルボードクラスの定義
//*****************************************************************************
class CBillboardObject : public CSceneBillboard
{
public:    // 誰でもアクセス可能
	//----------------------
	// 種類
	//----------------------
	typedef enum
	{
		TYPE_NONE = 0,   // 何もなし
		TYPE_BAZOOKA,    // バズーカ
		TYPE_HORMING,    // ホーミング
		TYPE_WIRE,       // ワイヤー
		TYPE_MAX
	}TYPE;

	CBillboardObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_BILLOBJ);
	~CBillboardObject();

	static CBillboardObject *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, int nType = 0, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, bool bLighting = false, bool bDrawAddtive = false, int nTexIdx = 0, int nType = 0);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void SetType(const int nType);
	void SetTexIdx(const int nTexIdx);
	void SetDrawAddtive(const bool bDrawAddtive);

	int GetType(void);
	int GetTexIdx(void);
	bool GetDrawAddtive(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int   m_nType;         // 種類番号
	int   m_nTexIdx;       // 使用しているテクスチャの番号
	bool  m_bDrawAddtive;  // 加算合成で描画するかしないか
};

//*****************************************************************************
//    山クラスの定義
//*****************************************************************************
class CMountain : public CMeshCylinder
{
public:    // 誰でもアクセス可能
	CMountain(int nPriority = 3, OBJTYPE objType = OBJTYPE_MOUNTAIN);
	~CMountain();
	static CMountain *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeNormal(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};

//*****************************************************************************
//    配置物クラスの定義
//*****************************************************************************
class CObject : public CScene
{
public:    // 誰でもアクセス可能
	CObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECT);
	~CObject();

	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, int nNumParts, int nNumCollision, CModel **pModel, CMotionManager *pMotionManager, CCollision **pCollision, bool bBreak = false, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand, CScene *pRideObject);
	void Damage(int nDamage);

	void SetType(const int nType);
	void SetNumParts(const int nNumParts);
	void SetNumCollision(const int nNumCollision);
	void SetModel(CModel **pModel);
	void SetMotionManager(CMotionManager *pMotionManager);
	void SetCollision(CCollision **pCollision);
	void SetPos(const D3DXVECTOR3 pos);
	void SetPosOld(const D3DXVECTOR3 posold);
	void SetPosStd(const D3DXVECTOR3 posstd);
	void SetRot(const D3DXVECTOR3 rot);
	void SetRotStd(const D3DXVECTOR3 rotstd);
	void SetScale(const D3DXVECTOR3 scale);
	void SetRollSpeed(const D3DXVECTOR3 RollSpeed);
	void SetBreak(const bool bBreak);

	int GetType(void);
	int GetNumParts(void);
	int GetNumCollision(void);
	CModel **GetModel(void);
	CMotionManager *GetMotionManager(void);
	CCollision **GetCollision(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetPosStd(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetRotStd(void);
	D3DXVECTOR3 GetScale(void);
	D3DXVECTOR3 GetRollSpeed(void);
	bool GetBreak(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int             m_nType;                 // 配置物の種類
	int             m_nNumParts;             // 使用するパーツ数
	int             m_nNumCollision;         // 当たり判定を取る数
	CMotionManager  *m_pMotionManager;       // モーションクラスへのポインタ
	CModel          **m_apModel;             // モデルクラスへのポインタ
	CCollision      **m_apCollision;         // 当たり判定クラスへのポインタ
	D3DXMATRIX      m_MtxWorld;              // ワールドマトリックス
	D3DXVECTOR3     m_Pos;                   // 現在の座標
	D3DXVECTOR3     m_PosOld;                // 前回の座標
	D3DXVECTOR3     m_PosStd;                // 生成時の座標
	D3DXVECTOR3     m_Rot;                   // 現在の向き
	D3DXVECTOR3     m_RotStd;                // 生成時の向き
	D3DXVECTOR3     m_Scale;                 // 大きさ
	D3DXVECTOR3     m_RollSpeed;             // 向きを回転させるスピード
	bool            m_bBreak;                // 壊せるオブジェクトかどうか
	int             m_nLife;                 // 体力
};


//*****************************************************************************
//    スター配置物クラスの定義
//*****************************************************************************
class CObjectStar : public CObject
{
public:    // 誰でもアクセス可能
	CObjectStar(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTSTAR);
	~CObjectStar();

	static CObjectStar *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, int nNumParts, int nNumCollision, CModel **pModel, CMotionManager *pMotionManager, CCollision **pCollision, bool bBreak = false, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Death(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能

};

//*****************************************************************************
//    配置物読み取り用クラスの定義
//*****************************************************************************
class CObjectManager
{
public:    // 誰でもアクセス可能
	CObjectManager();
	~CObjectManager();

	static CObjectManager *Create(char *pFileName, CModelManager *pModelManager);

	HRESULT Init(void);
	void Uninit(void);
	void CreateObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, bool bBreak = false, int nPriority = 3);
	void CreateStarObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale, D3DXVECTOR3 RollSpeed, int nType, bool bBreak = false, int nPriority = 3);

	void SetFileName(const char *pFileName);
	void SetNumParts(const int nNumParts);
	void SetNumCollision(const int nNumCollision);
	void SetModel(CModel **pModel);
	void SetModelManager(CModelManager *pModelManager);
	void SetMotionManager(CMotionManager *pMotionManager);
	void SetCollision(CCollision **pCollision);

	char *GetFileName(void);
	int GetNumParts(void);
	int GetNumCollision(void);
	CModel **GetModel(void);
	CModelManager *GetModelManager(void);
	CMotionManager *GetMotionManager(void);
	CCollision **GetCollision(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CModel **ReadModelSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts);
	CModel *ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel);
	CMotionAttack *ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile);
	CMotionCollision **ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData);
	CMotionOrbit **ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData);
	CMotion *ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	void ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	CCollision **ReadCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumCollision);
	CCollision *ReadCollision(char *pLine, char *pStrCur, FILE *pFile, CCollision **apCollision);

	char           m_aFileName[256];     // ファイル名
	int            m_nNumParts;          // パーツ数
	int            m_nNumCollision;      // 当たり判定を取る数
	int            *m_pNumParent;        // 親モデルの番号
	int            *m_pNumColParent;     // 当たり判定の親番号
	int            *m_pNumModelIdx;      // 使用するモデルの番号
	CModel         **m_apModel;          // モデルクラスへのポインタ
	CModelManager  *m_pModelManager;     // モデル管轄クラスへのポインタ
	CMotionManager *m_pMotionManager;    // モーションクラスへのポインタ
	CCollision     **m_apCollision;      // コリジョンクラスへのポインタ
};

//*****************************************************************************
//    空クラスの定義
//*****************************************************************************
class CSky : public CMeshDome
{
public:    // 誰でもアクセス可能
	CSky(int nPriority = 3, OBJTYPE objType = OBJTYPE_SKY);
	~CSky();
	static CSky *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, float fRotSpeed, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, float fRotSpeed);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeNormal(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	float m_fRotSpeed;     // 回転するスピード
};

#endif