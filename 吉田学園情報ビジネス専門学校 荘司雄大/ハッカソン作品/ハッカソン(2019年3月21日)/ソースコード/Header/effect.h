//*****************************************************************************
//
//     エフェクトの処理[effect.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"
#include "2DPolygon.h"
#include "3DPolygon.h"
#include "3DMesh.h"

//*****************************************************************************
//     マクロ定義
//*****************************************************************************
#define EFFECT_PRIORITY     (9)       // エフェクトの処理優先順位
#define MAX_EMMITER         (100)     // エミッタデータの最大数
#define MAX_PARTICLEDATA    (100)     // パーティクルデータの最大数
#define MAX_RINGEFFECTDATA  (100)     // リングエフェクトデータの最大数

//*****************************************************************************
//     前方宣言
//*****************************************************************************
class CTextureManager;

//*****************************************************************************
//     エミッタデータクラスの定義
//*****************************************************************************
class CEmitterData
{
public:    // 誰でもアクセス可能
	CEmitterData();
	~CEmitterData();

	void SetType(const int nType) { m_nType = nType; }
	void SetTexIdx(const int nTexIdx) { m_nTexIdx = nTexIdx; }
	void SetEffectIdx(const int nEffectIdx) { m_nEffectIdx = nEffectIdx; }
	void SetRangeMax(const D3DXVECTOR3 RangeMax) { m_RangeMax = RangeMax; }
	void SetRangeMin(const D3DXVECTOR3 RangeMin) { m_RangeMin = RangeMin; }
	void SetLife(const int nLife) { m_nLife = nLife; }
	void SetLap(const int nLap) { m_nLap = nLap; }
	void SetAppear(const int nAppear) { m_nAppear = nAppear; }
	void SetLoop(const bool bLoop) { m_bLoop = bLoop; }

	int GetType(void) { return m_nType; }
	int GetTexIdx(void) { return m_nTexIdx; }
	int GetEffectIdx(void) { return m_nEffectIdx; }
	D3DXVECTOR3 GetRangeMax(void) { return m_RangeMax; }
	D3DXVECTOR3 GetRangeMin(void) { return m_RangeMin; }
	int GetLife(void) { return m_nLife; }
	int GetLap(void) { return m_nLap; }
	int GetAppear(void) { return m_nAppear; }
	bool GetLoop(void) { return m_bLoop; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int         m_nType;            // 使用するエフェクトの種類
	int         m_nTexIdx;          // 使用するテクスチャの番号
	int         m_nEffectIdx;       // 使用するエフェクトデータの番号
	D3DXVECTOR3 m_RangeMax;         // エフェクトを生成する範囲の最大値
	D3DXVECTOR3 m_RangeMin;         // エフェクトを生成する範囲の最小値
	int         m_nLife;            // 寿命
	int         m_nLap;             // 1回の放出で出すエフェクトの個数
	int         m_nAppear;          // エフェクトを出す間隔
	bool        m_bLoop;            // ループするかしないか
};

//*****************************************************************************
//    パーティクルデータクラスの定義
//*****************************************************************************
class CParData
{
public:    // 誰でもアクセス可能
	CParData();
	~CParData();

