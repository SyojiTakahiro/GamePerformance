//*****************************************************************************
//
//     �V�[���̏���[scene.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "scene.h"
#include "debuglog.h"

//*****************************************************************************
//    �ÓI�����o�ϐ��錾
//*****************************************************************************
int     CScene::m_nNumAll;                      // �V�[���̃C���X�^���X������
CScene *CScene::m_apTop[MAX_PRIORITY_NUM] = {};  // �擪�̃V�[���N���X�ւ̃|�C���^(�J�n�ʒu���킩��Ȃ��Ȃ�̂ŕۑ�����)
CScene *CScene::m_apCur[MAX_PRIORITY_NUM] = {};  // ���݂̍Ō���V�[���N���X�ւ̃|�C���^(�V�����V�[���N���X�ւ̃|�C���^��ǉ����邽�߂ɕۑ�)

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CScene::CScene(int nPriority, OBJTYPE objType)
{
	// �e��l�̃N���A
	m_pPrev = NULL;            // ���g�̑O�̃A�h���X
	m_pNext = NULL;            // ���g�̎��̃A�h���X
	m_bDeath = false;          // ���S�t���O
	m_ObjType = objType;       // �I�u�W�F�N�g�̎��
	m_nPriority = nPriority;   // �`��D�揇��

	if (m_apTop[nPriority] == NULL)
	{// �擪�A�h���X����ł���
		m_apTop[nPriority] = this; // ���g�̃|�C���^����
	}
	CScene *pCurOld = m_apCur[nPriority];  // ���݂̍Ō����ۑ�(���Ƃŋ�����̂Ɏg��)
	m_apCur[nPriority] = this;             // ���݂̍Ō�����X�V
	m_apCur[nPriority]->m_pPrev = pCurOld; // ���g�̑O�̃A�h���X�ɑO��̍Ō����ۑ�����
	if (pCurOld != NULL)
	{// �O��̍Ō���A�h���X������
		pCurOld->m_pNext = m_apCur[nPriority]; // �O��̍Ō���A�h���X�Ɏ��̃A�h���X�����g�ł��邱�Ƃ�`����
	}

	m_nNumAll++;  // �C���X�^���X�������𑝂₷
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CScene::~CScene()
{

}

//=============================================================================
//    �V�[�������ׂčX�V����
//=============================================================================
void CScene::UpdateAll(void)
{
	int nCountPriority[MAX_PRIORITY_NUM] = { 0,0,0,0,0,0,0,0 };   // �X�V�������s�����I�u�W�F�N�g�̐�(�D�揇�ʂ̐������p��)

	CScene *pScene;  // �X�V�����̊J�n�|�C���^��錾
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = m_apTop[nCntPriority];  // �X�V�����̊J�n�|�C���^����
		while (pScene != NULL)
		{// ����������ɂȂ�܂Ń��[�v
			CScene *pSceneNext = pScene->m_pNext; // ���g�̎��̃A�h���X��ۑ�(�X�V�����Ń|�C���^����ɂȂ�\��������̂�)
			pScene->Update();                     // �X�V����
			pScene = pSceneNext;                  // ���̍X�V�����͎��g�̎��̃A�h���X�̍X�V�������s��
			nCountPriority[nCntPriority]++;
		}
	}

	// ���S�t���O�`�F�b�N
	DeathCheck();
}
//=============================================================================
//    �V�[�������ׂĕ`�悷��
//=============================================================================
void CScene::DrawAll(void)
{
	CScene *pScene;  // �`�揈���̊J�n�|�C���^��錾
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = m_apTop[nCntPriority];  // �`�揈���̊J�n�|�C���^����
		while (pScene != NULL)
		{// ����������ɂȂ�܂Ń��[�v
			CScene *pSceneNext = pScene->m_pNext; // ���g�̎��̃A�h���X��ۑ�(�`�揈���Ŏ��g�̃|�C���^����ɂȂ�\��������̂�)
			pScene->Draw();                       // �`�揈��
			pScene = pSceneNext;                  // ���̕`�揈���̎��g�̎��̃A�h���X�̕`�揈�����s��
		}
	}
}
//=============================================================================
//    �V�[����S�ĊJ�����鏈��
//=============================================================================
void CScene::ReleaseAll(void)
{
	CScene *pScene;  // �I�������̊J�n�|�C���^��錾
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = m_apTop[nCntPriority];  // �I�������̊J�n�|�C���^����
		while (pScene != NULL)
		{// ����������ɂȂ�܂Ń��[�v
			CScene *pSceneNext = pScene->m_pNext; // ���g�̎��̃A�h���X��ۑ�(�I�������Ŏ��g�̃|�C���^����ɂȂ�\��������̂�)
			pScene->Uninit();                     // �I������
			pScene = pSceneNext;                  // ���̏I�������̎��g�̎��̃A�h���X�̏I���������s��
		}
	}
	// ���S�t���O�`�F�b�N
	DeathCheck();

	// �����������������Ă���
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		m_apTop[nCntPriority] = NULL;   // �擪�A�h���X
		m_apCur[nCntPriority] = NULL;   // ���݂̍Ō���A�h���X
	}
	m_nNumAll = 0;   // �C���X�^���X��������߂�
}

