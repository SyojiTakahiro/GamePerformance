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

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CMap;
class CScene2DSquare;

//*****************************************************************************
//    �Q�[���N���X�̒�`
//*****************************************************************************
class CGame : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	//------------------------
	//  �Q�[���̏��
	//------------------------
	typedef enum
	{
		STATE_NORMAL = 0,    // �ʏ���
		STATE_CHALLENGE,     // �}�b�v�ɒ��킵�Ă�����
		STATE_PLAYERFALL,    // �v���C���[�����������
		STATE_MAPCHANGE,     // �}�b�v�؂�ւ����
		STATE_MAPCLEAR,      // �}�b�v�N���A���
		STATE_END,           // �I�����
		STATE_MAX
	}STATE;

	CGame(int nPriority = 0, OBJTYPE objType = OBJTYPE_GAME);
	~CGame();

	static CGame *Create(void);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	STATE GetState(void);
	bool GetMapClear(void);

	void SetState(STATE state);
	void SetMapClear(bool bMapClear);
	void SetMapName(char *aMapFileName);
	void SetMap(void);
	void ClearMapEvent(void);
	void PlayerFall(void);
	void PlayerRespawnEvent(void);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3DTEXTURE9 m_pTexture;
	CScene2DSquare *m_pScene2D; // ������@�\���p�̃|���S��
	CMap  *m_pMap;              // �}�b�v���ւ̃|�C���^
	STATE m_State;              // �Q�[���̏��
	char  m_aMapFileName[256];  // �}�b�v�̃X�N���v�g�t�@�C����
	int   m_nStatementCounter;  // ��Ԃ��Ǘ�����J�E���^�[
	int   m_nNumClearMap;       // �N���A�����}�b�v��
	int   m_nMapClearBonus;     // �}�b�v�N���A�{�[�i�X
	bool  m_bMapClear;          // �}�b�v���N���A�������ǂ���
};

#endif