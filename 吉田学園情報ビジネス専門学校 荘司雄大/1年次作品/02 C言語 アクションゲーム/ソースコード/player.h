//=============================================================================
//
// �v���C���[�̕\������ [player.h]
// Author : Takahiro Syoji
//
//=============================================================================
#include "main.h"
#ifndef _PLAYER_H_
#define _PLAYER_H_

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_PLAYER	(2)	// �v���C���[�̍ő吔

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
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	float fWidth;
	float fHeight;
	int nType;
	bool bUse;
	PLAYERSTATE state;	//�v���C���[�̏��
	int nCounterState;	//��ԊǗ��̃J�E���^�[
	int nLife;			//�̗�
	int DirectionMove;
	BULLETKIND BulletKind;
	bool bDisp;
	bool bJump;
	//Block *pBlock;
}Player;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
void HitPlayer(int nDmage);
Player *GetPlayer(void);
#endif
