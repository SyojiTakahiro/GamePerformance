//*****************************************************************************
//
//     �G�t�F�N�g�̏���[effect.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EFFECT_H_
#define _EFFECT_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "2DPolygon.h"
#include "3DPolygon.h"
#include "3DMesh.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define EFFECT_PRIORITY     (9)       // �G�t�F�N�g�̏����D�揇��
#define MAX_EMMITER         (100)     // �G�~�b�^�f�[�^�̍ő吔
#define MAX_PARTICLEDATA    (100)     // �p�[�e�B�N���f�[�^�̍ő吔
#define MAX_RINGEFFECTDATA  (100)     // �����O�G�t�F�N�g�f�[�^�̍ő吔

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CTextureManager;

//*****************************************************************************
//     �G�~�b�^�f�[�^�N���X�̒�`
//*****************************************************************************
class CEmitterData
{
public:    // �N�ł��A�N�Z�X�\
	CEmitterData();
	~CEmitterData();

	void SetType(const int nType) { m_nType = nType; }
	void SetTexIdx(const int nTexIdx) { m_nTexIdx = nTexIdx; }
	void SetEffectIdx(const int nEffectIdx) { m_nEffectIdx = nEffectIdx; }
	void SetRangeMax(const D3DXVECTOR3 RangeMax) { m_RangeMax = RangeMax; }
	void SetRangeMin(const D3DXVECTOR3 RangeMin) { m_RangeMin = RangeMin; }
	void SetLife(const int nLife) { m_nLife = nLife; }
	void SetLap(const int nLap) { m_nLap = nLap; }
	void SetAppear(const int nAppear) { m_nAppear = nAppear; }
	void SetLoop(const bool bLoop) { m_bLoop = bLoop; }

	int GetType(void) { return m_nType; }
	int GetTexIdx(void) { return m_nTexIdx; }
	int GetEffectIdx(void) { return m_nEffectIdx; }
	D3DXVECTOR3 GetRangeMax(void) { return m_RangeMax; }
	D3DXVECTOR3 GetRangeMin(void) { return m_RangeMin; }
	int GetLife(void) { return m_nLife; }
	int GetLap(void) { return m_nLap; }
	int GetAppear(void) { return m_nAppear; }
	bool GetLoop(void) { return m_bLoop; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int         m_nType;            // �g�p����G�t�F�N�g�̎��
	int         m_nTexIdx;          // �g�p����e�N�X�`���̔ԍ�
	int         m_nEffectIdx;       // �g�p����G�t�F�N�g�f�[�^�̔ԍ�
	D3DXVECTOR3 m_RangeMax;         // �G�t�F�N�g�𐶐�����͈͂̍ő�l
	D3DXVECTOR3 m_RangeMin;         // �G�t�F�N�g�𐶐�����͈͂̍ŏ��l
	int         m_nLife;            // ����
	int         m_nLap;             // 1��̕��o�ŏo���G�t�F�N�g�̌�
	int         m_nAppear;          // �G�t�F�N�g���o���Ԋu
	bool        m_bLoop;            // ���[�v���邩���Ȃ���
};

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
//    �����O�G�t�F�N�g�f�[�^�N���X�̒�`
//*****************************************************************************
class CRingData
{
public:    // �N�ł��A�N�Z�X�\
	CRingData();
	~CRingData();

	void SetRot(const D3DXVECTOR3 Rot) { m_Rot = Rot; }
	void SetMaxMove(const D3DXVECTOR3 MaxMove) { m_MaxMove = MaxMove; }
	void SetMinMove(const D3DXVECTOR3 MinMove) { m_MinMove = MinMove; }
	void SetChangeMove(const D3DXVECTOR3 ChangeMove) { m_ChangeMove = ChangeMove; }
	void SetInitCol(const D3DXCOLOR InitCol) { m_InitCol = InitCol; }
	void SetChangeCol(const D3DXCOLOR ChangeCol) { m_ChangeCol = ChangeCol; }
	void SetHeight(const float fHeight) { m_fHeight = fHeight; }
	void SetRadius(const float fRadius) { m_fRadius = fRadius; }
	void SetDiffusion(const float fDiffusion) { m_fDiffusion = fDiffusion; }
	void SetChangeHeight(const float fChangeHeight) { m_fChangeHeight = fChangeHeight; }
	void SetChangeRadius(const float fChangeRadius) { m_fChangeRadius = fChangeRadius; }
	void SetChangeDiffusion(const float fChangeDiffusion) { m_fChangeDiffusion = fChangeDiffusion; }
	void SetMaxLife(const int nMaxLife) { m_nMaxLife = nMaxLife; }
	void SetMinLife(const int nMinLife) { m_nMinLife = nMinLife; }
	void SetXBlock(const int nXBlock) { m_nXBlock = nXBlock; }
	void SetYBlock(const int nYBlock) { m_nYBlock = nYBlock; }
	void SetRotPattern(const int nRotPattern) { m_nRotPattern = nRotPattern; }
	void SetRotSpeed(const D3DXVECTOR3 RotSpeed) { m_RotSpeed = RotSpeed; }
	void SetCulling(const bool bCulling) { m_bCulling = bCulling; }
	void SetDrawAddtive(const bool bDrawAddtive) { m_bDrawAddtive = bDrawAddtive; }