	void SetRange(const D3DXVECTOR3 Range) { m_Range = Range; }
	void SetRangeMag(const float fRangeMag) { m_fRangeMag = fRangeMag; }
	void SetMaxRot(const float fMaxRot) { m_fMaxRot = fMaxRot; }
	void SetMinRot(const float fMinRot) { m_fMinRot = fMinRot; }
	void SetMaxMove(const D3DXVECTOR3 MaxMove) { m_MaxMove = MaxMove; }
	void SetMinMove(const D3DXVECTOR3 MinMove) { m_MinMove = MinMove; }
	void SetChangeMove(const D3DXVECTOR3 ChangeMove) { m_ChangeMove = ChangeMove; }
	void SetInitCol(const D3DXCOLOR InitCol) { m_InitCol = InitCol; }
	void SetChangeCol(const D3DXCOLOR ChangeCol) { m_ChangeCol = ChangeCol; }
	void SetMaxWidth(const float fMaxWidth) { m_fMaxWidth = fMaxWidth; }
	void SetMinWidth(const float fMinWidth) { m_fMinWidth = fMinWidth; }
	void SetMaxHeight(const float fMaxHeight) { m_fMaxHeight = fMaxHeight; }
	void SetMinHeight(const float fMinHeight) { m_fMinHeight = fMinHeight; }
	void SetSpread(const float fSpread) { m_fSpread = fSpread; }
	void SetMaxLife(const int nMaxLife) { m_nMaxLife = nMaxLife; }
	void SetMinLife(const int nMinLife) { m_nMinLife = nMinLife; }
	void SetGravity(const float fGravity) { m_fGravity = fGravity; }
	void SetBouncing(const float fBouncing) { m_fBouncing = fBouncing; }
	void SetRotPattern(const int nRotPattern) { m_nRotPattern = nRotPattern; }
	void SetRotSpeed(const float fRotSpeed) { m_fRotSpeed = fRotSpeed; }
	void SetCollision(const bool bCollision) { m_bCollision = bCollision; }
	void SetDrawAddtive(const bool bDrawAddtive) { m_bDrawAddtive = bDrawAddtive; }
	void SetLighting(const bool bLighting) { m_bLighting = bLighting; }

	D3DXVECTOR3 GetRange(void) { return m_Range; }
	float GetRangeMag(void) { return m_fRangeMag; }
	float GetMaxRot(void) { return m_fMaxRot; }
	float GetMinRot(void) { return m_fMinRot; }
	D3DXVECTOR3 GetMaxMove(void) { return m_MaxMove; }
	D3DXVECTOR3 GetMinMove(void) { return m_MinMove; }
	D3DXVECTOR3 GetChangeMove(void) { return m_ChangeMove; }
	D3DXCOLOR GetInitCol(void) { return m_InitCol; }
	D3DXCOLOR GetChangeCol(void) { return m_ChangeCol; }
	float GetMaxWidth(void) { return m_fMaxWidth; }
	float GetMinWidth(void) { return m_fMinWidth; }
	float GetMaxHeight(void) { return m_fMaxHeight; }
	float GetMinHeight(void) { return m_fMinHeight; }
	float GetSpread(void) { return m_fSpread; }
	int GetMaxLife(void) { return m_nMaxLife; }
	int GetMinLife(void) { return m_nMinLife; }
	float GetGravity(void) { return m_fGravity; }
	float GetBouncing(void) { return m_fBouncing; }
	int GetRotPattern(void) { return m_nRotPattern; }
	float GetRotSpeed(void) { return m_fRotSpeed; }
	bool GetCollision(void) { return m_bCollision; }
	bool GetDrawAddtive(void) { return m_bDrawAddtive; }
	bool GetLighting(void) { return m_bLighting; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3 m_Range;         // 生成する範囲(オフセット)
	float       m_fRangeMag;     // 生成する範囲の倍率
	float       m_fMaxRot;       // 生成時の向き(最大値)
	float       m_fMinRot;       // 生成時の向き(最小値)
	D3DXVECTOR3 m_MaxMove;       // 移動量の最大値
	D3DXVECTOR3 m_MinMove;       // 移動量の最小値
	D3DXVECTOR3 m_ChangeMove;    // 移動量の変化量
	D3DXCOLOR   m_InitCol;       // 生成時の色
	D3DXCOLOR   m_ChangeCol;     // フレーム毎の色の変化量
	float       m_fMaxWidth;     // 幅の最大値
	float       m_fMinWidth;     // 幅の最小値
	float       m_fMaxHeight;    // 高さの最大値
	float       m_fMinHeight;    // 高さの最小値
	float       m_fSpread;       // 大きさの変化量
	int         m_nMaxLife;      // 寿命の最大値
	int         m_nMinLife;      // 寿命の最小値
	float       m_fGravity;      // 毎フレームかける重力
	float       m_fBouncing;     // バウンド量
	int         m_nRotPattern;   // 回転の種類[ 0:時計回り 1:反時計回り 2:ランダム ]
	float       m_fRotSpeed;     // 回転するスピード
	bool        m_bCollision;    // 当たり判定をするかどうか
	bool        m_bDrawAddtive;  // 加算合成するかしないか[ 0:なし 1:あり ]
	bool        m_bLighting;     // ライティングするかしないか[ 0:なし 1:あり ]
};

//*****************************************************************************
//    リングエフェクトデータクラスの定義
//*****************************************************************************
class CRingData
{
public:    // 誰でもアクセス可能
	CRingData();
	~CRingData();

