//=============================================================================
//
// �e���� [bullet.cpp]
// Author : ���i�@�Y��
//
//=============================================================================
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "player.h"
#include "effect.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_BULLET			(25600)					//�e�̍ő吔
#define BULLET_WIDTH		(10)					//�e�̕�
#define BULLET_HEIGHT		(10)					//�e�̍���
#define BULLET_TEXTURENAME	"data/TEXTURE/bullet000.png"	//�ǂݍ��ޔw�i�e�N�X�`��

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXCOLOR color;
	BULLETTYPE type;
	int nLife;			//����
	bool bUse;			//�g�p���Ă��邩�ǂ���

}Bullet;

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posBullet;							//�|���S���̈ʒu
D3DXVECTOR3 g_moveBullet;							//�|���S���̈ړ���
D3DXCOLOR g_color[5];

Bullet g_aBullet[MAX_BULLET];						//�e�̏��


//=============================================================================
// ����������
//=============================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;//���_���ւ̃|�C���^
	int nCntBullet;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�̏��̏�����
	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_posBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_moveBullet = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].nLife = 30;
		g_aBullet[nCntBullet].bUse = false;
	}

	g_color[0] = D3DXCOLOR(1.0f, 0.0f, 0.0f,1.0f);
	g_color[1] = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	g_color[2] = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	g_color[3] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	g_color[4] = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
	//g_color[5] = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, BULLET_TEXTURENAME,&g_pTextureBullet);

	// ���_���̍쐬
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,	//�m�ۂ���o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2), -(BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
		pVtx[1].pos = D3DXVECTOR3((BULLET_WIDTH / 2), -(BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
		pVtx[2].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2), (BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
		pVtx[3].pos = D3DXVECTOR3((BULLET_WIDTH / 2), (BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			pVtx += 4;//���_�f�[�^�̃|�C���^��4�i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
// �I������
//=============================================================================
void UninitBullet(void)
{
	// �e�N�X�`���̊J��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateBullet(void)
{
	int nCntBullet;
	int nCntEnemy;
	ENEMY *pEnemy;

	VERTEX_2D *pVtx;//���_���ւ̃|�C���^

	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)//�e���g�p�����
		{
			//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;//�e�̈ʒu���ړ�������
			g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;//�e�̈ʒu���ړ�������


			//���_���W�̐ݒ�
			pVtx[nCntBullet * 4].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2), -(BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
			pVtx[nCntBullet * 4 + 1].pos = D3DXVECTOR3((BULLET_WIDTH / 2), -(BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
			pVtx[nCntBullet * 4 + 2].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2), (BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
			pVtx[nCntBullet * 4 + 3].pos = D3DXVECTOR3((BULLET_WIDTH / 2), (BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;

			g_aBullet[nCntBullet].nLife--;

			if (g_aBullet[nCntBullet].nLife <= 0)
			{
				//SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				g_aBullet[nCntBullet].bUse = false;
			}


			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffBullet->Unlock();

			pEnemy = GetEnemy();

			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{
				for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == true)
					{
						if (g_aBullet[nCntBullet].pos.x > pEnemy->pos.x - 80
							&& g_aBullet[nCntBullet].pos.x < pEnemy->pos.x + 80
							&& g_aBullet[nCntBullet].pos.y > pEnemy->pos.y - 80
							&& g_aBullet[nCntBullet].pos.y < pEnemy->pos.y + 80)
						{
							HitEnemy(nCntEnemy, 1);
							g_aBullet[nCntBullet].bUse = false;
						}
					}
				}
				SetEffect(g_aBullet[nCntBullet].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 20.0f, 30);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{
				Player *pPlayer;
				//�v���C���[�̎擾
				pPlayer = GetPlayer();
				if (pPlayer->bUse == true)
				{
					if (g_aBullet[nCntBullet].pos.x > pPlayer->pos.x - 10
						&& g_aBullet[nCntBullet].pos.x < pPlayer->pos.x + 10
						&& g_aBullet[nCntBullet].pos.y > pPlayer->pos.y - 10
						&& g_aBullet[nCntBullet].pos.y < pPlayer->pos.y + 5)
					{
						HitPlayer(1);

						//SetExplosion(pPlayer->pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
						g_aBullet[nCntBullet].bUse = false;
					}
				}
				SetEffect(g_aBullet[nCntBullet].pos, g_aBullet[nCntBullet].color, 15.0f, 10);
			}
			if (g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH|| g_aBullet[nCntBullet].pos.x < 0 || g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT || g_aBullet[nCntBullet].pos.y < 0)
			{
				g_aBullet[nCntBullet].bUse = false;
			}
		}
	}
}


//=============================================================================
// �`�揈��
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);

	// �|���S���̕`��
	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntBullet * 4,//�`����J�n���钸�_�C���f�b�N�X
				2);
		}
	}
}

//=============================================================================
// �e�̐ݒ菈��
//=============================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, BULLETTYPE type)
{
	int nCntBullet;
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	for(nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aBullet[nCntBullet].pos = pos;

			//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

			//pVtx += 4;

			//���_���W�̐ݒ�
			pVtx[nCntBullet*4].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2), -(BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
			pVtx[nCntBullet*4+1].pos = D3DXVECTOR3((BULLET_WIDTH / 2), -(BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
			pVtx[nCntBullet*4+2].pos = D3DXVECTOR3(-(BULLET_WIDTH / 2), (BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;
			pVtx[nCntBullet*4+3].pos = D3DXVECTOR3((BULLET_WIDTH / 2), (BULLET_HEIGHT / 2), 0.0f) + g_aBullet[nCntBullet].pos;

			//���_�o�b�t�@���A�����b�N����
			g_pVtxBuffBullet->Unlock();


			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].color = g_color[rand() % 5];
			g_aBullet[nCntBullet].nLife = 1000;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].bUse = true;//�g�p���Ă����Ԃɂ���

			break;
		}
	}
}

