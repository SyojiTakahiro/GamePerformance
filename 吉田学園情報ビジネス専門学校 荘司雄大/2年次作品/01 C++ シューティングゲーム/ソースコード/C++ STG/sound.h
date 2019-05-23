//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : Takahiro Syoji
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

class CSound
{
public:
	//*************************************
	// �T�E���h�t�@�C��
	//*************************************
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,		// BGM0
		SOUND_LABEL_BGM001,			// BGM1
		SOUND_LABEL_BGM002,			// BGM2
		SOUND_LABEL_BGM003,			// BGM2
		SOUND_LABEL_BGM004,			// BGM2
		SOUND_LABEL_SE_SHOT,		// �e���ˉ�
		SOUND_LABEL_SE_HIT000,		// �q�b�g��0
		SOUND_LABEL_SE_EXPLOSION,	// ������
		SOUND_LABEL_SE_DECIDE,		// ���艹
		SOUND_LABEL_SE_SELECT,
		SOUND_LABEL_SE_CANCEL,
		SOUND_LABEL_SE_PAUSE,
		SOUND_LABEL_SE_SLASH,
		SOUND_LABEL_SE_SPSHOT,
		SOUND_LABEL_SE_HEAL,
		SOUND_LABEL_SE_GAUGE_MAX,
		SOUND_LABEL_SE_BOSS_DOWN,
		SOUND_LABEL_MAX,
	} SOUND_LABEL;

	CSound();
	~CSound();
	HRESULT Init(HWND hWnd);
	void Uninit(void);
	HRESULT PlaySound(SOUND_LABEL label);
	void StopSound(SOUND_LABEL label);
	void StopSound(void);
private:
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	//*****************************************************************************
	// �p�����[�^�\���̒�`
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// �t�@�C����
		int nCntLoop;		// ���[�v�J�E���g(-1�Ń��[�v�Đ�)
	} SOUNDPARAM;

	// �e���f�ނ̃p�����[�^
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/TITLE.wav", -1 },		// BGM0
		{ "data/BGM/TUTORIAL.wav", -1 },		// BGM1
		{ "data/BGM/GAME.wav", -1 },		// BGM2
		{ "data/BGM/GAMECLEAR.wav", -1 },		// BGM3
		{ "data/BGM/GAMEOVER.wav", -1 },		// BGM4
		{ "data/SE/shot000.wav", 0 },			// �e���ˉ�
		{ "data/SE/hit000.wav", 0 },			// �q�b�g��0
		{ "data/SE/explosion000.wav", 0 },	// ������
		{ "data/SE/decide000.wav", 0 },		// �I����
		{ "data/SE/select000.wav", 0 },		// SELECT��
		{ "data/SE/cancel000.wav", 0 },		// cancel��
		{ "data/SE/pause000.wav", 0 },		// �|�[�Y��
		{ "data/SE/SLASH.wav", 0 },		// �I����
		{ "data/SE/SP.wav", -1 },		// �I����
		{ "data/SE/HEAL.wav", 0 },		// �I����
		{ "data/SE/GAUGE_MAX.wav", 0 },		// �I����
		{ "data/SE/BOSS_DOWN.wav", 0 }		// �I����
	};

	IXAudio2 *m_pXAudio2 ;								// XAudio2�I�u�W�F�N�g�ւ̃|�C���^
	IXAudio2MasteringVoice *m_pMasteringVoice;			// �}�X�^�[�{�C�X�ւ̃|�C���^
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X�ւ̃|�C���^
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y
																
};

#endif