	void SetRot(const D3DXVECTOR3 Rot) { m_Rot = Rot; }
	void SetMaxMove(const D3DXVECTOR3 MaxMove) { m_MaxMove = MaxMove; }
	void SetMinMove(const D3DXVECTOR3 MinMove) { m_MinMove = MinMove; }
	void SetChangeMove(const D3DXVECTOR3 ChangeMove) { m_ChangeMove = ChangeMove; }
	void SetInitCol(const D3DXCOLOR InitCol) { m_InitCol = InitCol; }
	void SetChangeCol(const D3DXCOLOR ChangeCol) { m_ChangeCol = ChangeCol; }
	void SetHeight(const float fHeight) { m_fHeight = fHeight; }
	void SetRadius(const float fRadius) { m_fRadius = fRadius; }
	void SetDiffusion(const float fDiffusion) { m_fDiffusion = fDiffusion; }
	void SetChangeHeight(const float fChangeHeight) { m_fChangeHeight = fChangeHeight; }
	void SetChangeRadius(const float fChangeRadius) { m_fChangeRadius = fChangeRadius; }
	void SetChangeDiffusion(const float fChangeDiffusion) { m_fChangeDiffusion = fChangeDiffusion; }
	void SetMaxLife(const int nMaxLife) { m_nMaxLife = nMaxLife; }
	void SetMinLife(const int nMinLife) { m_nMinLife = nMinLife; }
	void SetXBlock(const int nXBlock) { m_nXBlock = nXBlock; }
	void SetYBlock(const int nYBlock) { m_nYBlock = nYBlock; }
	void SetRotPattern(const int nRotPattern) { m_nRotPattern = nRotPattern; }
	void SetRotSpeed(const D3DXVECTOR3 RotSpeed) { m_RotSpeed = RotSpeed; }
	void SetCulling(const bool bCulling) { m_bCulling = bCulling; }
	void SetDrawAddtive(const bool bDrawAddtive) { m_bDrawAddtive = bDrawAddtive; }

