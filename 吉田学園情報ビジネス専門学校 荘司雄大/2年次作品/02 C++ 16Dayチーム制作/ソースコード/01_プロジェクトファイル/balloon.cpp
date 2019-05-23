//=============================================================================
//
// �����o������ [balloon.cpp]
// Author : Atsushi Yamashita
//
//=============================================================================
#include "balloon.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sceneX.h"
#include "fade.h"
#include "debuglog.h"
//============================================================================
//	�ÓI�����o�ϐ��錾
//============================================================================
LPDIRECT3DTEXTURE9 CBalloon::m_pTexture[MAX_BALLOON_TEXTURE] = {};
//int CBalloon::m_nType = 0;
//============================================================================
//	�}�N����`
//============================================================================

#define BALLOON_TEXTURENAME_0 "data/TEXTURE/GAME/STAGE1.png"
#define BALLOON_TEXTURENAME_1 "data/TEXTURE/GAME/STAGE2.png"
#define BALLOON_TEXTURENAME_2 "data/TEXTURE/GAME/STAGE3.png"
#define BALLOON_TEXTURENAME_3 "data/TEXTURE/GAME/STAGE4.png"
#define BALLOON_TEXTURENAME_4 "data/TEXTURE/GAME/STAGE5.png"

//=============================================================================
//	�R���X�g���N�^
//=============================================================================
CBalloon::CBalloon(int nPriority, OBJTYPE objType) : CScene2DSquare(nPriority, objType)
{
	m_pVtxBuff = NULL;						// ���_�o�b�t�@�ւ̃|�C���^
	m_nCounter = 0;

	m_pos = D3DXVECTOR3(0, 0, 0);			// �����o���̈ʒu
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f,0.0f);			// �F

}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CBalloon::~CBalloon()
{

}

//=============================================================================
// �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CBalloon::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, BALLOON_TEXTURENAME_0, &m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice, BALLOON_TEXTURENAME_1, &m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice, BALLOON_TEXTURENAME_2, &m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice, BALLOON_TEXTURENAME_3, &m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice, BALLOON_TEXTURENAME_4, &m_pTexture[4]);

	return S_OK;
}

//=============================================================================
// �e�N�X�`���̊J������
//=============================================================================
void CBalloon::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_BALLOON_TEXTURE; nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (m_pTexture[nCntTex] != NULL)
		{
			m_pTexture[nCntTex]->Release();
			m_pTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
// �����o���̏���������
//=============================================================================
HRESULT CBalloon::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X���擾
	CManager Manager;
	pDevice = Manager.GetRenderer()->GetDevice();

	//�e�N�X�`���̓\��t��
	BindTexture(m_pTexture[m_nTexNumber]);

	//�����o���̏���������
	CScene2DSquare::Init(pos, col,fWidth,fHeight);

	return S_OK;
}

//=============================================================================
// �����o���̏I������
//=============================================================================
void CBalloon::Uninit(void)
{
	//�����o���̏I������
	CScene2DSquare::Uninit();
}

//=============================================================================
// �����o���̍X�V����
//=============================================================================
void CBalloon::Update(void)
{
	float fHeight = CScene2DSquare::GetHeight();
	float fWidth = CScene2DSquare::GetWidth();
	D3DXVECTOR3 BalloonPos = CScene2D::GetPos();

	//�����o���̐F���擾
	m_col = CScene2D::GetCol();

	if (m_nType == 0)
	{//�����o�����g��
		//fHeight += 1.0f;	//�����̉��Z
		//fWidth += 1.0f;		//���̉�
		m_col.a += 0.02f;	//�����x�̉��Z

		//if (fHeight >= 70.0f && fWidth >= 70.0f)
		//{//�傫����70�ɂȂ�����~�߂�
		//	fHeight = 70.0f;
		//	fWidth = 70.0f;
		//}
		if (m_col.a >= 1.0f)
		{//�����x��1.0f�ȏ�ɂȂ�����1.0f�Ŏ~�߂�
			m_col.a = 1.0f;
		}

	}
	else if (m_nType == 1)
	{//�����o���̏k��
		//fHeight -= 1.0f;	//�����̌��Z
		//fWidth -= 1.0f;		//���̌��Z
		m_col.a -= 0.02f;	//�����x�̌��Z

		//if (fHeight <= 0.0f && fWidth <= 0.0f)
		//{//�傫����0�ɂȂ�����~�߂�
		//	fHeight = 0.0f;
		//	fWidth = 0.0f;
		//}
		if (m_col.a <= 0.0f)
		{//�����x��0.0f�ȉ��ɂȂ�����0.0f�Ŏ~�߂�
			m_col.a = 0.0f;
		}
	}

	//�����̐ݒ�
	CScene2DSquare::SetHeight(fHeight);

	//���̐ݒ�
	CScene2DSquare::SetWidth(fWidth);

	//�ʒu�̐ݒ�
	CScene2DSquare::SetVtxBuffPos(BalloonPos);

	//�F�̐ݒ�
	CScene2DSquare::SetVtxBuffCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_col.a));
}

//=============================================================================
//�����o���̕`�揈��
//=============================================================================
void CBalloon::Draw(void)
{
	//�����o���̕`�揈��
	CScene2DSquare::Draw();
}

//=============================================================================
//�����o���̐�������
//=============================================================================
CBalloon *CBalloon::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight ,int nTexNumber)
{
	CBalloon *pBalloon = {};

	if (pBalloon == NULL)
	{
		//�������𓮓I�m��
		pBalloon = new CBalloon;

		if (pBalloon != NULL)
		{//NULL�`�F�b�N
			//�e�N�X�`���̔ԍ�����
			pBalloon->m_nTexNumber = nTexNumber;

			//����������
			pBalloon->Init(pos,col,fWidth,fHeight);
		}
		else
		{
			MessageBox(0, "Balloon��NULL�ł���", "�x��", MB_OK);
		}
	}
	else
	{
		MessageBox(0, "Balloon��NULL����Ȃ��ł�", "�x��", MB_OK);
	}
	return pBalloon;
}

//=============================================================================
//�����o���̎擾
//=============================================================================
void CBalloon::SetType(int nType)
{
	m_nType = nType;
}