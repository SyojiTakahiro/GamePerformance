//*****************************************************************************
//
//     �v���C���[�̏���[player.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "sceneX.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define PLAYER_PRIORITY (5)    // �v���C���[�̏����D�揇��

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CShadow;
class CModel;
class CMotionManager;

//*****************************************************************************
//    �v���C���[�N���X�̒�`
//*****************************************************************************
class CPlayer : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	//------------------------
	//  ���
	//------------------------
	typedef enum
	{
		STATE_NORMAL = 0,  // �ʏ���
		STATE_MOVE_WALK,   // �������
		STATE_MOVE_RUN,    // ������
		STATE_JUMP,        // �W�����v���Ă�����
		STATE_LANDING,     // ���n���Ă�����
		STATE_PUSH,        // �����������Ă�����
		STATE_PANCH,       // �p���`�����Ă�����
		STATE_KICK,        // �L�b�N�����Ă�����
		STATE_ZIPLINE,     // �W�b�v���C���ɂ��܂��Ă�����
		STATE_MAX
	}STATE;

	CPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CPlayer();

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority = 3);
	static HRESULT Load(char *pFileName);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice);
	void SetDefault(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetMove(void);
	float GetColHeight(void);
	bool GetJump(void);

	void SetState(const STATE state);
	void SetMove(const D3DXVECTOR3 move);
	void SetJump(const bool bJump);
	void SetPos(const D3DXVECTOR3 pos);
	void SetPosOld(const D3DXVECTOR3 posOld);
	void SetRot(const D3DXVECTOR3 rot);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Movement(void);
	void AddMovement(void);
	bool Collision(void);
	void Action(void);
	void Statement(void);
	void Landing(void);
	bool AttackCollision(D3DXVECTOR3 pos, D3DXVECTOR3 VtxMin, D3DXVECTOR3 VtxMax);
	void CreateEffect(void);
	void Sound(void);

	static LPD3DXMESH     *m_apMesh;         // ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER   *m_apBuffMat;      // �}�e���A�����ւ̃|�C���^
	static DWORD          *m_anNumMat;       // �}�e���A�����̐�
	static D3DXVECTOR3    *m_aVtxMin;        // ���_���W�̍ŏ��l
	static D3DXVECTOR3    *m_aVtxMax;        // ���_���W�̍ő�l
	static int            m_nNumModel;       // �v���C���[�̎g�p����p�[�c��
	static char           m_aFileName[256];  // �v���C���[�̃f�[�^�t�@�C����

	CModel          **m_apModel;          // ���f���N���X�ւ̃|�C���^
	int             m_nNumPart;           // �g�p����p�[�c��
	CMotionManager  *m_pMotionManager;    // ���[�V�����Ǘ��N���X�ւ̃|�C���^
	D3DXVECTOR3     m_Pos;                // ���݂̍��W
	D3DXVECTOR3     m_PosOld;             // �O��̍��W
	D3DXVECTOR3     m_Move;               // �ړ���
	D3DXVECTOR3     m_Rot;                // ���݂̌���
	D3DXVECTOR3     m_DiffAngle;          // ���݂̌����ƖړI�̌����̍���
	D3DXVECTOR3     m_DestAngle;          // �ړI�̌���
	D3DXMATRIX      m_MtxWorld;           // ���[���h�}�g���b�N�X
	STATE           m_State;              // ���
	float           m_fGravity;           // �d��
	float           m_fJumpPower;         // �W�����v��
	bool            m_bJump;              // �W�����v�������ǂ���
	bool            m_bAction;            // �A�N�V������Ԃł��邩�ǂ���
	float           m_fAccel;             // �����x
	float           m_fInertia;           // ����
	float           m_fRivisionRot;       // ������␳����{��
	float           m_fColHeight;         // �����蔻����Ƃ鍂��
	float           m_fMaxWalkSpeed;      // �v���C���[�������X�s�[�h�̍ő�l
	CShadow         *m_pShadow;           // �e�N���X�ւ̃|�C���^
	CScene          *m_pLandScene;        // �v���C���[������Ă���N���X�ւ̃|�C���^
	bool            m_bMove;              // �ړ����Ă��邩�ǂ���
	bool            m_bLand;              // �I�u�W�F�N�g�ɏ���Ă��邩�ǂ���
	bool            m_bPush;               // �I�u�W�F�N�g�������Ă��邩�ǂ���
	bool            m_bCol;
	D3DXVECTOR3     m_OffsetPosStd;       // �I�u�W�F�N�g����̃I�t�Z�b�g��ʒu
	D3DXVECTOR3     m_OffsetRot;          // �I�u�W�F�N�g����̃I�t�Z�b�g����
	D3DXVECTOR3     m_OffsetRotStart;     // �I�u�W�F�N�g�ɏ�������̌���(�I�u�W�F�N�g�ɏ���Ă�����������Z����)
	D3DXVECTOR3     m_OffsetPosAdd;       // ���݂̃I�u�W�F�N�g����̃I�t�Z�b�g���Z�ʒu
	D3DXVECTOR3     m_OffsetPosAddOld;    // �O��̃I�u�W�F�N�g����̃I�t�Z�b�g���Z�ʒu
};

#if 0 // ���f���P�̂̃v���C���[�����Ȃ̂ō��񂱂�͎g��Ȃ��ł�
//*****************************************************************************
//    �v���C���[(���f���P��)�N���X�̒�`
//*****************************************************************************
class CSceneXPlayer : public CSceneX
{
public:    // �N�ł��A�N�Z�X�\
	CSceneXPlayer(int nPriority = 3, OBJTYPE objType = OBJTYPE_PLAYER);
	~CSceneXPlayer();

	static CSceneXPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CalcMtxWorld(const LPDIRECT3DDEVICE9 pDevice);

	D3DXVECTOR3 GetMove(void);
	bool GetJump(void);

	void SetMove(const D3DXVECTOR3 move);
	void SetJump(const bool bJump);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Movement(void);
	bool Collision(void);
	void Action(void);

	static LPD3DXMESH     m_apMesh;      // ���b�V�����ւ̃|�C���^
	static LPD3DXBUFFER   m_apBuffMat;   // �}�e���A�����ւ̃|�C���^
	static DWORD          m_anNumMat;    // �}�e���A�����̐�

	D3DXVECTOR3 m_Move;       // �ړ���
	D3DXVECTOR3 m_DiffAngle;  // ���݂̌����ƖړI�̌����̍���
	D3DXVECTOR3 m_DestAngle;  // �ړI�̌���
	float       m_fGravity;   // �d��
	float       m_fJumpPower; // �W�����v��
	bool        m_bJump;      // �W�����v�������ǂ���
	float       m_fAccel;     // �����x
	float       m_fInertia;   // ����
	CShadow     *m_pShadow;   // �e�N���X�ւ̃|�C���^
};
#endif

#endif
