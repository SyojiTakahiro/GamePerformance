//=============================================================================
//
// �������� [explosion.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "explosion.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************

//�ÓI�����o�ϐ�
CRenderer *m_pExplosionRenderer = NULL;
LPDIRECT3DTEXTURE9 CExplosion::m_pTexture = NULL;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CExplosion::CExplosion() : CScene2D(3)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CExplosion::~CExplosion()
{

}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CExplosion::Load(void)
{
	CManager manager;

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, EXPLOSION_TEXTURE, &m_pTexture);

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CExplosion::Unload(void)
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
HRESULT CExplosion::Init(void)
{
	CScene2D::BindTexture(m_pTexture);

	//�e�̏����ݒ�
	CScene2D::Init();

	SetExplosion(0);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CExplosion::Uninit(void)
{
	CScene2D::Uninit();
}
//=============================================================================
// �X�V����
//=============================================================================
void CExplosion::Update(void)
{
	m_nCounterAnim++;
	//�J�E���^�[�̍X�V
	if ((m_nCounterAnim % 2) == 0)
	{
		m_nPatternAnim = (m_nPatternAnim + 1) % 8;
		SetExplosion(m_nPatternAnim);

		//�p�^�[���I�����ɏI��
		if (m_nPatternAnim >= 8 - 1 )
		{
			Uninit();
		}
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CExplosion::Draw(void)
{
	CScene2D::Draw();
}

//=============================================================================
// �N���G�C�g
//=============================================================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos,float size)
{
	CExplosion *pExplosion = NULL;
	if (pExplosion == NULL)
	{
		// ���I�m��
		pExplosion = new CExplosion;
		if (pExplosion != NULL)
		{
			pExplosion->m_posPolygon = pos;
			pExplosion->SetSize(size);

			// ����������
			pExplosion->Init();
		}
	}
	return pExplosion;
}