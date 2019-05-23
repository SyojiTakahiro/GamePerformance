//*****************************************************************************
//
//     �O�ՃG�t�F�N�g�̏���[orbitEffect.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _ORBITEFFECT_H_
#define _ORBITEFFECT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "meshOrbit.h"

//*****************************************************************************
//    �O�ՃN���X�̒�`
//*****************************************************************************
class COrbitEffect : public CMeshOrbit
{
public:    // �N�ł��A�N�Z�X�\
	COrbitEffect(int nPriority = 3, OBJTYPE objType = OBJTYPE_ORBITEFFECT);
	~COrbitEffect();

	static COrbitEffect *Create(D3DXVECTOR3 OffsetPos1, D3DXVECTOR3 OffsetPos2, D3DXVECTOR3 OffsetAmp1, D3DXVECTOR3 OffsetAmp2,
		D3DXCOLOR ColUp, D3DXCOLOR ColDown,float fAlphaDecayUp, float fAlphaDecayDown, int nXBlock, int nZBlock, D3DXMATRIX *pMtxParent, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetColUp(const D3DXCOLOR ColUp);
	void SetColDown(const D3DXCOLOR ColDown);
	void SetAlphaDecayUp(const float fAlphaDecayUp);
	void SetAlphaDecayDown(const float fAlphaDecayDown);
	void SetOffsetAmp1(const D3DXVECTOR3 OffsetAmp);
	void SetOffsetAmp2(const D3DXVECTOR3 OffsetAmp);

	D3DXCOLOR GetColUp(void);
	D3DXCOLOR GetColDown(void);
	float GetAlphaDecayUp(void);
	float GetAlphaDecayDown(void);
	D3DXVECTOR3 GetOffsetAmp1(void);
	D3DXVECTOR3 GetOffsetAmp2(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXCOLOR   m_ColUp;            // ���_�㑤�̐F
	D3DXCOLOR   m_ColDown;          // ���_�����̐F
	float       m_fAlphaDecayUp;    // ���_�㑤�����x�̌����l
	float       m_fAlphaDecayDown;  // ���_���������x�̌����l
	D3DXVECTOR3 m_OffsetAmp[2];     // ���_�I�t�Z�b�g�̑����l
};

#endif