//*****************************************************************************
//
//     �O�Ղ̏���[meshOrbit.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MESHORBIT_H_
#define _MESHORBIT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    �O�ՃN���X�̒�`
//*****************************************************************************
class CMeshOrbit : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CMeshOrbit(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHORBIT);
	~CMeshOrbit();

	static CMeshOrbit *Create(D3DXVECTOR3 OffsetPos1, D3DXVECTOR3 OffsetPos2, int nXBlock, int nZBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetOffsetPos(const D3DXVECTOR3 OffsetPos, int nIdx);
	void SetXBlock(const int nXBlock);
	void SetZBlock(const int nZBlock);
	void SetVertexPos(D3DXVECTOR3 pos, int nIdx);
	void SetVertexCol(D3DXCOLOR col, int nIdx);
	void SetVertexPos(D3DXVECTOR3 *pPos);
	void SetVertexCol(D3DXCOLOR *pCol);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetMtxParent(D3DXMATRIX *pMtxParent);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetOffsetPos(int nIdx);
	int GetXBlock(void);
	int GetZBlock(void);
	int GetNumVertex(void);
	int GetNumPolygon(void);
	D3DXVECTOR3 *GetVertexPos(void);
	D3DXCOLOR *GetVertexCol(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXMATRIX *GetMtxParent(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;      // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	     // �e�N�X�`���ւ̃|�C���^
	int                     m_nXBlock;       // ���̕�����
	int                     m_nZBlock;       // ���s�̕�����
	int                     m_nNumVertex;    // ���_��
	int                     m_nNumPolygon;   // �|���S����
	D3DXVECTOR3             *m_pVertexPos;   // ���_�̍��W(���_�������I�Ɋm��)
	D3DXVECTOR3             m_aOffsetPos[2]; // ���_�̃I�t�Z�b�g
	D3DXCOLOR               *m_pVertexCol;   // ���_�̐F(���_�������I�Ɋm��)
	D3DXMATRIX              m_MtxWorld;      // ���[���h�}�g���b�N�X
	D3DXMATRIX              *m_pMtxParent;   // �e�̃}�g���b�N�X�ւ̃|�C���^
};

#endif