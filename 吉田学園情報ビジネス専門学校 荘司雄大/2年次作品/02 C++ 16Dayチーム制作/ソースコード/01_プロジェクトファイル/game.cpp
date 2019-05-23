//*****************************************************************************
//
//     �Q�[���̏���[game.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "game.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "result.h"
#include "pause.h"
#include "renderer.h"
#include "camera.h"

#include "scene2D.h"
#include "scene3D.h"
#include "sceneBillboard.h"
#include "scene3DMesh.h"
#include "sceneX.h"
#include "player.h"
#include "field.h"
#include "shadow.h"
#include "object.h"
#include "cylinder.h"
#include "dome.h"
#include "map.h"
#include "balloon.h"
#include "timer.h"
#include "number.h"
#include "water.h"
#include "pause.h"
#include "logo.h"
#include "result.h"
#include "particle.h"

#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define GAME_MANAGER_INITFILENAME   "data/TEXT/MODE/game.ini"       // �Q�[���̃V�X�e���t�@�C����
#define GAME_MAPSELECT_FILENAME     "data/TEXT/MAP/map_select.txt"  // �}�b�v�I����ʂ̃t�@�C����
#define GAME_MAPCLEAR_CHANGETIMING  (60)                            // �}�b�v�N���A��ԂɂȂ��Ă���}�b�v�I����ʂɖ߂�܂ł̎���
#define GAME_MODE_CHANGE_TIMING     (60)                            // �I����ԂɂȂ��Ă����ʑJ�ڂ���܂ł̎���
#define GAME_PLAYER_RESPAWNTIMING   (120)                           // �v���C���[�����񂾏�ԂɂȂ��Ă���}�b�v�U����ʂɖ߂�܂ł̎���

#define GAME_TEXTURE_NAME           "data/TEXTURE/GAME/sousa.png"
#define GAME_POLYGON_POS            (D3DXVECTOR3(150.0f,75.0f,0.0f))
#define GAME_POLYGON_COL            (D3DXCOLOR(1.0f,1.0f,1.0f,1.0f))
#define GAME_POLYGON_WIDTH          (150.0f)
#define GAME_POLYGON_HEIGHT         (75.0f)

// �l��ǂݎ��p�X��
#define PLAYER_FILENAME "PLAYER_FILENAME"
#define MAP_FILENAME    "MAP_FILENAME"

