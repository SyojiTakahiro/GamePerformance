//*****************************************************************************
//
//     ���̏���[water.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _WATER_H_
#define _WATER_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene3DMesh.h"

//*****************************************************************************
//    �}�N����`
//*****************************************************************************
#define MAX_WATER_TEX  (2)   // �ǂݍ��ސ��̃e�N�X�`���̐�

//*****************************************************************************
//    ���N���X�̒�`
//*****************************************************************************
class CWater : public CScene3DMesh
{
public:    // �N�ł��A�N�Z�X�\
	typedef enum
	{
		STATE_NONE = 0,    // �������Ă��Ȃ����
		STATE_RIPPLE,      // �g����N�������
		STATE_MAX
	}STATE;

	CWater(int nPriority = 3, OBJTYPE objType = OBJTYPE_WATER);
	~CWater();
	static CWater *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fDepth, int nXBlock, int nZBlock, char *aVertexFileName, int nTexIdx, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fDepth, int nXBlock, int nZBlock, char *aVertexFileName, int nTexIdx);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);
	void MakeNormal(void);
	void MakeIndex(const LPDIRECT3DDEVICE9 pDevice);

	float GetRipplesHeight(void);
	float GetPolyHeight(const D3DXVECTOR3 pos, D3DXVECTOR3 *pPolygonNormal);

	void SetState(const STATE state);
	void SetRipplePos(const D3DXVECTOR3 RipplePos);
	void SetRipplesHeight(const float fRipplesHeight);
	void SetRipples(D3DXVECTOR3 pos, float fRange, float fHeight);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static int                m_nNumAll;
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_WATER_TEX];  // �e�N�X�`���ւ̃|�C���^
	STATE       m_State;                 // ���
	D3DXVECTOR3 m_RipplePos;             // �g����N�������W
	float       m_fWidth;                // 1�u���b�N���̕�
	float       m_fDepth;                // 1�u���b�N���̉��s
	int         m_nXBlock;               // ���̕�����
	int         m_nZBlock;               // ���s�̕�����
	char        m_aVertexFileName[256];  // ���_���ւ̃X�N���v�g�t�@�C����
	float       m_fTexU;                 // �e�N�X�`��U���W
	float       m_fTexV;                 // �e�N�X�`��V���W
	float       m_fRipples;              // �g��Ɏg�p����{��
	float       m_fRipplesHeight;        // �g����ǂꂭ�炢�������邩
};

#endif