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

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CMeshField;
class CMountain;
class CSky;
class CTextureManager;
class CModelManager;

//*****************************************************************************
//     マップクラスの定義
//*****************************************************************************
class CMap
{
public:    // 誰でもアクセス可能
	CMap();
	~CMap();

	static CMap *Create(char *aFileName);

	void Init(char *aFileName);
	void Uninit(void);
	void SetObject(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 RollSpeed, int nModelIdx);
	void SetObjectMove(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR3 RollSpeed, int nModelIdx, D3DXVECTOR3 Move, int nMaxMoveCounter);

	char *GetMapName(void);
	CModelManager *GetModelManager(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void SaveObject(void);
	void FieldLoad(char *pFieldFileName, char *pStrCur, char *pLine);
	void ObjectLoad(char *pObjectFileName, char *pStrCur, char *pLine);
	void MountainLoad(char *pMountainFileName, char *pStrCur, char *pLine);
	void SkyLoad(char *pSkyFileName, char *pStrCur, char *pLine);

	CModelManager   *m_pModelManager;      // モデル管轄クラスへのポインタ
	CTextureManager *m_pTextureManager;    // テクスチャ管理クラスへのポインタ
	CMeshField      *m_pField;             // 地面クラスへのポインタ
	CMountain       *m_pMoutain;           // 山クラスへのポインタ
	CSky            *m_pSky;               // 空クラスへのポインタ
	char            m_aFileName[256];      // 読み込むスクリプトファイル名
};

#endif