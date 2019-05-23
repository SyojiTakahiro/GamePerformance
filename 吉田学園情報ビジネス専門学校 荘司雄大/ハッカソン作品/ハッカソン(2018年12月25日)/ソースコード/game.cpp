//*****************************************************************************
//
//     �Q�[���̏���[game.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"
#include "textureManager.h"
#include "pause.h"
#include "map.h"
#include "functionlib.h"
#include "player.h"
#include "enemy.h"
#include "effectManager.h"
#include "timer.h"
#include "score.h"
#include "scene2D.h"
#include "ranking.h"
#include "meshField.h"
#include "sky.h"
#include "mountain.h"
#include "object.h"
#include "ui.h"
#include "Billpresent.h"

#include "input.h"
#include "debuglog.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define GAME_SYSTEM_FILENAME      "data/TEXT/MODE/game.ini"              // �Q�[���̏��������X�N���v�g�t�@�C����
#define GAME_MODECHANGE_TIMING    (60)                                   // ��ʑJ�ڂ������ōs���^�C�~���O
#define GAME_PLAYERRESPAWN_TIMING (180)                                  // �v���C���[�����X�|�[��������^�C�~���O
#define GAME_PLAYER_STOCK         (3)                                    // �v���C���[�̎c�@��
#define GAME_MAPCLEAR_CHANGETIMING (120)                                 // �}�b�v��؂�ւ���^�C�~���O

// �l��ǂݎ��p�X��
#define NUM_TEXTURE            "NUM_TEXTURE = "            // �ǂݍ��ރe�N�X�`���̐�
#define TEXTURE_FILENAME       "TEXTURE_FILENAME = "       // �ǂݍ��ރe�N�X�`���̃t�@�C����
#define PLAYER_FILENAME        "PLAYER_FILENAME = "        // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
#define ENEMY_FILENAME         "ENEMY_FILENAME = "         // �G�l�~�[�f�[�^�̃X�N���v�g�t�@�C����
#define EFFECT_FILENAME        "EFFECT_FILENAME = "        // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����
#define MAP_FILENAME           "MAP_FILENAME = "           // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����
#define GAME_TIMER             "GAME_TIMER = "             // �^�C�}�[�̕b��

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************


//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CGame::CGame(int nPriority, OBJTYPE objType) : CScene(nPriority, objType)
{
	// �e��l�̃N���A
	m_pUi = NULL;
	m_pMap = NULL;             // �}�b�v�N���X�ւ̃|�C���^
	m_State = STATE_NORMAL;    // ���
	m_nStateCounter = 0;       // ��Ԃ��Ǘ�����J�E���^�[
	m_pTextureManager = NULL;  // �e�N�X�`���Ǌ��N���X�ւ̃|�C���^
	m_nNumStock = 0;           // �v���C���[�̎c�@��
	m_nNumPresent = 0;
	m_nNumStageClear = 0;      // ���݂̃N���A�����X�e�[�W��

	m_pMesh = NULL;                            // ���b�V�����ւ̃|�C���^
	m_pBuffMat = NULL;                         // �}�e���A�����ւ̃|�C���^
	m_nNumMat = 0;                             // �}�e���A�����̐�
	m_VtxMax = D3DXVECTOR3(0.0f,0.0f,0.0f);    // ���_���W�̍ő�l�ւ̃|�C���^
	m_VtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);  // ���_���W�̍ŏ��l�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CGame::~CGame()
{

}

