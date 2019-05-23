//=============================================================================
//
// �G���� [enemy.cpp]
// Author :
//
//=============================================================================
#include "enemy.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "input.h"
#include "game.h"
#include "sound.h"
#include "effect.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MAX_TYPE_ENEMY		(5)						// �G�̎��
#define POLYGON_TEXTURENAME0		"data/TEXTURE/enemy0000.png"
#define POLYGON_TEXTURENAME1		"data/TEXTURE/enemy000.png"
#define POLYGON_TEXTURENAME2		"data/TEXTURE/enemy001.png"
#define POLYGON_TEXTURENAME3		"data/TEXTURE/enemy002.png"
#define POLYGON_TEXTURENAME4		"data/TEXTURE/enemy003.png"

//*****************************************************************************
// �O���[�o���ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apTextureEnemy[MAX_TYPE_ENEMY] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
ENEMY g_aEnemy[MAX_ENEMY];
int g_nNumEnemy;
float g_eSpin = 5.0;
float g_eSpin2 = 6.0;
int g_eBulletRecast = 30;

D3DXCOLOR g_eColor[2];
//=============================================================================
// ����������
//=============================================================================
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntEnemy;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �G�̏��̏�����
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].nType = 0;
		g_aEnemy[nCntEnemy].nCounterState=0;
		g_aEnemy[nCntEnemy].state;
		g_aEnemy[nCntEnemy].nLife = 5;
		g_aEnemy[nCntEnemy].bUse = false;
	}

	g_eColor[0] = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);
	g_eColor[1] = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

	// �e�N�X�`���̓ǂݍ���(�S�̕�)
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME1, &g_apTextureEnemy[0]);
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME2, &g_apTextureEnemy[1]);
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME3, &g_apTextureEnemy[2]);
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME4, &g_apTextureEnemy[3]);
	D3DXCreateTextureFromFile(pDevice, POLYGON_TEXTURENAME0, &g_apTextureEnemy[4]);

	// ���_���̍쐬
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�𐶐�
    pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,
									D3DUSAGE_WRITEONLY,
									FVF_VERTEX_2D,
									D3DPOOL_MANAGED,
									&g_pVtxBuffEnemy,
									NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for(nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 50, g_aEnemy[nCntEnemy].pos.y - 50, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 50, g_aEnemy[nCntEnemy].pos.y - 50, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 50, g_aEnemy[nCntEnemy].pos.y + 50, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 50, g_aEnemy[nCntEnemy].pos.y + 50, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		pVtx[0].tex = D3DXVECTOR2(0, 0);
		pVtx[1].tex = D3DXVECTOR2(1, 0);
		pVtx[2].tex = D3DXVECTOR2(0, 1);
		pVtx[3].tex = D3DXVECTOR2(1, 1);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();

	g_nNumEnemy = 0;
}

