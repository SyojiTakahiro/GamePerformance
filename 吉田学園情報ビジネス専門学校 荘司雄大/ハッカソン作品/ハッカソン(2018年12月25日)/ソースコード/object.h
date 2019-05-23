//*****************************************************************************
//
//     配置物の処理[object.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _OBJECT_H_
#define _OBJECT_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "sceneX.h"

//*****************************************************************************
//    オブジェクトクラスの定義
//*****************************************************************************
class CObject : public CSceneX
{
public:    // 誰でもアクセス可能
	CObject(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECT);
	~CObject();

	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);
	void SetTransParent(float fAlpha);

	void SetDisp(const bool bDisp);
	void SetRollSpeed(const D3DXVECTOR3 RollSpeed);
	void SetPosOld(const D3DXVECTOR3 PosOld);
	void SetModelIdx(const int ModelIdx) { m_ModelIdx = ModelIdx; };
	void SetRotStd(const D3DXVECTOR3 RotStd);

	bool GetDisp(void);
	int GetModelIdx(void) { return m_ModelIdx; };
	D3DXVECTOR3 GetRollSpeed(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetRotStd(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	bool        m_bDisp;        // 描画するかしないか
	int         m_nDispCounter; // 描画を管理するカウンター
	int         m_ModelIdx;     // 使用しているモデルの番号
	D3DXVECTOR3 m_PosOld;       // 前回の座標
	D3DXVECTOR3 m_RollSpeed;    // 向きを回転させるスピード
	D3DXVECTOR3 m_PosStd;       // モデルの座標(生成時)
	D3DXVECTOR3 m_RotStd;       // モデルの向き(生成時)
};

//*****************************************************************************
//    動くオブジェクトクラスの定義
//*****************************************************************************
class CObjectMove : public CObject
{
public:    // 誰でもアクセス可能
	CObjectMove(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTMOVE);
	~CObjectMove();

	static CObjectMove *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, D3DXVECTOR3 Move, int nMaxMoveCounter);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);

	void SetMove(const D3DXVECTOR3 Move);
	void SetMoveCurrent(const D3DXVECTOR3 MoveCurrent);
	void SetCounter(const int nCounter);
	void SetMaxMoveCounter(const int nMaxMoveCounter);
	void SetRivisionMove(const float fRivisionMove);
	void SetPosStd(const D3DXVECTOR3 PosStd);

	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetMoveCurrent(void);
	int GetCounter(void);
	int GetMaxMoveCounter(void);
	float GetRivisionMove(void);
	D3DXVECTOR3 GetPosStd(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	D3DXVECTOR3 m_Move;             // 移動量
	D3DXVECTOR3 m_MoveCurrent;      // 現在の移動量
	int         m_nCounter;         // 移動量を管理するカウンター
	int         m_nMaxMoveCounter;  // 移動量が最大値になる時間
	float       m_fRivisionMove;    // 移動の倍率
	D3DXVECTOR3 m_PosStd;           // モデルの座標(生成時)
};


//*****************************************************************************
//    ゴール用オブジェクトクラスの定義
//*****************************************************************************
class CObjectGoal : public CObject
{
public:    // 誰でもアクセス可能
	CObjectGoal(int nPriority = 3, OBJTYPE objType = OBJTYPE_OBJECTGOAL);
	~CObjectGoal();

	static CObjectGoal *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 RollSpeed);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fRange, CScene *pRideObject);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};
#endif