#define NUM_MODEL        "NUM_MODEL = "
#define MODEL_FILENAME   "MODEL_FILENAME = "

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGame::CGame(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	m_pTexture = NULL;        // �e�N�X�`���ւ̃|�C���^
	m_pScene2D = NULL;        // ������@�\���p�̃|���S��
	m_nStatementCounter = 0;  // ��ԊǗ��J�E���^�[
	m_nNumClearMap = 0;       // �N���A�����}�b�v��
	m_nMapClearBonus = 0;     // �N���A�{�[�i�X
	m_State = STATE_NORMAL;   // ���
	m_pMap = NULL;            // �}�b�v���ւ̃|�C���^
	m_bMapClear = false;      // �}�b�v���N���A�������ǂ���
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//    ��������
//=============================================================================
CGame *CGame::Create(void)
{
	CGame *pGame = NULL;  // �Q�[���N���X�^�̃|�C���^
	if (pGame == NULL)
	{// ���������m�ۂł����Ԃɂ���
		pGame = new CGame;
		if (pGame != NULL)
		{// ���������m�ۂł���
			// �������������s��
			pGame->Init();
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
	return pGame;
}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CGame::Init(void)
{
	FILE *pFile = NULL;
	char aString[256];
	char aPlayerFileName[256];
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
				if (strcmp(&aString[0], PLAYER_FILENAME) == 0)
				{// �v���C���[�̃X�N���v�g�t�@�C����������
					fscanf(pFile, "%s %s", &aString[0], &aPlayerFileName[0]);
				}
			}
		}
	}

	// �f�[�^�̓ǂݍ���
	// �����_�����O�N���X�^�̃|�C���^
	CRenderer *pRenderer = CManager::GetRenderer();

	if (pRenderer != NULL)
	{// �����_�����O�N���X����������Ă���
	 // �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
		if (pDevice != NULL)
		{// �f�o�C�X���擾�ł���
			D3DXCreateTextureFromFile(pDevice, GAME_TEXTURE_NAME, &m_pTexture);
		}
	}
	CPlayer::Load(aPlayerFileName); // �v���C���[
	CField::Load();                 // �t�B�[���h
	CWater::Load();                 // ��
	CShadow::Load();                // �e
	CCylinder::Load();              // �R
	CDome::Load();                  // ��
	CNumber::Load();                // ����
	CPause::Load();                 // �|�[�Y
	CLogoPause::Load();             // �|�[�Y���S
	CBalloon::Load();               // �����o��

	// �v���C���[�̐���
	CPlayer::Create(D3DXVECTOR3(-1000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), PLAYER_PRIORITY);

	// �^�C�}�[�̐���
	CTimer::Create();

	// �}�b�v�̐���
	if (m_pMap == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pMap = CMap::Create(aMapFileName);
	}

	// ������@�|���S���̐���
	if (m_pScene2D == NULL)
	{
		m_pScene2D = CScene2DSquare::Create(GAME_POLYGON_POS, GAME_POLYGON_COL, GAME_POLYGON_WIDTH, GAME_POLYGON_HEIGHT, 7);
		if (m_pScene2D != NULL)
		{
			m_pScene2D->BindTexture(m_pTexture);
		}
	}

	// �p�[�e�B�N���Ǘ��N���X�̐���
	CParticleManager::Create(PARTICLEDATA_FILENAME, PARTICLE_TEXTURE_FILENAME);

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGame::Uninit(void)
{
	// �f�[�^�̊J��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	CPlayer::UnLoad();             // �v���C���[
	CField::UnLoad();              // �t�B�[���h
	CWater::UnLoad();              // ��
	CShadow::UnLoad();             // �e
	CCylinder::UnLoad();           // �R
	CDome::UnLoad();               // ��
	CNumber::UnLoad();             // ����
	CPause::UnLoad();              // �|�[�Y
	CLogoPause::UnLoad();          // �|�[�Y���S
	CBalloon::UnLoad();            // �����o��

	// �}�b�v�̔j��
	if (m_pMap != NULL)
	{// ���������m�ۂ���Ă���
		m_pMap->Uninit();

		// �������̊J��
		delete m_pMap;
		m_pMap = NULL;
	}

	// ������@�|���S���̔j��
	if (m_pScene2D != NULL)
	{
		m_pScene2D->Uninit();
		m_pScene2D = NULL;
	}

	// ���U���g�ɕK�v�ȃX�R�A��n��
	CResult::SetNumClearStage(m_nNumClearMap);
	CResult::SetStageBonus(m_nMapClearBonus);

	// �Q�[���I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
//    �X�V����
//=============================================================================
void CGame::Update(void)
{
	switch (m_State)
	{// ��Ԃɂ���ď����킯
	case STATE_NORMAL:        // �ʏ���
		break;
	case STATE_MAPCHANGE:     // �}�b�v�؂�ւ����
		CDebugProc::Print("�}�b�v�؂�ւ���\n");
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			CManager::GetFade()->SetFade(CFade::TYPE_SWITCHMAP);
		}
		break;
	case STATE_CHALLENGE:     // �}�b�v������
		CDebugProc::Print("�}�b�v���풆\n");
		break;
	case STATE_PLAYERFALL:    // �v���C���[�����������
		CDebugProc::Print("�v���C���[���������`���b�^\n");
		PlayerFall();
		break;
	case STATE_MAPCLEAR:      // �}�b�v�N���A���
		CDebugProc::Print("�}�b�v�N���A�[\n");
		ClearMapEvent();
		break;
	case STATE_END:           // �I�����
		m_nStatementCounter++;
		if (m_nStatementCounter % GAME_MODE_CHANGE_TIMING)
		{// ��ʑJ�ڂ���^�C�~���O�ɂȂ���
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
				CManager::GetFade()->SetFade(CManager::MODE_RESULT);
			}
		}
		break;
	}
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
//    ��Ԃ��擾����
//=============================================================================
CGame::STATE CGame::GetState(void)
{
	return m_State;
}

//=============================================================================
//    �}�b�v���N���A�������ǂ������擾����
//=============================================================================
bool CGame::GetMapClear(void)
{
	return m_bMapClear;
}

//=============================================================================
//    ��Ԑݒ菈��
//=============================================================================
void CGame::SetState(STATE state)
{
	m_State = state;
	m_nStatementCounter = 0;  // ��ԊǗ��J�E���^�[��߂�
}

//=============================================================================
//    �}�b�v���N���A�������ǂ�����ݒ肷��
//=============================================================================
void CGame::SetMapClear(bool bMapClear)
{
	m_bMapClear = bMapClear;
}

//=============================================================================
//    �}�b�v�̃X�N���v�g�t�@�C�����ݒ菈��
//=============================================================================
void CGame::SetMapName(char *aMapFileName)
{
	strcpy(m_aMapFileName, aMapFileName);
}

//=============================================================================
//    �}�b�v��؂�ւ��鏈��
//=============================================================================
void CGame::SetMap(void)
{
	m_State = STATE_NORMAL;  // ��Ԃ�߂��Ă���

	// �}�b�v�̔j��
	if (m_pMap != NULL)
	{// ���������m�ۂ���Ă���
		m_pMap->Uninit();

		// �������̊J��
		delete m_pMap;
		m_pMap = NULL;
	}

	// �����o���̍폜
	// �����o���|���S���N���X�ւ̃|�C���^���擾����
	CBalloon *pBalloon = NULL;
	CScene *pScene = NULL;
	for (int nCntScene = 0; nCntScene < MAX_PRIORITY_NUM; nCntScene++)
	{// �����D�揇�ʂ̐������J��Ԃ�
		pScene = GetScene(nCntScene);   // �擪�A�h���X���m��
		if (pScene != NULL)
		{// �|�C���^���擾�ł���
			CScene *pSceneNext;  // ���̃|�C���^�ۑ��p
			while (pScene != NULL)
			{// ���̃|�C���^���Ȃ��Ȃ�܂Ń��[�v
				pSceneNext = pScene->GetNext();
				if (pScene->GetObjType() == CScene::OBJTYPE_BALLOON)
				{// �v���C���[�N���X�̃|�C���^������
					pBalloon = (CBalloon*)pScene;
					pBalloon->Uninit();
					pBalloon = NULL;
				}
				pScene = pSceneNext;
			}
		}
	}

	// �}�b�v�̐���
	if (m_pMap == NULL)
	{// ���������m�ۂł����Ԃɂ���
		m_pMap = CMap::Create(m_aMapFileName);
		if (m_bMapClear == true)
		{// �}�b�v�N���A������
			SetState(STATE_NORMAL);
		}
		else
		{// �U���}�b�v��������
			SetState(STATE_CHALLENGE);
		}
	}

	m_bMapClear = false;    // �}�b�v�𖢃N���A��Ԃɂ���
}

//=============================================================================
//    �}�b�v���N���A�����Ƃ��̏���
//=============================================================================
void CGame::ClearMapEvent(void)
{
	m_nStatementCounter++;
	if (m_nStatementCounter % GAME_MAPCLEAR_CHANGETIMING == 0)
	{// �}�b�v�؂�ւ��̃^�C�~���O�ɂȂ���
	    // �N���A�{�[�i�X�����Z
		m_nNumClearMap++;

		char *pMapName = m_pMap->GetMapName();
		if (strcmp(pMapName, "data/TEXT/MAP/map_stage1-1.txt") == 0)
		{// �X�e�[�W1��������
			m_nMapClearBonus += 3000;
		}
		else if (strcmp(pMapName, "data/TEXT/MAP/map_stage1-2.txt") == 0)
		{// �X�e�[�W2��������
			m_nMapClearBonus += 5000;
		}
		else if (strcmp(pMapName, "data/TEXT/MAP/map_stage1-3.txt") == 0)
		{// �X�e�[�W3��������
			m_nMapClearBonus += 15000;
		}
		else if (strcmp(pMapName, "data/TEXT/MAP/map_stage1-4.txt") == 0)
		{// �X�e�[�W4��������
			m_nMapClearBonus += 30000;
		}
		else if (strcmp(pMapName, "data/TEXT/MAP/map_stage1-5.txt") == 0)
		{// �X�e�[�W5��������
			m_nMapClearBonus += 50000;
		}

		SetMapName(GAME_MAPSELECT_FILENAME);
		SetState(CGame::STATE_MAPCHANGE);
	}
	// �������o���āA�t�F�[�h���ă}�b�v�I���ɖ߂�

}

//=============================================================================
//    �v���C���[�����ɗ��������̏���
//=============================================================================
void CGame::PlayerFall(void)
{
	m_nStatementCounter++;
	if (m_nStatementCounter % GAME_PLAYER_RESPAWNTIMING == 0)
	{// �v���C���[�����X�|�[��������^�C�~���O�ɂȂ���
		if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
		{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
			CManager::GetFade()->SetFade(CFade::TYPE_RESTART);
		}
	}
}

//=============================================================================
//    �v���C���[�����X�|�[�������鏈��
//=============================================================================
void CGame::PlayerRespawnEvent(void)
{
	if (m_pMap != NULL)
	{// ���������m�ۂ���Ă���
		// �v���C���[�N���X�ւ̃|�C���^���擾����
		CPlayer *pPlayer = NULL;
		CScene *pScene = CScene::GetScene(PLAYER_PRIORITY);
		if (pScene != NULL)
		{// �|�C���^���擾�ł���
			CScene *pSceneNext;  // ���̃|�C���^�ۑ��p
			while (pScene != NULL)
			{// ���̃|�C���^���Ȃ��Ȃ�܂Ń��[�v
				pSceneNext = pScene->GetNext();
				if (pScene->GetObjType() == CScene::OBJTYPE_PLAYER)
				{// �v���C���[�N���X�̃|�C���^������
					pPlayer = (CPlayer*)pScene;
					break;
				}
				pScene = pSceneNext;
			}
		}

		// �v���C���[�̈ʒu���f�t�H���g�ɖ߂�
		if (pPlayer != NULL)
		{// �|�C���^���擾�ł���
			pPlayer->SetDefault(m_pMap->GetDefaultPlayerPos(), m_pMap->GetDefaultPlayerRot());
		}

		// �J�����̌������f�t�H���g�ɖ߂�
		CCamera *pCamera = CManager::GetCamera();
		if (pCamera != NULL)
		{// �J�������擾�ł���
			pCamera->SetDefaultCamera(m_pMap->GetDefaultCameraRot());
		}

		// �}�b�v�̔z�u�����f�t�H���g�̈ʒu�ɖ߂�
		m_pMap->SetObjectDefault();
	}
}