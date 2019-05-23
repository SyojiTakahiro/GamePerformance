//*****************************************************************************
//
//     �e�N�X�`���}�l�[�W�����g�̏���[texturManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************

//*****************************************************************************
//    �O���錾
//*****************************************************************************

//*****************************************************************************
//    �e�N�X�`���Ǌ��N���X�̒�`
//*****************************************************************************
class CTextureManager
{
public:     // �N�ł��A�N�Z�X�\
	CTextureManager();
	~CTextureManager();

	static CTextureManager *Create(int nNumTex);
	HRESULT Init(void);
	void Uninit(void);

	void SetNumTexture(int nNumTex);
	void SetFileName(char *pFileName, int nIdx);

	int GetNumTexture(void);
	char *GetFileName(int nIdx);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture, int nIdx);

	LPDIRECT3DTEXTURE9 GetTexture(int nIdx);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	void ReleaseTexture(void);

	char               **m_pFileName;   // �e�N�X�`���̃t�@�C���p�X��
	int                m_nNumTexture;   // �ǂݍ��񂾃e�N�X�`���̐�
	LPDIRECT3DTEXTURE9 *m_apTexture;    // �e�N�X�`���ւ̃|�C���^
};

#endif