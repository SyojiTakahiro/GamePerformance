//=============================================================================
//
// ���f������ [cpu.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "cpu.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "particle.h"
#include "explosion.h"
#include "building.h"
#include "game.h"
#include "cpulife.h"
#include "fade.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MODEL_NAME		"data/MODEL/P2.x"
#define NORMAL_SPEED    (5)
#define DASH_SPEED		(6)
#define MAX_PARTS		(1)
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPD3DXMESH g_pMeshcpu = NULL;			//���b�V�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatcpu = NULL;	//�}�e���A�����ւ̃|�C���^

CPU g_cpu;

LPDIRECT3DTEXTURE9 g_pTexturecpu = NULL;
DWORD g_nNumMatcpu = 0;				//�}�e���A�����̐�

										//D3DXVECTOR3 g_vtxMinModel, g_vtxMaxModel;

										//D3DXMATRIX mtxWorld2;

										//D3DXMATRIX g_mtxWorld2cpu;	
int g_nIdxCPUShadow;

#define CPU_LIFE			 (2)								//�v���C���[�̗̑�

int g_BulletRecastCPU = 150;

//=============================================================================
// ����������
//=============================================================================
void InitCPU(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntcpu = 0; nCntcpu < MAX_PLAYER; nCntcpu++)
	{
		g_cpu.state = CPUSTATE_NORMAL;
		g_cpu.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_cpu.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_cpu.rot = D3DXVECTOR3(0, 0, 0);
		g_cpu.nType = 0;
		g_cpu.nCounterState = 60;
		g_cpu.state;
		g_cpu.nLife = CPU_LIFE;
		g_cpu.DirectionMove = 0;
		g_cpu.bJump = false;
		g_cpu.bUse = true;
	}

	g_cpu.pos = D3DXVECTOR3(350.0f, 10.0f, -160.0f);

	//// X�t�@�C���̓ǂݍ���
	//D3DXLoadMeshFromX(MODEL_NAME,
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatcpu,
	//	NULL,
	//	&g_nNumMatcpu,
	//	&g_pMeshcpu);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_cpu.aModel[0].pBuffMat,
		NULL,
		&g_cpu.aModel[0].nNumMat,
		&g_cpu.aModel[0].pMesh);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_NAME2,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_cpu.aModel[1].pBuffMat,
		NULL,
		&g_cpu.aModel[1].nNumMat,
		&g_cpu.aModel[1].pMesh);

	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 vtx;

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_cpu.VtxMin = D3DXVECTOR3(10000, 0, 10000);
		g_cpu.VtxMax = D3DXVECTOR3(-10000, 0, -10000);
		g_cpu.nType = 0;
	}

	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		//���_�����擾
		nNumVtx = g_cpu.aModel[nCntModel].pMesh->GetNumVertices();
		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_cpu.aModel[nCntModel].pMesh->GetFVF());

		//���_�@���b�N
		g_cpu.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{	//���_���W�̑��
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (g_cpu.VtxMin.x > vtx.x)
			{
				g_cpu.VtxMin.x = vtx.x;
			}
			if (g_cpu.VtxMin.y > vtx.y)
			{
				g_cpu.VtxMin.y = vtx.y;
			}
			if (g_cpu.VtxMin.z > vtx.z)
			{
				g_cpu.VtxMin.z = vtx.z;
			}

			if (g_cpu.VtxMax.x < vtx.x)
			{
				g_cpu.VtxMax.x = vtx.x;
			}
			if (g_cpu.VtxMax.y < vtx.y)
			{
				g_cpu.VtxMax.y = vtx.y;
			}
			if (g_cpu.VtxMax.z < vtx.z)
			{
				g_cpu.VtxMax.z = vtx.z;
			}
			//��������
			pVtxBuff += sizeFVF;
		}
		//			//���_�o�b�t�@���A�����b�N
		g_cpu.aModel[nCntModel].pMesh->UnlockVertexBuffer();
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, MODEL_NAME, &g_pTexturecpu);

	//�I�D�t�Z�b�g
	g_cpu.aModel[0].nIdxModelParent = -1;
	g_cpu.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_cpu.aModel[1].nIdxModelParent = 0;
	g_cpu.aModel[1].pos = D3DXVECTOR3(0.0f, 15.0f, 8.0f);
	g_cpu.aModel[1].rot = D3DXVECTOR3(-150.0f, 0.0f, 0.0f);

	g_nIdxCPUShadow = SetShadow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_cpu.rot);
}

