//*****************************************************************************
//
//     弾の処理[bullet.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _BULLET_H_
#define _BULLET_H_

//*****************************************************************************
//    インクルードファイル
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//    前方宣言
//*****************************************************************************
class CModel;
class CModelManager;
class CPlayer;

//*****************************************************************************
//    弾クラスの定義
//*****************************************************************************
class CBullet : public CScene
{
public:    // 誰でもアクセス可能
	CBullet(int nPriority = 3, OBJTYPE objType = OBJTYPE_BULLET);
	~CBullet();

	static CBullet *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, int nAttackPlayerIdx);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMove(const D3DXVECTOR3 move);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetAttackPlayerIdx(const int nAttackPlayerIdx);

	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetMove(void);
	D3DXMATRIX GetMtxWorld(void);
	int GetAttackPlayerIdx(void);

protected: // このクラスと派生クラスだけがアクセス可能

private:   // このクラスだけがアクセス可能
	void HitDecision(void);
	bool HitPlayer(CPlayer *pPlayer);

	static CModelManager *m_pModelManager;
	CModel *m_pModel;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_PosOld;
	D3DXVECTOR3 m_Rot;
	D3DXVECTOR3 m_Move;
	D3DXMATRIX m_MtxWorld;
	int m_nAttackPlayerIdx;
	int m_nLife;
};

#endif