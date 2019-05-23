//=============================================================================
//
// ���f������ [model.cpp]
// Author : 
//
//=============================================================================
#include "building.h"
#include "input.h"
//#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "polygon.h"
#include "player.h"
#include "player2.h"
#include "game.h"
#include "explosion.h"
//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPD3DXMESH g_pMeshBuilding[MAX_MODEL];					//���b�V�����ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatBuilding[MAX_MODEL];			//�}�e���A�����ւ̃|�C���^
LPDIRECT3DTEXTURE9	g_pTextureBuilding[MAX_MODEL];		//�e�N�X�`���ւ̃|�C���^
DWORD g_nNumMatBuilding = 0;							//�}�e���A�����̐�
BUILDING g_aBuilding[MAX_MODEL];
VTX g_aBuildingVtx[MAX_MODEL_TYPE];
//=============================================================================
// ����������
//=============================================================================
void InitBuilding(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_NAME0,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatBuilding[0],
		NULL,
		&g_nNumMatBuilding,
		&g_pMeshBuilding[0]);

	// X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(MODEL_NAME1,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatBuilding[1],
		NULL,
		&g_nNumMatBuilding,
		&g_pMeshBuilding[1]);



	int nNumVtex;	//���_��
	DWORD sizeFvF;	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff[MAX_MODEL];	//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 vtx[MAX_MODEL];

	for (int nCntBuilding = 0; nCntBuilding < MAX_MODEL; nCntBuilding++)
	{
		g_aBuilding[nCntBuilding].VtxMinBuilding = D3DXVECTOR3(10000, 0, 10000);
		g_aBuilding[nCntBuilding].VtxMaxBuilding = D3DXVECTOR3(-10000, 0, -10000);
		g_aBuilding[nCntBuilding].nType = 0;
		g_aBuilding[nCntBuilding].bUse = false;
	}
	for (int nCntTypeBuilding = 0; nCntTypeBuilding < MAX_MODEL_TYPE; nCntTypeBuilding++)
	{
		g_aBuildingVtx[nCntTypeBuilding].VtxMaxBuilding = D3DXVECTOR3(-10000, 0, -10000);
		g_aBuildingVtx[nCntTypeBuilding].VtxMinBuilding = D3DXVECTOR3(10000, 0, 10000);
	}

	//���_�����擾
	nNumVtex = g_pMeshBuilding[0]->GetNumVertices();
	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFvF = D3DXGetFVFVertexSize(g_pMeshBuilding[0]->GetFVF());

	for (int nCntBuilding = 0; nCntBuilding < MAX_MODEL; nCntBuilding++)
	{
		//���_�@���b�N
		g_pMeshBuilding[0]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nCntBuilding]);
		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
		{	//���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff[nCntBuilding];
			if (g_aBuilding[nCntBuilding].VtxMinBuilding.x > vtx.x)
			{
				g_aBuilding[nCntBuilding].VtxMinBuilding.x = vtx.x;
			}
			if (g_aBuilding[nCntBuilding].VtxMinBuilding.y > vtx.y)
			{
				g_aBuilding[nCntBuilding].VtxMinBuilding.y = vtx.y;
			}
			if (g_aBuilding[nCntBuilding].VtxMinBuilding.z > vtx.z)
			{
				g_aBuilding[nCntBuilding].VtxMinBuilding.z = vtx.z;
			}

			if (g_aBuilding[nCntBuilding].VtxMaxBuilding.x < vtx.x)
			{
				g_aBuilding[nCntBuilding].VtxMaxBuilding.x = vtx.x;
			}
			if (g_aBuilding[nCntBuilding].VtxMaxBuilding.y < vtx.y)
			{
				g_aBuilding[nCntBuilding].VtxMaxBuilding.y = vtx.y;
			}
			if (g_aBuilding[nCntBuilding].VtxMaxBuilding.z < vtx.z)
			{
				g_aBuilding[nCntBuilding].VtxMaxBuilding.z = vtx.z;
			}
			//��������
#if 0
			//			//���_�o�b�t�@�����b�N
			//			g_pMeshBuilding[0]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
			//			for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
			//			{	//���_���W�̑��
			//
			//			vtx[nCntBuilding] = *(D3DXVECTOR3*)pVtxBuff;
			//
			//			if (g_aBuilding[nCntBuilding].VtxMinBuilding.x > vtx[nCntBuilding].x)
			//			{
			//				g_aBuilding[nCntBuilding].VtxMinBuilding.x = vtx[nCntBuilding].x;
			//			}
			//			if (g_aBuilding[nCntBuilding].VtxMinBuilding.y > vtx[nCntBuilding].y)
			//			{
			//				g_aBuilding[nCntBuilding].VtxMinBuilding.y = vtx[nCntBuilding].y;
			//			}
			//			if (g_aBuilding[nCntBuilding].VtxMinBuilding.z > vtx[nCntBuilding].z)
			//			{
			//				g_aBuilding[nCntBuilding].VtxMinBuilding.z = vtx[nCntBuilding].z;
			//			}
			//
			//			if (g_aBuilding[nCntBuilding].VtxMaxBuilding.x < vtx[nCntBuilding].x)
			//			{
			//				g_aBuilding[nCntBuilding].VtxMaxBuilding.x = vtx[nCntBuilding].x;
			//			}
			//			if (g_aBuilding[nCntBuilding].VtxMaxBuilding.y < vtx[nCntBuilding].y)
			//			{
			//				g_aBuilding[nCntBuilding].VtxMaxBuilding.y = vtx[nCntBuilding].y;
			//			}
			//			if (g_aBuilding[nCntBuilding].VtxMaxBuilding.z < vtx[nCntBuilding].z)
			//			{
			//				g_aBuilding[nCntBuilding].VtxMaxBuilding.z = vtx[nCntBuilding].z;
			//			}
			//
			//			//�T�C�Y���|�C���^��i�߂�
			//			pVtxBuff += sizeFvF;
#endif

			pVtxBuff[nCntBuilding] += sizeFvF;
		}
		//			//���_�o�b�t�@���A�����b�N
		g_pMeshBuilding[0]->UnlockVertexBuffer();
	}


	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		MODEL_NAME0,
		&g_pTextureBuilding[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		MODEL_NAME1,
		&g_pTextureBuilding[1]);

	//float testpos = 100;
	//for (int nCntBuilding = 0; nCntBuilding < MAX_MODEL; nCntBuilding++)
	//{
	//	// �ʒu�E�����̏����ݒ�
	//	g_aBuilding[nCntBuilding].pos = D3DXVECTOR3(testpos, 0, 0);
	//	g_aBuilding[nCntBuilding].rot = D3DXVECTOR3(0, 0, 0);
	//	g_aBuilding[nCntBuilding].fShadow = 12;
	////	g_aBuilding[nCntBuilding].fDestAngle = 0;
	//	//g_aBuilding
	//	//�@�e�̃Z�b�g
	//	g_aBuilding[nCntBuilding].nIdxShadow = SetShadow(g_aBuilding[nCntBuilding].pos, D3DXVECTOR3(0, 0, 0), 20.0f);

	//	testpos -= 100;
	//	g_aBuilding[nCntBuilding].bUse = true;
	//}
}

