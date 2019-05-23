//=============================================================================
//
// �|�[�Y���j���[���� [pauseselect.cpp]
// Author : 
//
//=============================================================================
#include "input.h"
#include "fade.h"
#include "modeselect.h" 
#include "player.h"
#include "t.h"
#include "sound.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_CONTINUE		"data/TEXTURE/1P.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RETRY			"data/TEXTURE/2P.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_QUIT			"data/TEXTURE/tutorial.png"	// �ǂݍ��ރe�N�X�`���t�@�C����
#define MODESELECT_POS_X			(120)						//���_���WX�̈ړ���
#define MODESELECT_POS_Y			(30)						//���_���WY�̈ړ���
//#define TEX_PS_X_INIT			(1.0f)						//�e�N�X�`�����WU�̏����ʒu
//#define TEX_PS_Y_INIT			(1.0f)						//�e�N�X�`�����WV�̏����ʒu
#define TEX_LEFT				(0.0f)						//�e�N�X�`�����WU��
#define TEX_RIGHT				(1.0f)						//�e�N�X�`�����WU�E
#define TEX_TOP					(0.0f)						//�e�N�X�`�����WV��
#define TEX_BOT					(1.0f)						//�e�N�X�`�����WV��
#define MAX_PAUSEMENU			(3)							//���j���[�̐�

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void MakeVertexModeSelect(LPDIRECT3DDEVICE9 pDevice);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureModeSelect[MAX_PAUSEMENU] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffModeSelect = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
MODESELECT				g_aModeMenu[MAX_PAUSEMENU];					// �|�[�Y���j���[
D3DXVECTOR3				g_ModeMenuPos;
GAMEMODESELECT			g_SelectModeT = GAMEMODESELECT_NONE;
int						g_nGameSelect;
//=============================================================================
// ����������
//=============================================================================
void InitModeSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	float fMenuPosY = 0;
	g_nGameSelect = 0;
	g_ModeMenuPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SelectModeT = GAMEMODESELECT_NONE;

	g_aModeMenu[0].pos = D3DXVECTOR3(650.0f, 470.0f + (fMenuPosY), 0.0f);
	g_aModeMenu[0].col = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f);
	g_aModeMenu[0].selectgame = SELECTGAMETYPE_SELECT;
	fMenuPosY += 150;
	for (int nCnt = 1; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		g_aModeMenu[nCnt].pos = D3DXVECTOR3(650.0f, 400.0f + (fMenuPosY), 0.0f);
		g_aModeMenu[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aModeMenu[nCnt].selectgame = SELECTGAMETYPE_NONE;
		fMenuPosY += 80;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
		TEXTURE_CONTINUE,						// �t�@�C���̖��O
		&g_pTextureModeSelect[0]);				// �e�N�X�`���ւ̃|�C���^


	D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
		TEXTURE_RETRY,							// �t�@�C���̖��O
		&g_pTextureModeSelect[1]);				// �e�N�X�`���ւ̃|�C���^

	D3DXCreateTextureFromFile(pDevice,			// �f�o�C�X�ւ̃|�C���^
		TEXTURE_QUIT,							// �t�@�C���̖��O
		&g_pTextureModeSelect[2]);				// �e�N�X�`���ւ̃|�C���^

												// ���_���̍쐬
	MakeVertexModeSelect(pDevice);
}

