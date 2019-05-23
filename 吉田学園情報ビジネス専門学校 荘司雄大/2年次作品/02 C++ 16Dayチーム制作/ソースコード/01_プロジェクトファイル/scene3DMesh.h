//*****************************************************************************
//
//     �V�[��(3D)���b�V���̏���[scene3DMesh.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE3DMESH_H_
#define _SCENE3DMESH_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene3D.h"

//*****************************************************************************
//    �V�[��(3D)���b�V���N���X�̒�`
//*****************************************************************************
class CScene3DMesh : public CScene3D
{
public:    // �N�ł��A�N�Z�X�\
	CScene3DMesh(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESH);
	~CScene3DMesh();
	static CScene3DMesh *Create(int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	virtual void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetNumVertex(const int nNumVertex);
	void SetNumIndex(const int nNumIndex);
	void SetNumPolygon(const int nNumPolygon);

	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9 m_apTexture;  // �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;     // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	int                     m_nNumVertex;   // ���_��
	int                     m_nNumIndex;    // �C���f�b�N�X��
	int                     m_nNumPolygon;  // �|���S����
};

#endif