//=============================================================================
// �I������
//=============================================================================
void UninitCPU(void)
{
	// ���b�V���̊J��
	if (g_pMeshcpu != NULL)
	{
		g_pMeshcpu->Release();
		g_pMeshcpu = NULL;
	}

	// �}�e���A���̊J��
	if (g_pBuffMatcpu != NULL)
	{
		g_pBuffMatcpu->Release();
		g_pBuffMatcpu = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTexturecpu != NULL)
	{
		g_pTexturecpu->Release();
		g_pTexturecpu = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCPU(void)
{
	CAMERA *pCamera;
	pCamera = GetCamera();

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	GAMESTATE GameState = GetGameState();

	float fData;

	g_cpu.posOld = g_cpu.pos;

	g_cpu.rot.y++;

	//g_cpu.pos.x -= sinf(g_cpu.rot.y + D3DX_PI * 0.75f) * NORMAL_SPEED;
	//g_cpu.pos.z -= cosf(g_cpu.rot.y + D3DX_PI * 0.75f) * NORMAL_SPEED;

	//pCamera->rot.y;

	if (GetKeyboardPress(DIK_RCONTROL) == true)
	{
		g_cpu.pos.y -= cosf(D3DX_PI * 0) * 4;
	}

	/*if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
	g_cpu.rot.y -= cosf(-D3DX_PI) * 6;
	}*/
	/*if (GetKeyboardPress(DIK_RSHIFT) == true)
	{
	g_cpu.rot.y -= cosf(D3DX_PI * 0) * 6;
	}*/
	g_BulletRecastCPU++;
	if (g_BulletRecastCPU >= 150)
	{
		/*if (GetKeyboardTrigger(DIK_L) == true)
		{*/
			fData = atan2f(pPlayer->pos.x - g_cpu.pos.x, pPlayer->pos.z - g_cpu.pos.z);
			SetBullet(D3DXVECTOR3(g_cpu.pos.x, g_cpu.pos.y + 5, g_cpu.pos.z), D3DXVECTOR3(sinf(fData) * 5.0f, tanf(fData) * 5.0f, cosf(fData) * 5.0f), 10.0f, 10.0f, BULLETTYPE_CPU);
			g_BulletRecastCPU = 0;
		//}
	}

	if (g_cpu.pos.x  < 30)
	{
		g_cpu.pos.x = 80;
		CutCPULife(1);
		g_cpu.nLife--;
	}
	if (g_cpu.pos.x > 500)
	{
		g_cpu.pos.x = 450;
		CutCPULife(1);
		g_cpu.nLife--;
	}
	if (g_cpu.pos.z  > 50)
	{
		g_cpu.pos.z = 0;
		CutCPULife(1);
		g_cpu.nLife--;
	}
	if (g_cpu.pos.z < -340)
	{
		g_cpu.pos.z = -260;
		CutCPULife(1);
		g_cpu.nLife--;
	}

	/*if (g_cpu.nLife == 0)
	{
	if (GameState != GAMESTATE_P1WIN)
	{
	SetFade(MODE_P1WIN);
	}
	}*/

	//if (GetKeyboardPress(DIK_RSHIFT) == false)
	//{
	//	SetParticle(g_cpu.pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), 10.0f, 100);
	//	//�΂߈ړ��̏���
	//	if (GetKeyboardPress(DIK_LEFT) == true)
	//	{
	//		if (GetKeyboardPress(DIK_UP) == true)
	//		{//����ړ�
	//			g_cpu.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.75f) * NORMAL_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.75f) * NORMAL_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.75f);
	//		}
	//		else if (GetKeyboardPress(DIK_DOWN) == true)
	//		{//�����ړ�
	//			g_cpu.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.25f) * NORMAL_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.25f) * NORMAL_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.25f);
	//		}
	//		else
	//		{//���ړ�
	//			g_cpu.pos.x += sinf(pCamera->rot.y - D3DX_PI * 0.5f) * NORMAL_SPEED;
	//			g_cpu.pos.z += cosf(pCamera->rot.y - D3DX_PI * 0.5f) * NORMAL_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.5f);
	//		}
	//	}

	//	//�΂߈ړ��̏���
	//	else if (GetKeyboardPress(DIK_RIGHT) == true)
	//	{
	//		if (GetKeyboardPress(DIK_UP) == true)
	//		{//�E��ړ�
	//			g_cpu.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.75f) * NORMAL_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.75f) * NORMAL_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y - D3DX_PI * 0.75f);
	//		}
	//		else if (GetKeyboardPress(DIK_DOWN) == true)
	//		{//�E���ړ�
	//			g_cpu.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.25f) * NORMAL_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.25f) * NORMAL_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y - D3DX_PI * 0.25f);
	//		}
	//		else
	//		{//�E�ړ�
	//			g_cpu.pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * NORMAL_SPEED;
	//			g_cpu.pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * NORMAL_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y - D3DX_PI * 0.5f);
	//		}
	//	}
	//	else if (GetKeyboardPress(DIK_UP) == true)
	//	{
	//		g_cpu.pos.x += sinf(pCamera->rot.y) * NORMAL_SPEED;
	//		g_cpu.pos.z += cosf(pCamera->rot.y) * NORMAL_SPEED;
	//		//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 1.0f);
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN) == true)
	//	{
	//		g_cpu.pos.x -= sinf(pCamera->rot.y) * NORMAL_SPEED;
	//		g_cpu.pos.z -= cosf(pCamera->rot.y) * NORMAL_SPEED;
	//		//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.0f);
	//	}

	//}
	//if (GetKeyboardPress(DIK_RSHIFT) == true)
	//{
	//	SetParticle(g_cpu.pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 20.0f, 100);
	//	//�΂߈ړ��̏���
	//	if (GetKeyboardPress(DIK_LEFT) == true)
	//	{
	//		if (GetKeyboardPress(DIK_UP) == true)
	//		{//����ړ�
	//			g_cpu.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.75f) * DASH_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.75f) * DASH_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.75f);
	//		}
	//		else if (GetKeyboardPress(DIK_DOWN) == true)
	//		{//�����ړ�
	//			g_cpu.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.25f) * DASH_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.25f) * DASH_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.25f);
	//		}
	//		else
	//		{//���ړ�
	//			g_cpu.pos.x += sinf(pCamera->rot.y - D3DX_PI * 0.5f) * DASH_SPEED;
	//			g_cpu.pos.z += cosf(pCamera->rot.y - D3DX_PI * 0.5f) * DASH_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.5f);
	//		}
	//	}

	//	//�΂߈ړ��̏���
	//	else if (GetKeyboardPress(DIK_RIGHT) == true)
	//	{
	//		if (GetKeyboardPress(DIK_UP) == true)
	//		{//�E��ړ�
	//			g_cpu.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.75f) * DASH_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.75f) * DASH_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y - D3DX_PI * 0.75f);
	//		}
	//		else if (GetKeyboardPress(DIK_DOWN) == true)
	//		{//�E���ړ�
	//			g_cpu.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.25f) * DASH_SPEED;
	//			g_cpu.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.25f) * DASH_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y - D3DX_PI * 0.25f);
	//		}
	//		else
	//		{//�E�ړ�
	//			g_cpu.pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * DASH_SPEED;
	//			g_cpu.pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * DASH_SPEED;
	//			//g_cpu.rot.y = (pCamera->rot.y - D3DX_PI * 0.5f);
	//		}
	//	}
	//	else if (GetKeyboardPress(DIK_UP) == true)
	//	{
	//		g_cpu.pos.x += sinf(pCamera->rot.y) * DASH_SPEED;
	//		g_cpu.pos.z += cosf(pCamera->rot.y) * DASH_SPEED;
	//		//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 1.0f);
	//	}
	//	else if (GetKeyboardPress(DIK_DOWN) == true)
	//	{
	//		g_cpu.pos.x -= sinf(pCamera->rot.y) * DASH_SPEED;
	//		g_cpu.pos.z -= cosf(pCamera->rot.y) * DASH_SPEED;
	//		//g_cpu.rot.y = (pCamera->rot.y + D3DX_PI * 0.0f);
	//	}
	//}

	/*if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		g_cpu.move.y -= cosf(-D3DX_PI) * 2;
	}*/

	g_cpu.pos.x -= pPlayer->move.x;
	g_cpu.pos.z -= pPlayer->move.z;
	fData = atan2f(pPlayer->pos.x - sinf(pPlayer->rot.y) - g_cpu.pos.x, pPlayer->pos.z -cosf(pPlayer->rot.y) - g_cpu.pos.z);
	g_cpu.pos += D3DXVECTOR3(sinf(fData) * NORMAL_SPEED, 0.0f, cosf(fData) * NORMAL_SPEED);

	g_cpu.move.y -= 0.1f;

	if (g_cpu.pos.y < 1.0f)
	{
		SetExplosion(g_cpu.pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
		g_cpu.pos.y = 1;
		g_cpu.move.y = 0.0f;
	}

	g_cpu.pos.x += g_cpu.move.x;
	g_cpu.pos.z += g_cpu.move.z;
	g_cpu.pos.y += g_cpu.move.y;

	if (CollisionPlayer(&g_cpu.pos, &g_cpu.posOld, &g_cpu.move, g_cpu.VtxMax, g_cpu.VtxMin) == true)
	{

	}

	if (CollisionBuilding(&g_cpu.pos, &g_cpu.posOld, &g_cpu.move, g_cpu.VtxMax, g_cpu.VtxMin) == true)
	{

	}

	g_cpu.move.x += (0.0f - g_cpu.move.x) * 0.15f;
	g_cpu.move.z += (0.0f - g_cpu.move.z) * 0.15f;

	SetPositionShadow(g_nIdxCPUShadow, D3DXVECTOR3(g_cpu.pos.x, 0.1f, g_cpu.pos.z));
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCPU(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;
	D3DXMATRIX mtxParent;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_cpu.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_cpu.rot.y, g_cpu.rot.x, g_cpu.rot.z);

	D3DXMatrixMultiply(&g_cpu.mtxWorld, &g_cpu.mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_cpu.pos.x, g_cpu.pos.y, g_cpu.pos.z);

	D3DXMatrixMultiply(&g_cpu.mtxWorld, &g_cpu.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_cpu.mtxWorld);
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (g_cpu.aModel[nCnt].nIdxModelParent == -1)
		{
			mtxParent = g_cpu.mtxWorld;
		}
		else
		{
			mtxParent = g_cpu.aModel[g_cpu.aModel[nCnt].nIdxModelParent].mtxWorld;
		}
		//=============================================================================
		// �e�̃��f��
		//=============================================================================
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_cpu.aModel[nCnt].mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_cpu.aModel[nCnt].rot.y, g_cpu.aModel[nCnt].rot.x, g_cpu.aModel[nCnt].rot.z);

		D3DXMatrixMultiply(&g_cpu.aModel[nCnt].mtxWorld, &g_cpu.aModel[nCnt].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_cpu.aModel[nCnt].pos.x, g_cpu.aModel[nCnt].pos.y, g_cpu.aModel[nCnt].pos.z);

		D3DXMatrixMultiply(&g_cpu.aModel[nCnt].mtxWorld, &g_cpu.aModel[nCnt].mtxWorld, &mtxTrans);

		D3DXMatrixMultiply(&g_cpu.aModel[nCnt].mtxWorld, &g_cpu.aModel[nCnt].mtxWorld, &mtxParent);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_cpu.aModel[nCnt].mtxWorld);


		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_cpu.aModel[nCnt].pBuffMat->GetBufferPointer();


		for (int nCntMat = 0; nCntMat < (int)g_cpu.aModel[nCnt].nNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̓ǂݍ���
			pDevice->SetTexture(0, g_pTexturecpu);

			// ���f��(�p�[�c)�̕`��
			g_cpu.aModel[nCnt].pMesh->DrawSubset(nCntMat);
		}
		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);
	}
	//=============================================================================
	// �q�̃��f��
	//=============================================================================
}

