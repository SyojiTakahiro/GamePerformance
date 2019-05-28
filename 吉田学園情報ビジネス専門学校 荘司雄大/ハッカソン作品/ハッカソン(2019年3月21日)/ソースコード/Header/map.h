//*****************************************************************************
//
//     マップの処理[map.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MAP_H_
#define _MAP_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    マクロ定義
//*****************************************************************************
#define SKY_PRIORITY       (1)   // 空の処理優先順位
#define MOUNTAIN_PRIORITY  (2)   // 山の処理優先順位
#define FIELD_PRIORITY     (3)   // 地面の処理優先順位
#define BILLBOARD_PRIORITY (5)   // ビルボードの処理優先順位
#define OBJECT_PRIORITY    (4)   // オブジェクトの処理優先順位
#define WALL_PRIORITY      (5)   // 壁の処理優先順位
#define MAP_PRIORITY       (6)   // マップの処理優先順位

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CTextureManager;
class CModelManager;
class CEffectManager;
class CObjectManager;

//*****************************************************************************
//     マップクラスの定義
//*****************************************************************************
class CMap
{
public:    // 誰でもアクセス可能
	CMap();
	~CMap();

	static CMap *Create(char *aFileName, CEffectManager *pEffectManager);

	void Init(char *aFileName, CEffectManager *pEffectManager);
	void Uninit(void);
	void SetBillboard(D3DXVECTOR3 Pos, D3DXCOLOR Col, float fWidth, float fHeight, float fRot, bool bLighting, bool bDrawAddtive, int nTexIdx, int nType);
	void SetEffect(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, int nType);
	void SetEnemy(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, int nType, float fAI_CheackDistance, float fAI_AttackDistance, int nAI_AttackTiming, int nLife, int nScore);
	void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx);

	char *GetMapName(void);
	int GetCntEnemyLoad(void);
	CModelManager *GetModelManager(void);
	CTextureManager *GetTextureManager(void);
	CObjectManager *GetObjectManager(int nIdx);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void Save(void);
	void SaveObject(void);
	void SaveEnemy(void);
	void SaveBillboard(void);
	void SaveEffect(void);
	void SaveWall(void);

	void FieldLoad(char *pFieldFileName, char *pStrCur, char *pLine);
	void ObjectLoad(char *pObjectFileName, char *pStrCur, char *pLine);
	void MountainLoad(char *pMountainFileName, char *pStrCur, char *pLine);
	void SkyLoad(char *pSkyFileName, char *pStrCur, char *pLine);
	//void EnemyLoad(char *pEnemyFileName, char *pStrCur, char *pLine);
	void BillboardLoad(char *pBillboardFileName, char *pStrCur, char *pLine);
	void EffectLoad(char *pEffectFileName, char *pStrCur, char *pLine);
	void WallLoad(char *pWallFileName, char *pStrCur, char *pLine);

	CModelManager   *m_pModelManager;      // モデル管轄クラスへのポインタ
	CTextureManager *m_pTextureManager;    // テクスチャ管理クラスへのポインタ
	CEffectManager  *m_pEffectManager;     // エフェクト管轄クラスへのポインタ
	//CEnemyAssembly  *m_pEnemyAssembly;     // 敵管轄まとめクラスへのポインタ
	CObjectManager  *m_pObjectManager[50];     // オブジェクト管轄クラスへのポインタ
	int             m_nCntLoadEnemy;       // 敵データを読み込んだ回数
	char            m_aFileName[256];      // 読み込むスクリプトファイル名
};

#endif