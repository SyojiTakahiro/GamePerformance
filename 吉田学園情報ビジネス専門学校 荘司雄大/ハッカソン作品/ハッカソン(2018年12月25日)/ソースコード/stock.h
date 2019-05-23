//*****************************************************************************
//
//     �c�@�̏���[stock.h]
//     Auther:Takuto Ishida
//
//*****************************************************************************
#ifndef _STOCK_H_
#define _STOCK_H_

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
class CStock : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CStock(int nPriority = 5, OBJTYPE objType = OBJTYPE_STOCK);
	~CStock();

	static CStock *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nStock, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetStockTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetStock(const int nStock);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetStock(void);

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DTEXTURE9 m_pTexture;         // �����e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3        m_Pos;              // �c�@�̍��W
	D3DXCOLOR          m_Col;              // �c�@�̐F
	float              m_fWidth;           // �c�@�̐����|���S���P���̕�
	float              m_fHeight;          // �c�@�̐����|���S���P���̍���
	CNumber            **m_apNumber;       // �����|���S���N���X�^�̃|�C���^
	int                m_nDigit;           // �c�@�̌���
	int                m_nStock;           // �c�@
	CScene2D		   *m_pFrame;		   // �g
	CTextureManager	   *m_pTextureManager; // �e�N�X�`���Ǘ��N���X�̃|�C���^
};

#endif