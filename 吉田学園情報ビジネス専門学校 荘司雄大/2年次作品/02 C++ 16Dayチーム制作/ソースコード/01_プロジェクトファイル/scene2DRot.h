//*****************************************************************************
//
//     �V�[��(2D)��]�̏���[scene2DRot.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE2DROT_H_
#define _SCENE2DROT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
//    �V�[��2D��]�N���X�̒�`
//*****************************************************************************
class CScene2DRot : public CScene2D
{
public:     // �N�ł��A�N�Z�X�\
	CScene2DRot(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CScene2DRot();

	static CScene2DRot *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, float fRotation, int nPriority);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, float fRotation);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetVtxBuffPos(D3DXVECTOR3 pos);
	void SetVtxBuffCol(D3DXCOLOR col);
	void SetScale(const float fScale);
	void SetRotation(const float fRotation);

	float GetScale(void);
	float GetRotation(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	float m_fRotation; // ����
	float m_fScale;    // �|���S���̑傫��
};

#endif