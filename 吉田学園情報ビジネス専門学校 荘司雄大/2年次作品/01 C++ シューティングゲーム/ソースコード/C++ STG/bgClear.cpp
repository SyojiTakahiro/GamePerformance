//=============================================================================
//
// �N���A�w�i���� [bgClear.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "bgClear.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_SPEED0			(0.000f)
#define BG_SPEED1			(0.003f)
#define BG_SPEED2			(0.000f)

//�ÓI�����o�ϐ�
CRenderer *m_pBgCleaRRenderer = NULL;
LPDIRECT3DTEXTURE9 CBgClear::m_apTexture[MAX_BG] = {};

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBgClear::CBgClear() : CScene(0)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBgClear::~CBgClear()
{

}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CBgClear::Load(void)
{
	CManager manager;

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, CLEAR_TEXTURE0, &m_apTexture[0]);
	D3DXCreateTextureFromFile(pDevice, CLEAR_TEXTURE1, &m_apTexture[1]);
	D3DXCreateTextureFromFile(pDevice, CLEAR_TEXTURE2, &m_apTexture[2]);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CBgClear::Unload(void)
{
	for (int nCntScene = 0; nCntScene < MAX_BG; nCntScene++)
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
HRESULT CBgClear::Init(void)
{
	for (int nCntScene = 0; nCntScene < MAX_BG; nCntScene++)
	{
		m_apCScene2D[nCntScene] = new CScene2D(0);
		m_apCScene2D[nCntScene]->Init();
		m_apCScene2D[nCntScene]->BindTexture(m_apTexture[nCntScene]);

	}

	for (int nCntScene = 0; nCntScene < MAX_BG; nCntScene++)
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
void CBgClear::Uninit(void)
{
	for (int nCntScene = 0; nCntScene < MAX_BG; nCntScene++)
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
void CBgClear::Update(void)
{
	//���_���W�̐ݒ襍X�V
	m_nCntAnim++;

	for (int nCntScene = 0; nCntScene < MAX_BG; nCntScene++)
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

		//�A�����b�N
		Vtx->Unlock();
	}

}

//=============================================================================
// �`�揈��
//=============================================================================
void CBgClear::Draw(void)
{

}

//=============================================================================
// �N���G�C�g
//=============================================================================
CBgClear *CBgClear::Create(void)
{
	CBgClear *pBg = NULL;
	if (pBg == NULL)
	{
		// ���I�m��
		pBg = new CBgClear;

		if (pBg != NULL)
		{
			// ����������
			pBg->Init();
		}
	}
	return pBg;
}