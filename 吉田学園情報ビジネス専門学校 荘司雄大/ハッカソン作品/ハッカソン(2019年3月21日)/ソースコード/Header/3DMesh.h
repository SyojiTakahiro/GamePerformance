//*****************************************************************************
//
//     ���b�V���̏���[3DMesh.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _3DMESH_H_
#define _3DMESH_H_

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

	static CMeshField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nZBlock, int nTexSplitU, int nTexSplitV, char *pFileName, bool bCulling, int nPriority = 3);

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
	void SetTexSplitU(const int nTexSplitU);
	void SetTexSplitV(const int nTexSplitV);
	void SetCulling(const bool bCulling);
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
	int GetTexSplitU(void);
	int GetTexSplitV(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	bool GetCulling(void);
	D3DXMATRIX GetMtxWorld(void);

	float GetPolyHeight(D3DXVECTOR3 pos, bool *pLand, D3DXVECTOR3 *pPolyNormal);
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
	int                     m_nTexSplitU;             // �e�N�X�`��U���W�̕�����
	int                     m_nTexSplitV;             // �e�N�X�`��V���W�̕�����
	int                     m_nNumVertex;             // ���_��
	int                     m_nNumIndex;              // �C���f�b�N�X��
	int                     m_nNumPolygon;            // �|���S����
	bool                    m_bCulling;               // �J�����O���邩�ǂ���
	D3DXMATRIX              m_MtxWorld;               // ���[���h�}�g���b�N�X
};

