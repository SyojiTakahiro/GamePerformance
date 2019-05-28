//*****************************************************************************
//
//     ���̏���[star.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _STAR_H_
#define _STAR_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "scene.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CModel;
class CModelManager;
class CPlayer;

//*****************************************************************************
//    ���N���X�̒�`
//*****************************************************************************
class CStar : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CStar(int nPriority = 3, OBJTYPE objType = OBJTYPE_STAR);
	~CStar();

	static CStar *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Move);
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

	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetMove(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void HitDecision(void);
	bool HitPlayer(CPlayer *pPlayer);

	static CModelManager *m_pModelManager;
	CModel *m_pModel;
	D3DXVECTOR3 m_Pos;
	D3DXVECTOR3 m_PosOld;
	D3DXVECTOR3 m_Move;
	D3DXVECTOR3 m_Rot;
	D3DXMATRIX m_MtxWorld;
	int m_nAttackPlayerIdx;
	int m_nLife;
	bool m_bGetOk;
};

#endif