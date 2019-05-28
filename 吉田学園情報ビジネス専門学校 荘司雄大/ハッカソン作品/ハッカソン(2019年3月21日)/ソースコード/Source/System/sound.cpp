//*****************************************************************************
//
//     �T�E���h�̏���[sound.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "system.h"
#include "functionlib.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define SOUND_DATA_FILENAME "data/TEXT/SOUND/sound.txt" // �T�E���h�̃X�N���v�g�t�@�C����

// �f�[�^�ǂݎ��p�̃p�X
#define NUM_SOUND    "NUM_SOUND = "
#define SOUNDSET     "SOUNDSET"
#define FILENAME     "FILENAME = "
#define LOOP         "LOOP = "
#define END_SOUNDSET "END_SOUNDSET"

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CSound::CSound()
{
	// �l�̃N���A
	m_pXAudio2 = NULL;          // XAudio2�I�u�W�F�N�g�ւ̃|�C���^
    m_pMasteringVoice = NULL;   // �}�X�^�[�{�C�X�ւ̃|�C���^
	m_apSourceVoice = NULL;  	// �\�[�X�{�C�X�ւ̃|�C���^
	m_apDataAudio = NULL;	    // �I�[�f�B�I�f�[�^�ւ̃|�C���^
	m_apSizeAudio = NULL;		// �I�[�f�B�I�f�[�^�T�C�Y�ւ̃|�C���^
	m_nNumSound = 0;            // �T�E���h�f�[�^�̐�
	m_apSoundParam = NULL;      // ���f�ނ̃p�����[�^�ւ̃|�C���^
}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CSound::~CSound()
{

}

//=============================================================================
//    ����������
//=============================================================================
HRESULT CSound::Init(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();
		return E_FAIL;
	}

	// �}�X�^�[�{�C�X�̐���
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if (m_pXAudio2)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�����t�@�C������ǂݎ��
	FILE *pFile = NULL;   // �t�@�C���|�C���^
	if (pFile == NULL)
	{// �t�@�C���|�C���^���m�ۂł����Ԃł���
		pFile = fopen(SOUND_DATA_FILENAME, "r");
		if (pFile != NULL)
		{// �t�@�C���|�C���^���m�ۂł���
		    // �X�N���v�g�ǂݍ��ݗp�̕ϐ���p��
			char *pLine = NULL;                         // 1�s���ǂݎ��p
			char *pStrCur = NULL;                       // ���݂̐擪�|�C���^
			char *pStr = NULL;                          // �擪�|�C���^�ۑ��p
			int nCntLoop = 0;                           // ���[�v�J�E���g
			int nNumSound = 0;                          // �ǂݍ��񂾃T�E���h���𐔂���
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
							if (CFunctionLib::Memcmp(pStrCur, NUM_SOUND) == 0)
							{// �T�E���h�f�[�^�̐�������
								m_nNumSound = CFunctionLib::ReadInt(pStrCur, NUM_SOUND);
								if (m_nNumSound >= 1)
								{// �T�E���h�f�[�^�̐���1�ł�����
									CreatePointer();
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, SOUNDSET) == 0)
							{// �����ǂݍ��݊J�n�̍��}������
								while (1)
								{// ���[�v�J�n
									pStrCur = CFunctionLib::ReadLine(pFile, pLine);  // �L���ȕ������������
									if (CFunctionLib::Memcmp(pStrCur, FILENAME) == 0)
									{// �t�@�C����������
										pStr = CFunctionLib::ReadString(pStrCur, pStr, FILENAME);
										strcpy(m_apSoundParam[nNumSound].pFilename, pStr);
									}
									else if (CFunctionLib::Memcmp(pStrCur, LOOP) == 0)
									{// ���[�v�J�E���g��񂾂���
										m_apSoundParam[nNumSound].nCntLoop = CFunctionLib::ReadInt(pStrCur, LOOP);
									}
									else if (CFunctionLib::Memcmp(pStrCur, END_SOUNDSET) == 0)
									{// �����ǂݍ��ݏI���̍��}������
										nNumSound++;
										break;  // ���[�v�I��
									}
								}
							}
							else if (CFunctionLib::Memcmp(pStrCur, END_SCRIPT) == 0)
							{// �X�N���v�g�ǂݍ��ݏI���̍��}������
								break;  // ���[�v�I��
							}
						}
					}

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
			fclose(pFile);
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

	// �T�E���h�f�[�^�̏�����
	if (m_apDataAudio != NULL && m_apSizeAudio != NULL && m_apSoundParam != NULL && m_apSourceVoice != NULL)
	{// ���������m�ۂł��Ă���
		for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
		{// �T�E���h�f�[�^�̐������J��Ԃ�
			HANDLE hFile;
			DWORD dwChunkSize = 0;
			DWORD dwChunkPosition = 0;
			DWORD dwFiletype;
			WAVEFORMATEXTENSIBLE wfx;
			XAUDIO2_BUFFER buffer;

			// �o�b�t�@�̃N���A
			memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
			memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

			// �T�E���h�f�[�^�t�@�C���̐���
			hFile = CreateFile(m_apSoundParam[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
			{
				MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
				return HRESULT_FROM_WIN32(GetLastError());
			}
			if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^��擪�Ɉړ�
				MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
				return HRESULT_FROM_WIN32(GetLastError());
			}

			// WAVE�t�@�C���̃`�F�b�N
			hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}
			hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}
			if (dwFiletype != 'EVAW')
			{
				MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}

			// �t�H�[�}�b�g�`�F�b�N
			hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}
			hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}

			// �I�[�f�B�I�f�[�^�ǂݍ���
			hr = CheckChunk(hFile, 'atad', &m_apSizeAudio[nCntSound], &dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}
			m_apDataAudio[nCntSound] = (BYTE*)malloc(m_apSizeAudio[nCntSound]);
			hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_apSizeAudio[nCntSound], dwChunkPosition);
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}

			// �\�[�X�{�C�X�̐���
			hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound], &(wfx.Format));
			if (FAILED(hr))
			{
				MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);
				return S_FALSE;
			}

			memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
			buffer.AudioBytes = m_apSizeAudio[nCntSound];
			buffer.pAudioData = m_apDataAudio[nCntSound];
			buffer.Flags = XAUDIO2_END_OF_STREAM;
			buffer.LoopCount = m_apSoundParam[nCntSound].nCntLoop;

			// �I�[�f�B�I�o�b�t�@�̓o�^
			m_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);
		}
	}

	return S_OK;
}

