//*****************************************************************************
//
//     �v���[���g�̏���[present.h]
//     Auther:Takuto Ishida
//
//*****************************************************************************
#ifndef _PRESENT_H_
#define _PRESENT_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CNumber;
class CTextureManager;
class CScene2D;

//*****************************************************************************
//     �^�C�}�[�N���X�̒�`
//*****************************************************************************
class CPresent : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CPresent(int nPriority = 5, OBJTYPE objType = OBJTYPE_PRESENT);
	~CPresent();

	static CPresent *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nStock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPresentTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetPresent(const int nStock);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetPresent(void);

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DTEXTURE9 m_pTexture;         // �����e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3        m_Pos;              // �v���[���g�̍��W
	D3DXCOLOR          m_Col;              // �v���[���g�̐F
	float              m_fWidth;           // �v���[���g�̐����|���S���P���̕�
	float              m_fHeight;          // �v���[���g�̐����|���S���P���̍���
	CNumber            **m_apNumber;       // �����|���S���N���X�^�̃|�C���^
	int                m_nDigit;           // �v���[���g�̌���
	int                m_nPresent;         // �v���[���g
	CScene2D		   *m_pFrame;		   // �g
	CTextureManager	   *m_pTextureManager; // �e�N�X�`���Ǘ��N���X�̃|�C���^
};

#endif