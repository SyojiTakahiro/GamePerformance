//*****************************************************************************
//
//     ���S�̏���[logo.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "logo.h"
#include "scene2D.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define PAUSELOGO_TEXTURE_NAME_0  "data/TEXTURE/GAME/Continue.png"          // �e�N�X�`���̃t�@�C����(CONTINUE)
#define PAUSELOGO_TEXTURE_NAME_1  "data/TEXTURE/GAME/Retry.png"             // �e�N�X�`���̃t�@�C����(RETRY)
#define PAUSELOGO_TEXTURE_NAME_2  "data/TEXTURE/GAME/Quit.png"              // �e�N�X�`���̃t�@�C����(QUIT)

#define SELECTLOGO_TEXTURE_NAME_0  "data/TEXTURE/SELECT/select000.png"            // �e�N�X�`���̃t�@�C����(GAME)
#define SELECTLOGO_TEXTURE_NAME_1  "data/TEXTURE/SELECT/select001.png"            // �e�N�X�`���̃t�@�C����(RANKING)
#define SELECTLOGO_TEXTURE_NAME_2  "data/TEXTURE/SELECT/select002.png"            // �e�N�X�`���̃t�@�C����(TITLE)

#define SELECT_ANIMATION_PATTERN   (1)                                            // �e�N�X�`���A�j���[�V�����̃p�^�[����
#define SELECT_ANIMATION_TEX_U_INI (1.0f / SELECT_ANIMATION_PATTERN)              // �e�N�X�`���A�j���[�V����U���W�̏����l
#define SELECT_ANIMATION_CALC_U    (SELECT_ANIMATION_TEX_U_INI * m_nPatternAnimU) // �e�N�X�`���A�j���[�V����U���W�̌v�Z��
#define SELECT_ANIMATION_SWITCH    (6)                                            // �e�N�X�`�����W��؂�ւ���^�C�~���O

#define PRESSLOGO_TEXTURE_NAME     "data/TEXTURE/COMMON/press.png"                // �e�N�X�`���̃t�@�C����(�����{�^���������Ă�������)
#define PRESSLOGO_DRAW_TIMING      (80)                                           // PRESSLOGO��`�悷�鎞��
#define PRESSLOGO_RETURN_TIMING    (100)                                          // PRESSLOGO�̃J�E���^�[��߂�����
#define PRESSLOGO_PUSH_DRAW_TIMING (4)                                            // ����{�^���������ꂽ�Ƃ���PRESSLOGO��`�悷��^�C�~���O

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CLogoSelect::m_apTexture[TYPE_MAX] = {};  // �e�N�X�`���ւ̃|�C���^(�Z���N�g���S�p)
LPDIRECT3DTEXTURE9 CLogoPause::m_apTexture[TYPE_MAX] = {};   // �e�N�X�`���ւ̃|�C���^(�|�[�Y���S�p)
LPDIRECT3DTEXTURE9 CLogoPress::m_pTexture = NULL;            // �e�N�X�`���ւ̃|�C���^(PRESSENTER�p)

float              CLogoSelect::m_fColTrans = 0.01f;         // �F�̕ω��Ɏg�p����
float              CLogoPause::m_fColTrans = 0.01f;          // �F�̕ω��Ɏg�p����

