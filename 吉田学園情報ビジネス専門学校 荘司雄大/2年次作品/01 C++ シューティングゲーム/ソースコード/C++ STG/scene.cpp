//=============================================================================
//
// �V�[������ [scene.cpp]
// Author : Takahiro Syoji 
//
//=============================================================================
#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "number.h"
#include "score.h"

//*****************************************************************************
// �ÓI�����o�ϐ�
//*****************************************************************************
CScene *CScene::m_apScene[MAX_PRI][MAX_SCENE] = {};
int CScene::m_nNumAll = 0;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CScene::CScene(int nPriority)
{
	for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
	{
		if (m_apScene[nPriority][nCntScene] == NULL)
		{
			m_apScene[nPriority][nCntScene] = this;
			m_nID = nCntScene;
			m_nPriority = nPriority;
			m_objType = OBJTYPE_NONE;
			m_nNumAll++;
			break;
		}
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
// �S�Ă̔j��
//=============================================================================
void CScene::ReleaseAll(void)
{
	for (int nCntPriority = 0; nCntPriority < MAX_PRI; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				m_apScene[nCntPriority][nCntScene]->Uninit();

				//�������̊J��
				delete m_apScene[nCntPriority][nCntScene];
				m_apScene[nCntPriority][nCntScene] = NULL;
			}
		}
	}
}
//10�� delete this

//=============================================================================
// �S�Ă̍X�V����
//=============================================================================
void CScene::UpdateAll(void)
{
	for (int nCntPriority = 0; nCntPriority < MAX_PRI; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				m_apScene[nCntPriority][nCntScene]->Update();
			}
		}
	}
}

//=============================================================================
// �S�Ă̕`�揈��
//=============================================================================
void CScene::DrawAll(void)
{
	for (int nCntPriority = 0; nCntPriority < MAX_PRI; nCntPriority++)
	{
		for (int nCntScene = 0; nCntScene < MAX_SCENE; nCntScene++)
		{
			if (m_apScene[nCntPriority][nCntScene] != NULL)
			{
				m_apScene[nCntPriority][nCntScene]->Draw();
			}
		}
	}
}

//=============================================================================
// �I�u�W�F�N�g�̔j��
//=============================================================================
void CScene::Release(void)
{
	if (m_apScene[m_nPriority][m_nID] != NULL)
	{
		int nID;
		nID = m_nID;	//m_nID��delete����NULL�ɂ���ƕs��ɂȂ��Ă��܂��B(�V�[���̒��g���Ȃ��Ȃ��Ă��܂�)
		int nPriority;
		nPriority = m_nPriority;

		//�������̊J��
		delete m_apScene[nPriority][nID];
		m_apScene[nPriority][nID] = NULL;
		m_nNumAll--;
	}
}

void CScene::SetObjType(CScene::OBJTYPE objType)
{
	m_objType = objType;
}

CScene *CScene::GetScene(int nPriority, int nIdx)
{
	return m_apScene[nPriority][nIdx];
}

CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_objType;
}