	D3DXVECTOR3 GetRot(void) { return m_Rot; }
	D3DXVECTOR3 GetMaxMove(void) { return m_MaxMove; }
	D3DXVECTOR3 GetMinMove(void) { return m_MinMove; }
	D3DXVECTOR3 GetChangeMove(void) { return m_ChangeMove; }
	D3DXCOLOR GetInitCol(void) { return m_InitCol; }
	D3DXCOLOR GetChangeCol(void) { return m_ChangeCol; }
	float GetHeight(void) { return m_fHeight; }
	float GetRadius(void) { return m_fRadius; }
	float GetDiffusion(void) { return m_fDiffusion; }
	float GetChangeHeight(void) { return m_fChangeHeight; }
	float GetChangeRadius(void) { return m_fChangeRadius; }
	float GetChangeDiffusion(void) { return m_fChangeDiffusion; }
	int GetMaxLife(void) { return m_nMaxLife; }
	int GetMinLife(void) { return m_nMinLife; }
	int GetXBlock(void) { return m_nXBlock; }
	int GetYBlock(void) { return m_nYBlock; }
	int GetRotPattern(void) { return m_nRotPattern; }
	D3DXVECTOR3 GetRotSpeed(void) { return m_RotSpeed; }
	bool GetCulling(void) { return m_bCulling; }
	bool GetDrawAddtive(void) { return m_bDrawAddtive; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3 m_Rot;               // ����
	D3DXVECTOR3 m_MaxMove;           // �ړ��ʂ̍ő�l
	D3DXVECTOR3 m_MinMove;           // �ړ��ʂ̍ŏ��l
	D3DXVECTOR3 m_ChangeMove;        // �ړ��ʂ̕ω���
	D3DXCOLOR   m_InitCol;           // �������̐F
	D3DXCOLOR   m_ChangeCol;         // �t���[�����̐F�̕ω���
	float       m_fHeight;           // ����
	float       m_fRadius;           // ���a
	float       m_fDiffusion;        // ��
	float       m_fChangeHeight;     // �����𖈃t���[���ǂꂭ�炢�ω������邩
	float       m_fChangeRadius;     // ���a���ǂꂭ�炢���t���[���ω������邩
	float       m_fChangeDiffusion;  // �����O�̕����ǂꂭ�炢���t���[���ω������邩
	int         m_nXBlock;           // ���̕�����
	int         m_nYBlock;           // �c�̕�����
	int         m_nMaxLife;          // �����̍ő�l
	int         m_nMinLife;          // �����̍ŏ��l
	int         m_nRotPattern;       // ��]�̎��[ 0:���v��� 1:�����v��� 2:�����_�� ]
	D3DXVECTOR3 m_RotSpeed;          // ��]����X�s�[�h
	bool        m_bCulling;          // �J�����O���邩�ǂ���[ 0:�Ȃ� 1:���� ]
	bool        m_bDrawAddtive;      // ���Z�������邩���Ȃ���[ 0:�Ȃ� 1:���� ]
};

//*****************************************************************************
//     �G�~�b�^�N���X�̒�`
//*****************************************************************************
class CEmitter : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CEmitter(int nPriority = 1, OBJTYPE objType = OBJTYPE_EMITTER);
	~CEmitter();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetType(const int nType);
	void SetTexIdx(const int nTexIdx);
	void SetEffectIdx(const int nEffectIdx);
	void SetRangeMax(const D3DXVECTOR3 RangeMax);
	void SetRangeMin(const D3DXVECTOR3 RangeMin);
	void SetLife(const int nLife);
	void SetLap(const int nLap);
	void SetAppear(const int nAppear);
	void SetLoop(const bool bLoop);

