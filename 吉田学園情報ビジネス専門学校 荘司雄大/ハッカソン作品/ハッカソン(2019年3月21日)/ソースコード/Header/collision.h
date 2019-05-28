//*****************************************************************************
//
//     �R���W�����̏���[collision.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _COLLISION_H_
#define _COLLISION_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �R���W�����N���X�̒�`
//*****************************************************************************
class CCollision
{
public:    // �N�ł��A�N�Z�X�\
	//--------------------
	//  ���
	//--------------------
	typedef enum
	{
		TYPE_NONE = -1,   // �o�^�Ȃ�
		TYPE_BOX,         // �{�b�N�X
		TYPE_SPHERE,      // �X�t�B�A
		TYPE_CIRCLE,      // �T�[�N��
		TYPE_MAX
	}TYPE;

	CCollision();
	~CCollision();

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual void SetMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	void SetType(const int nType);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetParent(CCollision *pParent);

	int GetType(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	CCollision *GetParent(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int         m_nType;       // �����蔻��̎��
	D3DXVECTOR3 m_Pos;         // ���W
	D3DXVECTOR3 m_Rot;         // ����
	D3DXMATRIX  m_MtxWorld;    // ���[���h�}�g���b�N�X
	CCollision  *m_pParent;    // �e�̃|�C���^
};

//*****************************************************************************
//    �{�b�N�X�R���C�_�[�N���X�̒�`
//*****************************************************************************
class CBoxCollider : public CCollision
{
public:    // �N�ł��A�N�Z�X�\
	CBoxCollider();
	~CBoxCollider();

	static CBoxCollider *Create(D3DXVECTOR3 pos, float fWidth, float fHeight, float fDepth, bool bReturnFlag, int nType);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand = NULL);

	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetDepth(const float fDepth);
	void SetReturnFlag(const bool bReturnFlag);

	float GetWidth(void);
	float GetHeight(void);
	float GetDepth(void);
	bool GetReturnFlag(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void MakeVertex(void);

	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
	}BOX_VERTEX;

	BOX_VERTEX              m_Vertex[8];    // ���̒��_���
	WORD                    m_wIndex[24];   // ���̕`��Ɏg�p����C���f�b�N�X���
	float                   m_fWidth;       // ���̉���
	float                   m_fHeight;      // ���̍���
	float                   m_fDepth;       // ���̉��s
	bool                    m_bReturnFlag;  // ���������ۂɖ߂����ǂ���
};

//*****************************************************************************
//    �T�[�N���R���C�_�[�N���X�̒�`
//*****************************************************************************
class CCircleCollider : public CCollision
{
public:    // �N�ł��A�N�Z�X�\
	CCircleCollider();
	~CCircleCollider();

	static CCircleCollider *Create(D3DXVECTOR3 pos, float fHeight, float fColRange, bool bReturnFlag, int nType);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand = NULL);

	void SetHeight(const float fHeight);
	void SetColRange(const float fColRange);
	void SetReturnFlag(const bool bReturnFlag);

	float GetHeight(void);
	float GetColRange(void);
	bool GetReturnFlag(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void MakeVertex(void);

	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
	}BOX_VERTEX;

	BOX_VERTEX              m_Vertex[8];    // �~���̒��_���
	WORD                    m_wIndex[24];   // �~���̕`��Ɏg�p����C���f�b�N�X���
	float                   m_fHeight;      // �~���̍���
	float                   m_fColRange;    // �~���̔��a
	bool                    m_bReturnFlag;  // ���������ۂɖ߂����ǂ���
};

//*****************************************************************************
//    �X�t�B�A�R���C�_�[�N���X�̒�`
//*****************************************************************************
class CSphereCollider : public CCollision
{
public:    // �N�ł��A�N�Z�X�\
	CSphereCollider();
	~CSphereCollider();

	static CSphereCollider *Create(D3DXVECTOR3 pos, float fColRange, bool bReturnFlag, int nType);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 ColRange, bool *pLand = NULL);

	void SetColRange(const float fColRange);
	void SetReturnFlag(const bool bReturnFlag);

	float GetColRange(void);
	bool GetReturnFlag(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void MakeVertex(void);

	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXCOLOR col;
	}BOX_VERTEX;

	BOX_VERTEX              m_Vertex[8];    // ���̒��_���
	WORD                    m_wIndex[24];   // ���̕`��Ɏg�p����C���f�b�N�X���
	float                   m_fColRange;    // ���̔��a
	bool                    m_bReturnFlag;  // ���������ۂɖ߂����ǂ���
};

#endif