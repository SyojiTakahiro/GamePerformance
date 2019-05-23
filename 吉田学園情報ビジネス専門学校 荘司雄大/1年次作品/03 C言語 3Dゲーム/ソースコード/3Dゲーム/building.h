//=============================================================================
//
// ���f������ [building.h]
// Author : 
//
//=============================================================================
#ifndef _BUILDING_H_
#define _BUILDING_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_NAME0		 "data//MODEL//P1.x"		//�e�N�X�`���̃t�@�C����
#define MODEL_NAME1		 "data//MODEL//P1.x"		//�e�N�X�`���̃t�@�C����
#define MODEL_NAME2		 "data//MODEL//box000.x"		//�e�N�X�`���̃t�@�C����
#define MOVE_MODEL			(2.0f)							//���f���ړ���
#define MOVE_SIN_COS_TOP	(0.75f)							//�|���S���ړ���
#define MOVE_SIN_COS_BOT	(0.25f)							//�|���S���ړ���
#define FIELD_MAX_SIZE		(FIELD_SIZE - 10)
#define MAX_MODEL			(256)
#define MAX_MODEL_TYPE		(3)								//���f����ނ̍ő吔
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 VtxMinBuilding, VtxMaxBuilding;	//���f���̍ŏ��l�A�ő�l
} VTX;
typedef struct
{
	DWORD nNumMat = 0;						//�}�e���A�����̐�
	D3DXVECTOR3 pos;						//�ʒu
	D3DXVECTOR3 rot;						//����
	D3DXMATRIX	mtxWorld;					//���[���h�}�g���b�N�X
	float fShadow;							//�e�̑傫��
	int nIdxShadow;							//�e�̃C���f�b�N�X
	int nType;								//���f���̎��
	bool bUse;								//�g�p���Ă��邩
	D3DXVECTOR3 VtxMinBuilding, VtxMaxBuilding;	//���f���̍ŏ��l�A�ő�l
} BUILDING;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitBuilding(void);
void UninitBuilding(void);
void UpdateBuilding(void);
void DrawBuilding(void);
BUILDING *GetBuilding(void);
bool CollisionBuilding(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pBuilding*/);
void SetBuilding(D3DXVECTOR3 pos, int nType);
#endif