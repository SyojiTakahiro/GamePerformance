//*****************************************************************************
//
//     �e�̏���[shadow.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene3D.h"

//*****************************************************************************
//    X�t�@�C�����f���N���X�̒�`
//*****************************************************************************
class CShadow : public CScene3D
{
public:    // �N�ł��A�N�Z�X�\
	CShadow(int nPriority = 3, OBJTYPE objType = OBJTYPE_SHADOW);
	~CShadow();
	static CShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius, int nPriority);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fRadius);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void SetRadius(const float fRadius);

	float GetRadius(void);
	D3DXVECTOR3 GetPosOld(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9 m_apTexture;   // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3 m_PosOld;    // �O��̍��W
	float       m_fRadius;   // �e�̔��a
};

#endif