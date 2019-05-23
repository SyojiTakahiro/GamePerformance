//*****************************************************************************
//
//     �}�b�v�̏���[map.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MAP_H_
#define _MAP_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MAX_WATER          (10)          // �z�u�ł��鐅�̍ő吔

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CField;
class CWater;
class CModelManager;
class CCylinder;
class CDome;

//*****************************************************************************
//     �}�b�v�N���X�̒�`
//*****************************************************************************
class CMap
{
public:    // �N�ł��A�N�Z�X�\
	CMap();
	~CMap();

	static CMap *Create(char *aFileName);

	void Init(char *aFileName);
	void Uninit(void);
	void SetObjectDefault(void);

	char *GetMapName(void);

	D3DXVECTOR3 GetDefaultPlayerPos(void);
	D3DXVECTOR3 GetDefaultPlayerRot(void);
	D3DXVECTOR3 GetDefaultCameraRot(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void FieldLoad(char *aFieldFileName);
	void WaterLoad(char *aWaterFileName);
	void ObjectLoad(char *aObjectFileName);
	void MountainLoad(char *aMountainFileName);
	void SkyLoad(char *aSkyFileName);

	CModelManager  *m_pModelManager;                         // ���f���Ǌ��N���X�ւ̃|�C���^
	CField         *m_pField;                                // �n�ʃN���X�ւ̃|�C���^
	CWater         *m_pWater[MAX_WATER];                     // ���N���X�ւ̃|�C���^
	CCylinder      *m_pMoutain;                              // �~���N���X�ւ̃|�C���^
	CDome          *m_pSky;                                  // �����N���X�ւ̃|�C���^
	char           m_aFileName[256];                         // �ǂݍ��ރX�N���v�g�t�@�C����
	D3DXVECTOR3    m_DefaultPlayerPos;                       // �v���C���[�̏������W
	D3DXVECTOR3    m_DefaultPlayerRot;                       // �v���C���[�̏�������
	D3DXVECTOR3    m_DefaultCameraRot;                       // �J�����̏�������
	char           m_aObjectFileName[256];                   // �I�u�W�F�N�g�f�[�^�̃X�N���v�g�t�@�C����
};

#endif