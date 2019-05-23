//=============================================================================
//
// �`���[�g���A���w�i���� [bgTutorial.cpp]
// Author : Takahiro Syoji
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "bgTutorial.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_SPEED0			(0.000f)
#define BG_SPEED1			(0.003f)
#define BG_SPEED2			(0.000f)
#define BG_SPEED3			(0.000f)

#define BGTUTORIAL_TEXTURE		"data/TEXTURE/tutorial.png"

//�ÓI�����o�ϐ�
CRenderer *m_pBgTutorialRenderer = NULL;
LPDIRECT3DTEXTURE9 CBgTutorial::m_apTexture[MAX_GAME_BG] = {};

bool g_bBgTutorialDelete;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBgTutorial::CBgTutorial() : CScene(0)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBgTutorial::~CBgTutorial()
{

}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CBgTutorial::Load(void)
{
	CManager manager;

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE0, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE1, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, BGTUTORIAL_TEXTURE, &m_apTexture[2]);
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE3, &m_apTexture[3]);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CBgTutorial::Unload(void)
{
	for (int nCntScene = 0; nCntScene < MAX_GAME_BG; nCntScene++)
	{
		// �e�N�X�`���̔j��
		if (m_apTexture[nCntScene] != NULL)
		{
			m_apTexture[nCntScene]->Release();
			m_apTexture[nCntScene] = NULL;
		}
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBgTutorial::Init(void)
{
	for (int nCntScene = 0; nCntScene < MAX_GAME_BG; nCntScene++)
	{
		m_apCScene2D[nCntScene] = new CScene2D(0);
		m_apCScene2D[nCntScene]->Init();
		m_apCScene2D[nCntScene]->BindTexture(m_apTexture[nCntScene]);

	}

	for (int nCntScene = 0; nCntScene < MAX_GAME_BG; nCntScene++)
	{
		VERTEX_2D *pVtx;
		LPDIRECT3DVERTEXBUFFER9 Vtx;
		Vtx = m_apCScene2D[nCntScene]->GetVtx();

		// ���_����ݒ�
		Vtx->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W
		pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0 + SCREEN_WIDTH, 0, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0, 0 + SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0 + SCREEN_WIDTH, 0 + SCREEN_HEIGHT, 0.0f);

		//�A�����b�N
		Vtx->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBgTutorial::Uninit(void)
{
	for (int nCntScene = 0; nCntScene < MAX_GAME_BG; nCntScene++)
	{
		m_apCScene2D[nCntScene]->Uninit();
		m_apCScene2D[nCntScene] = NULL;
	}
	//�����[�X
	CScene::Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CBgTutorial::Update(void)
{
	//���_���W�̐ݒ襍X�V
	m_nCntAnim++;

	for (int nCntScene = 0; nCntScene < MAX_GAME_BG; nCntScene++)
	{
		VERTEX_2D *pVtx;
		LPDIRECT3DVERTEXBUFFER9 Vtx;
		Vtx = m_apCScene2D[nCntScene]->GetVtx();

		Vtx->Lock(0, 0, (void**)&pVtx, 0);

		if (nCntScene == 0)
		{
			// ���_����ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED0), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED0), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED0), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED0), 1.0f);
		}
		else if (nCntScene == 1)
		{
			// ���_����ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED1), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED1), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED1), 1.0f);
		}
		else if (nCntScene == 2)
		{
			// ���_����ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED2), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED2), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED2), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED2), 1.0f);
		}
		else if (nCntScene == 3)
		{
			// ���_����ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED3), 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED3), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED3), 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED3), 1.0f);
		}

		//�A�����b�N
		Vtx->Unlock();
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void CBgTutorial::Draw(void)
{

}

//=============================================================================
// �N���G�C�g
//=============================================================================
CBgTutorial *CBgTutorial::Create(void)
{
	CBgTutorial *pBgTutorial = NULL;
	if (pBgTutorial == NULL)
	{
		// ���I�m��
		pBgTutorial = new CBgTutorial;

		if (pBgTutorial != NULL)
		{
			// ����������
			pBgTutorial->Init();
		}
	}
	return pBgTutorial;
}