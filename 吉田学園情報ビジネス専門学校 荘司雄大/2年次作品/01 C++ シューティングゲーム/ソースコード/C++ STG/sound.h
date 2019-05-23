//=============================================================================
//
// サウンド処理 [sound.h]
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
	// サウンドファイル
	//*************************************
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,		// BGM0
		SOUND_LABEL_BGM001,			// BGM1
		SOUND_LABEL_BGM002,			// BGM2
		SOUND_LABEL_BGM003,			// BGM2
		SOUND_LABEL_BGM004,			// BGM2
		SOUND_LABEL_SE_SHOT,		// 弾発射音
		SOUND_LABEL_SE_HIT000,		// ヒット音0
		SOUND_LABEL_SE_EXPLOSION,	// 爆発音
		SOUND_LABEL_SE_DECIDE,		// 決定音
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
	// パラメータ構造体定義
	//*****************************************************************************
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;

	// 各音素材のパラメータ
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =
	{
		{ "data/BGM/TITLE.wav", -1 },		// BGM0
		{ "data/BGM/TUTORIAL.wav", -1 },		// BGM1
		{ "data/BGM/GAME.wav", -1 },		// BGM2
		{ "data/BGM/GAMECLEAR.wav", -1 },		// BGM3
		{ "data/BGM/GAMEOVER.wav", -1 },		// BGM4
		{ "data/SE/shot000.wav", 0 },			// 弾発射音
		{ "data/SE/hit000.wav", 0 },			// ヒット音0
		{ "data/SE/explosion000.wav", 0 },	// 爆発音
		{ "data/SE/decide000.wav", 0 },		// 選択音
		{ "data/SE/select000.wav", 0 },		// SELECT音
		{ "data/SE/cancel000.wav", 0 },		// cancel音
		{ "data/SE/pause000.wav", 0 },		// ポーズ音
		{ "data/SE/SLASH.wav", 0 },		// 選択音
		{ "data/SE/SP.wav", -1 },		// 選択音
		{ "data/SE/HEAL.wav", 0 },		// 選択音
		{ "data/SE/GAUGE_MAX.wav", 0 },		// 選択音
		{ "data/SE/BOSS_DOWN.wav", 0 }		// 選択音
	};

	IXAudio2 *m_pXAudio2 ;								// XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice;			// マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ
																
};

#endif

