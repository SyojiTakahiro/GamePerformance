//*****************************************************************************
//
//     �|�[�Y�̏���[pause.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "pause.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "logo.h"
#include "scene2DSquare.h"
#include "scene2DRot.h"
#include "fade.h"
#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define PAUSE_PRIORITY         (7)                                                     // �|�[�Y�̕`��D�揇��

#define PAUSEBLACK_COL_INI     (D3DXCOLOR(0.0f,0.0f,0.0f,0.8f))                        // �|�[�Y�̍����|���S���̐F

#define PAUSEBG_TEXTURE_NAME   "data/TEXTURE/GAME/pause.png"                           // �|�[�Y�̔w�i�̃e�N�X�`���t�@�C�X�p�X��
#define PAUSEBG_POS_INI        (D3DXVECTOR3(SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0.0f))  // �|�[�Y�̔w�i�̍��W
#define PAUSEBG_WIDTH_INI      (300.0f)                                                // �|�[�Y�̔w�i�̕�
#define PAUSEBG_HEIGHT_INI     (310.0f)                                                // �|�[�Y�̔w�i�̍���

#define PAUSESELECT_POS_INI    (D3DXVECTOR3(SCREEN_WIDTH / 2,150.0f,0.0f))             // �|�[�Y���ڂ̍��W(�����l)
#define PAUSESELECT_COL_INI    (D3DXCOLOR(0.5f,0.5f,0.5f,1.0f))                        // �|�[�Y���ڂ̐F(�����l)
#define PAUSESELECT_SCALE_INI  (240.0f)                                                // �|�[�Y���ڂ̑傫��(�����l)
#define PAUSESELECT_SCALE_NONE (150.0f)                                                // �I������Ă��Ȃ��|�[�Y���ڂ̑傫��(�����l)
#define PAUSESELECT_COL_NONE   (D3DXCOLOR(0.1f,0.1f,0.1f,1.0f))                        // �I������Ă��Ȃ��|�[�Y���ڂ̐F(�����l)

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CPause::m_pTexture = NULL;    // �e�N�X�`���ւ̃|�C���^

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CPause::CPause(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̏�����
	m_pBlackPolygon = NULL;                // �|�[�Y�Ɏg�p���鍕���|���S��
	m_pPauseBg = NULL;                     // �|�[�Y�̔w�i�p�|���S��
	m_nSelect = SELECT_CONTINUE;           // ���ݑI�����Ă��鍀��
	m_nCounter = 0;                        // �`��Ǘ��J�E���^�[
	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// �|�[�Y��ʂ̑I�����ڂ̐������J��Ԃ�
		m_apLogoPause[nCntPause] = NULL;  // �|�[�Y���S�N���X�^�̃|�C���^
	}
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CPause::~CPause()
{

}

