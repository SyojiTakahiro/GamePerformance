//*****************************************************************************
//
//     �`���[�g���A���̏���[tutorial.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CScene2D;
class CScene2DPress;
class CTextureManager;
class CMap;

//*****************************************************************************
//    �`���[�g���A���N���X�̒�`
//*****************************************************************************
class CTutorial : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	//--------------------
	//  �e�N�X�`���̔ԍ�
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // PressEnter�p
		TEX_NUMBER_1,  // PressEnter�p
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CTutorial(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_TUTORIAL);
	~CTutorial();

	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	CScene2D        *m_pScene;
	int             m_nCounter;           // �J�ڂ��Ǘ�����J�E���^�[
	CScene2DPress   *m_pPress;            // �v���X�G���^�[�p��2D�|���S���N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;   // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
};

#endif