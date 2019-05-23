//*****************************************************************************
//
//     �L�����N�^�[�̏���[character.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

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
class CModel;
class CMotionManager;
class CModelManager;
class CTextureManager;

//*****************************************************************************
//    �L�����N�^�[�N���X�̒�`
//*****************************************************************************
class CCharacter : public CScene
{
public:    // �N�ł��A�N�Z�X�\
	CCharacter(int nPriority = 3,OBJTYPE objType = OBJTYPE_CHARACTER);
	~CCharacter();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetMtxWorld(LPDIRECT3DDEVICE9 pDevice);

	void SetNumParts(const int nNumParts);
	void SetModel(CModel **pModel);
	void SetModel(CModel *pModel, const int nIdx);
	void SetMotionManager(CMotionManager *pMotionManager);
	void SetTextureManager(CTextureManager *pTextureManager);
	void SetMtxWorld(const D3DXMATRIX mtxWorld);
	void SetPos(const D3DXVECTOR3 pos);
	void SetPosOld(const D3DXVECTOR3 posOld);
	void SetRot(const D3DXVECTOR3 rot);
	void SetDestRot(const D3DXVECTOR3 Destrot);
	void SetDiffRot(const D3DXVECTOR3 Diffrot);
	void SetGravity(const float fGravity);
	void SetColHeight(const float fColHeight);
	void SetColRange(const float fColRange);
	void SetLand(const bool bLand);
	void SetLandScene(CScene *pLandScene);
	void SetLandOffsetPos(const D3DXVECTOR3 LandOffsetPos);
	void SetLandOffsetAdd(const D3DXVECTOR3 LandOffsetAdd);
	void SetLandOffsetRot(const D3DXVECTOR3 LandOffsetRot);
	void SetLandOffsetRotStart(const D3DXVECTOR3 LandOffsetRotStart);

	int GetNumParts(void);
	CModel **GetModel(void);
	CModel *GetModel(const int nIdx);
	CMotionManager *GetMotionManager(void);
	CTextureManager *GetTextureManager(void);
	D3DXMATRIX GetMtxWorld(void);
	D3DXVECTOR3 GetPos(void);
	D3DXVECTOR3 GetPosOld(void);
	D3DXVECTOR3 GetRot(void);
	D3DXVECTOR3 GetDestRot(void);
	D3DXVECTOR3 GetDiffRot(void);
	float GetGravity(void);
	float GetColHeight(void);
	float GetColRange(void);
	bool GetLand(void);
	CScene *GetLandScene(void);
	D3DXVECTOR3 GetLandOffsetPos(void);
	D3DXVECTOR3 GetLandOffsetAdd(void);
	D3DXVECTOR3 GetLandOffsetRot(void);
	D3DXVECTOR3 GetLandOffsetRotStart(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	int             m_nNumParts;             // �g�p����p�[�c��
	CModel          **m_apModel;             // ���f���N���X�ւ̃|�C���^
	CMotionManager  *m_pMotionManager;       // ���[�V�����N���X�ւ̃|�C���^
	CTextureManager *m_pTextureManager;      // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	D3DXMATRIX      m_MtxWorld;              // ���[���h�}�g���b�N�X
	D3DXVECTOR3     m_Pos;                   // ���݂̍��W
	D3DXVECTOR3     m_PosOld;                // �O��̍��W
	D3DXVECTOR3     m_Rot;                   // ���݂̌���
	D3DXVECTOR3     m_DestRot;               // �ړI�̌���
	D3DXVECTOR3     m_DiffRot;               // �ړI�̌����ƌ��݂̌����̍���
	float           m_fGravity;              // �d��
	float           m_fColHeight;            // �����蔻����Ƃ鍂��
	float           m_fColRange;             // �����蔻������͈�
	CScene          *m_pLandScene;           // ����Ă���I�u�W�F�N�g�ւ̃|�C���^
	bool            m_bLand;                 // �O��I�u�W�F�N�g�ɏ���Ă������ǂ���
	D3DXVECTOR3     m_LandOffsetPos;         // �I�u�W�F�N�g����̃I�t�Z�b�g����
	D3DXVECTOR3     m_LandOffsetAdd;         // �I�t�Z�b�g�����ɉ�����l
	D3DXVECTOR3     m_LandOffsetRot;         // �I�u�W�F�N�g����̃I�t�Z�b�g����
	D3DXVECTOR3     m_LandOffsetRotStart;    // ����Ă���̌���
};

#endif