//=============================================================================
//
// �|�[�Y���j���[���� [pauseselect.h]
// Author : 
//
//=============================================================================
#ifndef _MODESELECT_H_
#define _MODESELECT_H_

#include "main.h"

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef enum
{
	GAMEMODESELECT_NONE = 0,
	GAMEMODESELECT_SINGLEMODE,		//���s���
	GAMEMODESELECT_2PMODE,			//���g���C���
	GAMEMODESELECT_TUTORIAL,			//�I��
	GAMEMODESELECT_MAX				//��Ԃ̑���
}GAMEMODESELECT;						//�񋓌^

typedef enum
{
	SELECTGAMETYPE_NONE = 0,	//�I�΂�Ă��Ȃ����
	SELECTGAMETYPE_SELECT,		//�I�΂�Ă���	���
	SELECTGAMETYPE_MAX				//��Ԃ̑���
}SELECTGAME;

typedef struct
{
	SELECTGAME	selectgame;		//�Z���N�g
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR	col;		//�J���[
}MODESELECT;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitModeSelect(void);
void UninitModeSelect(void);
void UpdateModeSelect(void);
void DrawModeSelect(void);
void SetModeSelect(D3DXVECTOR3 pos, D3DXCOLOR col);
GAMEMODESELECT *GetModeMode(void);
#endif