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
		SOUND_LABEL_BGM000 = 0,     // �^�C�g����BGM
		SOUND_LABEL_BGM001,         // �`���[�g���A����BGM
		SOUND_LABEL_BGM002,         // �Q�[����BGM
		SOUND_LABEL_BGM003,         // ���U���g��BGM
		SOUND_LABEL_BGM004,         // �����L���O��BGM
		SOUND_LABEL_SE_WALK000,     // ������(1��)
		SOUND_LABEL_SE_WALK001,     // ������(2��)
		SOUND_LABEL_SE_JUMP,        // �W�����v���鉹
		SOUND_LABEL_SE_LANDING,     // ���n���鉹
		SOUND_LABEL_SE_PUSH,        // ������
		SOUND_LABEL_SE_PANCH,       // �p���`���鉹
		SOUND_LABEL_SE_KICK,        // �L�b�N���鉹
		SOUND_LABEL_SE_ZIPLINE,     // �W�b�v���C���̉�
		SOUND_LABEL_SE_CARRYFINISH, // �W�b�v���C���ŉ^�яI�������̉�
		SOUND_LABEL_SE_BREAK,       // ���̂��󂷉�
		SOUND_LABEL_SE_WATER,       // ���ɓ��������̉�
		SOUND_LABEL_SE_PRESS,       // ����{�^���������ꂽ�Ƃ�
		SOUND_LABEL_SE_SELECT,      // �I�����ڂ��ړ������Ƃ�
		SOUND_LABEL_SE_PAUSE_OPEN,  // �|�[�Y���J�����Ƃ��̉�
		SOUND_LABEL_SE_FINISH,      // �Q�[���I�����̉�
		SOUND_LABEL_SE_WARNING,     // �c�莞�Ԃ������Ȃ������̉�
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
		{ "data/SE/walk000.wav",0 },
		{ "data/SE/walk001.wav",0 },
		{ "data/SE/jump.wav",0 },
		{ "data/SE/landing.wav",0 },
		{ "data/SE/push.wav",0 },
		{ "data/SE/panch.wav",0 },
		{ "data/SE/kick.wav",0 },
		{ "data/SE/zipline.wav",-1 },
		{ "data/SE/zipline_end.wav",0 },
		{ "data/SE/break.wav",0 },
		{ "data/SE/water.wav",0 },
		{ "data/SE/Gorira.wav",0 },
		{ "data/SE/pauseselect.wav",0 },
		{ "data/SE/pauseopen.wav",0 },
		{ "data/SE/finish.wav",0 },
		{ "data/SE/warning.wav",0 },
	};
};

#endif