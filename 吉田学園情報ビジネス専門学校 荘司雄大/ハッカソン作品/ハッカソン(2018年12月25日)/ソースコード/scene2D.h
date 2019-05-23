//*****************************************************************************
//
//     2D�|���S���̏���[scene2D.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    2D�|���S���N���X�̒�`
//*****************************************************************************
class CScene2D : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	CScene2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CScene2D();

	static CScene2D *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);
	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetRot(const float fRot);
	void SetWidth(const float fWidth);
	void SetHeight(const float fHeight);
	void SetAngle(const float fAngle);
	void SetLength(const float fLength);

	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);
	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	float GetRot(void);
	float GetWidth(void);
	float GetHeight(void);
	float GetAngle(void);
	float GetLength(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;   // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	  // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3				m_Pos;	      // �|���S���̈ʒu
	D3DXCOLOR               m_Col;        // �|���S���̐F
	float                   m_fRot;       // �|���S���̌���
	float                   m_fAngle;     // �|���S�����o���p�x
	float                   m_fWidth;     // �|���S���̕�
	float                   m_fHeight;    // �|���S���̍���
	float                   m_fLength;    // �|���S���̒���
};

//*****************************************************************************
//    2D�|���S���_�ŃN���X�̒�`
//*****************************************************************************
class CScene2DFlash : public CScene2D
{
public:     // �N�ł��A�N�Z�X�\
	typedef enum
	{
		STATE_NONE = 0,  // �ʏ���
		STATE_SELECT,    // �I�����ꂽ���
		STATE_PUSH,      // �����ꂽ���
		STATE_MAX,
	}STATE;

	CScene2DFlash(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CScene2DFlash();

	static CScene2DFlash *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, STATE State = STATE_NONE,
		D3DXCOLOR ColMax = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR ColMin = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f),
		float fColChange = 0.01f, int nPushTiming = 4, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, STATE State = STATE_NONE,
		D3DXCOLOR ColMax = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR ColMin = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f),
		float fColChange = 0.01f, int nPushTiming = 4);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetState(const STATE State);
	void SetColMax(const D3DXCOLOR ColMax);
	void SetColMin(const D3DXCOLOR ColMin);
	void SetColChange(const float fColChange);

	STATE GetState(void);
	D3DXCOLOR GetColMax(void);
	D3DXCOLOR GetColMin(void);
	float GetColChange(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Select(void);

	STATE     m_State;        // ���
	D3DXCOLOR m_ColMax;       // �F�̍ő�l
	D3DXCOLOR m_ColMin;       // �F�̍ŏ��l
	float     m_fColChange;   // �F�̕ω���
	int       m_nPushTiming;  // �`���؂�ւ���^�C�~���O(�����ꂽ��Ԃ̎�)
	int       m_nCounter;     // �`����Ǘ�����J�E���^�[
};

//*****************************************************************************
//    2D�|���S���v���X�N���X�̒�`
//*****************************************************************************
class CScene2DPress : public CScene2D
{
public:     // �N�ł��A�N�Z�X�\
	typedef enum
	{
		STATE_NONE = 0,  // �ʏ���
		STATE_PUSH,      // �����ꂽ���
		STATE_MAX,
	}STATE;

	CScene2DPress(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CScene2DPress();

	static CScene2DPress *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f,
		int nStopTiming = 80, int nAppearTiming = 100, int nPushTiming = 4, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f,
		int nStopTiming = 80, int nAppearTiming = 100, int nPushTiming = 4);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void FlashStart(void);

	void SetState(const STATE State);
	void SetAppearTiming(const int nAppearTiming);
	void SetStopTiming(const int nStopTiming);
	void SetCounter(const int nCounter);

	STATE GetState(void);
	int GetAppearTiming(void);
	int GetStopTiming(void);
	int GetCounter(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void None(void);

	STATE m_State;          // ���
	int   m_nStopTiming;    // �`����ꎞ��~����^�C�~���O
	int   m_nAppearTiming;  // �`����ĊJ����^�C�~���O
	int   m_nCounter;       // �`����Ǘ�����J�E���^�[
	int   m_nPushTiming;    // �`���؂�ւ���^�C�~���O(�����ꂽ��Ԃ̎�)
};

//*****************************************************************************
//    �����N���X�̒�`
//*****************************************************************************
class CNumber : public CScene2D
{
public:     // �N�ł��A�N�Z�X�\
	typedef enum
	{
		STATE_NONE = 0,  // �ʏ���
		STATE_FLASH,     // �_�ŕ\����������
		STATE_MAX
	}STATE;

	CNumber(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CNumber();

	static CNumber *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, STATE State = STATE_NONE,
		int nNumber = 0, int nFlashTiming = 3, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, float fRot = 0.0f, STATE State = STATE_NONE,
		int nNumber = 0, int nFlashTiming = 3);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetNumber(const int nNumber);
	void SetState(const STATE State);

	int GetNumber(void);
	STATE GetState(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Flash(void);

	STATE m_State;         // ���
	int   m_nNumber;       // �\�����鐔��
	int   m_nFlashTiming;  // �_�ł�����^�C�~���O
	int   m_nCounter;      // �`����Ǘ�����J�E���^�[
};

#endif