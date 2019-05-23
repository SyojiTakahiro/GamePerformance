//*****************************************************************************
//
//     �^�C�g���̏���[title.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "title.h"
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "logo.h"
#include "scene2DRot.h"
#include "sound.h"

#include "debuglog.h"
#include "map.h"
#include "cylinder.h"
#include "dome.h"
#include "shadow.h"
#include "field.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define TITLE_PRIORITY           (6)                                       // �^�C�g���̕`��D�揇��

#define TITLE_MODE_CHANGETIMING  (600)                                     // �ǂꂭ�炢�̎��Ԃŏ���ɑJ�ڂ��邩
#define TITLE_PRESSLOGO_POS      (D3DXVECTOR3(SCREEN_WIDTH / 2,640,0.0f))  // PRESSENTER���o���ʒu
#define TITLE_PRESSLOGO_SCALE    (350)                                     // PRESSENTER�̑傫��

#define TITLE_LOGO_POS           (D3DXVECTOR3(SCREEN_WIDTH / 2,280,0.0f))  // �^�C�g�����S���o���ʒu
#define TITLE_LOGO_SCALE         (550)                                     // �^�C�g�����S�̑傫��
#define TITLE_LOGO_TEXTURENAME   "data/TEXTURE/TITLE/titlelogo.png"

#define GAME_MANAGER_INITFILENAME  "data/TEXT/MODE/game.ini"   // �Q�[���̃V�X�e���t�@�C����

// �l��ǂݎ��p�X��
#define PLAYER_FILENAME "PLAYER_FILENAME"
#define MAP_FILENAME    "MAP_FILENAME"

#define NUM_MODEL        "NUM_MODEL = "
#define MODEL_FILENAME   "MODEL_FILENAME = "

