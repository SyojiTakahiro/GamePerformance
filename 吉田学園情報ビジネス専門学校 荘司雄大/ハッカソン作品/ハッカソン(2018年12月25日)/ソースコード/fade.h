//*****************************************************************************
//
//     �t�F�[�h�̏���[fade.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _FADE_H_
#define _FADE_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "manager.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CScene2D;

//*****************************************************************************
//    �t�F�[�h�N���X�̒�`
//*****************************************************************************
class CFade
{
public:     // �N�ł��A�N�Z�X�\
	//------------------------
	//  �t�F�[�h�̎��
	//------------------------
	typedef enum
	{
		TYPE_MODECHANGE = 0,  // ��ʑJ�ڂɎg�p����
		TYPE_SWITCHMAP,       // �}�b�v�؂�ւ��Ɏg�p����
		TYPE_MAX
	}TYPE;

	//------------------------
	//  �t�F�[�h�̏��
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // �������Ă��Ȃ����
		STATE_IN,        // �t�F�[�h�C�����
		STATE_STOP,      // �t�F�[�h�����܂܎~�߂���
		STATE_OUT,       // �t�F�[�h�A�E�g���
		STATE_MAX
	}STATE;

	CFade();
	~CFade();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFade(CManager::MODE modeNext = CManager::MODE_TITLE);
	void SetFade(TYPE type = TYPE_SWITCHMAP);

	STATE GetState(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	void FadeIn(void);
	void FadeStop(void);
	void FadeOut(void);

	CScene2D       *m_pScene2D;           // �t�F�[�h�Ɏg�p����|���S��
	CManager::MODE m_NextMode;            // ���ɑJ�ڂ��郂�[�h
	STATE          m_State;               // ���
	TYPE           m_Type;                // ���
	int            m_nCounter;            // �t�F�[�h���ǂꂾ���~�߂邩
};

#endif