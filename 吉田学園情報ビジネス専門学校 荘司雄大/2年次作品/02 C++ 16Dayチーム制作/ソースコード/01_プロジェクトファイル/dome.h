//*****************************************************************************
//
//     �����̏���[dome.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _DOME_H_
#define _DOME_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene3DMesh.h"

//*****************************************************************************
//    �����N���X�̒�`
//*****************************************************************************
class CDome : public CScene3DMesh
{
public:    // �N�ł��A�N�Z�X�\
	CDome(int nPriority = 3,OBJTYPE objType = OBJTYPE_DOME);
	~CDome();
	static CDome *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, int nXBlock, int nYBlock);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9 m_apTexture;  // �e�N�X�`���ւ̃|�C���^
	float m_fRadius;                        // �����̔��a
	int   m_nXBlock;                        // ���̕�����
	int   m_nYBlock;                        // �c�̕�����
};

#endif