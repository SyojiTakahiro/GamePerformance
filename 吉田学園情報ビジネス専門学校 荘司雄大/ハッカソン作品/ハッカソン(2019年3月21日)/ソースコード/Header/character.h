//*****************************************************************************
//
//     キャラクターの処理[character.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "scene.h"

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
class CTextureManager;
class CShadow;
class CCamera;

//*****************************************************************************
//    キャラクタークラスの定義
//*****************************************************************************
class CCharacter : public CScene
{
public:    // 誰でもアクセス可能
	CCharacter(int nPriority = 3, OBJTYPE objType = OBJTYPE_CHARACTER);
	~CCharacter();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(LPDIRECT3DDEVICE9 pDevice);
	void SetDefaultPos(void);
	void SetDefaultRot(void);
	void SetModelAlpha(float fAlpha);

	void SetNumParts(const int nNumParts);
	void SetModel(CModel **pModel);
	void SetModel(CModel *pModel, const int nIdx);
	void SetMotionManager(CMotionManager *pMotionManager);
	void SetTextureManager(CTextureManager *pTextureManager);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetPos(const D3DXVECTOR3 pos);
	void SetPosOld(const D3DXVECTOR3 posOld);
	void SetMove(const D3DXVECTOR3 Move);
	void SetRot(const D3DXVECTOR3 rot);
	void SetDestRot(const D3DXVECTOR3 Destrot);
	void SetDiffRot(const D3DXVECTOR3 Diffrot);
	void SetGravity(const float fGravity);
	void SetColHeight(const float fColHeight);
	void SetColRange(const float fColRange);
	void SetJumpPower(const float fJumpPower);
	void SetRivisionRot(const float fRivisionRot);
	void SetAccel(const float fAccel);
	void SetInertia(const float fInertia);
	void SetLand(const bool bLand);
	void SetLandScene(CScene *pLandScene);
	void SetLandSceneOld(CScene *pLandSceneOld);
	void SetLandOffsetPos(const D3DXVECTOR3 LandOffsetPos);
	void SetLandOffsetAdd(const D3DXVECTOR3 LandOffsetAdd);
	void SetLandOffsetRot(const D3DXVECTOR3 LandOffsetRot);
	void SetLandOffsetRotStart(const D3DXVECTOR3 LandOffsetRotStart);
	void SetShadow(CShadow *pShadow);

	int GetNumParts(void);
	CModel **GetModel(void);
	CModel *GetModel(const int nIdx);
	CMotionManager *GetMotionManager(void);
	CTextureManager *GetTextureManager(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetDestRot(void);
	D3DXVECTOR3 GetDiffRot(void);
	float GetGravity(void);
	float GetColHeight(void);
	float GetColRange(void);
	float GetJumpPower(void);
	float GetRivisionRot(void);
	float GetAccel(void);
	float GetInertia(void);
	bool GetLand(void);
	CScene *GetLandScene(void);
	CScene *GetLandSceneOld(void);
	D3DXVECTOR3 GetLandOffsetPos(void);
	D3DXVECTOR3 GetLandOffsetAdd(void);
	D3DXVECTOR3 GetLandOffsetRot(void);
	D3DXVECTOR3 GetLandOffsetRotStart(void);
	CShadow *GetShadow(void);

protected: // このクラスと派生クラスだけがアクセス可能
	void Movement(void);
	void Collision(void);
	void CalcOffsetRot(void);

private:   // このクラスだけがアクセス可能

	int             m_nNumParts;             // 使用するパーツ数
	CModel          **m_apModel;             // モデルクラスへのポインタ
	CMotionManager  *m_pMotionManager;       // モーションクラスへのポインタ
	CTextureManager *m_pTextureManager;      // テクスチャ管轄クラスへのポインタ
	D3DXMATRIX      m_MtxWorld;              // ワールドマトリックス
	D3DXVECTOR3     m_Pos;                   // 現在の座標
	D3DXVECTOR3     m_PosOld;                // 前回の座標
	D3DXVECTOR3     m_Move;                  // 移動量
	D3DXVECTOR3     m_Rot;                   // 現在の向き
	D3DXVECTOR3     m_DestRot;               // 目的の向き
	D3DXVECTOR3     m_DiffRot;               // 目的の向きと現在の向きの差分
	float           m_fGravity;              // 重力
	float           m_fColHeight;            // 当たり判定をとる高さ
	float           m_fColRange;             // 当たり判定を取る範囲
	float           m_fJumpPower;            // ジャンプ量
	float           m_fRivisionRot;          // 向きを補正する倍率
	float           m_fAccel;                // 移動スピード
	float           m_fInertia;              // 慣性
	bool            m_bLand;                 // 前回オブジェクトに乗っていたかどうか
	CScene          *m_pLandScene;           // 乗っているオブジェクトへのポインタ
	CScene          *m_pLandSceneOld;        // 前回乗っていたオブジェクトへのポインタ
	D3DXVECTOR3     m_LandOffsetPos;         // オブジェクトからのオフセット距離
	D3DXVECTOR3     m_LandOffsetAdd;         // オフセット距離に加える値
	D3DXVECTOR3     m_LandOffsetRot;         // オブジェクトからのオフセット向き
	D3DXVECTOR3     m_LandOffsetRotStart;    // 乗ってからの向き
	CShadow         *m_pShadow;              // 影クラスへのポインタ
};

//*****************************************************************************
//    プレイヤークラスの定義
//*****************************************************************************
class CPlayer : public CCharacter
{
public:    // 誰でもアクセス可能
	//--------------------
    //  状態の種類
	//--------------------
	typedef enum
	{
		STATE_NORMAL = 0,   // 通常状態
		STATE_WINNER,       // 勝利状態
		STATE_LOSER,        // 敗北状態
		STATE_MAX
	}STATE;

	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, int nControllerIdx, int nPriority = 3);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, int nControllerIdx);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Damage(D3DXVECTOR3 BulletMove);
	void AddScore(void);

	void SetState(const STATE State);
	void SetControllerIdx(const int nIdx);
	void SetCamera(CCamera *pCamera);

	STATE GetState(void);
	int GetControllerIdx(void);
	CCamera *GetCamera(void);
	int GetScore(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void InputMove(void);
	void Action(void);
	void Statement(void);
	void Landing(void);
	bool AttackCollision(int nModelIdx, float fRange, D3DXVECTOR3 Offset, int nDamage);

	void CreateBoostEffect(void);

	STATE m_State;              // 状態
	int   m_nControllerIdx;     // コントローラーの番号
	CCamera *m_pCamera;         // カメラクラスへのポインタ
	bool m_bDamage;             // ダメージを受けたかどうか
	bool m_bAttack;             // 攻撃をくらうかどうか
	int m_nDamageCounter;       // ダメージを受けた時間を管理するカウンター
	int m_nNumScore;            // キャラクターが星を集めた数
};

