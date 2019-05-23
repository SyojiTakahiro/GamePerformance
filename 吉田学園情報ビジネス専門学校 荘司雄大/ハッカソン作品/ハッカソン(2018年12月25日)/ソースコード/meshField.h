//*****************************************************************************
//
//     ���b�V���t�B�[���h�̏���[meshField.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    ���b�V���t�B�[���h�N���X�̒�`
//*****************************************************************************
class CMeshField : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CMeshField(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHFIELD);
	~CMeshField();

	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nZBlock, char *pFileName, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeNormal(void);

	void SetBinaryFileName(char *pFileName);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetXBlock(const int nXBlock);
	void SetZBlock(const int nZBlock);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	char *GetBinaryFileName(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetXBlock(void);
	int GetZBlock(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	D3DXMATRIX GetMtxWorld(void);

	float GetPolyHeight(D3DXVECTOR3 pos);
	void SetPolyHeight(const D3DXVECTOR3 pos, const float fValue, const float fRange);
	void SaveMesh(void);
	void LoadMesh(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	char                    m_aBinaryFileName[256];   // ���_��񂪊i�[���ꂽ�o�C�i���t�@�C����
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;               // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;               // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	              // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3             m_Pos;                    // ���b�V���t�B�[���h�̍��W
	D3DXVECTOR3             m_Rot;                    // ���b�V���t�B�[���h�̌���
	D3DXCOLOR               m_Col;                    // ���b�V���t�B�[���h�̐F
	float                   m_fWidth;                 // 1�u���b�N���̕�
	float                   m_fHeight;                // 1�u���b�N������
	int                     m_nXBlock;                // ���̕�����
	int                     m_nZBlock;                // ���s�̕�����
	int                     m_nNumVertex;             // ���_��
	int                     m_nNumIndex;              // �C���f�b�N�X��
	int                     m_nNumPolygon;            // �|���S����
	D3DXMATRIX              m_MtxWorld;               // ���[���h�}�g���b�N�X
};

#endif