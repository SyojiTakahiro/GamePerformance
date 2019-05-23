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
//    �}�N����`
//*****************************************************************************
#define PARTICLEDATA_FILENAME     "data/TEXT/EFFECT/particle.txt"           // �p�[�e�B�N���f�[�^���i�[���ꂽ�t�@�C���̃p�X��
#define PARTICLE_TEXTURE_FILENAME "data/TEXT/EFFECT/particle_texture.txt"   // �p�[�e�B�N���Ŏg�p����e�N�X�`���̃t�@�C���p�X��

//*****************************************************************************
//    �O���錾(���̂��Ȃ��錾)
//*****************************************************************************

//*****************************************************************************
//    �p�[�e�B�N���p�f�[�^�N���X�̍쐬
//*****************************************************************************
class CParticleData
{
public:     // �N�ł��A�N�Z�X�\
	CParticleData();
	~CParticleData();

	void SetMaxMove(D3DXVECTOR3 maxMove) { m_MaxMove = maxMove; };
	void SetMinMove(D3DXVECTOR3 minMove) { m_MinMove = minMove; };
	void SetInitCol(D3DXCOLOR InitCol) { m_InitCol = InitCol; };
	void SetChangeCol(D3DXCOLOR ChangeCol) { m_ChangeCol = ChangeCol; };
	void SetMinRadius(float fMinRadius) { m_fMinRadius = fMinRadius; };
	void SetMaxRadius(float fMaxRadius) { m_fMaxRadius = fMaxRadius; };
	void SetChangeRadius(float fChangeRadius) { m_fChangeRadius = fChangeRadius; };
	void SetRotSpeed(float fRotSpeed) { m_fRotSpeed = fRotSpeed; };
	void SetSpread(float fSpread) { m_fSpread = m_fSpread; };
	void SetGravity(float fGravity) { m_fGravity = fGravity; };
	void SetLap(int nLap) { m_nLap = nLap; };
	void SetLife(int nLife) { m_nLife = nLife; };
	void SetAppear(int nAppear) { m_nAppear = nAppear; };
	void SetMinLife(int nMinLife) { m_nMinLife = nMinLife; };
	void SetMaxLife(int nMaxLife) { m_nMaxLife = nMaxLife; };
	void SetTexIdx(int nTexIdx) { m_nTexIdx = nTexIdx; };
	void SetRotPattern(int nRotPattern) { m_nRotPattern = nRotPattern; };
	void SetLoop(bool bLoop) { m_bLoop = bLoop; };
	void SetBouncing(bool bBouncing) { m_bBouncing = bBouncing; };
	void SetDrawAddtive(bool bDrawAddtive) { m_bDrawAddtive = bDrawAddtive; };

	D3DXVECTOR3 GetMaxMove(void) { return m_MaxMove; };
	D3DXVECTOR3 GetMinMove(void) { return m_MinMove; };
	D3DXCOLOR GetInitCol(void) { return m_InitCol; };
	D3DXCOLOR GetChangeCol(void) { return m_ChangeCol; };
	float GetMinRadius(void) { return m_fMinRadius; };
	float GetMaxRadius(void) { return m_fMaxRadius; };
	float GetChangeRadius(void) { return m_fChangeRadius; };
	float GetRotSpeed(void) { return m_fRotSpeed; };
	float GetSpread(void) { return m_fSpread; };
	float GetGravity(void) { return m_fGravity; };
	int GetLap(void) { return m_nLap; };
	int GetLife(void) { return m_nLife; };
	int GetAppear(void) { return m_nAppear; };
	int GetMinLife(void) { return m_nMinLife; };
	int GetMaxLife(void) { return m_nMaxLife; };
	int GetTexIdx(void) { return m_nTexIdx; };
	int GetRotPattern(void) { return m_nRotPattern; };
	bool GetLoop(void) { return m_bLoop; };
	bool GetBouncing(void) { return m_bBouncing; };
	bool GetDrawAddtive(void) { return m_bDrawAddtive; };

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	D3DXVECTOR3   m_MaxMove;         // �ړ��ʂ̍ő�l
	D3DXVECTOR3   m_MinMove;         // �ړ��ʂ̍ŏ��l
	D3DXCOLOR     m_InitCol;         // �����l�̐F
	D3DXCOLOR     m_ChangeCol;       // ���t���[�����Z����F�̕ω���
	float         m_fMinRadius;      // �������̔��a�ŏ��l
	float         m_fMaxRadius;      // �������̔��a�ő�l
	float         m_fChangeRadius;   // ���t���[�����Z����|���S���̔��a
	float         m_fRotSpeed;       // ���t���[�����Z����|���S���̉�]��
	float         m_fSpread;         // ���t���[�����Z����L����
	float         m_fGravity;        // ���t���[�����Z����d��
	int           m_nLap;            // �P�t���[���ɐ�������|���S����
	int           m_nLife;			 // �G�~�b�^�̎���
	int           m_nAppear;         // �p�[�e�B�N���𐶐�����Ԋu
	int           m_nMinLife;		 // �\�����Ԃ̍ŏ��l
	int           m_nMaxLife;		 // �\�����Ԃ̍ő�l
	int           m_nTexIdx;         // �g�p����e�N�X�`���̔ԍ�
	int           m_nRotPattern;     // ��]�̎�ޕ����p[ 0:�Ȃ��@1:���v���@2:�����v��� 3:�����_�� ]
	bool          m_bLoop;           // ���[�v���邩���Ȃ���[ 0�Ȃ烋�[�v���Ȃ� / 1�Ȃ烋�[�v���� ]
	bool          m_bBouncing;		 // �o�E���h�����邩[ 0:�Ȃ� 1:���� ]
	bool          m_bDrawAddtive;	 // ���Z�������ǂ���[ 0:�Ȃ� 1:���� ]
};

