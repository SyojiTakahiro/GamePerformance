//*****************************************************************************
//
//     �p�[�e�B�N���̏���[particle.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "sceneBillboard.h"

//*****************************************************************************
//    �p�[�e�B�N���f�[�^�N���X�̒�`
//*****************************************************************************
class CParData
{
public:    // �N�ł��A�N�Z�X�\
	CParData();
	~CParData();

	void SetRange(const D3DXVECTOR3 Range) { m_Range = Range; }
	void SetRangeMag(const float fRangeMag) { m_fRangeMag = fRangeMag; }
	void SetMaxRot(const float fMaxRot) { m_fMaxRot = fMaxRot; }
	void SetMinRot(const float fMinRot) { m_fMinRot = fMinRot; }
	void SetMaxMove(const D3DXVECTOR3 MaxMove) { m_MaxMove = MaxMove; }
	void SetMinMove(const D3DXVECTOR3 MinMove) { m_MinMove = MinMove; }
	void SetChangeMove(const D3DXVECTOR3 ChangeMove) { m_ChangeMove = ChangeMove; }
	void SetInitCol(const D3DXCOLOR InitCol) { m_InitCol = InitCol; }
	void SetChangeCol(const D3DXCOLOR ChangeCol) { m_ChangeCol = ChangeCol; }
	void SetMaxWidth(const float fMaxWidth) { m_fMaxWidth = fMaxWidth; }
	void SetMinWidth(const float fMinWidth) { m_fMinWidth = fMinWidth; }
	void SetMaxHeight(const float fMaxHeight) { m_fMaxHeight = fMaxHeight; }
	void SetMinHeight(const float fMinHeight) { m_fMinHeight = fMinHeight; }
	void SetSpread(const float fSpread) { m_fSpread = fSpread; }
	void SetMaxLife(const int nMaxLife) { m_nMaxLife = nMaxLife; }
	void SetMinLife(const int nMinLife) { m_nMinLife = nMinLife; }
	void SetGravity(const float fGravity) { m_fGravity = fGravity; }
	void SetBouncing(const float fBouncing) { m_fBouncing = fBouncing; }
	void SetRotPattern(const int nRotPattern) { m_nRotPattern = nRotPattern; }
	void SetRotSpeed(const float fRotSpeed) { m_fRotSpeed = fRotSpeed; }
	void SetCollision(const bool bCollision) { m_bCollision = bCollision; }
	void SetDrawAddtive(const bool bDrawAddtive) { m_bDrawAddtive = bDrawAddtive; }
	void SetLighting(const bool bLighting) { m_bLighting = bLighting; }

