//*****************************************************************************
//
//     �����_�����O�̏���[renderer.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _RENDERER_H_
#define _RENDERER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �O���錾(���̂��Ȃ��錾)
//*****************************************************************************
class CPause;

//*****************************************************************************
//    �����_�����O�N���X�̒�`
//*****************************************************************************
class CRenderer
{
public:     // �N�ł��A�N�Z�X�\
	CRenderer();
	~CRenderer();
	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	LPDIRECT3DDEVICE9 GetDevice(void);

	void ReleasePause(void);
	void SetPause(bool bPause);
	bool GetPause(void);


protected:  // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:    // ���̃N���X�������A�N�Z�X�\
	LPDIRECT3D9				m_pD3D;			// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice;	// Device�I�u�W�F�N�g(�`��ɕK�v)
	CPause                  *m_pPause;      // �|�[�Y�N���X�^�̃|�C���^
	bool                    m_bPause;       // �|�[�Y�����ǂ���
	bool                    m_bPauseRelease;  // �|�[�Y
};

#endif