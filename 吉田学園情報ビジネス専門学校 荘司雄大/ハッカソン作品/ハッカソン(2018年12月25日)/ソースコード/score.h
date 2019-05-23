//*****************************************************************************
//
//     �X�R�A�̏���[score.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCORE_H_
#define _SCORE_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define  SCORE_NUMBER_MAX  (8)    // �X�R�A�̌���

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CNumber;
class CTextureManager;
class CScene2D;

//*****************************************************************************
//     �X�R�A�N���X�̒�`
//*****************************************************************************
class CScore : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CScore(int nPriority = 3, OBJTYPE objType = OBJTYPE_SCORE);
	~CScore();

	static CScore *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetScoreTexture(LPDIRECT3DTEXTURE9 pTexture);
	void AddScore(int nScore);

	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetScore(const int nScore);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetScore(void);

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DTEXTURE9 m_pTexture;                     // �����e�N�X�`���ւ̃|�C���^
	CNumber            *m_apNumber[SCORE_NUMBER_MAX];  // �����|���S���N���X�^�̃|�C���^
	D3DXVECTOR3        m_Pos;                          // �X�R�A�̍��W
	D3DXCOLOR          m_Col;                          // �X�R�A�̐F
	float              m_fWidth;                       // �X�R�A�̐����|���S���P���̍���
	float              m_fHeight;                      // �X�R�A�̐����|���S���P���̍���
	int                m_nScore;                       // �X�R�A
	CScene2D		   *m_pFrame;					   // �g
	CTextureManager	   *m_pTextureManager;			   // �e�N�X�`���Ǘ��N���X�̃|�C���^
};

#endif