//=============================================================================
//
// �J�������� [camera.cpp]
// Author : 
//
//=============================================================================
#include "camera.h"
#include "input.h"
#include "player.h"
#include "player2.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
CAMERA g_camera;
float fLength;

//=============================================================================
// �J�����̏���������
//=============================================================================
void InitCamera(void)
{
	g_camera.posV = D3DXVECTOR3(0,0,0);
	g_camera.posR = D3DXVECTOR3(0,0,0);
	g_camera.vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	///*g_camera.ViewPort.X = 0.0f;
	//g_camera.ViewPort.Y = 0.5f;*/
	/*g_camera.ViewPort.Width = SCREEN_WIDTH;
	g_camera.ViewPort.Height = SCREEN_HEIGHT/2;
	g_camera.ViewPort.MinZ = 0.0f;
	g_camera.ViewPort.MaxZ = 1.0f;*/
	g_camera.nCounterAngle = 0;
	g_camera.fLength = sqrtf((g_camera.posV.x - g_camera.posR.x)*(g_camera.posV.x - g_camera.posR.x) + (g_camera.posV.z - g_camera.posR.z)*(g_camera.posV.z - g_camera.posR.z));
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();
	PLAYER2 *pPlayer2;
	pPlayer2 = GetPlayer2();

	
//-----------------------------------------------------------------------------
// �J�����ړ�
//-----------------------------------------------------------------------------
	
	/*g_camera.posR.x = (pPlayer2->pos.x + pPlayer->pos.x) / 2;
	g_camera.posR.y = (pPlayer2->pos.y + pPlayer->pos.y);
	g_camera.posR.z = (pPlayer2->pos.z + pPlayer->pos.z) / 2;

	g_camera.posV.x = (pPlayer2->pos.x + pPlayer->pos.x) / 2;
	g_camera.posV.y = ((pPlayer2->pos.x + pPlayer2->pos.z) / 2 + (pPlayer->pos.x + pPlayer->pos.z)) / 2 +600;
	g_camera.posV.z = ((pPlayer2->pos.z) / 2 + (pPlayer->pos.z) / 2) + ((pPlayer2->pos.z)/2 + (pPlayer->pos.z)/2)  ;*/

	//���݂̃��[�h���擾
	MODE Mode = GetMode();

	switch (Mode)
	{
	case MODE_GAME:

	g_camera.posR.x = 250;
	g_camera.posR.y = 0;
	g_camera.posR.z = -180;

	g_camera.posV.x = 250;
	g_camera.posV.y = 400 + 100.0f;
	g_camera.posV.z = -250 - 100.0f;

	break;

	case MODE_SINGLEGAME:

		g_camera.posR.x = 250;
		g_camera.posR.y = 0;
		g_camera.posR.z = -180;

		g_camera.posV.x = 250;
		g_camera.posV.y = 400 + 100.0f;
		g_camera.posV.z = -250 - 100.0f;

		break;

	case MODE_TITLE:
		//�J�E���^�[�̉��Z
		g_camera.nCounterAngle++;

		g_camera.posR.x = 250;
		g_camera.posR.y = 0;
		g_camera.posR.z = -80;

		g_camera.posV.x = 250;
		g_camera.posV.y = 500;
		g_camera.posV.z = -350;

		if (g_camera.nCounterAngle == 300)
		{
			g_camera.posV = D3DXVECTOR3(0.0f, 400.0f, 0.0f);
		}
		if (g_camera.nCounterAngle == 600)
		{
			g_camera.posV = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
			fLength = 200;
		}
		if (g_camera.nCounterAngle == 900)
		{
			g_camera.nCounterAngle = 0;
		}
		if (g_camera.nCounterAngle < 300)
		{
			g_camera.posV = D3DXVECTOR3(0.0f, 100.0f, -300.0f);
		}

		g_camera.rot.y += 0.003f;
		if (g_camera.rot.y >= D3DX_PI)
		{
			g_camera.rot.y = -D3DX_PI;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y ) *sqrtf((g_camera.posV.x - g_camera.posR.x)*(g_camera.posV.x - g_camera.posR.x) + (g_camera.posV.z - g_camera.posR.z)*(g_camera.posV.z - g_camera.posR.z));
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * sqrtf((g_camera.posV.x - g_camera.posR.x)*(g_camera.posV.x - g_camera.posR.x) + (g_camera.posV.z - g_camera.posR.z)*(g_camera.posV.z - g_camera.posR.z));

		break;

	case MODE_P1WIN:
		//�J�E���^�[�̉��Z
		g_camera.nCounterAngle++;

		g_camera.posR.x = 250;
		g_camera.posR.y = 0;
		g_camera.posR.z = -80;

		g_camera.posV.x = 250;
		g_camera.posV.y = 500;
		g_camera.posV.z = -350;

		if (g_camera.nCounterAngle == 300)
		{
			g_camera.posV = D3DXVECTOR3(0.0f, 500.0f, 0.0f);
		}
		if (g_camera.nCounterAngle == 600)
		{
			g_camera.posV = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
			fLength = 200;
		}
		if (g_camera.nCounterAngle == 900)
		{
			g_camera.nCounterAngle = 0;
		}
		if (g_camera.nCounterAngle < 300)
		{
			g_camera.posV = D3DXVECTOR3(0.0f, 100.0f, -200.0f);
		}

		g_camera.rot.y += 0.003f;
		if (g_camera.rot.y >= D3DX_PI)
		{
			g_camera.rot.y = -D3DX_PI;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) *sqrtf((g_camera.posV.x - g_camera.posR.x)*(g_camera.posV.x - g_camera.posR.x) + (g_camera.posV.z - g_camera.posR.z)*(g_camera.posV.z - g_camera.posR.z));
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * sqrtf((g_camera.posV.x - g_camera.posR.x)*(g_camera.posV.x - g_camera.posR.x) + (g_camera.posV.z - g_camera.posR.z)*(g_camera.posV.z - g_camera.posR.z));

		break;

	case MODE_P2WIN:
		//�J�E���^�[�̉��Z
		g_camera.nCounterAngle++;

		g_camera.posR.x = 250;
		g_camera.posR.y = 0;
		g_camera.posR.z = -80;

		g_camera.posV.x = 250;
		g_camera.posV.y = 500;
		g_camera.posV.z = -350;

		if (g_camera.nCounterAngle == 300)
		{
			g_camera.posV = D3DXVECTOR3(0.0f, 500.0f, 0.0f);
		}
		if (g_camera.nCounterAngle == 600)
		{
			g_camera.posV = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
			fLength = 200;
		}
		if (g_camera.nCounterAngle == 900)
		{
			g_camera.nCounterAngle = 0;
		}
		if (g_camera.nCounterAngle < 300)
		{
			g_camera.posV = D3DXVECTOR3(0.0f, 100.0f, -200.0f);
		}

		g_camera.rot.y += 0.003f;
		if (g_camera.rot.y >= D3DX_PI)
		{
			g_camera.rot.y = -D3DX_PI;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) *sqrtf((g_camera.posV.x - g_camera.posR.x)*(g_camera.posV.x - g_camera.posR.x) + (g_camera.posV.z - g_camera.posR.z)*(g_camera.posV.z - g_camera.posR.z));
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * sqrtf((g_camera.posV.x - g_camera.posR.x)*(g_camera.posV.x - g_camera.posR.x) + (g_camera.posV.z - g_camera.posR.z)*(g_camera.posV.z - g_camera.posR.z));

		break;
	}
	/*g_camera.posR.x = pPlayer->pos.x;
	g_camera.posR.y = pPlayer->pos.y;
	g_camera.posR.z = pPlayer->pos.z;

	g_camera.posV.x = pPlayer->pos.x;
	g_camera.posV.y = pPlayer->pos.y + 100.0f;
	g_camera.posV.z = pPlayer->pos.z - 150.0f;*/

	/*g_camera.posRDest.x = pPlayer->pos.x - sinf(g_camera.posR.y) * 30;
	g_camera.posRDest.y = pPlayer->pos.y - sinf(g_camera.posR.y) * 30;
	g_camera.posRDest.z = pPlayer->pos.z - cosf(g_camera.posR.y) * 30;

	g_camera.posVDest.x = pPlayer->pos.x - sinf(pPlayer->pos.y) * fLength;
	g_camera.posVDest.y = pPlayer->pos.y - sinf(pPlayer->pos.y) * fLength;
	g_camera.posVDest.z = pPlayer->pos.z - cosf(pPlayer->pos.y) * fLength;

	g_camera.posV.x += g_camera.posVDest.x - g_camera.posV.x  * 0.5f;
	g_camera.posV.y += g_camera.posVDest.y - g_camera.posV.y  * 0.5f;
	g_camera.posV.z += g_camera.posVDest.z - g_camera.posV.z  * 0.5f;

	g_camera.posR.x = g_camera.posRDest.x - g_camera.posR.x  * 0.5f;
	g_camera.posR.y = g_camera.posRDest.y - g_camera.posR.y  * 0.5f;
	g_camera.posR.z = g_camera.posRDest.z - g_camera.posR.z  * 0.5f;*/

#if 0
	//�΂߈ړ��̏���
	if (GetKeyboardPress(DIK_A) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{//����ړ�
			g_camera.posV.x += sinf(g_camera.rot.y -D3DX_PI * 0.75f) * 3;
			g_camera.posV.z += cosf(g_camera.rot.y -D3DX_PI * 0.75f) * 3;

			g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * fLength;
			g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * fLength;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//�����ړ�
			g_camera.posV.x += sinf(g_camera.rot.y -D3DX_PI * 0.25f) * 3;
			g_camera.posV.z += cosf(g_camera.rot.y -D3DX_PI * 0.25f) * 3;

			g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * fLength;
			g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * fLength;
		}
		else
		{//���ړ�
			g_camera.posV.x += sinf(g_camera.rot.y - D3DX_PI * 0.5f) * 3;
			g_camera.posV.z += cosf(g_camera.rot.y - D3DX_PI * 0.5f) * 3;

			g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * fLength;
			g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * fLength;
		}
	}

	//�΂߈ړ��̏���
	if (GetKeyboardPress(DIK_D) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{//�E��ړ�
			g_camera.posV.x += sinf(g_camera.rot.y + D3DX_PI * 0.75f) * 3;
			g_camera.posV.z += cosf(g_camera.rot.y + D3DX_PI * 0.75f) * 3;

			g_camera.posR.x = g_camera.posV.x - sinf(g_camera.rot.y) * fLength;
			g_camera.posR.z = g_camera.posV.z - cosf(g_camera.rot.y) * fLength;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//�E���ړ�
			g_camera.posV.x += sinf(g_camera.rot.y + D3DX_PI * 0.25f) * 3;
			g_camera.posV.z += cosf(g_camera.rot.y + D3DX_PI * 0.25f) * 3;

			g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * fLength;
			g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * fLength;
		}
		else
		{//�E�ړ�
			g_camera.posV.x += sinf(g_camera.rot.y + D3DX_PI * 0.5f) * 3;
			g_camera.posV.z += cosf(g_camera.rot.y + D3DX_PI * 0.5f) * 3;

			g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * fLength;
			g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * fLength;
		}
	}

	if (GetKeyboardPress(DIK_W) == true)
	{
		g_camera.posV.x += sinf(g_camera.rot.y) * 3;
		g_camera.posV.z += cosf(g_camera.rot.y) * 3;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * fLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * fLength;
	}
	if (GetKeyboardPress(DIK_S) == true)
	{
		g_camera.posV.x -= sinf(g_camera.rot.y) * 3;
		g_camera.posV.z -= cosf(g_camera.rot.y) * 3;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * fLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * fLength;
	}

	//=============================================================================
	// ���_����
	//=============================================================================
	if (GetKeyboardPress(DIK_T) == true)
	{
		g_camera.posV.y -= cosf(-D3DX_PI) * 3;
	}
	if (GetKeyboardPress(DIK_G) == true)
	{
		g_camera.posV.y -= cosf(D3DX_PI * 0) * 3;
	}
	if (GetKeyboardPress(DIK_F) == true)
	{
		g_camera.rot.y += 0.03f;
		if (g_camera.rot.y >= D3DX_PI)
		{
			g_camera.rot.y = -D3DX_PI;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * fLength;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * fLength;

	}
	if (GetKeyboardPress(DIK_H) == true)
	{
		g_camera.rot.y -= 0.03f;
		if (g_camera.rot.y <= -D3DX_PI)
		{
			g_camera.rot.y = D3DX_PI;
		}
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * fLength;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * fLength;
	}

	//=============================================================================
	// �����_����(����)
	//=============================================================================
	if (GetKeyboardPress(DIK_I) == true)
	{
		g_camera.posR.y += 3;
	}
	if (GetKeyboardPress(DIK_K) == true)
	{
		g_camera.posR.y -=  3;
	}
	if (GetKeyboardPress(DIK_J) == true)
	{
		g_camera.rot.y -= 0.03f;
		if (g_camera.rot.y <= -D3DX_PI)
		{
			g_camera.rot.y = D3DX_PI;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * fLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * fLength;
	}
	if (GetKeyboardPress(DIK_L) == true)
	{
		g_camera.rot.y += 0.03f;
		if (g_camera.rot.y >= D3DX_PI)
		{
			g_camera.rot.y = -D3DX_PI;
		}
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.y) * fLength;
		g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.y) * fLength;
	}


#endif // 0

}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 1000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	// �r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView, &g_camera.posV, &g_camera.posR, &g_camera.vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

CAMERA *GetCamera(void)
{
	return &g_camera;
}