//*****************************************************************************
//    ���b�V���E�H�[���N���X�̒�`
//*****************************************************************************
class CMeshWall : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CMeshWall(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHWALL);
	~CMeshWall();

	static CMeshWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nXBlock, int nYBlock, int nTexSplitU, int nTexSplitV, int nTexIdx, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Offset, D3DXVECTOR3 *pInterPoint = NULL);

	void SetTexIdx(const int nTexIdx);
	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetTexSplitU(const int nTexSplitU);
	void SetTexSplitV(const int nTexSplitV);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	int GetTexIdx(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetTexSplitU(void);
	int GetTexSplitV(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int                     m_nTexIdx;                // �e�N�X�`���ԍ�
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;               // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;               // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	              // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3             m_Pos;                    // ���b�V���E�H�[���̍��W
	D3DXVECTOR3             m_Rot;                    // ���b�V���E�H�[���̌���
	D3DXCOLOR               m_Col;                    // ���b�V���E�H�[���̐F
	float                   m_fWidth;                 // 1�u���b�N���̕�
	float                   m_fHeight;                // 1�u���b�N������
	int                     m_nXBlock;                // ���̕�����
	int                     m_nYBlock;                // �c�̕�����
	int                     m_nTexSplitU;             // �e�N�X�`��U���W�̕�����
	int                     m_nTexSplitV;             // �e�N�X�`��V���W�̕�����
	int                     m_nNumVertex;             // ���_��
	int                     m_nNumIndex;              // �C���f�b�N�X��
	int                     m_nNumPolygon;            // �|���S����
	D3DXMATRIX              m_MtxWorld;               // ���[���h�}�g���b�N�X
	D3DXVECTOR3             m_WallNormal;             // �ǂ̖@��
};

//*****************************************************************************
//    ���b�V���V�����_�[�N���X�̒�`
//*****************************************************************************
class CMeshCylinder : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CMeshCylinder(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHCYLINDER);
	~CMeshCylinder();
	static CMeshCylinder *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeNormal(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetHeight(const float fHeight);
	void SetRadius(const float fRadius);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetHeight(void);
	float GetRadius(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;     // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	    // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3             m_Pos;          // ���b�V���V�����_�[�̍��W
	D3DXVECTOR3             m_Rot;          // ���b�V���V�����_�[�̌���
	D3DXCOLOR               m_Col;          // ���b�V���V�����_�[�̐F
	float                   m_fHeight;      // ���b�V���V�����_�[�̍���
	float                   m_fRadius;      // ���b�V���V�����_�[�̔��a
	int                     m_nXBlock;      // ���̕�����
	int                     m_nYBlock;      // �c�̕�����
	int                     m_nNumVertex;   // ���_��
	int                     m_nNumIndex;    // �C���f�b�N�X��
	int                     m_nNumPolygon;  // �|���S����
	D3DXMATRIX              m_MtxWorld;     // ���[���h�}�g���b�N�X
};

//*****************************************************************************
//    ���b�V���h�[���N���X�̒�`
//*****************************************************************************
class CMeshDome : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CMeshDome(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHDOME);
	~CMeshDome();
	static CMeshDome *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeNormal(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetRadius(const float fRadius);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetRadius(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;     // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	    // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3             m_Pos;          // ���b�V���h�[���̍��W
	D3DXVECTOR3             m_Rot;          // ���b�V���h�[���̌���
	D3DXCOLOR               m_Col;          // ���b�V���h�[���̐F
	float                   m_fRadius;      // ���b�V���h�[���̔��a
	int                     m_nXBlock;      // ���̕�����
	int                     m_nYBlock;      // �c�̕�����
	int                     m_nNumVertex;   // ���_��
	int                     m_nNumIndex;    // �C���f�b�N�X��
	int                     m_nNumPolygon;  // �|���S����
	D3DXMATRIX              m_MtxWorld;     // ���[���h�}�g���b�N�X
};

//*****************************************************************************
//    ���b�V���X�t�B�A�N���X�̒�`
//*****************************************************************************
class CMeshSphere : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CMeshSphere(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHSPHERE);
	~CMeshSphere();
	static CMeshSphere *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeNormal(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetRadius(const float fRadius);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetRadius(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;          // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffCover[2];  // ���_�o�b�t�@�ւ̃|�C���^(�W�p)
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;          // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	         // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3             m_Pos;               // ���b�V���X�t�B�A�̍��W
	D3DXVECTOR3             m_Rot;               // ���b�V���X�t�B�A�̌���
	D3DXCOLOR               m_Col;               // ���b�V���X�t�B�A�̐F
	float                   m_fRadius;           // ���b�V���X�t�B�A�̔��a
	int                     m_nXBlock;           // ���̕�����
	int                     m_nYBlock;           // �c�̕�����
	int                     m_nNumVertex;        // ���_��
	int                     m_nNumIndex;         // �C���f�b�N�X��
	int                     m_nNumPolygon;       // �|���S����
	D3DXMATRIX              m_MtxWorld;          // ���[���h�}�g���b�N�X
};

//*****************************************************************************
//    ���b�V���I�[�r�b�g�N���X�̒�`
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

//*****************************************************************************
//    ���b�V�������O�N���X�̒�`
//*****************************************************************************
class CMeshRing : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CMeshRing(int nPriority = 3, OBJTYPE objType = OBJTYPE_MESHRING);
	~CMeshRing();
	static CMeshRing *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	virtual void MakeNormal(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetIdxBuff(const LPDIRECT3DINDEXBUFFER9 pIdxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetCol(const D3DXCOLOR col);
	void SetHeight(const float fHeight);
	void SetRadius(const float fRadius);
	void SetDiffusion(const float fDiffusion);
	void SetXBlock(const int nXBlock);
	void SetYBlock(const int nYBlock);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	LPDIRECT3DINDEXBUFFER9 GetIdxBuff(void);
	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXCOLOR GetCol(void);
	float GetHeight(void);
	float GetRadius(void);
	float GetDiffusion(void);
	int GetXBlock(void);
	int GetYBlock(void);
	int GetNumVertex(void);
	int GetNumIndex(void);
	int GetNumPolygon(void);
	D3DXMATRIX GetMtxWorld(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;     // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9  m_pIdxBuff;     // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	    // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3             m_Pos;          // ���b�V�������O�̍��W
	D3DXVECTOR3             m_Rot;          // ���b�V�������O�̌���
	D3DXCOLOR               m_Col;          // ���b�V�������O�̐F
	float                   m_fHeight;      // ���b�V�������O�̍���
	float                   m_fRadius;      // ���b�V�������O�̔��a
	float                   m_fDiffusion;   // ���b�V�������O�̊g�U�l
	int                     m_nXBlock;      // ���̕�����
	int                     m_nYBlock;      // �c�̕�����
	int                     m_nNumVertex;   // ���_��
	int                     m_nNumIndex;    // �C���f�b�N�X��
	int                     m_nNumPolygon;  // �|���S����
	D3DXMATRIX              m_MtxWorld;     // ���[���h�}�g���b�N�X
};

#endif