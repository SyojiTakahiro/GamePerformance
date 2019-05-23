//*****************************************************************************
//
//     X�t�@�C�����f���̏���[sceneX.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENEX_H_
#define _SCENEX_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    X�t�@�C�����f���N���X�̒�`
//*****************************************************************************
class CSceneX : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CSceneX(int nPriority = 3, OBJTYPE objType = OBJTYPE_MODEL);
	~CSceneX();
	static CSceneX *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void BindModel(const LPDIRECT3DDEVICE9 pDevice, const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat);
	void SetVtxMin(const D3DXVECTOR3 VtxMin);
	void SetVtxMax(const D3DXVECTOR3 VtxMax);
	void SetPos(const D3DXVECTOR3 pos);
	void SetPosOld(const D3DXVECTOR3 posOld);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	D3DXVECTOR3 GetVtxMin(void);
	D3DXVECTOR3 GetVtxMax(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	LPD3DXMESH GetMesh(void);
	LPD3DXBUFFER GetBuffMat(void);
	DWORD GetNumMat(void);
	LPDIRECT3DTEXTURE9 GetTexture(int nIdxTex);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPD3DXMESH     m_apMesh;      // ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER   m_apBuffMat;   // �}�e���A�����ւ̃|�C���^
	static DWORD          m_anNumMat;    // �}�e���A�����̐�
	static D3DXVECTOR3    m_aVtxMin;     // ���_���W�̍ŏ��l
	static D3DXVECTOR3    m_aVtxMax;     // ���_���W�̍ő�l

	LPDIRECT3DTEXTURE9    *m_pTexture;  // �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH            m_pMesh;      // ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER          m_pBuffMat;   // �}�e���A�����ւ̃|�C���^
	DWORD                 m_nNumMat;    // �}�e���A�����̐�
	D3DXVECTOR3           m_VtxMin;     // ���_���W�̍ŏ��l
	D3DXVECTOR3           m_VtxMax;     // ���_���W�̍ő�l
	D3DXVECTOR3           m_Pos;        // ���݂̍��W
	D3DXVECTOR3           m_PosOld;     // �O��̍��W
	D3DXVECTOR3           m_Rot;        // ����
	D3DXMATRIX            m_MtxWorld;   // ���[���h�}�g���b�N�X
};

#endif