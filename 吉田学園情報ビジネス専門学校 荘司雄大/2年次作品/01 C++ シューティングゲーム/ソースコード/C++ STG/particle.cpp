//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "particle.h"

//�ÓI�����o�ϐ�
CRenderer *m_pParticleRenderer = NULL;
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CParticle::CParticle() : CScene2D(5)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CParticle::Load(void)
{
	CManager manager;

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, EFFECT_TEXTURE, &m_pTexture);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CParticle::Unload(void)
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
HRESULT CParticle::Init(void)
{
	CScene2D::BindTexture(m_pTexture);

	//�p�[�e�B�N���̏����ݒ�
	CScene2D::Init();

	SetParticle(m_posParticle,m_move, m_col, m_fRadius);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CParticle::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CParticle::Update(void)
{
	//�|�W�V�����̎擾
	D3DXVECTOR3 pos;
	pos = *GetPosition();

	pos -= m_move;

	m_nLife--;
	m_fRadius = m_fRadius;

	m_col.a -= 0.03f;

	SetParticle(pos,m_move, m_col, m_fRadius);

	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CParticle::Draw(void)
{
	CManager manager;

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CScene2D::Draw();

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// �N���G�C�g
//=============================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int life)
{
	CParticle *pParticle = NULL;
	if (pParticle == NULL)
	{
		// ���I�m��
		pParticle = new CParticle;
		if (pParticle != NULL)
		{
			pParticle->m_posParticle = pos;
			pParticle->m_move = move;
			pParticle->m_col = col;
			pParticle->m_fRadius = fRadius;
			pParticle->m_nLife = life;

			// ����������
			pParticle->Init();
		}
	}
	return pParticle;
}