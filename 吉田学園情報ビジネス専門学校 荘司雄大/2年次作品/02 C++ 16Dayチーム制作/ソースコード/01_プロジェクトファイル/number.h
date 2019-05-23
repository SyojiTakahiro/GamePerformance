//*****************************************************************************
//
//     �����|���S���̏���[number.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _NUMBER_H_
#define _NUMBER_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene2DSquare.h"

//*****************************************************************************
//     �����|���S���N���X�̒�`
//*****************************************************************************
class CNumber : public CScene2DSquare
{
public:    // �N�ł��A�N�Z�X�\
	CNumber(int nPriority = 3,OBJTYPE objType = OBJTYPE_NUMBER);
	~CNumber();

	static CNumber *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nNumber);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetNumber(int nNumber);

private:   // ���̃N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9      m_pTexture;  // �e�N�X�`���ւ̃|�C���^
	int m_nNumber;   // �\�����鐔��
};
#endif