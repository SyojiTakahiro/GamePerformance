//*****************************************************************************
//
//     �^�C�}�[�̏���[timer.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _TIMER_H_
#define _TIMER_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CNumber;
class CScene2D;
class CTextureManager;

//*****************************************************************************
//     �^�C�}�[�N���X�̒�`
//*****************************************************************************
class CTimer : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CTimer(int nPriority = 3, OBJTYPE objType = OBJTYPE_TIMER);
	~CTimer();

	static CTimer *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, int nTimer, int nWarningTime, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetTimerTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetTimer(const int nTimer);
	void SetWarningTime(const int nWarningTime);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetWidth(void);
	float GetHeight(void);
	int GetTimer(void);
	int GetWarningTime(void);

	void Reset(void) { m_nTimer = m_nInitTimer; };
	static void SetInitTimer(int nInitTimer) { m_nInitTimer = nInitTimer; };

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DTEXTURE9 m_pTexture;         // �����e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3        m_Pos;              // �^�C�}�[�̍��W
	D3DXCOLOR          m_Col;              // �^�C�}�[�̐F
	float              m_fWidth;           // �^�C�}�[�̐����|���S���P���̕�
	float              m_fHeight;          // �^�C�}�[�̐����|���S���P���̕�
	CNumber            **m_apNumber;       // �����|���S���N���X�^�̃|�C���^
	int                m_nDigit;           // �^�C�}�[�̌���
	int                m_nTimer;           // �^�C�}�[
	int                m_nWarningTime;     // �^�C�}�[���Ԃ��Ȃ�^�C�~���O
	static int		   m_nInitTimer;	   // �����^�C�}�[
	CScene2D		   *m_pFrame;		   // �g
	CTextureManager	   *m_pTextureManager; // �e�N�X�`���Ǘ��N���X�̃|�C���^
};

#endif