	LPDIRECT3DTEXTURE9 GetTexture(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	int GetType(void);
	int GetTexIdx(void);
	int GetEffectIdx(void);
	D3DXVECTOR3 GetRangeMax(void);
	D3DXVECTOR3 GetRangeMin(void);
	int GetLife(void);
	int GetLap(void);
	int GetAppear(void);
	bool GetLoop(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	virtual void SetEffect(void) = 0;

private:   // ���̃N���X�������A�N�Z�X�\
	void Movement(void);

	LPDIRECT3DTEXTURE9 m_pTexture;   // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3        m_Pos;        // ���W
	D3DXVECTOR3        m_Rot;        // ����
	D3DXMATRIX         m_MtxWorld;   // ���[���h�}�g���b�N�X
	int                m_nType;      // �g�p����G�t�F�N�g�̎��
	int                m_nTexIdx;    // �g�p����e�N�X�`���ԍ�
	int                m_nEffectIdx; // �g�p����G�t�F�N�g�̔ԍ�
	D3DXVECTOR3        m_RangeMax;   // �G�t�F�N�g�𐶐�����͈͂̍ő�l
	D3DXVECTOR3        m_RangeMin;   // �G�t�F�N�g�𐶐�����͈͂̍ŏ��l
	int                m_nLife;      // �G�~�b�^�̎���
	int                m_nLap;       // 1��̕��o�^�C�~���O�ɏo���G�t�F�N�g�̌�
	int                m_nAppear;    // ���o����Ԋu
	int                m_nCounter;   // ���o�^�C�~���O���Ǘ�����J�E���^�[
	bool               m_bLoop;      // ���[�v���邩���Ȃ���
};

//*****************************************************************************
//     �p�[�e�B�N���G�~�b�^�N���X�̒�`
//*****************************************************************************
class CParEmitter : public CEmitter
{
public:    // �N�ł��A�N�Z�X�\
	CParEmitter(int nPriority = 1, OBJTYPE objType = OBJTYPE_PAREMITTER);
	~CParEmitter();

	static CParEmitter *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexIdx, int nEffectIdx, D3DXVECTOR3 RangeMax, D3DXVECTOR3 RangeMin, int nLife, int nLap, int nAppear, bool bLoop, CParData *pParData, LPDIRECT3DTEXTURE9 pTexture, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetParData(CParData *pParData);
	CParData *GetParData(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void SetEffect(void);

private:   // ���̃N���X�������A�N�Z�X�\
	CParData  *m_pParData;  // �p�[�e�B�N���f�[�^�ւ̃|�C���^
};

//*****************************************************************************
//     �����O�G�t�F�N�g�G�~�b�^�N���X�̒�`
//*****************************************************************************
class CRingEmitter : public CEmitter
{
public:    // �N�ł��A�N�Z�X�\
	CRingEmitter(int nPriority = 3, OBJTYPE objType = OBJTYPE_RINGEMITTER);
	~CRingEmitter();

	static CRingEmitter *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexIdx, int nEffectIdx, D3DXVECTOR3 RangeMax, D3DXVECTOR3 RangeMin, int nLife, int nLap, int nAppear, bool bLoop, CRingData *pRingData, LPDIRECT3DTEXTURE9 pTexture, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetRingData(CRingData *pRingData);
	CRingData *GetRingData(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	void SetEffect(void);

private:   // ���̃N���X�������A�N�Z�X�\
	CRingData  *m_pRingData;  // �����O�G�t�F�N�g�f�[�^�ւ̃|�C���^
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
	void ChangeVertex(D3DXVECTOR3 pos, D3DXCOLOR col, float fRot, float fLength);

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
};

//*****************************************************************************
//    �����O�G�t�F�N�g�N���X�̒�`
//*****************************************************************************
class CRingEffect : public CMeshRing
{
public:    // �N�ł��A�N�Z�X�\
	CRingEffect(int nPriority = 3, OBJTYPE objType = OBJTYPE_RINGEFFECT);
	~CRingEffect();
	static CRingEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock,
		int nLife, D3DXVECTOR3 RotSpeed, int nRotPattern, D3DXVECTOR3 Move, D3DXVECTOR3 ChangeMove, D3DXCOLOR ChangeCol, float fChangeHeight,
		float fChangeRadius, float fChangeDiffusion, bool bCulling, bool bDrawAddtive, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fHeight, float fRadius, float fDiffusion, int nXBlock, int nYBlock,
		int nLife, D3DXVECTOR3 RotSpeed, int nRotPattern, D3DXVECTOR3 Move, D3DXVECTOR3 ChangeMove, D3DXCOLOR ChangeCol, float fChangeHeight,
		float fChangeRadius, float fChangeDiffusion, bool bCulling, bool bDrawAddtive);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void ChangeVertex(void);

	void SetLife(const int nLife);
	void SetRotSpeed(const D3DXVECTOR3 RotSpeed);
	void SetRotPattern(const int nRotPattern);
	void SetMove(const D3DXVECTOR3 Move);
	void SetChangeMove(const D3DXVECTOR3 ChangeMove);
	void SetChangeCol(const D3DXCOLOR ChangeCol);
	void SetChangeHeight(const float fChangeHeight);
	void SetChangeRadius(const float fChangeRadisu);
	void SetChangeDiffusion(const float fChangeDiffusion);
	void SetCulling(const bool bCulling);
	void SetDrawAddtive(const bool bDrawAddtive);

	int GetLife(void);
	D3DXVECTOR3 GetRotSpeed(void);
	int GetRotPattern(void);
	D3DXVECTOR3 GetMove(void);
	D3DXVECTOR3 GetChangeMove(void);
	D3DXCOLOR GetChangeCol(void);
	float GetChangeHeight(void);
	float GetChangeRadius(void);
	float GetChangeDiffusion(void);
	bool GetCulling(void);
	bool GetDrawAddtive(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int         m_nLife;             // ����
	D3DXVECTOR3 m_RotSpeed;          // ��]��
	int         m_nRotPattern;       // ��]�̃p�^�[��
	D3DXVECTOR3 m_Move;              // �ړ���
	D3DXVECTOR3 m_ChangeMove;        // �ړ��ʂ𖈃t���[���ǂꂭ�炢�ω������邩
	D3DXCOLOR   m_ChangeCol;         // �F�𖈃t���[���ǂꂭ�炢�ω������邩
	float       m_fChangeHeight;     // �����𖈃t���[���ǂꂭ�炢�ω������邩
	float       m_fChangeRadius;     // ���a���ǂꂭ�炢���t���[���ω������邩
	float       m_fChangeDiffusion;  // �����O�̕����ǂꂭ�炢���t���[���ω������邩
	bool        m_bCulling;          // �J�����O���邩�ǂ���
	bool        m_bDrawAddtive;      // ���Z�����ŕ`�悷�邩���Ȃ���
};


//*****************************************************************************
//     �G�t�F�N�g�Ǌ��N���X�̒�`
//*****************************************************************************
class CEffectManager
{
public:   // �N�ł��A�N�Z�X�\
	CEffectManager();
	~CEffectManager();

	static CEffectManager *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CEmitter *SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);

	void SetFileName(char *pFileName);
	void SetEmitter(CEmitter *pEmitter);
	void SetEmitterData(CEmitterData *pEmitterData, int nIdx);
	void SetParData(CParData *pParData, int nIdx);
	void SetRingEffectData(CRingData *pRingData, int nIdx);
	void SetTexManager(CTextureManager *pTexManager);
	void SetNumEmitterData(int nNumEmitterData);
	void SetNumParData(int nNumParData);
	void SetNumRingEffectData(int nNumRingEffectData);

	LPDIRECT3DTEXTURE9 BindTexture(int nIdx);
	CEmitter *GetEmitter(void);
	CEmitterData *GetEmitterData(int nIdx);
	CParData *GetParData(int nIdx);
	CRingData *GetRingEffectData(int nIdx);
	CTextureManager *GetTexManager(void);
	int GetNumEmitterData(void);
	int GetNumParData(void);
	int GetNumRingEffectData(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Save(void);
	void SaveTextureData(FILE *pFile);
	void SaveEmitterData(FILE *pFile);
	void SaveParticleData(FILE *pFile);
	void SaveRingEffectData(FILE *pFile);

	CEmitterData *ReadEmitterData(char *pLine, char *pStrCur, FILE *pFile, int nCntEmitter);
	CParData *ReadParticleData(char *pLine, char *pStrCur, FILE *pFile, int nCntParData);
	CRingData *ReadRingEffectData(char *pLine, char *pStrCur, FILE *pFile, int nCntRingData);

	char            m_aFileName[256];                  // �X�N���v�g�t�@�C����
	int             m_nNumEmitterData;                 // �G�~�b�^�f�[�^�̌�
	int             m_nNumParData;                     // �p�[�e�B�N���f�[�^�̌�
	int             m_nNumRingEffectData;              // �����O�G�t�F�N�g�f�[�^�̌�
	CEmitter        *m_pEmitter;                       // ���ݐ�������Ă���G�~�b�^�ւ̃|�C���^
	CEmitterData    *m_apEmitterData[MAX_EMMITER];     // �G�~�b�^�f�[�^�N���X�ւ̃|�C���^
	CParData        *m_apParData[MAX_PARTICLEDATA];    // �p�[�e�B�N���f�[�^�N���X�ւ̃|�C���^
	CRingData       *m_apRingData[MAX_RINGEFFECTDATA]; // �����O�G�t�F�N�g�f�[�^�N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;                // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
};

#endif