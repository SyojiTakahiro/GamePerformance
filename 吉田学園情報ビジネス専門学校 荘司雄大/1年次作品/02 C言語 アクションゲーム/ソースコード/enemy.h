//=============================================================================
//
// �G���� [enemy.h]
// Author :Takahiro
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_ENEMY	(256)	// �G�̍ő吔

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	ENEMYSTATE_NORMAL = 0,
	ENEMYSTATE_DAMAGE,
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	float fWidth;
	float fHeight;
	int nType;
	bool bUse;
	D3DXCOLOR color;
	ENEMYSTATE state;	//�G�̏��
	int nCounterState;	//��ԊǗ��̃J�E���^�[
	int nLife;			//�̗�
}ENEMY;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);
void SetEnemy(D3DXVECTOR3 pos, int nType);
void HitEnemy(int nCntEnemy, int nDmage);
bool CollisionEnemy(D3DXVECTOR3 *pePos, D3DXVECTOR3 *pePosOld, float fWidth, float fHeight);
ENEMY *GetEnemy(void);

#endif