	D3DXVECTOR3 GetRot(void) { return m_Rot; }
	D3DXVECTOR3 GetMaxMove(void) { return m_MaxMove; }
	D3DXVECTOR3 GetMinMove(void) { return m_MinMove; }
	D3DXVECTOR3 GetChangeMove(void) { return m_ChangeMove; }
	D3DXCOLOR GetInitCol(void) { return m_InitCol; }
	D3DXCOLOR GetChangeCol(void) { return m_ChangeCol; }
	float GetHeight(void) { return m_fHeight; }
	float GetRadius(void) { return m_fRadius; }
	float GetDiffusion(void) { return m_fDiffusion; }
	float GetChangeHeight(void) { return m_fChangeHeight; }
	float GetChangeRadius(void) { return m_fChangeRadius; }
	float GetChangeDiffusion(void) { return m_fChangeDiffusion; }
	int GetMaxLife(void) { return m_nMaxLife; }
	int GetMinLife(void) { return m_nMinLife; }
	int GetXBlock(void) { return m_nXBlock; }
	int GetYBlock(void) { return m_nYBlock; }
	int GetRotPattern(void) { return m_nRotPattern; }
	D3DXVECTOR3 GetRotSpeed(void) { return m_RotSpeed; }
	bool GetCulling(void) { return m_bCulling; }
	bool GetDrawAddtive(void) { return m_bDrawAddtive; }

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3 m_Rot;               // 向き
	D3DXVECTOR3 m_MaxMove;           // 移動量の最大値
	D3DXVECTOR3 m_MinMove;           // 移動量の最小値
	D3DXVECTOR3 m_ChangeMove;        // 移動量の変化量
	D3DXCOLOR   m_InitCol;           // 生成時の色
	D3DXCOLOR   m_ChangeCol;         // フレーム毎の色の変化量
	float       m_fHeight;           // 高さ
	float       m_fRadius;           // 半径
	float       m_fDiffusion;        // 幅
	float       m_fChangeHeight;     // 高さを毎フレームどれくらい変化させるか
	float       m_fChangeRadius;     // 半径をどれくらい毎フレーム変化させるか
	float       m_fChangeDiffusion;  // リングの幅をどれくらい毎フレーム変化させるか
	int         m_nXBlock;           // 横の分割数
	int         m_nYBlock;           // 縦の分割数
	int         m_nMaxLife;          // 寿命の最大値
	int         m_nMinLife;          // 寿命の最小値
	int         m_nRotPattern;       // 回転の種類[ 0:時計回り 1:反時計回り 2:ランダム ]
	D3DXVECTOR3 m_RotSpeed;          // 回転するスピード
	bool        m_bCulling;          // カリングするかどうか[ 0:なし 1:あり ]
	bool        m_bDrawAddtive;      // 加算合成するかしないか[ 0:なし 1:あり ]
};

//*****************************************************************************
//     エミッタクラスの定義
//*****************************************************************************
class CEmitter : public CScene
{
public:    // 誰でもアクセス可能
	CEmitter(int nPriority = 1, OBJTYPE objType = OBJTYPE_EMITTER);
	~CEmitter();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetType(const int nType);
	void SetTexIdx(const int nTexIdx);
	void SetEffectIdx(const int nEffectIdx);
	void SetRangeMax(const D3DXVECTOR3 RangeMax);
	void SetRangeMin(const D3DXVECTOR3 RangeMin);
	void SetLife(const int nLife);
	void SetLap(const int nLap);
	void SetAppear(const int nAppear);
	void SetLoop(const bool bLoop);

	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	int GetType(void);
	int GetTexIdx(void);
	int GetEffectIdx(void);
	D3DXVECTOR3 GetRangeMax(void);
	D3DXVECTOR3 GetRangeMin(void);
	int GetLife(void);
	int GetLap(void);
	int GetAppear(void);
	bool GetLoop(void);

protected: // このクラスと派生クラスだけがアクセス可能
	virtual void SetEffect(void) = 0;

private:   // このクラスだけがアクセス可能
	void Movement(void);

	LPDIRECT3DTEXTURE9 m_pTexture;   // テクスチャへのポインタ
	D3DXVECTOR3        m_Pos;        // 座標
	D3DXVECTOR3        m_Rot;        // 向き
	D3DXMATRIX         m_MtxWorld;   // ワールドマトリックス
	int                m_nType;      // 使用するエフェクトの種類
	int                m_nTexIdx;    // 使用するテクスチャ番号
	int                m_nEffectIdx; // 使用するエフェクトの番号
	D3DXVECTOR3        m_RangeMax;   // エフェクトを生成する範囲の最大値
	D3DXVECTOR3        m_RangeMin;   // エフェクトを生成する範囲の最小値
	int                m_nLife;      // エミッタの寿命
	int                m_nLap;       // 1回の放出タイミングに出すエフェクトの個数
	int                m_nAppear;    // 放出する間隔
	int                m_nCounter;   // 放出タイミングを管理するカウンター
	bool               m_bLoop;      // ループするかしないか
};

//*****************************************************************************
//     パーティクルエミッタクラスの定義
//*****************************************************************************
class CParEmitter : public CEmitter
{
public:    // 誰でもアクセス可能
	CParEmitter(int nPriority = 1, OBJTYPE objType = OBJTYPE_PAREMITTER);
	~CParEmitter();

	static CParEmitter *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexIdx, int nEffectIdx, D3DXVECTOR3 RangeMax, D3DXVECTOR3 RangeMin, int nLife, int nLap, int nAppear, bool bLoop, CParData *pParData, LPDIRECT3DTEXTURE9 pTexture, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetParData(CParData *pParData);
	CParData *GetParData(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void SetEffect(void);

private:   // このクラスだけがアクセス可能
	CParData  *m_pParData;  // パーティクルデータへのポインタ
};

//*****************************************************************************
//     リングエフェクトエミッタクラスの定義
//*****************************************************************************
class CRingEmitter : public CEmitter
{
public:    // 誰でもアクセス可能
	CRingEmitter(int nPriority = 3, OBJTYPE objType = OBJTYPE_RINGEMITTER);
	~CRingEmitter();

