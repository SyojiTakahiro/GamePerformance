//=============================================================================
//
// ���f������ [Player2.cpp]
// Author : 
//
//=============================================================================
#include "player.h"
#include "player2.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "particle.h"
#include "explosion.h"
#include "building.h"
#include "game.h"
#include "life2.h"
#include "sound.h"
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
LPD3DXMESH g_pMeshPlayer2 = NULL;			//���b�V�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatPlayer2 = NULL;	//�}�e���A�����ւ̃|�C���^

PLAYER2 g_Player2;

LPDIRECT3DTEXTURE9 g_pTexturePlayer2 = NULL;
DWORD g_nNumMatPlayer2 = 0;				//�}�e���A�����̐�

//D3DXVECTOR3 g_vtxMinModel, g_vtxMaxModel;

//D3DXMATRIX mtxWorld2;

//D3DXMATRIX g_mtxWorld2Player2;	
int g_nIdxShadow2;

#define PLAYER2_LIFE			 (2)								//�v���C���[�̗̑�

int g_BulletRecast2 = 150;

//=============================================================================
// ����������
//=============================================================================
void InitPlayer2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntPlayer2 = 0; nCntPlayer2 < MAX_PLAYER; nCntPlayer2++)
	{
		g_Player2.state = PLAYER2STATE_NORMAL;
		g_Player2.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player2.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player2.rot = D3DXVECTOR3(0, 0, 0);
		g_Player2.nType = 0;
		g_Player2.nCounterState = 60;
		g_Player2.state;
		g_Player2.nLife = PLAYER2_LIFE;
		g_Player2.DirectionMove = 0;
		g_Player2.bJump = false;
		g_Player2.bUse = true;
	}

	g_Player2.pos = D3DXVECTOR3(350.0f, 10.0f, -160.0f);

	//// X�t�@�C���̓ǂݍ���
	//D3DXLoadMeshFromX(MODEL_NAME,
	//	D3DXMESH_SYSTEMMEM,
	//	pDevice,
	//	NULL,
	//	&g_pBuffMatPlayer2,
	//	NULL,
	//	&g_nNumMatPlayer2,
	//	&g_pMeshPlayer2);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_NAME,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player2.aModel[0].pBuffMat,
		NULL,
		&g_Player2.aModel[0].nNumMat,
		&g_Player2.aModel[0].pMesh);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_NAME2,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_Player2.aModel[1].pBuffMat,
		NULL,
		&g_Player2.aModel[1].nNumMat,
		&g_Player2.aModel[1].pMesh);

	int nNumVtx;	//���_��
	DWORD sizeFVF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 vtx;

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_Player2.VtxMin = D3DXVECTOR3(10000, 0, 10000);
		g_Player2.VtxMax = D3DXVECTOR3(-10000, 0, -10000);
		g_Player2.nType = 0;

	}

	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		//���_�����擾
		nNumVtx = g_Player2.aModel[nCntModel].pMesh->GetNumVertices();
		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(g_Player2.aModel[nCntModel].pMesh->GetFVF());

		//���_�@���b�N
		g_Player2.aModel[nCntModel].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{	//���_���W�̑��
			vtx = *(D3DXVECTOR3*)pVtxBuff;

			if (g_Player2.VtxMin.x > vtx.x)
			{
				g_Player2.VtxMin.x = vtx.x;
			}
			if (g_Player2.VtxMin.y > vtx.y)
			{
				g_Player2.VtxMin.y = vtx.y;
			}
			if (g_Player2.VtxMin.z > vtx.z)
			{
				g_Player2.VtxMin.z = vtx.z;
			}

			if (g_Player2.VtxMax.x < vtx.x)
			{
				g_Player2.VtxMax.x = vtx.x;
			}
			if (g_Player2.VtxMax.y < vtx.y)
			{
				g_Player2.VtxMax.y = vtx.y;
			}
			if (g_Player2.VtxMax.z < vtx.z)
			{
				g_Player2.VtxMax.z = vtx.z;
			}
			//��������
			pVtxBuff += sizeFVF;
		}
		//			//���_�o�b�t�@���A�����b�N
		g_Player2.aModel[nCntModel].pMesh->UnlockVertexBuffer();
	}

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, MODEL_NAME, &g_pTexturePlayer2);
	
	//�I�D�t�Z�b�g
	g_Player2.aModel[0].nIdxModelParent = -1;
	g_Player2.aModel[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	g_Player2.aModel[1].nIdxModelParent = 0;
	g_Player2.aModel[1].pos = D3DXVECTOR3(0.0f, 15.0f, 8.0f);
	g_Player2.aModel[1].rot = D3DXVECTOR3(-150.0f, 0.0f, 0.0f);

	g_nIdxShadow2 = SetShadow(D3DXVECTOR3(0.0f, 0.0f, 0.0f), g_Player2.rot);
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer2(void)
{
	// ���b�V���̊J��
	if (g_pMeshPlayer2 != NULL)
	{
		g_pMeshPlayer2->Release();
		g_pMeshPlayer2 = NULL;
	}
	
	// �}�e���A���̊J��
	if (g_pBuffMatPlayer2 != NULL)
	{
		g_pBuffMatPlayer2->Release();
		g_pBuffMatPlayer2 = NULL;
	}

	//�e�N�X�`���̔j��
	if (g_pTexturePlayer2 != NULL)
	{
		g_pTexturePlayer2->Release();
		g_pTexturePlayer2 = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer2(void)
{
	CAMERA *pCamera;
	pCamera = GetCamera();

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	GAMESTATE GameState = GetGameState();

	float fData;

	g_Player2.posOld = g_Player2.pos;

	g_Player2.rot.y++;

	//pCamera->rot.y;
	
	/*if (GetKeyboardPress(DIK_RCONTROL) == true)
	{
		g_Player2.pos.y -= cosf(D3DX_PI * 0) * 4;
	}*/

	/*if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
		g_Player2.rot.y -= cosf(-D3DX_PI) * 6;
	}*/
	/*if (GetKeyboardPress(DIK_RSHIFT) == true)
	{
		g_Player2.rot.y -= cosf(D3DX_PI * 0) * 6;
	}*/
	g_BulletRecast2++;
	if (g_BulletRecast2 >= 150)
	{
		if (GetKeyboardTrigger(DIK_NUMPADENTER) == true)
		{
			PlaySound(SOUND_LABEL_SE_SHOT);
			fData = atan2f(pPlayer->pos.x - g_Player2.pos.x, pPlayer->pos.z - g_Player2.pos.z);
			SetBullet(D3DXVECTOR3(g_Player2.pos.x, g_Player2.pos.y + 5, g_Player2.pos.z), D3DXVECTOR3(sinf(fData) * 5.0f, tanf(fData) * 5.0f, cosf(fData) * 5.0f), 10.0f, 10.0f, BULLETTYPE_PLAYER2);
			g_BulletRecast2 = 0;
		}
	}

	if (g_Player2.pos.x  < 30)
	{
		g_Player2.pos.x = 80;
		CutLife2(1);
		PlaySound(SOUND_LABEL_SE_HIT000);
		g_Player2.nLife--;
	}
	if (g_Player2.pos.x > 500)
	{
		g_Player2.pos.x = 450;
		CutLife2(1);
		PlaySound(SOUND_LABEL_SE_HIT000);
		g_Player2.nLife--;
	}
	if (g_Player2.pos.z  > 50)
	{
		g_Player2.pos.z = 0;
		CutLife2(1);
		PlaySound(SOUND_LABEL_SE_HIT000);
		g_Player2.nLife--;
	}
	if (g_Player2.pos.z < -340)
	{
		g_Player2.pos.z = -260;
		CutLife2(1);
		PlaySound(SOUND_LABEL_SE_HIT000);
		g_Player2.nLife--;
	}

	/*if (g_Player2.nLife == 0)
	{
		if (GameState != GAMESTATE_P1WIN)
		{
			SetFade(MODE_P1WIN);
		}
	}*/

	if (GetKeyboardPress(DIK_RSHIFT) == false)
	{
		SetParticle(g_Player2.pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f), 10.0f, 100);
		//�΂߈ړ��̏���
		if (GetKeyboardPress(DIK_LEFT) == true)
		{
			if (GetKeyboardPress(DIK_UP) == true)
			{//����ړ�
				g_Player2.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.75f) * NORMAL_SPEED;
				g_Player2.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.75f) * NORMAL_SPEED;
				//g_Player2.rot.y = (pCamera->rot.y + D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_DOWN) == true)
			{//�����ړ�
				g_Player2.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.25f) * NORMAL_SPEED;
				g_Player2.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.25f) * NORMAL_SPEED;
				//g_Player2.rot.y = (pCamera->rot.y + D3DX_PI * 0.25f);
			}
			else
			{//���ړ�
				g_Player2.pos.x += sinf(pCamera->rot.y - D3DX_PI * 0.5f) * NORMAL_SPEED;
				g_Player2.pos.z += cosf(pCamera->rot.y - D3DX_PI * 0.5f) * NORMAL_SPEED;
				//g_Player2.rot.y = (pCamera->rot.y + D3DX_PI * 0.5f);
			}
		}

		//�΂߈ړ��̏���
		else if (GetKeyboardPress(DIK_RIGHT) == true)
		{
			if (GetKeyboardPress(DIK_UP) == true)
			{//�E��ړ�
				g_Player2.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.75f) * NORMAL_SPEED;
				g_Player2.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.75f) * NORMAL_SPEED;
				//g_Player2.rot.y = (pCamera->rot.y - D3DX_PI * 0.75f);
			}
			else if (GetKeyboardPress(DIK_DOWN) == true)
			{//�E���ړ�
				g_Player2.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.25f) * NORMAL_SPEED;
				g_Player2.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.25f) * NORMAL_SPEED;
				//g_Player2.rot.y = (pCamera->rot.y - D3DX_PI * 0.25f);
			}
			else
			{//�E�ړ�
				g_Player2.pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * NORMAL_SPEED;
				g_Player2.pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * NORMAL_SPEED;
				//g_Player2.rot.y = (pCamera->rot.y - D3DX_PI * 0.5f);
			}
		}
		else if (GetKeyboardPress(DIK_UP) == true)
		{
			g_Player2.pos.x += sinf(pCamera->rot.y) * NORMAL_SPEED;
			g_Player2.pos.z += cosf(pCamera->rot.y) * NORMAL_SPEED;
			//g_Player2.rot.y = (pCamera->rot.y + D3DX_PI * 1.0f);
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{
			g_Player2.pos.x -= sinf(pCamera->rot.y) * NORMAL_SPEED;
			g_Player2.pos.z -= cosf(pCamera->rot.y) * NORMAL_SPEED;
			//g_Player2.rot.y = (pCamera->rot.y + D3DX_PI * 0.0f);
		}

	}
	if (GetKeyboardPress(DIK_RSHIFT) == true)
	{
		//SetParticle(g_Player2.pos, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 20.0f, 100);
		////�΂߈ړ��̏���
		//if (GetKeyboardPress(DIK_LEFT) == true)
		//{
		//	if (GetKeyboardPress(DIK_UP) == true)
		//	{//����ړ�
		//		g_Player2.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.75f) * DASH_SPEED;
		//		g_Player2.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.75f) * DASH_SPEED;
		//		//g_Player2.rot.y = (pCamera->rot.y + D3DX_PI * 0.75f);
		//	}
		//	else if (GetKeyboardPress(DIK_DOWN) == true)
		//	{//�����ړ�
		//		g_Player2.pos.x -= sinf(pCamera->rot.y + D3DX_PI * 0.25f) * DASH_SPEED;
		//		g_Player2.pos.z -= cosf(pCamera->rot.y + D3DX_PI * 0.25f) * DASH_SPEED;
		//		//g_Player2.rot.y = (pCamera->rot.y + D3DX_PI * 0.25f);
		//	}
		//	else
		//	{//���ړ�
		//		g_Player2.pos.x += sinf(pCamera->rot.y - D3DX_PI * 0.5f) * DASH_SPEED;
		//		g_Player2.pos.z += cosf(pCamera->rot.y - D3DX_PI * 0.5f) * DASH_SPEED;
		//		//g_Player2.rot.y = (pCamera->rot.y + D3DX_PI * 0.5f);
		//	}
		//}

		////�΂߈ړ��̏���
		//else if (GetKeyboardPress(DIK_RIGHT) == true)
		//{
		//	if (GetKeyboardPress(DIK_UP) == true)
		//	{//�E��ړ�
		//		g_Player2.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.75f) * DASH_SPEED;
		//		g_Player2.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.75f) * DASH_SPEED;
		//		//g_Player2.rot.y = (pCamera->rot.y - D3DX_PI * 0.75f);
		//	}
		//	else if (GetKeyboardPress(DIK_DOWN) == true)
		//	{//�E���ړ�
		//		g_Player2.pos.x -= sinf(pCamera->rot.y - D3DX_PI * 0.25f) * DASH_SPEED;
		//		g_Player2.pos.z -= cosf(pCamera->rot.y - D3DX_PI * 0.25f) * DASH_SPEED;
		//		//g_Player2.rot.y = (pCamera->rot.y - D3DX_PI * 0.25f);
		//	}
		//	else
		//	{//�E�ړ�
		//		g_Player2.pos.x += sinf(pCamera->rot.y + D3DX_PI * 0.5f) * DASH_SPEED;
		//		g_Player2.pos.z += cosf(pCamera->rot.y + D3DX_PI * 0.5f) * DASH_SPEED;
		//		//g_Player2.rot.y = (pCamera->rot.y - D3DX_PI * 0.5f);
		//	}
		//}
		//else if (GetKeyboardPress(DIK_UP) == true)
		//{
		//	g_Player2.pos.x += sinf(pCamera->rot.y) * DASH_SPEED;
		//	g_Player2.pos.z += cosf(pCamera->rot.y) * DASH_SPEED;
		//	//g_Player2.rot.y = (pCamera->rot.y + D3DX_PI * 1.0f);
		//}
		//else if (GetKeyboardPress(DIK_DOWN) == true)
		//{
		//	g_Player2.pos.x -= sinf(pCamera->rot.y) * DASH_SPEED;
		//	g_Player2.pos.z -= cosf(pCamera->rot.y) * DASH_SPEED;
		//	//g_Player2.rot.y = (pCamera->rot.y + D3DX_PI * 0.0f);
		//}
	}

		///*if (GetKeyboardTrigger(DIK_SPACE) == true)
		//{
		//	g_Player2.move.y -= cosf(-D3DX_PI) * 2;
		//}*/

	g_Player2.move.y -= 0.1f;

	if (g_Player2.pos.y < 1.0f)
	{
	SetExplosion(g_Player2.pos, D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
	g_Player2.pos.y = 1;
	g_Player2.move.y = 0.0f;
	}

	g_Player2.pos.x += g_Player2.move.x;
	g_Player2.pos.z += g_Player2.move.z;
	g_Player2.pos.y += g_Player2.move.y;

	if (CollisionPlayer(&g_Player2.pos, &g_Player2.posOld, &g_Player2.move, g_Player2.VtxMax, g_Player2.VtxMin) == true)
	{
		
	}

	if (CollisionBuilding(&g_Player2.pos, &g_Player2.posOld, &g_Player2.move, g_Player2.VtxMax, g_Player2.VtxMin) == true)
	{

	}

	g_Player2.move.x += (0.0f - g_Player2.move.x) * 0.15f;
	g_Player2.move.z += (0.0f - g_Player2.move.z) * 0.15f;

	SetPositionShadow(g_nIdxShadow2, D3DXVECTOR3(g_Player2.pos.x, 0.1f, g_Player2.pos.z));
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;
	D3DMATERIAL9 matDef;
	D3DXMATERIAL *pMat;
	D3DXMATRIX mtxParent;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Player2.mtxWorld);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player2.rot.y, g_Player2.rot.x, g_Player2.rot.z);

	D3DXMatrixMultiply(&g_Player2.mtxWorld, &g_Player2.mtxWorld, &mtxRot);
	
	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, g_Player2.pos.x, g_Player2.pos.y, g_Player2.pos.z);

	D3DXMatrixMultiply(&g_Player2.mtxWorld, &g_Player2.mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Player2.mtxWorld);
	for (int nCnt = 0; nCnt < MAX_PARTS; nCnt++)
	{
		if (g_Player2.aModel[nCnt].nIdxModelParent == -1)
		{
			mtxParent = g_Player2.mtxWorld;
		}
		else
		{
			mtxParent = g_Player2.aModel[g_Player2.aModel[nCnt].nIdxModelParent].mtxWorld;
		}
		//=============================================================================
		// �e�̃��f��
		//=============================================================================
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_Player2.aModel[nCnt].mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player2.aModel[nCnt].rot.y, g_Player2.aModel[nCnt].rot.x, g_Player2.aModel[nCnt].rot.z);

		D3DXMatrixMultiply(&g_Player2.aModel[nCnt].mtxWorld, &g_Player2.aModel[nCnt].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_Player2.aModel[nCnt].pos.x, g_Player2.aModel[nCnt].pos.y, g_Player2.aModel[nCnt].pos.z);

		D3DXMatrixMultiply(&g_Player2.aModel[nCnt].mtxWorld, &g_Player2.aModel[nCnt].mtxWorld, &mtxTrans);

		D3DXMatrixMultiply(&g_Player2.aModel[nCnt].mtxWorld, &g_Player2.aModel[nCnt].mtxWorld, &mtxParent);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_Player2.aModel[nCnt].mtxWorld);


		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_Player2.aModel[nCnt].pBuffMat->GetBufferPointer();


		for (int nCntMat = 0; nCntMat < (int)g_Player2.aModel[nCnt].nNumMat; nCntMat++)
		{
			// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̓ǂݍ���
			pDevice->SetTexture(0, g_pTexturePlayer2);

			// ���f��(�p�[�c)�̕`��
			g_Player2.aModel[nCnt].pMesh->DrawSubset(nCntMat);
		}
		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);
	}
	//=============================================================================
	// �q�̃��f��
	//=============================================================================
}

