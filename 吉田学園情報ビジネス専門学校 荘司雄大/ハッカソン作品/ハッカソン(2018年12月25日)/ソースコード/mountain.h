//*****************************************************************************
//
//     �R�̏���[mountain.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MOUNTAIN_H_
#define _MOUNTAIN_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "meshCylinder.h"

//*****************************************************************************
//    �R�N���X�̒�`
//*****************************************************************************
class CMountain : public CMeshCylinder
{
public:    // �N�ł��A�N�Z�X�\
	CMountain(int nPriority = 3, OBJTYPE objType = OBJTYPE_MOUNTAIN);
	~CMountain();
	static CMountain *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, int nXBlock, int nYBlock);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeNormal(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};

#endif