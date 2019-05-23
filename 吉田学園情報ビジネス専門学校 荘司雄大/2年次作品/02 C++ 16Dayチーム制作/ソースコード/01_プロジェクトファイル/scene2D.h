//*****************************************************************************
//
//     �V�[��(2D)�̏���[scene2D.h]
//     Auther:Hodaka Niwa
//
//*****************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
//    �C���N���[�h�t�@�C��
//*****************************************************************************
#include "main.h"
#include "scene.h"

//*****************************************************************************
//    �V�[��2D�N���X�̒�`
//*****************************************************************************
class CScene2D : public CScene
{
public:     // �N�ł��A�N�Z�X�\
	//------------------------
	//  �e�N�X�`���̎��
	//------------------------
	typedef enum
	{
		TEXTURE_TYPE_0 = 0,
		TEXTURE_TYPE_MAX
	}TEXTURE_TYPE;

	CScene2D(int nPriority = 3, OBJTYPE objType = OBJTYPE_2DPOLYGON);
	~CScene2D();

	static CScene2D *Create(TEXTURE_TYPE type, int nPriority = 3);
	static HRESULT Load(void);
	static void UnLoad(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	virtual void MakeVertex(const LPDIRECT3DDEVICE9 pDevice);

	void BindTexture(const LPDIRECT3DTEXTURE9 pTexture);
	void SetPos(const D3DXVECTOR3 pos);
	void SetCol(const D3DXCOLOR col);
	void SetVtxBuff(const LPDIRECT3DVERTEXBUFFER9 pVtxBuff);

	D3DXVECTOR3 GetPos(void);
	D3DXCOLOR GetCol(void);
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void);

protected: // ���̃N���X�Ɣh���N���X�������A�N�Z�X�\

private:   // ���̃N���X�������A�N�Z�X�\
	static LPDIRECT3DTEXTURE9 m_apTexture[TEXTURE_TYPE_MAX];   // �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;   // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_pTexture;	  // �e�N�X�`���ւ̃|�C���^
	D3DXVECTOR3				m_pos;	      // �|���S���̈ʒu
	D3DXCOLOR               m_Col;        // �|���S���̐F
};

#endif