	static CRingEmitter *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexIdx, int nEffectIdx, D3DXVECTOR3 RangeMax, D3DXVECTOR3 RangeMin, int nLife, int nLap, int nAppear, bool bLoop, CRingData *pRingData, LPDIRECT3DTEXTURE9 pTexture, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetRingData(CRingData *pRingData);
	CRingData *GetRingData(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void SetEffect(void);

private:   // このクラスだけがアクセス可能
	CRingData  *m_pRingData;  // リングエフェクトデータへのポインタ
};

//*****************************************************************************
//    パーティクルクラスの定義
//*****************************************************************************
class CParticle : public CSceneBillboard
{
public:    // 誰でもアクセス可能
	CParticle(int nPriority = 3, OBJTYPE objType = OBJTYPE_PARTICLE);
	~CParticle();

	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col, float fWidth,
		float fHeight, float fRot, D3DXCOLOR ChangeCol, float fGravity, float fSpread,
		float fChangeRot, float fBouncing, int nLife, int nRotPattern,
		bool bCol, bool bDrawAddtive, bool bLighting, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col,
		float fWidth, float fHeight, float fRot, D3DXCOLOR ChangeCol, float fGravity, float fSpread,
		float fChangeRot, float fBouncing, int nLife, int nRotPattern,
		bool bCol, bool bDrawAddtive, bool bLighting);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void ChangeVertex(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fLength);

	void SetMove(const D3DXVECTOR3 move);
	void SetChangeMove(const D3DXVECTOR3 ChangeMove);
	void SetChangeCol(const D3DXCOLOR ChangeCol);
	void SetLife(const int nLife);
	void SetRotPattern(const int nRotPattern);
	void SetGravity(const float fGravity);
	void SetSpread(const float fSpread);
	void SetChangeRot(const float fChangeRot);
	void SetBouncing(const float fBouncing);
	void SetCollision(const bool bCollision);
	void SetDrawAddtive(const bool bDrawAddtive);

	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetChangeMove(void);
	D3DXCOLOR GetChangeCol(void);
	int GetLife(void);
	int GetRotPattern(void);
	float GetGravity(void);
	float GetSpread(void);
	float GetChangeRot(void);
	float GetBouncing(void);
	bool GetCollision(void);
	bool GetDrawAddtive(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3 m_Move;           // 移動量
	D3DXVECTOR3 m_ChangeMove;     // 移動量の変化量
	D3DXCOLOR   m_ChangeCol;      // 色の変化量
	int         m_nLife;          // 寿命
	int         m_nRotPattern;    // 回転の種類
	float       m_fGravity;       // 重力
	float       m_fSpread;        // 大きさの変化量
	float       m_fChangeRot;     // 回転の変化量
	float       m_fBouncing;      // バウンド量
	bool        m_bCollision;     // 当たり判定を取るかどうか
	bool        m_bDrawAddtive;   // 加算合成で描画するかしないか
};

//*****************************************************************************
//    リングエフェクトクラスの定義
//*****************************************************************************
class CRingEffect : public CMeshRing
{
public:    // 誰でもアクセス可能
	CRingEffect(int nPriority = 3, OBJTYPE objType = OBJTYPE_RINGEFFECT);
	~CRingEffect();
	static CRingEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock,
		int nLife, D3DXVECTOR3 RotSpeed, int nRotPattern, D3DXVECTOR3 Move, D3DXVECTOR3 ChangeMove, D3DXCOLOR ChangeCol, float fChangeHeight,
		float fChangeRadius, float fChangeDiffusion, bool bCulling, bool bDrawAddtive, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock,
		int nLife, D3DXVECTOR3 RotSpeed, int nRotPattern, D3DXVECTOR3 Move, D3DXVECTOR3 ChangeMove, D3DXCOLOR ChangeCol, float fChangeHeight,
		float fChangeRadius, float fChangeDiffusion, bool bCulling, bool bDrawAddtive);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeVertex(void);

