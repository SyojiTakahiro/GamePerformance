//*****************************************************************************
//
//     �|�[�Y�̏���[pause.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PAUSE_H_
#define _PAUSE_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CScene2D;
class CScene2DFlash;

//*****************************************************************************
//    �|�[�Y�N���X�̒�`
//*****************************************************************************
class CPause
{
public:    // �N�ł��A�N�Z�X�\
	//------------------------
	//  �|�[�Y���ڂ̎��
	//------------------------
	typedef enum
	{
		SELECT_CONTINUE = 0, // �ĊJ
		SELECT_RETRY,        // ��蒼��
		SELECT_QUIT,         // �I��
		SELECT_MAX
	}SELECT;

	CPause();
	~CPause();

	static CPause *Create(int nPriority = 3);

	HRESULT Init(int nPriority);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CScene2DFlash *GetScene2DFlash(int nIdx);
	CScene2D *GetPauseBg(void);
	int GetSelect(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CScene2DFlash             *m_apLogoPause[SELECT_MAX];     // �|�[�Y���ڂ̎��
	CScene2D                  *m_pBlackBg;                    // �|�[�Y�w�i�p(�����|���S��)
	CScene2D                  *m_pPauseBg;                    // �|�[�Y�̔w�i
	int                       m_nSelect;                      // ���ݑI�����Ă��鍀��
	int                       m_nCounter;                     // �`����Ǘ�����J�E���^�[
};

#endif