//=============================================================================
//
// �������� [number.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
#include "number.h"
#include "score.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//�ÓI�����o�ϐ�
//CRenderer *m_pRenderer = NULL;
LPDIRECT3DTEXTURE9 CNumber::m_pTexture = NULL;

bool g_bNumberDelete;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CNumber::CNumber()
{
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CNumber::~CNumber() 
{

}

//=============================================================================
// ���[�h
//=============================================================================
HRESULT CNumber::Load(void)
{
	CManager manager;

	//�e�N�X�`���̓ǂݍ���
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	if (m_pTexture == NULL)
	{
		D3DXCreateTextureFromFile(pDevice, NUMBER_TEXTURE, &m_pTexture);
	}

	return S_OK;
}

//=============================================================================
// �A�����[�h
//=============================================================================
void CNumber::Unload(void)
{
	//�����[�X
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// ����������
//=============================================================================
HRESULT CNumber::Init(void)
{
	CManager manager;

	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_2D *pVtx;

	// ���_����ݒ�
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_posNumber.x + sinf(-D3DX_PI*0.75f) * 30;
	pVtx[1].pos.x = m_posNumber.x + sinf(D3DX_PI*0.75f) * 30;
	pVtx[2].pos.x = m_posNumber.x + sinf(-D3DX_PI*0.25f) * 30;
	pVtx[3].pos.x = m_posNumber.x + sinf(D3DX_PI*0.25f) * 30;

	pVtx[0].pos.y = m_posNumber.y + cosf(-D3DX_PI*0.75f) * 30;
	pVtx[1].pos.y = m_posNumber.y + cosf(D3DX_PI*0.75f) * 30;
	pVtx[2].pos.y = m_posNumber.y + cosf(-D3DX_PI*0.25f) * 30;
	pVtx[3].pos.y = m_posNumber.y + cosf(D3DX_PI*0.25f) * 30;

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

	//�A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CNumber::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}
//=============================================================================
// �X�V����
//=============================================================================
void CNumber::Update(void)
{
	
}

//=============================================================================
// �`�揈��
//=============================================================================
void CNumber::Draw(void)
{
	CManager manager;

	LPDIRECT3DDEVICE9 pDevice = NULL;

	CRenderer *pRenderer;
	pRenderer = manager.GetRenderer();
	pDevice = pRenderer->GetDevice();
	//or
	//pDevice = manager.GetRenderer()->GetDevice();


	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,  0, 2);
}

//=============================================================================
// �N���G�C�g
//=============================================================================
CNumber *CNumber::Create(D3DXVECTOR3 pos)
{
	CNumber *pNumber = NULL;
	if (pNumber == NULL)
	{
		// ���I�m��
		pNumber = new CNumber;

		if (pNumber != NULL)
		{
			pNumber->m_posNumber = pos;
			// ����������
			pNumber->Init();
		}
	}
	return pNumber;
}

//=============================================================================
// �Z�b�g�i���o�[
//=============================================================================
void CNumber::SetNumber(int nNumber)
{
	VERTEX_2D *pVtx;

	// ���_����ݒ�
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`���ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (nNumber * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (nNumber * 0.1f), 1.0f);

	//�A�����b�N
	m_pVtxBuff->Unlock();
}