//=============================================================================
//
// bar���� [bar.cpp]
// Author : Takahiro Syoji
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "bar.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define BG_SPEED0			(0.000f)
#define BG_TEXTURE_BAR		"data/TEXTURE/bar.png"

//�ÓI�����o�ϐ�
CRenderer *m_pBarRenderer = NULL;
LPDIRECT3DTEXTURE9 CBar::m_pTexture = NULL;

bool g_bBarDelete;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CBar::CBar() : CScene(2)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CBar::~CBar()
{

}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CBar::Load(void)
{
	CManager manager;

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE_BAR, &m_pTexture);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CBar::Unload(void)
{
	// �e�N�X�`���̔j��
		if (m_pTexture != NULL)
		{
			m_pTexture->Release();
			m_pTexture = NULL;
		}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CBar::Init(void)
{

	m_pCScene2D = new CScene2D(4);
	m_pCScene2D->Init();
	m_pCScene2D->BindTexture(m_pTexture);



	VERTEX_2D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 Vtx;
	Vtx = m_pCScene2D->GetVtx();

	// ���_����ݒ�
	Vtx->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0 + SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, 0 + SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0 + SCREEN_WIDTH, 0 + SCREEN_HEIGHT, 0.0f);

	//�A�����b�N
	Vtx->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CBar::Uninit(void)
{
	if (m_pCScene2D != NULL)
	{
		m_pCScene2D->Uninit();
		m_pCScene2D = NULL;
	}
	//�����[�X
	CScene::Release();
}
//=============================================================================
// �X�V����
//=============================================================================
void CBar::Update(void)
{
	//���_���W�̐ݒ襍X�V
	m_nCntAnim++;

	VERTEX_2D *pVtx;
	LPDIRECT3DVERTEXBUFFER9 Vtx;
	Vtx = m_pCScene2D->GetVtx();

	Vtx->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED0), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED0), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nCntAnim * BG_SPEED0), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f + (m_nCntAnim * BG_SPEED0), 1.0f);


	//�A�����b�N
	Vtx->Unlock();

}

//=============================================================================
// �`�揈��
//=============================================================================
void CBar::Draw(void)
{

}

//=============================================================================
// �N���G�C�g
//=============================================================================
CBar *CBar::Create(void)
{
	CBar *pBar = NULL;
	if (pBar == NULL)
	{
		// ���I�m��
		pBar = new CBar;

		if (pBar != NULL)
		{
			// ����������
			pBar->Init();
		}
	}
	return pBar;
}