//*****************************************************************************
//    �p�[�e�B�N���}�l�[�W���[�N���X�̍쐬
//*****************************************************************************
class CParticleManager : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	CParticleManager(int nPriority = 3, OBJTYPE objType = OBJTYPE_PARTICLEMANAGER);
	~CParticleManager();

	static CParticleManager *Create(char *pParticleFileName, char *pTextureFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	CParticleData *GetParticleDate(int nIdx);
	LPDIRECT3DTEXTURE9 GetTexture(int nIdx);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	HRESULT ParticleDataLoad(void);
	HRESULT TextureLoad(void);
	void ReleaseParticleData(void);
	void ReleaseTexture(void);

	CParticleData      **m_pParticleData;          // �p�[�e�B�N���f�[�^�N���X�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 *m_pTexture;                // �e�N�X�`���ւ̃|�C���^
	char               m_aParticleFileName[256];   // �p�[�e�B�N���f�[�^���i�[���ꂽ�t�@�C���̃p�X��
	char               m_aTextureFileName[256];    // �p�[�e�B�N���Ŏg�p����e�N�X�`����񂪊i�[���ꂽ�t�@�C���̃p�X��
	int                m_nNumEmitter;              // �p�[�e�B�N���̎��
	int                m_nNumTexture;              // �ǂݍ��ރe�N�X�`���̐�
};

//*****************************************************************************
//    �p�[�e�B�N���G�~�b�^�N���X�̍쐬
//*****************************************************************************
class CParEmitter : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	CParEmitter(int nPriority = 3,OBJTYPE objType = OBJTYPE_PARTICLEEMITTER);
	~CParEmitter();

	static CParEmitter *Create(D3DXVECTOR3 pos, int nType, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetType(int nType);
	void SetPos(D3DXVECTOR3 pos);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	void SetParticle(void);

	LPDIRECT3DTEXTURE9 m_pTexture;        // �p�[�e�B�N���ɐݒ肷��e�N�X�`��
	D3DXVECTOR3        m_Pos;             // �G�~�b�^�̍��W
	CParticleData      *m_pParticleData;  // �p�[�e�B�N���f�[�^�N���X�ւ̃|�C���^
	int			       m_nLife;			  // �G�~�b�^�̎���
	int			       m_nTexture;	      // �e�N�X�`���̎��
	int			       m_nAppear;         // ���o����Ԋu
	int			       m_nLap;            // 1��̕��o�ŏo���p�[�e�B�N���̌�
	int                m_nType;           // ���o����p�[�e�B�N���̎��
	int                m_nCntEmitter;     // ���o����^�C�~���O���Ǘ�����J�E���^�[
	bool		       m_bLoop;           // ���[�v�̗L��
};

//*****************************************************************************
//    �p�[�e�B�N���N���X�̍쐬
//*****************************************************************************
class CParticle : public CSceneBillboard
{
public:     // �N�ł��A�N�Z�X�\
	CParticle(int nPriority = 3, OBJTYPE objType = OBJTYPE_PARTICLE);
	~CParticle();

	static CParticle *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fAngle,
		                     D3DXCOLOR ChangeCol, float fGravity, float fChangeRadius,
		                     float fChangeRot, float fBouncing, int nLife, int nRotPattern,
		                     int nType, bool bBound, bool bDrawAddtive, int nPriority = 3);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fAngle,
		         D3DXCOLOR ChangeCol, float fGravity, float fChangeRadius,
		         float fChangeRot, float fBouncing, int nLife, int nRotPattern,
		         int nType, bool bBound, bool bDrawAddtive);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	D3DXCOLOR   m_ChangeCol;     // �ǂꂭ�炢�F���ω����邩
	D3DXVECTOR3 m_Move;          // �ړ���
	float       m_fGravity;      // �d��
	float       m_fChangeRadius; // �ǂꂭ�炢�ό`���邩
	float       m_fChangeRot;    // �ǂꂭ�炢��]���邩
	float       m_fBouncing;     // �n�ʂɂ����ۂɃo�E���h����l
	int         m_nLife;         // ����
	int         m_nRotPattern;   // ��]�̎��
	int         m_nType;         // ���
	bool        m_bBound;        // �o�E���h���邩�ǂ���
	bool        m_bDrawAddtive;  // ���Z�����ŕ`�悷�邩�ǂ���
};

#endif