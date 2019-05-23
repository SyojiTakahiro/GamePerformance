//=============================================================================
//
// �|�[�Y���� [pause.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

#define PAUSE_TEXTURE_MAX  (4)          // �e�N�X�`����ǂݍ��ސ�

//*****************************************************************************
// 2D�|���S���N���X
//*****************************************************************************
class CPause : public CScene
{
public:
	typedef enum
	{
		SELECT_CONTINUE = 0,
		SELECT_RETRY,
		SELECT_QUIT,
		SELECT_MAX
	}SELECT;

	typedef enum
	{
		SELECTTYPE_NONE = 0,	//�I�΂�Ă��Ȃ����
		SELECTTYPE_SELECT,		//�I�΂�Ă���	���
		SELECTTYPE_MAX				//��Ԃ̑���
	}SELECTTYPE;

	typedef struct
	{
		SELECTTYPE	select;		//�Z���N�g
		D3DXVECTOR3 pos;		//�ʒu
		D3DXCOLOR	col;		//�J���[
	}PAUSESELECT;

	CPause();
	~CPause();
	static HRESULT Load(void);	//�e�N�X�`���̓ǂݍ���
	static void Unload(void);	//�e�N�X�`���̊J��

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPause *Create(void);
private:
	static LPDIRECT3DTEXTURE9	m_apTexture[PAUSE_TEXTURE_MAX];	//���L�e�N�X�`���ւ̃|�C���^

	CScene2D *m_pCScene2D;
	CScene2D *m_pPauseSelect[SELECT_MAX];           // ���S�̃|���S��

	PAUSESELECT				m_aPauseMenu[SELECT_MAX];					// �|�[�Y���j���[
	SELECT					m_SelectMode;
	int						m_nSelect;
};

#endif