//=============================================================================
//
// ���C�t���� [life.cpp]
// Author : Takahiro
//
//=============================================================================
#include "game.h"
#include "player.h"
#include "cpulife.h"
#include "particle.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TIMER		"data/TEXTURE/life000.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_SIZE        (20)                            // �e�N�X�`���̃T�C�Y
#define MAX_LIFE			(3)                             // �v���C���[�̃��C�t��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexCPULife(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureCPULife = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCPULife = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
CPULIFE                    g_life[MAX_LIFE];		// ���C�t
int                     g_nCounterCPULife;         // ���݂̃��C�t��
float g_lCPUSpin = 1.01f;

//=============================================================================
// ����������
//=============================================================================
void InitCPULife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntCPULife = 0; nCntCPULife < MAX_LIFE; nCntCPULife++)
	{// ���C�t�A�C�R���������J��Ԃ�
		g_life[nCntCPULife].pos = D3DXVECTOR3(1100, 40, 0);   // ���W��������
		g_life[nCntCPULife].bUse = true;                     // �g�p���Ă����Ԃɂ���
	}
	g_nCounterCPULife = MAX_LIFE;							  // ���݂̃��C�t����������

														  // �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					  // �f�o�C�X�ւ̃|�C���^
		TEXTURE_TIMER,									  // �t�@�C���̖��O
		&g_pTextureCPULife);								  // �e�N�X�`���ւ̃|�C���^

														  // ���_���̍쐬
	MakeVertexCPULife(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitCPULife(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureCPULife != NULL)
	{
		g_pTextureCPULife->Release();
		g_pTextureCPULife = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffCPULife != NULL)
	{
		g_pVtxBuffCPULife->Release();
		g_pVtxBuffCPULife = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateCPULife(void)
{
	GAMESTATE GameState;
	GameState = GetGameState();

	//int nCntCPULife;
	//VERTEX_2D*pVtx;
	//float pos = 0;

	////���_�o�b�t�@�����b�N
	//g_pVtxBuffCPULife->Lock(0, 0, (void**)&pVtx, 0);

	//for (nCntCPULife = 0; nCntCPULife < MAX_LIFE; nCntCPULife++)
	//{
	//	if (g_life[nCntCPULife].bUse == true)
	//	{
	//		pVtx[nCntCPULife * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
	//		pVtx[nCntCPULife * 4+1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//		pVtx[nCntCPULife * 4+2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//		pVtx[nCntCPULife * 4+3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	}
	//for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
	//{// �p�[�e�B�N������
	//	D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);
	//	movePar.x = sinf(D3DX_PI * g_lSpin) * (rand() % 10 * 1.5f);
	//	movePar.y = cosf(D3DX_PI * g_lSpin) * (rand() % 3);
	//	SetParticle(g_life[nCntCPULife].pos, movePar, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), rand() % 20, rand() % 15);
	//}
	//}
	////�A�����b�N
	//g_pVtxBuffCPULife->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawCPULife(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCPULife, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureCPULife);

	// �|���S���̕`��
	for (int nCntCPULife = 0; nCntCPULife < MAX_LIFE; nCntCPULife++)
	{// �\�����郉�C�t�A�C�R���������J��Ԃ�
		if (g_life[nCntCPULife].bUse == true)
		{// ���C�t���g�p����Ă���
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntCPULife * 4,
				2);
		}
	}
}
//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexCPULife(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCPULife,
		NULL);

	// ���_�o�b�t�@�𐶐�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffCPULife->Lock(0, 0, (void**)&pVtx, 0);

	float pos = 0;
	// ���_����ݒ�
	for (int nCntCPULife = 0; nCntCPULife < MAX_LIFE; nCntCPULife++)
	{// ���C�t�A�C�R���������J��Ԃ�
	 // ���_���W
		pVtx[0].pos = D3DXVECTOR3(g_life[nCntCPULife].pos.x - TEXTURE_SIZE + pos, g_life[nCntCPULife].pos.y - TEXTURE_SIZE, g_life[nCntCPULife].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_life[nCntCPULife].pos.x + TEXTURE_SIZE + pos, g_life[nCntCPULife].pos.y - TEXTURE_SIZE, g_life[nCntCPULife].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_life[nCntCPULife].pos.x - TEXTURE_SIZE + pos, g_life[nCntCPULife].pos.y + TEXTURE_SIZE, g_life[nCntCPULife].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_life[nCntCPULife].pos.x + TEXTURE_SIZE + pos, g_life[nCntCPULife].pos.y + TEXTURE_SIZE, g_life[nCntCPULife].pos.z);


		// ���_�e�N�X�`��
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
		pos -= 40.0f;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCPULife->Unlock();
}
//=============================================================================
// ���C�t�̌���
//=============================================================================
void CutCPULife(int cut)
{

	GAMESTATE GameState;
	GameState = GetGameState();

	for (int nCntCPULife = 0; nCntCPULife < MAX_LIFE; nCntCPULife++)
	{// ���C�t�A�C�R���������J��Ԃ�
		if (g_life[nCntCPULife].bUse == true)
		{// ���C�t���g�p����Ă���
			g_life[nCntCPULife].bUse = false;
			g_nCounterCPULife--;
			break;
		}

		if (g_nCounterCPULife == 1)
		{
			if (GameState != GAMESTATE_P1WIN)
			{
				SetFade(MODE_P1WIN);
				StopSound(SOUND_LABEL_BGM001);
			}
		}
	}
}