	void SetLife(const int nLife);
	void SetRotSpeed(const D3DXVECTOR3 RotSpeed);
	void SetRotPattern(const int nRotPattern);
	void SetMove(const D3DXVECTOR3 Move);
	void SetChangeMove(const D3DXVECTOR3 ChangeMove);
	void SetChangeCol(const D3DXCOLOR ChangeCol);
	void SetChangeHeight(const float fChangeHeight);
	void SetChangeRadius(const float fChangeRadisu);
	void SetChangeDiffusion(const float fChangeDiffusion);
	void SetCulling(const bool bCulling);
	void SetDrawAddtive(const bool bDrawAddtive);

	int GetLife(void);
	D3DXVECTOR3 GetRotSpeed(void);
	int GetRotPattern(void);
	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetChangeMove(void);
	D3DXCOLOR GetChangeCol(void);
	float GetChangeHeight(void);
	float GetChangeRadius(void);
	float GetChangeDiffusion(void);
	bool GetCulling(void);
	bool GetDrawAddtive(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	int         m_nLife;             // 寿命
	D3DXVECTOR3 m_RotSpeed;          // 回転量
	int         m_nRotPattern;       // 回転のパターン
	D3DXVECTOR3 m_Move;              // 移動量
	D3DXVECTOR3 m_ChangeMove;        // 移動量を毎フレームどれくらい変化させるか
	D3DXCOLOR   m_ChangeCol;         // 色を毎フレームどれくらい変化させるか
	float       m_fChangeHeight;     // 高さを毎フレームどれくらい変化させるか
	float       m_fChangeRadius;     // 半径をどれくらい毎フレーム変化させるか
	float       m_fChangeDiffusion;  // リングの幅をどれくらい毎フレーム変化させるか
	bool        m_bCulling;          // カリングするかどうか
	bool        m_bDrawAddtive;      // 加算合成で描画するかしないか
};


//*****************************************************************************
//     エフェクト管轄クラスの定義
//*****************************************************************************
class CEffectManager
{
public:   // 誰でもアクセス可能
	CEffectManager();
	~CEffectManager();

	static CEffectManager *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CEmitter *SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

	void SetFileName(char *pFileName);
	void SetEmitter(CEmitter *pEmitter);
	void SetEmitterData(CEmitterData *pEmitterData, int nIdx);
	void SetParData(CParData *pParData, int nIdx);
	void SetRingEffectData(CRingData *pRingData, int nIdx);
	void SetTexManager(CTextureManager *pTexManager);
	void SetNumEmitterData(int nNumEmitterData);
	void SetNumParData(int nNumParData);
	void SetNumRingEffectData(int nNumRingEffectData);

	LPDIRECT3DTEXTURE9 BindTexture(int nIdx);
	CEmitter *GetEmitter(void);
	CEmitterData *GetEmitterData(int nIdx);
	CParData *GetParData(int nIdx);
	CRingData *GetRingEffectData(int nIdx);
	CTextureManager *GetTexManager(void);
	int GetNumEmitterData(void);
	int GetNumParData(void);
	int GetNumRingEffectData(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Save(void);
	void SaveTextureData(FILE *pFile);
	void SaveEmitterData(FILE *pFile);
	void SaveParticleData(FILE *pFile);
	void SaveRingEffectData(FILE *pFile);

	CEmitterData *ReadEmitterData(char *pLine, char *pStrCur, FILE *pFile, int nCntEmitter);
	CParData *ReadParticleData(char *pLine, char *pStrCur, FILE *pFile, int nCntParData);
	CRingData *ReadRingEffectData(char *pLine, char *pStrCur, FILE *pFile, int nCntRingData);

	char            m_aFileName[256];                  // スクリプトファイル名
	int             m_nNumEmitterData;                 // エミッタデータの個数
	int             m_nNumParData;                     // パーティクルデータの個数
	int             m_nNumRingEffectData;              // リングエフェクトデータの個数
	CEmitter        *m_pEmitter;                       // 現在生成されているエミッタへのポインタ
	CEmitterData    *m_apEmitterData[MAX_EMMITER];     // エミッタデータクラスへのポインタ
	CParData        *m_apParData[MAX_PARTICLEDATA];    // パーティクルデータクラスへのポインタ
	CRingData       *m_apRingData[MAX_RINGEFFECTDATA]; // リングエフェクトデータクラスへのポインタ
	CTextureManager *m_pTextureManager;                // テクスチャ管理クラスへのポインタ
};

#endif