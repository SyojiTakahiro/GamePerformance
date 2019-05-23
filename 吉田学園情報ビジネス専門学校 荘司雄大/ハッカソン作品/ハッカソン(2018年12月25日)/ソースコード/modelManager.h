//*****************************************************************************
//
//     ���f���Ǌ��̏���[modelManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************

//*****************************************************************************
//    ���f���Ǌ��N���X�̒�`
//*****************************************************************************
class CModelManager
{
public:    // �N�ł��A�N�Z�X�\
	CModelManager();
	~CModelManager();
	static CModelManager *Create(int nNumModel);
	HRESULT Init(void);
	void Uninit(void);

	void SetNumModel(int nNumModel);
	void SetMesh(LPD3DXMESH pMesh, int nIdx);
	void SetMaterial(LPDIRECT3DDEVICE9 pDevice, LPD3DXBUFFER pBuffMat, DWORD nNumMat, int nIdx);
	void SetBuffMat(LPD3DXBUFFER pBuffMat, int nIdx);
	void SetNumMat(DWORD nNumMat, int nIdx);
	void SetVtxMax(D3DXVECTOR3 nNumMat, int nIdx);
	void SetVtxMin(D3DXVECTOR3 nNumMat, int nIdx);
	void BindTexture(LPDIRECT3DTEXTURE9 *pTexture, int nIdx);

	LPD3DXMESH GetMesh(int nIdx);
	LPD3DXBUFFER GetBuffMat(int nIdx);
	DWORD GetNumMat(int nIdx);
	D3DXVECTOR3 GetVtxMax(int nIdx);
	D3DXVECTOR3 GetVtxMin(int nIdx);
	LPDIRECT3DTEXTURE9 *GetTexture(int nIdx);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void ReleaseMesh(void);
	void ReleaseBuffMat(void);
	void ReleaseNumMat(void);
	void ReleaseVtxMax(void);
	void ReleaseVtxMin(void);
	void ReleaseTexture(void);

	int                   m_nNumModel;    // �ǂݍ��񂾃��f���̐�
	LPD3DXMESH            *m_pMesh;       // ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER          *m_pBuffMat;    // �}�e���A�����ւ̃|�C���^
	DWORD                 *m_pNumMat;     // �}�e���A�����̐�
	D3DXVECTOR3           *m_pVtxMax;     // ���_���W�̍ő�l�ւ̃|�C���^
	D3DXVECTOR3           *m_pVtxMin;     // ���_���W�̍ŏ��l�ւ̃|�C���^
	LPDIRECT3DTEXTURE9    **m_pTexture;   // �e�N�X�`���ւ̃|�C���^(�}�e���A����񂩂甲���o��)
};

#endif