PLAYER2 *GetPlayer2(void)
{
	return &g_Player2;
}

//*****************************************************************************
//���f���Ƃ̓����蔻�菈��
//*****************************************************************************
bool CollisionPlayer2(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pModel*/)
{
	bool bLand = false;

	//GAMESTATE GameState = GetGameState();



	for (int nCntModel = 0; nCntModel < MAX_PARTS; nCntModel++)
	{
		if (g_Player2.bUse == true)
		{
			//���f����Y���̒�
			if (pPos->y < g_Player2.pos.y + g_Player2.VtxMax.y
				&& pPos->y > g_Player2.pos.y + g_Player2.VtxMin.y)
			{
				//���f����Z���̒�
				if (pPos->z + VtxMin.z < g_Player2.pos.z + g_Player2.VtxMax.z
					&& pPos->z + VtxMax.z > g_Player2.pos.z + g_Player2.VtxMin.z)
				{
					//���f����X���̒�
					if (pPos->x + VtxMin.x < g_Player2.pos.x + g_Player2.VtxMax.x
						&& pPos->x + VtxMax.x > g_Player2.pos.x + g_Player2.VtxMin.x)
					{

						//X���̓����蔻��
						if (pPosOld->x > g_Player2.pos.x + g_Player2.VtxMax.x)
						{
							g_Player2.move.x += pMove->x-15;
							
							//g_Player2.move.x += bMove->x;

							pPos->x = g_Player2.pos.x + g_Player2.VtxMax.x + VtxMax.x;
							pPosOld->x = g_Player2.pos.x + g_Player2.VtxMax.x + VtxMax.x;
							//pMove->x = 0;
							bLand = true;
						}
						else if (pPosOld->x < g_Player2.pos.x + g_Player2.VtxMin.x)
						{
							if (g_Player2.nType == 1)
							{

							}

							g_Player2.move.x += pMove->x+15;
							pPos->x = g_Player2.pos.x + g_Player2.VtxMin.x + VtxMin.x;
							pPosOld->x = g_Player2.pos.x + g_Player2.VtxMin.x + VtxMin.x;
							//pMove->x = 0;
							bLand = true;

						}
						//Z���̓����蔻��
						if (pPosOld->z > g_Player2.pos.z + g_Player2.VtxMax.z)
						{
							if (g_Player2.nType == 1)
							{

							}
							bLand = true;

							g_Player2.move.z += pMove->z-15;

							pPos->z = g_Player2.pos.z + g_Player2.VtxMax.z + VtxMax.z;
							pPosOld->z = g_Player2.pos.z + g_Player2.VtxMax.z + VtxMax.z;
							//pMove->z = 0;
						}
						else if (pPosOld->z < g_Player2.pos.z + g_Player2.VtxMin.z)
						{
							if (g_Player2.nType == 1)
							{

							}
							bLand = true;

							g_Player2.move.z += pMove->z+15;

							pPos->z = g_Player2.pos.z + g_Player2.VtxMin.z + VtxMin.z;
							pPosOld->z = g_Player2.pos.z + g_Player2.VtxMin.z + VtxMin.z;
							//pMove->z = 0;
						}
						//Y���̓����蔻��
						if (pPosOld->y >= g_Player2.pos.y + g_Player2.VtxMax.y)
						{
							pPos->y = g_Player2.pos.y + g_Player2.VtxMax.y;
							pPosOld->y = g_Player2.pos.y + g_Player2.VtxMax.y;
							pMove->y = 0;

						}
						else if (pPosOld->y <= g_Player2.pos.y + g_Player2.VtxMin.y)
						{
							pPos->y = g_Player2.pos.y + g_Player2.VtxMin.y;
							pPosOld->y = g_Player2.pos.y + g_Player2.VtxMin.y;
						}
						bLand = true;
					}
				}
			}
		}
	}
	return bLand;
}