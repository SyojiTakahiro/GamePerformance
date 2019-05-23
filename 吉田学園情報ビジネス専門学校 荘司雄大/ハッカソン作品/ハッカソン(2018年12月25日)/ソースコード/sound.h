//*****************************************************************************
//
//     サウンドの処理[sound.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SOUND_H_
#define _SOUND_H_

//*****************************************************************************
//     インクルードファイル
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     サウンドクラスの定義
//*****************************************************************************
class CSound
{
public:    // 誰でもアクセス可能
	//------------------------
	//  サウンドラベル
	//------------------------
	typedef enum
	{
		SOUND_LABEL_BGM000 = 0,   // タイトルのBGM
		SOUND_LABEL_BGM001,       // チュートリアルのBGM
		SOUND_LABEL_BGM002,       // ゲームのBGM
		SOUND_LABEL_BGM003,       // リザルトのBGM
		SOUND_LABEL_BGM004,       // ランキングのBGM
		SOUND_LABEL_SE_PRESENT,   // プレゼントに触れた時の音
		SOUND_LABEL_SE_PLAYER_DEATH,   // プレイヤーが死んだときの音
		SOUND_LABEL_SE_GOAL,   // ゴールが現れたの音
		SOUND_LABEL_SE_BREAK,   // 床を壊した時の音
		SOUND_LABEL_SE_GOALHIT,   // ゴールに触れた時の音
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

private:   // このクラスだけがアクセス可能
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	IXAudio2 *m_pXAudio2;								    // XAudio2オブジェクトへのポインタ
	IXAudio2MasteringVoice *m_pMasteringVoice;			    // マスターボイスへのポインタ
	IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイスへのポインタ
	BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータへのポインタ
	DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// オーディオデータサイズ

	//------------------------
	//  各音素材のパラメータ
	//------------------------
	typedef struct
	{
		char *pFilename;	// ファイル名
		int nCntLoop;		// ループカウント(-1でループ再生)
	} SOUNDPARAM;
	SOUNDPARAM m_aSoundParam[SOUND_LABEL_MAX] =    // 音素材のパラメータ(サウンドラベル分用意)
	{// -1ならBGM  0ならSE
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
		{ "data/SE/ポーズ.wav",0 },
		{ "data/SE/SE_決定.wav",0 },
	};
};

#endif