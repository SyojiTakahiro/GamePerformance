//*****************************************************************************
//
//     �T�E���h�̏���[sound.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     �T�E���h�N���X�̒�`
//*****************************************************************************
class CSound
{
public:    // �N�ł��A�N�Z�X�\
	//------------------------
	//  �T�E���h���x��
	//------------------------
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,   // �^�C�g����BGM
		SOUND_LABEL_BGM001,       // �`���[�g���A����BGM
		SOUND_LABEL_BGM002,       // �Q�[����BGM
		SOUND_LABEL_BGM003,       // ���U���g��BGM
		SOUND_LABEL_BGM004,       // �����L���O��BGM
		SOUND_LABEL_SE_PRESENT,   // �v���[���g�ɐG�ꂽ���̉�
		SOUND_LABEL_SE_PLAYER_DEATH,   // �v���C���[�����񂾂Ƃ��̉�
		SOUND_LABEL_SE_GOAL,   // �S�[�������ꂽ�̉�
		SOUND_LABEL_SE_BREAK,   // �����󂵂����̉�
		SOUND_LABEL_SE_GOALHIT,   // �S�[���ɐG�ꂽ���̉�
		SOUND_LABEL_SE_SELECT,
		SOUND_LABEL_SE_PAUSE_OPEN,
		SOUND_LABEL_SE_PRESS,
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);

private:   // ���̃N���X�������A�N�Z�X�\
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								    // XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice *m_pMasteringVoice;			    // �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y

	//------------------------
	//  �e���f�ނ̃p�����[�^
	//------------------------
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	} SOUNDPARAM;
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =    // ���f�ނ̃p�����[�^(�T�E���h���x�����p��)
	{// -1�Ȃ�BGM  0�Ȃ�SE
		{ "data/BGM/bgm000.wav",-1 },
		{ "data/BGM/bgm001.wav",-1 },
		{ "data/BGM/bgm002.wav",-1 },
		{ "data/BGM/bgm003.wav",-1 },
		{ "data/BGM/bgm004.wav",-1 },
		{ "data/SE/SE_Item.wav",0 },
		{ "data/SE/SE_SINU.wav",0 },
		{ "data/SE/SE_Goal.wav",0 },
		{ "data/SE/SE_Break.wav",0 },
		{ "data/SE/SE_SORI.wav",0 },
		{ "data/SE/SE_SelectMove.wav",0 },
		{ "data/SE/�|�[�Y.wav",0 },
		{ "data/SE/SE_����.wav",0 },
	};
};

#endif