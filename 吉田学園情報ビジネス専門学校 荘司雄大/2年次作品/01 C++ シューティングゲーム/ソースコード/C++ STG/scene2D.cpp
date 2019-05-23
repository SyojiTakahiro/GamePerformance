//=============================================================================
//
// �V�[���QD���� [scene2D.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"
//*****************************************************************************
//�ÓI�����o�ϐ�
//*****************************************************************************
CRenderer *m_pRenderer = NULL;

bool g_bDelete;
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPriority) : CScene(nPriority)
{
	m_pTexture = NULL;		// �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// ����������
//=============================================================================
HRESULT CScene2D::Init(void)
{
	CManager manager;

	LPDIRECT3DDEVICE9 pDevice = NULL;
	if (m_pRenderer == NULL)	//�G���[�N����������Ă�
	{
		pDevice = manager.GetRenderer()->GetDevice();
	}

	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//�g��
	m_fScale = 0.5f;

	// ���_����ݒ�
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_posPolygon.x + sinf(-D3DX_PI*0.75f) * 100;
	pVtx[1].pos.x = m_posPolygon.x + sinf(D3DX_PI*0.75f) * 100;
	pVtx[2].pos.x = m_posPolygon.x + sinf(-D3DX_PI*0.25f) * 100;
	pVtx[3].pos.x = m_posPolygon.x + sinf(D3DX_PI*0.25f) * 100;

	pVtx[0].pos.y = m_posPolygon.y + cosf(-D3DX_PI*0.75f) * 100;
	pVtx[1].pos.y = m_posPolygon.y + cosf(D3DX_PI*0.75f) * 100;
	pVtx[2].pos.y = m_posPolygon.y + cosf(-D3DX_PI*0.25f) * 100;
	pVtx[3].pos.y = m_posPolygon.y + cosf(D3DX_PI*0.25f) * 100;

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


	/*if (g_bDelete == false)
	{
		Uninit();
		g_bDelete = true;
	}*/

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void CScene2D::Update(void)
{
	//VERTEX_2D *pVtx;
	CManager manager;
	m_fLength += m_fScale;

	CInputKeyboard *pInput;
	pInput = manager.GetInputKeyboard();

	// ��]
	//m_fSpin += 0.1f;

	//if (m_fSpin > D3DX_PI)
	//{
	//	m_fSpin *= -1;
	//}
	//else if (m_fSpin < -D3DX_PI)
	//{
	//	m_fSpin *= -1;
	//}

	//if (m_fLength > 35)
	//{
	//	m_fScale *= -1;
	//}
	//else if (m_fLength < 25)
	//{
	//	m_fScale *= -1;
	//}
}

//=============================================================================
// �`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	CManager manager;

	LPDIRECT3DDEVICE9 pDevice = NULL;
	if (m_pRenderer == NULL)
	{
		CRenderer *pRenderer;
		pRenderer = manager.GetRenderer();
		pDevice = pRenderer->GetDevice();
					//or
		//pDevice = manager.GetRenderer()->GetDevice();
	}

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}


//=============================================================================
// �N���G�C�g
//=============================================================================
CScene2D *CScene2D::Create(D3DXVECTOR3 pos,float size)
{
	CScene2D *pScene2D = NULL;
	if (pScene2D == NULL)
	{
		// ���I�m��
		//pScene2D = new CScene2D;
		if (pScene2D != NULL)
		{
			pScene2D->m_posPolygon = pos;
			pScene2D->m_fSize = size;

			// ����������
			pScene2D->Init();
		}
	}
	return pScene2D;
}

void CScene2D::SetPosition(D3DXVECTOR3 pos)
{
	m_posPolygon = pos;

	VERTEX_2D *pVtx;

	// ���_����ݒ�
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].pos.x = m_posPolygon.x + sinf(-D3DX_PI*0.75f + m_fSpin) * m_fSize;
		pVtx[1].pos.x = m_posPolygon.x + sinf(D3DX_PI*0.75f + m_fSpin) * m_fSize;
		pVtx[2].pos.x = m_posPolygon.x + sinf(-D3DX_PI*0.25f + m_fSpin) * m_fSize;
		pVtx[3].pos.x = m_posPolygon.x + sinf(D3DX_PI*0.25f + m_fSpin) * m_fSize;

		pVtx[0].pos.y = m_posPolygon.y + cosf(-D3DX_PI*0.75f + m_fSpin) * m_fSize;
		pVtx[1].pos.y = m_posPolygon.y + cosf(D3DX_PI*0.75f + m_fSpin) * m_fSize;
		pVtx[2].pos.y = m_posPolygon.y + cosf(-D3DX_PI*0.25f + m_fSpin) * m_fSize;
		pVtx[3].pos.y = m_posPolygon.y + cosf(D3DX_PI*0.25f + m_fSpin) * m_fSize;

		//�A�����b�N
		m_pVtxBuff->Unlock();
	}
}

void CScene2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;

	// ���_����ݒ�
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		//�A�����b�N
		m_pVtxBuff->Unlock();
	}
}

