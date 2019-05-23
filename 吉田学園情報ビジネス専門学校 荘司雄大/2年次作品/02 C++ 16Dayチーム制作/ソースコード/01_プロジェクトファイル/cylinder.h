//*****************************************************************************
//
//     �~���̏���[cylinder.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CYLINDER_H_
#define _CYLINDER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene3DMesh.h"

//*****************************************************************************
//    �~���N���X�̒�`
//*****************************************************************************
class CCylinder : public CScene3DMesh
{
public:    // �N�ł��A�N�Z�X�\
	CCylinder(int nPriority = 3, OBJTYPE objType = OBJTYPE_CYLINDER);
	~CCylinder();
	static CCylinder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nXBlock, int nYBlock, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fHeight, int nXBlock, int nYBlock);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9 m_apTexture;  // �e�N�X�`���ւ̃|�C���^
	float m_fRadius;  // �~���̔��a
	float m_fHeight;  // 1�u���b�N���̍���
	int   m_nXBlock;  // ���̕�����
	int   m_nYBlock;  // �c�̕�����
};

#endif