//=============================================================================
// �I������
//=============================================================================
void UninitBuilding(void)
{
	for (int nCntBuilding = 0; nCntBuilding < MAX_MODEL; nCntBuilding++)
	{
		// ���b�V���̊J��
		if (g_pMeshBuilding[nCntBuilding] != NULL)
		{
			g_pMeshBuilding[nCntBuilding]->Release();
			g_pMeshBuilding[nCntBuilding] = NULL;
		}
		// �}�e���A���̊J��
		if (g_pBuffMatBuilding[nCntBuilding] != NULL)
		{
			g_pBuffMatBuilding[nCntBuilding]->Release();
			g_pBuffMatBuilding[nCntBuilding] = NULL;
		}
		//�e�N�X�`���̔j��
		if (g_pTextureBuilding[nCntBuilding] != NULL)
		{
			g_pTextureBuilding[nCntBuilding]->Release();
			g_pTextureBuilding[nCntBuilding] = NULL;
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBuilding(void)
{
	//PLAYER *pPlayer = GetPlayer();

	//GAMESTATE GameState;
	//GameState = GetGameState();
	for (int nCntBuilding = 0; nCntBuilding < MAX_MODEL; nCntBuilding++)
	{
		g_aBuilding[nCntBuilding].rot.y++;
	}

	//for (int nCntBuilding = 0; nCntBuilding < MAX_MODEL; nCntBuilding++)
	//{
	//	if (g_aBuilding[nCntBuilding].bUse == true)
	//	{
	//		//���f����Y���̒�
	//		if (pPlayer->pos.y < g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMaxBuilding.y
	//			&& pPlayer->pos.y > g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMinBuilding.y)
	//		{
	//			//���f����Z���̒�
	//			if (pPlayer->pos.z < g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMaxBuilding.z
	//				&& pPlayer->pos.z > g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMinBuilding.z)
	//			{
	//				//���f����X���̒�
	//				if (pPlayer->pos.x < g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMaxBuilding.x
	//					&& pPlayer->pos.x > g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMinBuilding.x)
	//				{

	//					//X���̓����蔻��
	//					if (pPlayer->posold.x >= g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMaxBuilding.x)
	//					{
	//						if (GameState != GAMESTATE_GAMECLER)
	//						{
	//							SetGameState(GAMESTATE_GAMECLER);
	//						}

	//						pPlayer->pos.x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMaxBuilding.x;
	//						pPlayer->posold.x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMaxBuilding.x;

	//					}
	//					else if (pPlayer->posold.x <= g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMinBuilding.x)
	//					{
	//						if (GameState != GAMESTATE_GAMECLER)
	//						{
	//							SetGameState(GAMESTATE_GAMECLER);
	//						}

	//						pPlayer->pos.x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMinBuilding.x;
	//						pPlayer->posold.x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMinBuilding.x;
	//					}
	//					//Z���̓����蔻��
	//					if (pPlayer->posold.z >= g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMaxBuilding.z)
	//					{
	//						if (GameState != GAMESTATE_GAMEOVER)
	//						{
	//							SetGameState(GAMESTATE_GAMEOVER);
	//						}
	//						pPlayer->pos.z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMaxBuilding.z;
	//						pPlayer->posold.z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMaxBuilding.z;

	//					}
	//					else if (pPlayer->posold.z <= g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMinBuilding.z)
	//					{
	//						if (GameState != GAMESTATE_GAMEOVER)
	//						{
	//							SetGameState(GAMESTATE_GAMEOVER);
	//						}
	//						pPlayer->pos.z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMinBuilding.z;
	//						pPlayer->posold.z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMinBuilding.z;
	//					}
	//					//Y���̓����蔻��
	//					if (pPlayer->posold.y >= g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMaxBuilding.y)
	//					{
	//						pPlayer->pos.y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMaxBuilding.y;
	//						pPlayer->posold.y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMaxBuilding.y;

	//					}
	//					else if (pPlayer->posold.y <= g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMinBuilding.y)
	//					{
	//						pPlayer->pos.y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMinBuilding.y;
	//						pPlayer->posold.y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMinBuilding.y;
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawBuilding(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntBuilding = 0; nCntBuilding < MAX_MODEL; nCntBuilding++)
	{		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aBuilding[nCntBuilding].mtxWorld);
		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBuilding[nCntBuilding].rot.y, g_aBuilding[nCntBuilding].rot.x, g_aBuilding[nCntBuilding].rot.z);
		D3DXMatrixMultiply(&g_aBuilding[nCntBuilding].mtxWorld, &g_aBuilding[nCntBuilding].mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_aBuilding[nCntBuilding].pos.x, g_aBuilding[nCntBuilding].pos.y, g_aBuilding[nCntBuilding].pos.z);
		D3DXMatrixMultiply(&g_aBuilding[nCntBuilding].mtxWorld, &g_aBuilding[nCntBuilding].mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aBuilding[nCntBuilding].mtxWorld);
		// ���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);
		// �}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatBuilding[g_aBuilding[nCntBuilding].nType]->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_nNumMatBuilding; nCntMat++)
		{	// �}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			// �e�N�X�`��
			pDevice->SetTexture(0, g_pTextureBuilding[g_aBuilding[nCntBuilding].nType]);
			if (g_aBuilding[nCntBuilding].bUse == true)	//���f�����g�p����Ă���
			{
				// ���f��(�p�[�c)�̕`��
				g_pMeshBuilding[g_aBuilding[nCntBuilding].nType]->DrawSubset(nCntMat);
			}
		}
		// �}�e���A�����f�t�H���g�ɖ߂�
		pDevice->SetMaterial(&matDef);
	}
}

//*****************************************************************************
//���f���|�C���^����
//*****************************************************************************
BUILDING *GetBuilding(void)
{
	return &g_aBuilding[0];
}
//*****************************************************************************
//���f���Ƃ̓����蔻�菈��
//*****************************************************************************
//bool CollisionBuilding(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pBuilding*/)
//{
//	bool bLand = false;
//
//	GAMESTATE GameState = GetGameState();
//
//	for (int nCntBuilding = 0; nCntBuilding < MAX_MODEL; nCntBuilding++)
//	{
//		if (g_aBuilding[nCntBuilding].bUse == true)
//		{
//			//���f����Y���̒�
//			if (pPos->y < g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMaxBuilding.y
//				&& pPos->y > g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMinBuilding.y)
//			{
//				//���f����Z���̒�
//				if (pPos->z + VtxMin.z < g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMaxBuilding.z
//					&& pPos->z + VtxMax.z > g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMinBuilding.z)
//				{
//					//���f����X���̒�
//					if (pPos->x + VtxMin.x < g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMaxBuilding.x
//						&& pPos->x + VtxMax.x > g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMinBuilding.x)
//					{
//
//						//X���̓����蔻��
//						if (pPosOld->x > g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMaxBuilding.x)
//						{
//
//							pPos->x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMaxBuilding.x + VtxMax.x;
//							pPosOld->x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMaxBuilding.x + VtxMax.x;
//							//pMove->x = 0;
//							bLand = true;
//						}
//						else if (pPosOld->x < g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMinBuilding.x)
//						{
//							
//							pPos->x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMinBuilding.x + VtxMin.x;
//							pPosOld->x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMinBuilding.x + VtxMin.x;
//							//pMove->x = 0;
//							bLand = true;
//
//						}
//						//Z���̓����蔻��
//						if (pPosOld->z > g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMaxBuilding.z)
//						{
//							if (g_aBuilding[nCntBuilding].nType == 1)
//							{
//								
//							}
//							bLand = true;
//
//							pPos->z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMaxBuilding.z + VtxMax.z;
//							pPosOld->z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMaxBuilding.z + VtxMax.z;
//							//pMove->z = 0;
//						}
//						else if (pPosOld->z < g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMinBuilding.z)
//						{
//							if (g_aBuilding[nCntBuilding].nType == 1)
//							{
//								
//							}
//							bLand = true;
//
//							pPos->z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMinBuilding.z + VtxMin.z;
//							pPosOld->z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMinBuilding.z + VtxMin.z;
//							//pMove->z = 0;
//						}
//						//Y���̓����蔻��
//						if (pPosOld->y >= g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMaxBuilding.y)
//						{
//							pPos->y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMaxBuilding.y;
//							pPosOld->y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMaxBuilding.y;
//							pMove->y = 0;
//
//						}
//						else if (pPosOld->y <= g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMinBuilding.y)
//						{
//							pPos->y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMinBuilding.y;
//							pPosOld->y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMinBuilding.y;
//						}
//						bLand = true;
//					}
//				}
//			}
//		}
//	}
//	return bLand;
//}

bool CollisionBuilding(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 VtxMax, D3DXVECTOR3 VtxMin/*, MODEL **pBuilding*/)
{
	bool bLand = false;

	GAMESTATE GameState;
	GameState = GetGameState();

	PLAYER *pPlayer = GetPlayer();
	PLAYER2 *pPlayer2 = GetPlayer2();

	for (int nCntBuilding = 0; nCntBuilding < MAX_MODEL; nCntBuilding++)
	{
		if (g_aBuilding[nCntBuilding].bUse == true)
		{
#if 0
			//���f����Y���̒�
			if (pPos->y < g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMaxBuilding.y
				&& pPos->y > g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMinBuilding.y)
			{
				//���f����Z���̒�
				if (pPos->z + VtxMin.z < g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMaxBuilding.z
					&& pPos->z + VtxMax.z > g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMinBuilding.z)
				{
					//���f����X���̒�
					if (pPos->x + VtxMin.x < g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMaxBuilding.x
						&& pPos->x + VtxMax.x > g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMinBuilding.x)
					{
#if 1
						if (g_aBuilding[nCntBuilding].nType == 1)
						{

							//X���̓����蔻��
							if (pPosOld->x + 1 > g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMaxBuilding.x)
							{
								pPos->x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMaxBuilding.x + VtxMax.x;
								pPosOld->x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMaxBuilding.x + VtxMax.x;
							}
							else if (pPosOld->x - 1 < g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMinBuilding.x)
							{
								pPos->x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMinBuilding.x + VtxMin.x;
								pPosOld->x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[nCntBuilding].VtxMinBuilding.x + VtxMin.x;
							}

							//Z���̓����蔻��
							if (pPosOld->z + 1 > g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMaxBuilding.z)
							{
								pPos->z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMaxBuilding.z + VtxMax.z;
								pPosOld->z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMaxBuilding.z + VtxMax.z;
							}
							else if (pPosOld->z - 1 < g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMinBuilding.z)
							{
								pPos->z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMinBuilding.z + VtxMin.z;
								pPosOld->z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[nCntBuilding].VtxMinBuilding.z + VtxMin.z;
							}
							//Y���̓����蔻��
							if (pPosOld->y >= g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMaxBuilding.y)
							{
								pPos->y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMaxBuilding.y;
								pPosOld->y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMaxBuilding.y;

							}
							else if (pPosOld->y <= g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMinBuilding.y)
							{
								pPos->y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMinBuilding.y;
								pPosOld->y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[nCntBuilding].VtxMinBuilding.y;
							}
#endif
						}
					}
				}
			}
#endif

#if 1
			//���f����Y���̒�
			if (pPos->y < g_aBuilding[nCntBuilding].pos.y + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMaxBuilding.y
				&& pPos->y > g_aBuilding[nCntBuilding].pos.y + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMinBuilding.y)
			{
				//���f����Z���̒�
				if (pPos->z + VtxMin.z  < g_aBuilding[nCntBuilding].pos.z + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMaxBuilding.z
					&& pPos->z + VtxMax.z > g_aBuilding[nCntBuilding].pos.z + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMinBuilding.z)
				{
					//���f����X���̒�
					if (pPos->x + VtxMin.x < g_aBuilding[nCntBuilding].pos.x + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMaxBuilding.x
						&& pPos->x + VtxMax.x > g_aBuilding[nCntBuilding].pos.x + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMinBuilding.x)
					{
#if 1
						//X���̓����蔻��
						if (pPosOld->x - 10 > g_aBuilding[nCntBuilding].pos.x + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMaxBuilding.x)
						{
							pPos->x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMaxBuilding.x + VtxMax.x;
							pPosOld->x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMaxBuilding.x + VtxMax.x;
						}
						else if (pPosOld->x + 10 < g_aBuilding[nCntBuilding].pos.x + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMinBuilding.x)
						{
							pPos->x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMinBuilding.x + VtxMin.x;
							pPosOld->x = g_aBuilding[nCntBuilding].pos.x + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMinBuilding.x + VtxMin.x;
						}

						//Z���̓����蔻��
						else if (pPosOld->z + 5 > g_aBuilding[nCntBuilding].pos.z + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMaxBuilding.z)
						{
							pPos->z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMaxBuilding.z + VtxMax.z;
							pPosOld->z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMaxBuilding.z + VtxMax.z;
						}
						else if (pPosOld->z + 10 < g_aBuilding[nCntBuilding].pos.z + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMinBuilding.z)
						{
							pPos->z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMinBuilding.z + VtxMin.z;
							pPosOld->z = g_aBuilding[nCntBuilding].pos.z + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMinBuilding.z + VtxMin.z;
						}
						//Y���̓����蔻��
						else if (pPosOld->y >= g_aBuilding[nCntBuilding].pos.y + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMaxBuilding.y)
						{
							pPos->y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMaxBuilding.y;
							pPosOld->y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMaxBuilding.y;

							pPlayer->move.y = 0;
							pPlayer2->move.y = 0;
						}
						else if (pPosOld->y <= g_aBuilding[nCntBuilding].pos.y + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMinBuilding.y)
						{
							pPos->y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMinBuilding.y;
							pPosOld->y = g_aBuilding[nCntBuilding].pos.y + g_aBuilding[g_aBuilding[nCntBuilding].nType].VtxMinBuilding.y;
						}
#endif
					}
				}
			}
#endif


		}
	}
	return bLand;
}


//=============================================================================
// �ݒ菈��
//=============================================================================
void SetBuilding(D3DXVECTOR3 pos, int nType)
{

	int nNumVtex;	//���_��
	DWORD sizeFvF[MAX_MODEL];	//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff[MAX_MODEL];	//���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 vtx[MAX_MODEL];

	//���_�����擾
	nNumVtex = g_pMeshBuilding[nType]->GetNumVertices();
	//���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFvF[nType] = D3DXGetFVFVertexSize(g_pMeshBuilding[nType]->GetFVF());

	for (int nCntBuilding = 0; nCntBuilding < MAX_MODEL; nCntBuilding++)
	{
		//if (g_aBuilding[nCntBuilding].bUse == false)
		//{
		//���_�@���b�N
		g_pMeshBuilding[nType]->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff[nType]);
		for (int nCntVtx = 0; nCntVtx < nNumVtex; nCntVtx++)
		{	//���_���W�̑��
			vtx[nCntBuilding] = *(D3DXVECTOR3*)pVtxBuff[nType];
			if (g_aBuilding[nCntBuilding].VtxMinBuilding.x > vtx[nCntBuilding].x)
			{
				g_aBuilding[nCntBuilding].VtxMinBuilding.x = vtx[nCntBuilding].x;
			}
			if (g_aBuilding[nCntBuilding].VtxMinBuilding.y > vtx[nCntBuilding].y)
			{
				g_aBuilding[nCntBuilding].VtxMinBuilding.y = vtx[nCntBuilding].y;
			}
			if (g_aBuilding[nCntBuilding].VtxMinBuilding.z > vtx[nCntBuilding].z)
			{
				g_aBuilding[nCntBuilding].VtxMinBuilding.z = vtx[nCntBuilding].z;
			}

			if (g_aBuilding[nCntBuilding].VtxMaxBuilding.x < vtx[nCntBuilding].x)
			{
				g_aBuilding[nCntBuilding].VtxMaxBuilding.x = vtx[nCntBuilding].x;
			}
			if (g_aBuilding[nCntBuilding].VtxMaxBuilding.y < vtx[nCntBuilding].y)
			{
				g_aBuilding[nCntBuilding].VtxMaxBuilding.y = vtx[nCntBuilding].y;
			}
			if (g_aBuilding[nCntBuilding].VtxMaxBuilding.z < vtx[nCntBuilding].z)
			{
				g_aBuilding[nCntBuilding].VtxMaxBuilding.z = vtx[nCntBuilding].z;
			}
			pVtxBuff[nCntBuilding] += sizeFvF[nType];
		}
		//���_�o�b�t�@���A�����b�N
		g_pMeshBuilding[nType]->UnlockVertexBuffer();
		//	break;
		//}
	}

	for (int nCntBuilding = 0; nCntBuilding < MAX_MODEL; nCntBuilding++)
	{
		if (g_aBuilding[nCntBuilding].bUse == false)
		{
			// �ʒu�E�����̏����ݒ�
			g_aBuilding[nCntBuilding].pos = pos;
			g_aBuilding[nCntBuilding].rot = D3DXVECTOR3(0, 0, 0);
			g_aBuilding[nCntBuilding].fShadow = 12;
			g_aBuilding[nCntBuilding].nType = nType;
			//�@�e�̃Z�b�g
			//g_aBuilding[nCntBuilding].nIdxShadow = SetShadow(g_aBuilding[nCntBuilding].pos, D3DXVECTOR3(0, 0, 0));
			g_aBuilding[nCntBuilding].bUse = true;
			break;
		}
	}

}