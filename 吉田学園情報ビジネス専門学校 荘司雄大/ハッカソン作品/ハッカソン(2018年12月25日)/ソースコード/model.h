//*****************************************************************************
//
//     ���f���̏���[sceneX.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MODEL_H_
#define _MODEL_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    ���f���N���X�̒�`
//*****************************************************************************
class CModel
{
public:    // �N�ł��A�N�Z�X�\
	CModel();
	~CModel();

	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD nNumMat, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void BindModel(const LPD3DXMESH pMesh, const LPD3DXBUFFER pBuffMat, const DWORD nNumMat, const D3DXVECTOR3 VtxMax, const D3DXVECTOR3 VtxMin, LPDIRECT3DTEXTURE9 *pTexture);
	void BindTexture(LPDIRECT3DTEXTURE9 *pTexture);
	void SetMesh(const LPD3DXMESH pMesh);
	void SetBuffMat(const LPD3DXBUFFER pBuffMat);
	void SetNumMat(const DWORD nNumMat);
	void SetVtxMin(const D3DXVECTOR3 VtxMin);
	void SetVtxMax(const D3DXVECTOR3 VtxMax);
	void SetPos(const D3DXVECTOR3 pos);
	void SetAddPos(const D3DXVECTOR3 Addpos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetParent(CModel *pModel);

	LPD3DXMESH GetMesh(void);
	LPD3DXBUFFER GetBuffMat(void);
	DWORD GetNumMat(void);
	D3DXVECTOR3 GetVtxMin(void);
	D3DXVECTOR3 GetVtxMax(void);
	LPDIRECT3DTEXTURE9 *GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetAddPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXMATRIX *GetMtxWorldPointer(void);
	CModel *GetParent(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DTEXTURE9    *m_pTexture;  // �e�N�X�`���ւ̃|�C���^
	LPD3DXMESH            m_pMesh;      // ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER          m_pBuffMat;   // �}�e���A�����ւ̃|�C���^
	DWORD                 m_nNumMat;    // �}�e���A�����̐�
	D3DXVECTOR3           m_VtxMin;     // ���_���W�̍ŏ��l
	D3DXVECTOR3           m_VtxMax;     // ���_���W�̍ő�l
	D3DXVECTOR3           m_Pos;        // ���݂̍��W
	D3DXVECTOR3           m_AddPos;     // �I�t�Z�b�g�ɉ�������W
	D3DXVECTOR3           m_Rot;        // ����
	D3DXMATRIX            m_MtxWorld;   // ���[���h�}�g���b�N�X
	CModel                *m_pParent;   // �e���f���ւ̃|�C���^
};

#endif