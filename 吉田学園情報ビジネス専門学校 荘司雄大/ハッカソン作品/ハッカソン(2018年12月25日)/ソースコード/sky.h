//*****************************************************************************
//
//     ��̏���[sky.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SKY_H_
#define _SKY_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "meshDome.h"

//*****************************************************************************
//    ��N���X�̒�`
//*****************************************************************************
class CSky : public CMeshDome
{
public:    // �N�ł��A�N�Z�X�\
	CSky(int nPriority = 3, OBJTYPE objType = OBJTYPE_SKY);
	~CSky();
	static CSky *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, float fRotSpeed, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nXBlock, int nYBlock, float fRotSpeed);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeNormal(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	float m_fRotSpeed;     // ��]����X�s�[�h
};

#endif