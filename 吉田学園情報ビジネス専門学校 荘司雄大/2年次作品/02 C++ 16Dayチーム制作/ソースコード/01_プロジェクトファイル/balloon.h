//=============================================================================
//
// �����o������ [balloon.cpp]
// Author : Atsushi Yamashita
//
//=============================================================================
#ifndef _BALLOON_H_
#define _BALLOON_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene2DSquare.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CScene2DSquare;

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MAX_BALLOON_TEXTURE (5)
#define BALLOON_PRIORITY	(4)

//*****************************************************************************
//    �����o���N���X�̒�`
//*****************************************************************************
class CBalloon : public CScene2DSquare
{
public:
	CBalloon(int nPriority = BALLOON_PRIORITY, OBJTYPE objType = OBJTYPE_BALLOON);
	~CBalloon();

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CBalloon *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight,int nTexNumber);
	static HRESULT Load(void);										//�e�N�X�`���ǂݍ���
	static void Unload(void);

	 void SetType(int nType);
private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	static LPDIRECT3DTEXTURE9		m_pTexture[MAX_BALLOON_TEXTURE];

	D3DXVECTOR3 m_pos;			// ���_���W
	D3DXVECTOR3 m_move;			//�@���x�N�g��
	D3DXCOLOR   m_col;
	float m_fCol;
	D3DXMATRIX m_mtxWorld;		// ���_�J���[
	int m_nTexNumber;			//�e�N�X�`���̔ԍ�
	int m_nType;
	int m_nCounter;

};
#endif
