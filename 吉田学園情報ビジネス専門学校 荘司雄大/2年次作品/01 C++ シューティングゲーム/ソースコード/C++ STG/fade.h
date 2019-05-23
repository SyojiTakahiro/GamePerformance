//=============================================================================
//
// �t�F�[�h���� [fade.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "renderer.h"
#include "manager.h"

//*************************************
// �t�F�[�h�̏��
//*************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

class CFade
{
public:
	typedef enum
	{
		FADE_NONE = 0,		// �������Ă��Ȃ����
		FADE_IN,			// �t�F�[�h�C�����
		FADE_OUT,			// �t�F�[�h�A�E�g���
		FADE_MAX
	} FADE;


	CFade();
	~CFade();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetFade(CManager::MODE modeNext);
	static FADE GetFade(void);
	static CFade *Create(void);
private:
	LPDIRECT3DTEXTURE9		m_pTexture = NULL;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
	static FADE				m_fade;					// �t�F�[�h���
	CManager::MODE			m_modeNext;				// ���̃��[�h
	D3DXCOLOR				m_colorFade;			// �t�F�[�h�F
};
#endif