//=============================================================================
//    ��������
//=============================================================================
CPause *CPause::Create(void)
{
	CPause *pPause = NULL;  // �|�[�Y�N���X�^�̃|�C���^
	if (pPause == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pPause = new CPause;
		if (pPause != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pPause->Init();
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

	// �C���X�^���X��Ԃ�
	return pPause;
}

//=============================================================================
//    �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CPause::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	    // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if(pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			D3DXCreateTextureFromFile(pDevice, PAUSEBG_TEXTURE_NAME, &m_pTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`���̉������
//=============================================================================
void CPause::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CPause::Init(void)
{
	// �e��l�̐ݒ�
	SetObjType(OBJTYPE_PAUSE);  // �|�[�Y

	// �|�[�Y�̍����|���S���C���X�^���X��������
	if(m_pBlackPolygon == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pBlackPolygon = CScene2DSquare::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), PAUSEBLACK_COL_INI, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, PAUSE_PRIORITY);
	}

	// �|�[�Y�̔w�i�p�̃|���S���C���X�^���X��������
	if (m_pPauseBg == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pPauseBg = CScene2DSquare::Create(PAUSEBG_POS_INI, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PAUSEBG_WIDTH_INI, PAUSEBG_HEIGHT_INI, PAUSE_PRIORITY);
		if (m_pPauseBg != NULL)
		{// ���������m�ۂł���
			// �e�N�X�`���̊��蓖��
			m_pPauseBg->BindTexture(m_pTexture);
		}
	}

	D3DXVECTOR3 pos = PAUSESELECT_POS_INI;                // ���W
	D3DXCOLOR col = PAUSESELECT_COL_INI;                  // �F
	float fScale = PAUSESELECT_SCALE_INI;                 // �傫��
	CLogoPause::STATE state = CLogoPause::STATE_SELECT;   // ���

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// �|�[�Y��ʂ̑I�����ڂ̐������J��Ԃ�
		if (m_apLogoPause[nCntPause] == NULL)
		{// ���������m�ۂł����Ԃɂ���
			m_apLogoPause[nCntPause] = CLogoPause::Create(pos, col, fScale, (CLogoPause::TYPE)nCntPause, state, PAUSE_PRIORITY);
			if (m_apLogoPause[nCntPause] != NULL)
			{
				LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apLogoPause[nCntPause]->GetVtxBuff(); // ���_���擾
				if (pVtxBuff != NULL)
				{// ���_�����擾�ł���
				    // ���_���ݒ�
					VERTEX_2D *pVtx;
					// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
					pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

					// ���_���W
					pVtx[0].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.65f) * fScale, pos.y + cosf(-D3DX_PI * 0.65f) * fScale, 0.0f);
					pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.65f) * fScale, pos.y + cosf(D3DX_PI * 0.65f) * fScale, 0.0f);
					pVtx[2].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.35f) * fScale, pos.y + cosf(-D3DX_PI * 0.35f) * fScale, 0.0f);
					pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.35f) * fScale, pos.y + cosf(D3DX_PI * 0.35f) * fScale, 0.0f);

					// ���_�o�b�t�@���A�����b�N����
					pVtxBuff->Unlock();

					// ���_�o�b�t�@��ݒ肷��
					m_apLogoPause[nCntPause]->SetVtxBuff(pVtxBuff);
				}
			}
		}

		if (nCntPause == 0)
		{// 1��ڂ̌J��Ԃ����I�����
			fScale = PAUSESELECT_SCALE_NONE;
			col = PAUSESELECT_COL_NONE;
			state = CLogoPause::STATE_NONE;
		}
		pos.y += 200.0f;
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CPause::Uninit(void)
{
	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// �|�[�Y��ʂ̑I�����ڂ̐������J��Ԃ�
		if (m_apLogoPause[nCntPause] != NULL)
		{// ���������m�ۂ���Ă���
			// �|�[�Y���S�̏I������
			m_apLogoPause[nCntPause]->Uninit();
			m_apLogoPause[nCntPause] = NULL;
		}
	}

	// �w�i�|���S���̊J��
	if (m_pPauseBg != NULL)
	{// ���������m�ۂ���Ă���
		m_pPauseBg->Uninit();
		m_pPauseBg = NULL;
	}

	// �����|���S���̊J������
	if (m_pBlackPolygon != NULL)
	{// ���������m�ۂ���Ă���
		m_pBlackPolygon->Uninit();
		m_pBlackPolygon = NULL;
	}

	// ���S�t���O�𗧂Ă�
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CPause::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();  // �L�[�{�[�h�̎擾
	CJoyStick *pJoyStick = CManager::GetJoyStick()->GetJoyStickDevice(0);

	for (int nCntPause = 0; nCntPause < SELECT_MAX; nCntPause++)
	{// �|�[�Y��ʂ̑I�����ڂ̐������J��Ԃ�
		if (m_apLogoPause[nCntPause] != NULL)
		{// ���������m�ۂ���Ă���
		    // �|�[�Y���S�̏I������
			m_apLogoPause[nCntPause]->Update();
		}
	}


	// �I�����ڂ̏���
	if (pKeyboard->GetTrigger(DIK_W) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_20) == TRUE || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_28) == TRUE
		|| pKeyboard->GetRepeat(DIK_W) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_20) == TRUE || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_28) == TRUE)
	{// ������̓��͂����ꂽ

		if (m_apLogoPause[m_nSelect] != NULL)
		{
			// �I������Ă������ڂ̐ݒ�
			m_apLogoPause[m_nSelect]->SetState(CLogoPause::STATE_NONE);                 // ���
			m_apLogoPause[m_nSelect]->SetVtxBuffCol(PAUSESELECT_COL_NONE);              // �F
			m_apLogoPause[m_nSelect]->SetScale(PAUSESELECT_SCALE_NONE);                 // �傫��
			m_apLogoPause[m_nSelect]->SetVtxBuffPos(m_apLogoPause[m_nSelect]->GetPos());// ���W

			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apLogoPause[m_nSelect]->GetVtxBuff(); // ���_���擾
			if (pVtxBuff != NULL)
			{// ���_�����擾�ł���
			    // ���_���ݒ�
				VERTEX_2D *pVtx;
				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
				D3DXVECTOR3 pos = m_apLogoPause[m_nSelect]->GetPos();

				// ���_���W
				pVtx[0].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, 0.0f);

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();

				// ���_�o�b�t�@��ݒ肷��
				m_apLogoPause[m_nSelect]->SetVtxBuff(pVtxBuff);
			}
		}

		// ���ɑI������鍀�ڂ̐ݒ�
		if (m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX] != NULL)
		{
			m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetState(CLogoPause::STATE_SELECT);                                                 // ���
			m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetVtxBuffCol(PAUSESELECT_COL_INI);                                                 // �F
			m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetScale(PAUSESELECT_SCALE_INI);                                                    // �傫��
			m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetVtxBuffPos(m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->GetPos());// ���W

			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->GetVtxBuff(); // ���_���擾
			if (pVtxBuff != NULL)
			{// ���_�����擾�ł���
			    // ���_���ݒ�
				VERTEX_2D *pVtx;
				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
				D3DXVECTOR3 pos = m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->GetPos();

				// ���_���W
				pVtx[0].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, pos.y + cosf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, pos.y + cosf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, pos.y + cosf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, pos.y + cosf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, 0.0f);

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();

				// ���_�o�b�t�@��ݒ肷��
				m_apLogoPause[(m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX]->SetVtxBuff(pVtxBuff);
			}
		}

		// �ԍ���ݒ�
		m_nSelect = (m_nSelect + (SELECT_MAX - 1)) % SELECT_MAX;

		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	}
	else if (pKeyboard->GetTrigger(DIK_S) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_21) == TRUE || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_29) == TRUE
		|| pKeyboard->GetRepeat(DIK_S) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_21) == true || pJoyStick->GetRepeat(CJoyStick::DIJS_BUTTON_29) == TRUE)
	{// �������̓��͂����ꂽ
	    // �I������Ă������ڂ̐ݒ�
		if (m_apLogoPause[m_nSelect] != NULL)
		{
			m_apLogoPause[m_nSelect]->SetState(CLogoPause::STATE_NONE);                 // ���
			m_apLogoPause[m_nSelect]->SetVtxBuffCol(PAUSESELECT_COL_NONE);              // �F
			m_apLogoPause[m_nSelect]->SetScale(PAUSESELECT_SCALE_NONE);                 // �傫��
			m_apLogoPause[m_nSelect]->SetVtxBuffPos(m_apLogoPause[m_nSelect]->GetPos());// ���W

			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apLogoPause[m_nSelect]->GetVtxBuff(); // ���_���擾
			if (pVtxBuff != NULL)
			{// ���_�����擾�ł���
			    // ���_���ݒ�
				VERTEX_2D *pVtx;
				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
				D3DXVECTOR3 pos = m_apLogoPause[m_nSelect]->GetPos();

				// ���_���W
				pVtx[0].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_NONE, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, pos.y + cosf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_NONE, 0.0f);

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();

				// ���_�o�b�t�@��ݒ肷��
				m_apLogoPause[m_nSelect]->SetVtxBuff(pVtxBuff);
			}
		}

		// ���ɑI������鍀�ڂ̐ݒ�
		if (m_apLogoPause[(m_nSelect + 1) % SELECT_MAX] != NULL)
		{
			m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetState(CLogoPause::STATE_SELECT);                                  // ���
			m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetVtxBuffCol(PAUSESELECT_COL_INI);                                  // �F
			m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetScale(PAUSESELECT_SCALE_INI);                                     // �傫��
			m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetVtxBuffPos(m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->GetPos());// ���W

			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->GetVtxBuff(); // ���_���擾
			if (pVtxBuff != NULL)
			{// ���_�����擾�ł���
			    // ���_���ݒ�
				VERTEX_2D *pVtx;
				// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
				pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
				D3DXVECTOR3 pos = m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->GetPos();

				// ���_���W
				pVtx[0].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, pos.y + cosf(-D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, pos.y + cosf(D3DX_PI * 0.65f) * PAUSESELECT_SCALE_INI, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(pos.x + sinf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, pos.y + cosf(-D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(pos.x + sinf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, pos.y + cosf(D3DX_PI * 0.35f) * PAUSESELECT_SCALE_INI, 0.0f);

				// ���_�o�b�t�@���A�����b�N����
				pVtxBuff->Unlock();

				// ���_�o�b�t�@��ݒ肷��
				m_apLogoPause[(m_nSelect + 1) % SELECT_MAX]->SetVtxBuff(pVtxBuff);
			}
		}
		// �ԍ���ݒ�
		m_nSelect = (m_nSelect + 1) % SELECT_MAX;

		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_SELECT);
	}

	if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true || pJoyStick->GetTrigger(CJoyStick::DIJS_BUTTON_2) == TRUE)
	{// ����{�^���������ꂽ
		// �����_�����O�N���X�̎擾
		CRenderer *pRenderer = CManager::GetRenderer();

		CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);

		switch (m_nSelect)
		{// ���ݑI������Ă������ڂɂ���ď����킯
		case CPause::SELECT_CONTINUE:  // �ĊJ
			pRenderer->SetPause(false);
			break;
		case CPause::SELECT_RETRY:     // ��蒼��
			CManager::GetFade()->SetFade(CManager::MODE_GAME);
			break;
		case CPause::SELECT_QUIT:      // �I��
			CManager::GetFade()->SetFade(CManager::MODE_TITLE);
			break;
		}

		// �|�[�Y�̊J��
		pRenderer->ReleasePause();
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CPause::Draw(void)
{

}

//=============================================================================
//    �I�����ڂ̎擾
//=============================================================================
int CPause::GetSelect(void)
{
	return m_nSelect;
}