//=============================================================================
//    �S�[���p�̃��f����ǂݍ��ޏ���
//=============================================================================
void CGame::LoadGoalModel(void)
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
	// �Q�[���̏����������O������ǂݍ���
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(GAME_SYSTEM_FILENAME, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                         // 1�s���ǂݎ��p
			char *pStrCur = NULL;                       // ���݂̐擪�|�C���^
			char *pStr = NULL;                          // �擪�|�C���^�ۑ��p
			int nNumTex = 0;                            // �e�N�X�`����ǂݍ��ސ�
			int nCntTex = 0;                            // �e�N�X�`����ǂݍ��񂾉�
			LPDIRECT3DTEXTURE9 pTexture = NULL;         // �e�N�X�`���ǂݎ��p
			char aTexFileName[256] = "\0";              // �e�N�X�`���̃t�@�C����
			char aPlayerFileName[256] = "\0";           // �v���C���[�f�[�^�̃X�N���v�g�t�@�C����
			char aEnemyFileName[256] = "\0";            // �G�l�~�[�f�[�^�̃X�N���v�g�t�@�C����
			char aEffectFileName[256] = "\0";           // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����
			char aMapFileName[256] = "\0";              // �}�b�v�f�[�^�̃X�N���v�g�t�@�C����
			int nTimer = 10 * 60;                       // �^�C�}�[�̕b��

			if (pLine == NULL && pStr == NULL)
			{// ���������m�ۂł����Ԃł���
				pLine = new char[256];
				pStr = new char[256];
				if (pLine != NULL && pStr != NULL)
				{// ���������m�ۂł���
					// �������������
					strcpy(pLine, "\0");
					strcpy(pStr, "\0");

					pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
					if (CFunctionLib::Memcmp(pStrCur, SCRIPT) == 0)
					{// �X�N���v�g�ǂݍ��݊J�n�̍��}������
						while (1)
						{// ���[�v�J�n
							pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
							if (CFunctionLib::Memcmp(pStrCur, NUM_TEXTURE) == 0)
							{// �e�N�X�`���̌���񂪂�����
								nNumTex = CFunctionLib::ReadInt(pStrCur, NUM_TEXTURE);
								if (nNumTex >= 1)
								{// ���f������1�ȏ゠��
									m_pTextureManager = CTextureManager::Create(nNumTex);
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, TEXTURE_FILENAME) == 0)
							{// �e�N�X�`���̃t�@�C���p�X����񂪂�����
								// �e�N�X�`���̃t�@�C���p�X����ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, TEXTURE_FILENAME);

								// �e�N�X�`���̓ǂݍ���
								D3DXCreateTextureFromFile(CManager::GetRenderer()->GetDevice(), pStr, &pTexture);
								if (pTexture != NULL)
								{// �e�N�X�`����ǂݍ��߂�
									m_pTextureManager->BindTexture(pTexture, nCntTex);
									m_pTextureManager->SetFileName(pStr, nCntTex);
									pTexture = NULL;
									nCntTex++;
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, PLAYER_FILENAME) == 0)
							{// �v���C���[�f�[�^�̃X�N���v�g�t�@�C����������
							    // �v���C���[�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, PLAYER_FILENAME);
								strcpy(aPlayerFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, ENEMY_FILENAME) == 0)
							{// �G�l�~�[�f�[�^�̃X�N���v�g�t�@�C����������
							    // �G�l�~�[�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, ENEMY_FILENAME);
								strcpy(aEnemyFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, EFFECT_FILENAME) == 0)
							{// �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C����������
							    // �G�t�F�N�g�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, EFFECT_FILENAME);
								strcpy(aEffectFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, MAP_FILENAME) == 0)
							{// �}�b�v�f�[�^�̃X�N���v�g�t�@�C����������
							    // �}�b�v�f�[�^�̃X�N���v�g�t�@�C������ǂݎ��
								pStr = CFunctionLib::ReadString(pStrCur, pStr, MAP_FILENAME);
								strcpy(aMapFileName, pStr);
							}
							else if (CFunctionLib::Memcmp(pStrCur, GAME_TIMER) == 0)
							{// �^�C�}�[�̕b��������
								nTimer = CFunctionLib::ReadInt(pStrCur, GAME_TIMER);
								nTimer *= 60;
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}


					// �K�v�ȃN���X���쐬����
					// UI
					m_pUi = CUI::Create();

					// �}�b�v
					m_pMap = CMap::Create(aMapFileName);

					// �G�t�F�N�g�Ǌ��N���X
					CEffectManager *pEffectManager = CEffectManager::Create(aEffectFileName);
					//pEffectManager->SetEffect(D3DXVECTOR3(-100.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CEffectManager::EFFECT_TYPE_FIRE);

					// �v���C���[
					CPlayerManager::Create(aPlayerFileName);

					// �G�l�~�[
					CEnemyManager::Create(aEnemyFileName);

					// �v���[���g
					CBillPresent *pBillPresent = NULL;
					pBillPresent = CBillPresent::Create(D3DXVECTOR3(-250.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f,5);
					pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));

					pBillPresent = CBillPresent::Create(D3DXVECTOR3(150.0f, 225.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f,5);
					pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));

					pBillPresent = CBillPresent::Create(D3DXVECTOR3(450.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f,5);
					pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));

					pBillPresent = CBillPresent::Create(D3DXVECTOR3(-350.0f, 525.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f,5);
					pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));

					pBillPresent = CBillPresent::Create(D3DXVECTOR3(350.0f, 525.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
					pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));

					// �������̊J��
					if (pStr != NULL)
					{// ���������m�ۂ���Ă���
						delete[] pStr;
						pStr = NULL;
					}
					if (pLine != NULL)
					{// ���������m�ۂ���Ă���
						delete[] pLine;
						pLine = NULL;
					}
				}
				else
				{// ���������m�ۂł��Ȃ�����
					return E_FAIL;
				}
			}
			else
			{// ���������m�ۂł����ԂłȂ�
				return E_FAIL;
			}
		}
		else
		{// �t�@�C���|�C���^���m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// �t�@�C���|�C���^���m�ۂł����ԂłȂ�
		return E_FAIL;
	}


	// �e��l�̐ݒ�
	m_nNumStock = GAME_PLAYER_STOCK;  // �v���C���[�̎c�@��
	m_nNumPresent = 5;

	// �S�[���p�̃��f����ǂݍ���
	D3DXLoadMeshFromX("data/MODEL/OBJECT/tonakai.x",
		D3DXMESH_SYSTEMMEM,
		CManager::GetRenderer()->GetDevice(),
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

	// �ŏ��l�ƍő�l��������
	m_VtxMin = D3DXVECTOR3(10000000.0f, 10000000.0f, 10000000.0f);
	m_VtxMax = D3DXVECTOR3(-10000000.0f, -10000000.0f, -10000000.0f);

	int nNumVtx;     // ���_��
	DWORD sizeFVF;   // ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;  // ���_�o�b�t�@�ւ̃|�C���^

	// ���_�����l��
	nNumVtx = m_pMesh->GetNumVertices();

	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{// ���ׂĂ̒��_���r���ă��f���̍ŏ��l,�ő�l�𔲂��o��
		D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;   // ���_���W�̑��

		if (Vtx.x < m_VtxMin.x)
		{// ���o����X���W�̍ŏ��l�����܂ł̂���������
			m_VtxMin.x = Vtx.x;  // �ŏ��l�X�V
		}
		if (Vtx.y < m_VtxMin.y)
		{// ���o����Y���W�̍ŏ��l�����܂ł̂���������
			m_VtxMin.y = Vtx.y;  // �ŏ��l�X�V
		}
		if (Vtx.z < m_VtxMin.z)
		{// ���o����Z���W�̍ŏ��l�����܂ł̂���������
			m_VtxMin.z = Vtx.z;  // �ŏ��l�X�V
		}
		if (Vtx.x > m_VtxMax.x)
		{// ���o����X���W�̍ő�l�����܂ł̂����傫��
			m_VtxMax.x = Vtx.x;  // �ő�l�X�V
		}
		if (Vtx.y > m_VtxMax.y)
		{// ���o����Y���W�̍ő�l�����܂ł̂����傫��
			m_VtxMax.y = Vtx.y;  // �ő�l�X�V
		}
		if (Vtx.z > m_VtxMax.z)
		{// ���o����Z���W�̍ő�l�����܂ł̂����傫��
			m_VtxMax.z = Vtx.z;  // �ő�l�X�V
		}

		pVtxBuff += sizeFVF;  // �T�C�Y���|�C���^��i�߂�
	}
	// ���_�o�b�t�@���A�����b�N
	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CGame::Uninit(void)
{
	// UI�N���X���J������
	if (m_pUi != NULL)
	{
		m_pUi->Uninit();

		delete m_pUi;
		m_pUi = NULL;
	}

	// �}�b�v�N���X���J������
	if (m_pMap != NULL)
	{
		m_pMap->Uninit();

		delete m_pMap;
		m_pMap = NULL;
	}

	// �e�N�X�`���Ǌ��N���X���J������
	if (m_pTextureManager != NULL)
	{
		m_pTextureManager->Uninit();

		// �������̊J��
		delete m_pTextureManager;
		m_pTextureManager = NULL;
	}

	// �S�[���p�̃��f�����J������
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

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
	case STATE_NORMAL:    // �ʏ���
		break;
	case STATE_MAPCHANGE:     // �}�b�v�؂�ւ����
		m_nStateCounter++;
		if (m_nStateCounter >= GAME_MAPCLEAR_CHANGETIMING)
		{// �J�E���^�[������̒l�ɒB����
			CDebugProc::Print("�}�b�v�؂�ւ���\n");
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
				CManager::GetFade()->SetFade(CFade::TYPE_SWITCHMAP);
			}
		}
		break;
	case STATE_PLAYER_DEATH:       // �v���C���[�����񂾏��
		m_nStateCounter++;
		if (m_nStateCounter >= GAME_PLAYERRESPAWN_TIMING)
		{// �J�E���^�[������̒l�ɒB����
			CPlayerManager *pPlayerManager = NULL;
			CScene *pScene = NULL;
			CScene *pSceneNext = NULL;
			for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
			{// �D�揇�ʂ̐������J��Ԃ�
				pScene = GetTop(nCntPriority);
				while (pScene != NULL)
				{// ����������ɂȂ�܂ŌJ��Ԃ�
					pSceneNext = pScene->GetNext();
					if (pScene->GetObjType() == OBJTYPE_PLAYERMANAGER)
					{// �v���C���[�Ǌ��N���X�ւ̃|�C���^������
						pPlayerManager = (CPlayerManager*)pScene;
						pPlayerManager->SetRespawn();
						m_State = STATE_NORMAL;
						m_nStateCounter = 0;
						m_nNumStock--;
					}
					pScene = pSceneNext;
				}
			}
		}
		break;
	case STATE_END:       // �I�����
		m_nStateCounter++;
		if (m_nStateCounter >= GAME_MODECHANGE_TIMING)
		{// �J�E���^�[������̒l�ɒB����
			if (CManager::GetFade()->GetState() == CFade::STATE_NONE)
			{// �t�F�[�h�̏�Ԃ��g�p����Ă��Ȃ���Ԃł���
				CManager::GetFade()->SetFade(CManager::MODE_RESULT);
				CRanking::SetRankingScore(m_pUi->GetScore());
			}
		}
		break;
	}

	CDebugProc::Print("�Q�[�����\n");
	CDebugProc::Print("\n");
	CDebugProc::Print("( *** ������@ *** )\n");
	CDebugProc::Print("( ++ �L�[�{�[�h ++ )\n");
	CDebugProc::Print("( �|�[�Y )                     : [ P ]\n");
	CDebugProc::Print("( �ړ� )                       : [ W ] [ A ] [ S ] [ D ]\n");
	CDebugProc::Print("( �J�������� )                 : [ �� ] [ �� ] [ �� ] [ �� ]\n");
	CDebugProc::Print("( �W�����v )                   : [ ENTER ]\n");
	CDebugProc::Print("( �A�N�V����(3�i�K�܂Ŕh��) )  : [ W ]\n");
	CDebugProc::Print("\n");

	CDebugProc::Print("( ++ �Q�[���p�b�h(ELECOM�А��̏ꍇ) ++ )\n");
	CDebugProc::Print("( �|�[�Y )                     : [ START�{�^�� ]\n");
	CDebugProc::Print("( �ړ� )                       : [ ���A�i���O�X�e�B�b�N ] [ �\���L�[ ]\n");
	CDebugProc::Print("( �J�������� )                 : [ �E�A�i���O�X�e�B�b�N ]\n");
	CDebugProc::Print("( �W�����v )                   : [ A�{�^�� ]\n");
	CDebugProc::Print("( �A�N�V����(3�i�K�܂Ŕh��) )  : [ B�{�^�� ]\n");
}

