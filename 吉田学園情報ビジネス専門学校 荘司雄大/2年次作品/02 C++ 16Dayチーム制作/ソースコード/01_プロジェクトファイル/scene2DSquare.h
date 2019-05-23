//*****************************************************************************
//
//     �V�[��(2D)�X�N�G�A�̏���[scene2DSuqare.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE2DSQUARE_H_
#define _SCENE2DSQUARE_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene2D.h"

//*****************************************************************************
//    �V�[��2D�X�N�G�A�N���X�̒�`
//*****************************************************************************
class CScene2DSquare : public CScene2D
{
public:    // �N�ł��A�N�Z�X�\
	CScene2DSquare(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CScene2DSquare();
	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetVtxBuffPos(const D3DXVECTOR3 pos);
	void SetVtxBuffCol(const D3DXCOLOR col);

	float GetHeight(void);
	float GetWidth(void);

	static CScene2DSquare *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	float m_fWidth;   // ��
	float m_fHeight;  // ����
};

#endif