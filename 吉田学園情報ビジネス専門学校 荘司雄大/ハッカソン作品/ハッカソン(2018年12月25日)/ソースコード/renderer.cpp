//*****************************************************************************
//
//     �����_�����O�̏���[renderer.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "pause.h"
#include "camera.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "game.h"
#include "functionlib.h"

#include "debuglog.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
#define PAUSE_DRAW     (true)     // �|�[�Y��`�悷�邩�ǂ���
#define PAUSE_PRIORITY (6)        // �|�[�Y��ʂ̏����D�揇��

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CRenderer::CRenderer()
{
	// �e��l�̃N���A
	m_pPause = NULL;            // �|�[�Y�N���X�ւ̃|�C���^
	m_bPause = false;           // �|�[�Y�����ǂ���
	m_pD3DDevice = NULL;        // �f�o�C�X�ւ̃|�C���^
	m_pD3D = NULL;              // Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	m_pZBufferSurface = NULL;   // Z�o�b�t�@�ւ̃|�C���^
	m_pTexture = NULL;          // �e�N�X�`���ւ̃|�C���^
	m_pVtxBuff = NULL;          // ���_�o�b�t�@�ւ̃|�C���^
	m_Target = TARGET_TEXTURE;  // �I�u�W�F�N�g��`�悷�郌���_�����O�^�[�Q�b�g
	m_fRot = 0.0f;              // �|���S���̌���
	m_fAngle = 0.0f;            // �|���S���o���p�x
	m_fWidth = 0.0f;            // �|���S���̕�
	m_fHeight = 0.0f;           // �|���S���̍���
	m_fLength = 0.0f;           // �|���S���̒���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CRenderer::~CRenderer()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̍쐬
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));								// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;							            // �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;				            // �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;				            // �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;			            	// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;		                // �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;						    // �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;					// �f�v�X�o�b�t�@�Ƃ���24bit�A�X�e���V���o�b�t�@�Ƃ���8bit���g��
	d3dpp.Windowed = bWindow;						                // �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	    // �C���^�[�o��
	d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;              // �_�u���X�e���V��

	// �f�o�C�X�̐���
	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pD3DDevice)))
	{
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pD3DDevice)))
		{
			// ��L�̐ݒ肪���s������
			// �`��ƒ��_������CPU�ōs�Ȃ�
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF, hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp, &m_pD3DDevice)))
			{
				// �������s
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);              // ���ʂ��J�����O
	m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);                      // Z�o�b�t�@���g�p
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);             // ���u�����h���s��
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);                     // ���C�e�B���O���s��
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);        // ���\�[�X�J���[�̎w��
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);    // ���f�X�e�B�l�[�V�����J���[�̎w��

	// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(U�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`���A�h���b�V���O���@(V�l)��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���k���t�B���^���[�h��ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���g��t�B���^���[�h��ݒ�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE); // �A���t�@�u�����f�B���O������ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); // �ŏ��̃A���t�@����
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT); // �Q�Ԗڂ̃A���t�@����

	// �I�u�W�F�N�g�����_�����O�p�̃e�N�X�`�����쐬
	// D3DXCreateTexture�֐��ō쐬���܂�
	// ��P���� : �`��f�o�C�X�ւ̃|�C���^(��{�I�ɏ�ō쐬�ς݂̃f�o�C�X���g�p)
	// ��Q���� : �쐬����e�N�X�`���̕�(��ʕ��ɍ��킹�ČŒ�)
	// ��R���� : �쐬����e�N�X�`���̍���(��ʍ����ɍ��킹�ČŒ�)
	// ��S���� : �~�b�v�}�b�v���x��(�K���P���w�肷�邱�ƁI)
	// ��T���� : �����_�����O�\�ȃe�N�X�`���̃t�H�[�}�b�g(��{�I��[ D3DFMT_A8R8G8B8 ]��OK)
	// ��U���� : �������Ǘ����@(�t�H�[�}�b�g�ɂ���邪��{�I��[ D3DPOOL_DEFAULT ]��OK)
	// ��V���� : �e�N�X�`���ւ̃|�C���^(��̃|�C���^��p�ӂ��邱��)
	if (FAILED(D3DXCreateTexture(m_pD3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1,
	D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTexture)))
	{// �쐬�Ɏ��s����
		return E_FAIL;
	}

	// �e�N�X�`�������_�����O��p��Z�o�b�t�@���쐬
	// �e�N�X�`���̃T�[�t�F�C�X�����擾����(Z�o�b�t�@�̍쐬�ɕK�v)
	D3DSURFACE_DESC desc;
	m_pTexture->GetLevelDesc(0, &desc);

	// CreateDepthStencilSurface�֐��ō쐬���܂�
	// ��P���� : Z�l�e�N�X�`���̎�����(��ō쐬�����e�N�X�`���̃T�[�t�F�C�X��񂩂甲�����)
	// ��Q���� : Z�l�e�N�X�`���̎�������(��ō쐬�����e�N�X�`���̃T�[�t�F�C�X��񂩂甲�����)
	// ��R���� : �[�x�o�b�t�@�̃t�H�[�}�b�g(�f�o�C�X�̃t�H�[�}�b�g�Ƒ����邱�ƁI)
	// ��S���� : �}���`�T���v�����O�o�b�t�@�^�C�v(��{�I�ɂ�[ D3DMULTISAMPLE_NONE ]��OK)
	// ��T���� : �i�����x��(��{�I��0���w��)
	// ��U���� : Z�o�b�t�@�̔j����L���ɂ��邩�ǂ���(����͔j���ł���悤�ɂ��Ȃ��Ƃ����Ȃ��̂�TRUE���w��)
	// ��V���� : LPDIRECT3DSURFACE9�^�̃|�C���^(����̓����o�ϐ��Ɋi�[���Ă���)
	// ��W���� : ��{�I��NULL��OK
	if (FAILED(m_pD3DDevice->CreateDepthStencilSurface(desc.Width, desc.Height,
		D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, TRUE, &m_pZBufferSurface, NULL)))
	{// �쐬�Ɏ��s����
		return E_FAIL;
	}

	// �e�N�X�`���ݒ�p�̔|���S�����쐬����
	// ���_�o�b�t�@�𐶐�
	m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
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
	m_fWidth = SCREEN_WIDTH / 2;
	m_fHeight = SCREEN_HEIGHT / 2;

	// �I�t�Z�b�g�̒��������߂�
	m_fLength = sqrtf((m_fWidth * m_fWidth) + (m_fHeight * m_fHeight));

	// �I�t�Z�b�g�̊p�x�����߂�
	m_fAngle = atan2f(m_fWidth, -m_fHeight);

	// ���_�̉�]���l�����č��W���v�Z
	float XPos[4];
	float YPos[4];
	XPos[0] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[0] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[1] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	YPos[1] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, cosf(m_fAngle) * m_fLength);
	XPos[2] = CFunctionLib::RotationVectorX(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[2] = CFunctionLib::RotationVectorY(m_fRot, -sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	XPos[3] = CFunctionLib::RotationVectorX(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);
	YPos[3] = CFunctionLib::RotationVectorY(m_fRot, sinf(m_fAngle) * m_fLength, -cosf(m_fAngle) * m_fLength);

	// DirectX���X�^���C�Y�̃��[���㍶���0.5f���炷���Ƃŕ`���␳����
	// X���W
	XPos[0] -= 0.5f;
	XPos[2] -= 0.5f;

	// Y���W
	YPos[0] -= 0.5f;
	YPos[1] -= 0.5f;
	// ���_���W
	for (int nCntVer = 0; nCntVer < 4; nCntVer++)
	{// ���_�̐������J��Ԃ�
		pVtx[nCntVer].pos = D3DXVECTOR3(XPos[nCntVer], YPos[nCntVer], 0.0f) + D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);
	}

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

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CRenderer::Uninit(void)
{
	// �|�[�Y�N���X�̔j��
	ReleasePause();

	// ���_�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pVtxBuff);

	// �e�N�X�`���̔j��
	DIRECT_RELEASE(m_pTexture);

	// Z�o�b�t�@�̔j��
	DIRECT_RELEASE(m_pZBufferSurface);

	// �f�o�C�X�̔j��
	DIRECT_RELEASE(m_pD3DDevice);

	// Direct3D�I�u�W�F�N�g�̔j��
	DIRECT_RELEASE(m_pD3D);
}

//=============================================================================
//    �X�V����
//=============================================================================
void CRenderer::Update(void)
{
	CManager::MODE mode = CManager::GetMode();  // ���݂̃��[�h���擾

	if (mode == CManager::MODE_GAME)
	{// �Q�[������������
		if (CManager::GetGame()->GetState() != CGame::STATE_END)
		{// �Q�[�����ł���
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// �t�F�[�h���Ă��Ȃ�������
				if (CManager::GetKeyboard()->GetTrigger(DIK_P) == true || CManager::GetJoyStick()->GetJoyStickDevice()->GetTrigger(CJoyStick::DIJS_BUTTON_11) == TRUE)
				{// �|�[�YON/OFF
					m_bPause = m_bPause ? false : true;
					if (m_pPause == NULL && m_bPause == true)
					{// �|�[�Y���Ƀ��������m�ۂ���Ă��Ȃ�
						m_pPause = CPause::Create(PAUSE_PRIORITY);
						CManager::GetGame()->BindPauseTexture(m_pPause);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PAUSE_OPEN);
					}
					else if (m_pPause != NULL)
					{// �|�[�Y�������Ƀ��������m�ۂ���Ă���
					    // �|�[�Y�̊J��
						ReleasePause();
					}
				}
			}
		}
	}

	if (m_bPause == false)
	{// �|�[�Y���łȂ�
	    // �V�[���N���X�̍X�V����
		CScene::UpdateAll();
	}
	else
	{// �|�[�Y���ł���
		if (m_pPause != NULL)
		{// ���������m�ۂ���Ă���
		    // �|�[�Y�̍X�V����
			m_pPause->Update();
			CScene::DeathCheck();
		}
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CRenderer::Draw(void)
{
	if (m_Target == TARGET_DEFAULT)
	{// �ʏ�̕`��`���Ȃ�
		DefaultRender();
	}
	else if (m_Target == TARGET_TEXTURE)
	{// �^�[�Q�b�g���e�N�X�`���Ȃ��
		TextureRender();
	}
}

//=============================================================================
//    �ʏ�̕`��`��
//=============================================================================
void CRenderer::DefaultRender(void)
{
	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n(�����_�����O�^�[�Q�b�g�̓e�N�X�`��)
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// �J�����̐ݒ�
		CManager::GetCamera()->SetCamera();

		// �V�[���N���X�̕`�揈��
		CScene::DrawAll();

		// �f�o�b�N���\��
		CDebugProc::Draw();

		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
//    �e�N�X�`���������_�����O�^�[�Q�b�g�ɂ����ꍇ�̕`��`��
//=============================================================================
void CRenderer::TextureRender(void)
{
	LPDIRECT3DSURFACE9 pBackBufferDef;  // �f�t�H���g�̃o�b�N�o�b�t�@�ւ̃|�C���^(�ۑ��p)
	LPDIRECT3DSURFACE9 pZBufferDef;     // �f�t�H���g��Z�o�b�t�@�ւ̃|�C���^(�ۑ��p)

	// ���݂̃o�b�N�o�b�t�@�AZ�o�b�t�@��ۑ�����
	m_pD3DDevice->GetRenderTarget(0, &pBackBufferDef);
	m_pD3DDevice->GetDepthStencilSurface(&pZBufferDef);

	// �e�N�X�`���̃T�[�t�F�C�X�������
	LPDIRECT3DSURFACE9 pSurface = NULL;
	m_pTexture->GetSurfaceLevel(0, &pSurface);

	// �����_�����O�^�[�Q�b�g�����O�ō쐬�����e�N�X�`���ɐݒ�(���̍ۂ�Z�o�b�t�@���쐬�����e�N�X�`����p�̂��̂ɐ؂�ւ���)
	m_pD3DDevice->SetRenderTarget(0, pSurface);
	m_pD3DDevice->SetDepthStencilSurface(m_pZBufferSurface);
	DIRECT_RELEASE(pSurface);

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n(�����_�����O�^�[�Q�b�g�̓e�N�X�`��)
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		// �J�����̐ݒ�
		CManager::GetCamera()->SetCamera();

		// �V�[���N���X�̕`�揈��
		CScene::DrawAll();

		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �����_�����O�^�[�Q�b�g���f�t�H���g�̃o�b�N�o�b�t�@��Z�o�b�t�@�ɖ߂�
	m_pD3DDevice->SetRenderTarget(0, pBackBufferDef);
	m_pD3DDevice->SetDepthStencilSurface(pZBufferDef);
	DIRECT_RELEASE(pBackBufferDef);
	DIRECT_RELEASE(pZBufferDef);

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0x00, 0x00, 0x00, 0x00), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n(�����_�����O�^�[�Q�b�g�̓f�t�H���g�̃o�b�N�o�b�t�@)
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{
		DWORD LightingDef;   // ���C�e�B���O�̐ݒ�
		DWORD FillMode;      // �`����@

		// ���C�e�B���O�𖳌��ɂ��Ă���
		m_pD3DDevice->GetRenderState(D3DRS_LIGHTING, &LightingDef);
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);

		// �ʏ�̕`����@�ɖ߂��Ă���
		m_pD3DDevice->GetRenderState(D3DRS_FILLMODE, &FillMode);
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		// �|���S���ɂ����قǃI�u�W�F�N�g��`�悵���e�N�X�`����ݒ肵�|���S����`�悷��
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		m_pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		m_pD3DDevice->SetTexture(0, m_pTexture);

		// �|���S���̕`��
		m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		// ���C�e�B���O��߂��Ă���
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, LightingDef);

		// �`����@��߂��Ă���
		m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, FillMode);

		// �f�o�b�N���\��
		CDebugProc::Draw();

		// Direct3D�ɂ��`��̏I��
		m_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
//    �f�o�C�X�̎擾
//=============================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}

//=============================================================================
//    �|�[�Y�̊J��
//=============================================================================
void CRenderer::ReleasePause(void)
{
	if (m_pPause != NULL)
	{// �|�[�Y�������Ƀ��������m�ۂ���Ă���
	    // �|�[�Y�̏I������
		m_pPause->Uninit();

		// �������̊J��
		delete m_pPause;
		m_pPause = NULL;
	}
}

//=============================================================================
//    �|�[�Y�̐ݒ�
//=============================================================================
void CRenderer::SetPause(bool bPause)
{
	m_bPause = bPause;
}

//=============================================================================
//    �|�[�Y�̎擾
//=============================================================================
bool CRenderer::GetPause(void)
{
	return m_bPause;
}