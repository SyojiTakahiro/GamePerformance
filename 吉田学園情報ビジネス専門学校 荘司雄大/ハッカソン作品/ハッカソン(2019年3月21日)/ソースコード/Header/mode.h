//*****************************************************************************
//
//     ���[�h�̏���[mode.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MODE_H_
#define _MODE_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CScene2D;
class CScene2DFlash;
class CScene2DPress;
class CTextureManager;
class CPlayerManager;
class CPlayer;
class CEffectManager;
class CMap;
class CCamera;
class CPause;

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define MAX_CAMERA_NUM   (2)
#define MAX_TEXTURE		 (3)

//*****************************************************************************
//     ���[�h�x�[�X�N���X�̒�`
//*****************************************************************************
class CBaseMode
{
public:    // �N�ł��A�N�Z�X�\
	CBaseMode();
	~CBaseMode();
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void);
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	void SetModeCounter(const int nModeCounter);
	void SetPress(CScene2DPress *pPress);
	void SetTextureManager(CTextureManager *pTextureManager);
	void SetModeChange(const bool bModeChange);
	void SetPlayerManager(CPlayerManager *pPlayerManager);
	void SetPlayer(CPlayer *pPlayer);
	void SetEffectManager(CEffectManager *pEffectManager);
	void SetMap(CMap *pMap);
	void SetCamera(CCamera *pCamera, int nIdx);

	int GetModeCounter(void);
	CScene2DPress *GetPress(void);
	CTextureManager *GetTextureManager(void);
	bool GetModeChange(void);
	CPlayerManager *GetPlayerManager(void);
	CPlayer *GetPlayer(void);
	CEffectManager *GetEffectManager(void);
	CMap *GetMap(void);
	CCamera *GetCamera(int nIdx);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void ModeChange(int nAutoNext, int nPressNext, int nChangeTiming);
	void ReleaseTextureManager(void);
	void ReleasePlayerManager(void);
	void ReleaseEffectManager(void);
	void ReleaseMap(void);
	void ReleaseCamera(void);

private:   // ���̃N���X�������A�N�Z�X�\
	CScene2DPress   *m_pPress;                  // PRESSENTER�N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;         // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	int             m_nModeCounter;             // ���[�h�J�E���^�[(��Ɏ�����ʑJ�ڂɎg�p)
	bool            m_bModeChange;              // ��ʑJ�ڂ����Ă��邩�ǂ���
	CPlayerManager  *m_pPlayerManager;          // �v���C���[�Ǌ��N���X�ւ̃|�C���^
	CPlayer         *m_pPlayer;                 // �v���C���[�N���X�ւ̃|�C���^
	CEffectManager  *m_pEffectManager;          // �G�t�F�N�g�Ǌ��N���X�ւ̃|�C���^
	CMap            *m_pMap;                    // �}�b�v�Ǌ��N���X�ւ̃|�C���^
	CCamera         *m_pCamera[MAX_CAMERA_NUM]; // �J�����N���X�ւ̃|�C���^
};

//*****************************************************************************
//     �^�C�g���N���X�̒�`
//*****************************************************************************
class CTitle : public CBaseMode
{
public:    // �N�ł��A�N�Z�X�\
	CTitle();
	~CTitle();

	static CTitle *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	CScene2D *m_pBg[MAX_TEXTURE];                  // �Z�b�e�B���O��ʂ̔w�i
};

//*****************************************************************************
//     �}�b�v�Љ�[�h�N���X�̒�`
//*****************************************************************************
class CViewMap : public CBaseMode
{
public:    // �N�ł��A�N�Z�X�\
	CViewMap();
	~CViewMap();

	static CViewMap *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\

};

//*****************************************************************************
//     �`���[�g���A���N���X�̒�`
//*****************************************************************************
class CTutorial : public CBaseMode
{
public:    // �N�ł��A�N�Z�X�\
	CTutorial();
	~CTutorial();

	static CTutorial *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};

//*****************************************************************************
//     �Z�b�e�B���O�N���X�̒�`
//*****************************************************************************
class CSetting : public CBaseMode
{
public:    // �N�ł��A�N�Z�X�\
	//--------------------
	//  ���
	//--------------------
	typedef enum
	{
		STATE_CHECKJOIN = 0,   // �Q����t���
		STATE_MAPSELECT,       // �}�b�v�I�����
		STATE_MAX
	}STATE;

	CSetting();
	~CSetting();

	static CSetting *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void CheckJoin(void);
	void MapSelect(void);

	STATE m_State;                    // ���
	int m_nSelectMapIdx;              // �}�b�v�̔ԍ�
	CScene2D *m_pBg;                  // �Z�b�e�B���O��ʂ̔w�i
	CScene2D *m_pJoin[2];             // �v���C���[�Q���\���p�|���S��
	CScene2DFlash *m_pMapSelect[3];   // �}�b�v�I��\���p�|���S��
	CScene2DPress *m_pJoinPress;      // �Q����t�\���p�|���S��
	bool m_bJoin[2];                  // �Q�킷�邩�ǂ���(�l����)
	int m_nNumPlayer;                 // �Q������l��
	int m_nOnePlayer;
};

//*****************************************************************************
//     �Q�[���N���X�̒�`
//*****************************************************************************
class CGame : public CBaseMode
{
public:    // �N�ł��A�N�Z�X�\
	CGame();
	~CGame();

	//--------------------
	//  ���
	//--------------------
	typedef enum
	{
		STATE_NORMAL = 0,   // �ʏ���
		STATE_END,          // �I�����
		STATE_MAX
	}STATE;

	static CGame *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ReleasePause(void);
	void SetStarObject(void);

	static void SettingPlayer(const int nControllerIdx);
	static void SetGameMapIdx(const int nGameMapIdx);
	static void SetNumPlayer(const int nNumPlayer);

	void SetState(const STATE state);
	void SetPause(const bool bPause);
	void SetPauseController(const int nPauseController);

	STATE GetState(void);
	bool GetPause(void);
	int GetPauseController(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Normal(void);
	void End(void);

	typedef struct
	{
		bool bUse;
		int nControllerIdx;
	}PLAYER_SETTING;

	static int m_nNumPlayer;
	static int m_nGameMapIdx;
	static PLAYER_SETTING m_PlayerSetting[2];

	STATE m_State;            // ���
	int m_nStateCounter;      // ��Ԃ��Ǘ�����J�E���^�[
	bool m_bPause;            // �|�[�Y�����ǂ���
	CPause *m_pPause;         // �|�[�Y�N���X�ւ̃|�C���^
	int m_nPauseController;   // �|�[�Y���������R���g���[���[

	CPlayer *m_pPlayer[2];
};

//*****************************************************************************
//     ���U���g�N���X�̒�`
//*****************************************************************************
class CResult : public CBaseMode
{
public:    // �N�ł��A�N�Z�X�\
	CResult();
	~CResult();

	static CResult *Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static void SetNumPlayer(const int nNumPlayer);
	static void SetWinnerIdx(const int nWinnerIdx);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static int m_nNumPlayer;
	static int m_nWinnerIdx;

	CScene2D *m_pBg[2];
};

#endif