//=============================================================================
// �I������
//=============================================================================
void UninitEnemy(void)
{
	int nCntTex;

	// �e�N�X�`���̊J��
	for(nCntTex = 0; nCntTex < MAX_TYPE_ENEMY; nCntTex++)
	{
		if (g_apTextureEnemy[nCntTex] != NULL)
		{
			g_apTextureEnemy[nCntTex]->Release();
			g_apTextureEnemy[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̊J��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateEnemy(void)
{
	int nCntEnemy;
	float fData;
	Player *pPlayer;
	pPlayer = GetPlayer();

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)		//�G���g�p����Ă���
		{
			switch (g_aEnemy[nCntEnemy].state)
			{
			case ENEMYSTATE_NORMAL:
			g_eBulletRecast++;
				if (g_eBulletRecast >= 15)
					{
						SetEffect(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.8f, 0.0f, 0.5f), 300.0f, 1000);
						g_eBulletRecast = 0;
					}	
				if (rand() % 10 == 0)
				{
					g_eBulletRecast++;
					if (g_eBulletRecast >= 2)
					{

						fData = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);
						SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(fData) * 5.0f, cosf(fData) * 5.0f, 0.0f), BULLETTYPE_ENEMY);
						g_eBulletRecast = 0;
					}
					
				}
				
				if (g_aEnemy[nCntEnemy].nLife <= 250 && g_aEnemy[nCntEnemy].nLife >= 200)
				{
					g_eBulletRecast++;
					if (g_eBulletRecast >= 1)
					{
						g_eSpin += 0.1f;
						D3DXVECTOR3 SetSpinPos(g_aEnemy[nCntEnemy].pos);
						D3DXVECTOR3 MoveSpin(0.0f,0.0f, 0.0f);
						
						//�X�s��
						MoveSpin.x -= sinf(D3DX_PI*g_eSpin) * 3;
						MoveSpin.y -= cosf(D3DX_PI*g_eSpin) * 3;
						SetBullet(SetSpinPos, MoveSpin, BULLETTYPE_ENEMY);
						g_eBulletRecast = 0;

						if (rand() % 10 == 0)
						{
							g_eBulletRecast++;
							if (g_eBulletRecast >= 1)
							{
								fData = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);
								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(fData) * 5.0f, cosf(fData) * 5.0f, 0.0f), BULLETTYPE_ENEMY);
								g_eBulletRecast = 0;
							}
						}
					}
				}
				if (g_aEnemy[nCntEnemy].nLife <= 200 && g_aEnemy[nCntEnemy].nLife >= 100)
				{
					g_eBulletRecast++;
					if (g_eBulletRecast >= 1)
					{
						g_eSpin += 0.1f;
						g_eSpin2 += 0.11f;
						D3DXVECTOR3 SetSpinPos(g_aEnemy[nCntEnemy].pos);
						D3DXVECTOR3 MoveSpin(0.0f, 0.0f, 0.0f);

						//����
						MoveSpin.x -= sinf(D3DX_PI*g_eSpin2) * 3;
						MoveSpin.y -= cosf(D3DX_PI*g_eSpin2) * 3;
						SetBullet(SetSpinPos, MoveSpin, BULLETTYPE_ENEMY);


						g_eBulletRecast = 0;
					}
						if (rand() % 10 == 0)
						{
							g_eBulletRecast++;
							if (g_eBulletRecast >= 1.5f)
							{
								fData = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);
								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(fData) * 7.0f, cosf(fData) * 7.0f, 0.0f), BULLETTYPE_ENEMY);
								g_eBulletRecast = 0;
							}
						}
				}

				if (g_aEnemy[nCntEnemy].nLife <= 100)
				{
					g_eBulletRecast++;
					if (g_eBulletRecast >= 1)
					{
						g_eSpin += 0.1f;
						g_eSpin2 += 0.11f;
						D3DXVECTOR3 SetSpinPos(g_aEnemy[nCntEnemy].pos);
						D3DXVECTOR3 MoveSpin(0.0f, 0.0f, 0.0f);

						//����
						MoveSpin.x -= sinf(D3DX_PI*g_eSpin2) * 3;
						MoveSpin.y -= cosf(D3DX_PI*g_eSpin2) * 3;
						SetBullet(SetSpinPos, MoveSpin, BULLETTYPE_ENEMY);

						//�X�s��
						MoveSpin.x -= sinf(D3DX_PI*g_eSpin) * 2;
						MoveSpin.y -= cosf(D3DX_PI*g_eSpin) * 2;
						SetBullet(SetSpinPos, MoveSpin, BULLETTYPE_ENEMY);

						g_eBulletRecast = 0;
					}
						if (rand() % 10 == 0)
						{
							g_eBulletRecast++;
							if (g_eBulletRecast >= 1.5f)
							{
								fData = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x, pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);
								SetBullet(g_aEnemy[nCntEnemy].pos, D3DXVECTOR3(sinf(fData) * 10.0f, cosf(fData) * 10.0f, 0.0f), BULLETTYPE_ENEMY);
								g_eBulletRecast = 0;
							}
						}
				}
			
			break;

			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;
				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{//�F��߂�
					g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
					//g_aEnemy[nCntEnemy].nCounterState = 0;
					
					VERTEX_2D *pVtx;
					g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
					pVtx += (nCntEnemy * 4);
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					g_pVtxBuffEnemy->Unlock();
				}
				break;
			}
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
    pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �|���S���̕`��
	for(int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].nType]);
			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=============================================================================
// �G�̐ݒ�
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCntEnemy;
	VERTEX_2D*pVtx;

	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{
			g_aEnemy[nCntEnemy].pos = pos;

			pVtx[nCntEnemy * 4].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 150, g_aEnemy[nCntEnemy].pos.y - 150, 0.0f);
			pVtx[nCntEnemy * 4 + 1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 150, g_aEnemy[nCntEnemy].pos.y - 150, 0.0f);
			pVtx[nCntEnemy * 4 + 2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - 150, g_aEnemy[nCntEnemy].pos.y + 150, 0.0f);
			pVtx[nCntEnemy * 4 + 3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + 150, g_aEnemy[nCntEnemy].pos.y + 150, 0.0f);

			g_aEnemy[nCntEnemy].nType = nType;
			g_aEnemy[nCntEnemy].nCounterState = 0;
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			g_aEnemy[nCntEnemy].nLife = 300;
			g_aEnemy[nCntEnemy].bUse = true;
			g_nNumEnemy++;
			break;
		}
	}
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
// Hit���̏���
//=============================================================================
void HitEnemy(int nCntEnemy, int nDmage)
{
	ENEMY *pEnemy;
	pEnemy = GetEnemy();

	g_aEnemy[nCntEnemy].nLife -= nDmage;
	if (g_aEnemy[nCntEnemy].nLife<=0)
	{//���C�t���O�ȉ��ɂȂ����Ƃ�����
		SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		PlaySound(SOUND_LABEL_SE_EXPLOSION);
		g_aEnemy[nCntEnemy].bUse = false;
		g_nNumEnemy--;
		if (g_nNumEnemy <= 0)
		{
			SetGameState(GAMESTATE_CLEAR);
		}
	}
	else
	{//�q�b�g���G�̐F��ύX
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;
		g_aEnemy[nCntEnemy].nCounterState = 5;
		PlaySound(SOUND_LABEL_SE_HIT000);
		VERTEX_2D *pVtx;
		g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (nCntEnemy * 4);
		pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		g_pVtxBuffEnemy->Unlock();
	}
}

//=============================================================================
// �G�̎擾
//=============================================================================
ENEMY *GetEnemy(void)
{
	return &g_aEnemy[0];
}