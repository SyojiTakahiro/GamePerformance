//*****************************************************************************
//
//     3D�|���S���̏���[scene3D.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    3D�|���S���N���X�̒�`
//*****************************************************************************
class CScene3D : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CScene3D(int nPriority = 3, OBJTYPE objType = OBJTYPE_3DPOLYGON);
	~CScene3D();

	static CScene3D *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetAngle(const float fAngle);
	void SetLength(const float fLength);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	float GetAngle(void);
	float GetLength(void);
	D3DXMATRIX GetMtxWorld(void);
	float GetPolyHeight(D3DXVECTOR3 pos);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	    // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3             m_Pos;          // �|���S���̍��W
	D3DXVECTOR3             m_Rot;          // �|���S���̌���
	D3DXCOLOR               m_Col;          // �|���S���̐F
	float                   m_fWidth;       // �|���S���̕�
	float                   m_fHeight;      // �|���S���̍���
	float                   m_fAngle;       // �|���S���̊p�x
	float                   m_fLength;      // �|���S���̒���
	D3DXMATRIX              m_MtxWorld;     // ���[���h�}�g���b�N�X
};

#endif