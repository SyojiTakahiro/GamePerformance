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
#include "scene.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CScene2DSquare;
class CLogoPause;

//*****************************************************************************
//    �|�[�Y�N���X�̒�`
//*****************************************************************************
class CPause : CScene
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

	CPause(int nPriority = 0, OBJTYPE objType = OBJTYPE_PAUSE);
	~CPause();

	static CPause *Create(void);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetSelect(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9 m_pTexture;                 // �e�N�X�`���ւ̃|�C���^
	CLogoPause              *m_apLogoPause[SELECT_MAX];   // �|�[�Y���ڂ̎��
	CScene2DSquare          *m_pBlackPolygon;             // �|�[�Y�Ɏg�p���鍕���|���S��
	CScene2DSquare          *m_pPauseBg;                  // �|�[�Y�̔w�i
	int                     m_nSelect;                    // ���ݑI�����Ă��鍀��
	int                     m_nCounter;                   // �`����Ǘ�����J�E���^�[
};

#endif