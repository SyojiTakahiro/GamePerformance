//*****************************************************************************
//
//     UI�Ǘ��̏���[ui.h]
//     Auther:Takuto Ishida
//
//*****************************************************************************
#ifndef _UI_H_
#define _UI_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define UI_TIMER_PRIORITY	(5)		// �^�C�}�[�̏����D�揇��
#define UI_SCORE_PRIORITY	(5)		// �X�R�A�̏����D�揇��
#define UI_STOCK_PRIORITY	(5)		// �c�@�̏����D�揇��
#define UI_PRESENT_PRIORITY	(5)		// �v���[���g�̏����D�揇��

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CTimer;
class CScore;
class CStock;
class CPresent;
class CTextureManager;

//*****************************************************************************
//    �`���[�g���A���N���X�̒�`
//*****************************************************************************
class CUI
{
public:     // �N�ł��A�N�Z�X�\
	CUI();
	~CUI();

	static CUI *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	int GetTimer(void);			// �^�C�}�[���擾
	void SetTimer(int nTimer);	// �^�C�}�[�̐ݒ�

	void ResetTimer(void);		// �^�C�}�[�����Z�b�g

	int GetScore(void);			// �X�R�A���擾
	void SetScore(int nScore) ;	// �X�R�A�̐ݒ�

	int GetStock(void);			// �c�@���擾
	void SetStock(int nScore);	// �c�@�̐ݒ�

	int GetPresent(void);			// �v���[���g���擾
	void SetPresent(int nScore);	// �v���[���g�̐ݒ�

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	CTextureManager *m_pTextureManager;		// �e�N�X�`���Ǘ��N���X�̃|�C���^
	CTimer *m_pTimer;						// �^�C�}�[�N���X�̃|�C���^
	CScore *m_pScore;						// �X�R�A�N���X�̃|�C���^
	CStock *m_pStock;						// �c�@�N���X�̃|�C���^
	CPresent *m_pPresent;					// �v���[���g�N���X�̃|�C���^
};

#endif