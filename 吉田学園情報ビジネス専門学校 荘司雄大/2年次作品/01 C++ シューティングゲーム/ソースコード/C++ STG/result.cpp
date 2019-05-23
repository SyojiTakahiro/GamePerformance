////=============================================================================
////
//// ���C������ [main.cpp]
//// Author : 
////
////=============================================================================
//#include "main.h"
//#include "renderer.h"
//#include "input.h"
//#include "scene.h"
//#include "scene2D.h"
//#include "manager.h"
//#include "input.h"
//#include "result.h"
//#include "fade.h"
////*****************************************************************************
//// �}�N����`
////*****************************************************************************
//
////�ÓI�����o�ϐ�
//CRenderer *m_pResultRenderer = NULL;
//LPDIRECT3DTEXTURE9 CResult::m_pTexture = NULL;
//
////=============================================================================
//// �R���X�g���N�^
////=============================================================================
//CResult::CResult()
//{
//
//}
//
////=============================================================================
//// �f�X�g���N�^
////=============================================================================
//CResult::~CResult()
//{
//	m_pTexture = NULL;
//}
//
////=============================================================================
//// ���[�h
////=============================================================================
//HRESULT CResult::Load(void)
//{
//	CManager manager;
//
//	//�e�N�X�`���̓ǂݍ���
//	LPDIRECT3DDEVICE9 pDevice = NULL;
//	pDevice = manager.GetRenderer()->GetDevice();
//
//	// �e�N�X�`���̐���
//	D3DXCreateTextureFromFile(pDevice, BG_TEXTURE0, &m_pTexture);
//
//	return S_OK;
//}
//
////=============================================================================
//// �A�����[�h
////=============================================================================
//void CResult::Unload(void)
//{
//	// �e�N�X�`���̔j��
//	if (m_pTexture != NULL)
//	{
//		m_pTexture->Release();
//		m_pTexture = NULL;
//	}
//}
//
////=============================================================================
//// ����������
////=============================================================================
//HRESULT CResult::Init(void)
//{
//	//CManager manager;
//
//	//LPDIRECT3DDEVICE9 pDevice = NULL;
//	//if (m_pResultRenderer == NULL)	//�G���[�N����������Ă�
//	//{
//	//	pDevice = manager.GetRenderer()->GetDevice();
//	//}
//
//	//// ���_�o�b�t�@�̐���
//	//pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
//	//	D3DUSAGE_WRITEONLY,
//	//	FVF_VERTEX_2D,
//	//	D3DPOOL_MANAGED,
//	//	&m_pVtxBuff,
//	//	NULL);
//
//	//CManager::SetMode(CManager::MODE_TITLE);
//
//	return S_OK;
//}
//
////=============================================================================
//// �I������
////=============================================================================
//void CResult::Uninit(void)
//{
//	if (m_pTexture != NULL)
//	{
//		m_pTexture->Release();
//		m_pTexture = NULL;
//	}
//}
////=============================================================================
//// �X�V����
////=============================================================================
//void CResult::Update(void)
//{
//	CManager manager;
//	CFade *pFade = CManager::GetFade();
//
//	//�L�[�{�[�h�̎擾
//	CInputKeyboard *pInput;
//	pInput = manager.GetInputKeyboard();
//
//	if (pInput->GetKeyboardTrigger(DIK_RETURN) == true)
//	{
//		pFade->SetFade(CManager::MODE_TITLE);
//	}
//}
//
////=============================================================================
//// �`�揈��
////=============================================================================
//void CResult::Draw(void)
//{
//
//}
//
////=============================================================================
//// �N���G�C�g
////=============================================================================
//CResult *CResult::Create(void)
//{
//	CResult *pResult = NULL;
//	if (pResult == NULL)
//	{
//		// ���I�m��
//		pResult = new CResult;
//
//		if (pResult != NULL)
//		{
//			// ����������
//			pResult->Init();
//		}
//	}
//	return pResult;
//}