void CScene2D::SetExplosion(int nPattenAnim)
{
	VERTEX_2D *pVtx;

	// ���_����ݒ�
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(nPattenAnim * 0.125f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nPattenAnim * 0.125f + 0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nPattenAnim * 0.125f, 0.0f + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nPattenAnim * 0.125f + 0.125f, 0.0f + 1.0f);
	//�A�����b�N
	m_pVtxBuff->Unlock();

	nPattenAnim = 0;
}

void CScene2D::SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius)
{
	VERTEX_2D *pVtx;

	m_posPolygon = pos;

	// ���_����ݒ�
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-fRadius, -fRadius, 0.0f) + m_posPolygon;
	pVtx[1].pos = D3DXVECTOR3(fRadius, -fRadius, 0.0f) + m_posPolygon;
	pVtx[2].pos = D3DXVECTOR3(-fRadius, fRadius, 0.0f) + m_posPolygon;
	pVtx[3].pos = D3DXVECTOR3(fRadius, fRadius, 0.0f) + m_posPolygon;

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//�A�����b�N
	m_pVtxBuff->Unlock();
}

void CScene2D::SetParticle(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius)
{
	VERTEX_2D *pVtx;

	m_posPolygon = pos;

	m_move = move;

	// ���_����ݒ�
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_posPolygon.x - fRadius, m_posPolygon.y - fRadius, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_posPolygon.x + fRadius, m_posPolygon.y - fRadius, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_posPolygon.x - fRadius, m_posPolygon.y + fRadius, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_posPolygon.x + fRadius, m_posPolygon.y + fRadius, 0.0f);

	pVtx[0].col = col;
	pVtx[1].col = col;
	pVtx[2].col = col;
	pVtx[3].col = col;

	//�A�����b�N
	m_pVtxBuff->Unlock();
}

void CScene2D::SetPlayer(int nPattenAnim)
{
	VERTEX_2D *pVtx;

	// ���_����ݒ�
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(nPattenAnim * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nPattenAnim * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nPattenAnim * 0.1f, 0.0f + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nPattenAnim * 0.1f + 0.1f, 0.0f + 1.0f);
	//�A�����b�N
	m_pVtxBuff->Unlock();

	nPattenAnim = 0;
}

void CScene2D::SetBulletEX(int nPattenAnim)
{
	VERTEX_2D *pVtx;

	// ���_����ݒ�
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(nPattenAnim * 0.2f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nPattenAnim * 0.2f + 0.2f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nPattenAnim * 0.2f, 0.0f + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nPattenAnim * 0.2f + 0.2f, 0.0f + 1.0f);
	//�A�����b�N
	m_pVtxBuff->Unlock();

	nPattenAnim = 0;
}

void CScene2D::SetBulletSP(int nPattenAnim)
{
	VERTEX_2D *pVtx;

	// ���_����ݒ�
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(nPattenAnim * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nPattenAnim * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nPattenAnim * 0.1f, 0.0f + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nPattenAnim * 0.1f + 0.1f, 0.0f + 1.0f);
	//�A�����b�N
	m_pVtxBuff->Unlock();

	nPattenAnim = 0;
}

D3DXVECTOR3 *CScene2D::GetPosition(void)
{
	return &m_posPolygon;
}

void CScene2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	//�e�N�X�`���̊��蓖��
	m_pTexture = pTexture;

}

LPDIRECT3DVERTEXBUFFER9 CScene2D::GetVtx(void)
{
	return m_pVtxBuff;
}

void CScene2D::SetSize(float size)
{
	m_fSize = size;

	if (m_pVtxBuff != NULL)
	{
		VERTEX_2D *pVtx;

		// ���_����ݒ�
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx[0].pos.x = m_posPolygon.x + sinf(-D3DX_PI*0.75f + m_fSpin) * m_fSize;
		pVtx[1].pos.x = m_posPolygon.x + sinf(D3DX_PI*0.75f + m_fSpin) * m_fSize;
		pVtx[2].pos.x = m_posPolygon.x + sinf(-D3DX_PI*0.25f + m_fSpin) * m_fSize;
		pVtx[3].pos.x = m_posPolygon.x + sinf(D3DX_PI*0.25f + m_fSpin) * m_fSize;

		pVtx[0].pos.y = m_posPolygon.y + cosf(-D3DX_PI*0.75f + m_fSpin) * m_fSize;
		pVtx[1].pos.y = m_posPolygon.y + cosf(D3DX_PI*0.75f + m_fSpin) * m_fSize;
		pVtx[2].pos.y = m_posPolygon.y + cosf(-D3DX_PI*0.25f + m_fSpin) * m_fSize;
		pVtx[3].pos.y = m_posPolygon.y + cosf(D3DX_PI*0.25f + m_fSpin) * m_fSize;

		//�A�����b�N
		m_pVtxBuff->Unlock();
	}
}

float CScene2D::GetSize(void)
{
	return m_fSize;
}