CPU *GetCPU(void)
{
	return &g_cpu;
}

//*****************************************************************************
//���f���Ƃ̓����蔻�菈��
//*****************************************************************************
bool CollisionCPU(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pModel*/)
{
	bool bLand = false;

	//GAMESTATE GameState = GetGameState();



	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		if (g_cpu.bUse == true)
		{
			//���f����Y���̒�
			if (pPos->y < g_cpu.pos.y + g_cpu.VtxMax.y
				&& pPos->y > g_cpu.pos.y + g_cpu.VtxMin.y)
			{
				//���f����Z���̒�
				if (pPos->z + VtxMin.z < g_cpu.pos.z + g_cpu.VtxMax.z
					&& pPos->z + VtxMax.z > g_cpu.pos.z + g_cpu.VtxMin.z)
				{
					//���f����X���̒�
					if (pPos->x + VtxMin.x < g_cpu.pos.x + g_cpu.VtxMax.x
						&& pPos->x + VtxMax.x > g_cpu.pos.x + g_cpu.VtxMin.x)
					{

						//X���̓����蔻��
						if (pPosOld->x > g_cpu.pos.x + g_cpu.VtxMax.x)
						{
							g_cpu.move.x += pMove->x - 15;

							//g_cpu.move.x += bMove->x;

							pPos->x = g_cpu.pos.x + g_cpu.VtxMax.x + VtxMax.x;
							pPosOld->x = g_cpu.pos.x + g_cpu.VtxMax.x + VtxMax.x;
							//pMove->x = 0;
							bLand = true;
						}
						else if (pPosOld->x < g_cpu.pos.x + g_cpu.VtxMin.x)
						{
							if (g_cpu.nType == 1)
							{

							}

							g_cpu.move.x += pMove->x + 15;
							pPos->x = g_cpu.pos.x + g_cpu.VtxMin.x + VtxMin.x;
							pPosOld->x = g_cpu.pos.x + g_cpu.VtxMin.x + VtxMin.x;
							//pMove->x = 0;
							bLand = true;

						}
						//Z���̓����蔻��
						if (pPosOld->z > g_cpu.pos.z + g_cpu.VtxMax.z)
						{
							if (g_cpu.nType == 1)
							{

							}
							bLand = true;

							g_cpu.move.z += pMove->z - 15;

							pPos->z = g_cpu.pos.z + g_cpu.VtxMax.z + VtxMax.z;
							pPosOld->z = g_cpu.pos.z + g_cpu.VtxMax.z + VtxMax.z;
							//pMove->z = 0;
						}
						else if (pPosOld->z < g_cpu.pos.z + g_cpu.VtxMin.z)
						{
							if (g_cpu.nType == 1)
							{

							}
							bLand = true;

							g_cpu.move.z += pMove->z + 15;

							pPos->z = g_cpu.pos.z + g_cpu.VtxMin.z + VtxMin.z;
							pPosOld->z = g_cpu.pos.z + g_cpu.VtxMin.z + VtxMin.z;
							//pMove->z = 0;
						}
						//Y���̓����蔻��
						if (pPosOld->y >= g_cpu.pos.y + g_cpu.VtxMax.y)
						{
							pPos->y = g_cpu.pos.y + g_cpu.VtxMax.y;
							pPosOld->y = g_cpu.pos.y + g_cpu.VtxMax.y;
							pMove->y = 0;

						}
						else if (pPosOld->y <= g_cpu.pos.y + g_cpu.VtxMin.y)
						{
							pPos->y = g_cpu.pos.y + g_cpu.VtxMin.y;
							pPosOld->y = g_cpu.pos.y + g_cpu.VtxMin.y;
						}
						bLand = true;
					}
				}
			}
		}
	}
	return bLand;
}