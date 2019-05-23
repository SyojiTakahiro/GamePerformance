//*****************************************************************************
//
//     ���C���̏���[main.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _MAIN_H_
#define _MAIN_H_

//*****************************************************************************
//     �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS         // �W�����͌x���΍�}�N��
#include <stdio.h>                      // �W�����o�͏����ɕK�v
#include <stdlib.h>                     // �W���֐������ɕK�v
#include <stdarg.h>                     // �ψ������g�p���邽�߂ɕK�v
#include <windows.h>                    // Windows�W������
#include <time.h>                       // �����_���Ȓl���o�͂��邽�߂ɕK�v
#include "d3dx9.h"                      // DirectX(ver9.0)�̏����ɕK�v
#define DIRECTINPUT_VERSION (0x0800)	// �x���Ώ��p
#include "dinput.h"                     // ���͏����ɕK�v
#include "xaudio2.h"                    // �T�E���h�����ɕK�v
#include <XInput.h>                     // XInput�̏����ɕK�v
#include "resource.h"                   // ���s�t�@�C���̃f�t�H���g�l�̕ύX�Ɏg�p

#ifdef _DEBUG
// ���������[�N�`�F�b�N�ɕK�v
#include <crtdbg.h>
#define malloc(X) _malloc_dbg(X,_NORMAL_BLOCK,__FILE__,__LINE__)
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define _CRTDBG_MAP_ALLOC
#endif

//*****************************************************************************
//     ���C�u�����t�@�C���̃����N
//*****************************************************************************
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "XInput.lib")

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define SCREEN_WIDTH  (1280)   // �E�B���h�E�̕�
#define SCREEN_HEIGHT (720)    // �E�B���h�E�̍���
#define NUM_POLYGON   (2)      // �`�悷��|���S���̐�

// �Q�c�|���S�����_�t�H�[�}�b�g( ���_���W[2D] / ���_�J���[ / �e�N�X�`�����W )
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// �R�c�|���S�����_�t�H�[�}�b�g( ���_���W[3D] / �@���x�N�g�� / ���_�J���[ / �e�N�X�`�����W)
#define	FVF_VERTEX_3D	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// DirectX�ō쐬���ꂽ�|�C���^�̊J��(�}�N���Ŋy�ɂ��Ă���)
#define DIRECT_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

//*****************************************************************************
//     �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;  // ���_���W
	float rhw;        // ���_���W�ϊ��p(1.0f�ŌŒ�)
	D3DCOLOR col;     // ���_�J���[
	D3DXVECTOR2 tex;  // �e�N�X�`���摜
} VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;  // ���_���W
	D3DXVECTOR3 nor;  // �@���x�N�g��
	D3DCOLOR col;     // ���_�J���[
	D3DXVECTOR2 tex;  // �e�N�X�`���摜
} VERTEX_3D;

//*****************************************************************************
//     �v���g�^�C�v�錾
//*****************************************************************************
#if _DEBUG
int GetFPSCount(void);
#endif

#endif