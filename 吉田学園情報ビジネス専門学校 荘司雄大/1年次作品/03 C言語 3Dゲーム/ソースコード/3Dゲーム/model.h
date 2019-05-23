//=============================================================================
//
// ���f������ [model.h]
// Author : 
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "polygon.h"
#include "player.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_NAME000		 "data\\MODEL\\P1.x"		//�e�N�X�`���̃t�@�C����
#define MODEL_NAME001		 "data\\MODEL\\P2.x"		//�e�N�X�`���̃t�@�C����
#define MODEL_NAME002		 "data\\MODEL\\box000.x"		//�e�N�X�`���̃t�@�C����

#define MOVE_MODEL1			(2.0f)							//���f���ړ���
#define MOVE_SIN_COS_TOP1	(0.75f)							//�|���S���ړ���
#define MOVE_SIN_COS_BOT1	(0.25f)							//�|���S���ړ���
#define FIELD_MAX_SIZE1		(FIELD_SIZE - 10)
#define MAX_MODEL1			(300)							//���f���z�u�̍ő吔
#define MAX_MODEL_TYPE1		(2)								//���f����ނ̍ő吔
//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 VtxMinModel, VtxMaxModel;	//���f���̍ŏ��l�A�ő�l
	D3DXVECTOR3 VtxMinEnemy, VtxMaxEnemy;	//���f���̍ŏ��l�A�ő�l

} VTX1;
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
	D3DXVECTOR3 VtxMinModel, VtxMaxModel;	//���f���̍ŏ��l�A�ő�l
} MODEL1;

typedef enum
{
	COLLISION_PLAYER = 0,	//�v���C���[
	COLLISION_ENEMY,
	COLLISION_BULLET,		//�e
	COLLISION_ITEM,
	COLLISION_MAX,
} COLLISIONMODELTYPE;
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);
MODEL1 *GetModel(void);
bool CollisionModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin, COLLISIONMODELTYPE Type);
void SetModel(D3DXVECTOR3 pos,int nType);
#endif