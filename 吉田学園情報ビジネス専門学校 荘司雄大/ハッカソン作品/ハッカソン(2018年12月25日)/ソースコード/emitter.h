//*****************************************************************************
//
//     �G�~�b�^�̏���[emitter.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EMITTER_H_
#define _EMITTER_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     �}�N���錾
//*****************************************************************************
#define EMITTER_MOVEMENT   (6.0f)    // �G�~�b�^�̈ړ���
#define EMITTER_ROTATION   (0.2f)    // �G�~�b�^�̉�]��

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CParData;

//*****************************************************************************
//     �G�~�b�^�f�[�^�N���X�̒�`
//*****************************************************************************
class CEmitterData
{
public:    // �N�ł��A�N�Z�X�\
	CEmitterData();
	~CEmitterData();

	void SetTexIdx(const int nTexIdx) { m_nTexIdx = nTexIdx; }
	void SetType(const int nType) { m_nType = nType; }
	void SetRangeMax(const float fRangeMax) { m_fRangeMax = fRangeMax; }
	void SetRangeMin(const float fRangeMin) { m_fRangeMin = fRangeMin; }
	void SetLife(const int nLife) { m_nLife = nLife; }
	void SetLap(const int nLap) { m_nLap = nLap; }
	void SetAppear(const int nAppear) { m_nAppear = nAppear; }
	void SetLoop(const bool bLoop) { m_bLoop = bLoop; }

	int GetTexIdx(void) { return m_nTexIdx; }
	int GetType(void) { return m_nType; }
	float GetRangeMax(void) { return m_fRangeMax; }
	float GetRangeMin(void) { return m_fRangeMin; }
	int GetLife(void) { return m_nLife; }
	int GetLap(void) { return m_nLap; }
	int GetAppear(void) { return m_nAppear; }
	bool GetLoop(void) { return m_bLoop; }

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int   m_nTexIdx;     // �g�p����e�N�X�`���̔ԍ�
	int   m_nType;       // �g�p����G�t�F�N�g�̔ԍ�
	float m_fRangeMax;   // �G�t�F�N�g�𐶐�����͈͂̍ő�l
	float m_fRangeMin;   // �G�t�F�N�g�𐶐�����͈͂̍ŏ��l
	int   m_nLife;       // ����
	int   m_nLap;        // 1��̕��o�ŏo���G�t�F�N�g�̌�
	int   m_nAppear;     // �G�t�F�N�g���o���Ԋu
	bool  m_bLoop;       // ���[�v���邩���Ȃ���
};

//*****************************************************************************
//     �G�~�b�^�N���X�̒�`
//*****************************************************************************
class CEmitter : CScene
{
public:    // �N�ł��A�N�Z�X�\
	CEmitter(int nPriority = 3, OBJTYPE objType = OBJTYPE_EMITTER);
	~CEmitter();

	static CEmitter *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTexIdx, int nType, float fRangeMax, float fRangeMin, int nLife, int nLap, int nAppear, bool bLoop, CParData *pParData, LPDIRECT3DTEXTURE9 pTexture, int nPriority = 3);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetParData(CParData *pParData);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);
	void SetPos(const D3DXVECTOR3 pos);
	void SetRot(const D3DXVECTOR3 rot);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetTexIdx(const int nTexIdx);
	void SetType(const int nType);
	void SetRangeMax(const float fRangeMax);
	void SetRangeMin(const float fRangeMin);
	void SetLife(const int nLife);
	void SetLap(const int nLap);
	void SetAppear(const int nAppear);
	void SetLoop(const bool bLoop);

	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxWorld(void);
	int GetTexIdx(void);
	int GetType(void);
	float GetRangeMax(void);
	float GetRangeMin(void);
	int GetLife(void);
	int GetLap(void);
	int GetAppear(void);
	bool GetLoop(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Movement(void);
	void SetParticle(void);

	CParData           *m_pParData;  // �p�[�e�B�N���f�[�^�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTexture;   // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3        m_Pos;        // ���W
	D3DXVECTOR3        m_Rot;        // ����
	D3DXMATRIX         m_MtxWorld;   // ���[���h�}�g���b�N�X
	int                m_nTexIdx;    // �g�p����e�N�X�`���ԍ�
	int                m_nType;      // ���
	float              m_fRangeMax;  // �G�t�F�N�g�𐶐�����͈͂̍ő�l
	float              m_fRangeMin;  // �G�t�F�N�g�𐶐�����͈͂̍ŏ��l
	int                m_nLife;      // �G�~�b�^�̎���
	int                m_nLap;       // 1��̕��o�^�C�~���O�ɏo���G�t�F�N�g�̌�
	int                m_nAppear;    // ���o����Ԋu
	int                m_nCounter;   // ���o�^�C�~���O���Ǘ�����J�E���^�[
	bool               m_bLoop;      // ���[�v���邩���Ȃ���
};

#endif