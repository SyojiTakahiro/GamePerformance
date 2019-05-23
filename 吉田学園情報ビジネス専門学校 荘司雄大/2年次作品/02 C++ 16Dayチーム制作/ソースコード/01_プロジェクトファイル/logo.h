//*****************************************************************************
//
//     ���S�̏���[logo.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _LOGO_H_
#define _LOGO_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene2DRot.h"

//*****************************************************************************
//     ���S�N���X�̒�`
//*****************************************************************************
class CLogo : public CScene2DRot
{
public:     // �N�ł��A�N�Z�X�\
	CLogo(int nPriority = 3,OBJTYPE objType = OBJTYPE_LOGO);
	~CLogo();
	static CLogo *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, int nPriority);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:    // ���̃N���X�������A�N�Z�X�\
};

//*****************************************************************************
//     �Z���N�g���S�N���X�̒�`
//*****************************************************************************
class CLogoSelect : public CScene2DRot
{
public:    // �N�ł��A�N�Z�X�\
	//------------------------
	//  �Z���N�g���ڂ̎��
	//------------------------
	typedef enum
	{
		TYPE_GAME = 0,  // �Q�[�����
		TYPE_RANKING,   // �����L���O���
		TYPE_TITLE,     // �^�C�g�����
		TYPE_MAX
	}TYPE;

	//------------------------
	//  �Z���N�g���ڂ̏��
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // �I������Ă��Ȃ����
		STATE_SELECT,    // �I������Ă�����
		STATE_PUSH,      // ���肪�����ꂽ���
		STATE_MAX
	}STATE;

	CLogoSelect(int nPriority = 3);
	~CLogoSelect();

	static CLogoSelect *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state, int nPriority);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(TYPE type);
	void SetState(STATE state);

	void ResetTexU(void);

	TYPE GetType(void);
	STATE GetState(void);

private:   // ���̃N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9  m_apTexture[TYPE_MAX];  // �e�N�X�`���ւ̃|�C���^
	static float               m_fColTrans;            // �F�̕ω��Ɏg�p
	TYPE  m_Type;          // ���
	STATE m_State;         // ���
	int   m_nPatternAnimU; // ���݂̃e�N�X�`��U���W
	int   m_nCounterAnim;  // �A�j���[�V�����J�E���^�[
};

//*****************************************************************************
//     �|�[�Y���S�N���X�̒�`
//*****************************************************************************
class CLogoPause : public CScene2DRot
{
public:   // �N�ł��A�N�Z�X�\
	//------------------------
	//  �|�[�Y���ڂ̎��
	//------------------------
	typedef enum
	{
		TYPE_CONTINUE = 0,  // �ĊJ
		TYPE_RETRY,         // ��蒼��
		TYPE_QUIT,          // �I��
		TYPE_MAX
	}TYPE;

	//------------------------
	//  �|�[�Y���ڂ̏��
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // �I������Ă��Ȃ����
		STATE_SELECT,    // �I������Ă�����
		STATE_PUSH,      // ���肪�����ꂽ���
		STATE_MAX
	}STATE;

	CLogoPause(int nPriority = 3);
	~CLogoPause();

	static CLogoPause *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state, int nPriority);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(TYPE type);
	void SetState(STATE state);

	TYPE GetType(void);
	STATE GetState(void);

private:   // ���̃N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9  m_apTexture[TYPE_MAX];  // �e�N�X�`���ւ̃|�C���^
	static float               m_fColTrans;            // �F�̕ω��Ɏg�p
	TYPE  m_Type;      // ���
	STATE m_State;     // ���
};

//*****************************************************************************
//     �v���X���S�N���X�̒�`
//*****************************************************************************
class CLogoPress : public CScene2DRot
{
public:   // �N�ł��A�N�Z�X�\
	//------------------------
    //  PRESS�̏��
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // �Ȃɂ�����Ă��Ȃ����
		STATE_PUSH,      // ���肪�����ꂽ���
		STATE_MAX
	}STATE;

	CLogoPress(int nPriority = 3);
	~CLogoPress();

	static CLogoPress *Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, int nPriority);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetScale(float fScale);
	void SetState(STATE state);

	float GetScale(void);
	STATE GetState(void);

private:   // ���̃N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9  m_pTexture;  // �e�N�X�`���ւ̃|�C���^
	STATE m_State;     // ���
	int   m_nCounter;  // �`����Ǘ�����J�E���^�[
};

#endif