//*****************************************************************************
//    CLogo�̏���
//*****************************************************************************
//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CLogo::CLogo(int nPriority, OBJTYPE objType) : CScene2DRot(nPriority, objType)
{
	// �e��l�̏�����
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CLogo::~CLogo()
{

}

//=============================================================================
//    ��������
//=============================================================================
CLogo *CLogo::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, int nPriority)
{
	CLogo *pLogo = NULL;   // ���S�N���X�^�̃|�C���^
	if (pLogo == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pLogo = new CLogo(nPriority); // ���S�N���X�̃C���X�^���X����
		if (pLogo != NULL)
		{// ���������m�ۂł���
		    // ����������
			if (FAILED(pLogo->Init(pos, col, fScale)))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return NULL;
	}

	return pLogo;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CLogo::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale)
{
	// ���ʂ̏���������
	if (FAILED(CScene2DRot::Init(pos, col, fScale, 0.0f)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CLogo::Uninit(void)
{
	// ���ʂ̕`�揈��
	CScene2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CLogo::Update(void)
{

}

//=============================================================================
//    �`�揈��
//=============================================================================
void CLogo::Draw(void)
{
	// ���ʂ̕`�揈��
	CScene2D::Draw();
}

//*****************************************************************************
//    CLogoSelect�̏���
//*****************************************************************************
//=============================================================================
//    ��������
//=============================================================================
CLogoSelect *CLogoSelect::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state, int nPriority)
{
	CLogoSelect *pLogoSelect = NULL;   // �Z���N�g���S�N���X�^�̃|�C���^
	if (pLogoSelect == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pLogoSelect = new CLogoSelect(nPriority); // �Z���N�g���S�N���X�̃C���X�^���X����
		if (pLogoSelect != NULL)
		{// ���������m�ۂł���
		    // ����������
			if (FAILED(pLogoSelect->Init(pos, col, fScale, type, state)))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return NULL;
	}

	return pLogoSelect;
}

//=============================================================================
//    �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CLogoSelect::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	 // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		char aTextureName[TYPE_MAX][256] =
		{// �e�N�X�`���̃t�@�C���p�X��
			SELECTLOGO_TEXTURE_NAME_0,
			SELECTLOGO_TEXTURE_NAME_1,
			SELECTLOGO_TEXTURE_NAME_2
		};
		for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
		{// �ǂݍ��ރe�N�X�`���̐������J��Ԃ�
			D3DXCreateTextureFromFile(pDevice, aTextureName[nCntTex], &m_apTexture[nCntTex]);
		}
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`���̉������
//=============================================================================
void CLogoSelect::UnLoad(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CLogoSelect::CLogoSelect(int nPriority) : CScene2DRot(nPriority)
{
	// �e��l�̏�����
	m_fColTrans = 0.01f;     // �F�̕ω��Ɏg�p
	m_State = STATE_NONE;    // ���
	m_Type = TYPE_GAME;      // ���
	m_nCounterAnim = 0;      // �A�j���[�V�����J�E���^�[
	m_nPatternAnimU = 0;     // ���݂̃e�N�X�`��U���W
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CLogoSelect::~CLogoSelect()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CLogoSelect::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state)
{
	// �e��l�̐ݒ�
	SetObjType(OBJTYPE_LOGO);       // �I�u�W�F�N�g�̎��
	BindTexture(m_apTexture[type]); // �e�N�X�`��
	SetState(state);                // ���
	SetType(type);                  // ���

	// ���ʂ̏���������
	if (FAILED(CScene2DRot::Init(pos, col, fScale, 0.0f)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CLogoSelect::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CLogoSelect::Update(void)
{
	if (m_State == STATE_SELECT)
	{// �I������Ă����Ԃ�������
		m_nCounterAnim++;  // �A�j���[�V�����J�E���^�[��i�߂�

		D3DXCOLOR col = GetCol();  // �F���擾

		// �F��ω�������
		col.r += m_fColTrans;  // �ԐF
		col.g += m_fColTrans;  // �ΐF
		col.b += m_fColTrans;  // �F

		if (col.r < 0.3f || col.r > 1.0f
			|| col.g < 0.3f || col.g > 1.0f
			|| col.b < 0.3f || col.b > 1.0f)
		{// �F������̒l�𒴂���
			m_fColTrans *= -1;  // �F�̕ω����t�]������
		}

		// �F�̐ݒ�
		SetCol(col);

		// ���_���̐ݒ�
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // ���_�o�b�t�@�̎擾
		VERTEX_2D *pVtx;                                  // ���_�o�b�t�@�ւ̃|�C���^

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[
		pVtx[0].col = col;
		pVtx[1].col = col;
		pVtx[2].col = col;
		pVtx[3].col = col;

		if (m_nCounterAnim % SELECT_ANIMATION_SWITCH == 0)
		{// �A�j���[�V�����؂�ւ��^�C�~���O�ɓ��B����
			m_nPatternAnimU = m_nPatternAnimU + 1 % SELECT_ANIMATION_PATTERN;

			// �e�N�X�`�����W
			pVtx[0].tex.x = 0.0f + SELECT_ANIMATION_CALC_U;
			pVtx[1].tex.x = SELECT_ANIMATION_TEX_U_INI + SELECT_ANIMATION_CALC_U;
			pVtx[2].tex.x = 0.0f + SELECT_ANIMATION_CALC_U;
			pVtx[3].tex.x = SELECT_ANIMATION_TEX_U_INI + SELECT_ANIMATION_CALC_U;
		}

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CLogoSelect::Draw(void)
{
	// ���ʂ̕`�揈��
	CScene2D::Draw();
}

//=============================================================================
//    ��ނ̐ݒ�
//=============================================================================
void CLogoSelect::SetType(TYPE type)
{
	m_Type = type;
}

//=============================================================================
//    ��Ԃ̐ݒ�
//=============================================================================
void CLogoSelect::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
//    ��ނ̎擾
//=============================================================================
CLogoSelect::TYPE CLogoSelect::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    ��Ԃ̎擾
//=============================================================================
CLogoSelect::STATE CLogoSelect::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �A�j���[�V�����p�^�[����߂�����
//=============================================================================
void CLogoSelect::ResetTexU(void)
{
	// ���_���̐ݒ�
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // ���_�o�b�t�@�̎擾

	if (pVtxBuff != NULL)
	{// ���_�o�b�t�@����������Ă���
		VERTEX_2D *pVtx;  // ���_�o�b�t�@�ւ̃|�C���^

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// �e�N�X�`�����W
		pVtx[0].tex.x = 0.0f;
		pVtx[1].tex.x = SELECT_ANIMATION_TEX_U_INI;
		pVtx[2].tex.x = 0.0f;
		pVtx[3].tex.x = SELECT_ANIMATION_TEX_U_INI;

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();
	}
}

//*****************************************************************************
//    CLogoPause�̏���
//*****************************************************************************
//=============================================================================
//    ��������
//=============================================================================
CLogoPause *CLogoPause::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state, int nPriority)
{
	CLogoPause *pLogoPause = NULL;   // �|�[�Y���S�N���X�^�̃|�C���^
	if (pLogoPause == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pLogoPause = new CLogoPause(nPriority); // �|�[�Y���S�N���X�̃C���X�^���X����
		if (pLogoPause != NULL)
		{// ���������m�ۂł���
            // ����������
			if (FAILED(pLogoPause->Init(pos, col, fScale, type, state)))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return NULL;
	}

	return pLogoPause;
}

//=============================================================================
//    �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CLogoPause::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	 // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		char aTextureName[TYPE_MAX][256] =
		{// �e�N�X�`���̃t�@�C���p�X��
			PAUSELOGO_TEXTURE_NAME_0,
			PAUSELOGO_TEXTURE_NAME_1,
			PAUSELOGO_TEXTURE_NAME_2
		};
		for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
		{// �ǂݍ��ރe�N�X�`���̐������J��Ԃ�
			D3DXCreateTextureFromFile(pDevice, aTextureName[nCntTex], &m_apTexture[nCntTex]);
		}
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`���̉������
//=============================================================================
void CLogoPause::UnLoad(void)
{
	// �e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CLogoPause::CLogoPause(int nPriority) : CScene2DRot(nPriority)
{
	// �e��l�̏�����
	m_fColTrans = 0.01f;     // �F�̕ω��Ɏg�p
	m_State = STATE_NONE;    // ���
	m_Type = TYPE_CONTINUE;  // ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CLogoPause::~CLogoPause()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CLogoPause::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, TYPE type, STATE state)
{
	// �e��l�̐ݒ�
	SetObjType(OBJTYPE_LOGO);       // �I�u�W�F�N�g�̎��
	BindTexture(m_apTexture[type]); // �e�N�X�`��
	SetState(state);                // ���
	SetType(type);                  // ���

	// ���ʂ̏���������
	if (FAILED(CScene2DRot::Init(pos, col, fScale, 0.0f)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CLogoPause::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CLogoPause::Update(void)
{
	if (m_State == STATE_SELECT)
	{// �I������Ă����Ԃ�������
		D3DXCOLOR col = GetCol();  // �F���擾

		// �F��ω�������
		col.r += m_fColTrans;  // �ԐF
		col.g += m_fColTrans;  // �ΐF
		col.b += m_fColTrans;  // �F

		if (col.r < 0.3f || col.r > 1.0f
			|| col.g < 0.3f || col.g > 1.0f
			|| col.b < 0.3f || col.b > 1.0f)
		{// �F������̒l�𒴂���
			m_fColTrans *= -1;  // �F�̕ω����t�]������
		}

		// �F�̐ݒ�
		SetCol(col);

		// ���_���̐ݒ�
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = GetVtxBuff();  // ���_�o�b�t�@�̎擾

		if (pVtxBuff != NULL)
		{
			VERTEX_2D *pVtx;                                  // ���_�o�b�t�@�ւ̃|�C���^

			// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			// ���_�J���[
			pVtx[0].col = col;
			pVtx[1].col = col;
			pVtx[2].col = col;
			pVtx[3].col = col;

			// ���_�o�b�t�@���A�����b�N����
			pVtxBuff->Unlock();
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CLogoPause::Draw(void)
{
	// ���ʂ̏I������
	CScene2D::Draw();
}

//=============================================================================
//    ��ނ̐ݒ�
//=============================================================================
void CLogoPause::SetType(TYPE type)
{
	m_Type = type;
}

//=============================================================================
//    ��Ԃ̐ݒ�
//=============================================================================
void CLogoPause::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
//    ��ނ̎擾
//=============================================================================
CLogoPause::TYPE CLogoPause::GetType(void)
{
	return m_Type;
}

//=============================================================================
//    ��Ԃ̎擾
//=============================================================================
CLogoPause::STATE CLogoPause::GetState(void)
{
	return m_State;
}

//*****************************************************************************
//    CLogoPress�̏���
//*****************************************************************************
//=============================================================================
//    ��������
//=============================================================================
CLogoPress *CLogoPress::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale, int nPriority)
{
	CLogoPress *pLogoPress = NULL;   // PRESSENTER�N���X�^�̃|�C���^
	if (pLogoPress == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pLogoPress = new CLogoPress(nPriority); // PRESSENTER���S�N���X�̃C���X�^���X����
		if (pLogoPress != NULL)
		{// ���������m�ۂł���
		 // ����������
			if (FAILED(pLogoPress->Init(pos, col, fScale)))
			{// �������Ɏ��s����
				return NULL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return NULL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return NULL;
	}

	return pLogoPress;
}

//=============================================================================
//    �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CLogoPress::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	 // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			D3DXCreateTextureFromFile(pDevice, PRESSLOGO_TEXTURE_NAME, &m_pTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`���̉������
//=============================================================================
void CLogoPress::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CLogoPress::CLogoPress(int nPriority) : CScene2DRot(nPriority)
{
	// �e��l�̏�����
	m_State = STATE_NONE;    // ���
	m_nCounter = 0;          // �`����Ǘ�����J�E���^�[
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CLogoPress::~CLogoPress()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CLogoPress::Init(D3DXVECTOR3 pos, D3DXCOLOR col, float fScale)
{
	// �e��l�̐ݒ�
	SetObjType(OBJTYPE_LOGO);   // �I�u�W�F�N�g�̎��
	BindTexture(m_pTexture);    // �e�N�X�`��

	// ���ʂ̏���������
	if (FAILED(CScene2DRot::Init(pos, col, fScale, 0.0f)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CLogoPress::Uninit(void)
{
	// ���ʂ̏I������
	CScene2D::Uninit();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CLogoPress::Update(void)
{
	m_nCounter++;  // �J�E���^�[��i�߂�

	if (m_nCounter >= PRESSLOGO_RETURN_TIMING)
	{// �J�E���^�[������̒l�𒴂���
		m_nCounter = 0;
	}

	if (m_State == STATE_NONE)
	{// �����{�^����������Ă��Ȃ���Ԃ�������
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h���g�p����Ă��Ȃ���Ԃł���
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// ����{�^���������ꂽ
				m_State = STATE_PUSH;
			}
			else
			{// �L�[�{�[�h�̌���{�^����������Ă��Ȃ�
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_12; nCntButton++)
				{// ��������{�^�����̐������J��Ԃ�
					if (CManager::GetJoyStick()->GetJoyStickDevice(0)->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// �{�^���������ꂽ
						m_State = STATE_PUSH;
					}
				}
			}
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CLogoPress::Draw(void)
{
	if (m_State == STATE_NONE)
	{// �����{�^����������Ă��Ȃ���Ԃ�������
		if (m_nCounter / PRESSLOGO_DRAW_TIMING == 0)
		{// �J�E���^�[������̒l�ɒB����
            // ���ʂ̕`�揈��
			CScene2D::Draw();
		}
	}
	else if (m_State == STATE_PUSH)
	{// ����{�^���������ꂽ��Ԃ�������
		if (m_nCounter % PRESSLOGO_PUSH_DRAW_TIMING == 0)
		{// �J�E���^�[������̒l�ɂȂ���
		    // ���ʂ̕`�揈��
			CScene2D::Draw();
		}
	}
}

//=============================================================================
//    ��Ԃ̐ݒ�
//=============================================================================
void CLogoPress::SetState(STATE state)
{
	m_State = state;
}

//=============================================================================
//    ��Ԃ̎擾
//=============================================================================
CLogoPress::STATE CLogoPress::GetState(void)
{
	return m_State;
}