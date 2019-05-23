//*****************************************************************************
//
//     敵の処理[enemy.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ENEMY_H_
#define _ENEMY_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "main.h"
#include "character.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CMotion;
class CGaugeLife;

//*****************************************************************************
//    エネミー管轄クラスの定義
//*****************************************************************************
class CEnemyManager : CScene
{
public:    // 誰でもアクセス可能
	CEnemyManager(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMYMANAGER);
	~CEnemyManager();

	static CEnemyManager *Create(char *pFileName, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetFileName(char *pFileName);
	void SetModelManager(CModelManager *pModelManager);

	char *GetFileName(void);
	CModelManager *GetModelManager(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void SetEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, char *pSetFileName);
	void CreateEnemy(CModel **apModel, CMotionManager *pMotionManager, const int nNumParts, int *pNumParent, const float fGravity, const float fColHeight, const float fColRange, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nLife, const int nAddScore = 1000);
	CModel **ReadCharacterSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, float *pGravity, float *pColHeight, float *pColRange);
	CModel *ReadPartsSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, int *pNumParent, CModel **apModel);
	CMotion *ReadMotionSet(char *pLine, char *pStrCur, FILE *pFile, int *pNumParts, CMotion *pMotion, int nCntMotion);
	void ReadKeySet(char *pLine, char *pStrCur, FILE *pFile, int nCntKeySet, int *pPlaybackKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);
	void ReadKey(char *pLine, char *pStrCur, FILE *pFile, int nCntKey, D3DXVECTOR3 *AddPos, D3DXVECTOR3 *DestRot);

	char          m_aFileName[256];  // プレイヤーデータのスクリプトファイル名
	CModelManager *m_pModelManager;  // モデル管轄クラスへのポインタ
	int m_nCntPop;					 // ポップする順番
};

//*****************************************************************************
//    エネミークラスの定義
//*****************************************************************************
class CEnemy : public CCharacter
{
public:    // 誰でもアクセス可能
	typedef enum
	{
		STATE_NORMAL = 0,   // 通常状態
		STATE_MOVE,         // 移動状態
		STATE_FALL,         // 落下中モーション
		STATE_LANDING,      // 着地状態
		STATE_DEATH,        // 死亡状態
		STATE_MAX
	}STATE;

	CEnemy(int nPriority = 3, OBJTYPE objType = OBJTYPE_ENEMY);
	~CEnemy();

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife, int nAddScore = 1000, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel **apModel, CMotionManager *pMotionManager, int nNumParts, float fGravity, float fColHeight, float fColRange, int nLife,int nAddScore = 1000);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE State);
	void SetLife(const int nLife);
	void SetAddScore(const int nAddScore);

	STATE GetState(void);
	int GetLife(void);
	int GetAddScore(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Collision(void);
	void Statement(void);
	void Death(void);
	bool CollisionPlayer(D3DXVECTOR3 pos, float fRange);

	STATE       m_State;       // 状態
	D3DXVECTOR3 m_Move;        // 移動量
	int         m_nLife;       // 体力
	int         m_nAddScore;   // 倒した時に増えるスコア量
	CGaugeLife  *m_pGaugeLife; // 体力ゲージ
	bool		m_bMoveRot;	   // 進行方向
};

#endif