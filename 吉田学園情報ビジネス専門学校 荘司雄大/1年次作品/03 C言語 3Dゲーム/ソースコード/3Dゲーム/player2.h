//=============================================================================
//
// ���f������ [PLAYER2.h]
// Author : 
//
//=============================================================================
#ifndef _PLAYER2_H_
#define _PLAYER2_H_

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
	PLAYER2STATE_APPEAR = 0,
	PLAYER2STATE_NORMAL,
	PLAYER2STATE_DAMAGE,
	PLAYER2STATE_DEATH,
	PLAYER2STATE_MAX
}PLAYER2STATE;

typedef struct
{
	LPD3DXMESH pMesh;
	LPD3DXBUFFER pBuffMat;
	DWORD nNumMat;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nIdxModelParent;
}MODEL2;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;					//����
	float fWidth;
	float fHeight;
	int nType;
	bool bUse;
	PLAYER2STATE state;	//�v���C���[�̏��
	int nCounterState;	//��ԊǗ��̃J�E���^�[
	int nLife;			//�̗�
	int DirectionMove;
	D3DXMATRIX mtxWorld;
	bool bDisp;
	bool bJump;
	MODEL2 aModel[2];
	D3DXVECTOR3 VtxMin, VtxMax;	//���f���̍ŏ��l�A�ő�l
}PLAYER2;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitPlayer2(void);
void UninitPlayer2(void);
void UpdatePlayer2(void);
void DrawPlayer2(void);
PLAYER2 *GetPlayer2(void);
bool CollisionPlayer2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pModel*/);

#endif
