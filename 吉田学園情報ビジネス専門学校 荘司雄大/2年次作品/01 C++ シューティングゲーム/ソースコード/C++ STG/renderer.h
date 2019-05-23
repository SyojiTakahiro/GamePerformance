//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : Takahiro Syoji 
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
// �w�b�_�t�@�C���̃C���N���[�h
//*****************************************************************************
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>						//�C���N���[�h�t�@�C��

#include "main.h"

class CPause;
//*****************************************************************************
//�����_�����O�N���X
//*****************************************************************************

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void); /*{ return m_pD3DDevice; }*/

	void ReleasePause(void);

private:
	void DrawFPS(void);

	LPDIRECT3D9				m_pD3D = NULL;			// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)

	LPDIRECT3DTEXTURE9		m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^

	static CPause *m_pPause;

	bool m_bPause;
#ifdef _DEBUG
	LPD3DXFONT				m_pFont = NULL;			// �t�H���g�ւ̃|�C���^
#endif
};

#endif