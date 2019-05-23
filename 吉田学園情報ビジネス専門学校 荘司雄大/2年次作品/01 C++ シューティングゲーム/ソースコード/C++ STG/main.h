//=============================================================================
//
// ���C������ [main.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#include <windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"					//���͏����ɕK�v
#include "xaudio2.h"

class CRenderer;					//�O���錾
class CScene;						//�O���錾

//*****************************************************************************
// ���C�u�����t�@�C���̃����N
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	//DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")	//�V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")	//���͏����ɕK�v

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH	(1280)		// �E�C���h�E�̕�
#define SCREEN_HEIGHT	(720)		// �E�C���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

//�|���S���̍ő吔
#define MAX_SCENE			(300)

#define MAX_PRI				(8)

//�����̍ő吔
#define	MAX_EXPLOSION		(256)

//�e�N�X�`��
#define PLAYER_TEXTURE		"data/TEXTURE/Player001.png"
#define EXPLOSION_TEXTURE	"data/TEXTURE/explosion001.png"
#define EFFECT_TEXTURE		"data/TEXTURE/effect000.jpg"
#define NUMBER_TEXTURE		"data/TEXTURE/number000.png"

//�e
#define BULLET_TEXTURE		"data/TEXTURE/BULLET.png"
#define SLASH_TEXTURE		"data/TEXTURE/BULLET.png"
#define SPBULLET_TEXTURE	"data/TEXTURE/SPBULLET.png"
#define EXBULLET_TEXTURE	"data/TEXTURE/s_kama4.png"

//�w�i�e�N�X�`��
#define MAX_BG				(3)
#define MAX_GAME_BG			(4)

#define TITLE_TEXTURE0		"data/TEXTURE/title000.jpg"
#define TITLE_TEXTURE1		"data/TEXTURE/title001.png"
#define TITLE_TEXTURE2		"data/TEXTURE/titlelogo.png"

#define BG_TEXTURE0			"data/TEXTURE/BG1000.jpg"
#define BG_TEXTURE1			"data/TEXTURE/bg101.png"
#define BG_TEXTURE2			"data/TEXTURE/bg102.png"
#define BG_TEXTURE3			"data/TEXTURE/bg_bar.png"

#define CLEAR_TEXTURE0		"data/TEXTURE/BG_CLEAR.jpg"
#define CLEAR_TEXTURE1		"data/TEXTURE/cloud-2.png"
#define CLEAR_TEXTURE2		"data/TEXTURE/gameclear.png"

#define GAMEOVER_TEXTURE0	"data/TEXTURE/BG_GAMEOVER.jpg"
#define GAMEOVER_TEXTURE1	"data/TEXTURE/cloud-2.png"
#define GAMEOVER_TEXTURE2	"data/TEXTURE/gameover.png"

//���C�t�Q�[�W
#define MAX_GAUGE			(3)
#define GAUGE_TEXTURE0		"data/TEXTURE/GAUGE_BASE.png"
#define GAUGE_TEXTURE1		"data/TEXTURE/LIFE_GAUGE.png"
#define GAUGE_TEXTURE2		"data/TEXTURE/GAUGE_COVER.png"

#define GAUGE2_TEXTURE0		"data/TEXTURE/GAUGE2_BASE.png"
#define GAUGE2_TEXTURE1		"data/TEXTURE/LIFE_GAUGE2.png"
#define GAUGE2_TEXTURE2		"data/TEXTURE/GAUGE2_COVER.png"
//��
#define MAX_NUMBER			(9)

//�|�[�Y���j���[�̐�
#define MAX_PAUSEMENU		(3)

#define PAUSE_TEXTURE		"data\\TEXTURE\\pause100.png"	//�e�N�X�`���̃t�@�C����

#define	TEXTURE_CONTINUE	"data/TEXTURE/pause000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RETRY		"data/TEXTURE/pause001.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_QUIT		"data/TEXTURE/pause002.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
int GetFPS(void);
CScene **GetScene(void);

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
}VERTEX_2D;

#endif