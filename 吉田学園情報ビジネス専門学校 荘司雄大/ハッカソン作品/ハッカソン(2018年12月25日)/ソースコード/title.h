//*****************************************************************************
//
//     �^�C�g���̏���[title.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

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
//    �^�C�g���N���X�̒�`
//*****************************************************************************
class CTitle : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	//--------------------
	//  �e�N�X�`���̔ԍ�
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // PressEnter�p
		TEX_NUMBER_1,      // �^�C�g�����S�p
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CTitle(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_TITLE);
	~CTitle();

	static CTitle *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	CMap            *m_pMap;              // �}�b�v�N���X�ւ̃|�C���^
	int             m_nCounter;           // �J�ڂ��Ǘ�����J�E���^�[
	CScene2D        *m_pTitleLogo;        // �^�C�g�����S�p��2D�|���S���N���X�ւ̃|�C���^
	CScene2DPress   *m_pPress;            // �v���X�G���^�[�p��2D�|���S���N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;   // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
};

#endif