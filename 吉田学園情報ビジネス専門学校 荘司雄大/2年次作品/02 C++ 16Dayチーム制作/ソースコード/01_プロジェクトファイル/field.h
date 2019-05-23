//*****************************************************************************
//
//     �n�ʂ̏���[field.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _FIELD_H_
#define _FIELD_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene3DMesh.h"

//*****************************************************************************
//    �t�B�[���h�N���X�̒�`
//*****************************************************************************
class CField : public CScene3DMesh
{
public:    // �N�ł��A�N�Z�X�\
	CField(int nPriority = 3, OBJTYPE objType = OBJTYPE_FIELD);
	~CField();
	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fDepth, int nXBlock, int nZBlock, char *aVertexFileName, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fDepth, int nXBlock, int nZBlock, char *aVertexFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeNormal(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	float GetPolyHeight(const D3DXVECTOR3 pos, D3DXVECTOR3 *pPolygonNormal);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9 m_apTexture;  // �e�N�X�`���ւ̃|�C���^
	float m_fWidth;                // 1�u���b�N���̕�
	float m_fDepth;                // 1�u���b�N���̉��s
	int   m_nXBlock;               // ���̕�����
  	int   m_nZBlock;               // ���s�̕�����
	char  m_aVertexFileName[256];  // ���_���ւ̃X�N���v�g�t�@�C����
};

#endif