#define GAME_MAP_FILENAME_0        "data/TEXT/MAP/map000.txt"  // �Q�[���Ŏg�p����}�b�v�X�N���v�g�t�@�C����
#define GAME_MAP_FILENAME_1        "data/TEXT/MAP/map001.txt"  // �Q�[���Ŏg�p����}�b�v�X�N���v�g�t�@�C����
//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
LPDIRECT3DTEXTURE9 CTitle::m_apTexture = NULL;  // �^�C�g���̃e�N�X�`���摜

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CTitle::CTitle(int nPriority, OBJTYPE objType) : CScene(nPriority,objType)
{
	// �e��l�̃N���A
	m_pMap = NULL;        // �}�b�v�N���X�ւ̃|�C���^
	m_pLogoPress = NULL;  // PRESSENTER�N���X�ւ̃|�C���^
	m_nCounter = 0;       // �J�ڂ��Ǘ�����J�E���^�[
	m_pLogo = NULL;       // �^�C�g�����S�N���X�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CTitle::~CTitle()
{

}

//=============================================================================
//    ��������
//=============================================================================
CTitle *CTitle::Create(void)
{
	CTitle *pTitle = NULL;  // �^�C�g���N���X�^�̃|�C���^
	if (pTitle == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pTitle = new CTitle;
		if (pTitle != NULL)
		{// ���������m�ۂł���
		    // �������������s��
			pTitle->Init();
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
	return pTitle;
}

//=============================================================================
//    �e�N�X�`���̓ǂݍ��ݏ���
//=============================================================================
HRESULT CTitle::Load(void)
{
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	 // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			D3DXCreateTextureFromFile(pDevice, TITLE_LOGO_TEXTURENAME, &m_apTexture);
		}
	}

	return S_OK;
}

//=============================================================================
//    �e�N�X�`���̉������
//=============================================================================
void CTitle::UnLoad(void)
{
	// �e�N�X�`���̔j��
	if (m_apTexture != NULL)
	{
		m_apTexture->Release();
		m_apTexture = NULL;
	}
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CTitle::Init(void)
{
	// �e�N�X�`���̓ǂݍ���
	CLogoPress::Load();   // PRESSENTER
	CCylinder::Load();    // �R
	CDome::Load();        // ��
	CField::Load();       // �t�B�[���h
	CShadow::Load();      // �e
	Load();               // �^�C�g�����S

	FILE *pFile = NULL;
	char aString[256];
	char aMapFileName[256];
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃɂ���
		pFile = fopen(GAME_MANAGER_INITFILENAME, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
			while (fscanf(pFile, "%s", &aString[0]) != EOF)
			{// �e�L�X�g���Ō�̍s�ɓ��B����܂œǂݍ��݂𑱂���
				if (strcmp(&aString[0], MAP_FILENAME) == 0)
				{// �}�b�v�̃X�N���v�g�t�@�C����������
					fscanf(pFile, "%s %s", &aString[0], &aMapFileName[0]);
				}
			}
		}
	}

	if (m_pMap == NULL)
	{
		m_pMap = CMap::Create(aMapFileName);
	}

	// �^�C�g�����S�̃C���X�^���X����
	if (m_pLogo == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pLogo = CLogo::Create(TITLE_LOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TITLE_LOGO_SCALE, TITLE_PRIORITY);
		m_pLogo->BindTexture(m_apTexture);

		// ���_�o�b�t�@�̎擾
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pLogo->GetVtxBuff();

		// ���_���
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(-600.0f, -300.0f, 0.0f) + TITLE_LOGO_POS;
		pVtx[1].pos = D3DXVECTOR3(600.0f, -300.0f, 0.0f) + TITLE_LOGO_POS;
		pVtx[2].pos = D3DXVECTOR3(-600.0f, 300.0f, 0.0f) + TITLE_LOGO_POS;
		pVtx[3].pos = D3DXVECTOR3(600.0f, 300.0f, 0.0f) + TITLE_LOGO_POS;

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();

		// ���_�o�b�t�@�̐ݒ�
		m_pLogo->SetVtxBuff(pVtxBuff);
	}

	// PRESSENTER�̃C���X�^���X����
	if (m_pLogoPress == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pLogoPress = CLogoPress::Create(TITLE_PRESSLOGO_POS, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), TITLE_PRESSLOGO_SCALE, TITLE_PRIORITY);

		// ���_�o�b�t�@�̎擾
		LPDIRECT3DVERTEXBUFFER9 pVtxBuff = m_pLogoPress->GetVtxBuff();

		// ���_���
		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N��,���_�f�[�^�ւ̃|�C���^���擾
		pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// ���_���W
		pVtx[0].pos = D3DXVECTOR3(-300.0f, -80.0f, 0.0f) + TITLE_PRESSLOGO_POS;
		pVtx[1].pos = D3DXVECTOR3(300.0f, -80.0f, 0.0f) + TITLE_PRESSLOGO_POS;
		pVtx[2].pos = D3DXVECTOR3(-300.0f, 80.0f, 0.0f) + TITLE_PRESSLOGO_POS;
		pVtx[3].pos = D3DXVECTOR3(300.0f, 80.0f, 0.0f) + TITLE_PRESSLOGO_POS;

		// ���_�o�b�t�@���A�����b�N����
		pVtxBuff->Unlock();

		// ���_�o�b�t�@�̐ݒ�
		m_pLogoPress->SetVtxBuff(pVtxBuff);
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CTitle::Uninit(void)
{
	// �e�N�X�`���̊J��
	CLogoPress::UnLoad();   // PRESSENTER
	CCylinder::UnLoad();    // �R
	CDome::UnLoad();        // ��
	CField::UnLoad();       // �t�B�[���h
	CShadow::UnLoad();      // �e
	UnLoad();               // �^�C�g�����S

	if (m_pLogoPress != NULL)
	{// ���������m�ۂł����Ԃɂ���
		// PRESSENTER�̏I������
		m_pLogoPress->Uninit();
		m_pLogoPress = NULL;
	}

	if (m_pLogo != NULL)
	{// ���������m�ۂł����Ԃɂ���
	    // �^�C�g�����S�̏I������
		m_pLogo->Uninit();
		m_pLogo = NULL;
	}

	// �}�b�v�̔j��
	if (m_pMap != NULL)
	{// ���������m�ۂ���Ă���
		m_pMap->Uninit();

		// �������̊J��
		delete m_pMap;
		m_pMap = NULL;
	}

	// �^�C�g���I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CTitle::Update(void)
{
	if (m_pLogoPress != NULL)
	{
		m_pLogoPress->Update();
	}

	m_nCounter++;  // �J�E���^�[��i�߂�

	if (m_nCounter >= TITLE_MODE_CHANGETIMING)
	{// �J�E���^�[������̒l�𒴂���
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			CManager::GetFade()->SetFade(CManager::MODE_RANKING);
		}
	}
	else
	{// �J�E���^�[���܂�����̒l�𒴂��Ă��Ȃ�
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			if (CManager::GetKeyboard()->GetTrigger(DIK_RETURN) == true)
			{// ����{�^���������ꂽ
				CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
			}
			else
			{// �L�[�{�[�h�̌���{�^����������Ă��Ȃ�
				for (int nCntButton = 0; nCntButton < CJoyStick::DIJS_BUTTON_12; nCntButton++)
				{// ��������{�^�����̐������J��Ԃ�
					if (CManager::GetJoyStick()->GetJoyStickDevice(0)->GetTrigger((CJoyStick::DIJS_BUTTON)nCntButton) == TRUE)
					{// �{�^���������ꂽ
						CManager::GetFade()->SetFade(CManager::MODE_TUTORIAL);
						CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_PRESS);
					}
				}
			}
		}
	}

	CDebugProc::Print("�^�C�g���Ȃ�\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CTitle::Draw(void)
{

}