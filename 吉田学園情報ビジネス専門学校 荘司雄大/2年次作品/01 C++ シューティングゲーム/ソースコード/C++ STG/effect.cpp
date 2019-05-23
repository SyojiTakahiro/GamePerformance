//=============================================================================
//
// �G�t�F�N�g���� [effect.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "effect.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//�ÓI�����o�ϐ�
CRenderer *m_pEffectRenderer = NULL;
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CEffect::CEffect() : CScene2D(2)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CEffect::Load(void)
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
void CEffect::Unload(void)
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
HRESULT CEffect::Init(void)
{
	CScene2D::BindTexture(m_pTexture);

	//�e�̏����ݒ�
	CScene2D::Init();

	SetEffect(m_posEffect,m_col, m_fRadius);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CEffect::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CEffect::Update(void)
{
	m_nLife -= 3;
	m_fRadius -= 1.0f;

	if (m_fRadius <= 0)
	{
		m_fRadius = 0.0f;
	}

	SetEffect(m_posEffect, m_col, m_fRadius);

	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CEffect::Draw(void)
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
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius,int life)
{
	CEffect *pEffect = NULL;
	if (pEffect == NULL)
	{
		// ���I�m��
		pEffect = new CEffect;
		if (pEffect != NULL)
		{
			pEffect->m_posEffect = pos;
			pEffect->m_col = col;
			pEffect->m_fRadius = fRadius;
			pEffect->m_nLife = life;

			// ����������
			pEffect->Init();
		}
	}
	return pEffect;
}