//=============================================================================
// �I������
//=============================================================================
void UninitModeSelect(void)
{
	for (int nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{// �e�N�X�`���̊J��
		if (g_pTextureModeSelect[nCnt] != NULL)
		{
			g_pTextureModeSelect[nCnt]->Release();
			g_pTextureModeSelect[nCnt] = NULL;
		}
	}
	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffModeSelect != NULL)
	{
		g_pVtxBuffModeSelect->Release();
		g_pVtxBuffModeSelect = NULL;
	}

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateModeSelect(void)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;
	FADE fade;
	fade = GetFade();
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffModeSelect->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_DOWN) == true && fade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_SELECT);
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_NONE;
		g_nGameSelect = (g_nGameSelect + 1) % 3;
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_SELECT;
	}
	if (GetKeyboardTrigger(DIK_UP) == true && fade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_SELECT);
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_NONE;
		g_nGameSelect = (g_nGameSelect + 2) % 3;
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_SELECT;
	}
	if (GetKeyboardTrigger(DIK_S) == true && fade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_SELECT);
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_NONE;
		g_nGameSelect = (g_nGameSelect + 1) % 3;
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_SELECT;
	}
	if (GetKeyboardTrigger(DIK_W) == true && fade == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_SELECT);
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_NONE;
		g_nGameSelect = (g_nGameSelect + 2) % 3;
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_SELECT;
	}


	//�G���^�[�L�[
	if (GetKeyboardTrigger(DIK_SPACE) == true && fade == FADE_NONE )
	{
		if (g_nGameSelect == 0)
		{
			PlaySound(SOUND_LABEL_SE_CANCEL);
			StopSound(SOUND_LABEL_BGM000);
			g_SelectModeT = GAMEMODESELECT_SINGLEMODE;
		}
		else if (g_nGameSelect == 1)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			StopSound(SOUND_LABEL_BGM000);
			g_SelectModeT = GAMEMODESELECT_2PMODE;
		}
		else if (g_nGameSelect == 2)
		{
			PlaySound(SOUND_LABEL_SE_DECIDE);
			StopSound(SOUND_LABEL_BGM000);
			g_SelectModeT = GAMEMODESELECT_TUTORIAL;
		}

		switch (g_SelectModeT)
		{
		case GAMEMODESELECT_SINGLEMODE:
		{
			SetFade(MODE_SINGLEGAME);
			break;
		}
		case GAMEMODESELECT_2PMODE:
		{
			SetFade(MODE_GAME);

			break;
		}
		case GAMEMODESELECT_TUTORIAL:
		{
			SetFade(MODE_T);
			StopSound(SOUND_LABEL_BGM001);
			break;
		}
		}
	}

	if (GetKeyboardTrigger(DIK_SPACE) == true)
	{
		g_nGameSelect = 0;
		g_aModeMenu[g_nGameSelect].selectgame = SELECTGAMETYPE_SELECT;
		g_aModeMenu[1].selectgame = SELECTGAMETYPE_NONE;
		g_aModeMenu[2].selectgame = SELECTGAMETYPE_NONE;
	}

	for (int nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		if (g_aModeMenu[nCnt].selectgame == SELECTGAMETYPE_SELECT)
		{//�I�𒆂̐F	
			g_aModeMenu[nCnt].col = D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f);
		}
		else
		{//���I���̐F
			g_aModeMenu[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		//���_�J���[
		pVtx[0].col = g_aModeMenu[nCnt].col;
		pVtx[1].col = g_aModeMenu[nCnt].col;
		pVtx[2].col = g_aModeMenu[nCnt].col;
		pVtx[3].col = g_aModeMenu[nCnt].col;
		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffModeSelect->Unlock();
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawModeSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCnt;

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffModeSelect, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureModeSelect[nCnt]);

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			0 + (4 * nCnt),	//�J�n���钸�_�̃C���f�b�N�X
			2); //�`�悷��v���~�e�B�u��
	}

}

//=============================================================================
// ���_���̍쐬
//=============================================================================
void MakeVertexModeSelect(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_���̍쐬
	VERTEX_2D *pVtx;
	int nCnt;

	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PAUSEMENU,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffModeSelect,
		NULL);

	//���_����ݒ�
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffModeSelect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x - MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y - MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x + MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y - MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x - MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y + MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x + MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y + MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(TEX_LEFT, TEX_TOP);
		pVtx[1].tex = D3DXVECTOR2(TEX_RIGHT, TEX_TOP);
		pVtx[2].tex = D3DXVECTOR2(TEX_LEFT, TEX_BOT);
		pVtx[3].tex = D3DXVECTOR2(TEX_RIGHT, TEX_BOT);
		//���_
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//���_�J���[
		pVtx[0].col = g_aModeMenu[g_nGameSelect].col;
		pVtx[1].col = g_aModeMenu[g_nGameSelect].col;
		pVtx[2].col = g_aModeMenu[g_nGameSelect].col;
		pVtx[3].col = g_aModeMenu[g_nGameSelect].col;

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffModeSelect->Unlock();
}

//=============================================================================
// �|�[�Y�̐ݒ�
//=============================================================================
void SetModeSelect(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCnt;
	float fMenuPosY = 0;
	VERTEX_2D *pVtx;//���_���ւ̃|�C���^
					//���_����ݒ�
					//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffModeSelect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < MAX_PAUSEMENU; nCnt++)
	{
		//���_���W
		pVtx[0].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x - MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y - MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x + MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y - MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x - MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y + MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aModeMenu[nCnt].pos.x + MODESELECT_POS_X, g_aModeMenu[nCnt].pos.y + MODESELECT_POS_Y, g_aModeMenu[nCnt].pos.z);
		g_aModeMenu[nCnt].pos = pos;
		//�e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(TEX_LEFT, TEX_TOP);
		pVtx[1].tex = D3DXVECTOR2(TEX_RIGHT, TEX_TOP);
		pVtx[2].tex = D3DXVECTOR2(TEX_LEFT, TEX_BOT);
		pVtx[3].tex = D3DXVECTOR2(TEX_RIGHT, TEX_BOT);
		//���_
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		g_aModeMenu[nCnt].col = col;

		fMenuPosY += 100;

		pVtx += 4;	//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffModeSelect->Unlock();
}

//=============================================================================
// �Z���N�g���[�h�̎擾
//=============================================================================
GAMEMODESELECT *GetModeMode(void)
{
	return &g_SelectModeT;
}