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
//    �}�N����`
//*****************************************************************************
#define MAX_RANKING  (5)    // �����L���O�X�R�A�̐�

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CMap;
class CNumber;
class CScene2D;
class CTextureManager;

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

	//--------------------
	//  �e�N�X�`���̔ԍ�
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // �����p
		TEX_NUMBER_1,      // ���ʗp
		TEX_NUMBER_2,      // �����L���O���S�p
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CRanking(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_RANKING);
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
	static int      m_nRankingScore[MAX_RANKING];   // �����L���O�X�R�A
	static STATE    m_State[MAX_RANKING];           // �����L���O�X�R�A�̏��
	CMap            *m_pMap;                        // �}�b�v�N���X�ւ̃|�C���^
	CScene2D        *m_pRankingLogo;                // �����L���O���S�\���p�̃|���S��
	CScene2D        *m_apRankingRank[MAX_RANKING];  // �����L���O���ʕ\���p�̃|���S��
	CNumber         **m_apNumber[MAX_RANKING];      // �����L���O�X�R�A�\���p�̃|���S��
	int             m_aDigit[MAX_RANKING];          // �����L���O�X�R�A�̌���
	int             m_nCounter;                     // �J�ڂ��Ǘ�����J�E���^�[
	D3DXVECTOR3     m_Move[MAX_RANKING];            // �����L���O�X�R�A�̈ړ���
	CTextureManager *m_pTextureManager;             // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
};

#endif