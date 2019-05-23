//*****************************************************************************
//
//     �v���[���g�p�r���{�[�h�̏���[Billpresent.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _BILLPRESENT_H_
#define _BILLPRESENT_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    �v���[���g�p�r���{�[�h�N���X�̒�`
//*****************************************************************************
class CBillPresent : public CSceneBillboard
{
public:    // �N�ł��A�N�Z�X�\
	CBillPresent(int nPriority = 3, OBJTYPE objType = OBJTYPE_BILLPRESENT);
	~CBillPresent();

	static CBillPresent *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fWidth, float fHeight, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Collision(D3DXVECTOR3 pos);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};

#endif