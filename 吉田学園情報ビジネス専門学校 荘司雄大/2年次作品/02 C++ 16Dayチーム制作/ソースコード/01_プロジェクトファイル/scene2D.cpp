//*****************************************************************************
//
//     �V�[��(2D)�̏���[scene2D.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define POLYGON2D_TEXTURE_NAME_0  "data/TEXTURE/cutin002.png"    // �e�N�X�`���̃t�@�C����(���)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CScene2D::m_apTexture[TEXTURE_TYPE_MAX] = {};  // 2D�|���S���̃e�N�X�`���摜

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CScene2D::CScene2D(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pTexture = NULL;                         // �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;                         // ���_�o�b�t�@�ւ̃|�C���^
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);     // ���W
	m_Col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f); // �F
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
//    �|���S���̐�������(�e�N�X�`���̎�ނ���)
//=============================================================================
CScene2D *CScene2D::Create(TEXTURE_TYPE type, int nPriority)
{
	CScene2D *pScene2D = NULL;             // �V�[��2D�N���X�^�̃|�C���^
	int nIdxScene = CScene::GetNumAll();   // �V�[���N���X�̐������������擾
	if (nIdxScene < MAX_SCENE)
	{// �܂������ł���
		if (pScene2D == NULL)
		{// ����������ɂȂ��Ă���
			pScene2D = new CScene2D(nPriority);
			if (pScene2D != NULL)
			{// �C���X�^���X�𐶐��ł���
				pScene2D->BindTexture(m_apTexture[type]);
				pScene2D->SetPos(D3DXVECTOR3(200.0f + (200.0f * type), SCREEN_HEIGHT / 3, 0.0f));
				pScene2D->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				if (FAILED(pScene2D->Init()))
				{// �������Ɏ��s����
					return NULL;
				}
				else
				{// �������ɐ�������
				 // �C���X�^���X�̃A�h���X��Ԃ�
					return pScene2D;
				}
			}
			else
			{// �C���X�^���X�𐶐��ł��Ȃ�����
				return NULL;
			}
		}
		else
		{// �C���X�^���X�𐶐��ł��Ȃ�����
			return NULL;
		}
	}
	else
	{// �C���X�^���X�𐶐��ł��Ȃ�����
		return NULL;
	}
}

//=============================================================================
//    �|���S���̃e�N�X�`���ǂݍ��ݏ���
//=============================================================================
HRESULT CScene2D::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			char aTextureName[TEXTURE_TYPE_MAX][256] =
			{// �e�N�X�`���̃t�@�C���p�X��
				POLYGON2D_TEXTURE_NAME_0,
			};
			for (int nCntTex = 0; nCntTex < TEXTURE_TYPE_MAX; nCntTex++)
			{// �ǂݍ��ރe�N�X�`���̐������J��Ԃ�
				D3DXCreateTextureFromFile(pDevice, aTextureName[nCntTex], &m_apTexture[nCntTex]);
			}
		}
	}

	return S_OK;
}

//=============================================================================
//    �|���S���̃e�N�X�`���������
//=============================================================================
void CScene2D::UnLoad(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < TEXTURE_TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//    �|���S���̏���������(�A�j���[�V�����Ȃ�)
//=============================================================================
HRESULT CScene2D::Init(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			// ���_�o�b�t�@�̐���
			MakeVertex(pDevice);
		}
		else
		{// �f�o�C�X���擾�ł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// �����_�����O�N���X����������Ă��Ȃ�
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �|���S���̏I������
//=============================================================================
void CScene2D::Uninit(void)
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	// �����[�X����
	CScene::Release();
}

//=============================================================================
//    �|���S���̍X�V����
//=============================================================================
void CScene2D::Update(void)
{

}

//=============================================================================
//    �|���S���̕`�揈��
//=============================================================================
void CScene2D::Draw(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
		    // ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, m_pTexture);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
		}
	}
}

//=============================================================================
//    �|���S���̒��_�o�b�t�@��������
//=============================================================================
void CScene2D::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// ���_�o�b�t�@�𐶐�
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// ���_���
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W
	pVtx[0].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 150.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 150.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 - 150.0f, 100.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH / 2 + 150.0f, 100.0f, 0.0f);

	// ���_�e�N�X�`��
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================================
//    �|���S���̃e�N�X�`���ݒ菈��
//=============================================================================
void CScene2D::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//=============================================================================
//    �|���S���̈ʒu�ݒ菈��
//=============================================================================
void CScene2D::SetPos(const D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=============================================================================
//    �|���S���̐F�ݒ菈��
//=============================================================================
void CScene2D::SetCol(const D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
//    �|���S���̒��_���ݒ菈��
//=============================================================================
void CScene2D::SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff)
{
	m_pVtxBuff = pVtxBuff;
}

//=============================================================================
//    �|���S���̒��_���擾����
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CScene2D::GetVtxBuff(void)
{
	return m_pVtxBuff;
}

//=============================================================================
//    �|���S���̈ʒu�擾����
//=============================================================================
D3DXVECTOR3 CScene2D::GetPos(void)
{
	return m_pos;
}

//=============================================================================
//    �|���S���̐F�擾����
//=============================================================================
D3DXCOLOR CScene2D::GetCol(void)
{
	return m_Col;
}