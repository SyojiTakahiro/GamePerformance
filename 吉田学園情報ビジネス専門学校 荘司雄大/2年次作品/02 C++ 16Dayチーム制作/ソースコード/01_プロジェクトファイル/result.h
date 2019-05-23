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
//    �}�N����`
//*****************************************************************************

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CScene2DSquare;
class CLogo;
class CLogoPress;
class CNumber;
class CMap;

//*****************************************************************************
//    ���U���g�N���X�̒�`
//*****************************************************************************
class CResult : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	typedef enum
	{
		LOGO_RESULT = 0,	// ���U���g���S
		LOGO_CLEAR_STAGE,	// �N���A�X�e�[�W��
		LOGO_STAGE_BONUS,	// �X�e�[�W�{�[�i�X
		LOGO_TOTAL,      	// ���v�X�R�A
		LOGO_MAX
	}RESULT_LOGO;

	typedef enum
	{
		NUMBER_TOTAL = 0,	       // ���v�X�R�A
		NUMBER_CLEAR_STAGE,	       // �N���A�X�e�[�W��
		NUMBER_CLEAR_STAGE_BONUS,  // �N���A�X�e�[�W���{�[�i�X�X�R�A
		NUMBER_STAGE_BONUS,	       // �X�e�[�W�{�[�i�X
		NUMBER_MAX
	}RESULT_NUMBER;

	CResult(int nPriority = 0, OBJTYPE objType = OBJTYPE_RESULT);
	~CResult();

	static CResult *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	const static void SetNumClearStage(int nNumClearStage);				// �N���A�X�e�[�W���̐ݒ�
	static int GetNumClearStage(void) { return m_nNumClearStage; };		// �N���A�X�e�[�W���̎擾

	const static void SetStageBonus(int StageBonus);					// �X�e�[�W�{�[�i�X�̐ݒ�
	static int GetStageBonus(void) { return m_nStageBonus; };			// �X�e�[�W�{�[�i�X�̎擾

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	static int m_nNumClearStage;				// �N���A�����X�e�[�W��
	static int m_nStageBonus;					// �N���A�����X�e�[�W�{�[�i�X

	static LPDIRECT3DTEXTURE9 m_apTexture[LOGO_MAX];	// �e�N�X�`���̃|�C���^�ϐ�
	static LPDIRECT3DTEXTURE9 m_apTexture2;	// �e�N�X�`���̃|�C���^�ϐ�

	CScene2DSquare *m_pMulti;                   // �|����|���S��
	CNumber **m_apNumber[NUMBER_MAX];			// �i���o�[�N���X�̃|�C���^�ϐ�
	CLogo *m_apLogo[LOGO_MAX];					// ���S�N���X�̃|�C���^�ϐ�
	CLogoPress *m_pLogoPress;					// �v���X���S�N���X�̃|�C���^�ϐ�
	int m_nScoreTotal;							// �X�R�A�̑���
	int m_nScoreNumMap;                         // �N���A�����}�b�v���ŉ��Z����X�R�A
	int m_nCounter;								// �J�ڂ��Ǘ�����J�E���^�[
	CMap *m_pMap;								// �}�b�v�N���X�̃|�C���^�ϐ�
	bool m_bChange;
};

#endif