//=============================================================================
//    �I������
//=============================================================================
void CSound::Uninit(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
	{// �T�E���h�f�[�^�̐������J��Ԃ�
		if (m_apSourceVoice[nCntSound])
		{// ����������������Ă���
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);

			// �\�[�X�{�C�X�̔j��
			m_apSourceVoice[nCntSound]->DestroyVoice();
			m_apSourceVoice[nCntSound] = NULL;

			// �I�[�f�B�I�f�[�^�̊J��
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = NULL;
		}
	}

	// ���f�ރp�����[�^�̊J��
	for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
	{// �T�E���h�f�[�^�̐������J��Ԃ�
		free(m_apSoundParam[nCntSound].pFilename);
		m_apSoundParam[nCntSound].pFilename = NULL;
	}
	free(m_apSoundParam);
	m_apSoundParam = NULL;


	// �I�[�f�B�I�f�[�^���̊J��
	free(m_apSizeAudio);
	m_apSizeAudio = NULL;

	// �}�X�^�[�{�C�X�̔j��
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;

	// XAudio2�I�u�W�F�N�g�̊J��
	DIRECT_RELEASE(m_pXAudio2)

	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
//    �Z�O�����g�Đ�(��~)
//=============================================================================
HRESULT CSound::PlaySound(int nIdx, bool bPause)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	if(m_apDataAudio != NULL && m_apSourceVoice != NULL && m_apSizeAudio != NULL && m_apSoundParam != NULL)
	{// �T�E���h�f�[�^����������Ă���
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_apSizeAudio[nIdx];
		buffer.pAudioData = m_apDataAudio[nIdx];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = m_apSoundParam[nIdx].nCntLoop;

		// ��Ԏ擾
		m_apSourceVoice[nIdx]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// �Đ���
		    // �ꎞ��~
			m_apSourceVoice[nIdx]->Stop(0);

			if (bPause == false)
			{// �ꎞ��~���Ȃ��Ȃ�
			    // �I�[�f�B�I�o�b�t�@�̍폜
				m_apSourceVoice[nIdx]->FlushSourceBuffers();
			}
		}

		// �I�[�f�B�I�o�b�t�@�̓o�^
		m_apSourceVoice[nIdx]->SubmitSourceBuffer(&buffer);

		// �Đ�
		m_apSourceVoice[nIdx]->Start(0);
	}

	return S_OK;
}

