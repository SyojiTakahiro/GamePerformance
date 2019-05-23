//*****************************************************************************
//
//     �r���{�[�h�̏���[sceneBillboard.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENEBILLBOARD_H_
#define _SCENEBILLBOARD_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    �r���{�[�h�N���X�̒�`
//*****************************************************************************
class CSceneBillboard : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CSceneBillboard(int nPriority = 3, OBJTYPE objType = OBJTYPE_BILLBOARD);
	~CSceneBillboard();

	static CSceneBillboard *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetRot(const float fRot);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetAngle(const float fAngle);
	void SetLength(const float fLength);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetRot(void);
	float GetWidth(void);
	float GetHeight(void);
	float GetAngle(void);
	float GetLength(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	    // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3             m_Pos;          // �|���S���̍��W
	D3DXCOLOR               m_Col;          // �|���S���̐F
	float                   m_fRot;         // �|���S���̌���
	float                   m_fWidth;       // �|���S���̕�
	float                   m_fHeight;      // �|���S���̍���
	float                   m_fAngle;       // �|���S�����o���p�x
	float                   m_fLength;      // �|���S���̒���
	D3DXMATRIX              m_MtxWorld;     // ���[���h�}�g���b�N�X
};

#endif