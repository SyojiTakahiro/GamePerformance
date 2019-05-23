//*****************************************************************************
//
//     �Q�[���̏���[game.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define GAME_TIMER_PRIORITY    (5)    // �^�C�}�[�̏����D�揇��
#define GAME_SCORE_PRIORITY    (5)    // �X�R�A�̏����D�揇��

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CPause;
class CTextureManager;
class CMap;
class CTimer;
class CScore;
class CUI;

//*****************************************************************************
//    �Q�[���N���X�̒�`
//*****************************************************************************
class CGame : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	//-----------------
	//  ���
	//-----------------
	typedef enum
	{
		STATE_NORMAL = 0,   // �ʏ���
		STATE_PLAYER_DEATH, // �v���C���[������ł�����
		STATE_MAP_CLEAR,    // �}�b�v���N���A�������
		STATE_MAPCHANGE,    // �}�b�v��؂�ւ��Ă�����
		STATE_END,
		STATE_MAX
	}STATE;

	//--------------------
	//  �e�N�X�`���̔ԍ�
	//--------------------
	typedef enum
	{
		TEX_NUMBER_0 = 0,  // �|�[�Y�w�i�p
		TEX_NUMBER_1,      // �|�[�Y�I�����ڗp(CONTINUE)
		TEX_NUMBER_2,      // �|�[�Y�I�����ڗp(RETRY)
		TEX_NUMBER_3,      // �|�[�Y�I�����ڗp(QUIT)
		TEX_NUMBER_4,      // �v���[���g�p�r���{�[�h
		TEX_NUMBER_MAX,
	}TEX_NUMBER;

	CGame(int nPriotiry = 0, OBJTYPE objType = OBJTYPE_GAME);
	~CGame();

	static CGame *Create(void);
	static void LoadGoalModel(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	bool GetMapClear(void);
	void SetMapClear(bool bMapClear);
	void SetMap(void);

	void BindPauseTexture(CPause *pPause);
	void SetState(STATE state);
	STATE GetState(void);

	int GetNumStageClear(void);
	int GetNumStock(void);

	void CutPresentNum(void);
	void AddScore(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	LPD3DXMESH            m_pMesh;       // ���b�V�����ւ̃|�C���^
	LPD3DXBUFFER          m_pBuffMat;    // �}�e���A�����ւ̃|�C���^
	DWORD                 m_nNumMat;     // �}�e���A�����̐�
	D3DXVECTOR3           m_VtxMax;      // ���_���W�̍ő�l�ւ̃|�C���^
	D3DXVECTOR3           m_VtxMin;      // ���_���W�̍ŏ��l�ւ̃|�C���^

	int             m_nNumStock;          // �v���C���[�̎c�@��
	int             m_nNumPresent;        // �v���[���g�̎c�萔
	int             m_nNumStageClear;     // �X�e�[�W�N���A������
	CUI             *m_pUi;               // UI�N���X�ւ̃|�C���^
	CMap            *m_pMap;              // �}�b�v�N���X�ւ̃|�C���^
	STATE           m_State;              // ���
	int             m_nStateCounter;      // ��Ԃ��Ǘ�����J�E���^�[
	CTextureManager *m_pTextureManager;   // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
	bool            m_bMapClear;          // �}�b�v���N���A�������ǂ���
};

#endif