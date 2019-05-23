//=============================================================================
//
// ���f������ [player.h]
// Author : 
//
//=============================================================================
#ifndef _CPU_H_
#define _CPU_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_CPU	(1)	// �v���C���[�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	CPUSTATE_APPEAR = 0,
	CPUSTATE_NORMAL,
	CPUSTATE_DAMAGE,
	CPUSTATE_DEATH,
	CPUSTATE_MAX
}CPUSTATE;

typedef enum
{//�e
	CPUBULLETKIND_NOMAL = 0,	//�ʏ���
	CPUBULLETKIND_DIFFUSION,	//�g�U
	CPUBULLETKIND_MAX
} CPUBULLETKIND;

typedef struct
{
	LPD3DXMESH pMesh;
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nIdxModelParent;
}CPUMODEL;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;					//����
	int nType;
	bool bUse;
	CPUSTATE state;	//�v���C���[�̏��
	int nCounterState;	//��ԊǗ��̃J�E���^�[
	int nLife;			//�̗�
	int DirectionMove;
	CPUBULLETKIND BulletKind;
	bool bDisp;
	bool bJump;
	D3DXMATRIX mtxWorld;
	CPUMODEL aModel[2];
	D3DXVECTOR3 VtxMin, VtxMax;	//���f���̍ŏ��l�A�ő�l
}CPU;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCPU(void);
void UninitCPU(void);
void UpdateCPU(void);
void DrawCPU(void);
CPU *GetCPU(void);
bool CollisionCPU(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pModel*/);


#endif