//=============================================================================
//    �Z�O�����g��~
//=============================================================================
void CSound::StopSound(int nIdx, bool bPause)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	if(m_apSourceVoice[nIdx] != NULL)
	{// �{�C�X�f�[�^����������Ă���
		m_apSourceVoice[nIdx]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// �Đ���
		    // �ꎞ��~
			m_apSourceVoice[nIdx]->Stop(0);

			if (bPause == false)
			{// �ꎞ��~���Ȃ��Ȃ�
			 // �I�[�f�B�I�o�b�t�@�̍폜
				m_apSourceVoice[nIdx]->FlushSourceBuffers();
			}
		}
	}
}

//=============================================================================
//    �Z�O�����g��~
//=============================================================================
void CSound::StopSound(void)
{
	// �ꎞ��~
	for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
	{// �T�E���h�f�[�^�̐������J��Ԃ�
		if (m_apSourceVoice[nCntSound] != NULL)
		{// ���������m�ۂ���Ă���
			// �ꎞ��~
			m_apSourceVoice[nCntSound]->Stop(0);

			// �I�[�f�B�I�o�b�t�@�̍폜
			m_apSourceVoice[nCntSound]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
//    �{�����[���̐ݒ�
//=============================================================================
void CSound::SetVoluem(int nIdx, float fValue)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	if (m_apSourceVoice[nIdx] != NULL)
	{// �{�C�X�f�[�^����������Ă���
		m_apSourceVoice[nIdx]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// �Đ���
			m_apSourceVoice[nIdx]->SetVolume(fValue);
		}
	}
}

//=============================================================================
//    �T�E���h�f�[�^�̃|�C���^���m�ۂ���
//=============================================================================
HRESULT CSound::CreatePointer(void)
{
	// �\�[�X�{�C�X�ւ̃|�C���^
	if (m_apSourceVoice == NULL)
	{// ���������m�ۂł����Ԃł���
		m_apSourceVoice = new IXAudio2SourceVoice*[m_nNumSound];
		if (m_apSourceVoice != NULL)
		{// ���������m�ۂł���
			for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
			{// �T�E���h�f�[�^�̐������J��Ԃ�
				m_apSourceVoice[nCntSound] = NULL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	// �I�[�f�B�I�f�[�^�ւ̃|�C���^
	if (m_apDataAudio == NULL)
	{// ���������m�ۂł����Ԃł���
		m_apDataAudio = new BYTE*[m_nNumSound];
		if (m_apDataAudio != NULL)
		{// ���������m�ۂł���
			for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
			{// �T�E���h�f�[�^�̐������J��Ԃ�
				m_apDataAudio[nCntSound] = NULL;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	// �I�[�f�B�I�f�[�^�T�C�Y
	if (m_apSizeAudio == NULL)
	{// ���������m�ۂł����Ԃł���
		m_apSizeAudio = new DWORD[m_nNumSound];
		if (m_apSizeAudio != NULL)
		{// ���������m�ۂł���
			for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
			{// �T�E���h�f�[�^�̐������J��Ԃ�
				m_apSizeAudio[nCntSound] = 0x00;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	// ���f�ނ̃p�����[�^
	if (m_apSoundParam == NULL)
	{// ���������m�ۂł����Ԃł���
		m_apSoundParam = new SOUNDPARAM[m_nNumSound];
		if (m_apSoundParam != NULL)
		{// ���������m�ۂł���
			for (int nCntSound = 0; nCntSound < m_nNumSound; nCntSound++)
			{// �T�E���h�f�[�^�̐������J��Ԃ�
				m_apSoundParam[nCntSound].pFilename = new char[256];
				strcpy(m_apSoundParam[nCntSound].pFilename,"\0");
				m_apSoundParam[nCntSound].nCntLoop = 0;
			}
		}
		else
		{// ���������m�ۂł��Ȃ�����
			return E_FAIL;
		}
	}
	else
	{// ���������m�ۂł����ԂɂȂ�
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================================
//    �`�����N�̃`�F�b�N
//=============================================================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
//    �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}