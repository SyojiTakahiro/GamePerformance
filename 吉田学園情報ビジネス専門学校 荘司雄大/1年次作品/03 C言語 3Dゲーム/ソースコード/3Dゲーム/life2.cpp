//=============================================================================
//
// ���C�t���� [life.cpp]
// Author : Takahiro
//
//=============================================================================
#include "game.h"
#include "player2.h"
#include "life2.h"
#include "particle.h"
#include "fade.h"
#include "sound.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_TIMER2		"data/TEXTURE/life001.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define TEXTURE_SIZE2        (20)                            // �e�N�X�`���̃T�C�Y
#define MAX_LIFE2			 (3)                             // �v���C���[�̃��C�t��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexLife2(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureLife2 = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife2 = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
LIFE2                    g_life2[MAX_LIFE2];		// ���C�t
int                     g_nCounterLife2;         // ���݂̃��C�t��
float g_lSpin2 = 1.01f;

//=============================================================================
// ����������
//=============================================================================
void InitLife2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntLife = 0; nCntLife < MAX_LIFE2; nCntLife++)
	{// ���C�t�A�C�R���������J��Ԃ�
		g_life2[nCntLife].pos = D3DXVECTOR3(1100, 40, 0);   // ���W��������
		g_life2[nCntLife].bUse = true;                     // �g�p���Ă����Ԃɂ���
	}
	g_nCounterLife2 = MAX_LIFE2;							  // ���݂̃��C�t����������

														  // �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					  // �f�o�C�X�ւ̃|�C���^
		TEXTURE_TIMER2,									  // �t�@�C���̖��O
		&g_pTextureLife2);								  // �e�N�X�`���ւ̃|�C���^

	// ���_���̍쐬
	MakeVertexLife2(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitLife2(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureLife2 != NULL)
	{
		g_pTextureLife2->Release();
		g_pTextureLife2 = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffLife2 != NULL)
	{
		g_pVtxBuffLife2->Release();
		g_pVtxBuffLife2 = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateLife2(void)
{

	/*if (g_nCounterLife2 == 0)
	{
		if (GameState != GAMESTATE_P2WIN)
		{
			SetFade(MODE_P2WIN);
		}
	}*/
	//int nCntLife;
	//VERTEX_2D*pVtx;
	//float pos = 0;

	////���_�o�b�t�@�����b�N
	//g_pVtxBuffLife2->Lock(0, 0, (void**)&pVtx, 0);

	//for (nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	//{
	//	if (g_life2[nCntLife].bUse == true)
	//	{
	//		pVtx[nCntLife * 4].tex = D3DXVECTOR2(0.0f, 0.0f);
	//		pVtx[nCntLife * 4+1].tex = D3DXVECTOR2(1.0f, 0.0f);
	//		pVtx[nCntLife * 4+2].tex = D3DXVECTOR2(0.0f, 1.0f);
	//		pVtx[nCntLife * 4+3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//	}
		//for (int nCntParticle = 0; nCntParticle < 50; nCntParticle++)
		//{// �p�[�e�B�N������
		//	D3DXVECTOR3 movePar(0.0f, 1.0f, 0.0f);
		//	movePar.x = sinf(D3DX_PI * g_lSpin) * (rand() % 10 * 1.5f);
		//	movePar.y = cosf(D3DX_PI * g_lSpin) * (rand() % 3);
		//	SetParticle(g_life2[nCntLife].pos, movePar, D3DXCOLOR(1.0f, 0.1f, 0.1f, 1.0f), rand() % 20, rand() % 15);
		//}
	//}
	////�A�����b�N
	//g_pVtxBuffLife2->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawLife2(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLife2, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLife2);

	// �|���S���̕`��
	for (int nCntLife = 0; nCntLife < MAX_LIFE2; nCntLife++)
	{// �\�����郉�C�t�A�C�R���������J��Ԃ�
		if (g_life2[nCntLife].bUse == true)
		{// ���C�t���g�p����Ă���
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntLife * 4,
				2);
		}
	}
}
//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexLife2(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife2,
		NULL);

	// ���_�o�b�t�@�𐶐�
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffLife2->Lock(0, 0, (void**)&pVtx, 0);

	float pos = 0;
	// ���_����ݒ�
	for (int nCntLife = 0; nCntLife < MAX_LIFE2; nCntLife++)
	{// ���C�t�A�C�R���������J��Ԃ�
	 // ���_���W
		pVtx[0].pos = D3DXVECTOR3(g_life2[nCntLife].pos.x - TEXTURE_SIZE2 + pos, g_life2[nCntLife].pos.y - TEXTURE_SIZE2, g_life2[nCntLife].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_life2[nCntLife].pos.x + TEXTURE_SIZE2 + pos, g_life2[nCntLife].pos.y - TEXTURE_SIZE2, g_life2[nCntLife].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_life2[nCntLife].pos.x - TEXTURE_SIZE2 + pos, g_life2[nCntLife].pos.y + TEXTURE_SIZE2, g_life2[nCntLife].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_life2[nCntLife].pos.x + TEXTURE_SIZE2 + pos, g_life2[nCntLife].pos.y + TEXTURE_SIZE2, g_life2[nCntLife].pos.z);


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
	g_pVtxBuffLife2->Unlock();
}
//=============================================================================
// ���C�t�̌���
//=============================================================================
void CutLife2(int cut)
{
	GAMESTATE GameState;
	GameState = GetGameState();

	for (int nCntLife = 0; nCntLife < MAX_LIFE2; nCntLife++)
	{// ���C�t�A�C�R���������J��Ԃ�
		if (g_life2[nCntLife].bUse == true)
		{// ���C�t���g�p����Ă���
			g_life2[nCntLife].bUse = false;
			g_nCounterLife2--;
			break;
		}

		if (g_nCounterLife2 == 1)
		{
			if (GameState != GAMESTATE_P1WIN)
			{
				SetFade(MODE_P1WIN);
				StopSound(SOUND_LABEL_BGM001);
			}
		}
	}
}