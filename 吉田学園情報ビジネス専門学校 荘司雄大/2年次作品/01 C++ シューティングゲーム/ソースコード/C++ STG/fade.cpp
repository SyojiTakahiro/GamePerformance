//=============================================================================
//
// �t�F�[�h���� [fade.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "fade.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define FADE_POS_X			(0)					//X���W
#define FADE_POS_Y			(0)					//Y���W
#define FADE_WIDTH			(SCREEN_WIDTH)		//��ʂ̕�
#define FADE_HEIGHT			(SCREEN_HEIGHT)		//��ʂ̍���

//*****************************************************************************
// �ÓI�����o
//*****************************************************************************
CFade::FADE CFade::m_fade = CFade::FADE_IN;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CFade::CFade()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFade::~CFade()
{

}


//=============================================================================
// �N���G�C�g
//=============================================================================
CFade *CFade::Create()
{
	CFade *pFade = NULL;
	if (pFade == NULL)
	{
		// ���I�m��
		pFade = new CFade;
		if (pFade != NULL)
		{
			// ����������
			pFade->Init();
		}
	}
	return pFade;
}

//=============================================================================
// ����������
//=============================================================================
void CFade::Init(void)
{
	CManager manager;
	VERTEX_2D*pVtx;

	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_�o�b�t�@�̏���ݒ�
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(FADE_POS_X, FADE_POS_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(FADE_POS_X + FADE_WIDTH, FADE_POS_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(FADE_POS_X, FADE_POS_Y + FADE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(FADE_POS_X + FADE_WIDTH, FADE_POS_Y + FADE_HEIGHT, 0.0f);

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

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �l�̏�����
	m_fade = FADE_IN;
	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
}

//=============================================================================
// �I������
//=============================================================================
void CFade::Uninit(void)
{
	// �e�N�X�`���̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}

	// ���_�o�b�t�@�̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void CFade::Update(void)
{
	CManager manager;

	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			m_colorFade.a -= 1.0f / 30;
			if (m_colorFade.a <= 0.0f)
			{
				m_colorFade.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_colorFade.a += 1.0f /30;
			if (m_colorFade.a >= 1.0f)
			{
				m_colorFade.a = 1.0f;
				m_fade = FADE_IN;
				//
				manager.SetMode(m_modeNext);
			}
		}
		VERTEX_2D*pVtx;
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx[0].col = m_colorFade;
		pVtx[1].col = m_colorFade;
		pVtx[2].col = m_colorFade;
		pVtx[3].col = m_colorFade;
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// �^�C�g�����
//=============================================================================
void CFade::Draw(void)
{
	CManager manager;

	LPDIRECT3DDEVICE9 pDevice = NULL;
	pDevice = manager.GetRenderer()->GetDevice();

	// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���Ƀo�C���h
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//=============================================================================
// �t�F�[�h�̐ݒ�
//=============================================================================
void CFade::SetFade(CManager::MODE modeNext)
{
	if (m_fade != FADE_OUT)
	{
		m_fade = FADE_OUT;
		m_modeNext = modeNext;
		m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

//=============================================================================
// �t�F�[�h�̎擾
//=============================================================================
CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}

