//*****************************************************************************
//
//     ���U���g�̏���[result.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RESULT_H_
#define _RESULT_H_

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

//*****************************************************************************
//    ���U���g�N���X�̒�`
//*****************************************************************************
class CResult : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	//--------------------
	//  RESULT
	//--------------------
	typedef enum
	{
		RESULT_GAMEOVER = 0,
		RESULT_CLEAR,
		RESULT_MAX
	}RESULT;

	//--------------------
	//  �e�N�X�`���̔ԍ�
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // PressEnter�p
		TEX_NUMBER_1,      // PressEnter�p
		TEX_NUMBER_2,      // PressEnter�p
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CResult(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_RESULT);
	~CResult();

	static CResult *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetResult(RESULT result);
	static RESULT GetResult(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	static RESULT m_Result;                 // �N���A�������ǂ���
	CScene2D          *m_pScene;
	int             m_nCounter;             // �J�ڂ��Ǘ�����J�E���^�[
	CScene2DPress   *m_pPress;              // �v���X�G���^�[�p��2D�|���S���N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;     // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
};

#endif