//=============================================================================
//    ���S�t���O�𗧂Ă鏈��
//=============================================================================
void CScene::Release(void)
{
	if (m_bDeath == false)
	{// ���S�t���O�������Ă��Ȃ�
		m_bDeath = true;  // ���S�t���O�𗧂Ă�
	}
	else
	{// ���S�t���O�����łɗ����Ă���
		if (m_pPrev != NULL)
		{// ���g�̑O�̃A�h���X����ł͂Ȃ�
			m_pPrev->m_pNext = m_pNext;
		}
		if (m_pNext != NULL)
		{// ���g�̎��̃A�h���X����ł͂Ȃ�
			m_pNext->m_pPrev = m_pPrev;
		}
		if (this == m_apTop[m_nPriority])
		{// ���g�̃A�h���X���擪�A�h���X������
			m_apTop[m_nPriority] = m_pNext;
		}
		if (this == m_apCur[m_nPriority])
		{// ���g�̃A�h���X�����݂̍Ō���A�h���X������
			m_apCur[m_nPriority] = m_pPrev;
		}
		// �������̊J��
		delete this;
	}

	m_nNumAll--;  // �C���X�^���X�����������炷
}

//=============================================================================
//    ���S�t���O���m�F���鏈��
//=============================================================================
void CScene::DeathCheck(void)
{
	CScene *pScene;                  // �`�F�b�N�J�n�|�C���^
	for (int nCntPriority = 0; nCntPriority < MAX_PRIORITY_NUM; nCntPriority++)
	{// �`��D�揇�ʂ̐������J��Ԃ�
		pScene = m_apTop[nCntPriority];           // �`�F�b�N�J�n�|�C���^��擪�ɂ���
		while (pScene != NULL)
		{// ����������ɂȂ�܂Ń��[�v
			CScene *pSceneNext = pScene->m_pNext; // ���g�̎��̃A�h���X��ۑ�
			if (pScene->m_bDeath == true)
			{// ���S�t���O�������Ă���
				pScene->Release();                // ���X�g����폜����
			}
			pScene = pSceneNext;                  // ���̎��S�t���O�`�F�b�N�͎��g�̎��̃A�h���X�̃`�F�b�N���s��
		}
	}
}

//=============================================================================
//    �V�[���𐶐����������擾
//=============================================================================
int CScene::GetNumAll(void)
{
	return m_nNumAll;
}

//=============================================================================
//    �擪�A�h���X���擾
//=============================================================================
CScene *CScene::GetTop(int nPriority)
{
	return m_apTop[nPriority];
}

//=============================================================================
//    �I�u�W�F�N�g�̎�ނ�ݒ�
//=============================================================================
void CScene::SetObjType(OBJTYPE ObjType)
{
	m_ObjType = ObjType;
}

//=============================================================================
//    �I�u�W�F�N�g�̎�ނ��擾
//=============================================================================
CScene::OBJTYPE CScene::GetObjType(void)
{
	return m_ObjType;
}

//=============================================================================
//    ���g�̎��̃A�h���X���擾
//=============================================================================
CScene *CScene::GetNext(void)
{
	return m_pNext;
}

//=============================================================================
//    ���g�̑O�̃A�h���X���擾
//=============================================================================
CScene *CScene::GetPrev(void)
{
	return m_pPrev;
}

//=============================================================================
//    ���g�̏����D�揇�ʂ��擾
//=============================================================================
int CScene::GetPriority(void)
{
	return m_nPriority;
}