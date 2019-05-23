//*****************************************************************************
//
//     �J�����̏���[camera.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _CAMERA_H_
#define _CAMERA_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//    �O���錾
//*****************************************************************************
class CPlayer;

//*****************************************************************************
//    �J�����N���X�̒�`
//*****************************************************************************
class CCamera
{
public:    // �N�ł��A�N�Z�X�\
	CCamera();
	~CCamera();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetCamera(void);

	void SetPosV(const D3DXVECTOR3 posV);
	void SetPosR(const D3DXVECTOR3 posR);
	void SetRot(const D3DXVECTOR3 Rot);
	void SetMtxProjection(const D3DXMATRIX MtxProjection);
	void SetMtxView(const D3DXMATRIX MtxView);

	D3DXVECTOR3 GetPosV(void);
	D3DXVECTOR3 GetPosR(void);
	D3DXVECTOR3 GetRot(void);
	D3DXMATRIX GetMtxProjection(void);
	D3DXMATRIX GetMtxView(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	void Title(void);
	void Tutorial(void);
	void Game(void);
	void Result(void);
	void Ranking(void);
	void ObjectIntersect(void);

	CPlayer      *m_pPlayer;      // �v���C���[�N���X�ւ̃|�C���^
	D3DXVECTOR3  m_PosV;          // ���_
	D3DXVECTOR3  m_PosR;          // �����_
	D3DXVECTOR3  m_Rot;           // ����
	D3DXVECTOR3  m_VecU;          // ������x�N�g��
	D3DXMATRIX   m_MtxProjection; // �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX   m_MtxView;       // �r���[�}�g���b�N�X
	D3DVIEWPORT9 m_ViewPort;      // �r���[�|�[�g(�`��̈�)
	float        m_fLength;       // ����
};

#endif