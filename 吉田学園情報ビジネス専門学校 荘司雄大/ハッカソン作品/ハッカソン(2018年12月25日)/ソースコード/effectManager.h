//*****************************************************************************
//
//     �G�t�F�N�g�Ǌ��̏���[effectManager.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _EFFECTMANAGER_H_
#define _EFFECTMANAGER_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define MAX_EMMITER       (30)      // �G�~�b�^�f�[�^�̍ő吔
#define MAX_PARTICLEDATA  (100)     // �p�[�e�B�N���f�[�^�̍ő吔

//*****************************************************************************
//     �O���錾
//*****************************************************************************
class CParData;
class CEmitterData;
class CTextureManager;
class CEmitter;

//*****************************************************************************
//     �G�t�F�N�g�}�l�[�W���[�N���X�̒�`
//*****************************************************************************
class CEffectManager : CScene
{
public:   // �N�ł��A�N�Z�X�\
	typedef enum
	{
		EFFECT_TYPE_DASH = 0,
		EFFECT_TYPE_DAMAGE,
		EFFECT_TYPE_PRESENT,
		EFFECT_TYPE_BREAK,
		EFFECT_TYPE_MAX,
	}EFFECT_TYPE;

	CEffectManager(int nPriority = 3, OBJTYPE objType = OBJTYPE_EFFECTMANAGER);
	~CEffectManager();

	static CEffectManager *Create(char *pFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nPriority = 3);

	void SetFileName(char *pFileName);
	void SetEmitter(CEmitter *pEmitter);
	void SetParData(CParData *pParData, int nIdx);
	void SetEmitterData(CEmitterData *pEmitterData, int nIdx);
	void SetTexManager(CTextureManager *pTexManager);
	void SetNumEmitterData(int nNumEmitterData);
	void SetNumParData(int nNumParData);


	LPDIRECT3DTEXTURE9 BindTexture(int nIdx);
	CEmitter *GetEmitter(void);
	CParData *GetParData(int nIdx);
	CEmitterData *GetEmitterData(int nIdx);
	CTextureManager *GetTexManager(void);
	int GetNumEmitterData(void);
	int GetNumParData(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Save(void);
	void SaveTextureData(FILE *pFile);
	void SaveEmitterData(FILE *pFile);
	void SaveParticleData(FILE *pFile);

	CEmitterData *ReadEmitterData(char *pLine, char *pStrCur, FILE *pFile, int nCntEmitter);
	CParData *ReadParticleData(char *pLine, char *pStrCur, FILE *pFile, int nCntParData);

	char            m_aFileName[256];               // �X�N���v�g�t�@�C����
	int             m_nNumEmitterData;              // �G�~�b�^�f�[�^�̌�
	int             m_nNumParData;                  // �p�[�e�B�N���f�[�^�̌�
	CEmitter        *m_pEmitter;                    // ���ݐ�������Ă���G�~�b�^�ւ̃|�C���^
	CEmitterData    *m_apEmitterData[MAX_EMMITER];  // �G�~�b�^�f�[�^�N���X�ւ̃|�C���^
	CParData        *m_apParData[MAX_PARTICLEDATA]; // �p�[�e�B�N���f�[�^�N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;             // �e�N�X�`���Ǘ��N���X�ւ̃|�C���^
};

#endif