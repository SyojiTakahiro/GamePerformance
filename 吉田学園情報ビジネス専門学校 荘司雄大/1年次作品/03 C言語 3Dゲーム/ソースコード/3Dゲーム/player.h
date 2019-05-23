//=============================================================================
//
// ���f������ [player.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_PLAYER	(1)	// �v���C���[�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	PLAYERSTATE_APPEAR = 0,
	PLAYERSTATE_NORMAL,
	PLAYERSTATE_DAMAGE,
	PLAYERSTATE_DEATH,
	PLAYERSTATE_MAX
}PLAYERSTATE;

typedef enum
{//�e
	BULLETKIND_NOMAL = 0,	//�ʏ���
	BULLETKIND_DIFFUSION,	//�g�U
	BULLETKIND_MAX
} BULLETKIND;

typedef struct
{
	LPD3DXMESH pMesh;
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nIdxModelParent;
}MODEL;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;					//����
	int nType;
	bool bUse;
	PLAYERSTATE state;	//�v���C���[�̏��
	int nCounterState;	//��ԊǗ��̃J�E���^�[
	int nLife;			//�̗�
	int DirectionMove;
	BULLETKIND BulletKind;
	bool bDisp;
	bool bJump;
	D3DXMATRIX mtxWorld;
	MODEL aModel[2];
	D3DXVECTOR3 VtxMin, VtxMax;	//���f���̍ŏ��l�A�ő�l
}PLAYER;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
bool CollisionPlayer(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pModel*/);


#endif
