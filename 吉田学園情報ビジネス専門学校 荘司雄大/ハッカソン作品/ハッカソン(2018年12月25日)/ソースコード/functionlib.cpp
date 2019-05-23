//*****************************************************************************
//
//     ���ʂŎg���֐��̏���[functionlib.cpp]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#include "functionlib.h"

//*****************************************************************************
//     �}�N����`
//*****************************************************************************

//=============================================================================
//    �R���X�g���N�^
//=============================================================================
CFunctionLib::CFunctionLib()
{

}

//=============================================================================
//    �f�X�g���N�^
//=============================================================================
CFunctionLib::~CFunctionLib()
{

}

//=============================================================================
//    �e�L�X�g�����񕪕������ǂݐi�߂鏈��
//=============================================================================
char *CFunctionLib::ReadLine(FILE *pFile, char *pDest)
{
	// �������ǂݐi�߂�
	fgets(pDest, 256, pFile);
	while (1)
	{// �L���ȍs��������܂Ń��[�v
		if (*pDest == *STRING_COMMENT)
		{// �擪�̕������L���ȕ�����ł͂Ȃ�
			fgets(pDest, 256, pFile);
		}
		else if (*pDest == *STRING_ENTER)
		{// �擪�̕��������s�R�[�h��������
			fgets(pDest, 256, pFile);
		}
		else if (*pDest == *STRING_SPACE)
		{// �擪�̕������󔒍s��������
			pDest = GetLineTop(pDest);
		}
		else if (*pDest == *STRING_TAB)
		{// �擪�̕�����Tab��������
			pDest = GetLineTop(pDest);
		}
		else
		{// �L���ȕ����񂪔������ꂽ
			break;
		}
	}

	return pDest;  // ������̃A�h���X��Ԃ�
}

//=============================================================================
//    �w��̕����񂪓���������ł��邩��r���鏈��
//=============================================================================
int CFunctionLib::Memcmp(char *pSrc, char *pComp)
{
	return memcmp(pSrc, pComp, strlen(pComp));  // 0�Ȃ瓯��������ł���Ɣ���
}

//=============================================================================
//    tab��X�y�[�X�������������ĕ������擪�Ɏ����Ă�������
//=============================================================================
char *CFunctionLib::GetLineTop(char *pSrc)
{
	while (1)
	{// �L���ȕ������o�Ă���܂ŌJ��Ԃ�
		if (*pSrc == *STRING_SPACE)
		{// �擪�̕������󔒍s��������
			pSrc++;
		}
		else if (*pSrc == *STRING_TAB)
		{// �擪�̕�����Tab��������
			pSrc += strlen(STRING_TAB);
		}
		else
		{// �Ⴄ�����񂪏o�Ă���
			break;
		}
	}

	return pSrc;
}

//=============================================================================
//    �ǂݎ���������╶�����ǂꂭ�炢���̂���Ԃ�����
//=============================================================================
int CFunctionLib::PopString(char *pSrc, char *pDest)
{
	int nCntWord = 0;   // �ǂݎ��Ɏז��ȕ�����
	char aStr[256];     // ������ۑ��p

	strcpy(aStr, pSrc); // �������ۑ����Ă���
	while (1)
	{// �����񂪐擪�ɗ���悤�ɕԂ�
		nCntWord++;   // �����������Z����
		pSrc++;
		if (*pSrc == *STRING_SPACE || *pSrc == *STRING_TAB || *pSrc == *STRING_ENTER)
		{// �󔒂��^�u���擪�ɂ���
			nCntWord++;                      // SPACE�̕����l�����ĕ��������Z
			break;
		}
	}
	return nCntWord;
}

//=============================================================================
//    �擪�̕����������ē��o�������鏈��
//=============================================================================
char *CFunctionLib::HeadPutout(char *pDest, char *pHead)
{
	pDest = GetLineTop(pDest);  // �����ȕ���������
	pDest += strlen(pHead);     // �f�[�^�������ɕs�v�ȕ����������
	return pDest;
}

//=============================================================================
//    ������𐮐��^�ɕϊ����ĕԂ�����
//=============================================================================
int CFunctionLib::ReadInt(char *pSrc, char *pHead)
{
	int nData = 0;
	pSrc = HeadPutout(pSrc, pHead);
	nData = atoi(pSrc);
	return nData;
}

//=============================================================================
//    ������𕂓������_�^�ɕϊ����ĕԂ�����
//=============================================================================
float CFunctionLib::ReadFloat(char *pSrc, char *pHead)
{
	float fData = 0;
	pSrc = HeadPutout(pSrc, pHead);
	fData = (float)atof(pSrc);
	return fData;
}

//=============================================================================
//    �������K�v�ȕ������������o���ĕԂ�����
//=============================================================================
char *CFunctionLib::ReadString(char *pSrc, char *pDest, char *pHead)
{
	int nWord = 0;
	// ������𔲂��o��
	pSrc = CFunctionLib::HeadPutout(pSrc, pHead);
	nWord = CFunctionLib::PopString(pSrc, pDest);
	strcpy(pDest, "\0");
	strcpy(pDest, pSrc);
	strcpy(&pDest[nWord - 1], "\0");

	return pDest;
}

//=============================================================================
//    �������Bool�^�ɕϊ����ĕԂ�����
//=============================================================================
bool CFunctionLib::ReadBool(char *pSrc, char *pHead)
{
	bool bBool = false;
	int nBool = CFunctionLib::ReadInt(pSrc, pHead);
	if (nBool == 1)
	{// ���[�v����ԍ�������
		bBool = true;
	}
	else
	{// ���[�v���Ȃ��ԍ�������
		bBool = false;
	}

	return bBool;
}

//=============================================================================
//    ��������R�����x�N�g���^�ɕϊ����ĕԂ�����
//=============================================================================
D3DXVECTOR3 CFunctionLib::ReadVector3(char *pSrc, char *pHead)
{
	int nWord = 0;
	char aStr[256];
	D3DXVECTOR3 Vector = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// X
	pSrc = CFunctionLib::HeadPutout(pSrc, pHead);
	Vector.x = (float)atof(pSrc);

	// Y
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Vector.y = (float)atof(pSrc);

	// Z
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Vector.z = (float)atof(pSrc);

	return Vector;
}

//=============================================================================
//    ��������S�����x�N�g���^�ɕϊ����ĕԂ�����
//=============================================================================
D3DXCOLOR CFunctionLib::ReadVector4(char *pSrc, char *pHead)
{
	int nWord = 0;
	char aStr[256];

	D3DXCOLOR Color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	// r
	pSrc = CFunctionLib::HeadPutout(pSrc, pHead);
	Color.r = (float)atof(pSrc);

	// g
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Color.g = (float)atof(pSrc);

	// b
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Color.b = (float)atof(pSrc);

	// a
	nWord = PopString(pSrc, aStr);
	pSrc += nWord;
	Color.a = (float)atof(pSrc);

	return Color;
}

//=============================================================================
//    �x�N�g������]�����鏈��(X����)
//=============================================================================
float CFunctionLib::RotationVectorX(float fRot, float X, float Y)
{
	return cosf(fRot) * X - sinf(fRot) * Y;
}

//=============================================================================
//    �x�N�g������]�����鏈��(Y����)
//=============================================================================
float CFunctionLib::RotationVectorY(float fRot, float X, float Y)
{
	return sinf(fRot) * X + cosf(fRot) * Y;
}