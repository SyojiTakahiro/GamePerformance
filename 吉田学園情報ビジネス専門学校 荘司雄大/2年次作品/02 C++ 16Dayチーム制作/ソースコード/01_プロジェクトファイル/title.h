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
class CLogo;
class CLogoPress;
class CMap;
//*****************************************************************************
//    �^�C�g���N���X�̒�`
//*****************************************************************************
class CTitle : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	CTitle(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_TITLE);
	~CTitle();

	static CTitle *Create(void);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9 m_apTexture;	// �e�N�X�`���ւ̃|�C���^
	CLogo      *m_pLogo;        // �^�C�g�����S�N���X�ւ̃|�C���^
	CLogoPress *m_pLogoPress;   // PRESSENTER�N���X�ւ̃|�C���^
	int        m_nCounter;      // �J�ڂ��Ǘ�����J�E���^�[
	CMap	   *m_pMap;			// �}�b�v�N���X�ւ̃|�C���^
};

#endif