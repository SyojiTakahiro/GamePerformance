//*****************************************************************************
//
//     ���ʂŎg���֐��̏���[functionlib.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _FUNKTIONLIB_H_
#define _FUNKTIONLIB_H_

//*****************************************************************************
//     �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************
// ���ʂŎg�������̒�`
#define STRING_TAB       "\t"                     // Tab�̕�����
#define STRING_ENTER     "\n"                     // ���s�R�[�h
#define STRING_SPACE     " "                      // �󔒂̕�����
#define STRING_COMMENT   "#"                      // �R�����g���J�n����镶��
#define SCRIPT           "SCRIPT"                 // �X�N���v�g�ǂݍ��݊J�n�̍��}
#define END_SCRIPT       "END_SCRIPT"             // �X�N���v�g�ǂݍ��ݏI���̍��}

//*****************************************************************************
//     ���ʊ֐��N���X�̒�`
//*****************************************************************************
class CFunctionLib
{
public:    // �N�ł��A�N�Z�X�\
	CFunctionLib();
	~CFunctionLib();

	static char *ReadLine(FILE *pFile, char *pDest);
	static int Memcmp(char *pSrc, char *pComp);
	static char *GetLineTop(char *pSrc);
	static int PopString(char *pSrc, char *pDest);
	static char *HeadPutout(char *pDest, char *pHead);
	static int ReadInt(char *pSrc, char *pHead);
	static float ReadFloat(char *pSrc, char *pHead);
	static char *ReadString(char *pSrc, char *pDest, char *pHead);
	static bool ReadBool(char *pSrc, char *pHead);
	static D3DXVECTOR3 ReadVector3(char *pSrc, char *pHead);
	static D3DXCOLOR ReadVector4(char *pSrc, char *pHead);
	static float RotationVectorX(float fRot, float X, float Y);
	static float RotationVectorY(float fRot, float X, float Y);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
};

#endif