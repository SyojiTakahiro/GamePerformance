//*****************************************************************************
//
//     �����L���O�̏���[ranking.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RANKING_H_
#define _RANKING_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CNumber;
class CLogo;
class CScene2DSquare;
class CMap;

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MAX_RANKING             (5)     // �����L���O�̃X�R�A��
#define MAX_RANKING_TEXTURE		(2)		// �����L���O�̃e�N�X�`����

//*****************************************************************************
//    �����L���O�N���X�̒�`
//*****************************************************************************
class CRanking : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	//------------------------
	//  �����L���O�X�R�A�̏��
	//------------------------
	typedef enum
	{
		STATE_NONE = 0,  // �����Ȃ����
		STATE_UPDATE,    // �X�V���ꂽ���
		STATE_MAX
	}STATE;

	CRanking(int nPriority = 3, OBJTYPE objType = OBJTYPE_RANKING);
	~CRanking();

	static CRanking *Create(void);

	static void RankingScoreInit(void);
	static void SetRankingScore(int nScore);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	static int   m_nRankingScore[MAX_RANKING];   // �����L���O�X�R�A
	static STATE m_State[MAX_RANKING];           // �X�R�A�̏��
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_RANKING_TEXTURE];		// �����L���O���ʗp�̃e�N�X�`���ւ̃|�C���^
	CScene2DSquare *m_apSceneSquare[MAX_RANKING];					// �����L���O���ʗp�|���S��
	CNumber     **m_apNumber[MAX_RANKING];							// �����L���O�X�R�A�̐����|���S��
	CLogo       *m_pLogo;											// �����L���O���S�N���X�^�̃|�C���^
	int         m_nCounter;											// ��ʑJ�ڂ��Ǘ�����J�E���^�[
	static int	m_nScore;											// �ŐV�̊l�������X�R�A
	bool		m_bFlash;											// �_�ŗp�̔���
	int			m_aScoreDigits[MAX_RANKING];						// �X�R�A�̌���
	CMap		*m_pMap;											// �}�b�v�N���X�̃|�C���^�ϐ�
};

#endif