//*****************************************************************************
//    敵クラスの定義
//*****************************************************************************
class CEnemy : public CCharacter
{
public:    // 誰でもアクセス可能
	//--------------------
	//  種類
	//--------------------
	typedef enum
	{
		TYPE_NORMAL = 0,   // 通常の種類
		TYPE_MAX
	}TYPE;

	CEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemy();

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange, int nPriority = 3);
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, CTextureManager *pTextureManager, int nNumParts, float fAccel, float fInertia, float fJumpPower, float fGravity, float fRivisionRot, float fColHeight, float fColRange);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
};

//*****************************************************************************
//    キャラクター管轄クラスの定義
//*****************************************************************************
class CCharacterManager
{
public:    // 誰でもアクセス可能
	CCharacterManager();
	~CCharacterManager();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	void SetFileName(char *pFileName);
	void SetModelManager(CModelManager *pModelManager);
	void SetTextureManager(CTextureManager *pTextureManager);

	char *GetFileName(void);
	CModelManager *GetModelManager(void);
	CTextureManager *GetTextureManager(void);

protected: // このクラスと派生クラスだけがアクセス可能
	CModel **ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pAccel, float *pInertia, float *pJumpPower, float *pRivisionRot, float *pGravity, float *pColHeight, float *pColRange);
	CModel *ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel);
	CMotionAttack *ReadMotionAttackSet(char *pLine, char *pStrCur, FILE *pFile);
	CMotionCollision **ReadMotionCollisionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumColData);
	CMotionOrbit **ReadMotionOrbitSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumOrbitData);
	CMotion *ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	void ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);

private:   // このクラスだけがアクセス可能
	char            m_aFileName[256];    // キャラクターデータのスクリプトファイル名
	CModelManager   *m_pModelManager;    // モデル管轄クラスへのポインタ
	CTextureManager *m_pTextureManager;  // テクスチャ管轄クラスへのポインタ
};

//*****************************************************************************
//    プレイヤー管轄クラスの定義
//*****************************************************************************
class CPlayerManager : public CCharacterManager
{
public:    // 誰でもアクセス可能
	CPlayerManager();
	~CPlayerManager();

	static CPlayerManager *Create(char *pFileName, int nControllerIdx = 0);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetControllerIdx(const int nControllerIdx);

	int GetControllerIdx(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreatePlayer(CModel **apModel, CModelManager *pModelManager , CTextureManager *pTextureManager, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange);

	int m_nControllerIdx;
};

//*****************************************************************************
//    エネミー管轄クラスの定義
//*****************************************************************************
class CEnemyManager : public CCharacterManager
{
public:    // 誰でもアクセス可能
	CEnemyManager();
	~CEnemyManager();

	static CEnemyManager *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void CreateEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fAccel, const float fInertia, const float fJumpPower, const float fRivisionRot, const float fGravity, const float fColHeight, const float fColRange);
};

//*****************************************************************************
//    エネミー管轄まとめクラスの定義
//*****************************************************************************
class CEnemyAssembly
{
public:    // 誰でもアクセス可能
	CEnemyAssembly();
	~CEnemyAssembly();

	void SetEnemyManager(CEnemyManager *pEnemyManager, int nIdx);
	CEnemyManager *GetEnemyManager(int nIdx);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	CEnemyManager *m_pEnemyManager[CEnemy::TYPE_MAX];   // 敵管轄クラスへのポインタ
};

#endif