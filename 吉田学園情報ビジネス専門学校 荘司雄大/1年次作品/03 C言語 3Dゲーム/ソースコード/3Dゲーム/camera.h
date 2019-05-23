//=============================================================================
//
// �J�������� [camera.h]
// Author : 
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �J�����̍\����
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 posV;				//���_
	D3DXVECTOR3 posR;				//�����_
	D3DXVECTOR3 posVDest;				//���_
	D3DXVECTOR3 posRDest;				//����
	D3DXVECTOR3	vecU;				//������x�N�g��
	D3DXVECTOR3	rot;
	D3DXMATRIX mtxProjection;
	D3DXMATRIX mtxView;
	D3DVIEWPORT9 ViewPort;
	int nCounterAngle;
	float fLength;
}CAMERA;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
CAMERA *GetCamera(void);

#endif