//=============================================================================
//    �`�揈��
//=============================================================================
void CGame::Draw(void)
{

}

//=============================================================================
//    �}�b�v���N���A�������ǂ�����ݒ肷��
//=============================================================================
void CGame::SetMapClear(bool bMapClear)
{
	m_bMapClear = bMapClear;
}

//=============================================================================
//    �}�b�v��؂�ւ��鏈��
//=============================================================================
void CGame::SetMap(void)
{
	m_nNumStageClear++;
	m_State = STATE_NORMAL;  // ��Ԃ�߂��Ă���
	m_nStateCounter = 0;

	// �}�b�v�̔j��
	if (m_pMap != NULL)
	{// ���������m�ۂ���Ă���
		m_pMap->Uninit();

		// �������̊J��
		delete m_pMap;
		m_pMap = NULL;
	}

	// �z�u�����̔j��
	CObject *pObject = NULL;
	CObjectMove *pObjectMove = NULL;
	CObjectGoal *pObjectGoal = NULL;
	CMeshField *pMeshField = NULL;
	CSky *pSky = NULL;
	CMountain *pMountain = NULL;
	CScene *pScene = NULL;
	CScene *pSceneNext = NULL;
	CEnemy *pEnemy = NULL;
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �D�揇�ʂ̐������J��Ԃ�
		pScene = GetTop(nCntPriority);
		while (pScene != NULL)
		{// ����������ɂȂ�܂ŌJ��Ԃ�
			pSceneNext = pScene->GetNext();
			if (pScene->GetObjType() == OBJTYPE_OBJECT)
			{// �z�u���ւ̃|�C���^������
				pObject = (CObject*)pScene;
				pObject->Uninit();
			}
			else if (pScene->GetObjType() == OBJTYPE_OBJECTMOVE)
			{// �����z�u���ւ̃|�C���^������
				pObjectMove = (CObjectMove*)pScene;
				pObjectMove->Uninit();
			}
			else if (pScene->GetObjType() == OBJTYPE_OBJECTGOAL)
			{// �S�[���p�z�u���ւ̃|�C���^������
				pObjectGoal = (CObjectGoal*)pScene;
				pObjectGoal->Uninit();
			}
			else if (pScene->GetObjType() == OBJTYPE_MESHFIELD)
			{// �n�ʂւ̃|�C���^������
				pMeshField = (CMeshField*)pScene;
				pMeshField->Uninit();
			}
			else if (pScene->GetObjType() == OBJTYPE_SKY)
			{// ��ւ̃|�C���^������
				pSky = (CSky*)pScene;
				pSky->Uninit();
			}
			else if (pScene->GetObjType() == OBJTYPE_MOUNTAIN)
			{// �R�ւ̃|�C���^������
				pMountain = (CMountain*)pScene;
				pMountain->Uninit();
			}
			else if (pScene->GetObjType() == OBJTYPE_ENEMY)
			{// �����z�u���ւ̃|�C���^������
				pEnemy = (CEnemy*)pScene;
				pEnemy->Uninit();
			}
			pScene = pSceneNext;
		}
	}

	CScene::DeathCheck();

	// �}�b�v�̐���
	if (m_pMap == NULL)
	{// ���������m�ۂł����Ԃɂ���
		if (m_nNumStageClear == 1)
		{
			m_pMap = CMap::Create("data/TEXT/MAP/map_game_1.txt");

			CEnemyManager::Create("data/TEXT/CHARACTER/enemy.txt");

			// �v���[���g���Ĕz�u����
			// �v���[���g
			CBillPresent *pBillPresent = NULL;
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-650.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(750.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-650.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(500.0f, 225.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-350.0f, 225.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-800.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(150.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-300.0f, 525.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(100.0f, 525.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(400.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));


			m_nNumPresent = 10;
			m_pUi->SetPresent(m_nNumPresent);
		}
		else if (m_nNumStageClear == 2)
		{
			m_pMap = CMap::Create("data/TEXT/MAP/map_game_2.txt");

			CBillPresent *pBillPresent = NULL;
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-900.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(800.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(400.0f, 225.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-700.0f, 225.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-800.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(800.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(700.0f, 525.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(-150.0f, 525.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(400.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));
			pBillPresent = CBillPresent::Create(D3DXVECTOR3(200.0f, 375.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, 30.0f, 5);
			pBillPresent->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_4));

			// �v���[���g���Ĕz�u����
			m_nNumPresent = 10;
			m_pUi->SetPresent(m_nNumPresent);
		}
	}

	// �^�C�}�[��߂��Ă���
	if (m_pUi != NULL)
	{
		m_pUi->SetTimer(120 * 60);
	}

	m_bMapClear = false;    // �}�b�v�𖢃N���A��Ԃɂ���
}

//=============================================================================
//    �|�[�Y�N���X�Ƀe�N�X�`����ݒ肷�鏈��
//=============================================================================
void CGame::BindPauseTexture(CPause *pPause)
{
	if (pPause != NULL)
	{// ���������m�ۂ���Ă���
		// �|�[�Y�w�i�p�̃e�N�X�`����ݒ肷��
		pPause->GetPauseBg()->BindTexture(m_pTextureManager->GetTexture(TEX_NUMBER_0));

		// �I�����ڗp�̃e�N�X�`����ݒ肷��
		int nCntScene2DFlash = 0;
		for (int nCntTex = TEX_NUMBER_1; nCntTex < TEX_NUMBER_1 + 3; nCntTex++, nCntScene2DFlash++)
		{// �e�N�X�`����ǂݍ��މ񐔌J��Ԃ�
			pPause->GetScene2DFlash(nCntScene2DFlash)->BindTexture(m_pTextureManager->GetTexture(nCntTex));
		}
	}
}

//=============================================================================
//    �v���[���g�̐������炷����
//=============================================================================
void CGame::CutPresentNum(void)
{
	m_nNumPresent--;
	if (m_pUi != NULL)
	{
		m_pUi->SetPresent(m_nNumPresent);
	}

	if (m_nNumPresent <= 0)
	{// �c��v���[���g�͂����Ȃ�
		CObjectGoal *pObjectGoal = NULL;
		if (m_nNumStageClear == 0)
		{
			pObjectGoal = CObjectGoal::Create(D3DXVECTOR3(450.0f, 75.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			if (pObjectGoal != NULL)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GOAL);
				pObjectGoal->BindModel(m_pMesh, m_pBuffMat, m_nNumMat, m_VtxMax, m_VtxMin, NULL);
			}
		}
		else if (m_nNumStageClear == 1)
		{
			pObjectGoal = CObjectGoal::Create(D3DXVECTOR3(900.0f, 75.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			if (pObjectGoal != NULL)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GOAL);
				pObjectGoal->BindModel(m_pMesh, m_pBuffMat, m_nNumMat, m_VtxMax, m_VtxMin, NULL);
			}
		}
		else if (m_nNumStageClear == 2)
		{
			pObjectGoal = CObjectGoal::Create(D3DXVECTOR3(950.0f, 225.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
			if (pObjectGoal != NULL)
			{
				CManager::GetSound()->PlaySound(CSound::SOUND_LABEL_SE_GOAL);
				pObjectGoal->BindModel(m_pMesh, m_pBuffMat, m_nNumMat, m_VtxMax, m_VtxMin, NULL);
			}
		}
	}
}

//=============================================================================
//    ��Ԃ�ݒ肷�鏈��
//=============================================================================
void CGame::SetState(STATE state)
{
	m_State = state;
	if (m_State == STATE_PLAYER_DEATH)
	{
		m_pUi->SetStock(m_pUi->GetStock() - 1);
	}
}

//=============================================================================
//    ��Ԃ��擾���鏈��
//=============================================================================
CGame::STATE CGame::GetState(void)
{
	return m_State;
}

//=============================================================================
//    ���݂̃X�e�[�W�N���A�����擾���鏈��
//=============================================================================
int CGame::GetNumStageClear(void)
{
	return m_nNumStageClear;
}

//=============================================================================
//    �v���C���[�̎c�@�����擾���鏈��
//=============================================================================
int CGame::GetNumStock(void)
{
	return m_nNumStock;
}

//=============================================================================
//    �X�R�A�𑝂₷����
//=============================================================================
void CGame::AddScore(void)
{
	if (m_pUi != NULL)
	{
		m_pUi->SetScore(m_pUi->GetScore() + 5000);
	}
}