	D3DXVECTOR3 GetRange(void) { return m_Range; }
	float GetRangeMag(void) { return m_fRangeMag; }
	float GetMaxRot(void) { return m_fMaxRot; }
	float GetMinRot(void) { return m_fMinRot; }
	D3DXVECTOR3 GetMaxMove(void) { return m_MaxMove; }
	D3DXVECTOR3 GetMinMove(void) { return m_MinMove; }
	D3DXVECTOR3 GetChangeMove(void) { return m_ChangeMove; }
	D3DXCOLOR GetInitCol(void) { return m_InitCol; }
	D3DXCOLOR GetChangeCol(void) { return m_ChangeCol; }
	float GetMaxWidth(void) { return m_fMaxWidth; }
	float GetMinWidth(void) { return m_fMinWidth; }
	float GetMaxHeight(void) { return m_fMaxHeight; }
	float GetMinHeight(void) { return m_fMinHeight; }
	float GetSpread(void) { return m_fSpread; }
	int GetMaxLife(void) { return m_nMaxLife; }
	int GetMinLife(void) { return m_nMinLife; }
	float GetGravity(void) { return m_fGravity; }
	float GetBouncing(void) { return m_fBouncing; }
	int GetRotPattern(void) { return m_nRotPattern; }
	float GetRotSpeed(void) { return m_fRotSpeed; }
	bool GetCollision(void) { return m_bCollision; }
	bool GetDrawAddtive(void) { return m_bDrawAddtive; }
	bool GetLighting(void) { return m_bLighting; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3 m_Range;         // ��������͈�(�I�t�Z�b�g)
	float       m_fRangeMag;     // ��������͈͂̔{��
	float       m_fMaxRot;       // �������̌���(�ő�l)
	float       m_fMinRot;       // �������̌���(�ŏ��l)
	D3DXVECTOR3 m_MaxMove;       // �ړ��ʂ̍ő�l
	D3DXVECTOR3 m_MinMove;       // �ړ��ʂ̍ŏ��l
	D3DXVECTOR3 m_ChangeMove;    // �ړ��ʂ̕ω���
	D3DXCOLOR   m_InitCol;       // �������̐F
	D3DXCOLOR   m_ChangeCol;     // �t���[�����̐F�̕ω���
	float       m_fMaxWidth;     // ���̍ő�l
	float       m_fMinWidth;     // ���̍ŏ��l
	float       m_fMaxHeight;    // �����̍ő�l
	float       m_fMinHeight;    // �����̍ŏ��l
	float       m_fSpread;       // �傫���̕ω���
	int         m_nMaxLife;      // �����̍ő�l
	int         m_nMinLife;      // �����̍ŏ��l
	float       m_fGravity;      // ���t���[��������d��
	float       m_fBouncing;     // �o�E���h��
	int         m_nRotPattern;   // ��]�̎��[ 0:���v��� 1:�����v��� 2:�����_�� ]
	float       m_fRotSpeed;     // ��]����X�s�[�h
	bool        m_bCollision;    // �����蔻������邩�ǂ���
	bool        m_bDrawAddtive;  // ���Z�������邩���Ȃ���[ 0:�Ȃ� 1:���� ]
	bool        m_bLighting;     // ���C�e�B���O���邩���Ȃ���[ 0:�Ȃ� 1:���� ]
};

//*****************************************************************************
//    �p�[�e�B�N���N���X�̒�`
//*****************************************************************************
class CParticle : public CSceneBillboard
{
public:    // �N�ł��A�N�Z�X�\
	CParticle(int nPriority = 3, OBJTYPE objType = OBJTYPE_PARTICLE);
	~CParticle();

	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col, float fWidth,
		float fHeight, float fRot, D3DXCOLOR ChangeCol, float fGravity, float fSpread,
		float fChangeRot, float fBouncing, int nLife, int nRotPattern,
		bool bCol, bool bDrawAddtive, bool bLighting, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 ChangeMove, D3DXCOLOR col,
		float fWidth, float fHeight, float fRot, D3DXCOLOR ChangeCol, float fGravity, float fSpread,
		float fChangeRot, float fBouncing, int nLife, int nRotPattern,
		bool bCol, bool bDrawAddtive, bool bLighting);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetMove(const D3DXVECTOR3 move);
	void SetChangeMove(const D3DXVECTOR3 ChangeMove);
	void SetChangeCol(const D3DXCOLOR ChangeCol);
	void SetLife(const int nLife);
	void SetRotPattern(const int nRotPattern);
	void SetGravity(const float fGravity);
	void SetSpread(const float fSpread);
	void SetChangeRot(const float fChangeRot);
	void SetBouncing(const float fBouncing);
	void SetCollision(const bool bCollision);
	void SetDrawAddtive(const bool bDrawAddtive);

	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetChangeMove(void);
	D3DXCOLOR GetChangeCol(void);
	int GetLife(void);
	int GetRotPattern(void);
	float GetGravity(void);
	float GetSpread(void);
	float GetChangeRot(void);
	float GetBouncing(void);
	bool GetCollision(void);
	bool GetDrawAddtive(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3 m_Move;           // �ړ���
	D3DXVECTOR3 m_ChangeMove;     // �ړ��ʂ̕ω���
	D3DXCOLOR   m_ChangeCol;      // �F�̕ω���
	int         m_nLife;          // ����
	int         m_nRotPattern;    // ��]�̎��
	float       m_fGravity;       // �d��
	float       m_fSpread;        // �傫���̕ω���
	float       m_fChangeRot;     // ��]�̕ω���
	float       m_fBouncing;      // �o�E���h��
	bool        m_bCollision;     // �����蔻�����邩�ǂ���
	bool        m_bDrawAddtive;   // ���Z�����ŕ`�悷�邩���Ȃ���
	bool        m_bLighting;      // ���C�e�B���O���邩���Ȃ���
};

#endif