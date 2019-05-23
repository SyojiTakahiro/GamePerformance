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
	typedef enum
	{
		TARGET_DEFAULT = 0,   // �f�t�H���g�̐ݒ�
		TARGET_TEXTURE,       // �e�N�X�`��
		TARGET_MAX
	}TARGET;

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
	void DefaultRender(void);
	void TextureRender(void);

	CPause                  *m_pPause;              // �|�[�Y�N���X�^�̃|�C���^
	bool                    m_bPause;               // �|�[�Y�����ǂ���
	LPDIRECT3D9				m_pD3D;			        // Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice;	        // Device�I�u�W�F�N�g(�`��ɕK�v)
	LPDIRECT3DSURFACE9      m_pZBufferSurface;      // Z�o�b�t�@�ւ̃|�C���^(�e�N�X�`���Ƀ����_�����O����ۂ�Z�o�b�t�@��؂�ւ��邽��)
	LPDIRECT3DTEXTURE9      m_pTexture;             // �e�N�X�`���ւ̃|�C���^(���̃e�N�X�`���ɃI�u�W�F�N�g��`�悷��)
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;             // ���_�o�b�t�@�ւ̃|�C���^(�I�u�W�F�N�g��`�悵���e�N�X�`�����g�p����)
	TARGET                  m_Target;               // �I�u�W�F�N�g��`�悷�郌���_�����O�^�[�Q�b�g
	float                   m_fRot;                 // �|���S���̌���
	float                   m_fAngle;               // �|���S�����o���p�x
	float                   m_fWidth;               // �|���S���̕�
	float                   m_fHeight;              // �|���S���̍���
	float                   m_fLength;              // �|���S���̒���
};

#endif